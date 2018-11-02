#include "setup_params.hpp"

void setup_params(int argc, char *argv[]) {

	/*
		order of arguments is:
		1	file_name	name to output data
		2	runs		number of times to perform simulation
		3	icw			initial conditions within groups -- I(0)
							0) near DFE, S=K, I=1
							1) near EE, S=K/2, I=K/4
		4	icb			initial conditions within groups -- G(0)
		5	inftype		type of infection to use
							0) density dependent
							1) frequency dependent
		6	mt			type of dispersal to use
							0) nearest neighbour (torus)
							1) nearest neighbour (edges)
							2) global
							3) negative exponential
		7	mb			type of density dependent dispersal to use
							0) threshold (1/0)
							1) linear (1-N/c)
							2) very linear (1-N/K)
							3) diffusion (1)
		8	T			how long to run simulation for (usually 100)
		9	recs		number of records to take (usually = T)

		model parameters
		10	n			size of lattice (n^2)
		11	c			glass ceiling carrying capacity
		12	r			intrinsic reproduction rate
		13	mu			natural mortality rate
		14	nu			disease induced mortality rate for I
		15	kappa		dispersal rate
		16	a			threshold point for dispersal, alpha
		17	phi			movement distance penalty
		18	bw			within groups infection rate
		19	bb			between groups infection rate
		20	be			background infection rate
		21	pv			vertical transmission probability

		culling paramters
		22	cullb		type of culling to use between groups
							0) none, 1) random group, 2) clump, 3) inner core
		23	cullw		type of culling to use within groups
							1) continuous, 2) annual
		24	cull_pulse	cull pulse (time to cull)
		25	cull_gap	cull gap (interval when not culling)
		26	t0			year that culling starts
		27	cull_years	number of years to cull for
		28	p1			proportion of groups to cull
		29	p2			cw=1) increased mortality rate during cull
							cw=2) proportion remaining following cull
		30	pb			allow behaviour to change during cull

		switches
		31	details		how verbose the simulation is
							0) none
							1) show events
							2) show groups + events
		32	distn		distribution records (0-7)
							0 none
							+1 distribution of I
							+2 distribution of N
							+4 take sample
	*/

	// copy arguments into a struct (this will be a global variable)
	int arg = 0;
	P.file_name = argv[++arg];
	P.runs = atoi(argv[++arg]);
	P.icw = atoi(argv[++arg]);

	P.icb = atof(argv[++arg]);
	// fix ICs to be in [0,1]
	P.icb = clamp(P.icb, 0.0, 1.0);
	
	P.inftype = atoi(argv[++arg]);
	P.mt = atoi(argv[++arg]);
	P.mb = atoi(argv[++arg]);
	P.T = atof(argv[++arg]);
	P.recs = atoi(argv[++arg]);

	P.n = atoi(argv[++arg]);
	P.c = atof(argv[++arg]);
	P.r = atof(argv[++arg]);
	P.mu = atof(argv[++arg]);
	P.nu = atof(argv[++arg]);
	// m has both normal and temporary values
	P.kappa = atof(argv[++arg]);
	P.alpha = atof(argv[++arg]);
	P.phi = atof(argv[++arg]);
	// beta has both normal and temporary values
	P.bw = atof(argv[++arg]);
	P.bb = atof(argv[++arg]);
	P.be = atof(argv[++arg]);
	P.pv = atof(argv[++arg]);

	// cull specific parameters
	P.cullb = atoi(argv[++arg]);
	P.cullw = atoi(argv[++arg]);
	P.cull_pulse = atof(argv[++arg]);
	P.cull_gap = atof(argv[++arg]);
	P.t0 = atof(argv[++arg]);
	P.cull_years = atof(argv[++arg]);
	P.p1 = atof(argv[++arg]);
	P.p2 = atof(argv[++arg]);
	// switches
	P.details = atoi(argv[++arg]);
	P.distn = atoi(argv[++arg]);

	// other constants
	
	// disease free eqbm
	if (P.r == 0) {
		// if r=0, then k=c
		P.K = P.c;
	} else if (P.r == P.mu) {
		// if r=mu, then k=c/2
		P.K = P.c / 2;
	} else {
		// else usual eqbm
		P.K = P.c * (1.0 - P.mu / P.r);
	}
	P.num_groups = P.n * P.n; // total number of popns in lattice
	P.events = 5; // different possible events
	              // birth, death, infection, dispersal, nothing

	// number of items stored in records, and time between records
	P.items = (P.cullb == 3) ? 27 : 11; // S, I, N, G, birth, death, inf, H, H_eff, mig, vt,
	                                    // I, N, G, H, mig for 4 subsets
	P.rec_width = P.T / P.recs;	// width of a record

	// set core zones (inner 1/3 and middle band)
	P.zone = ivec(P.num_groups, 0);
	P.distance = dvec(P.num_groups, 0.0);
	P.centre = ifloor(P.n / 2.0);
	P.radius = ifloor(P.n / 4.0);
	P.subset = ivec(4, 0);

	for (int i = 0; i < P.num_groups; ++i) {
		P.distance[i] = max(fabs(P.centre - i % P.n), fabs(P.centre - i / P.n));
		if (P.distance[i] < P.radius) {
			P.zone[i] = 3;
		} else if (P.distance[i] < P.radius + 1) {
			P.zone[i] = 2;
		} else if (P.distance[i] < P.radius + 2) {
			P.zone[i] = 1;
		}
		++P.subset[P.zone[i]];
	}

	// show zone numbers
	//cout << "core areas are:\n";
	//display(P.zone, P.n);


	// sanity check
	if (P.cull_pulse + P.cull_gap == 0) {
		P.cull_pulse = 1;
	}

	// extend culling if there's a gap
	P.t1 = P.t0 + P.cull_years;
	P.t1 += ceil(P.cull_years / P.cull_pulse - 1) * P.cull_gap;

	string mtstr, mbstr, cbstr, cwstr, p2str;
	switch (P.mt) {
		case 0: mtstr = "nnt"; break;
		case 1: mtstr = "nne"; break;
		case 2: mtstr = "glb"; break;
		case 3: mtstr = "nxp"; break;
	}
	switch (P.mb) {
		case 0: mbstr = "step"; break;
		case 1: mbstr = "bias"; break;
		case 2: mbstr = "vbsd"; break;
		case 3: mbstr = "diff"; break;
	}
	switch (P.cullb) {
		case 0: cbstr = "none"; break;
		case 1: cbstr = "rand"; break;
		case 2: cbstr = "blok"; break;
		case 3: cbstr = "core"; break;
	}
	switch (P.cullw) {
		case 0: cwstr = "none"; p2str = "0"; break;
		case 1: cwstr = "cnts"; p2str = "r"; break;
		case 2: cwstr = "disc"; p2str = "p"; break;
	}


	// display parameters for verification
	cout << "file: " << P.file_name << "\n"
		<< "runs=" << P.runs << ", "
		<< "recs=" << P.recs << ", "
		<< "ics=" << (P.icw == 0 ? "df" : "ee") << "/" << P.icb << ", "
		<< "inf=" << (P.inftype ? "fd" : "dd") << ", "
		<< "disp=" << mtstr << "/" << mbstr << ", "
		<< "T=" << P.T << ", "
		<< "n=" << P.n << ", "
		<< "c=" << P.c << ", "
		<< "r=" << P.r << ", "
		<< "mu=" << P.mu << ", "
		<< "nu=" << P.nu << ", "
		<< "[K=" << P.K << "]" << ", "
		<< "kappa=" << P.kappa << ", "
		<< "alpha=" << P.alpha << ", "
		//<< "phi=" << P.phi << ", "
		<< "beta=" << P.bw << "/" << P.be << "/" << P.bb << ", "
		<< "pv=" << P.pv << ", "
		<< "cull=" << cbstr << "/" << cwstr << ", "
		<< "pulse=+" << P.cull_pulse << "-" << P.cull_gap << ", "
		<< "p=" << P.p1 << "x" << P.p2 << p2str << ", "
		<< "t=" << P.t0 << "-" << P.t1
		<< endl;

	// assign mu and nu to groups
	P.Mu = dvec(P.num_groups, P.mu);

	// pre-calculate nearest neighbours
	calc_nbhd();

	// normalise dispersal rate
	double normal = 4.0;
	if (P.mt == 2) {
		// global dispersal
		normal = (P.num_groups > 1) ? P.num_groups - 1 : 1;
	} else if (P.mt == 3) {
		// negative exponential
		normal = distance_kernel();
	}

	P.kappa /= normal;

	// set up a lookup table for dispersal bias f(N_k)
	calculate_bias();
}

