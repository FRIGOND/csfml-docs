#include <SFML/Graphics.h>
#include <stdio.h>
#include "test_mode.h"

static sfColor pixel_at(const sfRenderWindow *window, sfVector2f pos)
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
        px = sfImage_getPixel(img, (unsigned)pos.x, (unsigned)pos.y);
    sfImage_destroy(img);
    sfTexture_destroy(tex);
    return px;
}

static int check(const sfRenderWindow *window, const sfSprite *sprite)
{
    sfTransform tr = sfSprite_getTransform(sprite);
    sfVector2f screen = sfTransform_transformPoint(&tr, (sfVector2f){56, 70});
    sfColor px = pixel_at(window, screen);

    printf("point (56,70) du logo -> ecran (%.0f,%.0f) = %d,%d,%d\n",
        screen.x, screen.y, px.r, px.g, px.b);
    return !(px.g > px.r && px.g > px.b);
}

static void place(sfSprite *sprite, const sfTexture *tex)
{
    sfSprite_setTexture(sprite, tex, sfTrue);
    sfSprite_setOrigin(sprite, (sfVector2f){186, 56});
    sfSprite_setPosition(sprite, (sfVector2f){400, 300});
    sfSprite_setScale(sprite, (sfVector2f){1.5f, 1.5f});
}

static int run(sfRenderWindow *window, sfSprite *sprite)
{
    sfEvent event;
    int frames = example_frames();
    int status = 0;

    while (sfRenderWindow_isOpen(window) && frames != 0) {
        while (sfRenderWindow_pollEvent(window, &event))
            if (event.type == sfEvtClosed)
                sfRenderWindow_close(window);
        sfSprite_rotate(sprite, 0.5f);
        sfRenderWindow_clear(window, sfColor_fromRGB(30, 30, 60));
        sfRenderWindow_drawSprite(window, sprite, NULL);
        if (frames == 1)
            status = check(window, sprite);
        sfRenderWindow_display(window);
        if (frames > 0)
            frames--;
    }
    return status;
}

int main(void)
{
    sfVideoMode mode = {800, 600, 32};
    sfRenderWindow *window = sfRenderWindow_create(mode, "Sprite",
        sfTitlebar | sfClose, NULL);
    char path[256];
    sfTexture *tex = sfTexture_createFromFile(
        example_asset(path, sizeof(path), "sfml_logo.png"), NULL);
    sfSprite *sprite = tex ? sfSprite_create() : NULL;
    int status = 1;

    if (window != NULL && sprite != NULL) {
        place(sprite, tex);
        sfRenderWindow_setFramerateLimit(window, 60);
        status = run(window, sprite);
    }
    sfSprite_destroy(sprite);
    sfTexture_destroy(tex);
    sfRenderWindow_destroy(window);
    return status;
}
