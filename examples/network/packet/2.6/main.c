#include <SFML/Network.h>
#include <stdio.h>
#include <string.h>

static void fill(sfPacket *packet)
{
    sfPacket_writeInt32(packet, -42);
    sfPacket_writeFloat(packet, 3.5f);
    sfPacket_writeString(packet, "bonjour");
    sfPacket_writeBool(packet, sfTrue);
}

static int read_back(sfPacket *packet)
{
    char text[64];
    sfInt32 number = sfPacket_readInt32(packet);
    float real = sfPacket_readFloat(packet);
    sfBool flag;

    sfPacket_readString(packet, text);
    flag = sfPacket_readBool(packet);
    printf("int=%d float=%.1f string=%s sfBool=%d\n", number, real, text, flag);
    return !(number == -42 && real == 3.5f && strcmp(text, "bonjour") == 0
        && flag && sfPacket_endOfPacket(packet));
}

int main(void)
{
    sfPacket *packet = sfPacket_create();
    int status;

    if (packet == NULL)
        return 1;
    fill(packet);
    printf("taille des donnees : %zu octets\n", sfPacket_getDataSize(packet));
    status = read_back(packet);
    printf(status ? "ECHEC du round-trip\n" : "round-trip OK\n");
    sfPacket_destroy(packet);
    return status;
}
