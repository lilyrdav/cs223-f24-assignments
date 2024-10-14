/*----------------------------------------------
 * Author: 
 * Date: 
 * Description
 ---------------------------------------------*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "read_ppm.h"
#include "write_ppm.h"

int main(int argc, char** argv) {
  if (argc != 2) {
    printf("usage: encode <file.ppm>\n");
    return 0;
  }

  int w = 4;
  int h = 4;
  struct ppm_pixel* pixels = read_ppm(argv[1], &w, &h);
  if (pixels == NULL) {
    printf("Error reading file\n");
    return 1;
  }
  
  printf("Reading %s with width %d and height %d\n", argv[1], w, h);
  printf("Max number of characters in the image: %d\n", ((w * h * 3) / 8));
  printf("Enter a phrase: ");
  char phrase[100];
  scanf(" %c", phrase);

  // Ensure the phrase fits within the maximum characters
  if (strlen(phrase) > (w * h * 3) / 8) {
    printf("Error: Phrase is too long!\n");
    free(pixels);
    return 1;
  }

  // Encode the phrase into the pixel data (simple bitwise encoding)
  for (int i = 0; i < strlen(phrase); i++) {
    for (int bit = 0; bit < 8; bit++) {
      if (i * 8 + bit >= ((w * h * 3) / 8) * 8) {
        break;
      }
      pixels[i * 8 + bit / 3].red = (pixels[i * 8 + bit / 3].r & 0xFE) | ((phrase[i] >> (7 - bit)) & 0x01);
    }
  }


  //Change file name to file name -encoded.ppm
  char* orig_file = argv[1];
  char* insert = "-encoded";
  int new_len = strlen(orig_file) + strlen(insert);
  char* new_file = (char*)malloc(new_len + 1);
  int i = 0;
  while (orig_file[i] != '.') {
    new_file[i] = orig_file[i];
    i++;
  }
  new_file[i] = '\0';
  strcat(new_file, insert);
  strcat(new_file, ".ppm");
  printf("Writing file %s\n", new_file);
  printf("Writing file ");



  return 0;
}

