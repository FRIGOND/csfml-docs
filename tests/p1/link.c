#include <CSFML/Graphics.h>
int main(int argc,char**argv){(void)argv;
  if(argc>5){sfVideoMode m={{1,1},32};sfRenderWindow_create(m,"x",sfDefaultStyle,sfWindowed,NULL);}
  sfClock *c=sfClock_create(); sfClock_destroy(c); return 0;}
