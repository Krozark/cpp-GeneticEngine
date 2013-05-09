#ifndef GENETICENGINE_HPP
#define GENETICENGINE_HPP

#include "GeneticThread.hpp"

/* T is Individu Class  */
template <class T>
class GeneticEngine
{
    public:
        template <typename ... Args>
        GeneticEngine(int nb_threads,float taux_mut,std::string filename,int pop_size,int pop_child,Args& ... args);

        ~GeneticEngine();

        //template <typename ... Args>
        T* run(const int nb_generation/*,Args& ... args*/);

        //template <typename ... Args>
        T* run_while(bool (*f)(const T&)/*,Args& ... args*/);

        void stop();
        
    private:
        GeneticThread<T>** islands;
        int size;
        volatile bool running;

        void send();
        void send(T* id,GeneticThread<T>& dest);


        void wait();


        T* end();
};

#include "GeneticEngine.tpl"
#endif
