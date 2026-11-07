#include <CSFML/Audio.h>
#include <stdio.h>
#include "test_mode.h"

static void show(const char *step, const sfMusic *music)
{
    printf("%-8s statut=%d position=%d ms volume=%.0f\n", step,
        (int)sfMusic_getStatus(music),
        sfTime_asMilliseconds(sfMusic_getPlayingOffset(music)),
        sfMusic_getVolume(music));
}

int main(void)
{
    char path[256];
    sfMusic *music = sfMusic_createFromFile(
        example_asset(path, sizeof(path), "doodle_pop.ogg"));

    if (music == NULL)
        return 1;
    printf("duree = %d ms\n",
        sfTime_asMilliseconds(sfMusic_getDuration(music)));
    sfMusic_setVolume(music, 40.0f);
    sfMusic_setLooping(music, true);
    sfMusic_play(music);
    sfSleep(sfMilliseconds(100));
    show("play", music);
    sfMusic_pause(music);
    show("pause", music);
    sfMusic_setPlayingOffset(music, sfMilliseconds(200));
    show("offset", music);
    sfMusic_stop(music);
    show("stop", music);
    printf("boucle = %d\n", sfMusic_isLooping(music));
    sfMusic_destroy(music);
    return 0;
}
