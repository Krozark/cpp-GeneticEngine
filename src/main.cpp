#include "Individu.hpp"
#include "GeneticEngine.hpp"

#include "random.hpp"
#include "benchmarks-func.hpp"
#include "gnuplot/src/gnuplot_i.h"

#include <stdlib.h>
#include <stdio.h>

using namespace std;

gnuplot_ctrl* h1;
std::vector<double> points[2];
int gnuplottimeout;

     
int main(int argc,char * argv[])
{
    rand_init();
    h1 = gnuplot_init();
    int gnuplottimeout = 50;

    /*int i=1;
    while(i<argc)
    {
        string arg = string(argv[i]);
        if (arg =="-pop-total")
        {
            if(++i <argc)
            {
                pop_size = atoi(argv[i]);
            }
            else
                SHOW_ARGS("Pas de population de précisée")
        }
    }*/
    
    

    cout.precision(10);
    {
        std::vector<double> v = {0.0898,-0.7126};
        cout<<"min six_hump: "<<six_hump(v)<<" ("<<v[0]<<","<<v[1]<<")"<<endl;
    }

    {
        std::vector<double> v = {420.9687};
        cout<<"min schwefel: "<<schwefel(v)<<" ("<<v[0]<<")"<<endl;
    }


    int pop_size = 1000;
    float mutation_taux = 0.01;
    int pop_child = 1000;

    int nb_threads = -1;

    /*
    Individu<2>::benchmarks = six_hump;
    Individu<2>::bornes[0][Individu<2>::MIN] =-3;
    Individu<2>::bornes[0][Individu<2>::MAX] = 3;

    Individu<2>::bornes[1][Individu<2>::MIN] =-2;
    Individu<2>::bornes[1][Individu<2>::MAX] = 2;

    GeneticEngine<Individu<2> > engine(nb_threads,mutation_taux,"filename",pop_size,pop_child);
    engine.setTimeout(5000);
    bool (*stop)(const Individu<2>&) = [](const Individu<2>& best)
    {
        //auto coef = best.getCoef();
        //return (coef[0] == 0.0898 and coef[1] == -0.7126);
        return false;
    };

    engine.setCreationMode(GeneticEngine<Individu<2> >::CreationMode::TOURNAMENT);
    engine.setReductionMode(GeneticEngine<Individu<2> >::ReductionMode::TOURNAMENT);

    Individu<2>* best = engine.run_while(stop);
    //Individu<2>* best = engine.run(20);
    */

    Individu<1>::benchmarks = schwefel;
    Individu<1>::bornes[0][Individu<1>::MIN] =-500;
    Individu<1>::bornes[0][Individu<1>::MAX] = 500;

    GeneticEngine<Individu<1> > engine(nb_threads,mutation_taux,"filename",pop_size,pop_child);
    engine.setTimeout(3000);
    engine.setEvaluateAll(false);
    bool (*stop)(const Individu<1>&,const int) = [](const Individu<1>& best,const int generation)
    {
        static volatile int i=0;
        points[0].emplace_back(generation);
        points[1].emplace_back(best.get_score());

        if(++i == 1)
        {
            gnuplot_resetplot(h1) ;
            gnuplot_setstyle(h1, "lines") ;
            gnuplot_plot_xy(h1, &points[0][0], &points[1][0], points[0].size(), "best individu");
        }
        else if (i >= 50)
            i = 0;
        //auto coef = best.getCoef();
        //return (coef[0] == 0.0898 and coef[1] == -0.7126);
        return false;
    };

    //engine.setCreationMode(GeneticEngine<Individu<1> >::CreationMode::STUPIDE);
    //engine.setReductionMode(GeneticEngine<Individu<1> >::ReductionMode::STUPIDE);


    Individu<1>* best = engine.run_while(stop);
    //Individu<1>* best = engine.run(200);
    delete best;

    std::this_thread::sleep_for(std::chrono::milliseconds(10000));

    exit(0);
    return 0;
};
