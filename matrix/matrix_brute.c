/* Matrix multiplication, brute force (O(n^3)) */

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
struct _matrix* _mul_square_brute(struct _matrix*, struct _matrix*);

int main(int argc, char const *argv[])
{
	// init random generator
	srand(time(NULL));
	
	if (argc < 2) {
		printf("usage: matrix_brute 1000\n");
		return 1;
	}

	int n = atoi(argv[1]);

	struct _matrix *m1 = _gen_rand_matrix(n);
	struct _matrix *m2 = _gen_rand_matrix(n);
	struct _matrix *mul = _mul_square_brute(m1, m2);
	// int i, j;
	// for (i = 0; i < n; i++) {
	// 	for (j = 0; j < n; j++) {
	// 		printf("%10i ", mul->data[i][j]);
	// 	}
	// 	printf("\n");
	// }
	return 0;
}

struct _matrix* _mul_square_brute(struct _matrix *m1, struct _matrix *m2) {
	if (m1->rows != m1->cols || m2->rows != m2->cols || m1->rows != m2->rows) {
		return NULL;
	}
	int n = m1->rows;

	struct _matrix *res = (struct _matrix*)malloc(sizeof(struct _matrix));
	res->rows = m1->rows;
	res->cols = m2->cols;
	res->data = (int**)malloc(n * sizeof(int*));
	int i;
	for (i = 0; i < n; i++) {
		res->data[i] = (int*)malloc(n * sizeof(int));
	}

	int j, k;
	for (i = 0; i < n; i++) {
		for (j = 0; j < n; j++) {
			res->data[i][j] = 0;
			for (k = 0; k < n; k++) {
				res->data[i][j] += m1->data[i][k] * m2->data[k][j];
			}
		}
	}

	return res;
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