#include <SFML/Graphics.h>
#include <stdio.h>
#include "test_mode.h"

static sfInt64 on_read(void *data, sfInt64 size, void *user)
{
    return (sfInt64)fread(data, 1, size, user);
}

static sfInt64 on_seek(sfInt64 position, void *user)
{
    if (fseek(user, (long)position, SEEK_SET) != 0)
        return -1;
    return (sfInt64)position;
}

static sfInt64 on_tell(void *user)
{
    return (sfInt64)ftell(user);
}

static sfInt64 on_size(void *user)
{
    long here = ftell(user);
    long end;

    fseek(user, 0, SEEK_END);
    end = ftell(user);
    fseek(user, here, SEEK_SET);
    return (sfInt64)end;
}

int main(void)
{
    char path[256];
    FILE *file = fopen(example_asset(path, sizeof(path), "sfml_logo.png"),
        "rb");
    sfInputStream stream = {on_read, on_seek, on_tell, on_size, file};
    sfImage *image = NULL;
    sfVector2u size;

    if (file == NULL)
        return 1;
    image = sfImage_createFromStream(&stream);
    if (image != NULL) {
        size = sfImage_getSize(image);
        printf("image lue par callbacks : %u x %u\n", size.x, size.y);
        sfImage_destroy(image);
    }
    fclose(file);
    return image == NULL;
}
