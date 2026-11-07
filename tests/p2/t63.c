#include <CSFML/System.h>
int main(void){ sfTime t=sfClock_getElapsedTime(NULL); return (int)t.microseconds; }
