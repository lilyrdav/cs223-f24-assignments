/*----------------------------------------------
 * Author: 
 * Date: 
 * Description
 ---------------------------------------------*/
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
