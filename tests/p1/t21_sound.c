#include <CSFML/Audio.h>
#include <stdio.h>
int main(void){
  sfSoundBuffer *b=sfSoundBuffer_createFromFile("doodle_pop.ogg"); if(!b){puts("no buffer");return 1;}
  sfSound *s=sfSound_create(b);
  printf("sfSound_getBuffer==b: %d\n",sfSound_getBuffer(s)==b);
  sfSoundBuffer_destroy(b);                     /* BUG volontaire */
  sfSound_play(s); sfSleep(sfMilliseconds(200));
  sfSound_destroy(s); return 0;}
