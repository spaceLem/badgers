function fig6(gen=0)
	if !exist("~/badger_data/fig6-data.mat", "file") || gen == 12345
		tic; gen_data(); toc
	endif

	plot_data()
endfunction


function gen_data()
	P = std_pars();

	% parameters of interest
	par = {"p1", "p"};
	vals{1} = [0.1 0.2 0.5 1.0];
	vals{2} = [0.2 0.6];

	nvals = [length(vals{1}), length(vals{2})];

	% create fresh file, removing old one if necessary
	save -7 ~/badger_data/fig6-data.mat P par nvals vals

	for j = 1:nvals(2); for i = 1:nvals(1)
		P.p1 = vals{1}(i);
		P.p2 = vals{2}(j) / P.p1;

		P.name = sprintf("d%i_%i", i, j); % di_j

		generate(P);

		load(P.name)
		save("-append", "-7", "~/badger_data/fig6-data.mat", P.name)
		delete(P.name)
	endfor; endfor
endfunction


function plot_data()
	load ~/badger_data/fig6-data.mat % P par nvals vals

	for j = 1:nvals(2)
		for i = 1:nvals(1)
			figure(j)
			subplot(2,2,i)

			graphs(eval(sprintf("d%i_%i", i, j)), 4, 0, 2)

			axis([-5 29 0 1])
			legend off
			xlabel("Time")
			ylabel("")
			title_str = sprintf("p=%i: coverage %i x rate %i",
				vals{2}(j), vals{1}(i), vals{2}(j) / vals{1}(i));
			title(title_str)
			%legend("G", "I/20", "eff beta", "disperal");
		endfor
		print("-dpng", sprintf("gfx/Figure6%s.png", "abc"(j)))
	endfor

endfunction

