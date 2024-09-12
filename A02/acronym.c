#include <stdio.h>
#include <string.h>

int main() {
  char phrase[1024];
  printf("Enter a phrase: ");
  scanf("%[^\n]%*c", phrase);
  printf("Your acronym is: ");
  for (int i = 0; i < strlen(phrase); i++) {
    if (phrase[i] > 64 && phrase[i] < 91) {
      printf("%c", phrase[i]);
    }
  }
  printf("\n");
  return 0;
}
