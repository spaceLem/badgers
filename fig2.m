function fig2(gen=0)
	if !exist("~/badger_data/fig2-data.mat", "file") || gen == 12345
		tic; gen_data(); toc
	endif

	plot_data()
endfunction


function gen_data()
	P = std_pars();

	% parameters of interest
	par = "p1";
	vals = [0.1 0.2 0.5 1.0];

	nvals = length(vals);

	% create fresh file, removing old one if necessary
	save -7 ~/badger_data/fig2-data.mat P par nvals vals

	for i = 1:nvals
		P.p1 = vals(i);
		P.p2 = 0.7 / P.p1;

		P.name = sprintf("d2_%i", i); % di

		generate(P);

		load(P.name)
		save("-append", "-7", "~/badger_data/fig2-data.mat", P.name)
		delete(P.name)
	endfor
endfunction


function plot_data()
	load ~/badger_data/fig2-data.mat % P par nvals vals

	for i = 1:nvals(1)
		subplot(2,2,i)

		graphs(eval(sprintf("d2_%i", i)), 5)

		axis([-5 29 0 1])
		legend off
		xlabel("Time")
		ylabel("")
		title_str = sprintf("p=0.7: coverage %i x rate %i",
			vals(i), 0.7 / vals(i));
		title(title_str)
		%legend("G", "I/20", "eff beta", "disperal");
		print -dpng gfx/Figure2.png
	endfor

endfunction


