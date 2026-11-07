#include <CSFML/Graphics.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include "test_mode.h"

static const uint32_t ETE[] = {0xE9, 't', 0xE9, 0};

static void center_text(sfText *text, sfVector2f pos)
{
    sfFloatRect b = sfText_getLocalBounds(text);

    sfText_setOrigin(text, (sfVector2f){b.position.x + b.size.x / 2,
        b.position.y + b.size.y / 2});
    sfText_setPosition(text, pos);
}

static int check_center(const sfText *text, sfVector2f want)
{
    sfFloatRect g = sfText_getGlobalBounds(text);
    float dx = g.position.x + g.size.x / 2 - want.x;
    float dy = g.position.y + g.size.y / 2 - want.y;

    printf("texte %.0fx%.0f, ecart au centre voulu : %.2f, %.2f\n",
        g.size.x, g.size.y, dx, dy);
    return fabsf(dx) > 1 || fabsf(dy) > 1;
}

static void setup(sfText *hello, sfText *ete)
{
    sfText_setString(hello, "Hello CSFML");
    sfText_setCharacterSize(hello, 64);
    sfText_setFillColor(hello, sfYellow);
    sfText_setUnicodeString(ete, ETE);
    sfText_setCharacterSize(ete, 64);
    center_text(hello, (sfVector2f){400, 200});
    center_text(ete, (sfVector2f){400, 400});
}

static int run(sfRenderWindow *window, sfText *hello, sfText *ete)
{
    sfEvent event;
    int frames = example_frames();

    while (sfRenderWindow_isOpen(window) && frames != 0) {
        while (sfRenderWindow_pollEvent(window, &event))
            if (event.type == sfEvtClosed)
                sfRenderWindow_close(window);
        sfRenderWindow_clear(window, sfBlack);
        sfRenderWindow_drawText(window, hello, NULL);
        sfRenderWindow_drawText(window, ete, NULL);
        sfRenderWindow_display(window);
        if (frames > 0)
            frames--;
    }
    if (example_frames() < 0)
        return 0;
    return check_center(hello, (sfVector2f){400, 200})
        || check_center(ete, (sfVector2f){400, 400})
        || sfText_getUnicodeString(ete)[0] != 0xE9;
}

int main(void)
{
    sfVideoMode mode = {{800, 600}, 32};
    sfRenderWindow *window = sfRenderWindow_create(mode, "Texte",
        sfTitlebar | sfClose, sfWindowed, NULL);
    char path[256];
    sfFont *font = sfFont_createFromFile(
        example_asset(path, sizeof(path), "tuffy.ttf"));
    sfText *hello = font ? sfText_create(font) : NULL;
    sfText *ete = font ? sfText_create(font) : NULL;
    int status = 1;

    if (window != NULL && hello != NULL && ete != NULL) {
        setup(hello, ete);
        status = run(window, hello, ete);
    }
    sfText_destroy(ete);
    sfText_destroy(hello);
    sfFont_destroy(font);
    sfRenderWindow_destroy(window);
    return status;
}
