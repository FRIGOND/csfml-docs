/* wrapper généré — l'extrait est copié tel quel entre les marqueurs */
#include <SFML/Graphics.h>
#include <SFML/Audio.h>
#include <stdbool.h>
extern sfRenderWindow *win; extern sfEvent event; extern sfSprite *spr;
extern bool paused; extern const sfTexture *tileset;
void shoot(int x, int y); void resize(unsigned w, unsigned h);
void zoom(float d); void start(void); void hit(void);
int block(void)
{
/* ---- DEBUT EXTRAIT ---- */
/* APRÈS (3.0.0) */
sfFloatRect b = sfSprite_getGlobalBounds(spr);
sfVector2f p = sfRenderWindow_mapPixelToCoords(win, event.mouseButton.position, NULL);
if (sfFloatRect_contains(&b, p))
    hit();

/* ---- FIN EXTRAIT ---- */
return 0;
}
