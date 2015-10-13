#ifndef __PARITY_H__
#define __PARITY_H__

uint8_t xor(uint8_t d[]);
uint8_t rs(uint8_t d[]);
void find_one_data_missing(uint8_t d[], uint8_t pos, uint8_t p);
void find_one_data_parity_missing(uint8_t d[], uint8_t pos, uint8_t q);
void find_two_data_missing(uint8_t d[], uint8_t pos1, uint8_t pos2, 
                                uint8_t p, uint8_t q);

#endif
