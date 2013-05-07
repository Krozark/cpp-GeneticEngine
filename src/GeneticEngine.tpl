#include <utility>

#include <thread>
#include <chrono>

#include <iostream>


template <class T>
template <typename ... Args>
GeneticEngine<T>::GeneticEngine(int nb_threads,float taux_mut,int tranche_mut,std::string filename,int pop_size,Args& ... args) : size(nb_threads)
{
    if (size <= 0)
        size = std::thread::hardware_concurrency()-1;

    islands = new  GeneticThread<T>*[size];
    for(int i=0;i<size;++i)
        islands[i] = new GeneticThread<T>(taux_mut,tranche_mut,filename,pop_size/nb_threads,std::forward<Args>(args)...);


};

template <class T>
template <typename ... Args>
T* GeneticEngine<T>::run(const int nb_generation,const int size_enf,Args& ... args)
{
    T* (GeneticThread<T>::*ptm)(bool (*)(T const&, Args&...), int, Args&...) = &GeneticThread<T>::run;
    for(int i=0;i<size;++i)
        islands[i]->thread= std::thread(ptm,islands[i],nb_generation,size_enf,args ...);
    wait();
    return end();
};

template <class T>
template <typename ... Args>
T* GeneticEngine<T>::run_while(bool (*f)(const T&),const int size_enf,Args& ... args)
{
    for(int i=0;i<size;++i)
        islands[i]->run_while(f,size_enf/size,args ...);
    wait();
    return end();
};



template<class T>
void GeneticEngine<T>::wait()
{
    running = true;
    //sender thread
    void (GeneticEngine<T>::*func)() = &GeneticEngine<T>::send;
    std::thread thread(func,this);
    do
    { 
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        running = true;
        for(int i=0;i<size and running ;++i)
            running = islands[i]->thread.joinable() and islands[i]->running;
    }
    while(running);
    //stop all islands
    stop();
    //wait the end of the sender thread
    thread.join();
};


template<class T>
void GeneticEngine<T>::stop()
{
    running = false;
    for(int i=0;i<size;++i)
        islands[i]->stop();
};

template<class T>
void GeneticEngine<T>::send()
{
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    while(running)
    {
        if (size > 1)
        {
            GeneticThread<T>* dest = islands[random(0,size-1)];
            GeneticThread<T>* src_pt;

            do
            {
                src_pt = islands[random(0,size-1)];
            }
            while(src_pt == dest);


            GeneticThread<T>& src = *src_pt;

            src.mutex.lock();
            send(src.get_best()->clone(),*dest);
            src.mutex.unlock();
        }
        //wait a moment for the other send
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
};

template<class T>
void GeneticEngine<T>::send(T* id,GeneticThread<T>& dest)
{
    dest.mutex.lock();
    delete dest.individus[dest.size-1];
    dest.individus[dest.size-1] = id;
    dest.mutex.unlock();
};


template<class T>
T* GeneticEngine<T>::end()
{
    T* best = islands[0]->get_best();
    for(int i=1;i<size;++i)
    {
        T* __best = islands[i]->get_best();
        if(__best > best)
            std::swap(best,__best);
        delete __best;
    }
    return best;
};
