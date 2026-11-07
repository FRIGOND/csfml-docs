#include <CSFML/System.h>
#include <stdio.h>

static int check_conversions(void)
{
    sfTime t = sfSeconds(1.5f);
    int ms = sfTime_asMilliseconds(t);
    long long us = (long long)sfTime_asMicroseconds(t);

    printf("1.5 s = %d ms = %lld us\n", ms, us);
    t = sfMilliseconds(250);
    printf("250 ms = %.3f s\n", sfTime_asSeconds(t));
    t = sfMicroseconds(2000000);
    printf("2000000 us = %.1f s\n", sfTime_asSeconds(t));
    return !(ms == 1500 && us == 1500000);
}

static int check_clock(void)
{
    sfClock *clock = sfClock_create();
    sfTime lap;
    sfTime after;

    if (clock == NULL)
        return 1;
    sfSleep(sfMilliseconds(50));
    lap = sfClock_restart(clock);
    after = sfClock_getElapsedTime(clock);
    printf("restart a renvoye %d ms\n", sfTime_asMilliseconds(lap));
    printf("temps ecoule depuis : %d ms\n", sfTime_asMilliseconds(after));
    sfClock_destroy(clock);
    return !(sfTime_asMilliseconds(lap) >= 45
        && sfTime_asMicroseconds(after) < sfTime_asMicroseconds(lap));
}

int main(void)
{
    int status = check_conversions();

    status |= check_clock();
    printf(status ? "ECHEC\n" : "OK\n");
    return status;
}
