#include <CSFML/Graphics.h>
#include <stdio.h>
#include "test_mode.h"

#define WIN_W 800
#define WIN_H 600

/* En mode test : la fenetre doit faire la taille demandee. */
static int check_size(const sfRenderWindow *window)
{
    sfVector2u size = sfRenderWindow_getSize(window);

    printf("taille de la fenetre : %ux%u\n", size.x, size.y);
    return size.x != WIN_W || size.y != WIN_H;
}

static void handle_events(sfRenderWindow *window)
{
    sfEvent event;

    while (sfRenderWindow_pollEvent(window, &event)) {
        if (event.type == sfEvtClosed)
            sfRenderWindow_close(window);
        if (event.type == sfEvtKeyPressed && event.key.code == sfKeyEscape)
            sfRenderWindow_close(window);
    }
}

static int run(sfRenderWindow *window)
{
    int frames = example_frames();

    while (sfRenderWindow_isOpen(window) && frames != 0) {
        handle_events(window);
        sfRenderWindow_clear(window, sfBlack);
        sfRenderWindow_display(window);
        if (frames > 0)
            frames--;
    }
    return example_frames() >= 0 ? check_size(window) : 0;
}

int main(void)
{
    sfVideoMode mode = {{WIN_W, WIN_H}, 32};
    sfRenderWindow *window = sfRenderWindow_create(mode, "Casse-briques",
        sfTitlebar | sfClose, sfWindowed, NULL);
    int status = 1;

    if (window != NULL) {
        sfRenderWindow_setFramerateLimit(window, 60);
        status = run(window);
        sfRenderWindow_destroy(window);
    }
    return status;
}
