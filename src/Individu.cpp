#include "Individu.hpp"

Individu::Individu() : score(0), _size(0)
{
    x = random(-10.f,10.f);
    y = random(-10.f,10.f);
};

Individu::~Individu()
{
};

void Individu::mutate()
{
    x += random(-1.f,1.f);
    y += random(-1.f,1.f);
};

Individu* Individu::crossOver(const Individu& other) const
{
    Individu* res = new Individu();
    res->x = x;
    res->y = other.y;

    return res;
};

Individu* Individu::clone()
{
    Individu* res = new Individu();
    res->x = x;
    res->y = y;

    return res;
};

void Individu::eval()
{
    score = (x-y)/1000.0;
};


bool Individu::operator>(const Individu& other)const
{
    if (score > other.score)
        return true;

    if (this->score == other.score)
        return this->size()<other.size();

    return false;
};
