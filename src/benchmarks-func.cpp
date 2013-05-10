#include "benchmarks-func.hpp"

#include <cmath>

template<typename T>
inline constexpr T abs(const T& x) {return (x<T(0.0))?-x:x;};

float Q_rsqrt( float number )
{
        long i;
        float x2, y;
        const float threehalfs = 1.5F;
 
        x2 = number * 0.5F;
        y  = number;
        i  = * ( long * ) &y;                       // evil floating point bit level hacking
        i  = 0x5f3759df - ( i >> 1 );               // what the fuck?
        y  = * ( float * ) &i;
        y  = y * ( threehalfs - ( x2 * y * y ) );   // 1st iteration
        y  = y * ( threehalfs - ( x2 * y * y ) );   // 2nd iteration, this can be removed
 
        return y;
}

float InvSqrt(float x)
{
  float xhalf = 0.5f*x;
  int i = *(int*)&x; // get bits for floating value
  i = 0x5f375a86- (i>>1); // gives initial guess y0
  x = *(float*)&i; // convert bits back to float
  x = x*(1.5f-xhalf*x*x); // Newton step, repeating increases accuracy
  return x;
}

double schwefel(const std::vector<double>& dim)
{
    double res = 0.0;
    const int nb_dim = dim.size();
    assert(nb_dim > 0);

    for (int i=0;i<nb_dim;++i)
    {
        const double& xi = dim[i];
        res +=(-xi)*sin(sqrt(abs(xi)));
    }
    res /= nb_dim;
};
