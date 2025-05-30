#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void secret () {
	printf ("Hacked! You reached the secret function!\n");
	exit(0);
}

void vulnerable () {
	char buffer[64];
	printf ("Input");
	fgets (buffer, 256, stdin);
}

int main () {
	vulnerable();
	return 0;
}	
