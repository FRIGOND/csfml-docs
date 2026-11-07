#include <CSFML/Audio.h>
#include <stdio.h>
#include "test_mode.h"

/* Attend la fin du son, au maximum max_ms millisecondes. */
static void wait_end(const sfSound *sound, int max_ms)
{
    int waited = 0;

    while (sfSound_getStatus(sound) == sfPlaying && waited < max_ms) {
        sfSleep(sfMilliseconds(10));
        waited += 10;
    }
    printf("attendu %d ms, statut = %d\n", waited,
        (int)sfSound_getStatus(sound));
}

int main(void)
{
    char path[256];
    sfSoundBuffer *buffer = sfSoundBuffer_createFromFile(
        example_asset(path, sizeof(path), "doodle_pop.ogg"));
    sfSound *sound = NULL;

    if (buffer == NULL)
        return 1;
    sound = sfSound_create(buffer);
    if (sound == NULL) {
        sfSoundBuffer_destroy(buffer);
        return 1;
    }
    printf("duree = %d ms, %u Hz, %u canal(aux)\n",
        sfTime_asMilliseconds(sfSoundBuffer_getDuration(buffer)),
        sfSoundBuffer_getSampleRate(buffer),
        sfSoundBuffer_getChannelCount(buffer));
    sfSound_play(sound);
    wait_end(sound, 300);
    sfSound_stop(sound);
    sfSound_destroy(sound);
    sfSoundBuffer_destroy(buffer);
    return 0;
}
