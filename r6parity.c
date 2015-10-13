#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <stdbool.h>
#include <stdint.h>

#include "galoisfield.h"
#include "parity.h"

void print_array(uint8_t *data, uint8_t len)
{
  for(int i = 0; i < len; i++)
    printf("%02x ", data[i]);
  printf("\n");
}

int main()
{
  gfsetup();
  uint8_t data[] = {'H', 'E', 'L', 'L', 'O'};
  uint8_t data_len = 5;
  uint8_t data_len_bytes = data_len * sizeof(uint8_t);
  uint8_t *orig_data = (uint8_t *) malloc(data_len_bytes);
  memcpy(orig_data, data, data_len_bytes);

  bool test_pass;

  printf("Original: ");
  print_array(data, 5);

  uint8_t p  = xor(data);
  uint8_t q = rs(data);

  printf("Parity:   %x %x\n\n", p, q);

  printf("Missing Datum\n");
  data[3] = 0;
  print_array(data, 5);
  find_one_data_missing(data, 3, p);
  print_array(data, 5);
  test_pass = memcmp(data, orig_data, data_len_bytes);
  printf("Pass: %s\n\n", test_pass ? "NO" : "YES");

  printf("Missing Dataum and XOR Parity\n");
  data[0] = 0;
  print_array(data, 5);
  find_one_data_parity_missing(data, 0, q);
  print_array(data, 5);
  test_pass = memcmp(data, orig_data, data_len_bytes);
  printf("Pass: %s\n\n", test_pass ? "NO" : "YES");


  printf("Missing 2 Data\n");
  data[1] = 0;
  data[2] = 0;
  print_array(data, 5);
  find_two_data_missing(data, 1, 2, p, q);
  print_array(data, 5);
  test_pass = memcmp(data, orig_data, data_len_bytes);
  printf("Pass: %s\n\n", test_pass ? "NO" : "YES");

  return 0;
}
