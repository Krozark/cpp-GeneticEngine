#include "src/random.hpp"

using namespace std;

int main(int argc,char * argv[])
{
    rand_init();
    /*
    int pop_size = 1000;
    float mutation = 0.05;
    int mutation_tranche = 100;
    int pop_child = pop_size*0.75;

    GenetiqueEngine<Individu> engine(mutation_taux,mutation_tranche,"filename",pop_size);
    bool (*stop)(const Individu&) = [](const Individu& best)
    {
        return false;
    };
    Individu* best = engine.run_while(stop,pop_size);
    
    delete best;
    */
    return 0;
}
