/**
* The main driver program for A03 slow.c.
*
* This program turns a phrase into ent-speech. The program asks for the pause length and a phrase to change and outputs
* an ent-phrase. Ent-phrases contain '.' between each letter in the input.
*
* @author: Lily Davoren
* @version: September 19, 2024
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main() {
  char speech[32];
  char* pauseSpace;
  int pauses;

  printf("Pause length: ");
  scanf("%d", &pauses);
  printf("Text: ");
  scanf("%s", speech);

  for (int i = 0; i < strlen(speech); i++) {
    printf("%c", speech[i]);
    pauseSpace = (char*)malloc(pauses * sizeof(char));
    for (int j = 0; j < pauses; j++) {
      printf(".");
    }
    free(pauseSpace);
  }
  printf("\n");
  return 0;
}
