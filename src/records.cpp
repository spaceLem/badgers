#include "records.hpp"

void record_data(Records &records, ivec const &S, ivec const &I, ivec const &N, Rates const &rates, int const itn, int const current_record) {

	int static const distn = P.distn,
		num_groups = P.num_groups;

	double static const pv = P.pv,
		   T = P.T,
		   c = P.c,
		   rec_width = P.rec_width,
		   t0 = P.t0,
		   t1 = P.t1;

	double sum_S = sum(S),
		   sum_I = sum(I),
		   sum_N = sum_S + sum_I,
		   vert_r = 0.0,
		   proportion = 0.0; 

	// sum the number of groups containing infectives
	for (auto i : I)
		proportion += (i > 0);

	// quickly recalculate the vertical transmission rate
	// since it wasn't isolated earlier
	for (int i = 0; i < num_groups; ++i)
		vert_r += I[i] * (1.0 - N[i] / c);
	vert_r *= P.r * pv;

	// record global
	// records.data.push_back(...)
	records.data.insert(records.data.end(), {
			sum_S / num_groups,				//  0 mean S
			sum_I / num_groups,				//  1 mean I
			sum_N / num_groups,				//  2 mean N
			proportion / num_groups,		//  3 G
			rates.rates[0] / num_groups,	//  4 birth
			rates.rates[1] / num_groups,	//  5 death
			rates.rates[2] / num_groups,	//  6 infect
			rates.rates[3] / num_groups,	//  7 move
			div0(rates.rates[2] * num_groups, sum_S * sum_I), //  8 beta_eff
			vert_r / num_groups,			//  9 vt
			div0(sum_I, sum_N)				// 10 prevalence
			//div0(infect_r, sum_N);		// incidence
			});


	// record inside and outside segments in the ring culling
	// set 0-3 = outer, inner2, inner1, core
	if (P.cullb == 3) {
		dvec zone_sum_I(4, 0.0),
			 zone_sum_N(4, 0.0),
			 zone_prop(4, 0.0),
			 zone_infect_r(4, 0.0),
			 zone_movement_r(4, 0.0);

		for (int i = 0; i < num_groups; ++i) {
			int zone = P.zone[i];

			zone_sum_I[zone] += I[i];
			zone_sum_N[zone] += N[i];
			zone_prop[zone] += (I[i] > 0);
			zone_infect_r[zone] += rates.infect[i];
			zone_movement_r[zone] += rates.movement[i];
		}


		// record each zone in turn
		// 0 = periphery, 1 = outer border, 2 = inner border, 3 = core
		// if any subset is size 0, prevent divide by 0 errors
		for (int zone = 0; zone < 4; ++zone) {
			records.data.insert(records.data.end(), {
					div0(zone_sum_I[zone], P.subset[zone]),
					div0(zone_prop[zone], P.subset[zone]),
					div0(zone_infect_r[zone], P.subset[zone]),
					div0(zone_movement_r[zone], P.subset[zone])
					});
		}
	}

	// get distribution of I
	// 1) just before start of cull, 2) during the cull, 3) 5 years after the cull
	// and correct for record width
	if (distn) {
		bool static const get_dist_I = distn % 2,
			 get_dist_N = (distn / 2) % 2,
			 get_all_Is = (distn / 4) % 2;

		int static const max_group_size = 2 * iceil(c);

		int static const y1 = ifloor((t0 - 1) / rec_width),
			y2 = ifloor((min((t0 + t1) / 2, T - 5)) / rec_width),
			y3 = ifloor((min(t1 + 5, T - 1)) / rec_width);

		if (current_record == y1) {
			if (get_dist_I)
				for (auto x : I)
					++records.I_var[x];
			if (get_dist_N)
				for (auto x : N)
					++records.N_var[x];
			if (get_all_Is)
				for (auto x : I)
					records.sample.push_back(x);

		} else if (current_record == y2) {
			if (get_dist_I)
				for (auto x : I)
					++records.I_var[max_group_size + x];
			if (get_dist_N)
				for (auto x : N)
					++records.N_var[max_group_size + x];
			if (get_all_Is)
				for (auto x : I)
					records.sample.push_back(x);

		} else if (current_record == y3) {
			if (get_dist_I)
				for (auto x : I)
					++records.I_var[2 * max_group_size + x];
			if (get_dist_N)
				for (auto x : N)
					++records.N_var[2 * max_group_size + x];
			if (get_all_Is)
				for (auto x : I)
					records.sample.push_back(x);
		}
	}
}

