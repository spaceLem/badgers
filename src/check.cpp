#include "check.hpp"

int check(ivec const &N, ivec const &S, ivec const &I, int const last_event, double const t) {
	int popn_check = 0;

	if (sum(N) <= 0) {
		// if so, end this run
		// cout << "popn died out at time " << t << endl;
		popn_check = 1;
	}

	static const vector<string> events{
		"birth", "death", "infection", "movement", "non-event"};

	// check that for all groups 0 <= S,I,N <= c
	for (int i = 0; i < P.num_groups; ++i) {
		if (S[i] < 0 || I[i] < 0 || N[i] > 2 * P.c) {
			cerr << "popn bounds error following "
				<< events[last_event] 
				<< "at time " << t << endl;

			popn_check = -1;
			break;
		}
	}
	return popn_check; 
}

