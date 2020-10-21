#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <errno.h>

#define MSGSIZE 150

struct message {
    long msgType;
    char msgText[MSGSIZE];
};

int main(int argc, char* argv[]) {
    int msgID, status, start, end, counter = 0;
    struct message msg;
    struct msqid_ds buffer;

    if (argc != 3) {
        printf("Synopsis: %s <key> <type>\n", argv[0]);
        return 1;
    }

    //msgget(key, flag) fordert eine vorhandene Nachrichtenwarteschlange an
    msgID = msgget((key_t)atoi(argv[1]), 0);
    printf("\n msgID: %i", msgID);


    if (msgID == -1) {
        printf("Auf Nachrichtenwarteschlange mit dem Schluessel \"%s\" konnte nicht zugegriffen werden!\n", argv[1]);
        return 2;
    }

    //Nachricht aus der Schlange erhalten
    if(msgrcv(msgID, &msg, MSGSIZE, atoi(argv[2]), IPC_NOWAIT | MSG_NOERROR) < 0) {
        if (errno == ENOMSG) {
            printf("Keine Nachricht vom Typ %s vorhanden!\n", argv[2]);
        } else {
            printf("Es ist ein Fehler aufgetreten! Die Nachricht konnte nicht gelesen werden!\n");
            return 3;
        }
    } else {
        printf("[%ld] %s\n", msg.msgType, msg.msgText);
    }

    if(msgctl(msgID, IPC_STAT, &buffer) == 0) {
        if(buffer.msg_qnum <= 0) {
            if(msgctl(msgID, IPC_RMID, &buffer) == 0) {
                printf("Die Nachrichtenwarteschlange beinhaltet keine Nachrichten mehr und wurde geloescht!\n");
            }
        }
        return 0;
    }
}
