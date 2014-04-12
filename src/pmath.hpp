///
/// @file  pmath.hpp
///
/// Copyright (C) 2014 Kim Walisch, <kim.walisch@gmail.com>
///
/// This file is distributed under the BSD License. See the COPYING
/// file in the top level directory.
///

#ifndef PMATH_HPP
#define PMATH_HPP

#include <stdint.h>
#include <cmath>
#include <vector>
#include <limits>


template <typename T>
inline T number_of_bits(T)
{
  return static_cast<T>(sizeof(T) * 8);
}

/// @brief  Round up to the next power of 2.
/// @see    Book "Hacker's Delight".
///
template <typename T>
inline T next_power_of_2(T x)
{
  x--;
  for (T i = 1; i < number_of_bits(x); i += i)
    x |= (x >> i);
  return ++x;
}

inline int64_t isquare(int64_t x)
{
  return x * x;
}

/// Raise to power
template <int N>
inline int64_t ipow(int64_t x)
{
  int64_t r = 1;
  for (int i = 0; i < N; i++)
    r *= x;

  return r;
}

/// Integer suare root
inline int32_t isqrt(int64_t x)
{
  int32_t r = static_cast<int32_t>(std::sqrt(static_cast<double>(x)));
  // correct rounding error
  while (ipow<2>(r) > x)
    r--;
  while (ipow<2>(r + 1) <= x)
    r++;
  return r;
}

/// Integer nth root
template <int N>
inline int32_t iroot(int64_t x)
{
  int32_t r = static_cast<int32_t>(std::pow(static_cast<double>(x), 1.0 / N));
  // correct rounding error
  while (ipow<N>(r) > x)
    r--;
  while (ipow<N>(r + 1) <= x)
    r++;
  return r;
}

/// Initialize a vector with Möbius function values.
/// This implementation is based on code by Rick Sladkey
/// posted here: http://mathoverflow.net/a/99545
///
inline std::vector<int32_t> make_moebius(int64_t max)
{
  std::vector<int32_t> mu(max + 1, 1);

  for (int32_t i = 2; i * i <= max; i++)
  {
    if (mu[i] == 1)
    {
      for (int32_t j = i; j <= max; j += i)
        mu[j] *= -i;
      for (int32_t j = i * i; j <= max; j += i * i)
        mu[j] = 0;
    }
  }
  for (int32_t i = 2; i <= max; i++)
  {
    if (mu[i] == i)
      mu[i] = 1;
    else if (mu[i] == -i)
      mu[i] = -1;
    else if (mu[i] < 0)
      mu[i] = 1;
    else if (mu[i] > 0)
      mu[i] = -1;
  }
  return mu;
}

/// Initialize a vector with the least prime
/// factors of the integers <= max.
///
inline std::vector<int32_t> make_least_prime_factor(int64_t max)
{
  std::vector<int32_t> lpf(max + 1, 1);

  // phi(x / 1, c) contributes to the sum, thus
  // set lpf[1] = MAX in order to pass
  // if (lpf[1] > primes[c])
  if (lpf.size() > 1)
    lpf[1] = std::numeric_limits<int32_t>::max();

  for (int32_t i = 2; i * i <= max; i++)
    if (lpf[i] == 1)
      for (int32_t j = i * 2; j <= max; j += i)
        if (lpf[j] == 1)
          lpf[j] = i;

  for (int32_t i = 2; i <= max; i++)
    if (lpf[i] == 1)
      lpf[i] = i;

  return lpf;
}

#endif