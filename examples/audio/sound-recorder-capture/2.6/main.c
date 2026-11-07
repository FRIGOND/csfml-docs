#include <SFML/Audio.h>
#include <stdio.h>

static int record(sfSoundBufferRecorder *recorder)
{
    const sfSoundBuffer *buffer = NULL;

    if (!sfSoundBufferRecorder_start(recorder, 44100)) {
        printf("start a echoue (pas de peripherique utilisable)\n");
        return 0;
    }
    sfSleep(sfMilliseconds(200));
    sfSoundBufferRecorder_stop(recorder);
    buffer = sfSoundBufferRecorder_getBuffer(recorder);
    printf("%llu echantillons captures a %u Hz\n",
        (unsigned long long)sfSoundBuffer_getSampleCount(buffer),
        sfSoundBuffer_getSampleRate(buffer));
    return 0;
}

int main(void)
{
    sfSoundBufferRecorder *recorder = NULL;
    int status;

    if (!sfSoundRecorder_isAvailable()) {
        printf("Aucun peripherique de capture : rien a enregistrer.\n");
        return 0;
    }
    recorder = sfSoundBufferRecorder_create();
    if (recorder == NULL)
        return 1;
    status = record(recorder);
    sfSoundBufferRecorder_destroy(recorder);
    return status;
}
