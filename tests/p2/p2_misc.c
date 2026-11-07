/* Tests P2 divers (CSFML 3.0.0). Sortie texte, une ligne par test. */
#include <CSFML/Graphics.h>
#include <CSFML/Network.h>
#include <CSFML/System/Alloc.h>
#include <stdio.h>
#include <string.h>
#include <uchar.h>
int main(void){
  setvbuf(stdout,NULL,_IONBF,0);
  /* 67 : sfImage_saveToMemory remplit un sfBuffer créé par l'appelant */
  sfImage *img=sfImage_createFromColor((sfVector2u){2,2},sfRed);
  sfBuffer *buf=sfBuffer_create();
  bool ok=sfImage_saveToMemory(img,buf,"png");
  printf("67 saveToMemory ok=%d size=%zu\n",ok,sfBuffer_getSize(buf));
  sfBuffer_destroy(buf); sfImage_destroy(img);
  /* 78/79 : adresses IP */
  sfIpAddress a=sfIpAddress_fromBytes(255,255,255,255);
  printf("78 sizeof(sfIpAddress.address)=%zu\n",sizeof a.address);
  char out[64]; sfIpAddress_toString(a,out); printf("78 toString=%s\n",out);
  sfIpAddress lh=sfIpAddress_fromString("localhost"); sfIpAddress_toString(lh,out); printf("79 fromString(localhost)=%s\n",out);
  sfIpAddress bad=sfIpAddress_fromString("pas.une.adresse.invalide.zz"); sfIpAddress_toString(bad,out); printf("79 fromString(invalide)=%s\n",out);
  /* 87 : sfClock_reset / restart */
  sfClock *c=sfClock_create(); sfSleep(sfMilliseconds(100));
  sfTime t1=sfClock_restart(c); sfSleep(sfMilliseconds(100));
  printf("87 restart returned %.2f s, then elapsed %.2f s\n",sfTime_asSeconds(t1),sfTime_asSeconds(sfClock_getElapsedTime(c)));
  sfClock_destroy(c);
  /* 72 : sfText_getString renvoie un pointeur vers le texte interne */
  sfFont *f=sfFont_createFromFile("tuffy.ttf"); sfText *tx=sfText_create(f);
  sfText_setString(tx,"abc"); const char *p1=sfText_getString(tx); char c1[8]; strncpy(c1,p1,7); c1[7]=0;
  sfText_setString(tx,"xyz"); const char *p2=sfText_getString(tx);
  printf("72 getString p1==p2:%d, ancien contenu copié=%s, nouveau=%s\n",p1==p2,c1,p2);
  /* 81 : littéral U"..." accepté sans cast */
  sfText_setUnicodeString(tx,U"été"); const sfChar32 *u=sfText_getUnicodeString(tx); int n=0; while(u[n]) n++;
  printf("81 setUnicodeString(U\"été\") -> %d points de code\n",n);
  sfText_destroy(tx); sfFont_destroy(f);
  /* 82 : sfFree via Alloc.h */
  sfFree(NULL); puts("82 sfFree(NULL) via <CSFML/System/Alloc.h> OK");
  /* 101 : EXIT_FAILURE sans stdlib.h ? (voir t101.c) */
  return 0;}
