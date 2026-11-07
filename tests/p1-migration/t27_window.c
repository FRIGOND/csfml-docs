/* Test 27 : fenêtre + sprite + texte (style APRÈS 3.0.0), ferme après 30 frames */
#include <CSFML/Graphics.h>
#include <stdio.h>
int main(void)
{
    sfVideoMode mode = {{800, 600}, 32};
    sfRenderWindow *win = sfRenderWindow_create(mode, "t27", sfResize | sfClose, sfWindowed, NULL);
    sfTexture *tex = sfTexture_createFromFile("../p1/sfml_logo.png", NULL);
    sfFont *font = sfFont_createFromFile("../p1/tuffy.ttf");
    if (win == NULL || tex == NULL || font == NULL)
        return 84;
    sfSprite *spr = sfSprite_create(tex);
    sfIntRect rect = {{0, 0}, {110, 110}};
    rect.position.x += 10;
    sfSprite_setTextureRect(spr, rect);
    sfText *txt = sfText_create(font);
    sfText_setString(txt, "Score: 0");
    sfText_setFillColor(txt, sfWhite);
    sfEvent event;
    for (int frame = 0; frame < 30 && sfRenderWindow_isOpen(win); frame++) {
        while (sfRenderWindow_pollEvent(win, &event))
            if (event.type == sfEvtClosed)
                sfRenderWindow_close(win);
        sfRenderWindow_clear(win, sfBlack);
        sfRenderWindow_drawSprite(win, spr, NULL);
        sfRenderWindow_drawText(win, txt, NULL);
        sfRenderWindow_display(win);
    }
    printf("frames ok\n");
    sfText_destroy(txt);
    sfSprite_destroy(spr);
    sfFont_destroy(font);
    sfTexture_destroy(tex);
    sfRenderWindow_destroy(win);
    return 0;
}
