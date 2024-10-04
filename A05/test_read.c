/*----------------------------------------------
 * Author: 
 * Date: 
 * Description
 ---------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include "read_ppm.h"

int main() {
  int w = 4;
  int h = 4;
  struct ppm_pixel* pixels = read_ppm("feep-raw.ppm", &w, &h);

  printf("Testing file feep-raw.ppm: %d %d\n", w, h);
  for (int i = 0; i < w; i++) {
    for (int j = 0; j < h; j++) {
      printf("(%d, %d, %d)  ", pixels[i * w + j].red, pixels[i * w + j].green, pixels[i * w + j].blue);
    }
    printf("\n");
  }

  free(pixels);
  return 0;
}

