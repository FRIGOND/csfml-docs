#include <CSFML/Audio.h>
#include <CSFML/Graphics.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include "test_mode.h"

/* ---- Reglages du jeu ---- */
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
#define PADDLE_SPEED 500.f
#define BALL_R 8.f
#define BALL_SPEED 400.f
#define SPEED_UP 1.01f
#define MAX_SPEED 600.f
#define LIVES 3
#define SCORE_PER_BRICK 10
#define MAX_DT 0.05f
#define TEST_SPEEDUP 240

static const sfColor ROW_COLORS[ROWS] = {
    {230, 70, 70, 255}, {240, 150, 50, 255}, {240, 220, 60, 255},
    {80, 200, 100, 255}, {70, 140, 230, 255}
};

/* ---- Types ---- */
typedef enum {
    STATE_MENU,
    STATE_SERVE,
    STATE_PLAY,
    STATE_WON,
    STATE_LOST
} state_t;

static const sfChar32 *MESSAGES[] = {
    U"Appuie sur Entrée pour jouer", U"Balle perdue : Entrée pour continuer",
    U"", U"Victoire ! Entrée pour rejouer", U"Perdu... Entrée pour rejouer"
};

typedef struct {
    sfSprite *sprite;
    sfVector2f pos;
    sfVector2f vel;
    float speed;
} ball_t;

typedef struct {
    sfSprite *sprite;
    float x;
} paddle_t;

typedef struct {
    sfSprite *sprite;
    int alive;
} brick_t;

typedef struct {
    sfTexture *paddle;
    sfTexture *ball;
    sfTexture *brick;
} textures_t;

typedef struct {
    sfFont *font;
    sfText *score;
    sfText *lives;
    sfText *title;
    sfText *message;
} texts_t;

typedef struct {
    sfRenderWindow *window;
    sfClock *clock;
    textures_t tex;
    texts_t texts;
    sfSoundBuffer *buffer;
    sfSound *sound;
    paddle_t paddle;
    ball_t ball;
    brick_t bricks[BRICK_COUNT];
    state_t state;
    int destroyed;
    int plops;
    int hits;
    int lives;
    int score;
} game_t;

/* ---- Textures et sprites ---- */
static sfTexture *texture_from(sfImage *image)
{
    sfTexture *texture = NULL;

    if (image == NULL)
        return NULL;
    texture = sfTexture_createFromImage(image, NULL);
    sfImage_destroy(image);
    return texture;
}

static int in_disc(int x, int y)
{
    float dx = x + 0.5f - BALL_R;
    float dy = y + 0.5f - BALL_R;

    return dx * dx + dy * dy <= BALL_R * BALL_R;
}

static sfImage *ball_image(void)
{
    int size = (int)(BALL_R * 2);
    sfImage *image = sfImage_createFromColor((sfVector2u){size, size},
        sfTransparent);

    for (int y = 0; image != NULL && y < size; y++)
        for (int x = 0; x < size; x++)
            if (in_disc(x, y))
                sfImage_setPixel(image, (sfVector2u){x, y}, sfWhite);
    return image;
}

static sfImage *brick_image(void)
{
    sfImage *image = sfImage_createFromColor(
        (sfVector2u){BRICK_W, BRICK_H}, sfWhite);
    sfColor shade = sfWhite;

    for (int y = 0; image != NULL && y < BRICK_H; y++) {
        shade.r = shade.g = shade.b = 255 - y * 4;
        for (int x = 0; x < BRICK_W; x++)
            sfImage_setPixel(image, (sfVector2u){x, y}, shade);
    }
    return image;
}

static int create_textures(textures_t *tex)
{
    sfImage *image = sfImage_createFromColor(
        (sfVector2u){PADDLE_W, PADDLE_H}, sfWhite);

    tex->paddle = texture_from(image);
    tex->ball = texture_from(ball_image());
    tex->brick = texture_from(brick_image());
    return tex->paddle == NULL || tex->ball == NULL || tex->brick == NULL;
}

static sfSprite *make_sprite(const sfTexture *texture, sfColor color)
{
    sfSprite *sprite = sfSprite_create(texture);

    if (sprite != NULL)
        sfSprite_setColor(sprite, color);
    return sprite;
}

static int create_bricks(game_t *g)
{
    sfVector2f pos;

    for (int i = 0; i < BRICK_COUNT; i++) {
        g->bricks[i].sprite = make_sprite(g->tex.brick,
            ROW_COLORS[i / COLS]);
        if (g->bricks[i].sprite == NULL)
            return 1;
        pos.x = BRICKS_LEFT + (i % COLS) * (BRICK_W + BRICK_GAP);
        pos.y = BRICKS_TOP + (i / COLS) * (BRICK_H + BRICK_GAP);
        sfSprite_setPosition(g->bricks[i].sprite, pos);
        g->bricks[i].alive = 1;
    }
    return 0;
}

static int create_paddle_ball(game_t *g)
{
    sfColor blue = sfColor_fromRGB(200, 220, 255);

    g->paddle.sprite = make_sprite(g->tex.paddle, blue);
    g->ball.sprite = make_sprite(g->tex.ball, sfWhite);
    if (g->paddle.sprite == NULL || g->ball.sprite == NULL)
        return 1;
    sfSprite_setOrigin(g->ball.sprite, (sfVector2f){BALL_R, BALL_R});
    return 0;
}

/* ---- Textes et son ---- */
static sfText *make_text(const sfFont *font, unsigned int size)
{
    sfText *text = sfText_create(font);

    if (text != NULL)
        sfText_setCharacterSize(text, size);
    return text;
}

static void center_text(sfText *text, sfVector2f pos)
{
    sfFloatRect lb = sfText_getLocalBounds(text);

    sfText_setOrigin(text, (sfVector2f){lb.position.x + lb.size.x / 2,
        lb.position.y + lb.size.y / 2});
    sfText_setPosition(text, pos);
}

static int create_texts(texts_t *t)
{
    char path[256];

    t->font = sfFont_createFromFile(example_asset(path, sizeof(path),
        "tuffy.ttf"));
    if (t->font == NULL)
        return 1;
    t->score = make_text(t->font, 24);
    t->lives = make_text(t->font, 24);
    t->title = make_text(t->font, 24);
    t->message = make_text(t->font, 32);
    if (!t->score || !t->lives || !t->title || !t->message)
        return 1;
    sfText_setPosition(t->score, (sfVector2f){20, 14});
    sfText_setPosition(t->lives, (sfVector2f){WIN_W - 110, 14});
    sfText_setUnicodeString(t->title, U"Éclate-les toutes !");
    center_text(t->title, (sfVector2f){WIN_W / 2, 26});
    return 0;
}

static int create_sound(game_t *g)
{
    char path[256];

    g->buffer = sfSoundBuffer_createFromFile(example_asset(path,
        sizeof(path), "doodle_pop.ogg"));
    if (g->buffer == NULL)
        return 1;
    g->sound = sfSound_create(g->buffer);
    return g->sound == NULL;
}

/* ---- Destruction : dans l'ordre inverse de la creation ---- */
static void destroy_texts(texts_t *t)
{
    if (t->score != NULL)
        sfText_destroy(t->score);
    if (t->lives != NULL)
        sfText_destroy(t->lives);
    if (t->title != NULL)
        sfText_destroy(t->title);
    if (t->message != NULL)
        sfText_destroy(t->message);
    if (t->font != NULL)
        sfFont_destroy(t->font);
}

static void destroy_textures(textures_t *tex)
{
    if (tex->paddle != NULL)
        sfTexture_destroy(tex->paddle);
    if (tex->ball != NULL)
        sfTexture_destroy(tex->ball);
    if (tex->brick != NULL)
        sfTexture_destroy(tex->brick);
}

static void destroy_game(game_t *g)
{
    for (int i = 0; i < BRICK_COUNT; i++)
        if (g->bricks[i].sprite != NULL)
            sfSprite_destroy(g->bricks[i].sprite);
    if (g->ball.sprite != NULL)
        sfSprite_destroy(g->ball.sprite);
    if (g->paddle.sprite != NULL)
        sfSprite_destroy(g->paddle.sprite);
    destroy_texts(&g->texts);
    if (g->sound != NULL)
        sfSound_destroy(g->sound);
    if (g->buffer != NULL)
        sfSoundBuffer_destroy(g->buffer);
    destroy_textures(&g->tex);
    if (g->clock != NULL)
        sfClock_destroy(g->clock);
    if (g->window != NULL)
        sfRenderWindow_destroy(g->window);
}

/* ---- Etats et messages ---- */
static void set_state(game_t *g, state_t state)
{
    g->state = state;
    sfText_setUnicodeString(g->texts.message, MESSAGES[state]);
    center_text(g->texts.message, (sfVector2f){WIN_W / 2, WIN_H / 2});
}

/* La balle attend sur la raquette, immobile. */
static void stick_ball(game_t *g)
{
    g->ball.pos = (sfVector2f){g->paddle.x + PADDLE_W / 2,
        PADDLE_Y - BALL_R};
    g->ball.vel = (sfVector2f){0, 0};
    sfSprite_setPosition(g->ball.sprite, g->ball.pos);
}

static void new_game(game_t *g)
{
    for (int i = 0; i < BRICK_COUNT; i++)
        g->bricks[i].alive = 1;
    g->destroyed = 0;
    g->plops = 0;
    g->score = 0;
    g->lives = LIVES;
    g->ball.speed = BALL_SPEED;
    g->paddle.x = (WIN_W - PADDLE_W) / 2;
    sfSprite_setPosition(g->paddle.sprite,
        (sfVector2f){g->paddle.x, PADDLE_Y});
    stick_ball(g);
    set_state(g, STATE_MENU);
}

/* Entree : lance la balle, ou recommence une partie finie. */
static void press_enter(game_t *g)
{
    if (g->state == STATE_MENU || g->state == STATE_SERVE) {
        g->ball.vel = (sfVector2f){g->ball.speed * 0.5f,
            -g->ball.speed * 0.866f};
        set_state(g, STATE_PLAY);
    } else if (g->state != STATE_PLAY) {
        new_game(g);
    }
}

/* ---- Physique ---- */
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

static void lose_life(game_t *g)
{
    g->lives--;
    if (g->lives > 0) {
        stick_ball(g);
        set_state(g, STATE_SERVE);
    } else {
        set_state(g, STATE_LOST);
    }
}

static void update_ball(game_t *g, float dt)
{
    ball_t *ball = &g->ball;

    ball->pos.x += ball->vel.x * dt;
    ball->pos.y += ball->vel.y * dt;
    reflect(&ball->pos.x, &ball->vel.x, BALL_R, WIN_W - BALL_R);
    if (ball->vel.y < 0)
        reflect(&ball->pos.y, &ball->vel.y, BALL_R, WIN_H);
    if (ball->pos.y > WIN_H + BALL_R)
        lose_life(g);
    sfSprite_setPosition(ball->sprite, ball->pos);
}

/* L'angle de renvoi depend de l'endroit touche sur la raquette. */
static void hit_paddle(game_t *g)
{
    sfFloatRect bb = sfSprite_getGlobalBounds(g->ball.sprite);
    sfFloatRect pb = sfSprite_getGlobalBounds(g->paddle.sprite);
    float rel = (g->ball.pos.x - g->paddle.x - PADDLE_W / 2) / (PADDLE_W / 2);
    float speed = g->ball.speed;

    if (g->ball.vel.y <= 0 || !sfFloatRect_intersects(&bb, &pb, NULL))
        return;
    rel = rel > 1 ? 1 : (rel < -1 ? -1 : rel);
    g->ball.vel.x = rel * speed * 0.8f;
    g->ball.vel.y = -sqrtf(speed * speed - g->ball.vel.x * g->ball.vel.x);
    g->ball.pos.y = PADDLE_Y - BALL_R;
    g->hits++;
}

static void break_brick(game_t *g, int index, sfFloatRect inter)
{
    g->bricks[index].alive = 0;
    g->destroyed++;
    g->score += SCORE_PER_BRICK;
    if (g->ball.speed < MAX_SPEED) {
        g->ball.speed *= SPEED_UP;
        g->ball.vel.x *= SPEED_UP;
        g->ball.vel.y *= SPEED_UP;
    }
    if (inter.size.x < inter.size.y)
        g->ball.vel.x = -g->ball.vel.x;
    else
        g->ball.vel.y = -g->ball.vel.y;
    sfSound_play(g->sound);
    g->plops++;
}

static void hit_bricks(game_t *g)
{
    sfFloatRect bb = sfSprite_getGlobalBounds(g->ball.sprite);
    sfFloatRect rb;
    sfFloatRect inter;

    for (int i = 0; i < BRICK_COUNT; i++) {
        if (!g->bricks[i].alive)
            continue;
        rb = sfSprite_getGlobalBounds(g->bricks[i].sprite);
        if (sfFloatRect_intersects(&bb, &rb, &inter)) {
            break_brick(g, i, inter);
            return;
        }
    }
}

static void update_paddle(paddle_t *paddle, int dir, float dt)
{
    paddle->x += dir * PADDLE_SPEED * dt;
    if (paddle->x < 0)
        paddle->x = 0;
    if (paddle->x > WIN_W - PADDLE_W)
        paddle->x = WIN_W - PADDLE_W;
    sfSprite_setPosition(paddle->sprite,
        (sfVector2f){paddle->x, PADDLE_Y});
}

/* ---- Entrees ---- */
/* Test : la raquette vise la balle avec un decalage qui change a
 * chaque renvoi, pour que la balle parte dans des directions variees. */
static int scripted_direction(const game_t *g)
{
    float offset = (g->hits * 37 % 81) - 40;
    float gap = g->ball.pos.x + offset - g->paddle.x - PADDLE_W / 2;

    return gap > 5 ? 1 : (gap < -5 ? -1 : 0);
}

static int read_direction(const game_t *g)
{
    int dir = 0;

    if (example_frames() >= 0)
        return scripted_direction(g);
    if (sfKeyboard_isKeyPressed(sfKeyLeft))
        dir -= 1;
    if (sfKeyboard_isKeyPressed(sfKeyRight))
        dir += 1;
    return dir;
}

static void handle_events(game_t *g)
{
    sfEvent event;

    while (sfRenderWindow_pollEvent(g->window, &event)) {
        if (event.type == sfEvtClosed)
            sfRenderWindow_close(g->window);
        if (event.type != sfEvtKeyPressed)
            continue;
        if (event.key.code == sfKeyEscape)
            sfRenderWindow_close(g->window);
        if (event.key.code == sfKeyEnter)
            press_enter(g);
    }
}

/* ---- Mise a jour ---- */
static float next_dt(sfClock *clock)
{
    float dt = sfTime_asSeconds(sfClock_restart(clock));

    if (example_frames() >= 0)
        return 1.f / 60.f;
    return dt > MAX_DT ? MAX_DT : dt;
}

static void update_step(game_t *g, float dt)
{
    update_paddle(&g->paddle, read_direction(g), dt);
    if (g->state == STATE_PLAY) {
        update_ball(g, dt);
        hit_paddle(g);
        hit_bricks(g);
    } else {
        stick_ball(g);
    }
    if (g->destroyed == BRICK_COUNT)
        set_state(g, STATE_WON);
}

static void update_hud(const game_t *g)
{
    char buffer[32];

    snprintf(buffer, sizeof(buffer), "Score : %d", g->score);
    sfText_setString(g->texts.score, buffer);
    snprintf(buffer, sizeof(buffer), "Vies : %d", g->lives);
    sfText_setString(g->texts.lives, buffer);
}

/* En mode test : plusieurs pas par image, et "Entree" appuyee par le code
 * chaque fois que le jeu attend le joueur. */
static void update_game(game_t *g)
{
    float dt = next_dt(g->clock);
    int test = example_frames() >= 0;
    int steps = test ? TEST_SPEEDUP : 1;

    for (int i = 0; i < steps && g->state < STATE_WON; i++) {
        if (test && g->state < STATE_PLAY)
            press_enter(g);
        update_step(g, dt);
    }
    update_hud(g);
}

/* ---- Affichage ---- */
static void draw_game(const game_t *g)
{
    sfRenderWindow_clear(g->window, sfBlack);
    for (int i = 0; i < BRICK_COUNT; i++)
        if (g->bricks[i].alive)
            sfRenderWindow_drawSprite(g->window,
                g->bricks[i].sprite, NULL);
    sfRenderWindow_drawSprite(g->window, g->paddle.sprite, NULL);
    sfRenderWindow_drawSprite(g->window, g->ball.sprite, NULL);
    sfRenderWindow_drawText(g->window, g->texts.score, NULL);
    sfRenderWindow_drawText(g->window, g->texts.lives, NULL);
    sfRenderWindow_drawText(g->window, g->texts.title, NULL);
    if (g->state != STATE_PLAY)
        sfRenderWindow_drawText(g->window, g->texts.message, NULL);
    sfRenderWindow_display(g->window);
}

/* ---- Verifications du mode test ---- */
/* Partie scriptee : il faut au moins 30 images (EXAMPLE_FRAMES=60 ici). */
static int check_game(const game_t *g)
{
    int alive = 0;

    for (int i = 0; i < BRICK_COUNT; i++)
        alive += g->bricks[i].alive;
    printf("etat %d (victoire = %d), score %d, vies %d, %d son(s)\n",
        (int)g->state, (int)STATE_WON, g->score, g->lives, g->plops);
    return g->state != STATE_WON || alive != 0
        || g->destroyed != BRICK_COUNT
        || g->score != BRICK_COUNT * SCORE_PER_BRICK
        || g->lives <= 0 || g->plops != g->destroyed;
}

static int check_texts(const game_t *g)
{
    const char *got = sfText_getString(g->texts.score);
    sfTime length = sfSoundBuffer_getDuration(g->buffer);

    printf("texte : \"%s\", son de %d ms\n", got,
        sfTime_asMilliseconds(length));
    return strcmp(got, "Score : 500") != 0
        || sfText_getUnicodeString(g->texts.title)[0] != 0xC9
        || sfTime_asMilliseconds(length) <= 0;
}

/* ---- Programme ---- */
static int run(game_t *g)
{
    int frames = example_frames();

    new_game(g);
    while (sfRenderWindow_isOpen(g->window) && frames != 0) {
        handle_events(g);
        update_game(g);
        draw_game(g);
        if (frames > 0)
            frames--;
    }
    if (example_frames() < 0)
        return 0;
    return check_game(g) || check_texts(g);
}

static int init_game(game_t *g)
{
    sfVideoMode mode = {{WIN_W, WIN_H}, 32};

    g->window = sfRenderWindow_create(mode, "Casse-briques",
        sfTitlebar | sfClose, sfWindowed, NULL);
    if (g->window == NULL)
        return 1;
    sfRenderWindow_setFramerateLimit(g->window, 60);
    g->clock = sfClock_create();
    return g->clock == NULL || create_textures(&g->tex)
        || create_texts(&g->texts) || create_sound(g)
        || create_paddle_ball(g) || create_bricks(g);
}

int main(void)
{
    game_t g = {0};
    int status = 1;

    if (init_game(&g) == 0)
        status = run(&g);
    destroy_game(&g);
    return status;
}
