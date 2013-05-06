#ifndef GENETICTHREAD_HPP
#define GENETICTHREAD_HPP

#include <utility>
#include <functional>
#include <algorithm>
#include <fstream>
//rondom
#include <ctime>
#include <random>
#include "random.hpp"
//multi threading
#include <mutex>
#include <thread>



#include <iostream>

//template <class T> class GeneticEngine;

/* T = class Individu */
template <class T>
class GeneticThread
{
    public:
    /* taux_mut = mutation rate [0 .. 1]
    *  tranche_mut = mutate a individue each pop_size/tranche_mut
    *  filename = where the output have to be save
    *  pop_size = size of the population
    *  Args ... = argument to T(Args& ... args ) constructor */
        template <typename ... Args>
        GeneticThread(float taux_mut,int tranche_mut,std::string filename,int pop_size,Args& ... args);

        ~GeneticThread();

        /*return the best (greater score)
        * you have to delete it yourself
        * run for nb_generation
        * Args& ... args = argument to T.eval(Args& ... args)
        */
        template <typename ... Args>
        T* run(const int nb_generation,const int size_enf,Args& ... args);
        
        /* return the best
        * you have to delete it youself
        * run until f return true
        * Args& .. args = argumebt to T.eval( ... args) AND f(T& best,Args& ... args)
        */
        template <typename ... Args>
        T* run_while(bool (*f)(const T&,Args& ...),const int size_enf,Args& ... args);
        


    private:
        //friend class GeneticEngine<T>;
        T** individus;
        const int size;
        const float mutation_taux;
        const int mutation_tranche;
        int generation;
        const std::string prefix;
        
        /* eval all the population */
        template <typename ... Args>
        void init(Args& ... args);

        std::mutex mutex;
        std::thread thread;

        /* sort the pop
        *  make children using T.crossOver(const T& other) + childre.mutate()
        *  remove worst and replace them with childrens
        *  eache.mutate() [mutation_tranche]
        */
        template <typename ... Args>
        void corps(const int size_enf,Args& ... args);

        /* At the end:
        *  save it in last.res file
        *  return the best
        */
        T* end();

        /* save current best in file using << operator */
        void save(const std::string& name);
};

#include "GeneticThread.tpl"
#endif
