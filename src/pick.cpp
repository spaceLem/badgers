/* pick a random group, based on some value for each group
 * this may be popn size, or some rate associated with the popn
 *
 * get the cumulative sum for A and use it as a CDF to choose a group
 * if all values are 0, then return x = -1;
 */

#include "pick.hpp"

int pick(dvec const &A) {
	int const size = A.size();

	int x = -1;

	double last = 0.0;

	dvec B(size, 0.0);

	// B is the cumulative sum of A
	for (int i = 0; i < size; ++i) {
		last = B[i] = A[i] + last;
	}

	// exit with x = -1 if there is nothing to choose
	if (last == 0)
		return -1;

	// pick z ~ U(0, total(A))
	double z = last * gsl_rng_uniform(gen);

	/* linear search
	for (int i = 0; i < size; ++i) {
		if (z < B[i]) {
			x = i;
			break;
		}
	}
	*/ 

	// binary search
 	int a = 0,
		b = size;
	x = (a + b) / 2;

	while (a < b) {
		if (z < B[x])
			b = x;
		else
			a = x + 1;
		x = (a + b) / 2;
	}

	return x;
}

