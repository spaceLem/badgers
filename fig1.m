function fig1(gen=0)
	if !exist("~/badger_data/fig1-data.mat", "file") || gen == 12345
		gen_data()
	endif

	plot_data()
endfunction


function gen_data()
	P = std_pars();
	P.name = "d1";

	%P.cb = P.cw = P.p1 = P.p2 = 0;
	tic(); generate(P); toc()

	load(P.name)
	save("-7", "~/badger_data/fig1-data.mat", "P", P.name)
	%delete(P.name)
endfunction


function plot_data()
	load ~/badger_data/fig1-data.mat % P d1

	clf()
	subplot(111)

	graphs(d1, 5)
	xlabel("Time")
	ylabel("")
	axis([-5 25 0 0.7])
	title("")

	print -dpng gfx/Figure1.png
endfunction

