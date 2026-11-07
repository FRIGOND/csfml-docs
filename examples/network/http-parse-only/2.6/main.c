#include <SFML/Network.h>
#include <stdio.h>

/* sfHttpRequest n'a aucun getter : on construit la requete et on affiche
 * ce qu'on y a mis, sans jamais appeler sfHttp_sendRequest (pas de reseau). */
int main(void)
{
    sfHttpRequest *request = sfHttpRequest_create();
    sfHttp *http = sfHttp_create();

    if (request == NULL || http == NULL) {
        sfHttpRequest_destroy(request);
        sfHttp_destroy(http);
        return 1;
    }
    sfHttpRequest_setMethod(request, sfHttpPost);
    sfHttpRequest_setUri(request, "/api/score");
    sfHttpRequest_setHttpVersion(request, 1, 1);
    sfHttpRequest_setField(request, "Content-Type", "text/plain");
    sfHttpRequest_setBody(request, "score=42");
    sfHttp_setHost(http, "http://localhost", 8080);
    printf("POST /api/score HTTP/1.1 -> http://localhost:8080\n");
    printf("requete construite, rien n'est envoye\n");
    sfHttpRequest_destroy(request);
    sfHttp_destroy(http);
    return 0;
}
