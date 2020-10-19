#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

void main() {
	static char *command[] ={"date", "ls"};
	int zahl;

	while (1) {
		printf("\n Bitte geben Sie eine Zahl ein: ");
		printf("\n 0 = date, 1 = files\n");
		scanf("%i", &zahl);

		//Prozess erfolglos
		if (!(zahl == 0 | zahl == 1)) {
			exit(1);
		}

		//Kindprozess
		if (fork() == 0) {
			execlp(command[zahl], command[zahl], NULL);
			printf("\nFalsche Zahl. Kein Kommando verfügbar.");
			exit(1);
		} else {
			printf("\n Vaterprozess wartet");
			wait(0); //Vaterprozess wartet auf Ende des Kindprozesses
		}
	}
}
