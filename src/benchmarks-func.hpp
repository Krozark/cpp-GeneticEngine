#ifndef BENCHMARKS_FUNC_HPP
#define BENCHMARKS_FUNC_HPP

#include <vector>

template<typename T>
inline constexpr T abs(const T& x) {return (x<T(0.0))?-x:x;};

template <int N>
struct power {
    template <typename NUMERIC_TYPE>
    static inline NUMERIC_TYPE of(const NUMERIC_TYPE& x) {
        return x * power<N-1>::of(x);
    }
};
 
template <>
struct power<0> {
    template <typename NUMERIC_TYPE>
    static inline NUMERIC_TYPE of(const NUMERIC_TYPE& x) {
        return 1;
    }
};

double schwefel(const std::vector<double>& dim);
double six_hump(const std::vector<double>& dim);

#endif
