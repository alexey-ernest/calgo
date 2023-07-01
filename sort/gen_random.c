/* Generate a pseudo-random uint array with values between 0 and RAND_MAX 
and print it to stdout */

#include <time.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char const *argv[])
{
	if (argc < 2) {
		printf("usage: gen_random 1000000\n");
		return 1;
	}

	int num = atoi(argv[1]);
	if (num <= 0) {
		printf("array length must be a positive number.");
		return 1;
	}

	// init random generator
	srand(time(NULL));

	while (num-- > 0) {
		printf("%i\n", rand());
	}

	return 0;
}