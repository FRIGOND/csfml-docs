#include <SFML/Graphics.h>
#include <stdio.h>
#include "test_mode.h"

static sfVector2f read_direction(void)
{
    sfVector2f dir = {0, 0};

    if (sfKeyboard_isKeyPressed(sfKeyLeft))
        dir.x -= 1;
    if (sfKeyboard_isKeyPressed(sfKeyRight))
        dir.x += 1;
    if (sfKeyboard_isKeyPressed(sfKeyUp))
        dir.y -= 1;
    if (sfKeyboard_isKeyPressed(sfKeyDown))
        dir.y += 1;
    return dir;
}

static void move_square(sfRectangleShape *square, sfVector2f dir)
{
    sfVector2f pos = sfRectangleShape_getPosition(square);

    pos.x += dir.x * 4;
    pos.y += dir.y * 4;
    sfRectangleShape_setPosition(square, pos);
    if (dir.x != 0 || dir.y != 0)
        printf("fleches : direction (%.0f, %.0f)\n", dir.x, dir.y);
}

static void run(sfRenderWindow *window, sfRectangleShape *square)
{
    sfEvent event;
    int frames = example_frames();

    sfRenderWindow_setFramerateLimit(window, 60);
    while (sfRenderWindow_isOpen(window) && frames != 0) {
        while (sfRenderWindow_pollEvent(window, &event))
            if (event.type == sfEvtClosed)
                sfRenderWindow_close(window);
        move_square(square, read_direction());
        sfRenderWindow_clear(window, sfBlack);
        sfRenderWindow_drawRectangleShape(window, square, NULL);
        sfRenderWindow_display(window);
        if (frames > 0)
            frames--;
    }
}

int main(void)
{
    sfVideoMode mode = {800, 600, 32};
    sfRenderWindow *window = sfRenderWindow_create(mode, "Clavier",
        sfTitlebar | sfClose, NULL);
    sfRectangleShape *square = sfRectangleShape_create();

    if (window == NULL || square == NULL)
        return 1;
    sfRectangleShape_setSize(square, (sfVector2f){50, 50});
    sfRectangleShape_setPosition(square, (sfVector2f){375, 275});
    run(window, square);
    sfRectangleShape_destroy(square);
    sfRenderWindow_destroy(window);
    return 0;
}
