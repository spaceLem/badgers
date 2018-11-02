// implement various methods to cull the population
// this is the only time we are allowed to change P, so it can't be const

#include "cull.hpp"

void cull(ivec &S, ivec &I) {
	int static const n = P.n,
		num_groups = P.num_groups,
		cullb = P.cullb,
		cullw = P.cullw;

	double static const mu = P.mu,
		   p1 = P.p1,
		   p2 = P.p2;

	// used for storing groups
	int x = 0;

	// choose groups to cull
	dvec cull_groups(num_groups, 1.0);
	bvec cull_mask(num_groups, false);

	int const cull_num = fuzzyint(p1 * num_groups),
		border = iceil(sqrt(cull_num));

	//dvec copy_I(num_groups, 0.0), // weight towards greater I
	//	 inv_I(num_groups, 0.0);    // weight towards least I

	// reset death rates if necessary
	if (cullw == 1) {
		for (auto &x : P.Mu)
			x = mu;
	}

	// cull in pulses, with a gap
	//if ( (year-t0) % (cull_pulse+cull_gap) >= cull_pulse)
	//	return;

	// choose which groups to cull
	switch (cullb) {
		case 1:
			// randomly cull p1 groups
			if (P.details == 1)
				cout << "cull now, ";

			if (P.details >= 2)
				cout << "cull in ";

			for (int i = 0; i < cull_num; ++i) {
				x = pick(cull_groups);
				if (x == -1)
					break;
				
				// don't choose x again
				cull_groups[x] = 0.0;
				cull_mask[x] = true;

				if (P.details >= 2)
					cout << x / n << "," << x % n << " ";
			}
			break;
		case 2:
			// block cull p1 groups
			if (P.details == 1)
				cout << "cull now, ";

			if (P.details >= 2)
				cout << "cull in ";

			x = -1;
			for (int i = 0; i < num_groups; ++i)
				cull_mask[i] = (i % n < border) && (++x < cull_num);
			break;
		case 3:
			// cull inside core
			if (P.details >= 1)
				cout << "cull in core, ";

			// cull only in the core (zone 3)
			// don't cull in boundary (zone 1 and 2) or periphery (zone 0)
			for (int i = 0; i < num_groups; ++i) {
				if (P.zone[i] != 3) {
					cull_groups[i] = 0.0;
				}
			}

			// cull in this smaller area (adjust for size)
			for (int i = 0; i < p1 * P.subset[3]; ++i) {
				x = pick(cull_groups);
				if (x == -1)
					break;
				
				cull_groups[x] = 0.0;
				cull_mask[x] = true;
			}
			break;
		default:
			if (P.details >= 1)
				cout << "no cull happening, ";
			break;
	}

	// choose method of culling
	switch(cullw) {
		case 1:
			// increase death rate
			for (int i = 0; i < num_groups; ++i) {
				if (cull_mask[i])
					P.Mu[i] += P.p2;
			}
			break;
		case 2:
			// instant cull
			for (int i = 0; i < num_groups; ++i) {
				if (cull_mask[i]) {
					S[i] = gsl_ran_binomial(gen, 1 - p2, S[i]);
					I[i] = gsl_ran_binomial(gen, 1 - p2, I[i]);
				}
			}
			break;
		default:
			break;
	}
}

