#include <SFML/Graphics.h>
#include <stdio.h>
#include "test_mode.h"

static void follow_mouse(const sfRenderWindow *window, sfCircleShape *dot,
    sfVector2i *last)
{
    sfVector2i pos = sfMouse_getPositionRenderWindow(window);

    if (pos.x != last->x || pos.y != last->y)
        printf("souris : (%d, %d)\n", pos.x, pos.y);
    *last = pos;
    sfCircleShape_setPosition(dot, (sfVector2f){pos.x - 15, pos.y - 15});
}

static void run(sfRenderWindow *window, sfCircleShape *dot)
{
    sfVector2i last = {-1, -1};
    sfEvent event;
    int frames = example_frames();

    sfRenderWindow_setFramerateLimit(window, 60);
    while (sfRenderWindow_isOpen(window) && frames != 0) {
        while (sfRenderWindow_pollEvent(window, &event))
            if (event.type == sfEvtClosed)
                sfRenderWindow_close(window);
        follow_mouse(window, dot, &last);
        sfRenderWindow_clear(window, sfBlack);
        sfRenderWindow_drawCircleShape(window, dot, NULL);
        sfRenderWindow_display(window);
        if (frames > 0)
            frames--;
    }
}

int main(void)
{
    sfVideoMode mode = {800, 600, 32};
    sfRenderWindow *window = sfRenderWindow_create(mode, "Souris",
        sfTitlebar | sfClose, NULL);
    sfCircleShape *dot = sfCircleShape_create();

    if (window == NULL || dot == NULL)
        return 1;
    sfCircleShape_setRadius(dot, 15);
    sfCircleShape_setFillColor(dot, sfGreen);
    run(window, dot);
    sfCircleShape_destroy(dot);
    sfRenderWindow_destroy(window);
    return 0;
}
