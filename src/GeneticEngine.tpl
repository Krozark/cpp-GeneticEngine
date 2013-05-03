#include <utility>
#include <thread>

template<class C,typename ... Args>
void thread_method(C* obj,void(C::*func)(Args& ...),Args& ... args)
{
    (obj->*func)(std::forward<Args>(args)...);
};

template <class T>
template <typename ... Args>
GeneticEngine<T>::GeneticEngine(int nb_threads,float taux_mut,int tranche_mut,std::string filename,int pop_size,Args& ... args) : size(nb_threads)
{
    islands = new  GeneticThread(taux_mut,tranche_mut,filename,pop_size/nb_threads,std::forward<Args>(args)...)[size];

};

template <class T>
template <typename ... Args>
T* GeneticEngine<T>::run(const int nb_generation,const int size_enf,Args& ... args);
{
    for(int i=0;i<size;++i)
        islands[i].thread= std::thread(thread_method<GeneticThread>,&islands[i],&GeneticThread::run);

    for(int i=0;i<size;++i)
        islands[i].thread.join();
};
