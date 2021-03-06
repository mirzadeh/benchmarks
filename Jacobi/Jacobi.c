#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "jacobi_kernel.h"

#define nx 200
#define ny 200
#define tol 1e-6
#define index(i,j) (i)*ny + j
#define max(a, b) a > b ? a : b

void boundary(double *u) {
	int i;

	for (i = 0; i < nx; i++) u[index(i, ny - 1)] = 1;
}

double absdiff(double *u, double *unew) {
	int i, j;

	double d = fabs(u[0] - unew[0]);
	for (i = 0; i < nx; i++) {
		for (j = 0; j < ny; j++) {
			d = max(d, fabs(u[index(i, j)] - unew[index(i, j)]));
		}
	}

	return d;
}

void solve(double *u, double *unew) {
	double err = 1;
	int it = 1;
	double *utmp;

	while (err > tol) {
		relax(u, unew, nx, ny);
		err = absdiff(u, unew);

		utmp = u; u = unew; unew = utmp;
		it++;
	}

	printf("took %d Iterations with err = %1.5e \n", it, err);
}

int main() {
	int i, j;
	double *u    = calloc(nx*ny, sizeof(double));
	double *unew = calloc(nx*ny, sizeof(double));

	boundary(u);
	boundary(unew);

	clock_t t = clock();
	solve(u, unew);
	printf("Took %f seconds\n", (double)(clock() - t) / CLOCKS_PER_SEC);

	free(u);
	free(unew);
}
