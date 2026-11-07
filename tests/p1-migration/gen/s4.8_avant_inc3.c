/* wrapper généré — l'extrait est copié tel quel entre les marqueurs */
#include <CSFML/Graphics.h>
#include <CSFML/Audio.h>
#include <stdbool.h>
extern sfRenderWindow *win; extern sfEvent event; extern sfSprite *spr;
extern sfBool paused; extern const sfTexture *tileset;
void shoot(int x, int y); void resize(unsigned w, unsigned h);
void zoom(int d); void start(void); void hit(void);
int block(void)
{
/* ---- DEBUT EXTRAIT ---- */
/* AVANT (2.6.1) : 4 sommets, primitive sfQuads */
sfVertexArray *va = sfVertexArray_create();
sfVertexArray_setPrimitiveType(va, sfQuads);
/* append a, b, c, d */
sfRenderStates st = sfRenderStates_default();
st.texture = tileset;
sfRenderWindow_drawVertexArray(win, va, &st);

/* ---- FIN EXTRAIT ---- */
return 0;
}
