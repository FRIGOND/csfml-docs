#include <CSFML/Network.h>
#include <stdio.h>
#include <string.h>

/* toString ecrit au plus 16 octets : "255.255.255.255" + '\0'. */
#define IP_SIZE 16

static int check(const char *label, sfIpAddress ip, const char *expected)
{
    char text[IP_SIZE];

    sfIpAddress_toString(ip, text);
    printf("%-12s -> %s\n", label, text);
    return strcmp(text, expected) != 0;
}

int main(void)
{
    sfIpAddress parsed = sfIpAddress_fromString("192.168.1.56");
    sfIpAddress bytes = sfIpAddress_fromBytes(10, 0, 0, 7);
    sfIpAddress back = sfIpAddress_fromInteger(sfIpAddress_toInteger(parsed));
    int status = 0;

    status |= check("fromString", parsed, "192.168.1.56");
    status |= check("fromBytes", bytes, "10.0.0.7");
    status |= check("fromInteger", back, "192.168.1.56");
    status |= check("LocalHost", sfIpAddress_LocalHost, "127.0.0.1");
    status |= check("Broadcast", sfIpAddress_Broadcast, "255.255.255.255");
    /* 3.0.0 : adresse invalide = chaine vide (octets a zero). */
    status |= check("invalide", sfIpAddress_fromString("pas une ip!"),
        "");
    printf("127.0.0.1 en entier : %u\n",
        (unsigned)sfIpAddress_toInteger(sfIpAddress_LocalHost));
    return status;
}
