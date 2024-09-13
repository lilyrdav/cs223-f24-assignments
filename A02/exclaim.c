/**
* The main driver program for A02 exclaim.c.
*
* This program turns a word into an exclaimation. The program loops through each
* character. If the character is a lowercase letter, it is replaced with a randomly
* chosen character from the set {'@', '!', '#', '*'}.
*
* @author: Lily Davoren
* @version: September 12, 2024
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main() {
  char buff[32];
  const char *random_chars = "@!#*";
  printf("Enter a word: ");
  scanf("%s", buff);
  printf("OMG! ");
  
  // Loops through the entered exclamation and replaces any lowercase letters 
  // with one of the random characters stored in the array above.
  for (int i = 0; i < strlen(buff); i++) {
    if (buff[i] > 96 && buff[i] < 123) {
      int random_index = rand() % 4;
      char random_char = random_chars[random_index];
      buff[i] = random_char;
    }
  }
  printf("%s\n", buff);
  return 0;
}