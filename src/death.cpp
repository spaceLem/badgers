// calculate death rates

#include "death.hpp"

void death(ivec const &N, ivec const &I, Rates &rates, bvec const &update) {
	int static const num_groups = P.num_groups;

	double static const nu = P.nu;

	for (int i = 0; i < num_groups; ++i) {
		if (update[i]) {
			rates.death[i] = P.Mu[i] * N[i] + nu * I[i];
		}
	}
}

