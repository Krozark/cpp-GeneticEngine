#include <utility>

#include <thread>
#include <chrono>

template<class C,typename ... Args>
void thread_method(C* obj,void(C::*func)(Args& ...),Args& ... args)
{
    (obj->*func)(std::forward<Args>(args)...);
};

template <class T>
template <typename ... Args>
GeneticEngine<T>::GeneticEngine(int nb_threads,float taux_mut,int tranche_mut,std::string filename,int pop_size,Args& ... args) : size(nb_threads)
{
    islands = new  GeneticThread<T>*[size];
    for(int i=0;i<size;++i)
        islands[i] = new GeneticThread<T>(taux_mut,tranche_mut,filename,pop_size/nb_threads,std::forward<Args>(args)...);

};

template <class T>
template <typename ... Args>
T* GeneticEngine<T>::run(const int nb_generation,const int size_enf,Args& ... args)
{
    for(int i=0;i<size;++i)
        islands[i]->thread= std::thread(thread_method<GeneticThread<T> >,islands[i],&GeneticThread<T>::run,nb_generation,size_enf,args ...);
    Wait();
    return 0;
};

template <class T>
template <typename ... Args>
T* GeneticEngine<T>::run_while(bool (*f)(const T&,Args& ...),const int size_enf,Args& ... args)
{
    return 0;
};


template<class T>
void GeneticEngine<T>::Wait()
{
    bool end=false;
    while(not end)
    { 
        end = false;
        for(int i=0;i<size and not end ;++i)
            end = islands[i]->thread.joinable();

        if (end)
            break;
         std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
};

template<class T>
void GeneticEngine<T>::send()
{

};

template<class T>
void GeneticEngine<T>::send(T* id,GeneticThread<T>& dest)
{
};
