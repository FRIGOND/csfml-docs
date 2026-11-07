#include <CSFML/Graphics.h>
#include <stdio.h>
#include "test_mode.h"

#define SIZE 64
static const sfColor COLORS[4] = {{255, 0, 0, 255}, {0, 255, 0, 255},
    {0, 0, 255, 255}, {255, 255, 0, 255}};

static void fill_frame(sfImage *img, unsigned int n)
{
    for (unsigned int y = 0; y < SIZE; y++)
        for (unsigned int x = 0; x < SIZE; x++)
            sfImage_setPixel(img, (sfVector2u){n * SIZE + x, y},
                (x < 8 && y < 8) ? sfWhite : COLORS[n]);
}

static sfTexture *make_sheet(void)
{
    sfImage *img = sfImage_createFromColor((sfVector2u){SIZE * 4, SIZE},
        sfBlack);
    sfTexture *tex = NULL;

    if (img == NULL)
        return NULL;
    for (unsigned int n = 0; n < 4; n++)
        fill_frame(img, n);
    tex = sfTexture_createFromImage(img, NULL);
    sfImage_destroy(img);
    return tex;
}

static sfColor pixel_at(const sfRenderWindow *window, sfVector2u pos)
{
    sfTexture *tex = sfTexture_create(sfRenderWindow_getSize(window));
    sfImage *img = NULL;
    sfColor px = {0, 0, 0, 0};

    if (tex == NULL)
        return px;
    sfTexture_updateFromRenderWindow(tex, window, (sfVector2u){0, 0});
    img = sfTexture_copyToImage(tex);
    if (img != NULL)
        px = sfImage_getPixel(img, pos);
    sfImage_destroy(img);
    sfTexture_destroy(tex);
    return px;
}

static int check(const sfRenderWindow *window, const sfSprite *sprite)
{
    int frame = sfSprite_getTextureRect(sprite).position.x / SIZE;
    sfColor px = pixel_at(window, (sfVector2u){400, 300});
    sfColor want = COLORS[frame];

    printf("image %d : pixel = %d,%d,%d\n", frame, px.r, px.g, px.b);
    return !(px.r == want.r && px.g == want.g && px.b == want.b);
}

static void place(sfSprite *sprite)
{
    sfSprite_setPosition(sprite, (sfVector2f){272, 172});
    sfSprite_setScale(sprite, (sfVector2f){4, 4});
}

static void animate(sfSprite *sprite, const sfClock *clock)
{
    float t = sfTime_asSeconds(sfClock_getElapsedTime(clock));
    int n = (int)(t / 0.2f);

    sfSprite_setTextureRect(sprite,
        (sfIntRect){{(n % 4) * SIZE, 0}, {SIZE, SIZE}});
}

static int run(sfRenderWindow *window, sfSprite *sprite, sfClock *clock)
{
    sfEvent event;
    int frames = example_frames();
    int status = 0;

    while (sfRenderWindow_isOpen(window) && frames != 0) {
        while (sfRenderWindow_pollEvent(window, &event))
            if (event.type == sfEvtClosed)
                sfRenderWindow_close(window);
        animate(sprite, clock);
        sfRenderWindow_clear(window, sfBlack);
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
    sfVideoMode mode = {{800, 600}, 32};
    sfRenderWindow *window = sfRenderWindow_create(mode, "Animation",
        sfTitlebar | sfClose, sfWindowed, NULL);
    sfTexture *tex = make_sheet();
    sfSprite *sprite = tex ? sfSprite_create(tex) : NULL;
    sfClock *clock = sfClock_create();
    int status = 1;

    if (window != NULL && sprite != NULL && clock != NULL) {
        place(sprite);
        sfRenderWindow_setFramerateLimit(window, 60);
        status = run(window, sprite, clock);
    }
    sfClock_destroy(clock);
    sfSprite_destroy(sprite);
    sfTexture_destroy(tex);
    sfRenderWindow_destroy(window);
    return status;
}
