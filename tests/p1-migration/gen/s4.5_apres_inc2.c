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
sfFont *font = sfFont_createFromFile("arial.ttf");
if (font == NULL)
    return 84;
sfText *txt = sfText_create(font);              /* NULL interdit : assert */
sfText_setString(txt, "Score: 0");
sfText_setFillColor(txt, sfWhite);

/* ---- FIN EXTRAIT ---- */
return 0;
}
