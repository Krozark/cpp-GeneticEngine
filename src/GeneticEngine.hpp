#ifndef GENETICENGINE_HPP
#define GENETICENGINE_HPP

#include "GeneticThread.hpp"

/* T is Individu Class  */
template <class T>
class GeneticEngine
{
    public:
        template <typename ... Args>
        GeneticEngine(int nb_threads,float taux_mut,int tranche_mut,std::string filename,int pop_size,Args& ... args);

        template <typename ... Args>
        T* run(const int nb_generation,const int size_enf,Args& ... args);

        template <typename ... Args>
        T* run_while(bool (*f)(const T&,Args& ...),const int size_enf,Args& ... args);

        void stop();
        
    private:
        GeneticThread<T>** islands;
        void send();
        void send(T* id,GeneticThread<T>& dest);
        int size;

        void wait();
};

#include "GeneticEngine.tpl"
#endif
