function fig8(gen=0)
	if !exist("~/badger_data/fig8-data.mat", "file") || gen == 12345
		tic; gen_data(); toc
	endif

	plot_data()
endfunction


function gen_data()
	P = std_pars();

	% parameters of interest
	P.cy = 1;
	P.p1 = 1.0;
	P.p2 = 1.2;
	P.name = "d1";

	generate(P);

	load d1
	save -7 ~/badger_data/fig8-data.mat d1 P
	delete d1
endfunction


function plot_data()
	load ~/badger_data/fig8-data.mat % P par vals

	subplot(111)

	graphs(d1, 4, 0)

	axis([-5 29 0 1])
	legend off
	xlabel("Time")
	ylabel("")
	title(sprintf("p=%i, coverage %i x rate %i",
		P.p1 * P.p2, P.p1, P.p2))

	print -dpng gfx/Figure8.png
endfunction

