/*----------------------------------------------
 * Author: 
 * Date: 
 * Description
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

int binary_to_decimal(const char *binary) {
    int decimal = 0;
    for (int i = 0; i < 8; i++) {
        if (binary[i] == '1') {
            decimal += (1 << (7 - i));
        }
    }
    return decimal;
}

int main(int argc, char** argv) {
  if (argc != 2) {
    prin