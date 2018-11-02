// a collection of definitions and headers

#pragma once

#include <fstream>
#include <iostream>
#include <vector>
#include <cmath>
#include <gsl/gsl_rng.h>        // for good RNG generation
#include <gsl/gsl_randist.h>    // needed for providing pdfs

using namespace std;

// I really hate having to type all these out all the time
typedef vector<int> ivec;
typedef vector<double> dvec;
typedef vector<bool> bvec;

//enum direction_t {UP, DOWN, LEFT, RIGHT};

struct Parameters {
	string file_name;
	int details, distn, runs, icw, recs, items, events, n, c,
		cullb, cullw, num_groups, mb, mt, pb, inftype, centre, radius;
	double icb, T, r, mu, nu, kappa, alpha, phi, be, bw, bb, pv, p1, p2,
		   K, k0, k1, k2, km, t0, t1,
		   cull_years, cull_pulse, cull_gap, rec_width;
	dvec Mu, kernel, bias, distance;
	ivec nbhd, zone, subset;
};

struct Rates {
	double total_events;
	dvec rates, cumrates;
	dvec birth, death, infect, movement;
};

extern gsl_rng *gen;
extern Parameters P;

