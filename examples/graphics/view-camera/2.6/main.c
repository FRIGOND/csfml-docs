#include <SFML/Graphics.h>
#include <math.h>
#include <stdio.h>
#include "test_mode.h"

static void update_camera(sfView *view, int tick)
{
    float a = (float)tick * 0.03f;

    sfView_setCenter(view, (sfVector2f){400 + 200 * cosf(a),
        300 + 150 * sinf(a)});
    sfView_setSize(view, (sfVector2f){800, 600});
    sfView_zoom(view, 1.0f + 0.3f * sinf(a * 0.5f));
    sfView_setRotation(view, 10 * sinf(a));
}

static void draw_world(sfRenderWindow *window, sfRectangleShape *tile)
{
    for (int y = 0; y < 8; y++) {
        for (int x = 0; x < 10; x++) {
            sfRectangleShape_setPosition(tile,
                (sfVector2f){x * 100.0f, y * 100.0f});
            sfRectangleShape_setFillColor(tile, (x + y) % 2 ?
                sfColor_fromRGB(200, 80, 80) : sfColor_fromRGB(80, 80, 200));
            sfRenderWindow_drawRectangleShape(window, tile, NULL);
        }
    }
}

static int check(const sfRenderWindow *window, const sfView *view)
{
    sfVector2f p = sfRenderWindow_mapPixelToCoords(window, (sfVector2i){400,
        300}, sfRenderWindow_getView(window));
    sfVector2f c = sfView_getCenter(view);

    printf("pixel (400,300) -> monde (%.1f, %.1f), centre vue (%.1f, %.1f)\n",
        p.x, p.y, c.x, c.y);
    return fabsf(p.x - c.x) > 1 || fabsf(p.y - c.y) > 1;
}

static int run(sfRenderWindow *window, sfView *view, sfRectangleShape *tile)
{
    sfEvent event;
    int frames = example_frames();
    int status = 0;

    for (int tick = 0; sfRenderWindow_isOpen(window) && frames != 0; tick++) {
        while (sfRenderWindow_pollEvent(window, &event))
            if (event.type == sfEvtClosed)
                sfRenderWindow_close(window);
        update_camera(view, tick);
        sfRenderWindow_setView(window, view);
        sfRenderWindow_clear(window, sfBlack);
        draw_world(window, tile);
        if (frames == 1)
            status = check(window, view);
        sfRenderWindow_setView(window, sfRenderWindow_getDefaultView(window));
        sfRenderWindow_display(window);
        if (frames > 0)
            frames--;
    }
    return status;
}

int main(void)
{
    sfVideoMode mode = {800, 600, 32};
    sfRenderWindow *window = sfRenderWindow_create(mode, "Camera",
        sfTitlebar | sfClose, NULL);
    sfView *view = sfView_createFromRect((sfFloatRect){0, 0, 800, 600});
    sfRectangleShape *tile = sfRectangleShape_create();
    int status = 1;

    if (window != NULL && view != NULL && tile != NULL) {
        sfRectangleShape_setSize(tile, (sfVector2f){96, 96});
        sfRenderWindow_setFramerateLimit(window, 60);
        status = run(window, view, tile);
    }
    sfRectangleShape_destroy(tile);
    sfView_destroy(view);
    sfRenderWindow_destroy(window);
    return status;
}
