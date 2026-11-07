#include <CSFML/System.h>
#include <math.h>
#include <stdio.h>

/* Pas de fonctions mathematiques sur les vecteurs : on les ecrit. */
static sfVector2f add(sfVector2f a, sfVector2f b)
{
    sfVector2f r = {a.x + b.x, a.y + b.y};

    return r;
}

static float length(sfVector2f v)
{
    return sqrtf(v.x * v.x + v.y * v.y);
}

static sfVector3f cross(sfVector3f a, sfVector3f b)
{
    sfVector3f r = {a.y * b.z - a.z * b.y,
        a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x};

    return r;
}

int main(void)
{
    sfVector2f a = {3.0f, 4.0f};
    sfVector2f b = {1.0f, 2.0f};
    sfVector2f sum = add(a, b);
    sfVector2i pixel = {-5, 12};
    sfVector2u size = {800, 600};
    sfVector3f x = {1.0f, 0.0f, 0.0f};
    sfVector3f y = {0.0f, 1.0f, 0.0f};
    sfVector3f z = cross(x, y);

    printf("a + b = (%.1f, %.1f)\n", sum.x, sum.y);
    printf("|a| = %.1f\n", length(a));
    printf("sfVector2i (%d, %d), sfVector2u (%u, %u)\n",
        pixel.x, pixel.y, size.x, size.y);
    printf("x ^ y = (%.1f, %.1f, %.1f)\n", z.x, z.y, z.z);
    return !(sum.x == 4.0f && sum.y == 6.0f && length(a) == 5.0f
        && z.z == 1.0f);
}
