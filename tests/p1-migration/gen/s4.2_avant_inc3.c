/* wrapper généré — l'extrait est copié tel quel entre les marqueurs */
#include <CSFML/Graphics.h>
#include <CSFML/Audio.h>
#include <stdbool.h>
extern sfRenderWindow *win; extern sfEvent event; extern sfSprite *spr;
extern int paused; extern const sfTexture *tileset;
void shoot(int x, int y); void resize(unsigned w, unsigned h);
void zoom(int d); void start(void); void hit(void);
int block(void)
{
/* ---- DEBUT EXTRAIT ---- */
/* AVANT (2.6.1) */
sfEvent event;
while (sfRenderWindow_pollEvent(win, &event)) {
    if (event.type == sfEvtClosed)
        sfRenderWindow_close(win);
    if (event.type == sfEvtMouseButtonPressed && event.mouseButton.button == sfMouseLeft)
        shoot(event.mouseButton.x, event.mouseButton.y);
    if (event.type == sfEvtResized)
        resize(event.size.width, event.size.height);
    if (event.type == sfEvtLostFocus)
        paused = sfTrue;
    if (event.type == sfEvtKeyPressed && event.key.code == sfKeyReturn)
        start();
    if (event.type == sfEvtMouseWheelMoved)
        zoom(event.mouseWheel.delta);
}

/* ---- FIN EXTRAIT ---- */
return 0;
}
