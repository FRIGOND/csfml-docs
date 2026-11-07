#include <CSFML/Audio.h>
#include <stdio.h>
#include "test_mode.h"

int main(void)
{
    char path[256];
    sfSoundBuffer *buffer = sfSoundBuffer_createFromFile(
        example_asset(path, sizeof(path), "doodle_pop.ogg"));
    sfMusic *music = sfMusic_createFromFile(path);

    printf("buffer = %s, music = %s\n", buffer ? "cree" : "NULL",
        music ? "cree" : "NULL");
    if (music != NULL) {
        sfMusic_play(music);
        printf("statut apres play = %d (0 arret, 2 lecture)\n",
            (int)sfMusic_getStatus(music));
    }
    printf("enregistrement possible : %d\n", sfSoundRecorder_isAvailable());
    sfMusic_destroy(music);
    sfSoundBuffer_destroy(buffer);
    return 0;
}
