#include <CSFML/Network.h>
#include <CSFML/System.h>
#include <stdio.h>
#include <string.h>

#define ANY_PORT sfUdpSocket_anyPort()

static int receive(sfUdpSocket *socket, char *buf, size_t size, size_t *got)
{
    sfIpAddress from;
    unsigned short port;

    for (int i = 0; i < 100; i++) {
        if (sfUdpSocket_receive(socket, buf, size, got, &from, &port)
            == sfSocketDone)
            return 0;
        sfSleep(sfMilliseconds(5));
    }
    return 1;
}

static int exchange(sfUdpSocket *server, sfUdpSocket *client)
{
    unsigned short port = sfUdpSocket_getLocalPort(server);
    char buf[64] = {0};
    size_t got = 0;

    printf("serveur lie au port %u\n", port);
    if (sfUdpSocket_send(client, "salut", 5, sfIpAddress_LocalHost, port)
        != sfSocketDone || receive(server, buf, sizeof(buf) - 1, &got))
        return 1;
    printf("recu %zu octets : %s\n", got, buf);
    return strcmp(buf, "salut") != 0;
}

int main(void)
{
    sfUdpSocket *server = sfUdpSocket_create();
    sfUdpSocket *client = sfUdpSocket_create();
    int status = 1;

    if (server && client) {
        sfUdpSocket_setBlocking(server, false);
        if (sfUdpSocket_bind(server, ANY_PORT, sfIpAddress_LocalHost)
            == sfSocketDone)
            status = exchange(server, client);
    }
    sfUdpSocket_destroy(client);
    sfUdpSocket_destroy(server);
    printf(status ? "ECHEC\n" : "OK\n");
    return status;
}
