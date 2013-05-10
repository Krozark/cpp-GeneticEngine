#ifndef INDIVIDU_HPP
#define INDIVIDU_HPP

#include "random.hpp"

#include <iostream>

template<int DIM>
class Individu
{
    public:
        Individu(bool init=true);
        ~Individu();
        /* change current individu */
        void mutate();
        /* create a new Individu with 2 others */
        Individu * crossOver(const Individu& other)const;

        Individu* clone()const ;
        /* siple getters */
        inline const float get_score()const {return score;};
        inline const int size()const {return _size;};
        /* display the individu to save it in file */
        friend std::ostream& operator<<(std::ostream& output,const Individu& self)
        {
            for(int i=0;i<DIM;++i)
                output<<i<<": <"<<self.coef[i]<<"> ";
            return output;
        };

        /* set the score */
        void eval();
        
        bool need_eval(){return not evaluate;};

        bool operator>(const Individu& other)const;

        
        enum _BORNE{MIN=0,MAX};
        static float bornes[DIM][2]; // [dim] {min,max}

    private:
        float score;
        int _size;
        bool evaluate;

        float coef[DIM];
};


#include "Individu.tpl"

#endif
