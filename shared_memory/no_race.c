#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/wait.h>
#define MAXCOUNT 100000
#define NUM_OF_CHILDS 4
#define SEGSIZE sizeof(int)
    /*
    struct sembuf {
        //Nummer des Semaphors innerhalb der Gruppe
        short sem_num;
        //Auszuführende Operation
        //Wert > 0 -> UP-Operation
        //Wert < 0 -> DOWN-Operation
        short sem_op;
        //Flag zur Steuerung der Operationen
        short sem_flg;
    };
*/
int main() {



    int i, shm_id, sem_id, *shar_mem, pid[NUM_OF_CHILDS], count = 0;
    //Structs für Semaphore
    struct sembuf up, down;

    unsigned short marker[1];

    //Anlegen einer Semaphorengruppe
    sem_id = semget (IPC_PRIVATE, 1, IPC_CREAT|0644);

    marker[0] = 1;
    //
    semctl(sem_id, 1, SETALL, marker);
    //Anforderung des Shared Memory Segments
    shm_id = shmget(IPC_PRIVATE, SEGSIZE, IPC_CREAT|0644);

    shar_mem = (int *)shmat(shm_id, 0, 0);
    *shar_mem = 0;

    //Semaphor 0 in der Gruppe
    down.sem_num = 0;
    //blockieren
    down.sem_op = -1;
    down.sem_flg = SEM_UNDO;
    //Semaphor 0 in der Gruppe
    up.sem_num = 0;
    //freigeben
    up.sem_op = 1;
    up.sem_flg = SEM_UNDO;

    //Vaterprozess erzeugt eine bestimmte Anzahl Kindprozesse
    for (i = 0; i < NUM_OF_CHILDS; i++) {
        pid[i] = fork();

        if(pid[i] == 0) {
            printf("\nKindprozess %i", i);
            while (1) {
                //Eintritt in den kritischen Bereich
                printf("\nKind %i tritt in den kritischen Bereich ein", i);
                semop(sem_id, &down, 1);
                if (*shar_mem < MAXCOUNT) {
                    *shar_mem += 1;
                    count++;
                    //Verlassen des kritischen Bereichs
                    printf("\nKind %i verlässt den kritischen Bereich", i);
                    semop(sem_id, &up, 1);
                } else {
                    //Verlassen des kritischen Bereichs
                    printf("\nKind %i verlässt den kritischen Bereich", i);
                    semop(sem_id, &up, 1);
                    break;
                }
            }
            printf("\n%ites Kind erhöhte Wert um %i!", i, count);
            return 0;
        }
    }

    //Vaterprozess wartet, bis alle Kindprozesse beendet sind.
    for (i = 0; i < NUM_OF_CHILDS; i++) {
        printf("\nVaterprozess wartet auf Kind %i", i);
        waitpid(pid[i], NULL, 0);
        printf("\nVaterprozess wartet nicht mehr auf Kind %i", i);
    }

    printf("\nShared M. = %i, MAXCOUNT = %i\n", *shar_mem, MAXCOUNT);

    //Semaphorengruppe und das Shared Memory werden wieder freigegeben

    shmdt(shar_mem);
    shmctl(shm_id, IPC_RMID, 0);
    semctl(sem_id, 0, IPC_RMID);

    return 0;
}
