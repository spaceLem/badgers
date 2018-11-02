#include "simulate.hpp"

int simulate(Records &records) {
	// switches
	int const details = P.details;			// verbosity, 0=nothing, 1=events, 2=events+details

	// copy variables that we want to use in main()
	int const n = P.n, recs = P.recs, num_groups = P.num_groups, runs = P.runs;
	double const nu = P.nu, pv = P.pv, T = P.T, t0 = P.t0, t1 = P.t1,
		   rec_width = P.rec_width, cull_pulse = P.cull_pulse, cull_gap = P.cull_gap;
		// cull_width = cull_pulse + cull_gap,
		// minstep = 0.5 * min(rec_width, cull_width);	// don't jump more than this during time step

	// *** RUN simulation as many times as requested ***
	for (int itn = 0; itn < runs; ++itn) {
		// need clean structures before each run, so declare them here

		// time markers
		bool cull_now = false, // check whether to cull
			 is_pulse = false; // use to work out time till next cull event

		double t = 0.0,                // keep track of time in the simulation
			   dt = 0.0,               // time step
			   next_record_time = 0.0, // time of next record
			   next_cull_check = t0;   // time of next cull event

		int current_record = -1, // the current record
			popn_check = 0,      // test if something has gone wrong
			last_event = 4,      // for debugging purposes
			event_info = 0;

		// vectors for holding popns and rates
		ivec S(num_groups, 0), // susceptibles
			 I(num_groups, 0), // infecteds
			 N(num_groups, 0); // total popn

		Rates rates;
		rates.birth = dvec(num_groups, 0.0);
		rates.death = dvec(num_groups, 0.0);
		rates.infect = dvec(num_groups, 0.0);
		rates.movement = dvec(num_groups, 0.0);
		rates.total_events = 0.0;
		rates.rates = dvec(4, 0.0);	// birth, death, infect, move
		rates.cumrates = dvec(4, 0.0);

		bvec update(num_groups, true); // matrix of groups that need updating (update all at start)


		// also reset perturbation parameters if they didn't get reset already
		P.Mu = dvec(num_groups, P.mu);


		// setup pattern for initial infection, e.g. near eqbm, or chessboard
		setup_popns(S, I);
		add(N, S, I);

		// locations
		int x = 0, // store where things happen in a group
			y = 0; // if a movement occurs, it goes from x -> y

		double z; // initialise random number z


		// *** MAIN LOOP starts here ***
		while (t < T) {

			// check if records need updating
			while (t >= next_record_time && ++current_record < recs) {
				next_record_time += rec_width;

				record_data(records, S, I, N, rates, itn, current_record);
				show_recording(current_record, rates);

				// progress counter
				show_progress(t, itn);
			}

			show_time(t);

			// check if it's time to cull
			if (t >= next_cull_check) {
				// is_pulse flips between pulse and gap
				is_pulse = !is_pulse;
				next_cull_check += is_pulse ? cull_pulse : cull_gap;
				cull_now = true;
			}

			if (cull_now) {
				// perform the cull for t in [t0, t1), tidy up after t1, and reset
				P.Mu = dvec(num_groups, P.mu);
				if (t >= t1) {
					next_cull_check = T;
				} else if (is_pulse && t >= t0 && t < t1) {
					cull(S, I);
					// prevent overshoot
					if (t+cull_pulse >= t1) {
						next_cull_check = t1;
					}
				}
				for (int i = 0; i < num_groups; ++i)
					update[i] = true;
			}
			cull_now = false;


			// calculate N=S+I, and check popn bounds
			// adds some overhead
			add(N, S, I);

			popn_check = check(N, S, I, last_event, t);
			if (popn_check == 1) {
				break;		// popn has died out
			} else if (popn_check == -1) {
				describe_situation(S, I, N, rates, update, true);
				return -1;	// popn out of bounds
			}


			// update rate matrices & reset update matrix
			birth(N, rates, update);
			death(N, I, rates, update);
			infect(S, I, rates, update);
			movement(N, rates, update);

			for (int i = 0; i < num_groups; ++i)
				update[i] = false;

			// calculate rates, and TER
			rates.rates[0] = sum(rates.birth);
			rates.rates[1] = sum(rates.death);
			rates.rates[2] = sum(rates.infect);
			rates.rates[3] = sum(rates.movement);
			rates.total_events = cumsum(rates.rates, rates.cumrates);

			// time between events is ~exp(1/TER)
			if (rates.total_events > 0) {
				dt = gsl_ran_exponential(gen, 1 / rates.total_events);
			} else {
				dt = rec_width;
			}

			// advance t and pick z ~ U(0, TER), to decide which event happens next
			// note capping of timestep if dt > rec_width, and corresponding adjustment to z
			// (pretend TER includes rate that nothing happens)
			t += min(dt, rec_width);
			z = rates.total_events * gsl_rng_uniform(gen) * max(1.0, dt / rec_width);


			// choose the next event and randomly pick a group for it to apply to
			if (z < rates.cumrates[0]) {
				// --- REPRODUCTION ---
				last_event = 1;
				x = pick(rates.birth);
				z = N[x] * gsl_rng_uniform(gen);
				if (z < pv * I[x]) {
					++I[x];
					event_info = 0;
				} else {
					++S[x];
					event_info = 1;
				}
				++N[x];

			} else if (z < rates.cumrates[1]) {
				// --- natural or disease induced MORTALITY ---
				last_event = 2;
				x = pick(rates.death);
				// choose whether S or I dies
				z = gsl_rng_uniform(gen) * (P.Mu[x] * N[x] + nu * I[x]);
				if (z < P.Mu[x] * S[x]) {
					--S[x];
					event_info = 0;
				} else {
					--I[x];
					event_info = 1;
				}
				--N[x];

			} else if (z < rates.cumrates[2]) {
				// --- INFECTION ---
				last_event = 3;
				x = pick(rates.infect);
				--S[x]; ++I[x];

			} else if (z < rates.cumrates[3]) {
				// --- MOVEMENT ---
				last_event = 4;
				x = pick(rates.movement);
				y = pick_to(N, x);

				// choose S or I
				z = N[x] * gsl_rng_uniform(gen);

				if (z < S[x]) {
					--S[x]; ++S[y];
					event_info = 0;
				} else {
					--I[x]; ++I[y];
					event_info = 1;
				}
				--N[x]; ++N[y];
				// update to_group that has changed, and its neighbours
				update_nbhd(update, y);

			} else {
				// time step too small and nothing happened
				last_event = 0;
			}
			describe_event(last_event, event_info, x, y);

			// update group that has changed, and its neighbours
			update_nbhd(update, x);


			// for when we really have to know what's going on
			if (details >= 2) {
				cout << "S = " << sum(S) << "\n";
				display(S, n);
				cout << "I = " << sum(I) << "\n";
				display(I, n);
				if (details >= 3) {
					cout << "N = " << sum(N) << "\n";
					display(N, n);
					if (details >= 4) {
						cout << "birth rates = " << rates.rates[0] << "\n";
						display(rates.birth, n);
						cout << "death rates = " << rates.rates[1] << "\n";
						display(rates.death, n);
						cout << "infection rates = " << rates.rates[2] << "\n";
						display(rates.infect, n);
						cout << "movement rates = " << rates.rates[3] << "\n";
						display(rates.movement, n);
					}
				}
			}

		}	// end of main loop

		while (++current_record < recs) {
			record_data(records, S, I, N, rates, itn, current_record);
			show_recording(current_record, rates);

			show_progress(t, itn);
		}

	}	// end of runs
	show_finished();

	return 0;
}

