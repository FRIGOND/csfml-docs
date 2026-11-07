#include <CSFML/Window.h>
#include <stdio.h>
#include <stdlib.h>
int main(void){
  char *d=sfKeyboard_getDescription(sfScanA);
  printf("description(sfScanA)=%s\n",d?d:"(null)");
  free(d); return 0;}
