#ifndef __GALOISFIELD_H__
#define __GALOISFIELD_H__

/* https://www.kernel.org/pub/linux/kernel/people/hpa/raid6.pdf */

/* x^8 + x^4 + x^3 + x^2 + 1 mod 255 */
#define LFSR(c) ((( (c) << 1) ^ (((c) & 0x80) ? 0x1d : 0)) % 256)

int gfsetup();

uint8_t gfmult(uint8_t a, uint8_t b);
uint8_t gfdiv(int a, int b);
uint8_t gfpow(uint8_t exp);
uint8_t gfmiv(uint8_t i);

#endif
