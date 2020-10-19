#include <stdio.h>
#include <unistd.h>

void main() {
	int pid_des_kindes;

	printf("\n V steht für Vaterprozess");
	printf("\n K steht für Kindprozess");

	pid_des_kindes = fork();

	//Fehlerbehandlung führt zum Programmabbruch
	if (pid_des_kindes < 0) {
		perror("\nFork war nicht erfolgreich!");
	}

	//Kindprozess
	if (pid_des_kindes == 0) {
		printf("\n K: Die ID dieses Prozesses ist: %i", getpid());
		printf("\n K: Die ID des Vaters ist: %i", getppid());
	}

	//Vaterprozess
	if (pid_des_kindes > 0) {
		printf("\n V: Die ID dieses Prozesses ist: %i", getpid());
		printf("\n V: Die ID des Vaters ist: %i", getppid());
	}
}
