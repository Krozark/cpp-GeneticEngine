#include "Individu.hpp"
#include "GeneticEngine.hpp"

#include "random.hpp"

using namespace std;

int main(int argc,char * argv[])
{
    rand_init();

    int pop_size = 100;
    float mutation_taux = 0.05;
    int pop_child = pop_size*0.25;

    int nb_threads = 1;

    GeneticEngine<Individu> engine(nb_threads,mutation_taux,"filename",pop_size,pop_child/*args to Individu constructor*/);
    bool (*stop)(const Individu&) = [](const Individu& best)
    {
        return false;
    };
    engine.setCreationMode(GeneticEngine<Individu>::CreationMode::TOURNAMENT);
    engine.setReductionMode(GeneticEngine<Individu>::ReductionMode::TOURNAMENT);
    //Individu* best = engine.run_while(stop,pop_child);
    Individu* best = engine.run(10);
    delete best;
    return 0;
};
