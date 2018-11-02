// calculate birth rates

#include "birth.hpp"

void birth(ivec const &N, Rates &rates, bvec const &update) {
	int static const num_groups = P.num_groups;

	double static const c = P.c,
		   r = P.r;

	for (int i = 0; i < num_groups; ++i) {
		if (update[i]) {
			rates.birth[i] = r * N[i] * max(1.0 - N[i] / c, 0.0);
		}
	}
}

