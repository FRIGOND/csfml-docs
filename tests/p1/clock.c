#include <CSFML/System.h>
#include <stdio.h>
int main(void){
  sfClock *c=sfClock_create();
  sfSleep(sfMilliseconds(100));
  printf("%.2f\n",sfTime_asSeconds(sfClock_getElapsedTime(c)));
  sfClock_destroy(c); return 0;}
