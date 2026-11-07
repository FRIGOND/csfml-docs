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
sfSoundBuffer *buf = sfSoundBuffer_createFromFile("shot.ogg");
if (buf == NULL)
    return 84;
sfSound *snd = sfSound_create(buf);             /* NULL interdit : assert */
sfSound_play(snd);
sfMusic *mus = sfMusic_createFromFile("theme.ogg");
if (mus == NULL)
    return 84;
sfMusic_setLooping(mus, true);
sfMusic_play(mus);

/* ---- FIN EXTRAIT ---- */
return 0;
}
