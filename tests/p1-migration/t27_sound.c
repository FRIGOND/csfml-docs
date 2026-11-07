/* Test 27 : son et musique (style APRÈS 3.0.0), ~30 frames d'attente */
#include <CSFML/Audio.h>
#include <CSFML/System.h>
#include <stdio.h>
int main(void)
{
    sfSoundBuffer *buf = sfSoundBuffer_createFromFile("../p1/doodle_pop.ogg");
    if (buf == NULL)
        return 84;
    sfSound *snd = sfSound_create(buf);
    sfSound_play(snd);
    sfMusic *mus = sfMusic_createFromFile("../p1/doodle_pop.ogg");
    if (mus == NULL)
        return 84;
    sfMusic_setLooping(mus, true);
    sfMusic_play(mus);
    for (int i = 0; i < 30; i++)
        sfSleep(sfMilliseconds(16));
    printf("status snd=%d mus=%d\n", (int)sfSound_getStatus(snd), (int)sfMusic_getStatus(mus));
    sfMusic_stop(mus);
    sfSound_stop(snd);
    sfMusic_destroy(mus);
    sfSound_destroy(snd);
    sfSoundBuffer_destroy(buf);
    return 0;
}
