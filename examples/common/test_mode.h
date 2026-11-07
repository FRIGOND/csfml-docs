/* Mode test des exemples : EXAMPLE_FRAMES=N ferme l'exemple après N images.
 * Sans la variable, l'exemple reste interactif. Aucun rapport avec CSFML. */
#ifndef TEST_MODE_H
#define TEST_MODE_H
#include <stdlib.h>

/* Renvoie le nombre d'images à afficher, ou -1 (illimité). */
static inline int example_frames(void)
{
    const char *v = getenv("EXAMPLE_FRAMES");

    return v ? atoi(v) : -1;
}
#endif
