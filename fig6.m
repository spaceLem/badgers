function fig6(gen=0)
	if !exist("~/badger_data/fig6-data.mat", "file") || gen == 12345
		tic; gen_data(); toc
	endif

	plot_data()
endfunction


function gen_data()
	P = std_pars();

	% parameters of interest
	P.cy = 3;
	P.name = "d6";

	generate(P);

	load d6
	save -7 ~/badger_data/fig6-data.mat d6 P
	delete d6
endfunction


function plot_data()
	load ~/badger_data/fig6-data.mat % P par vals

	subplot(111)

	graphs(d6, 5)

	axis([-5 29 0 0.7])
	%legend off
	xlabel("Time")
	ylabel("")
	%title(sprintf("p=%i, coverage %i x rate %i", P.p1 * P.p2, P.p1, P.p2))
	title("")

	print -dpng gfx/Figure6.png
endfunction

