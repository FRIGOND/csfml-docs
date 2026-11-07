#include <CSFML/Graphics.h>
#include <stdio.h>
int main(void){
  sfVideoMode m={{200,100},32};
  sfRenderWindow *w=sfRenderWindow_create(m,"t",sfTitlebar|sfClose,sfWindowed,NULL); if(!w) return 1;
  sfTexture *tex=sfTexture_createFromFile("sfml_logo.png",NULL); if(!tex) return 2;
  sfSprite *s=sfSprite_create(tex);
  printf("sfSprite_getTexture==tex: %d\n",sfSprite_getTexture(s)==tex);
  sfTexture_destroy(tex);                       /* BUG volontaire */
  sfRenderWindow_clear(w,sfBlack);
  sfRenderWindow_drawSprite(w,s,NULL);          /* use-after-free attendu */
  sfRenderWindow_display(w);
  sfSprite_destroy(s); sfRenderWindow_destroy(w); return 0;}
