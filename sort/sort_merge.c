/* Merge sort algorithm, read from stdin.
   Options: -Os for sentinel optimization;
   			-Oi for insertion sort on smaller sub-arrays optimization;
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_BUF_LEN 	12
#define MAX_ARR_LEN 	10000000
#define INF				(1u << 31) - 1
#define INSERT_SORT_LEN	32 // array size to sort with the insertion sort

int _getnum();

/* classic merge sort */
void _sort_merge(int*, int, int);
void _merge(int*, int, int, int);

/* optimized with sentinels */
void _sort_merge_s(int*, int, int);
void _merge_s(int*, int, int, int);

/* optimized with insertion sort for smaller sub-arrays */
void _sort_merge_i(int*, int, int);
void _sort_insert(int*, int, int);

char _opt = '0';

int main(int argc, char const *argv[])
{
	while (--argc > 0) {
		++argv;
		if (*(*argv)++ != '-')
			continue;
		switch (**argv) {
			case 'O':
				switch (*++(*argv)) {
					case 's':
						_opt = 's';
						break;
					case 'i':
						_opt = 'i';
						break;
					default:
						printf("unknown optimization param %s\n", *argv);
						return 2;
				}
				break;
			default: 
				printf("unknown option %s\n", *argv);
				return 1;
		}
	}

	int *arr = (int *)malloc(MAX_ARR_LEN * sizeof(int));
	int i = 0;
	int num;

	// read an array from stdin
	while ((num = _getnum()) != EOF && i < MAX_ARR_LEN) {
		arr[i++] = num;
	}

	// sort the array using merge sort algorithm
	switch (_opt) {
		case 's': 
			_sort_merge_s(arr, 0, i - 1);
			break;
		case 'i':
			_sort_merge_i(arr, 0, i - 1);
			break;
		default:
			_sort_merge(arr, 0, i - 1);
	}	

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

void _merge(int *a, int p, int q, int r) {
	int *la, *ra;
	int n1, n2, i, j, k;

	// copy left and right parts of the array
	n1 = q - p + 1;
	n2 = r - q;
	la = (int *)malloc(n1 * sizeof(int));
	ra = (int *)malloc(n2 * sizeof(int));
	k = p;
	for (i = 0; i < n1; i++) {
		la[i] = a[k++];
	}
	for (j = 0; j < n2; j++) {
		ra[j] = a[k++];
	}

	// merge
	k = p;
	i = 0;
	j = 0;
	while (i < n1 && j < n2) {
		if (la[i] < ra[j]) {
			a[k++] = la[i++];
		} else {
			a[k++] = ra[j++];
		}
	}

	// copy tales
	// if (i < n1) {
	// 	memcpy((void *)(a + k), (void *)(la + i), (n1 - i) * sizeof(int));
	// } else {
	// 	memcpy((void *)(a + k), (void *)(ra + j), (n2 - j) * sizeof(int));
	// }
	while (i < n1) {
		a[k++] = la[i++];
	}
	while (j < n2) {
		a[k++] = ra[j++];
	}

	free((void *)la);
	free((void *)ra);
}

void _sort_merge_s(int *a, int p, int r) {
	if (p >= r)
		return;
	int q = (p + r) / 2; // greatest int <= q
	_sort_merge_s(a, p, q);
	_sort_merge_s(a, q + 1, r);
	_merge_s(a, p, q, r);
}

void _merge_s(int *a, int p, int q, int r) {
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
	la[n1] = INF;
	ra[n2] = INF;

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

void _sort_insert(int *a, int p, int r) {
	int i, j, k;
	for (i = p + 1; i <= r; i++) {
		k = a[i];
		for (j = i - 1; j >= p && a[j] > k; j--) {
			a[j + 1] = a[j];
		}
		a[j + 1] = k;
	}
}

void _sort_merge_i(int *a, int p, int r) {
	if (p >= r)
		return;

	if (r - p + 1 <= INSERT_SORT_LEN) {
		// utilizing the insertion sort algorithm for smaller sub-arrays
		_sort_insert(a, p, r);
		return;
	}

	int q = (p + r) / 2; // greatest int <= q
	_sort_merge_i(a, p, q);
	_sort_merge_i(a, q + 1, r);
	_merge_s(a, p, q, r);
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