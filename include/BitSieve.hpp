///
/// @file  BitSieve.hpp
/// @brief The BitSieve class is a bit array for prime sieving
///        that packs 64 numbers into 8 bytes i.e. each bit
///        corresponds to one integer.
///
/// Copyright (C) 2014 Kim Walisch, <kim.walisch@gmail.com>
///
/// This file is distributed under the BSD License. See the COPYING
/// file in the top level directory.
///

#ifndef BITSIEVE_HPP
#define BITSIEVE_HPP

#if !defined(__STDC_CONSTANT_MACROS)
  #define __STDC_CONSTANT_MACROS
#endif

#include <popcount.hpp>

#include <cassert>
#include <cstddef>
#include <vector>
#include <stdint.h>

namespace primecount {

class BitSieve
{
public:
  BitSieve(std::size_t size);

  /// Set all bits to 1, except bits corresponding
  /// to 0, 1 and even numbers > 2.
  void fill(uint64_t low, uint64_t /* unused */);

  /// Count the number of 1 bits inside the interval [start, stop]
  uint64_t count(uint64_t start, uint64_t stop) const
  {
    if (start > stop)
      return 0;

    assert(stop < size_);

    uint64_t start_idx = start / 64;
    uint64_t stop_idx = stop / 64;
    uint64_t m1 = UINT64_C(0xffffffffffffffff) << (start % 64);
    uint64_t m2 = UINT64_C(0xffffffffffffffff) >> (63 - stop % 64);
    uint64_t bit_count;

    if (start_idx == stop_idx)
    {
      uint64_t bits = bits_[start_idx] & (m1 & m2);
      bit_count = popcount64(bits);
    }
    else
    {
      uint64_t bits = bits_[start_idx] & m1;
      bit_count = popcount64(bits);

      for (uint64_t i = start_idx + 1; i < stop_idx; i++)
        bit_count += popcount64(bits_[i]);

      bits = bits_[stop_idx] & m2;
      bit_count += popcount64(bits);
    }

    return bit_count;
  }

  /// Count the number of 1 bits inside [0, stop]
  uint64_t count(uint64_t stop) const
  {
    return count(0, stop);
  }

  bool operator[](uint64_t pos) const
  {
    assert(pos < size_);
    uint64_t bit = ((uint64_t) 1) << (pos & 63);
    return (bits_[pos >> 6] & bit) != 0;
  }

  void unset(uint64_t pos)
  {
    assert(pos < size_);
    bits_[pos >> 6] &= unset_bit_[pos & 63];
  }

  std::size_t size() const
  {
    return size_;
  }
private:
  static const uint64_t unset_bit_[64];
  std::vector<uint64_t> bits_;
  std::size_t size_;
};

} // namespace

#endif
