#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <stdint.h>

/* https://www.kernel.org/pub/linux/kernel/people/hpa/raid6.pdf */

/* x^8 + x^4 + x^3 + x^2 + 1 mod 255 */
#define LFSR(c) ((( (c) << 1) ^ (((c) & 0x80) ? 0x1d : 0)) % 256)

uint8_t *gfilog;
uint8_t *gflog;

int setup()
{
  gfilog = (uint8_t *) malloc(255 * sizeof(uint8_t));
  gflog  = (uint8_t *) malloc(255 * sizeof(uint8_t));
  memset(gfilog, 0, 255 * sizeof(uint8_t));
  memset(gflog, 0, 255 * sizeof(uint8_t));
  
  int c = 1;
  for(int i = 0; i < 256; i++)
  {
    gfilog[i] = c;
    gflog[c] = i;
    c = LFSR(c);
  }
}

uint8_t gfmult(uint8_t a, uint8_t b)
{
  if (a == 0 || b == 0) return 0;

  int16_t log_sum = gflog[a] + gflog[b];
  if (log_sum >= 255) log_sum -= 255;
  return gfilog[log_sum];
}

int gfdiv(int a, int b)
{
  if (a == 0 || b == 0) return 0;

  int16_t gflog_sub = gflog[a] - gflog[b];
  if (gflog_sub < 0) gflog_sub += 255;
  return gfilog[gflog_sub];
}

uint8_t gfpow(uint8_t exp) { return gfilog[exp]; }
uint8_t gfmiv(uint8_t i) { return gfdiv(1, i); }

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

void print_array(uint8_t *data, uint8_t len)
{
  for(int i = 0; i < len; i++)
    printf("%02x ", data[i]);
  printf("\n");
}

int main()
{
  setup();
  uint8_t data[] = {'H', 'E', 'L', 'L', 'O'};
  print_array(data, 5);

  uint8_t p  = xor(data);
  uint8_t q = rs(data);

  printf("%x %x\n", p, q);

  data[3] = 0;
  print_array(data, 5);
  find_one_data_missing(data, 3, p);
  print_array(data, 5);

  data[3] = 0;
  print_array(data, 5);
  find_one_data_parity_missing(data, 3, q);
  print_array(data, 5);

  data[2] = 0;
  print_array(data, 5);
  find_one_data_parity_missing(data, 2, q);
  print_array(data, 5);

  data[1] = 0;
  data[2] = 0;
  print_array(data, 5);
  find_two_data_missing(data, 1, 2, p, q);
  print_array(data, 5);

  return 0;
}
