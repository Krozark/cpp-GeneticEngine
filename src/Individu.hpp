#ifndef INDIVIDU_HPP
#define INDIVIDU_HPP

#include "random.hpp"

class Individu
{
    public:
        Individu();
        ~Individu();
        /* change current individu */
        void mutate();
        /* create a new Individu with 2 others */
        Individu * crossOver(Individu& other);
        /* siple getters */
        inline const int get_score()const {return score;};
        inline const int size()const {return _size;};
        /* display the individu to save it in file */
        friend std::ostream& operator<<(std::ostream& output,const Individu& self)
        {
            return output;
        };

        /* set the score */
        void eval();

        bool operator>(const Individu& other)const;

    private:
        int score;
        int _size;
};

#endif
