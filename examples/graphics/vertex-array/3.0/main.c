#include <CSFML/Graphics.h>
#include <stdio.h>
#include "test_mode.h"

/* Un quad = 2 triangles (6 sommets) : sfQuads est supprime en 3.0. */
static void add(sfVertexArray *va, float x, float y, sfColor color)
{
    sfVertex v = {{x, y}, color, {0, 0}};

    sfVertexArray_append(va, v);
}

static void build(sfVertexArray *va)
{
    sfVertexArray_setPrimitiveType(va, sfTriangles);
    add(va, 100, 100, sfRed);
    add(va, 300, 100, sfGreen);
    add(va, 300, 300, sfBlue);
    add(va, 100, 100, sfRed);
    add(va, 300, 300, sfBlue);
    add(va, 100, 300, sfYellow);
    add(va, 400, 100, sfRed);
    add(va, 600, 100, sfRed);
    add(va, 500, 300, sfRed);
}

static int check(const sfRenderWindow *window, sfVertexArray *va)
{
    sfTexture *tex = sfTexture_create(sfRenderWindow_getSize(window));
    sfImage *img = NULL;
    sfColor px = {0, 0, 0, 0};

    if (tex == NULL)
        return 1;
    sfTexture_updateFromRenderWindow(tex, window, (sfVector2u){0, 0});
    img = sfTexture_copyToImage(tex);
    if (img != NULL)
        px = sfImage_getPixel(img, (sfVector2u){500, 150});
    printf("sommets : %zu, pixel(500,150) = %d,%d,%d\n",
        sfVertexArray_getVertexCount(va), px.r, px.g, px.b);
    sfImage_destroy(img);
    sfTexture_destroy(tex);
    return !(px.r == 255 && px.g == 0 && px.b == 0
        && sfVertexArray_getVertexCount(va) == 9);
}

static int run(sfRenderWindow *window, sfVertexArray *va)
{
    sfEvent event;
    int frames = example_frames();
    int status = 0;

    while (sfRenderWindow_isOpen(window) && frames != 0) {
        while (sfRenderWindow_pollEvent(window, &event))
            if (event.type == sfEvtClosed)
                sfRenderWindow_close(window);
        sfRenderWindow_clear(window, sfBlack);
        sfRenderWindow_drawVertexArray(window, va, NULL);
        if (frames == 1)
            status = check(window, va);
        sfRenderWindow_display(window);
        if (frames > 0)
            frames--;
    }
    return status;
}

int main(void)
{
    sfVideoMode mode = {{700, 400}, 32};
    sfRenderWindow *window = sfRenderWindow_create(mode, "Sommets",
        sfTitlebar | sfClose, sfWindowed, NULL);
    sfVertexArray *va = sfVertexArray_create();
    int status = 1;

    if (window != NULL && va != NULL) {
        build(va);
        sfRenderWindow_setFramerateLimit(window, 60);
        status = run(window, va);
    }
    sfVertexArray_destroy(va);
    sfRenderWindow_destroy(window);
    return status;
}
