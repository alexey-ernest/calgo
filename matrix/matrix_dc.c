/* Matrix multiplication, divide-and-conquer (O(n^3) time and O(n^3) space) */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_VAL			100
#define BRUTE_SIZE_OPT	4

struct _matrix {
	int rows;
	int cols;
	int **data;
};

struct _matrix* _gen_rand_matrix(int);
void _mul_square_dc(struct _matrix*, struct _matrix*, int[4], int[4], struct _matrix*);
void _mul_square_dc_cache(struct _matrix*, struct _matrix*, int[4], int[4], struct _matrix*, int, struct _matrix ***);
void _mul_square_brute(struct _matrix *, struct _matrix *, int[4], int[4], struct _matrix *);
void _free_matrix(struct _matrix*);
void _print(struct _matrix*);
void _certify_mul(struct _matrix*, struct _matrix*, struct _matrix*);

int main(int argc, char const *argv[])
{
	// init random generator
	srand(time(NULL));

	if (argc < 2) {
		printf("usage: matrix_dc 1024\n");
		return 1;
	}

	int n = atoi(argv[1]);
	if ((n & (n-1)) != 0) {
		printf("matrix dimensions should be a power of 2\n");
		return 1;
	}

	struct _matrix *m1 = _gen_rand_matrix(n);
	struct _matrix *m2 = _gen_rand_matrix(n);

	// allocating result matrix
	struct _matrix *mul = (struct _matrix*)malloc(sizeof(struct _matrix));
	mul->rows = n;
	mul->cols = n;
	mul->data = (int **)malloc(n * sizeof(int *));
	int i;
	for (i = 0; i < n; i++) {
		mul->data[i] = (int *)malloc(n * sizeof(int));
	}

	// allocating multi-level caches for temporal calculations
	int k = n;
	int d = 0;
	while (k > 0) {
		k = k >> 1;
		d++;
		if (k <= BRUTE_SIZE_OPT) {
			break;
		}		
	}

	struct _matrix ***cache = (struct _matrix ***)malloc(d * sizeof(struct _matrix**));
	int j, p = n;
	for (k = 0; k < d; k++) {
		cache[k] = (struct _matrix **)malloc(8 * sizeof(struct _matrix*));
		for (i = 0; i < 8; i++) {
			cache[k][i] = (struct _matrix*)malloc(sizeof(struct _matrix));
			cache[k][i]->rows = p/2;
			cache[k][i]->cols = p/2;
			cache[k][i]->data = (int **)malloc(p/2 * sizeof(int *));
			for (j = 0; j < p/2; j++) {
				cache[k][i]->data[j] = (int *)malloc(p/2 * sizeof(int));
			}
		}
		p = p/2;
	}

	int ind[4] = {0, n-1, 0, n-1};
	_mul_square_dc_cache(m1, m2, ind, ind, mul, 0, cache);
	
	// _print(m1);
	// printf("\n");
	// _print(m2);
	// printf("\n");
	// _print(mul);
	
	// _certify_mul(m1, m2, mul);

	// de-allocate the cache
	for (k = 0; k < d - 1; k++) {
		for (i = 0; i < 8; i++) {
			_free_matrix(cache[k][i]);
			free(cache[k][i]);
		}
		free(cache[k]);
	}
	free(cache);

	_free_matrix(m1);
	_free_matrix(m2);
	_free_matrix(mul);
	free(m1);
	free(m2);
	free(mul);

	return 0;
}

void _mul_square_brute(struct _matrix *m1, struct _matrix *m2, 
	int ind1[4], int ind2[4], struct _matrix *res) {

	int n = ind1[1] - ind1[0] + 1;
	if (res->rows != res->cols || res->rows != n) {
		printf("incompatible inputs or the output\n");
		return;
	}

	int i, j, k;
	for (i = 0; i < n; i++) {
		for (j = 0; j < n; j++) {
			res->data[i][j] = 0;
			for (k = 0; k < n; k++) {
				res->data[i][j] += m1->data[ind1[0] + i][ind1[2] + k] * m2->data[ind2[0] + k][ind2[2] + j];
			}
		}
	}
}

void _certify_mul(struct _matrix* m1, struct _matrix* m2, struct _matrix* res) {
	int i, j, k, r;
	int n = m1->rows;
	for (i = 0; i < n; i++) {
		for (j = 0; j < n; j++) {
			r = 0;
			for (k = 0; k < n; k++) {
				r += m1->data[i][k] * m2->data[k][j];
			}
			if (r != res->data[i][j]) {
				printf("matrix multiplication is incorrect\n");
				return;
			}
		}
	}
	printf("matrix multiplication is correct\n");
}

void _print(struct _matrix* m) {
	int i, j;
	for (i = 0; i < m->rows; i++) {
		for (j = 0; j < m->cols; j++) {
			printf("%10i ", m->data[i][j]);
		}
		printf("\n");
	}
}

void _mul_square_dc(struct _matrix *m1, struct _matrix *m2, 
	int ind1[4], int ind2[4], struct _matrix *res) {
	if ((ind1[1] - ind1[0]) != (ind1[3] - ind1[2]) || 
		(ind2[1] - ind2[0]) != (ind2[3] - ind2[2]) ||
		(ind1[1] - ind1[0]) != (ind2[1] - ind2[0])) {
		// incompatible
		printf("error: incompatible matrices\n");
		return;
	}

	int i1 = ind1[0], i2 = ind1[1], j1 = ind1[2], j2 = ind1[3],
		p1 = ind2[0], p2 = ind2[1], r1 = ind2[2], r2 = ind2[3];
	int n = i2 - i1 + 1;
	if (res->rows != n || res->cols !=n) {
		// incompatible result dimension
		printf("error: incompatible matrices\n");
		return;
	}

	if (n == 1) {
		// base case
		res->data[0][0] = m1->data[i1][j1] * m2->data[p1][r1];
		return;
	}
	
	// divide and conquer
	int q = (i2 - i1) / 2; // mid point relative index
	// a11*b11 + a12*b21, a12*b22 + a11*b12
	// a21*b11 + a22*b21, a22*b22 + a21*b12

	// calculate indices: O(1)
	int a[4][4] = {
		{i1, i1 + q, j1, j1 + q}, 		{i1, i1 + q, j1 + q + 1, j2},
		{i1 + q + 1, i2, j1, j1 + q}, 	{i1 + q + 1, i2, j1 + q + 1, j2}
	};
	int b[4][4] = {
		{p1, p1 + q, r1, r1 + q}, 		{p1, p1 + q, r1 + q + 1, r2},
		{p1 + q + 1, p2, r1, r1 + q}, 	{p1 + q + 1, p2, r1 + q + 1, r2}
	};

	// solve sub-problems: 8*T(n/2) time and O(n^2) space
	struct _matrix subs[8];
	int k, i;
	for (k = 0; k < 8; k++) {
		subs[k].rows = n/2;
		subs[k].cols = n/2;
		subs[k].data = (int **)malloc(n/2 * sizeof(int *));
		for (i = 0; i < n/2; i++) {
			subs[k].data[i] = (int *)malloc(n/2 * sizeof(int));
		}
	}
	_mul_square_dc(m1, m2, a[0], b[0], &subs[0]);
	_mul_square_dc(m1, m2, a[1], b[2], &subs[1]);
	_mul_square_dc(m1, m2, a[1], b[3], &subs[2]);
	_mul_square_dc(m1, m2, a[0], b[1], &subs[3]);
	_mul_square_dc(m1, m2, a[2], b[0], &subs[4]);
	_mul_square_dc(m1, m2, a[3], b[2], &subs[5]);
	_mul_square_dc(m1, m2, a[3], b[3], &subs[6]);
	_mul_square_dc(m1, m2, a[2], b[1], &subs[7]);

	// combine the results: O(n^2)
	int j;
	for (i = 0; i < n/2; i++) {
		for (j = 0; j < n/2; j++) {
			// a11*b11 + a12*b21
			res->data[i][j] = subs[0].data[i][j] + subs[1].data[i][j];
			// a12*b22 + a11*b12
			res->data[i][j+q+1] = subs[2].data[i][j] + subs[3].data[i][j];
			// a21*b11 + a22*b21
			res->data[i+q+1][j] = subs[4].data[i][j] + subs[5].data[i][j];
			// a22*b22 + a21*b12
			res->data[i+q+1][j+q+1] = subs[6].data[i][j] + subs[7].data[i][j];
		}
	}
	
	// total: T(n) = 8*T(n/2) + O(n^2)

	// free memory
	for (k = 0; k < 8; k++) {
		_free_matrix(&subs[k]);
	}
}

void _mul_square_dc_cache(struct _matrix *m1, struct _matrix *m2, 
	int ind1[4], int ind2[4], struct _matrix *res, int clevel, struct _matrix ***cache) {
	if ((ind1[1] - ind1[0]) != (ind1[3] - ind1[2]) || 
		(ind2[1] - ind2[0]) != (ind2[3] - ind2[2]) ||
		(ind1[1] - ind1[0]) != (ind2[1] - ind2[0])) {
		// incompatible
		printf("error: incompatible matrices\n");
		return;
	}

	int i1 = ind1[0], i2 = ind1[1], j1 = ind1[2], j2 = ind1[3],
		p1 = ind2[0], p2 = ind2[1], r1 = ind2[2], r2 = ind2[3];
	int n = i2 - i1 + 1;
	if (res->rows != n || res->cols != n) {
		// incompatible result dimension
		printf("error: incompatible result domensions, it should %ix%i\n", n, n);
		return;
	}

	if (n <= BRUTE_SIZE_OPT) {
		// using brute force algorithm for smaller dimensions
		_mul_square_brute(m1, m2, ind1, ind2, res);
		return;
	}

	// if (n == 1) {
	// 	// base case
	// 	res->data[0][0] = m1->data[i1][j1] * m2->data[p1][r1];
	// 	return;
	// }
	
	// divide and conquer
	int q = (i2 - i1) / 2; // mid point relative index
	// a11*b11 + a12*b21, a12*b22 + a11*b12
	// a21*b11 + a22*b21, a22*b22 + a21*b12

	// calculate indices: O(1)
	int a[4][4] = {
		{i1, i1 + q, j1, j1 + q}, 		{i1, i1 + q, j1 + q + 1, j2},
		{i1 + q + 1, i2, j1, j1 + q}, 	{i1 + q + 1, i2, j1 + q + 1, j2}
	};
	int b[4][4] = {
		{p1, p1 + q, r1, r1 + q}, 		{p1, p1 + q, r1 + q + 1, r2},
		{p1 + q + 1, p2, r1, r1 + q}, 	{p1 + q + 1, p2, r1 + q + 1, r2}
	};

	// solve sub-problems: 8*T(n/2) time and O(n^2) space
	struct _matrix **subs = cache[clevel];
	_mul_square_dc_cache(m1, m2, a[0], b[0], subs[0], clevel+1, cache);
	_mul_square_dc_cache(m1, m2, a[1], b[2], subs[1], clevel+1, cache);
	_mul_square_dc_cache(m1, m2, a[1], b[3], subs[2], clevel+1, cache);
	_mul_square_dc_cache(m1, m2, a[0], b[1], subs[3], clevel+1, cache);
	_mul_square_dc_cache(m1, m2, a[2], b[0], subs[4], clevel+1, cache);
	_mul_square_dc_cache(m1, m2, a[3], b[2], subs[5], clevel+1, cache);
	_mul_square_dc_cache(m1, m2, a[3], b[3], subs[6], clevel+1, cache);
	_mul_square_dc_cache(m1, m2, a[2], b[1], subs[7], clevel+1, cache);

	// combine the results: O(n^2)
	int i, j;
	for (i = 0; i < n/2; i++) {
		for (j = 0; j < n/2; j++) {
			// a11*b11 + a12*b21
			res->data[i][j] = subs[0]->data[i][j] + subs[1]->data[i][j];
			// a12*b22 + a11*b12
			res->data[i][j+q+1] = subs[2]->data[i][j] + subs[3]->data[i][j];
			// a21*b11 + a22*b21
			res->data[i+q+1][j] = subs[4]->data[i][j] + subs[5]->data[i][j];
			// a22*b22 + a21*b12
			res->data[i+q+1][j+q+1] = subs[6]->data[i][j] + subs[7]->data[i][j];
		}
	}
}

void _free_matrix(struct _matrix *m) {
	int i;
	for (i = 0; i < m->rows; i++) {
		free(m->data[i]);
	}
	free(m->data);
}

struct _matrix* _gen_rand_matrix(int n) {
	int max_int = (int)(~(0u) >> 1);

	int i, j;
	struct _matrix *res = (struct _matrix*)malloc(sizeof(struct _matrix));
	res->rows = n;
	res->cols = n;
	res->data = (int**)malloc(n * sizeof(int*));
	for (i = 0; i < n; i++) {
		res->data[i] = (int*)malloc(n * sizeof(int));
		for (j = 0; j < n; j++) {
			res->data[i][j] = (float)rand() / (float)max_int * MAX_VAL;
		}
	}

	return res;
}