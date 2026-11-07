/* Test 17 : sfRenderWindow_waitEvent(w, timeout, &e) revient-il après le timeout ? */
#include <CSFML/Graphics.h>
#include <stdio.h>
int main(void){
  setvbuf(stdout,NULL,_IONBF,0);
  sfVideoMode m={{200,100},32};
  sfRenderWindow *w=sfRenderWindow_create(m,"t",sfTitlebar|sfClose,sfWindowed,NULL); if(!w) return 1;
  sfEvent e; sfClock *c=sfClock_create(); sfSleep(sfMilliseconds(300));
  while(sfRenderWindow_pollEvent(w,&e)) printf("creation event type=%d\n",(int)e.type);
  puts("file vidée, appel waitEvent(200 ms)...");
  sfClock_restart(c);
  bool got=sfRenderWindow_waitEvent(w,sfMilliseconds(200),&e);
  printf("waitEvent(200ms) -> got=%d, elapsed=%.0f ms\n",got,sfTime_asSeconds(sfClock_getElapsedTime(c))*1000);
  sfClock_destroy(c); sfRenderWindow_destroy(w); return 0;}
