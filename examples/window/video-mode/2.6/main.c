#include <SFML/Window.h>
#include <stdio.h>

static void print_mode(const char *label, sfVideoMode m)
{
    printf("%s : %ux%u, %u bits\n", label, m.width, m.height,
        m.bitsPerPixel);
}

int main(void)
{
    sfVideoMode desktop = sfVideoMode_getDesktopMode();
    const sfVideoMode *modes = NULL;
    size_t count = 0;

    print_mode("mode du bureau", desktop);
    modes = sfVideoMode_getFullscreenModes(&count);
    printf("%zu mode(s) plein ecran\n", count);
    for (size_t i = 0; i < count && i < 5; i++)
        print_mode("  mode", modes[i]);
    printf("bureau valide : %d\n", sfVideoMode_isValid(desktop));
    if (desktop.width == 0 || desktop.height == 0)
        return 1;
    if (count > 0 && !sfVideoMode_isValid(modes[0]))
        return 1;
    return 0;
}
