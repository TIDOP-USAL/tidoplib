// 1.2 Files to include
// Intrinsic functions are made available in different header files, based on the version of the SIMD instruction set they belong to:
// 
// <xmmintrin.h> : SSE, operations on 4 single precision floating point numbers (float).
// <emmintrin.h> : SSE 2, operations on integers and on 2 double precision floating point numbers (double).
// <pmmintrin.h> : SSE 3, horizontal operations on SIMD registers.
// <tmmintrin.h> : SSSE 3, additional instructions.
// <smmintrin.h> : SSE 4.1, dot product and many operations on integers
// <nmmintrin.h> : SSE 4.2, additional instructions.
// <immintrin.h> : AVX, operations on integers, 8 float or 4 double.
// Each of these files includes the previous one, so you only have to include the one matching the highest version of the SIMD 
// instruction set available in your processor. Later we will see how to detect at compile time which version on SIMD instruction 
// set is available and thus which file to include. For now, just assume we’re able to include the right file each time we need it.

#include <immintrin.h>
template<typename T>
struct Packed;

template<>
struct Packed<float>
{
  using type = __m256;
  int size = 8;
  type value;
};

template<>
struct Packed<double>
{
  using type = __m256d;
  int size = 4;
  type value;
};

template<>
struct Packed<int>
{
  using type = __m256i;
  int size = 8;
  type value;
};

template<>
struct Packed<unsigned int>
{
  using type = __m256i;
  int size = 8;
  type value;
};

template<>
struct Packed<char>
{
  using type = __m256i;
  int size = 32;
  type value;
};

template<>
struct Packed<unsigned char>
{
  using type = __m256i;
  int size = 32;
  type value;
};

template<>
struct Packed<long>
{
  using type = __m256i;
  int size = 4;
  type value;
};

template<>
class Packed<unsigned long>
{
  using type = __m256i;
  int size = 4;
  type value;
};

template<typename T> inline
typename std::enable_if<
  std::is_same<float, typename std::remove_cv<T>::type>::value, Packed<typename std::remove_cv<T>::type>>::type
mm256_load(T *data)
{
  Packed<T> r = _mm256_load_ps(data);
  return r;
}

template<typename T> inline
typename std::enable_if<
  std::is_same<double, typename std::remove_cv<T>::type>::value, void>::type
mm256_load(T *data)
{
  Packed<T> r = _mm256_load_pd(data);
  return r;
}

template<typename T> inline
typename std::enable_if<
  std::is_same<float, typename std::remove_cv<T>::type>::value, Packed<T>>::type
mm256_fmadd(const Packed<T> &A, const Packed<T> &B, Packed<T> &C)
{
  Packed<T> r = _mm256_fmadd_ps(A, B, C);
  return r;
}

template<typename T> inline
typename std::enable_if<
  std::is_same<double, typename std::remove_cv<T>::type>::value, Packed<T>>::type
  mm256_fmadd(const Packed<T> &A, const Packed<T> &B, Packed<T> &C)
{
  Packed<T> r = _mm256_fmadd_pd(A, B, C);
  return r;
}

template<typename T> inline
typename std::enable_if<
  std::is_same<float, typename std::remove_cv<T>::type>::value, void>::type
mm256_storeu(T *data, const Packed<T> &B, Packed<T> &result)
{
  _mm256_storeu_ps(data, result);
}

template<typename T> inline
typename std::enable_if<
  std::is_same<double, typename std::remove_cv<T>::type>::value,void>::type
mm256_storeu(T *data, const Packed<T> &B, Packed<T> &result)
{
  _mm256_storeu_pd(data, result);
}
