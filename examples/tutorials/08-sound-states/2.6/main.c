#include <SFML/Audio.h>
#include <SFML/Graphics.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
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

#define SCORE_PER_BRICK 10
#define BALL_SPEED 400.f
#define PADDLE_SPEED 500.f
#define MAX_DT 0.05f
#define TEST_SPEEDUP 60

typedef struct {
    sfSprite *sprite;
    sfVector2f pos;
    sfVector2f vel;
    int bounces;
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

typedef enum {
    STATE_MENU,
    STATE_PLAY,
    STATE_WON,
    STATE_LOST
} state_t;

static const sfUint32 *MESSAGES[] = {
    U"Appuie sur Entrée pour jouer", U"",
    U"Victoire ! Entrée pour rejouer", U"Perdu... Entrée pour rejouer"
};

typedef struct {
    sfRenderWindow *window;
    textures_t tex;
    sfFont *font;
    sfText *score_text;
    sfText *title_text;
    sfText *message_text;
    sfSoundBuffer *buffer;
    sfSound *sound;
    state_t state;
    int plops;
    int score;
    sfClock *clock;
    paddle_t paddle;
    ball_t ball;
    brick_t bricks[BRICK_COUNT];
    int destroyed;
} game_t;

/* Une image unie ; la texture en garde une copie, donc on detruit l'image. */
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

/* Disque blanc sur fond transparent, pixel par pixel. */
static sfImage *ball_image(void)
{
    int size = (int)(BALL_R * 2);
    sfImage *image = sfImage_createFromColor(size, size, sfTransparent);

    for (int y = 0; image != NULL && y < size; y++)
        for (int x = 0; x < size; x++)
            if (in_disc(x, y))
                sfImage_setPixel(image, x, y, sfWhite);
    return image;
}

/* Blanc, de plus en plus sombre vers le bas : la couleur de la ligne
 * sera melangee a cette teinte. */
static sfImage *brick_image(void)
{
    sfImage *image = sfImage_createFromColor(BRICK_W, BRICK_H, sfWhite);
    sfColor shade = sfWhite;

    for (int y = 0; image != NULL && y < BRICK_H; y++) {
        shade.r = shade.g = shade.b = 255 - y * 4;
        for (int x = 0; x < BRICK_W; x++)
            sfImage_setPixel(image, x, y, shade);
    }
    return image;
}

static int create_textures(textures_t *tex)
{
    sfImage *image = sfImage_createFromColor(PADDLE_W, PADDLE_H, sfWhite);

    tex->paddle = texture_from(image);
    tex->ball = texture_from(ball_image());
    tex->brick = texture_from(brick_image());
    return tex->paddle == NULL || tex->ball == NULL || tex->brick == NULL;
}

static sfSprite *make_sprite(const sfTexture *texture, sfColor color)
{
    sfSprite *sprite = sfSprite_create();

    if (sprite == NULL)
        return NULL;
    sfSprite_setTexture(sprite, texture, sfTrue);
    sfSprite_setColor(sprite, color);
    return sprite;
}

static void place_brick(sfSprite *brick, int index)
{
    int col = index % COLS;
    int row = index / COLS;
    sfVector2f pos = {BRICKS_LEFT + col * (BRICK_W + BRICK_GAP),
        BRICKS_TOP + row * (BRICK_H + BRICK_GAP)};

    sfSprite_setPosition(brick, pos);
    sfSprite_setColor(brick, ROW_COLORS[row]);
}

/* La balle repart au centre de l'ecran, vers le haut. */
static void reset_ball(ball_t *ball, float x)
{
    ball->pos = (sfVector2f){x, 350};
    ball->vel = (sfVector2f){BALL_SPEED * 0.5f, -BALL_SPEED * 0.866f};
}

static sfText *make_text(const sfFont *font, unsigned int size)
{
    sfText *text = sfText_create();

    if (text == NULL)
        return NULL;
    sfText_setFont(text, font);
    sfText_setCharacterSize(text, size);
    return text;
}

/* Le titre contient des accents : on passe par setUnicodeString, qui
 * attend des caracteres 32 bits (U"..."), pas de l'UTF-8. */
static int create_texts(game_t *g)
{
    char path[256];
    sfFloatRect lb;

    g->font = sfFont_createFromFile(example_asset(path, sizeof(path),
        "tuffy.ttf"));
    if (g->font == NULL)
        return 1;
    g->score_text = make_text(g->font, 24);
    g->title_text = make_text(g->font, 24);
    g->message_text = make_text(g->font, 32);
    if (!g->score_text || !g->title_text || !g->message_text)
        return 1;
    sfText_setPosition(g->score_text, (sfVector2f){20, 14});
    sfText_setUnicodeString(g->title_text, U"Éclate-les toutes !");
    lb = sfText_getLocalBounds(g->title_text);
    sfText_setOrigin(g->title_text, (sfVector2f){lb.width / 2, 0});
    sfText_setPosition(g->title_text, (sfVector2f){WIN_W / 2, 14});
    return 0;
}

static int create_sound(game_t *g)
{
    char path[256];

    g->buffer = sfSoundBuffer_createFromFile(example_asset(path,
        sizeof(path), "doodle_pop.ogg"));
    if (g->buffer == NULL)
        return 1;
    g->sound = sfSound_create();
    if (g->sound == NULL)
        return 1;
    sfSound_setBuffer(g->sound, g->buffer);
    return 0;
}

static int create_paddle_ball(game_t *g)
{
    sfColor blue = sfColor_fromRGB(200, 220, 255);

    g->paddle.sprite = make_sprite(g->tex.paddle, blue);
    g->ball.sprite = make_sprite(g->tex.ball, sfWhite);
    g->clock = sfClock_create();
    if (!g->paddle.sprite || !g->ball.sprite || !g->clock)
        return 1;
    g->paddle.x = (WIN_W - PADDLE_W) / 2;
    sfSprite_setPosition(g->paddle.sprite,
        (sfVector2f){g->paddle.x, PADDLE_Y});
    sfSprite_setOrigin(g->ball.sprite, (sfVector2f){BALL_R, BALL_R});
    reset_ball(&g->ball, WIN_W / 2);
    return 0;
}

static int create_bricks(game_t *g)
{
    for (int i = 0; i < BRICK_COUNT; i++) {
        g->bricks[i].sprite = make_sprite(g->tex.brick, sfWhite);
        if (g->bricks[i].sprite == NULL)
            return 1;
        g->bricks[i].alive = 1;
        place_brick(g->bricks[i].sprite, i);
    }
    return 0;
}

static void destroy_sound(game_t *g)
{
    if (g->sound != NULL)
        sfSound_destroy(g->sound);
    if (g->buffer != NULL)
        sfSoundBuffer_destroy(g->buffer);
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

static void destroy_texts(game_t *g)
{
    if (g->score_text != NULL)
        sfText_destroy(g->score_text);
    if (g->title_text != NULL)
        sfText_destroy(g->title_text);
    if (g->message_text != NULL)
        sfText_destroy(g->message_text);
    if (g->font != NULL)
        sfFont_destroy(g->font);
}

/* Les sprites d'abord, les textures ensuite : un sprite garde un
 * pointeur vers sa texture. Meme regle pour les textes et la police. */
static void destroy_game(game_t *g)
{
    for (int i = 0; i < BRICK_COUNT; i++)
        if (g->bricks[i].sprite != NULL)
            sfSprite_destroy(g->bricks[i].sprite);
    if (g->ball.sprite != NULL)
        sfSprite_destroy(g->ball.sprite);
    if (g->paddle.sprite != NULL)
        sfSprite_destroy(g->paddle.sprite);
    destroy_texts(g);
    destroy_sound(g);
    destroy_textures(&g->tex);
    if (g->clock != NULL)
        sfClock_destroy(g->clock);
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
        g->state = STATE_LOST;
    }
    sfSprite_setPosition(ball->sprite, ball->pos);
}

/* Balle contre raquette : l'angle depend de l'endroit touche. */
static void hit_paddle(game_t *g)
{
    sfFloatRect bb = sfSprite_getGlobalBounds(g->ball.sprite);
    sfFloatRect pb = sfSprite_getGlobalBounds(g->paddle.sprite);
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
    if (inter.width < inter.height)
        ball->vel.x = -ball->vel.x;
    else
        ball->vel.y = -ball->vel.y;
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
        if (!sfFloatRect_intersects(&bb, &rb, &inter))
            continue;
        g->bricks[i].alive = 0;
        g->destroyed++;
        g->score += SCORE_PER_BRICK;
        sfSound_play(g->sound);
        g->plops++;
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
 * En mode test, aucun clavier : la raquette fonce a gauche. */
static int read_direction(void)
{
    int dir = 0;

    if (example_frames() >= 0)
        return -1;
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
    sfSprite_setPosition(paddle->sprite,
        (sfVector2f){paddle->x, PADDLE_Y});
}

/* En mode test : la texture de la balle a bien un disque opaque au
 * centre et un coin transparent. */
static int check_ball_texture(const sfTexture *texture)
{
    sfImage *image = sfTexture_copyToImage(texture);
    sfVector2u size = sfTexture_getSize(texture);
    int center = 0;
    int corner = 255;

    if (image == NULL)
        return 1;
    center = sfImage_getPixel(image, size.x / 2, size.y / 2).a;
    corner = sfImage_getPixel(image, 0, 0).a;
    sfImage_destroy(image);
    printf("texture de la balle : %ux%u, centre a=%d, coin a=%d\n",
        size.x, size.y, center, corner);
    return size.x != BALL_R * 2 || center != 255 || corner != 0;
}

/* En mode test : la balle reste dans la fenetre, des briques sont
 * tombees et le compte est bon. */
static int check_game(const game_t *g)
{
    int alive = 0;

    for (int i = 0; i < BRICK_COUNT; i++)
        alive += g->bricks[i].alive;
    printf("briques detruites : %d\n", g->destroyed);
    if (alive + g->destroyed != BRICK_COUNT)
        return 1;
    if (g->ball.pos.x < 0 || g->ball.pos.x > WIN_W)
        return 1;
    return 0;
}

/* En mode test : la raquette n'a pas bouge, la balle est tombee : on
 * doit etre dans l'etat "perdu", avec un son joue par brique cassee. */
static int check_state(const game_t *g)
{
    sfTime length = sfSoundBuffer_getDuration(g->buffer);

    printf("etat final : %d (3 = perdu), %d son(s), son de %d ms\n",
        (int)g->state, g->plops, sfTime_asMilliseconds(length));
    return g->state != STATE_LOST || g->plops != g->destroyed
        || sfTime_asMilliseconds(length) <= 0;
}

/* Affiche le message de l'etat courant, centre dans la fenetre. */
static void show_message(game_t *g)
{
    sfFloatRect lb;

    sfText_setUnicodeString(g->message_text, MESSAGES[g->state]);
    lb = sfText_getLocalBounds(g->message_text);
    sfText_setOrigin(g->message_text,
        (sfVector2f){lb.left + lb.width / 2,
            lb.top + lb.height / 2});
    sfText_setPosition(g->message_text, (sfVector2f){WIN_W / 2, WIN_H / 2});
}

static void draw_texts(const game_t *g)
{
    sfRenderWindow_drawText(g->window, g->score_text, NULL);
    sfRenderWindow_drawText(g->window, g->title_text, NULL);
    if (g->state != STATE_PLAY)
        sfRenderWindow_drawText(g->window, g->message_text, NULL);
}

static void draw_game(const game_t *g)
{
    sfRenderWindow_clear(g->window, sfBlack);
    for (int i = 0; i < BRICK_COUNT; i++)
        if (g->bricks[i].alive)
            sfRenderWindow_drawSprite(g->window,
                g->bricks[i].sprite, NULL);
    sfRenderWindow_drawSprite(g->window, g->paddle.sprite, NULL);
    sfRenderWindow_drawSprite(g->window, g->ball.sprite, NULL);
    draw_texts(g);
    sfRenderWindow_display(g->window);
}

/* Le texte du score est reecrit a chaque image : snprintf fabrique la
 * chaine, sfText_setString la copie. */
/* Entree : lance la partie, ou en recommence une apres la fin. */
static void press_enter(game_t *g)
{
    if (g->state == STATE_PLAY)
        return;
    if (g->state != STATE_MENU) {
        for (int i = 0; i < BRICK_COUNT; i++)
            g->bricks[i].alive = 1;
        g->destroyed = 0;
        g->score = 0;
        g->plops = 0;
    }
    reset_ball(&g->ball, WIN_W / 2);
    g->state = STATE_PLAY;
    show_message(g);
}

static void handle_events(game_t *g)
{
    sfEvent event;

    while (sfRenderWindow_pollEvent(g->window, &event)) {
        if (event.type == sfEvtClosed)
            sfRenderWindow_close(g->window);
        if (event.type == sfEvtKeyPressed && event.key.code == sfKeyEnter)
            press_enter(g);
    }
}

static void update_score_text(game_t *g)
{
    char buffer[32];

    snprintf(buffer, sizeof(buffer), "Score : %d", g->score);
    sfText_setString(g->score_text, buffer);
}

/* En mode test : le texte affiche le bon score et le titre garde son
 * premier caractere accentue. */
static int check_texts(const game_t *g)
{
    char want[32];
    const char *got = sfText_getString(g->score_text);
    sfUint32 first = sfText_getUnicodeString(g->title_text)[0];

    snprintf(want, sizeof(want), "Score : %d", g->score);
    printf("texte : \"%s\", premier caractere du titre : U+%04X\n",
        got, (unsigned int)first);
    return strcmp(got, want) != 0 || first != 0xC9
        || g->score != g->destroyed * SCORE_PER_BRICK;
}

static void update_step(game_t *g, float dt)
{
    update_paddle(&g->paddle, read_direction(), dt);
    update_ball(g, dt);
    hit_paddle(g);
    hit_bricks(g);
    if (g->destroyed == BRICK_COUNT)
        g->state = STATE_WON;
    if (g->state != STATE_PLAY)
        show_message(g);
}

/* En mode test, on joue plusieurs pas par image : la partie va vite. */
static void update_game(game_t *g)
{
    float dt = next_dt(g->clock);
    int steps = example_frames() >= 0 ? TEST_SPEEDUP : 1;

    for (int i = 0; i < steps && g->state == STATE_PLAY; i++)
        update_step(g, dt);
    update_score_text(g);
}

static int run(game_t *g)
{
    int frames = example_frames();

    show_message(g);
    while (sfRenderWindow_isOpen(g->window) && frames != 0) {
        handle_events(g);
        if (frames == example_frames())
            press_enter(g);
        update_game(g);
        draw_game(g);
        if (frames > 0)
            frames--;
    }
    if (example_frames() < 0)
        return 0;
    return check_game(g) || check_ball_texture(g->tex.ball)
        || check_texts(g) || check_state(g);
}

static int init_game(game_t *g)
{
    sfVideoMode mode = {WIN_W, WIN_H, 32};

    g->window = sfRenderWindow_create(mode, "Casse-briques",
        sfTitlebar | sfClose, NULL);
    if (g->window == NULL)
        return 1;
    sfRenderWindow_setFramerateLimit(g->window, 60);
    return create_textures(&g->tex) || create_texts(g) || create_sound(g)
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
