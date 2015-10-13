/* malloc */
#include <stdlib.h>
/* memset */
#include <string.h>
#include <stdint.h>

/* https://www.kernel.org/pub/linux/kernel/people/hpa/raid6.pdf */

/* x^8 + x^4 + x^3 + x^2 + 1 mod 255 */
#define LFSR(c) ((( (c) << 1) ^ (((c) & 0x80) ? 0x1d : 0)) % 256)

uint8_t *gfilog;
uint8_t *gflog;

int gfsetup()
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

