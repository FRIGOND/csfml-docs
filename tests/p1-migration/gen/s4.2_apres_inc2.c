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
sfEvent event;
while (sfRenderWindow_pollEvent(win, &event)) {
    if (event.type == sfEvtClosed)
        sfRenderWindow_close(win);
    if (event.type == sfEvtMouseButtonPressed && event.mouseButton.button == sfMouseLeft)
        shoot(event.mouseButton.position.x, event.mouseButton.position.y);
    if (event.type == sfEvtResized)
        resize(event.size.size.x, event.size.size.y);
    if (event.type == sfEvtFocusLost)
        paused = true;
    if (event.type == sfEvtKeyPressed && event.key.code == sfKeyEnter)
        start();
    if (event.type == sfEvtMouseWheelScrolled
        && event.mouseWheelScroll.wheel == sfMouseVerticalWheel)
        zoom(event.mouseWheelScroll.delta);   /* float */
}
/* attente bloquante : sfRenderWindow_waitEvent(win, sfTime_Zero, &event) */

/* ---- FIN EXTRAIT ---- */
return 0;
}
