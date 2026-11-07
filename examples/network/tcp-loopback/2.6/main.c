#include <SFML/Network.h>
#include <SFML/System.h>
#include <stdio.h>
#include <string.h>

#define ANY_PORT 0

static sfTcpSocket *accept_client(sfTcpListener *listener)
{
    sfTcpSocket *connected = NULL;

    for (int i = 0; i < 100; i++) {
        if (sfTcpListener_accept(listener, &connected) == sfSocketDone)
            return connected;
        sfSleep(sfMilliseconds(5));
    }
    return NULL;
}

static int read_message(sfTcpSocket *socket, char *buf, size_t size)
{
    size_t got = 0;

    for (int i = 0; i < 100; i++) {
        if (sfTcpSocket_receive(socket, buf, size, &got) == sfSocketDone)
            return 0;
        sfSleep(sfMilliseconds(5));
    }
    return 1;
}

static int talk(sfTcpListener *listener, sfTcpSocket *client)
{
    unsigned short port = sfTcpListener_getLocalPort(listener);
    sfTcpSocket *peer = NULL;
    char buf[64] = {0};
    int status = 1;

    printf("listener sur le port %u\n", port);
    if (sfTcpSocket_connect(client, sfIpAddress_LocalHost, port,
        sfMilliseconds(500)) != sfSocketDone)
        return 1;
    peer = accept_client(listener);
    if (peer != NULL) {
        sfTcpSocket_send(client, "hello", 5);
        sfTcpSocket_setBlocking(peer, sfFalse);
        if (read_message(peer, buf, sizeof(buf) - 1) == 0)
            status = strcmp(buf, "hello") != 0;
        printf("le serveur a recu : %s\n", buf);
    }
    sfTcpSocket_destroy(peer);
    return status;
}

int main(void)
{
    sfTcpListener *listener = sfTcpListener_create();
    sfTcpSocket *client = sfTcpSocket_create();
    int status = 1;

    if (listener && client) {
        sfTcpListener_setBlocking(listener, sfFalse);
        if (sfTcpListener_listen(listener, ANY_PORT, sfIpAddress_LocalHost)
            == sfSocketDone)
            status = talk(listener, client);
    }
    sfTcpSocket_destroy(client);
    sfTcpListener_destroy(listener);
    printf(status ? "ECHEC\n" : "OK\n");
    return status;
}
