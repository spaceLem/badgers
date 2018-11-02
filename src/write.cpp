#include "write.hpp"

void write(const Records &records) {
	int const recs = P.recs,
		runs = P.runs,
		n = P.n,
		items = P.items,
		distn = P.distn;

	double const c = P.c;

	bool const get_dist_I = distn % 2,
		 get_dist_N = (distn / 2) % 2,
		 get_all_Is = (distn / 4) % 2;

	if (records.data.size() != P.items * P.recs * P.runs)
		cout << "ERROR: records not the right size" << endl;


	// makes storing parameters *much* easier
	string str = "\n# type cell\n# rows: 1\n# columns: 1\n# name: <cell-element>\n# type: ",
		   matrix_str = str + " matrix\n",
		   scalar_str = str + " scalar\n",
		   struct_str = str + " struct\n",
		   name = "# name: ";

	// write records to file, formatted for GNU Octave
	ofstream myfile(P.file_name.c_str());

	// store data as a struct
	myfile << name << P.file_name << "\n"
		<< "# type: struct\n"
		<< "# length: " << 2 + get_dist_I + get_dist_N + get_all_Is
		<< "\n\n";


	// store parameters
	myfile << name << "parameters"
		<< struct_str
		<< "# length: 24\n"
		<< "\n"
		<< name << "runs"       << scalar_str << P.runs << "\n"
		<< name << "recs"       << scalar_str << P.recs << "\n"
		<< name << "T"          << scalar_str << P.T << "\n"
		<< name << "n"          << scalar_str << P.n << "\n"
		<< name << "icb"        << scalar_str << P.icb << "\n"
		<< name << "r"          << scalar_str << P.r << "\n"
		<< name << "c"          << scalar_str << P.c << "\n"
		<< name << "mu"         << scalar_str << P.mu << "\n"
		<< name << "nu"         << scalar_str << P.nu << "\n"
		<< name << "kappa"      << scalar_str << P.kappa << "\n"
		<< name << "alpha"      << scalar_str << P.alpha << "\n"
		<< name << "be"         << scalar_str << P.be << "\n"
		<< name << "bw"         << scalar_str << P.bw << "\n"
		<< name << "bb"         << scalar_str << P.bb << "\n"
		<< name << "pv"         << scalar_str << P.pv << "\n"
		<< name << "t0"         << scalar_str << P.t0 << "\n"
		<< name << "t1"         << scalar_str << P.t1 << "\n"
		<< name << "cull_years" << scalar_str << P.cull_years << "\n"
		<< name << "cullb"      << scalar_str << P.cullb << "\n"
		<< name << "cullw"      << scalar_str << P.cullw << "\n"
		<< name << "cull_pulse" << scalar_str << P.cull_pulse << "\n"
		<< name << "cull_gap"   << scalar_str << P.cull_gap << "\n"
		<< name << "p1"         << scalar_str << P.p1 << "\n"
		<< name << "p2"         << scalar_str << P.p2 << "\n"
		<< "\n";



	// store annual rates
	myfile << name << "data"
		<< matrix_str
		<< "# ndims: 3\n"
		<< items << " " << recs << " " << runs << "\n";

	for (auto rd : records.data) {
		// FIXME this patches the 1e-300 problem that sometimes crops up
		// and conflicts with a bug in Octave that prevents it from loading the file
		// NB: assumes no negative values!
		if (rd > 1e-50) {
			myfile << rd << "\n";
		} else {
			myfile << "0\n";
		}
	}
	myfile << "\n";

	int max_group_size = iceil(2 * c);

	// store I's distn
	if (get_dist_I) {
		myfile << name << "Ivar"
			<< matrix_str
			<< "# rows: 3\n"
			<< "# columns: " << max_group_size << "\n";

		myfile.precision(3);
		int val = 0;
		for (auto x : records.I_var) {
			myfile << x << (++val % max_group_size ? " " : "\n");
		}
		myfile << "\n";
	}

	// store N's distn
	if (get_dist_N) {
		myfile << name << "Nvar\n"
			<< matrix_str
			<< "# rows: 3\n"
			<< "# columns: " << max_group_size << "\n";

		myfile.precision(3);
		int val = 0;
		for (auto x : records.N_var) {
			myfile << x << (++val % max_group_size ? " " : "\n");
		}
		myfile << "\n";
	}

	// write samples to file_name\s
	if (get_all_Is) {
		myfile << name << "sample\n"
			<< matrix_str
			<< "# ndims: 4\n"
			<< n << " " << n << " 3 " << runs << "\n";

		int val = 0;
		for (auto x : records.sample) {
			myfile << x << (++val % n ? " " : "\n");
		}
		myfile << "\n";
	}

	myfile << endl;
	myfile.close();
}

