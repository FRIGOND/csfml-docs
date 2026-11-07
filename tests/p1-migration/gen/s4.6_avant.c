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
sfSoundBuffer *buf = sfSoundBuffer_createFromFile("shot.ogg");
sfSound *snd = sfSound_create();
sfSound_setBuffer(snd, buf);
sfSound_play(snd);
sfMusic *mus = sfMusic_createFromFile("theme.ogg");
sfMusic_setLoop(mus, sfTrue);
sfMusic_play(mus);

/* ---- FIN EXTRAIT ---- */
return 0;
}
