
template<int DIM>
Individu<DIM>::Individu(bool init) : score(0), _size(0)
{

    for(int i=0;init and i<DIM;++i)
        coef[i] = random(bornes[i][MIN],bornes[i][MAX]);
    evaluate = false;
};

template<int DIM>
Individu<DIM>::~Individu()
{
};

template<int DIM>
void Individu<DIM>::mutate()
{
    for(int i=0;i<DIM;++i)
    {
        coef[i] += random(-0.5f,0.5f);
        coef[i] = (coef[i]<bornes[MIN])?bornes[MIN]:coef[i];
        coef[i] = (coef[i]>bornes[MAX])?bornes[MAX]:coef[i];
    }
    evaluate = false;
};

template<int DIM>
Individu<DIM>* Individu<DIM>::crossOver(const Individu& other) const
{
    Individu<DIM>* res = this->clone();

    float gamma, alpha = 0.5;
    for(int i=0;i<DIM;++i)
    {
        gamma = (1 + 2* alpha) * random(0.f,1.f) - alpha;
        res->coef[i] = (1-gamma) * coef[i] + gamma*other.coef[i];
        res->coef[i] = (res->coef[i]<bornes[MIN])?bornes[MIN]:res->coef[i];
        res->coef[i] = (res->coef[i]>bornes[MAX])?bornes[MAX]:res->coef[i];

    }
    res->y = other.y;
    res->evaluate = false;

    return res;
};

template<int DIM>
Individu<DIM>* Individu<DIM>::clone()const
{
    Individu<DIM>* res = new Individu(false);
    for(int i=0;i<DIM;++i)
        res->coef[i] = coef[i];
    res->score = score;
    res->_size = _size;
    res->evaluate = true;

    return res;
};

template<int DIM>
void Individu<DIM>::eval()
{
    //score = (x-y)/1000.0;
    evaluate = true;
};


template<int DIM>
bool Individu<DIM>::operator>(const Individu& other)const
{
    if (score > other.score)
        return true;

    if (this->score == other.score)
        return this->size()<other.size();

    return false;
};
