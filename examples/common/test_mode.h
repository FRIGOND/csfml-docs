/* Mode test des exemples : EXAMPLE_FRAMES=N ferme l'exemple après N images.
 * Sans la variable, l'exemple reste interactif. Aucun rapport avec CSFML. */
#ifndef TEST_MODE_H
#define TEST_MODE_H
#include <stdio.h>
#include <stdlib.h>

/* Renvoie le nombre d'images à afficher, ou -1 (illimité). */
static inline int example_frames(void)
{
    const char *v = getenv("EXAMPLE_FRAMES");

    return v ? atoi(v) : -1;
}

/* Construit le chemin d'un asset : $EXAMPLE_ASSETS/<nom>, ou ../../assets/<nom>
 * (exemple lancé depuis son dossier examples/<module>/<nom>/). */
static inline const char *example_asset(char *buf, size_t size, const char *name)
{
    const char *dir = getenv("EXAMPLE_ASSETS");

    snprintf(buf, size, "%s/%s", dir ? dir : "../../assets", name);
    return buf;
}
#endif
