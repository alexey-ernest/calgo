/* For any integers 0 < a < b, produce heads (1) with
   probability a/b and tails (0) with probabiliy (b-a)/b */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char const *argv[])
{
	// init random generator
	srand(time(NULL));

	if (argc < 4) {
		printf("usage: a_div_b_prob 8 10 1000, where a=8, b=10, a < b, n=1000 trials.\n");
		return 1;
	}

	int a = atoi(argv[1]);
	int b = atoi(argv[2]);
	int n = atoi(argv[3]);

	if (a == 0 || b == 0 || n == 0 || b <= a) {
		printf("usage: a, b and n integers should satisfy conditions 0 < a < b, n > 0.\n");
		return 1;
	}

	int k, l;
	l = b;
	k = 0;
	while (l != 0) {
		k++;
		l = l >> 1;
	}

	printf("lg(b)=%i bits\n", k);

	int i, j;
	int h, t;
	h = 0;
	t = 0;
	unsigned int mask;
	int num;
	float toss;
	int head_c = 0;
	for (i = 0; i < n; i++) {

		mask = 1u;
		num = 0;
		
		// toss a coin k times to set random bits in a number
		for (j = 0; j < k; j++) {
			// tossing a fair coin
			toss = (float)rand() / (float)RAND_MAX;
			if (toss < 0.5) {
				// head
				num |= mask;
			}
			mask = mask << 1;
		}

		// normalize the number by b
		num = num % b;

		// if a number less than a, it falls in a probability a/b
		if (num < a) {
			head_c++;
		}
	}

	printf("%i heads in %i trials\n", head_c, n);

	return 0;
}