#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
	char buffer[256];
	printf("%p\n", buffer);  
	gets(buffer);        	
	printf("%s\n", buffer);
	return 0;
}
