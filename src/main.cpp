#include "Individu.hpp"
#include "GeneticEngine.hpp"

#include "random.hpp"

#include "benchmarks-func.hpp"

#include <stdlib.h>
#include <stdio.h>
#define NUM_POINTS 5
#define NUM_COMMANDS 2

using namespace std;

     
#define GNUPLOT_PATH "/usr/bin/gnuplot"


int main(int argc,char * argv[])
{
    rand_init();
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
    bool (*stop)(const Individu<1>&) = [](const Individu<1>& best)
    {
        //auto coef = best.getCoef();
        //return (coef[0] == 0.0898 and coef[1] == -0.7126);
        return false;
    };

    //engine.setCreationMode(GeneticEngine<Individu<1> >::CreationMode::STUPIDE);
    //engine.setReductionMode(GeneticEngine<Individu<1> >::ReductionMode::STUPIDE);


    //Individu<1>* best = engine.run_while(stop);
    Individu<1>* best = engine.run(200);


    delete best;
    std::cout<<"plop"<<std::endl;


     
    FILE *gp;

    gp = popen(GNUPLOT_PATH, "w");
    if(gp == NULL){
        fprintf(stderr, "Oops, I can't find %s.", GNUPLOT_PATH);
         exit(EXIT_FAILURE);
    }
    fprintf(gp, "cd '~/Desktop'\n load \"config\"\n");
      fflush(gp); /* On oublie pas le buffer. */
      getchar();
      pclose(gp);
     

    
    
/*    double xvals[NUM_POINTS] = {1.0, 2.0, 3.0, 4.0, 5.0};
    double yvals[NUM_POINTS] = {5.0 ,3.0, 1.0, 3.0, 5.0};
    FILE * temp = fopen("data.temp", "w");
    FILE * gnuplotPipe = popen ("gnuplot -persistent", "w");

    fprintf(gnuplotPipe, "plot '-' \n");

    for (int i = 0; i < NUM_POINTS; i++)
    {
        fprintf(gnuplotPipe, "%lf %lf\n", xvals[i], yvals[i]);
    }
    fprintf(gnuplotPipe, "e");

    fflush(gnuplotPipe);
    */


    std::this_thread::sleep_for(std::chrono::milliseconds(10000));


    exit(0);
    return 0;
};
