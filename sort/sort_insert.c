/* Insertion sort algorithm implementation for an uint array, read from stdin */

#include <stdio.h>
#include <stdlib.h>

#define MAX_BUF_LEN 12
#define MAX_ARR_LEN 1000000

int _getnum();
void _sort_insert(int *arr, int n);

int main(int argc, char const *argv[])
{
	int arr[MAX_ARR_LEN];
	int i = 0;
	int num;

	// read an array from stdin
	while ((num = _getnum()) != EOF && i < MAX_ARR_LEN) {
		arr[i++] = num;
	}

	// sort the array using insertion sort algorithm
	_sort_insert(arr, i);

	// print the result
	int n = i;
	for (i = 0; i < n; i++) {
		printf("%d\n", arr[i]);
	}

	return 0;
}

void _sort_insert(int *arr, int n) {
	int i, j, k;
	for (i = 1; i < n; i++) {
		for (j = i - 1; arr[j] > arr[j+1] && j >= 0; j--) {
			k = arr[j];
			arr[j] = arr[j + 1];
			arr[j + 1] = k;
		}
	}
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