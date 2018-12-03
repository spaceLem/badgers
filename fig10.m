function fig10(gen=0)
	if !exist("~/badger_data/fig10-data.mat", "file") || gen == 12345
		tic; gen_data(); toc;
	endif

	plot_data()
endfunction


function gen_data()
	P = std_pars();

	% parameters of interest
	P.n = 10;
	P.runs = 1000;
	P.cb = 3; % turns on core + boundary!
	P.p1 = 1.0;
	P.p2 = 1.0;
	par = "kappa";
	vals = [0.1, 0.5, 1.0];
	nvals = length(vals);

	% create fresh file, removing old one if necessary
	save -7 ~/badger_data/fig10-data.mat P par nvals vals

	for i = 1:nvals
		P.kappa = vals(i);
		P.name = sprintf("d10_%i", i); % di

		generate(P);

		load(P.name)
		save("-append", "-7", "~/badger_data/fig10-data.mat", P.name)
		delete(P.name)
	endfor
endfunction


function plot_data()
	load ~/badger_data/fig10-data.mat % P par nvals vals

	figure(1)
	for i = 1:nvals
		subplot(2,2,i)

		graphs(eval(sprintf("d10_%i", i)), 13, 0)
		legend off
		title(sprintf("kappa = %i", vals(i)))
	endfor
	print -dpng gfx/Figure10A.png

	figure(2)
	for i = 1:nvals
		subplot(2,2,i)

		graphs(eval(sprintf("d10_%i", i)), 12, 0)
		legend off
		title(sprintf("kappa = %i", vals(i)))
	endfor
	print -dpng gfx/Figure10B.png
endfunction


