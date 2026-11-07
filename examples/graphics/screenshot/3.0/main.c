#include <CSFML/Graphics.h>
#include <stdio.h>
#include "test_mode.h"

static int check_pixel(const sfRenderWindow *window)
{
    sfTexture *tex = sfTexture_create((sfVector2u){800, 600});
    sfImage *img = NULL;
    sfColor px;

    if (tex == NULL)
        return 1;
    sfTexture_updateFromRenderWindow(tex, window, (sfVector2u){0, 0});
    img = sfTexture_copyToImage(tex);
    px = sfImage_getPixel(img, (sfVector2u){400, 300});
    sfImage_saveToFile(img, "screenshot.png");
    printf("pixel(400,300) = %d,%d,%d\n", px.r, px.g, px.b);
    sfImage_destroy(img);
    sfTexture_destroy(tex);
    return !(px.r == 255 && px.g == 0 && px.b == 0);
}

int main(void)
{
    sfVideoMode mode = {{800, 600}, 32};
    sfRenderWindow *window = sfRenderWindow_create(mode, "Capture",
        sfTitlebar | sfClose, sfWindowed, NULL);
    sfRectangleShape *rect = sfRectangleShape_create();
    int frames = example_frames();
    int status = 0;

    if (window == NULL || rect == NULL)
        return 1;
    sfRectangleShape_setSize(rect, (sfVector2f){800, 600});
    sfRectangleShape_setFillColor(rect, sfRed);
    sfRenderWindow_clear(window, sfBlack);
    sfRenderWindow_drawRectangleShape(window, rect, NULL);
    sfRenderWindow_display(window);
    if (frames >= 0)
        status = check_pixel(window);
    sfRectangleShape_destroy(rect);
    sfRenderWindow_destroy(window);
    return status;
}
