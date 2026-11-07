/* wrapper généré — l'extrait est copié tel quel entre les marqueurs */
#include <SFML/Graphics.h>
#include <SFML/Audio.h>
#include <stdbool.h>
extern sfRenderWindow *win; extern sfEvent event; extern sfSprite *spr;
extern sfBool paused; extern const sfTexture *tileset;
void shoot(int x, int y); void resize(unsigned w, unsigned h);
void zoom(int d); void start(void); void hit(void);
int block(void)
{
/* ---- DEBUT EXTRAIT ---- */
/* AVANT (2.6.1) */
sfImage *img = sfImage_create(64, 64);
sfImage_setPixel(img, 10, 20, sfRed);
sfTexture *t = sfTexture_create(64, 64);
sfTexture_updateFromImage(t, img, 0, 0);
sfRenderWindow_setIcon(win, 64, 64, sfImage_getPixelsPtr(img));

/* ---- FIN EXTRAIT ---- */
return 0;
}
