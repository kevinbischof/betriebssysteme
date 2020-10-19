#include <stdio.h>
#include <unistd.h>

void main() {
	int i;
	if (fork()) {
		//Vaterprozess
		for (i = 0; i < 500; i++) {
			printf("\n Vater: %i", i);
		}
	} else {
		//Kindprozess
		for (i = 0; i < 500; i++) {
			printf("\n Kind: %i", i);
		}
	}
}
