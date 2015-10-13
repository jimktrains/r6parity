#include "galoisfield.h"
#include <stdint.h>

uint8_t xor(uint8_t d[])
{
  return d[0] ^ d[1] ^ d[2] ^ d[3] ^ d[4];
}

uint8_t rs(uint8_t d[])
{
  uint8_t r = 0;
  r = LFSR(r ^ d[4]);
  r = LFSR(r ^ d[3]);
  r = LFSR(r ^ d[2]);
  r = LFSR(r ^ d[1]);
  r = r ^ d[0];
  
  return r;
}


/*
 * Missing block should be set to 0
 */
void find_one_data_missing(uint8_t d[], uint8_t pos, uint8_t p)
{
  d[pos] = xor(d) ^ p;
}

void find_one_data_parity_missing(uint8_t d[], uint8_t pos, uint8_t q)
{
  uint8_t qx = rs(d);
  qx = qx ^ q;

  d[pos] = gfmult(qx, gfmiv(gfpow(pos)));
}

uint8_t coeffA(uint8_t x, uint8_t y)
{
  return gfmult(gfmiv(gfpow(y-x) ^ 1), gfpow(y-x));
}
uint8_t coeffB(uint8_t x, uint8_t y)
{
  return gfmult(gfmiv(gfpow(y-x) ^ 1), gfmiv(gfpow(x)));
}

void find_two_data_missing(uint8_t d[], uint8_t pos1, uint8_t pos2, 
                                uint8_t p, uint8_t q)
{
  uint8_t pxy = xor(d);
  uint8_t qxy = rs(d);

  uint8_t A = coeffA(pos1, pos2);
  uint8_t B = coeffB(pos1, pos2);
  d[pos1] = gfmult(A, p^pxy) ^ gfmult(B, q ^ qxy);

  find_one_data_missing(d, pos2, p);
}

