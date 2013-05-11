#ifndef INDIVIDU_HPP
#define INDIVIDU_HPP

#include "random.hpp"

#include <iostream>
#include <vector>

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
                output<<"["<<i<<"]: <"<<self.coef[i]<<"> ";
            return output;
        };

        /* set the score */
        void eval();
        
        bool need_eval(){return not evaluate;};

        bool operator>(const Individu& other)const;

        
        enum _BORNE{MIN=0,MAX};
        static float bornes[DIM][2]; // [dim] {min,max}
        static double (*benchmarks)(const std::vector<double>&);

        inline const std::vector<double>& getCoef()const{return coef;};
        

    private:
        float score;
        int _size;
        bool evaluate;
        

        std::vector<double> coef;
};

template<int DIM>
float Individu<DIM>::bornes[DIM][2];

template<int DIM>
double (*Individu<DIM>::benchmarks)(const std::vector<double>&);


#include "Individu.tpl"

#endif
