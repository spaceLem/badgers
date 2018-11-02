function fig1(gen=0)
	if !exist("~/badger_data/fig1-data.mat", "file") || gen == 12345
		gen_data()
	endif

	plot_data()
endfunction


function gen_data()
	P = std_pars();
	P.name = "d1";
	P.n = 10;
	P.kappa = 1;
	P.cy = 10;
	P.T = 30;
	P.cb = 3;
	P.p1 = 1;
	P.p2 = 1;

	tic; generate(P); toc;

	load d1
	save -7 ~/badger_data/fig1-data.mat d1
	%delete d1
endfunction


function plot_data()
	load ~/badger_data/fig1-data.mat

	clf()
	subplot(111)

	graphs(d1, 5)
	axis([-5 29 0 1])
	title("")

	print -dpng gfx/Figure1.png
endfunction

