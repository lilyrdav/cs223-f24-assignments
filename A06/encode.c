#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "read_ppm.h"
#include "write_ppm.h"

void char_to_binary(unsigned char c, int binary[8]) {
  for (int i = 0; i < 8; i++) {
    binary[i] = (c >> (7 - i)) & 1;
  }
}

int main(int argc, char** argv) {
  if (argc != 2) {
    printf("usage: encode <file.ppm>\n");
    return 0;
  }

  int w = 4, h = 4;
  struct ppm_pixel* pixels = read_ppm(argv[1], &w, &h);
  if (pixels == NULL) {
    printf("Error reading file\n");
    return 1;
  }

  printf("Reading %s with width %d and height %d\n", argv[1], w, h);
  int max_chars = (w * h * 3) / 8; 
  printf("Max number of characters in the image: %d\n", max_chars);

  printf("Enter a phrase: ");
  char phrase[100];
  scanf(" %99[^\n]", phrase);
  int phrase_len = strlen(phrase) + 1;

  if (phrase_len > max_chars) {
    printf("Message is too long to fit in the image.\n");
    free(pixels); 
    return 1;
  }

  for (int i = 0; i < phrase_len; i++) {
    int binary[8];
    char_to_binary(phrase[i], binary);

    for (int j = 0; j < 8; j++) {
      int pixel_idx = (i * 8 + j) / 3;
      int channel_idx = (i * 8 + j) % 3;

      if (channel_idx == 0) {
        int binary_red[8];
        char_to_binary(pixels[pixel_idx].red, binary_red);
        binary_red[7] = binary[j];
        int decimal = 0;
        for (int k = 0; k < 8; k++) {
          decimal = decimal * 2 + binary_red[k];
        }
        pixels[pixel_idx].red = (unsigned char)decimal;
      } else if (channel_idx == 1) {
        int binary_green[8];
        char_to_binary(pixels[pixel_idx].green, binary_green);
        binary_green[7] = binary[j];
        int decimal = 0;
        for (int k = 0; k < 8; k++) {
          decimal = decimal * 2 + binary_green[k];
        }
        pixels[pixel_idx].green = (unsigned char)decimal;
      } else {
        int binary_blue[8];
        char_to_binary(pixels[pixel_idx].blue, binary_blue);
        binary_blue[7] = binary[j];
        int decimal = 0;
        for (int k = 0; k < 8; k++) {
          decimal = decimal * 2 + binary_blue[k];
        }
        pixels[pixel_idx].blue = (unsigned char)decimal;
      }
    }
  }

  char* orig_file = argv[1];
  char* insert = "-encoded";
  int new_len = strlen(orig_file) + strlen(insert);
  char* new_file = (char*)malloc(new_len + 1);
  if (new_file == NULL) {
    printf("Memory allocation failed\n");
    free(pixels);
    return 1;
  }
  strcpy(new_file, orig_file);
  char* dot_position = strrchr(new_file, '.');
  if (dot_position != NULL) {
    *dot_position = '\0';
  }
  strcat(new_file, insert);
  strcat(new_file, ".ppm");

  printf("Writing file %s\n", new_file);
  write_ppm(new_file, pixels, w, h);

  free(new_file);
  free(pixels);

  return 0;
}