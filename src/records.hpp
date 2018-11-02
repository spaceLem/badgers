// store useful information in records

#pragma once

#include "defs.hpp"
#include "utils.hpp"

struct Records {
	dvec data;
	ivec I_var, N_var, sample;

	Records() {
		// don't need this line if starting with size 0
		//data = dvec(P.items * P.recs * P.runs, 0.);

		// store distn of I, N, and a sample at 3 different times
		// assume N never reaches more than 2*c
		I_var = ivec(3 * 2 * iceil(P.c), 0);
		N_var = ivec(3 * 2 * iceil(P.c), 0);
		//sample = ivec(3 * P.runs * P.num_groups, 0);
	}
};


void record_data(Records &records,
		ivec const &S, ivec const &I, ivec const &N, Rates const &rates,
		int const itn, int const current_record);

