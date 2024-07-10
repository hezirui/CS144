#include "wrapping_integers.hh"
#include <math.h>
#include <iostream>

using namespace std;

Wrap32 Wrap32::wrap( uint64_t n, Wrap32 zero_point )
{
  return zero_point + static_cast<uint32_t>(n);
}

uint64_t Wrap32::unwrap( Wrap32 zero_point, uint64_t checkpoint ) const
{
  uint64_t max2_32 = (uint64_t)1 << 32;
  uint64_t n = checkpoint / max2_32;
  uint64_t offset = raw_value_ - zero_point.raw_value_;
  checkpoint %= max2_32;
  if((offset < checkpoint) && (checkpoint - offset > max2_32/2))
    return (n+1) * max2_32 + offset;
  else if((offset > checkpoint) && (offset - checkpoint > max2_32/2) &&( n >0))
    return (n-1) * max2_32 + offset;
  return n * max2_32 + offset;
}
