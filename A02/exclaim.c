#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main() {
  char buff[32];
  const char *random_chars = "@!#*";
  printf("Enter a word: ");
  scanf("%s", buff);
  printf("OMG! ");
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