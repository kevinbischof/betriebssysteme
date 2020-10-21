#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>

#define MAXCOUNT 1000000
#define NUM_CHILDREN 4
#define SHMSEGSIZE sizeof(int)

int main() {
    int i, shmID, *shared_mem, count = 0;
    int pid[NUM_CHILDREN];

    shmID = shmget(IPC_PRIVATE, SHMSEGSIZE, IPC_CREAT | 644);
    shared_mem = (int *)shmat(shmID, 0, 0);
    *shared_mem = 0;

    for(int i = 0; i < NUM_CHILDREN; i++) {
        pid[i] = fork();

        if(pid[i] == -1) {
            printf("%iter Kindprozess nicht erzeugbar!\n", i);
            return 0;
        }

        if(pid[i] == 0) {
            while (*shared_mem < MAXCOUNT) {
                *shared_mem += 1;
                count++;
            }

            printf("%ites Kind erhöhte Wert um %i!\n", i, count);
            shmdt(shared_mem);
            return 0;
        }
    }

    for (i = 0; i < NUM_CHILDREN; i++) {
        waitpid(pid[i], NULL, 0);
    }

    printf("Shared M. = %i - MAXCOUNT = %i\n", *shared_mem, MAXCOUNT);
    shmdt(shared_mem);
    shmctl(shmID, IPC_RMID, 0);
    return 0;
}
