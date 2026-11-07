#include <SFML/Graphics.h>
#include <stdio.h>
#include "test_mode.h"

static void print_event(const sfEvent *e)
{
    switch (e->type) {
    case sfEvtClosed:
        printf("Closed\n");
        break;
    case sfEvtKeyPressed:
        printf("KeyPressed code=%d shift=%d\n", (int)e->key.code,
            e->key.shift);
        break;
    case sfEvtMouseButtonPressed:
        printf("MouseButtonPressed button=%d position=(%d, %d)\n",
            (int)e->mouseButton.button, e->mouseButton.x,
            e->mouseButton.y);
        break;
    case sfEvtResized:
        printf("Resized size=%ux%u\n", e->size.width, e->size.height);
        break;
    default:
        break;
    }
}

static void run(sfRenderWindow *window)
{
    sfEvent event;
    int frames = example_frames();

    sfRenderWindow_setFramerateLimit(window, 60);
    while (sfRenderWindow_isOpen(window) && frames != 0) {
        while (sfRenderWindow_pollEvent(window, &event)) {
            print_event(&event);
            if (event.type == sfEvtClosed)
                sfRenderWindow_close(window);
        }
        sfRenderWindow_clear(window, sfBlack);
        sfRenderWindow_display(window);
        if (frames > 0)
            frames--;
    }
}

int main(void)
{
    sfVideoMode mode = {800, 600, 32};
    sfRenderWindow *window = sfRenderWindow_create(mode, "Evenements",
        sfDefaultStyle, NULL);

    if (window == NULL)
        return 1;
    run(window);
    sfRenderWindow_destroy(window);
    return 0;
}
