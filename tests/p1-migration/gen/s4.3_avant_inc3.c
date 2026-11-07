/* wrapper généré — l'extrait est copié tel quel entre les marqueurs */
#include <CSFML/Graphics.h>
#include <CSFML/Audio.h>
#include <stdbool.h>
extern sfRenderWindow *win; extern sfEvent event; extern sfSprite *spr;
extern sfBool paused; extern const sfTexture *tileset;
void shoot(int x, int y); void resize(unsigned w, unsigned h);
void zoom(int d); void start(void); void hit(void);
int block(void)
{
/* ---- DEBUT EXTRAIT ---- */
/* AVANT (2.6.1) */
sfTexture *tex = sfTexture_createFromFile("duck.png", NULL);
sfSprite *spr = sfSprite_create();
sfSprite_setTexture(spr, tex, sfTrue);
sfIntRect rect = {0, 0, 110, 110};
rect.left += 110;
sfSprite_setTextureRect(spr, rect);

/* ---- FIN EXTRAIT ---- */
return 0;
}
