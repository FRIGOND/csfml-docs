#include <CSFML/Graphics.h>
#include "test_mode.h"

int main(void)
{
    sfVideoMode mode = {{800, 600}, 32};
    sfRenderWindow *window = sfRenderWindow_create(mode, "Ma fenetre",
        sfTitlebar | sfClose, sfWindowed, NULL);
    sfEvent event;
    int frames = example_frames();

    if (window == NULL)
        return 1;
    sfRenderWindow_setFramerateLimit(window, 60);
    while (sfRenderWindow_isOpen(window) && frames != 0) {
        while (sfRenderWindow_pollEvent(window, &event))
            if (event.type == sfEvtClosed)
                sfRenderWindow_close(window);
        sfRenderWindow_clear(window, sfBlack);
        sfRenderWindow_display(window);
        if (frames > 0)
            frames--;
    }
    sfRenderWindow_destroy(window);
    return 0;
}
