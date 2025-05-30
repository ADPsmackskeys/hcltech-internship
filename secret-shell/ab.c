#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

void secret () {
	printf ("This is a secret function. Hello!\n");
	if (setuid (0) != -1)
    	printf ("This is Root\n");
	else
    	printf ("Launching normal shell (no root)\n");
	system ("/bin/sh -i");
    
	exit(0);
}

void vulnerable () {
	char buffer[64];
	printf ("Input: \n");
	fgets (buffer, 256, stdin);
}

int main () {

	vulnerable();
	return 0;
}
