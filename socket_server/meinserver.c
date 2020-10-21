#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <netinet/in.h>
#define SERVER_PORT 4711

typedef enum {false, true} bool;

//Internet Socket-Adressenstruktur
/*
struct sockaddr_in {
    short           sin_family;
    unsigned short  sin_port;
    struct in_addr  sin_addr;
    char            sin_zero[8];
};

struct in_addr {
    unsigned long   s_addr;
};
*/

int main() {
    //Anlegen eines Sockets (Rendez-Vous-Descriptor)
    int sock = socket(AF_INET, SOCK_STREAM, 0);

    //Binden einer Adresse an das Socket
    struct sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(SERVER_PORT);

    //UNIX-Kern muss wissen, dass wir an diesem Socket Verbindungen akzeptieren
    listen(sock, 5);

    //Server wartet auf Verbindungsanfrage eines Clients. fileDescriptor = Verbindungsdescriptor
    struct sockaddr_in *client;
    int client_len = sizeof client;
    int fileDescriptor = accept(sock, (struct sockaddr *)&client, &client_len);

    char in[2000];
    char out[2000];

    while(true) {
        while(read(fileDescriptor, in, 2000) > 0) {
            write(fileDescriptor, out, 2000);
        }
    }
    close(fileDescriptor);
}
