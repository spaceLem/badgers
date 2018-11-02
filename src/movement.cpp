/*
 * calculate dispersal matrix, containing the rate individuals leave each group
 * dispersal is weighted towards emptier groups
 */

#include "movement.hpp"

void movement(ivec const &N, Rates &rates, bvec const &update) {

	int static const num_groups = P.num_groups,
			   mt = P.mt;

	double static const kappa = P.kappa;

	// calculate weightings for valid neighbours
	switch (mt) {
		case 0:
		case 1:
			// nearest neighbour
			for (int i = 0; i < num_groups; ++i) {
				if (update[i]) {
					double tmp = 0.0;
					for (int j = 0; j < 4; ++j) {
						tmp += P.bias[N[get_nbhd(i, j)]];
					}
					rates.movement[i] = kappa * N[i] * tmp;
				}
			}
			break;

		case 2:
			// global
			for (int i = 0; i < num_groups; ++i) {
				rates.movement[i] = 0.0;
				for (int j = 0; j < num_groups; ++j) {
					rates.movement[i] += P.bias[N[j]];
				}
				rates.movement[i] -= P.bias[N[i]];
				rates.movement[i] *= kappa * N[i];
			}
			break;

		case 3:
			// negative exponential
			for (int i = 0; i < num_groups; ++i) {
				if (update[i]) {
					rates.movement[i] = 0.0;
					for (int j = 0; j < num_groups; ++j) {
						if (P.kernel[i * num_groups + j]) {
							rates.movement[i] += P.bias[N[j]] * P.kernel[i * num_groups + j];
						}
					}
					rates.movement[i] *= kappa * N[i];
				}
			}
	}
}

