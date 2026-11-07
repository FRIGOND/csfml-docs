#include <CSFML/Graphics.h>
#include <stdio.h>
int main(void){
  sfVideoMode m={{200,100},32};
  sfRenderWindow *w=sfRenderWindow_create(m,"t",sfTitlebar|sfClose,sfWindowed,NULL);
  printf("window=%p\n",(void*)w); if(w) sfRenderWindow_destroy(w); return w?0:3;}
