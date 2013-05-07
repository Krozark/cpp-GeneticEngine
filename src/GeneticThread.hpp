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

template <class T> class GeneticEngine;

/* T = class Individu */
template <class T>
class GeneticThread
{
    public:
    /* taux_mut = mutation rate [0 .. 1]
    *  filename = where the output have to be save
    *  pop_size = size of the population
    *  size_child = number of child
    *  Args ... = argument to T(Args& ... args ) constructor */
        template <typename ... Args>
        GeneticThread(float taux_mut,std::string filename,int pop_size,int pop_child,Args& ... args);

        ~GeneticThread();

        /*return the best (greater score)
        * you have to delete it yourself
        * run for nb_generation
        * Args& ... args = argument to T.eval(Args& ... args)
        */
        template <typename ... Args>
        void run(const int nb_generation,Args& ... args);
        
        /* return the best
        * you have to delete it youself
        * run until f return true
        * Args& .. args = argumebt to T.eval( ... args) AND f(T& best,Args& ... args)
        */
        
        template <typename ... Args>
        void run_while(bool (*f)(const T&,Args& ... args),Args& ... args);

        T* get_best()const {return individus[0];};

        inline void stop(){running=false;}


    private:
        friend class GeneticEngine<T>;
        T** individus;
        const int size;
        const int size_child;
        int pop_cursor;
        const float mutation_taux;
        int generation;
        const std::string prefix;
        
        /* eval all the population */
        template <typename ... Args>
        void init(Args& ... args);

        std::mutex mutex;
        std::thread thread;
        volatile bool running;

        /* sort the pop
        *  make children using T.crossOver(const T& other) + childre.mutate()
        *  remove worst and replace them with childrens
        */
        template <typename ... Args>
        void corps(Args& ... args);

        /* create a new T using parent1 & 2
         * It use crossover, and mutate internatly
         */
        T* makeNew(const T* parent1,const T& parent2);

        /* At the end:
        *  save it in last.res file
        *  return the best
        */
        void end();

        /* save current best in file using << operator */
        void save(const std::string& name);
};

#include "GeneticThread.tpl"
#endif
