/* Generate a pseudo-random uint array with values between MIN and MAX 
and print it to stdout */

#include <time.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char const *argv[])
{
	if (argc < 4) {
		printf("usage: gen_random 1000000 100 1000\n");
		return 1;
	}

	int num = atoi(argv[1]);
	if (num <= 0) {
		printf("array length must be a positive number.");
		return 1;
	}

	int min, max;
	min = atoi(argv[2]);
	max = atoi(argv[3]);

	// init random generator
	srand(time(NULL));

	double f;
	int int_max = (int)(~(0u) >> 1);
	while (num-- > 0) {
		f = (double)min + (double)rand() / (double)int_max * (max - min);
		printf("\"%i\",\"%i\"\n", num + 1, (int)f);
	}

	return 0;
}