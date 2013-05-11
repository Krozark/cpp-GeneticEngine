#include "Individu.hpp"
#include "GeneticEngine.hpp"

#include "random.hpp"

#include "benchmarks-func.hpp"

using namespace std;


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
    int pop_child = 100;

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
    engine.setEvaluateAll(true);


    Individu<2>* best = engine.run_while(stop);
    //Individu<2>* best = engine.run(20);
    */

    Individu<1>::benchmarks = schwefel;
    Individu<1>::bornes[0][Individu<1>::MIN] =-500;
    Individu<1>::bornes[0][Individu<1>::MAX] = 500;

    GeneticEngine<Individu<1> > engine(nb_threads,mutation_taux,"filename",pop_size,pop_child);
    engine.setTimeout(5000);
    bool (*stop)(const Individu<1>&) = [](const Individu<1>& best)
    {
        //auto coef = best.getCoef();
        //return (coef[0] == 0.0898 and coef[1] == -0.7126);
        return false;
    };

    engine.setCreationMode(GeneticEngine<Individu<1> >::CreationMode::TOURNAMENT);
    //engine.setReductionMode(GeneticEngine<Individu<1> >::ReductionMode::TOURNAMENT);
    engine.setEvaluateAll(true);


    Individu<1>* best = engine.run_while(stop);
    //Individu<1>* best = engine.run(20);

    delete best;

    return 0;
};
