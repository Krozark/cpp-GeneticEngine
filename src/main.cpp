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
        cout<<six_hump(v)<<endl;
    }

    {
        std::vector<double> v = {420.9687,420.9687};
        cout<<schwefel(v)<<endl;
    }


    /*
    int pop_size = 10000;
    float mutation_taux = 0.05;
    int pop_child = pop_size*0.75;

    int nb_threads = 1;

    GeneticEngine<Individu> engine(nb_threads,mutation_taux,"filename",pop_size,pop_child);
    bool (*stop)(const Individu&) = [](const Individu& best)
    {
        return false;
    };

    engine.setCreationMode(GeneticEngine<Individu>::CreationMode::TOURNAMENT);
    engine.setReductionMode(GeneticEngine<Individu>::ReductionMode::TOURNAMENT);
    engine.setEvaluateAll(true);
    //Individu* best = engine.run_while(stop,pop_child);
    Individu* best = engine.run(200);
    delete best;
    */
    return 0;
};
