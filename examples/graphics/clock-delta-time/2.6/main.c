#include <SFML/Graphics.h>
#include <math.h>
#include <stdio.h>
#include "test_mode.h"

#define SPEED 200.0f

/* Deplace le carre de SPEED pixels par seconde, quelle que soit la
 * cadence : on multiplie la vitesse par le temps ecoule (dt). */
static float step(sfRectangleShape *square, sfClock *clock)
{
    float dt = sfTime_asSeconds(sfClock_restart(clock));
    sfVector2f pos = sfRectangleShape_getPosition(square);

    pos.x = fmodf(pos.x + SPEED * dt, 800);
    sfRectangleShape_setPosition(square, pos);
    return dt;
}

static int check(float sum_dt, const sfClock *total, int tick)
{
    float elapsed = sfTime_asSeconds(sfClock_getElapsedTime(total));

    printf("%d images en %.2f s (%.0f im/s), somme des dt = %.2f s\n",
        tick, elapsed, (float)tick / elapsed, sum_dt);
    return fabsf(elapsed - sum_dt) > 0.1f;
}

static int run(sfRenderWindow *window, sfRectangleShape *square,
    sfClock *clock, const sfClock *total)
{
    sfEvent event;
    int frames = example_frames();
    float sum = 0;
    int tick = 0;

    while (sfRenderWindow_isOpen(window) && frames != 0) {
        while (sfRenderWindow_pollEvent(window, &event))
            if (event.type == sfEvtClosed)
                sfRenderWindow_close(window);
        sum += step(square, clock);
        sfRenderWindow_clear(window, sfBlack);
        sfRenderWindow_drawRectangleShape(window, square, NULL);
        sfRenderWindow_display(window);
        tick++;
        if (frames > 0)
            frames--;
    }
    return example_frames() >= 0 && check(sum, total, tick);
}

int main(void)
{
    sfVideoMode mode = {800, 200, 32};
    sfRenderWindow *window = sfRenderWindow_create(mode, "Delta time",
        sfTitlebar | sfClose, NULL);
    sfRectangleShape *square = sfRectangleShape_create();
    sfClock *clock = sfClock_create();
    sfClock *total = sfClock_create();
    int status = 1;

    if (window && square && clock && total) {
        sfRectangleShape_setSize(square, (sfVector2f){40, 40});
        sfRectangleShape_setPosition(square, (sfVector2f){0, 80});
        sfRenderWindow_setFramerateLimit(window, 60);
        status = run(window, square, clock, total);
    }
    sfClock_destroy(total);
    sfClock_destroy(clock);
    sfRectangleShape_destroy(square);
    sfRenderWindow_destroy(window);
    return status;
}
