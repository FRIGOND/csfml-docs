#include <CSFML/Graphics.h>
#include <math.h>
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

#define BALL_SPEED 400.f
#define PADDLE_SPEED 500.f
#define MAX_DT 0.05f

typedef struct {
    sfCircleShape *shape;
    sfVector2f pos;
    sfVector2f vel;
    int bounces;
} ball_t;

typedef struct {
    sfRectangleShape *shape;
    float x;
} paddle_t;

typedef struct {
    sfRectangleShape *shape;
    int alive;
} brick_t;

typedef struct {
    sfRenderWindow *window;
    sfClock *clock;
    paddle_t paddle;
    ball_t ball;
    brick_t bricks[BRICK_COUNT];
    int destroyed;
    int lost;
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

/* La balle repart au centre de l'ecran, vers le haut. */
static void reset_ball(ball_t *ball, float x)
{
    ball->pos = (sfVector2f){x, 350};
    ball->vel = (sfVector2f){BALL_SPEED * 0.5f, -BALL_SPEED * 0.866f};
}

static int create_paddle_ball(game_t *g)
{
    g->paddle.shape = sfRectangleShape_create();
    g->ball.shape = sfCircleShape_create();
    g->clock = sfClock_create();
    if (g->paddle.shape == NULL || g->ball.shape == NULL || g->clock == NULL)
        return 1;
    sfRectangleShape_setSize(g->paddle.shape,
        (sfVector2f){PADDLE_W, PADDLE_H});
    g->paddle.x = (WIN_W - PADDLE_W) / 2;
    sfRectangleShape_setPosition(g->paddle.shape,
        (sfVector2f){g->paddle.x, PADDLE_Y});
    sfCircleShape_setRadius(g->ball.shape, BALL_R);
    sfCircleShape_setOrigin(g->ball.shape, (sfVector2f){BALL_R, BALL_R});
    reset_ball(&g->ball, WIN_W / 2);
    return 0;
}

static int create_bricks(game_t *g)
{
    for (int i = 0; i < BRICK_COUNT; i++) {
        g->bricks[i].shape = sfRectangleShape_create();
        if (g->bricks[i].shape == NULL)
            return 1;
        g->bricks[i].alive = 1;
        place_brick(g->bricks[i].shape, i);
    }
    return 0;
}

static void destroy_game(game_t *g)
{
    for (int i = 0; i < BRICK_COUNT; i++)
        if (g->bricks[i].shape != NULL)
            sfRectangleShape_destroy(g->bricks[i].shape);
    if (g->ball.shape != NULL)
        sfCircleShape_destroy(g->ball.shape);
    if (g->clock != NULL)
        sfClock_destroy(g->clock);
    if (g->paddle.shape != NULL)
        sfRectangleShape_destroy(g->paddle.shape);
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
    if (ball->vel.y < 0)
        ball->bounces += reflect(&ball->pos.y, &ball->vel.y,
            BALL_R, WIN_H);
}

static void update_ball(game_t *g, float dt)
{
    ball_t *ball = &g->ball;

    ball->pos.x += ball->vel.x * dt;
    ball->pos.y += ball->vel.y * dt;
    bounce_walls(ball);
    if (ball->pos.y > WIN_H + BALL_R) {
        g->lost++;
        reset_ball(ball, g->paddle.x + PADDLE_W / 2);
    }
    sfCircleShape_setPosition(ball->shape, ball->pos);
}

/* Balle contre raquette : l'angle depend de l'endroit touche. */
static void hit_paddle(game_t *g)
{
    sfFloatRect bb = sfCircleShape_getGlobalBounds(g->ball.shape);
    sfFloatRect pb = sfRectangleShape_getGlobalBounds(g->paddle.shape);
    float rel = (g->ball.pos.x - g->paddle.x - PADDLE_W / 2) / (PADDLE_W / 2);

    if (g->ball.vel.y <= 0 || !sfFloatRect_intersects(&bb, &pb, NULL))
        return;
    rel = rel > 1 ? 1 : (rel < -1 ? -1 : rel);
    g->ball.vel.x = rel * BALL_SPEED * 0.8f;
    g->ball.vel.y = -sqrtf(BALL_SPEED * BALL_SPEED
        - g->ball.vel.x * g->ball.vel.x);
    g->ball.pos.y = PADDLE_Y - BALL_R;
}

/* Balle contre une brique : on rebondit sur l'axe le moins enfonce. */
static void bounce_on(ball_t *ball, sfFloatRect inter)
{
    if (inter.size.x < inter.size.y)
        ball->vel.x = -ball->vel.x;
    else
        ball->vel.y = -ball->vel.y;
}

static void hit_bricks(game_t *g)
{
    sfFloatRect bb = sfCircleShape_getGlobalBounds(g->ball.shape);
    sfFloatRect rb;
    sfFloatRect inter;

    for (int i = 0; i < BRICK_COUNT; i++) {
        if (!g->bricks[i].alive)
            continue;
        rb = sfRectangleShape_getGlobalBounds(g->bricks[i].shape);
        if (!sfFloatRect_intersects(&bb, &rb, &inter))
            continue;
        g->bricks[i].alive = 0;
        g->destroyed++;
        bounce_on(&g->ball, inter);
        return;
    }
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

/* Direction voulue : -1 (gauche), 0 (rien) ou +1 (droite).
 * En mode test, aucun clavier : la raquette suit la balle. */
static int read_direction(const game_t *g)
{
    float gap = g->ball.pos.x - g->paddle.x - PADDLE_W / 2;
    int dir = 0;

    if (example_frames() >= 0)
        return gap > 5 ? 1 : (gap < -5 ? -1 : 0);
    if (sfKeyboard_isKeyPressed(sfKeyLeft))
        dir -= 1;
    if (sfKeyboard_isKeyPressed(sfKeyRight))
        dir += 1;
    return dir;
}

static void update_paddle(paddle_t *paddle, int dir, float dt)
{
    paddle->x += dir * PADDLE_SPEED * dt;
    if (paddle->x < 0)
        paddle->x = 0;
    if (paddle->x > WIN_W - PADDLE_W)
        paddle->x = WIN_W - PADDLE_W;
    sfRectangleShape_setPosition(paddle->shape,
        (sfVector2f){paddle->x, PADDLE_Y});
}

/* En mode test : la balle reste dans la fenetre, des briques sont
 * tombees et le compte est bon. */
static int check_game(const game_t *g)
{
    int alive = 0;

    for (int i = 0; i < BRICK_COUNT; i++)
        alive += g->bricks[i].alive;
    printf("briques detruites : %d, balles perdues : %d\n",
        g->destroyed, g->lost);
    if (alive + g->destroyed != BRICK_COUNT)
        return 1;
    if (g->ball.pos.x < 0 || g->ball.pos.x > WIN_W)
        return 1;
    return example_frames() >= 60 && g->destroyed == 0;
}

static void draw_game(const game_t *g)
{
    sfRenderWindow_clear(g->window, sfBlack);
    for (int i = 0; i < BRICK_COUNT; i++)
        if (g->bricks[i].alive)
            sfRenderWindow_drawRectangleShape(g->window,
                g->bricks[i].shape, NULL);
    sfRenderWindow_drawRectangleShape(g->window, g->paddle.shape, NULL);
    sfRenderWindow_drawCircleShape(g->window, g->ball.shape, NULL);
    sfRenderWindow_display(g->window);
}

static int run(game_t *g)
{
    sfEvent event;
    int frames = example_frames();
    float dt = 0;

    while (sfRenderWindow_isOpen(g->window) && frames != 0) {
        while (sfRenderWindow_pollEvent(g->window, &event))
            if (event.type == sfEvtClosed)
                sfRenderWindow_close(g->window);
        dt = next_dt(g->clock);
        update_paddle(&g->paddle, read_direction(g), dt);
        update_ball(g, dt);
        hit_paddle(g);
        hit_bricks(g);
        draw_game(g);
        if (frames > 0)
            frames--;
    }
    if (example_frames() < 0)
        return 0;
    return check_game(g);
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
