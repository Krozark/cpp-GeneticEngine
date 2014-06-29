#ifndef INDIVIDU_HPP
#define INDIVIDU_HPP

#include <GeneticEngine/random.hpp>

template<int N,int K>
class Individu
{
    public:
        Individu(bool init=true);
        ~Individu();
        /* change current individu */
        void mutate();
        /* create a new Individu with 2 others */
        Individu<N,K> * crossOver(const Individu<N,K>& other)const;

        Individu<N,K>* clone()const ;
        /* siple getters */
        inline const float get_score()const {return score;};
        inline const int size()const {return _size;};
        /* display the individu to save it in file */
        friend std::ostream& operator<<(std::ostream& output,const Individu<N,K>& self)
        {
            for(int i=0;i<N;++i)
                output<<self.values[i]<<",";
            return output;
        };

        /* set the score */
        void eval();
        
        bool need_eval(){return not evaluate;};

        bool operator>(const Individu<N,K>& other)const;

        
    private:
        int score;
        int _size;
        bool evaluate;
        int values[N];
};

#include "Individus.tpl"
#endif
