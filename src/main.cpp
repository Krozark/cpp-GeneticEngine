#include "Individu.hpp"
#include "GeneticEngine.hpp"

#include "random.hpp"
#include "benchmarks-func.hpp"
#include "gnuplot/src/gnuplot_i.h"

#include <stdlib.h>
#include <stdio.h>
#include <time.h>

using namespace std;

#define SHOW_ARGS(x) {cout<<x<<endl\
    <<"\t -h, -help, montre ce message"<<endl\
    <<"\t -pop-total (defaut = 1000) population"<<endl\
    <<"\t -pop-enf (defaut = 1000) population d'enfants"<<endl\
    <<"\t -mutation (defaut = 1 %) [entre 0 et 100]) taux de mutation"<<endl\
    <<"\t -prefix prefix du nom de fichier de log (default = [fonction])"<<endl\
    <<"\t -fonction (defaut = schwefel) [schwefel/six_hump] "<<endl\
    <<"\t -create (defaut = tournament) [perso/tournament] creation mode"<<endl\
    <<"\t -delete (defaut = tournament) [perso/tournament] delete mode delete mode"<<endl\
    <<"\t -eval (defaujt = 0) [1/0] always eval new"<<endl\
    <<"\t -threads (defaut = 1) [-1 pour le max possible] nombre de thread à utiliser"<<endl\
    <<"\t -runtime (defaut = 180) temps en seconde d'exicution"<<endl\
    <<"\t -slow (default = -1) permet de faire une pause de X ms à la fin de chaque génération (pour voir la courbe)"<<endl\
    ;exit(1);\
}


gnuplot_ctrl* h1;
std::vector<double> points[2];
int gnuplottimeout;
int runtime;
clock_t start;
string min;
int slowtime;

     
int main(int argc,char * argv[])
{
    rand_init();
    h1 = gnuplot_init();

    cout.precision(10);

    int gnuplottimeout = 50;

    int pop_size = 1000;
    int pop_child = 1000;
    float mutation_taux = 1;
    std::string filename= "";
    double (*benchmarks)(const std::vector<double>&) = schwefel;
    int nb_threads = 1;
    runtime = 3*60;
    slowtime = -1;
    string creation = "tournament";
    string del = "tournament";
    bool eval = false;

    
    {
        int i=1;
        while(i<argc)
        {
            string arg = string(argv[i]);
            if (arg =="-pop-size")
            {
                if(++i <argc)
                {
                    pop_size = atoi(argv[i]);
                    if(pop_size <0)
                        SHOW_ARGS("Pas de population négative possible")
                }
                else
                    SHOW_ARGS("Pas de population de précisée")
            }
            else if (arg == "-pop-enf")
            {
                if(++i <argc)
                {
                    pop_child = atoi(argv[i]);
                    if(pop_child <0)
                        SHOW_ARGS("Pas de population négative possible")
                }
                else
                    SHOW_ARGS("Pas de population de précisée")
            }
            else if(arg == "-mutation")
            {
                if(++i <argc)
                {
                    mutation_taux = atoi(argv[i]);
                    if(mutation_taux <0 or mutation_taux > 100)
                        SHOW_ARGS("Taux de mutation mauvais")

                }
                else
                    SHOW_ARGS("Pas de mutation de précisée")
            }
            else if (arg == "-prefix")
            {
                if(++i <argc)
                {
                    filename = argv[i];
                }
                else
                    SHOW_ARGS("Pas de préfix de précisé")
            }
            else if (arg == "-fonction")
            {
                if(++i < argc)
                {
                    arg = argv[i];
                    if(arg == "schwefel")
                        benchmarks = schwefel;
                    else if (arg == "six_hump")
                        benchmarks = six_hump;
                    else
                        SHOW_ARGS("mauvais nom de foction");
                }
                else
                    SHOW_ARGS("Pas de fonction de précisé");
            }
            else if(arg == "-threads")
            {
                if(++i < argc)
                {
                    nb_threads = atoi(argv[i]);
                }
                else
                    SHOW_ARGS("Pas de nombre de précisé");
            }
            else if(arg =="-runtime")
            {
                if(++i < argc)
                {
                    runtime = atoi(argv[i]);
                }
                else
                    SHOW_ARGS("Pas de nombre de précisé");
            }
            else if (arg =="-h" or arg=="-help")
                SHOW_ARGS("Aide")            
            else if (arg == "-slow")
            {
                if(++i < argc)
                {
                    slowtime = atoi(argv[i]);
                }
                else
                    SHOW_ARGS("Pas de nombre de précisé");
            }
            else if(arg == "-create")
            {
                if(++i < argc)
                {
                    creation = argv[i];
                }
                else
                    SHOW_ARGS("Pas de mode de précisé");
            }
            else if(arg == "-delete")
            {
               if(++i < argc)
               {
                   del = argv[i];
               }
               else
                   SHOW_ARGS("Pas de mode de précisé");
            }
            else if (arg == "-eval")
            {
                if(++i < argc)
                {
                    eval = (atoi(argv[i])==1);
                }
                else
                    SHOW_ARGS("Pas de nombre de précisé");
            }
            else
                SHOW_ARGS("Mauvais argument");
            ++i;
        }

        if(filename == "")
        {
            filename = (benchmarks==schwefel)?"schwefel":"six_hump";
        }
        mutation_taux /=100;
    }

    cout<<"Aguments: "
    <<"\n pop-size: "<<pop_size
    <<"\n pop-enf: "<< pop_child
    <<"\n mutation: "<<mutation_taux*100
    <<"\n prefix: "<<filename
    <<"\n fonction: "<<((benchmarks==schwefel)?"schwefel":"six_hump")
    <<"\n create: "<<creation
    <<"\n delelte: "<<del
    <<"\n eval: "<<eval
    <<"\n threads: "<<nb_threads
    <<"\n runtime: "<<runtime
    <<"\n slow: "<<slowtime
    <<endl;

    if(benchmarks == six_hump)
    {
        std::vector<double> v = {0.0898,-0.7126};
        cout<<"min six_hump: "<<six_hump(v)<<" ("<<v[0]<<","<<v[1]<<")"<<endl;

        Individu<2>::benchmarks = six_hump;
        Individu<2>::bornes[0][Individu<2>::MIN] =-3;
        Individu<2>::bornes[0][Individu<2>::MAX] = 3;

        Individu<2>::bornes[1][Individu<2>::MIN] =-2;
        Individu<2>::bornes[1][Individu<2>::MAX] = 2;

        GeneticEngine<Individu<2> > engine(nb_threads,mutation_taux,filename,pop_size,pop_child);
        engine.setTimeout(1000);
        engine.setEvaluateAll(eval);

        bool (*stop)(const Individu<2>&,const int) = [](const Individu<2>& best,const int generation)
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
            if(slowtime >0)
                std::this_thread::sleep_for(std::chrono::milliseconds(slowtime));
            cerr<<float(clock() - start)/CLOCKS_PER_SEC<<" sec"<<endl;
            return float(clock() - start)/CLOCKS_PER_SEC > runtime;
        };
        
        if(creation == "perso")
            engine.setCreationMode(GeneticEngine<Individu<2> >::CreationMode::STUPIDE);
        if(del == "perso")
            engine.setReductionMode(GeneticEngine<Individu<2> >::ReductionMode::STUPIDE);

        start = clock();
        Individu<2>* best = engine.run_while(stop);
        delete best;
        exit(0);
    }
    else if (benchmarks == schwefel)
    {
        std::vector<double> v = {420.9687};
        cout<<"min schwefel: "<<schwefel(v)<<" ("<<v[0]<<")"<<endl;

        Individu<1>::benchmarks = schwefel;
        Individu<1>::bornes[0][Individu<1>::MIN] =-500;
        Individu<1>::bornes[0][Individu<1>::MAX] = 500;

        GeneticEngine<Individu<1> > engine(nb_threads,mutation_taux,filename,pop_size,pop_child);
        engine.setTimeout(1000);
        engine.setEvaluateAll(eval);
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
            if(slowtime >0)
                std::this_thread::sleep_for(std::chrono::milliseconds(slowtime));
            cerr<<float(clock() - start)/CLOCKS_PER_SEC<<" sec"<<endl;
            return float(clock() - start)/CLOCKS_PER_SEC > runtime;
        };

        if(creation == "perso")
            engine.setCreationMode(GeneticEngine<Individu<1> >::CreationMode::STUPIDE);
        if(del == "perso")
            engine.setReductionMode(GeneticEngine<Individu<1> >::ReductionMode::STUPIDE);

        start = clock();
        Individu<1>* best = engine.run_while(stop);
        delete best;
        exit(0);
    }

    return 0;
};
