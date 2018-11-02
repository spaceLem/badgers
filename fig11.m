function fig11(gen=0)
	if !exist("~/badger_data/fig11-data.mat", "file") || gen == 12345
		tic; gen_data(); toc;
	endif

	plot_data()
endfunction


function gen_data()
	P = std_pars();

	% parameters of interest
	P.n = 20;
	P.cb = 3; % turns on core + boundary!
	P.p1 = 1.0;
	P.p2 = 1.0;
	par = "kappa";
	vals = [0.1, 0.5, 1.0];
	nvals = length(vals);

	% create fresh file, removing old one if necessary
	save -7 ~/badger_data/fig11-data.mat P par nvals vals

	for i = 1:nvals
		P.kappa = vals(i);
		P.name = sprintf("d%i", i); % di

		generate(P);

		load(P.name)
		save("-append", "-7", "~/badger_data/fig11-data.mat", P.name)
		delete(P.name)
	endfor
endfunction


function plot_data()
	load ~/badger_data/fig11-data.mat % P par nvals vals

	figure(1)
	for i = 1:nvals
		subplot(2,2,i)

		graphs(eval(sprintf("d%i", i)), 13)
		legend off
		title(sprintf("kappa = %i", vals(i)))
	endfor
	print -dpng gfx/Figure11a.png

	figure(2)
	for i = 1:nvals
		subplot(2,2,i)

		graphs(eval(sprintf("d%i", i)), 12)
		legend off
		title(sprintf("kappa = %i", vals(i)))
	endfor
	print -dpng gfx/Figure11b.png
endfunction


