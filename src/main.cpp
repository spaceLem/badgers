// *************************************************************************
// spatial stochastic SI Model, with dispersal and culling
//
// Author: Jamie Prentice
// Date: Oct 2008
// *************************************************************************

#include "defs.hpp"
#include "records.hpp"
#include "simulate.hpp"
#include "write.hpp"

gsl_rng *gen = gsl_rng_alloc(gsl_rng_taus);
Parameters P;

int main (int argc, char **argv) {
	// parameter check
	if (argc != 33) {
		cerr << "Provided " << argc-1 << " parameters, expected 32." << endl;
		return -1;
	}

	gsl_rng_set(gen, (unsigned) time(0));	// seed is UNIX time in seconds
	//gsl_rng_set(gen, (unsigned) 0);		// seed is 0

	setup_params(argc, argv);
	Records records;

	int outcome = simulate(records);

	write(records);

	return outcome;
}

