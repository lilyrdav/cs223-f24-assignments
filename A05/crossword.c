/*----------------------------------------------
 * Author: 
 * Date: 
 * Description
 ---------------------------------------------*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char* argv[]) {
  if (argc < 3) {
    printf("Usage: %s <word1> <word2>\n", argv[0]);
    exit(0);
  }

  //Dynamically allocate a 2D array
  char* two_d_array = malloc(sizeof(char) * (strlen(argv[1]) * strlen(argv[2])));

  //Find the common letter between the two words and make note of the row and column
  int row = 0;
  int col = 0;
  for (int k = 0; k < strlen(argv[1]); k++) {
    for (int l = 0; l < strlen(argv[2]); l++) {
      if ((argv[1])[k] == (argv[2])[l]) {
        row = k;
        col = l;
        break;
      }
      else {
        printf("No common letter found between the two words.\n");
        free(two_d_array);
        exit(0);
      }
    }
  }

  //Fill in the 2D array with the first word
  for (int m = 0; m < strlen(argv[1]); m++) {
    two_d_array[m * strlen(argv[2]) + col] = (argv[1])[m];
  }

  //Fill in the 2D array with the second word
  for (int n = 0; n < strlen(argv[2]); n++) {
    two_d_array[row * strlen(argv[2]) + n] = (argv[2])[n];
  }

  //Fill the rest of the 2D array with periods
  for (int o = 0; o < strlen(argv[1]); o++) {
    for (int p = 0; p < strlen(argv[2]); p++) {
      if (two_d_array[o * strlen(argv[2]) + p] != (argv[1])[o] && two_d_array[o * strlen(argv[2]) + p] != (argv[2])[p]) {
        two_d_array[o * strlen(argv[2]) + p] = '.';
      }
    }
  }

  //Print the 2D array
  for (int q = 0; q < strlen(argv[1]); q++) {
    for (int r = 0; r < strlen(argv[2]); r++) {
      printf("%c", two_d_array[q * strlen(argv[2]) + r]);
    }
    printf("\n");
  }

  //Free the 2D array
  free(two_d_array);

  return 0;
}