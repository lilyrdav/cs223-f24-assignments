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
        if (binary[i] == '1') {
            decimal += (1 << (7 - i));
        }
    }
    return decimal;
}

int main(int argc, char** argv) {
  if (argc != 2) {
    printf("usage: ./decode <file.ppm>\n");
    return 0;
  }

  int w = 606;
  int h = 771;
  struct ppm_pixel* pixels = read_ppm(argv[1], &w, &h);
  if (pixels == NULL) {
    printf("Error reading file\n");
    return 1;
  }

  printf("Reading %s with width %d and height %d\n", argv[1], w, h);
  printf("Max number of characters in the image: %d\n", ((w * h * 3) / 8));

  char* binary = (char*)malloc((w * h * 3));
  for (int i = 0; i < w * h * 3; i++) {
    if (pixels[i].red % 2 == 0) {
      binary[i * 3] = '0';
    } else {
      binary[i * 3] = '1';
    }
    if (pixels[i].green % 2 == 0) {
      binary[i * 3 + 1] = '0';
    } else {
      binary[i * 3 + 1] = '1';
    }
    if (pixels[i].blue % 2 == 0) {
      binary[i * 3 + 2] = '0';
    } else {
      binary[i * 3 + 2] = '1';
    }
  }

  for (int k = 0; k < w * h * 3; k += 8) {
    char* substring = get_substring(binary, k, 8);

    if (strcmp(substring, "00000000") == 0) {
      break;
    }
    int decimal = binary_to_decimal(substring);
    printf("%c", decimal);
  }

  free(binary);
  free(pixels);
  
  return 0;
}