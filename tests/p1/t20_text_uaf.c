#include <CSFML/Graphics.h>
#include <stdio.h>
int main(void){
  sfFont *f=sfFont_createFromFile("tuffy.ttf"); if(!f) return 1;
  sfText *t=sfText_create(f); sfText_setString(t,"Hello");
  printf("sfText_getFont==f: %d\n",sfText_getFont(t)==f);
  sfFont_destroy(f);                            /* BUG volontaire */
  sfFloatRect r=sfText_getLocalBounds(t);       /* use-after-free attendu */
  printf("bounds w=%.1f\n",r.size.x);
  sfText_destroy(t); return 0;}
