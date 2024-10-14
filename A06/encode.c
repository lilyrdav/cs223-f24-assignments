/*----------------------------------------------
 * Author: Lily Davoren
 * Date: October 13, 2024
 * Description: This program reads in a PPM file and encodes a message in the least significant bit of each color channel of each pixel. 
 * The program reads in a PPM file and a message from the user. The program then encodes the message in the image and writes the new
 * image to a file with the same name as the original file with "-encoded" appended to the name. The program uses the read_ppm() and 
 * write_ppm() functions to read and write the PPM files.
 ---------------------------------------------*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "read_ppm.h"
#include "write_ppm.h"

void char_to_binary(unsigned char c, int binary[8]) {
  for (int i = 7; i >= 0; i--) {
    binary[7 - i] = (c >> i) & 1;
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
  scanf(" %c", phrase);
  int phrase_len = strlen(phrase) + 1;

  if (phrase_len > max_chars) {
    printf("Message is too long to fit in the image.\n");
    free(pixels); 
    return 1;
  }

  int bit_idx = 0;
  for (int i = 0; i < phrase_len; i++) {
    int binary[8];
    char_to_binary(phrase[i], binary);

    for (int j = 0; j < 8; j++) {
      int pixel_idx = bit_idx / 3;
      int color_channel_idx = bit_idx % 3;

      if (color_channel_idx == 0) {
        pixels[pixel_idx].red = (pixels[pixel_idx].red & 0xFE) | binary[j];
      } else if (color_channel_idx == 1) {
        pixels[pixel_idx].green = (pixels[pixel_idx].green & 0xFE) | binary[j];
      } else {
        pixels[pixel_idx].blue = (pixels[pixel_idx].blue & 0xFE) | binary[j];
      }
      bit_idx++;
    }
  }

    char* orig_file = argv[1];
    char* insert = "-encoded";
    int new_len = strlen(orig_file) + strlen(insert);
    char* new_file = (char*)malloc(new_len + 1);
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
