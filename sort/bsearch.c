/* Binary search algorithm (on a sorted array from stdin) */

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#define MAX_BUF_LEN		12
#define MAX_ARR_LEN 	10000000

int _getnum();
int _bsearch(int*, int, int);

int main(int argc, char const *argv[])
{
	if (argc == 1) {
		printf("usage: bsearch 25");
		return 1;
	}

	int key = atoi(argv[1]);

	int *arr = (int *)malloc(MAX_ARR_LEN * sizeof(int));
	int i = 0;
	int lastnum, num;
	lastnum = ~((~0u) >> 1);

	// read an array from stdin
	while ((num = _getnum()) != EOF && i < MAX_ARR_LEN) {
		if (num < lastnum) {
			printf("error: an array should be in a non-decreasing order");
			return 1;
		}
		arr[i++] = num;
		lastnum = num;
	}

	// search the key in the array
	struct timeval t1, t2;
	double elapsed;

	gettimeofday(&t1, NULL);
	i = _bsearch(arr, i, key);
	gettimeofday(&t2, NULL);

	elapsed = (t2.tv_sec - t1.tv_sec) * 1000.0;     // sec to ms
    elapsed += (t2.tv_usec - t1.tv_usec) / 1000.0;  // us to ms

	if (i < 0) {
		printf("the key %i could not be found in %fms\n", key, elapsed);
		return 2;
	}

	printf("the key %i has been found at position %i in %fms\n", key, i, elapsed);

	return 0;
}

int _bsearch(int *a, int n, int k) {
	int p, q, r;
	p = 0;
	r = n - 1;
	while (p <= r) {
		q = (p + r) / 2;
		if (a[q] == k) {
			return q;
		} else if (a[q] < k) {
			p = q + 1;
		} else {
			r = q - 1;
		}
	}

	return -1;
}

char _buf[MAX_BUF_LEN];
int _getnum() {
	int c;
	char *ptr = _buf;

	// skip spaces until we get a start of a new number
	for (c = getchar(); c == '\n' || c == ' '; c = getchar());

	// read chars of a number
	while (c != EOF && c != '\n' && c != ' ' 
		&& ptr < _buf + MAX_BUF_LEN - 1) {
		*ptr++ = c;
		c = getchar();
	}

	*ptr = '\0';

	if (ptr == _buf && c == EOF) {
		return EOF;
	}

	return atoi(_buf);
}