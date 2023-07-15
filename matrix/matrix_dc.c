/* Matrix multiplication, divide-and-conquer (O(n^3)) */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_VAL	1000

struct _matrix {
	int rows;
	int cols;
	int **data;
};

struct _matrix* _gen_rand_matrix(int);
struct _matrix* _mul_square_dc(struct _matrix*, struct _matrix*, int[4], int[4]);
void _free_matrix(struct _matrix*);
void _add_matrix(struct _matrix*, struct _matrix*, struct _matrix*, int, int);
void _print(struct _matrix*);

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

	int ind[4] = {0, n-1, 0, n-1};
	struct _matrix *mul = _mul_square_dc(m1, m2, ind, ind);
	
	// _print(m1);
	// printf("\n");
	// _print(m2);
	// printf("\n");
	// _print(mul);
	
	_free_matrix(m1);
	_free_matrix(m2);
	_free_matrix(mul);

	return 0;
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

struct _matrix* _mul_square_dc(struct _matrix *m1, struct _matrix *m2, 
	int ind1[4], int ind2[4]) {
	if (m1->rows != m1->cols || m2->rows != m2->cols || 
		m1->rows != m2->rows) {
		return NULL;
	}
	if ((ind1[1] - ind1[0]) != (ind1[3] - ind1[2]) || 
		(ind2[1] - ind2[0]) != (ind2[3] - ind2[2]) ||
		(ind1[1] - ind1[0]) != (ind2[1] - ind2[0])) {
		// incompatible
		return NULL;
	}

	int n = ind1[1] - ind1[0] + 1;

	// init res matrix
	struct _matrix *res = (struct _matrix*)malloc(sizeof(struct _matrix));
	res->rows = n;
	res->cols = n;
	res->data = (int **)malloc(n * sizeof(int*));
	int i;
	for (i = 0; i < n; i++) {
		res->data[i] = (int *)malloc(n * sizeof(int));
	}

	if (n == 1) {
		// base case
		res->data[0][0] = m1->data[ind1[0]][ind1[2]] * m2->data[ind2[0]][ind2[2]];
		return res;
	}
	
	// divide and conquer sub-problems
	int q = (ind1[1] - ind1[0]) / 2; // mid point
	// a11*b11 + a12*b21, a11*b12 + a12*b22
	// a21*b11 + a22*b21, a21*b12 + a22*b22
	int i1 = ind1[0], i2 = ind1[1], j1 = ind1[2], j2 = ind1[3],
		p1 = ind2[0], p2 = ind2[1], r1 = ind2[2], r2 = ind2[3];
	int a[4][4] = {
		{i1, i1 + q, j1, j1 + q}, 		{i1, i1 + q, j1 + q + 1, j2},
		{i1 + q + 1, i2, j1, j1 + q}, 	{i1 + q + 1, i2, j1 + q + 1, j2}
	};
	int b[4][4] = {
		{p1, p1 + q, r1, r1 + q}, 		{p1, p1 + q, r1 + q + 1, r2},
		{p1 + q + 1, p2, r1, r1 + q}, 	{p1 + q + 1, p2, r1 + q + 1, r2}
	};

	struct _matrix *a11b11 = _mul_square_dc(m1, m2, a[0], b[0]);
	struct _matrix *a12b21 = _mul_square_dc(m1, m2, a[1], b[2]);
	struct _matrix *a11b12 = _mul_square_dc(m1, m2, a[0], b[1]);
	struct _matrix *a12b22 = _mul_square_dc(m1, m2, a[1], b[3]);
	struct _matrix *a21b11 = _mul_square_dc(m1, m2, a[2], b[0]);
	struct _matrix *a22b21 = _mul_square_dc(m1, m2, a[3], b[2]);
	struct _matrix *a21b12 = _mul_square_dc(m1, m2, a[2], b[1]);
	struct _matrix *a22b22 = _mul_square_dc(m1, m2, a[3], b[3]);

	// merge results
	_add_matrix(a11b11, a12b21, res, 0, 0);
	_add_matrix(a11b12, a12b22, res, 0, q + 1);
	_add_matrix(a21b11, a22b21, res, q + 1, 0);
	_add_matrix(a21b12, a22b22, res, q + 1, q + 1);

	// free memory
	_free_matrix(a11b11);
	_free_matrix(a12b21);
	_free_matrix(a11b12);
	_free_matrix(a12b22);
	_free_matrix(a21b11);
	_free_matrix(a22b21);
	_free_matrix(a21b12);
	_free_matrix(a22b22);

	return res;
}

void _add_matrix(struct _matrix* m1, struct _matrix* m2, struct _matrix* res, 
	int p, int q) {
	int i, j;
	for (i = 0; i < m1->rows; i++) {
		for (j = 0; j < m1->cols; j++) {
			res->data[p + i][q + j] = m1->data[i][j] + m2->data[i][j];
		}
	}
}

void _free_matrix(struct _matrix *m) {
	int i;
	for (i = 0; i < m->rows; i++) {
		free(m->data[i]);
	}
	free(m->data);
	free(m);
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