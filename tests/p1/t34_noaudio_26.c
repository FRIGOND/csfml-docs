#include <SFML/Audio.h>
#include <stdio.h>
int main(void){
  setvbuf(stdout,NULL,_IONBF,0);
  sfMusic *m=sfMusic_createFromFile("doodle_pop.ogg"); printf("music=%p\n",(void*)m);
  if(m){ sfMusic_play(m); sfSleep(sfMilliseconds(500)); printf("status=%d\n",(int)sfMusic_getStatus(m)); sfMusic_destroy(m);}
  sfSoundBuffer *b=sfSoundBuffer_createFromFile("doodle_pop.ogg"); printf("buffer=%p\n",(void*)b);
  if(b){ sfSound *s=sfSound_create(); sfSound_setBuffer(s,b); sfSound_play(s); sfSleep(sfMilliseconds(300)); sfSound_destroy(s); sfSoundBuffer_destroy(b);}
  puts("fin normale"); return 0;}
