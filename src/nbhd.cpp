#include "nbhd.hpp"

void calc_nbhd(void) {
	int static const num_groups = P.num_groups,
		n = P.n,
		mt = P.mt;

	bool static top, bottom, left, right;

	P.nbhd = ivec(4 * num_groups, 0);

	int pos = -1;
	for (int x = 0; x < num_groups; ++x) {
		top		= (x/n == 0);
		bottom	= (x/n == n-1);
		left	= (x%n == 0);
		right	= (x%n == n-1);

		if (mt == 0) {
			// nearest neighbour (torus)
			P.nbhd[++pos] = x - n + (top    ? num_groups : 0);
			P.nbhd[++pos] = x + n - (bottom ? num_groups : 0);
			P.nbhd[++pos] = x - 1 + (left   ? n          : 0);
			P.nbhd[++pos] = x + 1 - (right  ? n          : 0);

		} else if (mt == 1) {
			// nearest neighbour (edges)
			P.nbhd[++pos] = top    ? x : x - n;
			P.nbhd[++pos] = bottom ? x : x + n;
			P.nbhd[++pos] = left   ? x : x - 1;
			P.nbhd[++pos] = right  ? x : x + 1;
		}
	}
}


void update_nbhd(bvec &update, int const x) {

	int static const num_groups = P.num_groups,
		mt = P.mt;

	switch (mt) {
		case 2:
			// global movement, update everything
			for (int i = 0; i < num_groups; ++i)
				update[i] = true;
			break;

		case 3:
			// negative exponential
			for (int i = 0; i < num_groups; ++i)
				if (P.kernel[x * num_groups + i])
					update[i] = true;
			break;

		default:
			// nearest neighbour
			for (int i = 0; i < 4; ++i)
				update[P.nbhd[4 * x + i]] = true;
	}
	update[x] = true;
}

