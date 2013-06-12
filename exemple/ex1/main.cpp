#include <src/random.hpp>
#include "Individu.hpp"
#include <src/GeneticEngine.hpp>

#include <string>

using namespace std;

typedef Individu<10,3> MyType;

int main(int argc,char * argv[])
{
    rand_init();
    int nb_threads = -1;
    float mutation_taux = 0.1;
    std::string filename = "255,3";
    int pop_size = 100;
    int pop_child = pop_size/2;

    GeneticEngine<MyType> engine(nb_threads,mutation_taux,filename,pop_size,pop_child);
    engine.setTimeout(1000);
    engine.setEvaluateAll(false);

    bool(*stop)(const MyType&, const int) = [](const MyType& best, const int generation)
    {
        return generation > 200;
    };
    
    engine.setCreationMode(GeneticEngine<MyType>::CreationMode::TOURNAMENT);
    engine.setReductionMode(GeneticEngine<MyType>::ReductionMode::TOURNAMENT);
    
    MyType* best = engine.run_while(stop);
    std::cout<<*best<<std::endl;
    delete best;

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
    exit(0);
    return 0;
}
