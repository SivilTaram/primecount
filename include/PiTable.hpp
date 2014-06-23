///
/// @file    PiTable.hpp
/// @brief   The PiTable class is a compressed lookup table for prime
///          counts. It uses only n / 128 * 12 bytes of memory and
///          returns the number of primes below n using O(1) operations.
///
///          How it works:
///
///          The prime count is calculated in 2 steps:
///          1) pi(n - n % 128) is stored in pi_[n / 128].prime_count
///          2) The remaining primes inside ]n - n % 128, n]
///             are stored in pi_[n / 128].bits, each bit corresponds
///             to an odd integer i.e. bit[13] = n - n % 128 + 2 * 13.
///             If the bit is set the corresponding integer is a
///             prime.
///
/// Copyright (C) 2014 Kim Walisch, <kim.walisch@gmail.com>
///
/// This file is distributed under the BSD License. See the COPYING
/// file in the top level directory.
///

#ifndef PITABLE_HPP
#define PITABLE_HPP

#if !defined(__STDC_CONSTANT_MACROS)
  #define __STDC_CONSTANT_MACROS
#endif

#include <stdint.h>
#include <cassert>
#include <vector>

namespace primecount {

class PiTable
{
public:
  PiTable(uint64_t max);
  void init();

  /// Get the number of primes <= n.
  /// This implementation uses only 19 arithmetic operations.
  /// @pre n > 2
  ///
  uint64_t operator()(uint64_t n) const
  {
    assert(n > 2);
    assert(n <= max_);
    return pi_[n / 128].prime_count + popcount_3(pi_[n / 128].bits & bitmasks_[n % 128]);
  }
private:
  struct PiPair
  {
    PiPair() : prime_count(0), bits(0) {}
    uint32_t prime_count;
    uint64_t bits;
  };

  static const uint64_t bitmasks_[128];
  std::vector<PiPair> pi_;
  uint64_t max_;

  /// Count the number of 1 bits in x.
  /// This implementation uses only 12 arithmetic operations.
  /// http://en.wikipedia.org/wiki/Hamming_weight#Efficient_implementation
  /// 
  static uint64_t popcount_3(uint64_t x)
  {
    const uint64_t m1  = UINT64_C(0x5555555555555555);
    const uint64_t m2  = UINT64_C(0x3333333333333333);
    const uint64_t m4  = UINT64_C(0x0f0f0f0f0f0f0f0f);
    const uint64_t h01 = UINT64_C(0x0101010101010101);

    x -= (x >> 1) & m1;
    x = (x & m2) + ((x >> 2) & m2);
    x = (x + (x >> 4)) & m4;
    return (x * h01) >> 56;
  }
};

} // namespace

#endif