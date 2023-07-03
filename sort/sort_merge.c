/* Merge sort algorithm, read from stdin */

#include <stdio.h>
#include <stdlib.h>

#define MAX_BUF_LEN 12
#define MAX_ARR_LEN 1000000

int _getnum();
void _sort_merge(int*, int, int);
void _merge(int *, int, int, int);

int main(int argc, char const *argv[])
{
	int arr[MAX_ARR_LEN];
	int i = 0;
	int num;

	// read an array from stdin
	while ((num = _getnum()) != EOF && i < MAX_ARR_LEN) {
		arr[i++] = num;
	}

	// sort the array using merge sort algorithm
	_sort_merge(arr, 0, i - 1);

	// print the result
	int n = i;
	for (i = 0; i < n; i++) {
		printf("%d\n", arr[i]);
	}

	return 0;
}

void _sort_merge(int *a, int p, int r) {
	if (p >= r)
		return;
	int q = (p + r) / 2; // greatest int <= q
	_sort_merge(a, p, q);
	_sort_merge(a, q + 1, r);
	_merge(a, p, q, r);
}

int inf = (1u << 31) - 1;

void _merge(int *a, int p, int q, int r) {
	int *la, *ra;
	int n1, n2, i, j, k;

	// copy left and right parts of the array
	n1 = q - p + 1;
	n2 = r - q;
	la = (int *)malloc((n1 + 1) * sizeof(int));
	ra = (int *)malloc((n2 + 1) * sizeof(int));
	k = p;
	for (i = 0; i < n1; i++) {
		la[i] = a[k++];
	}
	for (j = 0; j < n2; j++) {
		ra[j] = a[k++];
	}

	// add sentinel elements to the end
	la[n1] = inf;
	ra[n2] = inf;

	// merge
	k = p;
	i = 0;
	j = 0;
	while (k <= r) {
		if (la[i] < ra[j]) {
			a[k++] = la[i++];
		} else {
			a[k++] = ra[j++];
		}
	}

	free((void *)la);
	free((void *)ra);
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