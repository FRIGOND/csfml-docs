#include <CSFML/Graphics.h>
#include <stdio.h>
#include <stdlib.h>
int main(void){
  FILE *f=fopen("tuffy.ttf","rb"); if(!f) return 1;
  fseek(f,0,SEEK_END); long n=ftell(f); rewind(f);
  unsigned char *buf=malloc(n); if(fread(buf,1,n,f)!=(size_t)n) return 2; fclose(f);
  sfFont *font=sfFont_createFromMemory(buf,(size_t)n); if(!font) return 3;
  sfGlyph g0=sfFont_getGlyph(font,'A',30,false,0.f);       /* avant free */
  free(buf);                                               /* le buffer est libéré */
  sfGlyph g1=sfFont_getGlyph(font,'Z',30,false,0.f);       /* glyphe jamais demandé */
  sfGlyph g2=sfFont_getGlyph(font,0x3B1,30,false,0.f);     /* encore un autre */
  printf("A advance=%.1f Z advance=%.1f alpha advance=%.1f\n",g0.advance,g1.advance,g2.advance);
  sfFont_destroy(font); return 0;}
