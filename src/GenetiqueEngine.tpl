/* return *x>*y */
template <typename T>
struct gt_ptr : std::binary_function<T,T,bool>
{
    bool operator() (const T* x, const T* y) const {return *x>*y;}
};

template <typename T>
template <typename ... Args>
GenetiqueEngine<T>::GenetiqueEngine(float taux_mut,int tranche_mut,std::string filename,int pop_size,Args& ... args) : size(pop_size), mutation_taux(1-taux_mut), mutation_tranche(tranche_mut) , generation(0), prefix(filename)
{
    individus = new T*[pop_size];
    for(int i=0;i<pop_size;++i)
    {
        individus[i] = new T(std::forward<Args>(args) ...);
    }
};

template <typename T>
GenetiqueEngine<T>::~GenetiqueEngine()
{
    for(int i=0;i<size;++i)
        if(individus[i])
            delete individus[i];
    delete individus;
};

template <typename T>
template <typename ... Args>
T* GenetiqueEngine<T>::run(const int nb_generation,const int size_enf,Args& ... args)
{
    //eval initiale
    init(args ...);
    //boucle de génération
    for(int generation=0;generation<nb_generation;++generation)
        corps(size_enf,args ...);
    return end();
};

template <typename T>
template <typename ... Args>
T* GenetiqueEngine<T>::run_while(bool (*f)(const T&,Args& ...),const int size_enf,Args& ... args)
{
    //eval initiale
    init(args ...);
    do
    {
        corps(size_enf,args ...);
    }while (not f(*individus[0],args...));

    return end();
};

template <typename T>
template <typename ... Args>
void GenetiqueEngine<T>::init(Args& ... args)
{
    for(int i=0;i<size;++i)
        individus[i]->eval(args...);
};

template <typename T>
template <typename ... Args>
void GenetiqueEngine<T>::corps(const int size_enf,Args& ... args)
{
    std::partial_sort(individus,individus+(size-size_enf),individus+size,gt_ptr<T>());//en tri les size - size_enf
    //creation des enfants + evaluation
    T* enfants[size_enf];
    for(int i=0;i<size_enf;++i)//on prend que les meilleur, mais avec random
    {
        enfants[i] = individus[i]->crossOver(*individus[random(0,size-1)]);
        if (random(0.f,1.f) > mutation_taux)//Mutation !
        {
            enfants[i]->mutate();

            for (int j=100;j<enfants[i]->size();j+=100)
                enfants[i]->mutate();
        }

        enfants[i]->eval(args...);
    }
    //on mélange un peu histoir d'aider les mauvais en gardant un bout des bon (quand même)...
    const int borne = (size - size_enf)*0.85;
    std::shuffle(individus+borne, individus+size-size_enf, generator);

    //réduction population
    for(int i=0;i<size_enf;++i)
    {
        std::swap(individus[size-size_enf+i],enfants[i]);
        delete enfants[i];
    }

    //evite de trop stagner

    if(mutation_taux > 0)
        for(int i=0;i<size;i+=mutation_tranche)
        {
            int j = random(1,size-1);
            individus[j]->mutate();
            individus[j]->eval(args ...);
        }

    std::cout<<"generation #"<<generation++<<std::endl;
    #if GENETIQUE_SAVE_RESULTS
        save(std::to_string(individus[0]->get_score()));
    #endif
};

template <typename T>
T* GenetiqueEngine<T>::end()
{
    std::partial_sort(individus,individus+1,individus+size,gt_ptr<T>());//en tri les size - size_enf
    //on renvoi le meilleur
    T* res = individus[0];
    save("last");
    individus[0] = 0;
    return res;
};

template <typename T>
void GenetiqueEngine<T>::save(const std::string& name)
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
            <<"\n*\tscore: "<<individus[0]->get_score()
            <<"\n*\tsize: "<<individus[0]->size()
            <<"\n*\tgeneration: "<<generation
            <<"\n*/"
            <<"\n\tbest ="<<*individus[0]
            <<"\n";
        file.close();
        std::cout<<format<<" best("<<individus[0]->get_score()<<"): "<<*individus[0]<<std::endl<<std::endl;

    }
};
