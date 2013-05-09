extern std::default_random_engine generator;


#include <iostream>
#include <utility>

/* return *x>*y */
template <typename T>
struct gt_ptr : std::binary_function<T,T,bool>
{
    bool operator() (const T* x, const T* y) const {return *x>*y;}
};

template <typename T>
template <typename ... Args>
GeneticThread<T>::GeneticThread(float taux_mut,std::string filename,int pop_size,int pop_child, Args& ... args) : size(pop_size), size_child(pop_child), mutation_taux(taux_mut), generation(0), prefix(filename), running(false)
{
    mutex.lock();
    individus = new T*[pop_size+pop_child];
    for(int i=0;i<pop_size;++i)
    {
        individus[i] = new T(std::forward<Args>(args) ...);
    }
    pop_cursor = pop_size;

    /***** init fonction to use *********/
    setCreationMode(CreationMode::STUPIDE);
    setReductionMode(ReductionMode::STUPIDE);

    mutex.unlock();
};

template <typename T>
GeneticThread<T>::~GeneticThread()
{
    for(int i=0;i<size;++i)
        if(individus[i] and individus[i] != best)
        {
            delete individus[i];
            individus[i] = 0;
        }
    delete [] individus;
};

template <typename T>
void GeneticThread<T>::run(const int nb_generation)
{
    //will be execute in thread
    auto lambda = [&](int nb_generation)
    {
        //eval initiale
        this->init();
        //boucle de génération
        for(int generation=0;generation<nb_generation and this->running ;++generation)
            this->corps();
        this->end();
    };

    //start thread
    thread = std::thread(lambda,nb_generation);
};

template <typename T>
void GeneticThread<T>::run_while(bool (*f)(const T&))
{

    //will be execute in thread
    auto lambda = [&](bool (*f)(const T&))
    {
        //eval initiale
        this->init();
        do
        {
            this->corps();
        }while ((not f(*this->best)) and this->running);
        this->end();
    };

    //start thread
    thread = std::thread(lambda,f);
};

template <typename T>
void GeneticThread<T>::init()
{
    mutex.lock();
    if(initial_evaluation_req)
    {
        for(int i=0;i<size;++i)
        {
            individus[i]->eval();
            if(i == 0)
                best = individus[i];
            else if (individus[i] > best)
                best = individus[i];
        }
    }
    running = true;
    mutex.unlock();
};

template <typename T>
void GeneticThread<T>::corps()
{
    mutex.lock();
    
    // creat children
    (this->*creatChildFunc)();

    //reduce pop
    (this->*reducePopFunc)();

    mutex.unlock();

    std::cout<<"["<<thread.get_id()<<"] generation #"<<generation++<<std::endl;
    #if GENETIQUE_SAVE_RESULTS
        save(std::to_string(best->get_score()));
    #endif
};


template<class T>
T* GeneticThread<T>::makeNew(const T* parent1,const T& parent2)
{
    T* res = parent1->crossOver(parent2);
    if (random(0.f,1.f) < mutation_taux)//Mutation !
    {
        res->mutate();

        for (int j=100;j<res->size();j+=100) //mutation proportianal to his size
            res->mutate();
    }

    return res;
};

template <typename T>
void GeneticThread<T>::end()
{
    mutex.lock();

    std::partial_sort(individus,individus+1,individus+size,gt_ptr<T>());//en tri les size - size_child
    best = individus[0];

    save("last");
    mutex.unlock();
    running = false;
    exit(0);
};

template <typename T>
void GeneticThread<T>::save(const std::string& name)
{
    std::string filename("best/"+prefix+"_"+name+".res");
    if(not std::ifstream(filename))
    {
        time_t temps;
        struct tm* datetime;
        char  format[32];
        time(&temps);
        datetime = localtime(&temps);
        strftime(format, 32, "%d-%m-%Y %H:%M", datetime);

        std::ofstream file;
        file.open(filename);
        file<<"/*"
            <<"\n*\tdate: "<<format
            <<"\n*\tscore: "<<best->get_score()
            <<"\n*\tsize: "<<best->size()
            <<"\n*\tgeneration: "<<generation
            <<"\n*/"
            <<"\n\tbest ="<<*best
            <<"\n";
        file.close();
        std::cout<<"["<<thread.get_id()<<"] "<<format<<" best("<<best->get_score()<<"): "<<*best<<std::endl<<std::endl;
    }
};

/******************** CUSTOM FONCTION **********************/
template<class T>
void GeneticThread<T>::setCreationMode(CreationMode val)
{
    switch(val)
    {
        case CreationMode::STUPIDE:
            creatChildFunc = &GeneticThread<T>::stupideCreation;
            initial_evaluation_req = true;
        break;

        case CreationMode::TOURNAMENT:
            creatChildFunc = &GeneticThread<T>::tournamentCreation;
        break;
        
    }
};

template<class T>
void GeneticThread<T>::setReductionMode(ReductionMode val)
{
    switch(val)
    {
        case ReductionMode::STUPIDE:
            reducePopFunc = &GeneticThread<T>::stupidReduction;
            initial_evaluation_req = true;
        break;

        case ReductionMode::TOURNAMENT:
            reducePopFunc = &GeneticThread<T>::tournamentReduction;
        break;
    }
};

//creation
template<class T>
void GeneticThread<T>::stupideCreation()
{
    //en tri les size - size_child
    std::partial_sort(individus,individus+size_child,individus+size,gt_ptr<T>());
    best = individus[0];
    //creation des enfants
    for(int i=0;i<size_child;++i)
    {
        int c = size+i;
        //on prend que les meilleurs, mais avec random
        individus[c] = makeNew(individus[i],*individus[random(0,size-1)]);
        if(initial_evaluation_req)
            individus[pop_cursor]->eval();
    }
};

template<class T>
void GeneticThread<T>::tournamentCreation()
{
    while(pop_cursor < size + size_child)
    {
        //random individus
        T* id_rand[7];
        for(int i=0;i<7;++i)
        {
            id_rand[i] = individus[random(0,size-1)];
            if(not initial_evaluation_req and id_rand[i]->need_eval())
                id_rand[i]->eval();

        }

        T* tmp_best[2];

        tmp_best[0] = (*id_rand[0]>*id_rand[1])?id_rand[0]:id_rand[1];
        tmp_best[1] = (*id_rand[2]>*id_rand[3])?id_rand[2]:id_rand[3];
        tmp_best[0] = (*tmp_best[0]>*tmp_best[1])?tmp_best[0]:tmp_best[1];

        tmp_best[1] = (*id_rand[4]>*id_rand[5])?id_rand[4]:id_rand[5];
        tmp_best[1] = (*id_rand[6]>*tmp_best[1])?id_rand[6]:tmp_best[1];
        //create child
        individus[pop_cursor] = tmp_best[1]->crossOver(*tmp_best[0]);
        if(initial_evaluation_req)
            individus[pop_cursor]->eval();

        //maj on best
        best = (*tmp_best[0]>*best)?tmp_best[0]:(*tmp_best[1]>*best)?tmp_best[1]:best;
        //nex
        ++pop_cursor;
    }
};


//reduction
template<class T>
void GeneticThread<T>::stupidReduction()
{
    for(int i=0;i<size_child;++i)
    {
        int c = size+i;
        std::swap(individus[size-size_child+i],individus[c]);
        delete individus[c];
        individus[c] = 0;
    }
};

template <class T>
void GeneticThread<T>::tournamentReduction()
{
    while(pop_cursor > size)
    {
        //random individus
        std::pair<int,T*> id_rand[7]; //i,Individu*
        for(int i=0;i<7;++i)
        {
            int c = random(0,pop_cursor);
            id_rand[i] = std::make_pair(c,individus[c]);
            if(not initial_evaluation_req and individus[c]->need_eval())
                individus[c]->eval();
        }

        std::pair<int,T*> worst[2];
        
        //calc the worst
        worst[0] = (*id_rand[0].second>*id_rand[1].second)?id_rand[1]:id_rand[0];
        worst[1] = (*id_rand[2].second>*id_rand[3].second)?id_rand[3]:id_rand[2];
        worst[0] = (*worst[0].second>*worst[1].second)?worst[1]:worst[2];

        worst[1] = (*id_rand[4].second>*id_rand[5].second)?id_rand[5]:id_rand[4];
        worst[1] = (*id_rand[6].second>*worst[1].second)?worst[1]:id_rand[6];

        worst[0] = (*worst[0].second>*worst[1].second)?worst[1]:worst[0];

        //delete worst
        delete worst[0].second;
        //move last at new free place
        individus[worst[0].first] = individus[pop_cursor];
        //next
        --pop_cursor;
    }
};

