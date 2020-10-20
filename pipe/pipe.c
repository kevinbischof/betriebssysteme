#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

void main() {
    int pipe_verbindung[2];
    pipe(pipe_verbindung);

    //Erzeugung des ersten Sohnprozesses
    if (fork() == 0) {
    dup2(pipe_verbindung[1],1);
    close(pipe_verbindung[0]);
    execlp("who", "who", NULL);
    } else if (fork() == 0) {
        //Erzeugung des zweiten Sohnprozesses
        dup2(pipe_verbindung[0], 0);
        close(pipe_verbindung[1]);
        execlp("sort", "sort", NULL);
    } else {
        //Vaterprozess
        close(pipe_verbindung[0]);
        close(pipe_verbindung[1]);
        wait(0);
        wait(0);
    }
}
