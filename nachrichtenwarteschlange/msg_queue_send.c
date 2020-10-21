#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#define MSGSIZE 150

struct message {
    long msgType;
    char msgText[MSGSIZE];
};

int main(int argc, char* argv[]) {
    int msgID;
    struct message msg;

    if (argc != 4) {
        printf("Usage: %s <key> <type> <text>\n", argv[0]);
        return 1;
    }

    if (atoi(argv[1]) == 0) {
        printf("Der Schluessel \"%s\" ist ungueltig!\n", argv[1]);
        return 2;
    }

    //msgget(key, flag) legt eine neue Nachrichtenwarteschlange an
    msgID = msgget((key_t)atoi(argv[1]), IPC_CREAT | 0600);
    printf("\n msgID: %i", msgID);

    //Nachricht anlegen
    msg.msgType = atoi(argv[2]);
    if (msg.msgType == 0) {
        printf("Der Typ \"%s\" ist ungueltig!\n", argv[2]);
        return 4;
    }

    strncpy(msg.msgText, argv[3], MSGSIZE-1);
    msg.msgText[MSGSIZE-1] = '\0';

    //Nachricht in die Schlange senden
    if(msgsnd(msgID, &msg, strlen(argv[3])+1, IPC_NOWAIT) < 0) {
        printf("Die Nachricht konnte nicht gesendet werden!\n");
        return 5;
    }

    return 0;
}
