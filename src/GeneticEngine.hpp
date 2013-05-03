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
        
    private:
        GeneticThread<T>* islands;
};

#include "GeneticEngine.tpl"
#endif
