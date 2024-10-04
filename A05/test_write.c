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
  char* width = argv[1];
  char* height = argv[2];

  int w = atoi(width);
  int h = atoi(height);

  struct ppm_pixel* pixels = read_ppm("feep-raw.ppm", &w, &h);
  write_ppm("test.ppm", pixels, w, h);
  struct ppm_pixel* pixels2 = read_ppm("test.ppm", &w, &h);

  printf("Testing file test.ppm: %d %d\n", w, h);
  for (int i = 0; i < w; i++) {
    for (int j = 0; j < h; j++) {
      printf("(%d, %d, %d)  ", pixels2[i * w + j].red, pixels2[i * w + j].green, pixels2[i * w + j].blue);
    }
    printf("\n");
  }

  free(pixels2);
  free(pixels);
  return 0;
}
