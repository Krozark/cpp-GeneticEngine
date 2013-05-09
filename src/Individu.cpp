#include "Individu.hpp"

Individu::Individu() : score(0), _size(0)
{
    x = random(-10.f,10.f);
    y = random(-10.f,10.f);
    evaluate = false;
};

Individu::~Individu()
{
};

void Individu::mutate()
{
    x += random(-1.f,1.f);
    y += random(-1.f,1.f);
    evaluate = false;
};

Individu* Individu::crossOver(const Individu& other) const
{
    Individu* res = this->clone();
    res->y = other.y;
    res->evaluate = false;

    return res;
};

Individu* Individu::clone()const
{
    Individu* res = new Individu();
    res->x = x;
    res->y = y;
    res->score = score;
    res->_size = _size;
    res->evaluate = true;

    return res;
};

void Individu::eval()
{
    score = (x-y)/1000.0;
    evaluate = true;
};


bool Individu::operator>(const Individu& other)const
{
    if (score > other.score)
        return true;

    if (this->score == other.score)
        return this->size()<other.size();

    return false;
};
