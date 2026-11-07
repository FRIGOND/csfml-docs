#include <SFML/Graphics.h>
#include <stdio.h>
#include "test_mode.h"

typedef struct {
    sfRectangleShape *red;
    sfRectangleShape *green;
} pair_t;

static void draw_pair(sfRenderWindow *window, const pair_t *p, float dx,
    sfBlendMode blend)
{
    sfRenderStates states = sfRenderStates_default();

    states.blendMode = blend;
    sfRectangleShape_setPosition(p->red, (sfVector2f){20 + dx, 50});
    sfRectangleShape_setPosition(p->green, (sfVector2f){70 + dx, 80});
    sfRenderWindow_drawRectangleShape(window, p->red, &states);
    sfRenderWindow_drawRectangleShape(window, p->green, &states);
}

static sfColor pixel_at(const sfRenderWindow *window, sfVector2u pos)
{
    sfTexture *tex = sfTexture_create(sfRenderWindow_getSize(window).x,
        sfRenderWindow_getSize(window).y);
    sfImage *img = NULL;
    sfColor px = {0, 0, 0, 0};

    if (tex == NULL)
        return px;
    sfTexture_updateFromRenderWindow(tex, window, 0, 0);
    img = sfTexture_copyToImage(tex);
    if (img != NULL)
        px = sfImage_getPixel(img, pos.x, pos.y);
    sfImage_destroy(img);
    sfTexture_destroy(tex);
    return px;
}

static int check(const sfRenderWindow *window)
{
    sfColor alpha = pixel_at(window, (sfVector2u){95, 115});
    sfColor add = pixel_at(window, (sfVector2u){395, 115});

    printf("alpha : %d,%d,%d (attendu 0,255,0)\n", alpha.r, alpha.g, alpha.b);
    printf("add   : %d,%d,%d (attendu 255,255,0)\n", add.r, add.g, add.b);
    return !(alpha.r == 0 && alpha.g == 255 && add.r == 255 && add.g == 255
        && add.b == 0);
}

static int run(sfRenderWindow *window, const pair_t *p)
{
    sfEvent event;
    int frames = example_frames();
    int status = 0;

    while (sfRenderWindow_isOpen(window) && frames != 0) {
        while (sfRenderWindow_pollEvent(window, &event))
            if (event.type == sfEvtClosed)
                sfRenderWindow_close(window);
        sfRenderWindow_clear(window, sfBlack);
        draw_pair(window, p, 0, sfBlendAlpha);
        draw_pair(window, p, 300, sfBlendAdd);
        if (frames == 1)
            status = check(window);
        sfRenderWindow_display(window);
        if (frames > 0)
            frames--;
    }
    return status;
}

int main(void)
{
    sfVideoMode mode = {600, 250, 32};
    sfRenderWindow *window = sfRenderWindow_create(mode, "Blend",
        sfTitlebar | sfClose, NULL);
    pair_t p = {sfRectangleShape_create(), sfRectangleShape_create()};
    int status = 1;

    if (window != NULL && p.red != NULL && p.green != NULL) {
        sfRectangleShape_setSize(p.red, (sfVector2f){100, 100});
        sfRectangleShape_setSize(p.green, (sfVector2f){100, 100});
        sfRectangleShape_setFillColor(p.red, sfRed);
        sfRectangleShape_setFillColor(p.green, sfGreen);
        sfRenderWindow_setFramerateLimit(window, 60);
        status = run(window, &p);
    }
    sfRectangleShape_destroy(p.green);
    sfRectangleShape_destroy(p.red);
    sfRenderWindow_destroy(window);
    return status;
}
