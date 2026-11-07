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
sfTexture *tex = sfTexture_createFromFile("duck.png", NULL);
if (tex == NULL)
    return 84;
sfSprite *spr = sfSprite_create(tex);          /* NULL interdit : assert */
sfIntRect rect = {{0, 0}, {110, 110}};
rect.position.x += 110;
sfSprite_setTextureRect(spr, rect);

/* ---- FIN EXTRAIT ---- */
return 0;
}
