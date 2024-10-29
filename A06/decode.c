/*----------------------------------------------
 * Author: Lily Davoren
 * Date: October 13, 2024
 * Description: This program reads in a PPM file and decodes a message from the least significant bit of each color channel of each pixel.
 ---------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include "read_ppm.h"
# include <string.h>

char* get_substring(char* string, int position, int length) {
  char* substring = (char*)malloc(length + 1);
  for (int i = 0; i < length; i++) {
    substring[i] = string[position + i];
  }
  substring[length] = '\0';
  return substring;
  free(substring);
}

int binary_to_decimal(char *binary) {
    int decimal = 0;
    for (int i = 0; i < 8; i++) {
        if (binary[i] == 