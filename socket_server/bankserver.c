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

int com(int sd, struct sockaddr_in sin, int *sin_len);
int aus(FILE *sockstream, int *sum);
int ein(FILE *sockstream, int *sum);
int stand(FILE *sockstream, int *sum);

int main() {
    int sd;
    struct sockaddr_in sin;

    //Binden einer Adresse an das Socket
    int sin_len = sizeof sin;
    sin.sin_family = AF_INET;
    sin.sin_addr.s_addr = INADDR_ANY;
    sin.sin_port = htons(SERVER_PORT);

    //Anlegen eines Sockets (Rendez-Vous-Descriptor)
    sd = socket(AF_INET, SOCK_STREAM, 0);
    bind(sd, (struct sockaddr *)& sin, sizeof(sin));

    //UNIX-Kern muss wissen, dass wir an diesem Socket Verbindungen akzeptieren
    listen(sd, 5);

    //Socket-Descriptor wird an die Hauptfunktion übergeben
    com(sd, sin, &sin_len);

    //Sobald das Programm aus der Hauptfunktion zurückkehrt, wird das Socket geschlossen
    unlink((const char *)& sin);
    return(EXIT_SUCCESS);

}

int com(int sd, struct sockaddr_in sin, int *sin_len) {
    int n_sd, choose, sum = 1000;
    FILE *sockstream;

    char *wel = "Herzlich Willkommen bei der G-Bank.\n";
    char *next = "Geld abheben (1)\nGeld einzahlen (2)\nKontostand abfragen (3)\nVerbindung beenden (4)\nIhre Eingabe: ";
    char *ciao = "Vielen Dank. Sie haben einen einfachen Geldautomaten sehr glücklich gemacht rofl.\n\n";

    while(1) {
        n_sd = accept(sd, (struct sockaddr *)& sin, sin_len);
        sockstream = fdopen(n_sd, "r+");
        fprintf(sockstream, "%s", wel);
        rewind(sockstream);
        while(1) {
            fprintf(sockstream, "%s", next);
            rewind(sockstream);
            fscanf(sockstream, "%i", &choose);
            rewind(sockstream);
            if(choose == 1) {
                aus(sockstream, &sum);
            }
            if(choose == 2) {
                ein(sockstream, &sum);
            }
            if(choose == 3) {
                stand(sockstream, &sum);
            }
            if(choose == 4) {
                fprintf(sockstream, "%s", ciao);
                rewind(sockstream);
                break;
            }
        }
        close(n_sd);
    }
    return(EXIT_SUCCESS);
}

int aus(FILE *sockstream, int *sum) {
    int in_sum;
    char *ask = "Wieviel Geld möchten Sie abheben?";
    char *tell = "Ihr neuer Kontostand:";
    fprintf(sockstream, "%s", ask);
    rewind(sockstream);
    fscanf(sockstream, "%i", &in_sum);
    rewind(sockstream);
    *sum -= in_sum;
    fprintf(sockstream, "%s%i\n\n", tell, *sum);
    rewind(sockstream);

    return(EXIT_SUCCESS);
}

int ein(FILE *sockstream, int *sum) {
    int in_sum;
    char *ask = "Wieviel Geld möchten Sie einzahlen?";
    char *tell = "Ihr neuer Kontostand:";
    fprintf(sockstream, "%s", ask);
    rewind(sockstream);
    fscanf(sockstream, "%i", &in_sum);
    rewind(sockstream);
    *sum += in_sum;
    fprintf(sockstream, "%s%i\n\n", tell, *sum);
    rewind(sockstream);

    return(EXIT_SUCCESS);
}

int stand(FILE *sockstream, int *sum) {
    char *tell = "Ihr neuer Kontostand:";
    fprintf(sockstream, "%s%i\n\n", tell, *sum);
    rewind(sockstream);

    return(EXIT_SUCCESS);
}
