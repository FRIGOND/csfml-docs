#include <CSFML/Graphics.h>
#include <stdio.h>
#include "test_mode.h"

static void set_icon(sfRenderWindow *window)
{
    sfImage *icon = sfImage_createFromColor((sfVector2u){32, 32}, sfCyan);

    if (icon == NULL)
        return;
    sfRenderWindow_setIcon(window, sfImage_getSize(icon),
        sfImage_getPixelsPtr(icon));
    sfImage_destroy(icon);
}

static void update_title(sfRenderWindow *window, int tick)
{
    char title[64];

    snprintf(title, sizeof(title), "Reglages : image %d", tick);
    sfRenderWindow_setTitle(window, title);
}

static int report(const sfRenderWindow *window)
{
    sfVector2u size = sfRenderWindow_getSize(window);
    sfVector2i pos = sfRenderWindow_getPosition(window);

    printf("taille : %ux%u, position : (%d, %d)\n", size.x, size.y,
        pos.x, pos.y);
    return !(size.x == 500 && size.y == 400);
}

static void run(sfRenderWindow *window)
{
    sfEvent event;
    int frames = example_frames();
    int tick = 0;

    sfRenderWindow_setFramerateLimit(window, 60);
    while (sfRenderWindow_isOpen(window) && frames != 0) {
        while (sfRenderWindow_pollEvent(window, &event))
            if (event.type == sfEvtClosed)
                sfRenderWindow_close(window);
        update_title(window, tick++);
        sfRenderWindow_clear(window, sfBlack);
        sfRenderWindow_display(window);
        if (frames > 0)
            frames--;
    }
}

int main(void)
{
    sfVideoMode mode = {{640, 480}, 32};
    sfRenderWindow *window = sfRenderWindow_create(mode, "Titre initial",
        sfTitlebar | sfClose, sfWindowed, NULL);
    int status = 0;

    if (window == NULL)
        return 1;
    sfRenderWindow_setSize(window, (sfVector2u){500, 400});
    sfRenderWindow_setPosition(window, (sfVector2i){100, 100});
    set_icon(window);
    run(window);
    if (example_frames() >= 0)
        status = report(window);
    sfRenderWindow_destroy(window);
    return status;
}
