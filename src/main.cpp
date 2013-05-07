#include "Individu.hpp"
#include "GeneticEngine.hpp"

#include "random.hpp"

using namespace std;

int main(int argc,char * argv[])
{
    rand_init();

    int pop_size = 1000;
    float mutation_taux = 0.05;
    int mutation_tranche = 100;
    int pop_child = pop_size*0.75;

    int nb_threads = 2;

    GeneticEngine<Individu> engine(nb_threads,mutation_taux,mutation_tranche,"filename",pop_size/*args to Individu constructor*/);
    bool (*stop)(const Individu&) = [](const Individu& best)
    {
        return false;
    };
    Individu* best = engine.run_while(stop,pop_child);
    delete best;
    return 0;
}
