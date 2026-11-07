#include <CSFML/Graphics.h>
#include <stdio.h>
#include "test_mode.h"

typedef struct {
    sfRectangleShape *rect;
    sfCircleShape *circle;
    sfConvexShape *convex;
} shapes_t;

static void setup(shapes_t *s)
{
    sfRectangleShape_setSize(s->rect, (sfVector2f){200, 120});
    sfRectangleShape_setPosition(s->rect, (sfVector2f){50, 50});
    sfRectangleShape_setFillColor(s->rect, sfRed);
    sfRectangleShape_setOutlineColor(s->rect, sfWhite);
    sfRectangleShape_setOutlineThickness(s->rect, 6);
    sfCircleShape_setRadius(s->circle, 70);
    sfCircleShape_setPosition(s->circle, (sfVector2f){300, 50});
    sfCircleShape_setFillColor(s->circle, sfGreen);
    sfConvexShape_setPointCount(s->convex, 3);
    sfConvexShape_setPoint(s->convex, 0, (sfVector2f){0, 0});
    sfConvexShape_setPoint(s->convex, 1, (sfVector2f){120, 0});
    sfConvexShape_setPoint(s->convex, 2, (sfVector2f){60, 120});
    sfConvexShape_setPosition(s->convex, (sfVector2f){550, 50});
    sfConvexShape_setFillColor(s->convex, sfBlue);
}

static int pixel_is(const sfRenderWindow *window, sfVector2u pos, sfColor c)
{
    sfTexture *tex = sfTexture_create(sfRenderWindow_getSize(window));
    sfImage *img = NULL;
    sfColor px = {0, 0, 0, 0};

    if (tex == NULL)
        return 0;
    sfTexture_updateFromRenderWindow(tex, window, (sfVector2u){0, 0});
    img = sfTexture_copyToImage(tex);
    if (img != NULL)
        px = sfImage_getPixel(img, pos);
    printf("pixel(%u,%u) = %d,%d,%d\n", pos.x, pos.y, px.r, px.g, px.b);
    sfImage_destroy(img);
    sfTexture_destroy(tex);
    return px.r == c.r && px.g == c.g && px.b == c.b;
}

static int check(const sfRenderWindow *window)
{
    int ok = pixel_is(window, (sfVector2u){150, 110}, sfRed);

    ok &= pixel_is(window, (sfVector2u){370, 120}, sfGreen);
    ok &= pixel_is(window, (sfVector2u){47, 110}, sfWhite);
    ok &= pixel_is(window, (sfVector2u){610, 60}, sfBlue);
    return !ok;
}

static int run(sfRenderWindow *window, shapes_t *s)
{
    sfEvent event;
    int frames = example_frames();
    int status = 0;

    while (sfRenderWindow_isOpen(window) && frames != 0) {
        while (sfRenderWindow_pollEvent(window, &event))
            if (event.type == sfEvtClosed)
                sfRenderWindow_close(window);
        sfRenderWindow_clear(window, sfBlack);
        sfRenderWindow_drawRectangleShape(window, s->rect, NULL);
        sfRenderWindow_drawCircleShape(window, s->circle, NULL);
        sfRenderWindow_drawConvexShape(window, s->convex, NULL);
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
    sfVideoMode mode = {{800, 300}, 32};
    sfRenderWindow *window = sfRenderWindow_create(mode, "Formes",
        sfTitlebar | sfClose, sfWindowed, NULL);
    shapes_t s = {sfRectangleShape_create(), sfCircleShape_create(),
        sfConvexShape_create()};
    int status = 1;

    if (window != NULL && s.rect && s.circle && s.convex) {
        setup(&s);
        sfRenderWindow_setFramerateLimit(window, 60);
        status = run(window, &s);
    }
    if (window != NULL)
        sfRenderWindow_destroy(window);
    sfRectangleShape_destroy(s.rect);
    sfCircleShape_destroy(s.circle);
    sfConvexShape_destroy(s.convex);
    return status;
}
