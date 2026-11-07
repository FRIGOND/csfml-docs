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

#define BALL_SPEED_X 300.f
#define BALL_SPEED_Y -400.f
#define MAX_DT 0.05f

typedef struct {
    sfCircleShape *shape;
    sfVector2f pos;
    sfVector2f vel;
    int bounces;
} ball_t;

typedef struct {
    sfRenderWindow *window;
    sfClock *clock;
    sfRectangleShape *paddle;
    ball_t ball;
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
    g->ball.shape = sfCircleShape_create();
    g->clock = sfClock_create();
    if (g->paddle == NULL || g->ball.shape == NULL || g->clock == NULL)
        return 1;
    sfRectangleShape_setSize(g->paddle, (sfVector2f){PADDLE_W, PADDLE_H});
    sfRectangleShape_setPosition(g->paddle,
        (sfVector2f){(WIN_W - PADDLE_W) / 2, PADDLE_Y});
    sfCircleShape_setRadius(g->ball.shape, BALL_R);
    sfCircleShape_setOrigin(g->ball.shape, (sfVector2f){BALL_R, BALL_R});
    g->ball.pos = (sfVector2f){WIN_W / 2, 300};
    g->ball.vel = (sfVector2f){BALL_SPEED_X, BALL_SPEED_Y};
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
    if (g->ball.shape != NULL)
        sfCircleShape_destroy(g->ball.shape);
    if (g->clock != NULL)
        sfClock_destroy(g->clock);
    if (g->paddle != NULL)
        sfRectangleShape_destroy(g->paddle);
    if (g->window != NULL)
        sfRenderWindow_destroy(g->window);
}

/* Rebond sur un mur : renvoie 1 si la balle a touche l'un des deux. */
static int reflect(float *pos, float *vel, float min, float max)
{
    if (*pos < min) {
        *pos = min;
        *vel = -*vel;
        return 1;
    }
    if (*pos > max) {
        *pos = max;
        *vel = -*vel;
        return 1;
    }
    return 0;
}

static void bounce_walls(ball_t *ball)
{
    ball->bounces += reflect(&ball->pos.x, &ball->vel.x,
        BALL_R, WIN_W - BALL_R);
    ball->bounces += reflect(&ball->pos.y, &ball->vel.y,
        BALL_R, WIN_H - BALL_R);
}

static void update_ball(ball_t *ball, float dt)
{
    ball->pos.x += ball->vel.x * dt;
    ball->pos.y += ball->vel.y * dt;
    bounce_walls(ball);
    sfCircleShape_setPosition(ball->shape, ball->pos);
}

/* Temps ecoule depuis la derniere image. En mode test : 1/60 s fixe,
 * pour que la partie soit toujours la meme. */
static float next_dt(sfClock *clock)
{
    float dt = sfTime_asSeconds(sfClock_restart(clock));

    if (example_frames() >= 0)
        return 1.f / 60.f;
    return dt > MAX_DT ? MAX_DT : dt;
}

/* En mode test : la balle reste dans la fenetre et a rebondi. */
static int check_ball(const game_t *g)
{
    sfFloatRect bb = sfCircleShape_getGlobalBounds(g->ball.shape);

    printf("balle : x=%.0f y=%.0f, %d rebond(s)\n", g->ball.pos.x,
        g->ball.pos.y, g->ball.bounces);
    if (bb.position.x < 0 || bb.position.x + bb.size.x > WIN_W
        || bb.position.y < 0 || bb.position.y + bb.size.y > WIN_H)
        return 1;
    return example_frames() >= 60 && g->ball.bounces == 0;
}

static void draw_game(const game_t *g)
{
    sfRenderWindow_clear(g->window, sfBlack);
    for (int i = 0; i < BRICK_COUNT; i++)
        sfRenderWindow_drawRectangleShape(g->window, g->bricks[i], NULL);
    sfRenderWindow_drawRectangleShape(g->window, g->paddle, NULL);
    sfRenderWindow_drawCircleShape(g->window, g->ball.shape, NULL);
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
        update_ball(&g->ball, next_dt(g->clock));
        draw_game(g);
        if (frames > 0)
            frames--;
    }
    return example_frames() >= 0 ? check_ball(g) : 0;
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
