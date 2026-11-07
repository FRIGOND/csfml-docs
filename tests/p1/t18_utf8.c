#include <CSFML/Graphics.h>
#include <stdio.h>
int main(void){
  sfFont *f=sfFont_createFromFile("tuffy.ttf"); if(!f) return 1;
  sfText *t=sfText_create(f);
  sfText_setString(t,"été");
  const sfChar32 *u=sfText_getUnicodeString(t); int n=0; while(u[n]) n++;
  printf("UTF-8 \"été\" (5 octets) -> %d code points:",n); for(int i=0;i<n;i++) printf(" U+%04X",(unsigned)u[i]); puts("");
  sfText_setUnicodeString(t,(const sfChar32[]){0xE9,0x74,0xE9,0});
  u=sfText_getUnicodeString(t); n=0; while(u[n]) n++; printf("setUnicodeString(U+E9,t,U+E9) -> %d code points\n",n);
  sfText_destroy(t); sfFont_destroy(f); return 0;}
