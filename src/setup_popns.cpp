#include "setup_popns.hpp"

void setup_popns(ivec &S, ivec &I) {
	// set some initial values to assign to popns
	int num_groups = P.num_groups,
		K = round(P.K),
		S1 = K, S2 = K,
		I1 = 0, I2 = 0;

	double mu = P.mu, nu = P.nu, bw = P.bw;

	// For EE, I* is a reasonable approximation, and correct when nu=0;
	int EES = min(max(static_cast<int>(round((mu + nu) / bw)), 1), K),
		EEI = max(K - EES, 1);

	if (P.icw == 0) {
		// near disease free eqbm
		S1 = K; I1 = 0;
		S2 = K - 1; I2 = 1;
	} else {
		// near endemic eqbm
		S1 = K; I1 = 0;
		S2 = EES; I2 = EEI;
	}


	int x;
	dvec chooseI(num_groups, 1);

	S = ivec(num_groups, S1);
	I = ivec(num_groups, I1);
	for (int i = 0; i < ceil(P.icb * num_groups); ++i) {
		x = pick(chooseI);
		chooseI[x] = 0;
		S[x] = S2;
		I[x] = I2;
	}
}

