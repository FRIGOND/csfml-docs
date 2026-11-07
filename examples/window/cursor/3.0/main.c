#include <CSFML/Graphics.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "test_mode.h"

static sfCursor *make_square_cursor(void)
{
    uint8_t pixels[16 * 16 * 4];

    memset(pixels, 255, sizeof(pixels));
    for (size_t i = 0; i < 16 * 16; i++) {
        pixels[i * 4 + 1] = 0;
        pixels[i * 4 + 2] = 0;
    }
    return sfCursor_createFromPixels(pixels, (sfVector2u){16, 16},
        (sfVector2u){8, 8});
}

static int make_cursors(sfCursor *cursors[3])
{
    cursors[0] = sfCursor_createFromSystem(sfCursorHand);
    cursors[1] = sfCursor_createFromSystem(sfCursorCross);
    cursors[2] = make_square_cursor();
    return cursors[0] && cursors[1] && cursors[2];
}

static void run(sfRenderWindow *window, sfCursor *cursors[3])
{
    sfEvent event;
    int frames = example_frames();
    int current = 0;

    sfRenderWindow_setFramerateLimit(window, 60);
    sfRenderWindow_setMouseCursor(window, cursors[current]);
    while (sfRenderWindow_isOpen(window) && frames != 0) {
        while (sfRenderWindow_pollEvent(window, &event)) {
            if (event.type == sfEvtClosed)
                sfRenderWindow_close(window);
            if (event.type == sfEvtMouseButtonPressed) {
                current = (current + 1) % 3;
                sfRenderWindow_setMouseCursor(window, cursors[current]);
            }
        }
        sfRenderWindow_clear(window, sfBlack);
        sfRenderWindow_display(window);
        if (frames > 0)
            frames--;
    }
}

int main(void)
{
    sfVideoMode mode = {{800, 600}, 32};
    sfRenderWindow *window = sfRenderWindow_create(mode, "Curseur",
        sfTitlebar | sfClose, sfWindowed, NULL);
    sfCursor *cursors[3] = {NULL, NULL, NULL};
    int ok = window != NULL && make_cursors(cursors);

    if (ok)
        run(window, cursors);
    if (window != NULL)
        sfRenderWindow_destroy(window);
    for (int i = 0; i < 3; i++)
        if (cursors[i] != NULL)
            sfCursor_destroy(cursors[i]);
    return !ok;
}
