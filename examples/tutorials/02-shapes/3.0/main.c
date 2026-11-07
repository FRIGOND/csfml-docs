#include <CSFML/Graphics.h>
#include <stdio.h>
#include "test_mode.h"

#define WIN_W 800
#define WIN_H 600
#define COLS 10
#define ROWS 5
#define BRICK_COUNT (COLS * ROWS)
#define BRICK_W 70.f
#define BRICK_H 20.f
#define BRICK_GAP 6.f
#define BRICKS_LEFT 20.f
#define BRICKS_TOP 60.f
#define PADDLE_W 100.f
#define PADDLE_H 15.f
#define PADDLE_Y 550.f
#define BALL_R 8.f

static const sfColor ROW_COLORS[ROWS] = {
    {230, 70, 70, 255}, {240, 150, 50, 255}, {240, 220, 60, 255},
    {80, 200, 100, 255}, {70, 140, 230, 255}
};

typedef struct {
    sfRenderWindow *window;
    sfRectangleShape *paddle;
    sfCircleShape *ball;
    sfRectangleShape *bricks[BRICK_COUNT];
} game_t;

static void place_brick(sfRectangleShape *brick, int index)
{
    int col = index % COLS;
    int row = index / COLS;
    sfVector2f pos = {BRICKS_LEFT + col * (BRICK_W + BRICK_GAP),
        BRICKS_TOP + row * (BRICK_H + BRICK_GAP)};

    sfRectangleShape_setSize(brick, (sfVector2f){BRICK_W, BRICK_H});
    sfRectangleShape_setPosition(brick, pos);
    sfRectangleShape_setFillColor(brick, ROW_COLORS[row]);
}

static int create_paddle_ball(game_t *g)
{
    g->paddle = sfRectangleShape_create();
    g->ball = sfCircleShape_create();
    if (g->paddle == NULL || g->ball == NULL)
        return 1;
    sfRectangleShape_setSize(g->paddle, (sfVector2f){PADDLE_W, PADDLE_H});
    sfRectangleShape_setPosition(g->paddle,
        (sfVector2f){(WIN_W - PADDLE_W) / 2, PADDLE_Y});
    sfCircleShape_setRadius(g->ball, BALL_R);
    sfCircleShape_setOrigin(g->ball, (sfVector2f){BALL_R, BALL_R});
    sfCircleShape_setPosition(g->ball, (sfVector2f){WIN_W / 2, 400});
    return 0;
}

static int create_bricks(game_t *g)
{
    for (int i = 0; i < BRICK_COUNT; i++) {
        g->bricks[i] = sfRectangleShape_create();
        if (g->bricks[i] == NULL)
            return 1;
        place_brick(g->bricks[i], i);
    }
    return 0;
}

static void destroy_game(game_t *g)
{
    for (int i = 0; i < BRICK_COUNT; i++)
        if (g->bricks[i] != NULL)
            sfRectangleShape_destroy(g->bricks[i]);
    if (g->ball != NULL)
        sfCircleShape_destroy(g->ball);
    if (g->paddle != NULL)
        sfRectangleShape_destroy(g->paddle);
    if (g->window != NULL)
        sfRenderWindow_destroy(g->window);
}

/* En mode test : toutes les briques tiennent dans la fenetre,
 * au-dessus de la raquette, et la balle est centree. */
static int check_layout(const game_t *g)
{
    sfRectangleShape *last = g->bricks[BRICK_COUNT - 1];
    sfFloatRect rb = sfRectangleShape_getGlobalBounds(last);
    sfFloatRect bb = sfCircleShape_getGlobalBounds(g->ball);

    printf("derniere brique : x=%.0f..%.0f, bas=%.0f\n", rb.position.x,
        rb.position.x + rb.size.x, rb.position.y + rb.size.y);
    printf("balle : centre x=%.0f\n", bb.position.x + bb.size.x / 2);
    return rb.position.x + rb.size.x > WIN_W
        || rb.position.y + rb.size.y > PADDLE_Y
        || bb.position.x + bb.size.x / 2 != WIN_W / 2;
}

static void draw_game(const game_t *g)
{
    sfRenderWindow_clear(g->window, sfBlack);
    for (int i = 0; i < BRICK_COUNT; i++)
        sfRenderWindow_drawRectangleShape(g->window, g->bricks[i], NULL);
    sfRenderWindow_drawRectangleShape(g->window, g->paddle, NULL);
    sfRenderWindow_drawCircleShape(g->window, g->ball, NULL);
    sfRenderWindow_display(g->window);
}

static int run(game_t *g)
{
    sfEvent event;
    int frames = example_frames();

    while (sfRenderWindow_isOpen(g->window) && frames != 0) {
        while (sfRenderWindow_pollEvent(g->window, &event))
            if (event.type == sfEvtClosed)
                sfRenderWindow_close(g->window);
        draw_game(g);
        if (frames > 0)
            frames--;
    }
    return example_frames() >= 0 ? check_layout(g) : 0;
}

int main(void)
{
    game_t g = {0};
    sfVideoMode mode = {{WIN_W, WIN_H}, 32};
    int status = 1;

    g.window = sfRenderWindow_create(mode, "Casse-briques",
        sfTitlebar | sfClose, sfWindowed, NULL);
    if (g.window != NULL && create_paddle_ball(&g) == 0
        && create_bricks(&g) == 0) {
        sfRenderWindow_setFramerateLimit(g.window, 60);
        status = run(&g);
    }
    destroy_game(&g);
    return status;
}
