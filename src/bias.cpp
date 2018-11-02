// calculate a look-up table for the density dependence function f(N_k)
// values should always be in [0, 1]

#include "bias.hpp"

void calculate_bias(void) {
	double const c = P.c,
		   alpha = P.alpha,
		   K = P.K;

	int const max_group_size = 2 * iceil(c);

	P.bias = dvec(max_group_size, 0.0);

	for (int i = 0; i < max_group_size; ++i) {
		double x = 1.0;

		if (P.mb == 1) {
			// linear, for i >= c, this should always be 0
			x = 1.0 - i / c;

		} else if (P.mb == 2) {
			// linear, up to alpha
			x = 1.0 - i / (alpha * K);

		} else if (P.mb == 3) {
			// diffusion
			x = 1.0;

		} else {
			// smoothed threshold, something like
			// 1  1 ... 1  0.6  0  0 ... 0
			x = alpha * K - i;
		}

		P.bias[i] = clamp(x, 0.0, 1.0);
	}
}

