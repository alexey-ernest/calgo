/* Find a maximum k constant, where sort_insert(k) faster than sort_merge(k) */

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#define MAX_BUF_LEN 	12
#define MAX_ARR_LEN 	1000
#define INF				(1u << 31) - 1

int _getnum();
void _sort_insert(int *arr, int n);

/* merge sort, optimized with sentinels */
void _sort_merge_s(int*, int, int);
void _merge_s(int*, int, int, int);

int main(int argc, char const *argv[])
{
	int arr[MAX_ARR_LEN];
	int i = 0;
	int num;

	// read an array from stdin
	while ((num = _getnum()) != EOF && i < MAX_ARR_LEN) {
		arr[i++] = num;
	}

	// measure sorting times
	int tmp[MAX_ARR_LEN];
	int j, k;
	struct timeval t1, t2;
	double elapsed1, elapsed2;
	int c = 0;

	printf("n\tinsert, ms\tmerge, ms\n");
	for (k = 1; k <= i; k++) {
		// insert sort
		for (j = 0; j < k; j++) {
			tmp[j] = arr[j];
		}
		gettimeofday(&t1, NULL);
		_sort_insert(tmp, k);
		gettimeofday(&t2, NULL);
		elapsed1 = (t2.tv_sec - t1.tv_sec) * 1000.0;     // sec to ms
    	elapsed1 += (t2.tv_usec - t1.tv_usec) / 1000.0;  // us to ms

    	// merge sort
    	for (j = 0; j < k; j++) {
			tmp[j] = arr[j];
		}
		gettimeofday(&t1, NULL);
		_sort_merge_s(tmp, 0, k - 1);
		gettimeofday(&t2, NULL);
		elapsed2 = (t2.tv_sec - t1.tv_sec) * 1000.0;     // sec to ms
    	elapsed2 += (t2.tv_usec - t1.tv_usec) / 1000.0;  // us to ms

    	printf("%i\t%f\t%f\n", k, elapsed1, elapsed2);
    	
    	if ((elapsed1 - elapsed2) >= 0.001) {
    		c++;
    	} else {
    		c = 0;
    	}
    	if (c > 10) {
    		break;
    	}
	}

	return 0;
}

void _sort_insert(int *arr, int n) {
	int i, j, k;
	for (i = 1; i < n; i++) {
		k = arr[i];
		for (j = i - 1; j >= 0 && arr[j] > k; j--) {
			arr[j + 1] = arr[j];
		}
		arr[j + 1] = k;
	}
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