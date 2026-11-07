/* wrapper généré — l'extrait est copié tel quel entre les marqueurs */
#include <SFML/Graphics.h>
#include <SFML/Audio.h>
#include <stdbool.h>
extern sfRenderWindow *win; extern sfEvent event; extern sfSprite *spr;
extern int paused; extern const sfTexture *tileset;
void shoot(int x, int y); void resize(unsigned w, unsigned h);
void zoom(int d); void start(void); void hit(void);
int block(void)
{
/* ---- DEBUT EXTRAIT ---- */
/* AVANT (2.6.1) */
sfFont *font = sfFont_createFromFile("arial.ttf");
sfText *txt = sfText_create();
sfText_setFont(txt, font);
sfText_setString(txt, "Score: 0");
sfText_setColor(txt, sfWhite);

/* ---- FIN EXTRAIT ---- */
return 0;
}
