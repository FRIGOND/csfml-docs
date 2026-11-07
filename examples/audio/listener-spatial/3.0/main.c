#include <CSFML/Audio.h>
#include <stdio.h>
#include "test_mode.h"

static int check_listener(void)
{
    sfVector3f pos = {1.0f, 0.0f, 2.0f};
    sfVector3f dir = {0.0f, 0.0f, -1.0f};
    sfVector3f got;

    sfListener_setGlobalVolume(50.0f);
    sfListener_setPosition(pos);
    sfListener_setDirection(dir);
    got = sfListener_getPosition();
    printf("auditeur : (%.1f, %.1f, %.1f), volume %.0f\n",
        got.x, got.y, got.z, sfListener_getGlobalVolume());
    return !(got.x == 1.0f && got.z == 2.0f);
}

static int check_sound(sfSound *sound)
{
    sfVector3f pos = {5.0f, 0.0f, 0.0f};
    sfVector3f got;

    sfSound_setRelativeToListener(sound, false);
    sfSound_setMinDistance(sound, 2.0f);
    sfSound_setAttenuation(sound, 1.0f);
    sfSound_setPosition(sound, pos);
    got = sfSound_getPosition(sound);
    printf("son : (%.1f, %.1f, %.1f), distance min %.1f\n",
        got.x, got.y, got.z, sfSound_getMinDistance(sound));
    sfSound_play(sound);
    sfSleep(sfMilliseconds(50));
    sfSound_stop(sound);
    /* Sans peripherique audio (2.6), les getters renvoient 0 : pas de test. */
    return 0;
}

int main(void)
{
    char path[256];
    sfSoundBuffer *buffer = sfSoundBuffer_createFromFile(
        example_asset(path, sizeof(path), "doodle_pop.ogg"));
    sfSound *sound = buffer ? sfSound_create(buffer) : NULL;
    int status = 1;

    if (sound != NULL)
        status = check_listener() | check_sound(sound);
    sfSound_destroy(sound);
    sfSoundBuffer_destroy(buffer);
    return status;
}
