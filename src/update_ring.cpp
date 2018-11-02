// perform a ring cull

#include "update_ring.hpp"

void update_ring(ivec const &I) {
	int static const n = P.n,
			   num_groups = P.num_groups;
	int static radius = 0;
	int infecteds = 1;

	while (infecteds > 0 && radius < n / 2) {
		infecteds = 0;
		for (int i = 0; i < num_groups; ++i) {
			if (P.distance[i] < radius)
				P.zone[i] = 2;
			else if (P.distance[i] < radius + 1) {
				P.zone[i] = 1;
				infecteds += (I[i] > 0);
			}
		}
		if (infecteds > 0)
			++radius;
	}

	if (P.details >= 2)
		cout << "radius=" << radius << ", ";

	// update subsets
	for (int i = 0; i < num_groups; ++i)
		++P.subset[P.zone[i]];
}

