cpp-GeneticEngine
=================

A classe that implement all the function to run a genetic algo



GeneticEngine<T>
==================

The engine that will create threads and distribute the population in each of them. It olso distribute the best T of a thread on a other (random) threads each sec.

T must have some function (see Individu.hpp):

Fonctions
---------

GeneticEngine<T>::GeneticEngine(int nb_threads,float taux_mut,std::string filename,int pop_size,int pop_child,Args& ... args);

* nb_threads is the nuber of GeneticThread<T> created
* taux_mut is the mutation rate
* filename is the filename where results are saves
* pop_size is the population initial size
* pop_child is the number of childrens to creat on each generation
* Args& ... is agrument for T constructor.

start it:
* void GeneticEngine<T>::run(const int nb_generation);
run for nb_generation

or
* void GeneticEngine<T>::run_while(bool (*f)(const T&));
run while f return true

stop it:
* GeneticEngine<T>::stop();

Get best:
* T* GeneticEngine<T>::get_best()const;
You must delete the result of get_best() when call after run/run_while.


Comportement:
* enum class CreationMode {STUPIDE=0,TOURNAMENT};
* void GeneticEngine<T>::setCreationMode(CreationMode val);
set the fonction to use for the creation.

* enum class ReductionMode {STUPIDE=0,TOURNAMENT};
* void GeneticEngine<T>::setReductionMode(ReductionMode val);
set the fonction to use for the reduction.

* void GeneticEngine<T>::setEvaluateAll(bool v);
if set to true, each T will be evaluate when created, else just when needed. Set it to false when using Stupide Creation/Reduction will have a undefined comportement.



GeneticThread<T>
================

This class don't have to be use directly. GeneticEngine<T> use it internaly.

A generic implementation of a genetic algorithme.

GeneticThread(float taux_mut,std::string filename,int pop_size,int pop_child,Args& ... args);
* nb_threads is the nuber of GeneticThread<T> created
* taux_mut is the mutation rate
* filename is the filename where results are saves
* pop_size is the population initial size
* pop_child is the number of childrens to creat on each generation
* Args& ... is agrument for T constructor.


Fonctions:
---------

start it:
* void GeneticThread<T>::run(const int nb_generation);
run for nb_generation

or

* void GeneticThread<T>::run_while(bool (*f)(const T&));
run while f return true

stop it:
* GeneticThread<T>::stop();

Get best:
* T* GeneticThread<T>::get_best()const;
You must delete the result of get_best() when call after run/run_while.



Individu.hpp
============

Exemple of individu with fonction to implement

fonctions nedded
----------------

* void T.mutate()
* T* crossOver(T& other)const
* T* clone()
* int get_score()
* int size()
* void eval() 
* bool need_eval();
* bool operator>(const T& other)
* friend std::ostream& operator<<(std::ostream& output,const T& self)



random.hpp
==========

Just to use random generator

* float random(float min,float max)
* int random(int min, int max)
* int random(int max)

Exemple
=======

See main.cpp

