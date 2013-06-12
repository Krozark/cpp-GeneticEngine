template<int N,int K>
Individu<N,K>::Individu(bool init): score(0), _size(0)
{
    for(int i=0;init and i<N;++i)
        values[i] = random(0,K-1);
    evaluate = false;
};

template<int N,int K>
Individu<N,K>::~Individu()
{
}

template<int N,int K>
void Individu<N,K>::mutate()
{
   values[random(0,N-1)] = random(0,K); 
};

template<int N,int K>
Individu<N,K>* Individu<N,K>::crossOver(const Individu<N,K>& other) const
{
    Individu<N,K>* res= this->clone();
    int max = (score>other.score)?score:other.score;
    if (max <=0)
        return res;
    res->evaluate = false;
    int point = random(0,max-1);
    for (int i=point;i<N;++i)
    {
        res->values[i] = other.values[i];
    }
    return res; 
};

template<int N,int K>
Individu<N,K>* Individu<N,K>::clone()const
{
    Individu<N,K>* res = new Individu<N,K>(false);
    for(int i=0;i<N;++i)
        res->values[i] = values[i];
    return res;
}

template<int N,int K>
void Individu<N,K>::eval()
{
    score = 0;
    evaluate = true;
}

template<int N,int K>
bool Individu<N,K>::operator>(const Individu<N,K>& other) const
{
    return score > other.score;
}
