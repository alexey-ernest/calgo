/* Finds a crossover n starting from which divide-and-conquer algorithm beats
   a brute force solution */

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#define MAX_LEN 		100000
#define MAX_BUF_LEN 	100
#define MAX_COL_NUM		20
#define MAX_K			1000

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
void _max_subarray_dc(double *, int, int, struct maxsum_t*);
void _max_subarray_x_dc(double *, int, int, int, struct maxsum_t*);
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
	double returns[i];
	_prices_to_returns(arr, returns, i);

	// find a maximum subarray of returns
	struct maxsum_t res1, res2;
	struct timeval t1, t2;
	double elapsed1, elapsed2;
	int k;

	printf("k\tbrute, us\td-n-c, us\n");
	for (k = 1; k < MAX_K; k++) {
		gettimeofday(&t1, NULL);
		_max_subarray_dc(returns, 0, k-1, &res1);
		gettimeofday(&t2, NULL);
		elapsed1 = (t2.tv_sec - t1.tv_sec) * 1000.0 * 1000.0;     // sec to us
    	elapsed1 += (t2.tv_usec - t1.tv_usec);

		gettimeofday(&t1, NULL);
		_max_subarray_brute(returns, k, &res2);
		gettimeofday(&t2, NULL);
		elapsed2 = (t2.tv_sec - t1.tv_sec) * 1000.0 * 1000.0;     // sec to us
    	elapsed2 += (t2.tv_usec - t1.tv_usec);

    	if ((res1.sum - res2.sum) > 0.01 || (res2.sum - res1.sum) > 0.01) {
    		printf("brute and divide-and-conqure algorithms gave different results: %f and %f for k=%i\n", res1.sum, res2.sum, k);
    		return 1;
    	}

    	printf("%i\t%f\t%f\n", k, elapsed2, elapsed1);
	}
	

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

void _max_subarray_dc(double *returns, int p, int r, struct maxsum_t* res) {
	if (p == r) {
		res->sum = returns[p];
		res->l = p;
		res->r = r;
		return;
	}

	int q = (p + r) / 2;
	
	// find max subarray completely in the left part, right part or crosses the middle
	struct maxsum_t lsum, rsum, xsum;
	_max_subarray_dc(returns, p, q, &lsum);
	_max_subarray_dc(returns, q + 1, r, &rsum);
	_max_subarray_x_dc(returns, p, q, r, &xsum);

	if (lsum.sum >= rsum.sum && lsum.sum >= xsum.sum) {
		*res = lsum;
	} else if (rsum.sum >= lsum.sum && rsum.sum >= xsum.sum) {
		*res = rsum;
	} else {
		*res = xsum;
	}
}

void _max_subarray_x_dc(double *returns, int p, int q, int r, struct maxsum_t* res) {
	int i, j, imax, jmax;
	double lsum, rsum, lsummax, rsummax;
	lsum = 0.0;
	rsum = 0.0;
	lsummax = (int)~(~(0u) >> 1);
	rsummax = lsummax;
	for (i = q; i >= p; i--) {
		lsum += returns[i];
		if (lsum > lsummax) {
			lsummax = lsum;
			imax = i;
		}
	}
	for (j = q + 1; j <= r; j++) {
		rsum += returns[j];
		if (rsum > rsummax) {
			rsummax = rsum;
			jmax = j;
		}
	}

	res->sum = lsummax + rsummax;
	res->l = imax;
	res->r = jmax;
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