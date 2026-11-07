/* wrapper généré — l'extrait est copié tel quel entre les marqueurs */
#include <CSFML/Graphics.h>
#include <CSFML/Audio.h>
#include <stdbool.h>
extern sfRenderWindow *win; extern sfEvent event; extern sfSprite *spr;
extern bool paused; extern const sfTexture *tileset;
void shoot(int x, int y); void resize(unsigned w, unsigned h);
void zoom(float d); void start(void); void hit(void);
int block(void)
{
/* ---- DEBUT EXTRAIT ---- */
/* APRÈS (3.0.0) */
sfImage *img = sfImage_create((sfVector2u){64, 64});
sfImage_setPixel(img, (sfVector2u){10, 20}, sfRed);
sfTexture *t = sfTexture_create((sfVector2u){64, 64});
sfTexture_updateFromImage(t, img, (sfVector2u){0, 0});
sfRenderWindow_setIcon(win, (sfVector2u){64, 64}, sfImage_getPixelsPtr(img));

/* ---- FIN EXTRAIT ---- */
return 0;
}
