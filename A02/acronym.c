/**
* The main driver program for A02 acronym.c.
*
* This program computes an acronym from a given phrase. The program creates
* a string consisting of all capital letters from the given phrase.
*
* @author: Lily Davoren
* @version: September 12, 2024
*/

#include <stdio.h>
#include <string.h>

int main() {
  char phrase[1024];
  printf("Enter a phrase: ");
  scanf("%[^\n]%*c", phrase);
  printf("Your acronym is: ");

  //Loops through the entered phrase, searching for capital letters, and prints them.
  for (int i = 0; i < strlen(phrase); i++) {
    if (phrase[i] > 64 && phrase[i] < 91) {
      printf("%c", phrase[i]);
    }
  }

  printf("\n");
  return 0;
}
