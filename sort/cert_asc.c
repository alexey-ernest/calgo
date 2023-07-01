/* Certifies that an input array is in the ascending order */

#include <stdio.h>
#include <stdlib.h>

#define MAX_BUF_LEN 12

int _getnum();

int main(int argc, char const *argv[])
{
	int lastnum, num, n;

	// read an array from stdin
	n = 0;
	while ((num = _getnum()) != EOF) {
		n++;
		if (num < lastnum) {
			printf("non-decreasing order is not satisfied at the number %d.\n", num);
			return 1;
		}

		lastnum = num;
	}

	printf(
		"non-decreasing order of an array of size %d has been successfully certified.\n", 
		n);

	return 0;
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