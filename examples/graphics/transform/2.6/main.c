#include <SFML/Graphics.h>
#include <math.h>
#include <stdio.h>
#include "test_mode.h"

static sfTransform make_transform(void)
{
    sfTransform t = sfTransform_Identity;

    sfTransform_translate(&t, 100, 50);
    sfTransform_rotate(&t, 90);
    sfTransform_scale(&t, 2, 2);
    return t;
}

static int check(const sfRenderWindow *window, const sfTransform *t)
{
    sfVector2f p = sfTransform_transformPoint(t, (sfVector2f){10, 0});
    sfVector2f c = sfTransform_transformPoint(t, (sfVector2f){20, 10});
    sfTexture *tex = sfTexture_create(sfRenderWindow_getSize(window).x,
        sfRenderWindow_getSize(window).y);
    sfImage *img = NULL;
    sfColor px = {0, 0, 0, 0};

    if (tex == NULL)
        return 1;
    sfTexture_updateFromRenderWindow(tex, window, 0, 0);
    img = sfTexture_copyToImage(tex);
    if (img != NULL)
        px = sfImage_getPixel(img, (unsigned)c.x, (unsigned)c.y);
    printf("(10,0) -> (%.1f, %.1f), pixel du centre (%.0f,%.0f) = %d,%d,%d\n",
        p.x, p.y, c.x, c.y, px.r, px.g, px.b);
    sfImage_destroy(img);
    sfTexture_destroy(tex);
    return !(fabsf(p.x - 100) < 0.01f && fabsf(p.y - 70) < 0.01f
        && px.r == 255 && px.g == 0);
}

static int run(sfRenderWindow *window, sfRectangleShape *rect)
{
    sfTransform t = make_transform();
    sfRenderStates states = sfRenderStates_default();
    sfEvent event;
    int frames = example_frames();
    int status = 0;

    states.transform = t;
    while (sfRenderWindow_isOpen(window) && frames != 0) {
        while (sfRenderWindow_pollEvent(window, &event))
            if (event.type == sfEvtClosed)
                sfRenderWindow_close(window);
        sfRenderWindow_clear(window, sfBlack);
        sfRenderWindow_drawRectangleShape(window, rect, &states);
        if (frames == 1)
            status = check(window, &t);
        sfRenderWindow_display(window);
        if (frames > 0)
            frames--;
    }
    return status;
}

int main(void)
{
    sfVideoMode mode = {400, 300, 32};
    sfRenderWindow *window = sfRenderWindow_create(mode, "Transform",
        sfTitlebar | sfClose, NULL);
    sfRectangleShape *rect = sfRectangleShape_create();
    int status = 1;

    if (window != NULL && rect != NULL) {
        sfRectangleShape_setSize(rect, (sfVector2f){40, 20});
        sfRectangleShape_setFillColor(rect, sfRed);
        sfRenderWindow_setFramerateLimit(window, 60);
        status = run(window, rect);
    }
    sfRectangleShape_destroy(rect);
    sfRenderWindow_destroy(window);
    return status;
}
