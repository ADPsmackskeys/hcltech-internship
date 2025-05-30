#include <stdio.h>
#include <stdint.h>

int main() {
	// Integer Overflow
	uint32_t balance = 100000;
	int32_t signed_input;
	printf("Old Balance: %u\n", balance);
	printf("Enter Transaction amount (positive for depositing, negative for withdrawing): \n");
	fflush(stdout);
	scanf("%d", &signed_input);
	balance += signed_input;
	printf("New Balance: %u\n", balance);
    
	// Heap Overflow
	uint32_t current_year = 2025;
	int32_t birth_year;
	float cost[] = {100, 400, 500, 300, 200 };
	size_t max_age = 100;
	float *ptr = cost;
	float i;
	for (i = 0; i < 6; i++) {
	  printf ("Value in heap, memory location %p, is %f\n", ptr, *(ptr));
  	ptr++;
	}
  
	printf("Enter your year of birth: ");
	scanf("%d", &birth_year);
	int32_t age = current_year - birth_year;
	printf("Derived age: %d\n", age);
	printf("Cost for your age group: %.2f\n", cost [age / 20]);
	return 0;
}
