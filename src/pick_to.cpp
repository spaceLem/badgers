// given from-group x, pick a possible to-group y for movement

#include "pick_to.hpp"

int pick_to(ivec const &N, int const &x) {

	int static const n = P.n,
				 num_groups = P.num_groups,
				 mt = P.mt;

	int static y, group;
	y = 0, group = 0;

	double static tmp;

	dvec p_to_group(num_groups, 0.0);
	//kernel = P.kernel;


	if (mt < 2) {
		// nearest neighbour

		dvec direction(4, 0.0);
		tmp = 0.0;

		for (int i = 0; i < 4; ++i) {
			tmp += P.bias[N[get_nbhd(x, i)]];
			direction[i] = tmp; 
		}

		double z = tmp * gsl_rng_uniform(gen);

		if (tmp == 0) {
			// check for incorrectly choosing movement
			cout << "movement was allowed in group " << x/n << "," << x%n << " (" << x
				<< ") when it shouldn't have been" << endl;
			y = x;

			cout << "N = " << endl;
			display(N, n);

		} else if (z == 1) {
			// shouldn't happen, but just in case
			cout << "RNG error during movement" << endl;
			y = x;
		} else {
			for (group = 0; group < 4; ++group) {
				if (z < direction[group]) {
					y = P.nbhd[4 * x + group];
					break;
				}
			}
		}
	} else {
		if (mt == 2) {
			for (int i = 0; i < num_groups; ++i) {
				p_to_group[i] = P.bias[N[i]];
			}
			p_to_group[x] = 0;	// don't allow movement back to x
		} else if (mt == 3) {
			for (int i = 0; i < num_groups; ++i) {
				if (P.kernel[x * num_groups + i]) {
					p_to_group[i] = P.bias[N[i]] * P.kernel[x * num_groups + i];
				}
			}

		}

		y = pick(p_to_group);
		if (y == x) {
			cerr << "movement was allowed at group " << x/n << "," << x%n
				<< " when it shouldn't have been" << endl;
		}
	}

	return y;
}

