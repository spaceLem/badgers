// calculate infection matrix, containing the infection rate for each group

#include "infect.hpp"

void infect(ivec const &S, ivec const &I, Rates &rates, bvec const &update) {
	int static const num_groups = P.num_groups,
			   inftype = P.inftype;

	// can't make these static, as they change during cull
	double const c = P.c,
		   be = P.be,
		   bw = P.bw,
		   bb = P.bb;

	for (int i = 0; i < num_groups; ++i) {
		if (update[i]) {
			// zero, then calculate background infection rate
			rates.infect[i] = be * S[i];

			// calculate within group infection rate
			if (inftype == 0) {
				// density dependent
				rates.infect[i] += bw * S[i] * I[i];
			} else {
				// frequency dependent
				// scale by c, and avoid dividing by 0
				rates.infect[i] += div0(c * bw * S[i] * I[i], S[i] + I[i]);
			}

			// calculate between group infection rate, if neighbour exists
			if (bb) {
				double tmp = 0.0;
				for (int j = 0; j < 4; ++j) {
					tmp += P.nbhd[4 * i + j];
				}
				rates.infect[i] += bb * S[i] * tmp;
			}
		}
	}
}

