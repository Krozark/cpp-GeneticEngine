cpp-GeneticEngine
=================

A classe that implement all the function to run a genetic algo



GenetiqueEngine<T>
==================

The engine
T must have some function (see Individu.hpp):

* void T.mutate()
* T* crossOver(T& other)
* void eval() 
* int get_score()
* int size()
* bool operator>(const T& other)
* friend std::ostream& operator<<(std::ostream& output,const T& self)




Individu.hpp
============

Exemple of individu with fonction to implement


random.hpp
==========

Juste to use random generator


Exemple
=======

See main.cpp

