/* wrapper généré — l'extrait est copié tel quel entre les marqueurs */
#include <SFML/Graphics.h>
#include <SFML/Audio.h>
#include <stdbool.h>
extern sfRenderWindow *win; extern sfEvent event; extern sfSprite *spr;
extern bool paused; extern const sfTexture *tileset;
void shoot(int x, int y); void resize(unsigned w, unsigned h);
void zoom(float d); void start(void); void hit(void);
int block(void)
{
/* ---- DEBUT EXTRAIT ---- */
/* APRÈS (3.0.0) : 6 sommets (2 triangles) par quad */
sfVertexArray *va = sfVertexArray_create();
sfVertexArray_setPrimitiveType(va, sfTriangles);
/* append a, b, c puis a, c, d */
sfRenderStates st = sfRenderStates_default;    /* variable, plus de () */
st.texture = tileset;
sfRenderWindow_drawVertexArray(win, va, &st);

/* ---- FIN EXTRAIT ---- */
return 0;
}
