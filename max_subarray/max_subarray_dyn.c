/* Find a consequetive subarray with the maximum sum (dynamic in O(n) time and space) */

#include <stdio.h>
#include <stdlib.h>

#define MAX_LEN 		100000000
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
void _prices_to_returns(struct price_t*, double *, int);
void _max_subarray_dyn(double *, int, struct maxsum_t*);

int main(int argc, char const *argv[])
{
	struct price_t *arr = malloc(MAX_LEN * sizeof(struct price_t));
	struct price_t p;
	int i = 0;
	while (_getprice(&p) != EOF && i < MAX_LEN) {
		if (p.timestamp != NULL && p.price != 0.0) {
			arr[i++] = p;
		}
	}

	// make the prices in historical order
	_reverse_prices(arr, i);

	// convert prices to daily returns
	double *returns = malloc(i * sizeof(double));
	_prices_to_returns(arr, returns, i);

	// find a maximum subarray of returns
	struct maxsum_t res;
	_max_subarray_dyn(returns, i, &res);
	printf("%f\t[%f, %f]\t[%s, %s]\n", 
		res.sum, arr[res.l-1].price, arr[res.r].price, 
		arr[res.l-1].timestamp, arr[res.r].timestamp);

	free(returns);
	free(arr);
	return 0;
}

void _max_subarray_dyn(double *returns, int n, struct maxsum_t* res) {
	res->sum = (int)(~((~0u) >> 1)); // minimum int32
	res->l = -1;
	res->r = -1;
	if (n < 2) {
		return;
	}

	struct maxsum_t *dyn, *at;
	dyn = malloc(n * sizeof(struct maxsum_t)); // dynamic array of results for each i [0,n)
	at = malloc(n * sizeof(struct maxsum_t)); 	// max subarray ending for each index i [0,n)

	int i;
	dyn[1] = (struct maxsum_t) {1, 1, returns[1]};
	at[1] = (struct maxsum_t) {1, 1, returns[1]};
	for (i = 2; i < n; i++) { // O(n)
		// calculate max subarray ending at index i
		// we either extend existing array of start a new one
		if (at[i-1].sum + returns[i] > returns[i]) {
			at[i] = (struct maxsum_t){at[i-1].l, i, at[i-1].sum + returns[i]};
		} else {
			at[i] = (struct maxsum_t){i, i, returns[i]};
		}

		// calculate a solution for current i
		if (dyn[i-1].sum > at[i].sum) {
			dyn[i] = dyn[i-1];
		} else {
			dyn[i] = at[i];
		}
	}

	*res = dyn[n-1];

	free(dyn);
	free(at);
}

void _prices_to_returns(struct price_t *p, double *r, int n) {
	int i;
	r[0] = 0.0;
	for (i = 1; i < n; i++) {
		r[i] = p[i].price - p[i-1].price;
	}
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