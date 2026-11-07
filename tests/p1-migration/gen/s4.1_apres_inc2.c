/* wrapper généré — l'extrait est copié tel quel entre les marqueurs */
#include <SFML/Graphics.h>
#include <stdbool.h>
extern sfRenderWindow *win; extern sfEvent event; extern sfSprite *spr;
extern bool paused; extern const sfTexture *tileset;
void shoot(int x, int y); void resize(unsigned w, unsigned h);
void zoom(float d); void start(void); void hit(void);
int block(void)
{
/* ---- DEBUT EXTRAIT ---- */
/* APRÈS (3.0.0) */
sfVideoMode mode = {{800, 600}, 32};
sfRenderWindow *win = sfRenderWindow_create(mode, "my_hunter", sfResize | sfClose, sfWindowed, NULL);
sfRenderWindow *full = sfRenderWindow_create(sfVideoMode_getDesktopMode(), "full", sfDefaultStyle, sfFullscreen, NULL);
/* mode.size.x, mode.size.y */

/* ---- FIN EXTRAIT ---- */
return 0;
}
