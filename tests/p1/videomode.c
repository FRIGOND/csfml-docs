#include <CSFML/Window.h>
#include <stdio.h>
int main(void){sfVideoMode m = {800, 600, 32};
  printf("size=%u,%u bpp=%u\n",m.size.x,m.size.y,m.bitsPerPixel);return 0;}
