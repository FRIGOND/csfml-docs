#include <CSFML/Audio.h>
#include <stdio.h>
#include <stdlib.h>
int main(void){
  FILE *f=fopen("doodle_pop.ogg","rb"); if(!f) return 1;
  fseek(f,0,SEEK_END); long n=ftell(f); rewind(f);
  unsigned char *buf=malloc(n); if(fread(buf,1,n,f)!=(size_t)n) return 2; fclose(f);
  sfMusic *m=sfMusic_createFromMemory(buf,(size_t)n); if(!m){puts("create failed");return 3;}
  free(buf);                                               /* buffer libéré avant lecture */
  sfMusic_play(m); sfSleep(sfSeconds(1));
  printf("status=%d offset=%.2fs\n",(int)sfMusic_getStatus(m),sfTime_asSeconds(sfMusic_getPlayingOffset(m)));
  sfMusic_destroy(m); return 0;}
