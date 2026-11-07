/* wrapper généré — l'extrait est copié tel quel entre les marqueurs */
#include <CSFML/Graphics.h>
#include <stdbool.h>
extern sfRenderWindow *win; extern sfEvent event; extern sfSprite *spr;
extern int paused; extern const sfTexture *tileset;
void shoot(int x, int y); void resize(unsigned w, unsigned h);
void zoom(int d); void start(void); void hit(void);
int block(void)
{
/* ---- DEBUT EXTRAIT ---- */
/* AVANT (2.6.1) */
sfVideoMode mode = {800, 600, 32};
sfRenderWindow *win = sfRenderWindow_create(mode, "my_hunter", sfResize | sfClose, NULL);
sfRenderWindow *full = sfRenderWindow_create(sfVideoMode_getDesktopMode(), "full", sfFullscreen, NULL);
/* mode.width, mode.height */

/* ---- FIN EXTRAIT ---- */
return 0;
}
