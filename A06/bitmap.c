/*----------------------------------------------
 * Author: Lily Davoren
 * Date: October 9, 2024
 * Description: This program implements a program that reads
 * in a single 64-bit unsigned integer and outputs it as an 8x8 1bpp sprite. The program
 * uses a bitmask. For example, to generate a 64-bit mask that obtains
 * the left-most bit, do 0x1ul << 63. The ul indicates an unsigned long value.
 * 0x1 is the number one in hexadecimal.The bitmap files store the values as
 * hexadecimal values.
 ---------------------------------------------*/
#include <stdio.h>

int main() {
  unsigned long img;
  scanf(" %lx", &img);
  printf("Image (unsigned long): %lx\n", img);

  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 8; j++) {
      unsigned long mask = 0x1ul << (63 - (i * 8 + j));
      printf(" %c", (img & mask) ? '@' : ' ');
    }
    printf