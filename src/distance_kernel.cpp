// this only needs to be run once, at the start of the simulation
//
// normalising calculates the quarter seen by the negative exponential,
// since the nearest neighbour sees only 4 neighbours (and has value 1 for each),
// we can easily normalise by dividing m by the sum for the negx in one quadrant
//
// fn therefore returns the normalising constant

#include "distance_kernel.hpp"

double distance_kernel(void) {
	int const n = P.n,
		num_groups = P.num_groups;
	int limit = n;

	double const phi = P.phi;
	double normal = 0.0,
		   k, dist;

	P.kernel = dvec(num_groups * num_groups, 0.0);

	// save some calculations by calculating how far a jump is possible
	// 1000 is an arbitrary cap, as beyond this dispersal is so low, it is not worth considering
	// since exp(-phi*limit) < 0.001 => limit > ln(1000)/phi
	if (phi > 0)
		limit = iceil(log(1000) / phi);

	// calculate one quadrant of the kernel for one group (NB: j starts at 1!)
	// groups close to the edge get less movement, but that doesn't matter
	// fairly similar to NN at edges
	for (int i = 0; i < min(limit, n); ++i) {
		for (int j = 1; j < min(limit, n); ++j) {
			dist = hypot(i, j);
			if (dist < min(limit, n)) {
				normal += exp(-dist * phi);
			}
		}
	}
	normal *= 4;

	// calculate distance kernel
	for (int i = 0; i < num_groups; ++i) {
		for (int j = 0; j < num_groups; ++j) {
			dist = hypot(i/n - j/n, i%n - j%n);
			if (dist < limit) {
				k = exp(-dist * phi);
				P.kernel[num_groups * i + j] = k;
			}
		}
		// reset kernel to 0 for i == j
		P.kernel[num_groups * i + i] = 0.0;
	}

	return normal;
}

