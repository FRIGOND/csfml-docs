#include <CSFML/Graphics.h>
#include <stdio.h>
#include "test_mode.h"

static int paint(sfRenderTexture *rt)
{
    sfCircleShape *disc = sfCircleShape_create();

    if (disc == NULL)
        return 0;
    sfCircleShape_setRadius(disc, 80);
    sfCircleShape_setPosition(disc, (sfVector2f){20, 20});
    sfCircleShape_setFillColor(disc, sfRed);
    sfRenderTexture_clear(rt, sfBlue);
    sfRenderTexture_drawCircleShape(rt, disc, NULL);
    sfRenderTexture_display(rt);
    sfCircleShape_destroy(disc);
    return 1;
}

static int check(const sfTexture *tex)
{
    sfImage *img = sfTexture_copyToImage(tex);
    sfColor center = {0, 0, 0, 0};
    sfColor corner = {0, 0, 0, 0};

    if (img == NULL)
        return 1;
    center = sfImage_getPixel(img, (sfVector2u){100, 100});
    corner = sfImage_getPixel(img, (sfVector2u){2, 2});
    printf("centre = %d,%d,%d  coin = %d,%d,%d\n", center.r, center.g,
        center.b, corner.r, corner.g, corner.b);
    sfImage_destroy(img);
    return !(center.r == 255 && center.b == 0 && corner.b == 255);
}

static void draw_copies(sfRenderWindow *window, sfSprite *sprite)
{
    for (int i = 0; i < 3; i++) {
        sfSprite_setPosition(sprite, (sfVector2f){100.0f + i * 250, 100});
        sfSprite_setRotation(sprite, (float)i * 15);
        sfRenderWindow_drawSprite(window, sprite, NULL);
    }
}

static void run(sfRenderWindow *window, sfSprite *sprite)
{
    sfEvent event;
    int frames = example_frames();

    while (sfRenderWindow_isOpen(window) && frames != 0) {
        while (sfRenderWindow_pollEvent(window, &event))
            if (event.type == sfEvtClosed)
                sfRenderWindow_close(window);
        sfRenderWindow_clear(window, sfBlack);
        draw_copies(window, sprite);
        sfRenderWindow_display(window);
        if (frames > 0)
            frames--;
    }
}

int main(void)
{
    sfVideoMode mode = {{800, 400}, 32};
    sfRenderWindow *window = sfRenderWindow_create(mode, "RenderTexture",
        sfTitlebar | sfClose, sfWindowed, NULL);
    sfRenderTexture *rt = sfRenderTexture_create((sfVector2u){200, 200}, NULL);
    sfSprite *sprite = rt ? sfSprite_create(sfRenderTexture_getTexture(rt))
        : NULL;
    int status = 1;

    if (window != NULL && sprite != NULL && paint(rt)) {
        sfRenderWindow_setFramerateLimit(window, 60);
        run(window, sprite);
        status = example_frames() >= 0 ?
            check(sfRenderTexture_getTexture(rt)) : 0;
    }
    sfSprite_destroy(sprite);
    sfRenderTexture_destroy(rt);
    sfRenderWindow_destroy(window);
    return status;
}
