/* Find a consequetive subarray with the maximum sum (brute force in O(n^2)) */

#include <stdio.h>
#include <stdlib.h>

#define MAX_LEN 		100000
#define MAX_BUF_LEN 	100
#define MAX_COL_NUM		20

struct price_t {
	char 	*timestamp;
	double 	price;
};

struct maxsum_t {
	int 	l;
	int   	r;
	double 	sum;
};

int _getprice(struct price_t*);
void _reverse_prices(struct price_t*, int);
double *_prices_to_returns(struct price_t*, int);
void _max_subarray_brute(double *, int, struct maxsum_t*);

int main(int argc, char const *argv[])
{
	struct price_t arr[MAX_LEN];
	struct price_t p;
	int i = 0;
	while (_getprice(&p) != EOF) {
		if (p.timestamp != NULL && p.price > 0.0) {
			arr[i++] = p;
		}
	}

	// make the prices in historical order
	_reverse_prices(arr, i);

	// convert prices to daily returns
	double *returns = _prices_to_returns(arr, i);
	// int j;
	// for (j = 0; j < i; j++) {
	// 	printf("%f\n", returns[j]);
	// }

	// find a maximum subarray of returns
	struct maxsum_t res;
	_max_subarray_brute(returns, i, &res);
	printf("%f\t[%f, %f]\t[%s, %s]\n", 
		res.sum, arr[res.l-1].price, arr[res.r].price, 
		arr[res.l-1].timestamp, arr[res.r].timestamp);

	return 0;
}

void _max_subarray_brute(double *returns, int n, struct maxsum_t* res) {
	int i, j, maxi, maxj;
	double sum, maxsum;
	maxsum = (int)(~((~0u) >> 1));
	for (i = 0; i < n; i++) {
		sum = 0.0;
		for (j = i; j < n; j++) {
			sum += returns[j];
			if (sum > maxsum) {
				maxsum = sum;
				maxi = i;
				maxj = j;
			}
		}
	}
	res->sum = maxsum;
	res->l = maxi;
	res->r = maxj;
}

double *_prices_to_returns(struct price_t *p, int n) {
	int i;
	double *returns = malloc(n * sizeof(double));
	returns[0] = 0.0;
	for (i = 1; i < n; i++) {
		returns[i] = p[i].price - p[i-1].price;
	}

	return returns;
}

void _reverse_prices(struct price_t *prices, int n) {
	int i;
	struct price_t p;
	for (i = 0; i <= n/2; i++) {
		p = prices[i];
		prices[i] = prices[n - i - 1];
		prices[n - i - 1] = p;
	}
}

void _price_remove_commas(char *s) {
	int i, j;
	for (i = 0, j = 0; s[i]; i++) {
		if (s[i] != ',') {
			s[j++] = s[i];
		}
	}
	s[j] = '\0';
}

int _getprice(struct price_t *p) {
	char *buf = (char *)malloc(MAX_BUF_LEN);
	char *cells[MAX_COL_NUM];
	char *ptr = buf;
	int c, j, q;
	j = 0;
	q = 0;
	while ((c = getchar()) != EOF && c != '\n' && ptr < buf + MAX_BUF_LEN - 1) {
		*ptr++ = c;
		if (c == '"') {
			// a new cell started or just ended
			*(ptr-1) = '\0';
			
			q++;
			if (q % 2 != 0) {
				// starting a new cell right after an opening quote
				cells[j++] = ptr;
			}
		}
	}
	*ptr = '\0';

	if (ptr > buf) {
		if (j > 1) {
			// fill in the price struct
			p->timestamp = cells[0];

			// remove commas from the price string
			_price_remove_commas(cells[1]);
			p->price = atof(cells[1]);
		} else {
			p->timestamp = buf + 1;
		}

		return (ptr - buf);
	}

	return EOF;
}