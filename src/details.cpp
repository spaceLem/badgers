#include "details.hpp"

void show_time(double const t) {
	int static const details = P.details;

	if (details >= 1) {
		cout.precision(4);
		cout << "time: " << setw(7) << t << "\t";
	}
}


void show_progress(double const t, int const itn) {
	int static const details = P.details,
			   runs = P.runs;
	
	double static const T = P.T;

	if (details == 0) {
		cout << "progress: " << setw(3)
			<< round(100 * (T * itn + t) / (T * runs))
			<< "\%   \r";
		fflush(stdout);
	}
}


void show_finished() {
	int static const details = P.details;

	if (details == 0) {
		cout << "progress: 100\% ... done!" << endl;
	}

}


void show_recording(int const current_record, Rates const &rates) {
	int static const details = P.details,
		num_groups = P.num_groups;

	if (details >= 1) {
		cout << "\n"
			<< "recording: record #" << current_record << "\n"
			<< "birth:     " << rates.rates[0] / num_groups << "\n"
			<< "death:     " << rates.rates[1] / num_groups << "\n"
			<< "infection: " << rates.rates[2] / num_groups << "\n"
			<< "movement:  " << rates.rates[3] / num_groups << "\n"
			<< endl;
	}
}


void describe_event(int const last_event, int const event_info, int const x, int const y) {
	int static const details = P.details,
		n = P.n;

	if (details >= 1) {
		switch (last_event) {
			case 1:
				cout << "birth of S in " << x
					<< " (" << x/n << "," << x%n << ")" << endl;
				break;
			case 2:
				cout << "death of " << (event_info == 0 ? "S" : "I") << " in " << x
					<< " (" << x/n << "," << x%n << ")" << endl;
				break;
			case 3:
				cout << "infection in " << x
					<< " (" << x/n << "," <<x%n << ")" << endl;
				break;
			case 4:
				cout << (event_info == 2 ? "recovery and " : "")
					<< "movement of " << (event_info == 0 ? "S" : "I") << " in " << x
					<< " (" << x/n << "," << x%n << ")" << " to " << y
					<< " (" << y/n << "," << y%n << ")"  << endl;
				break;
			case 0:
				cout << "nothing happened" << endl;
				break;
		}

	}
}

void describe_situation(ivec const &S, ivec const &I, ivec const &N,
		Rates const &rates, bvec const &update, bool const failure) {

	int static const details = P.details,
		n = P.n;

	if (details == 0 && !failure)
		return;

	if (details == -1) {
		// scratch area for specific details
	}

	if (details >= 2 || failure) {
		// for when we really have to know what's going on
		cout << "S = " << sum(S) << "\n";
		display(S, n);
		cout << "I = " << sum(I) << "\n";
		display(I, n);
		cout << "\n";
	}

	if (details >= 3 || failure) {
		cout << "N = " << sum(N) << "\n";
		display(N, n);
	}

	if (details >= 4 || failure) {
		cout << "birth rates = " << rates.rates[0] << "\n";
		display(rates.birth, n);
		cout << "death rates = " << rates.rates[1] << "\n";
		display(rates.death, n);
		cout << "infection rates = " << rates.rates[2] << "\n";
		display(rates.infect, n);
		cout << "movement rates = " << rates.rates[3] << "\n";
		display(rates.movement, n);
		cout << "update matrix\n";
		display(update, n);
	}
}

