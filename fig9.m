function fig9(gen=0)
	if !exist("~/badger_data/fig9-data.mat", "file") || gen == 12345
		tic; gen_data(); toc;
	endif

	plot_data()
endfunction


function gen_data()
	P = std_pars();
	P.kappa = 0.1;
	P.cy = 20;
	P.T = 40;

	% parameters of interest
	P.cb = 3; % turns on core + boundary!
	P.p1 = 1.0;
	P.p2 = 0.6;
	par = "alpha";
	vals = 0.3:0.2:1.1;
	nvals = length(vals);

	% create fresh file, removing old one if necessary
	save -7 ~/badger_data/fig9-data.mat P par nvals vals

	for i = 1:nvals
		P.alpha = vals(i);
		P.name = sprintf("d9_%i", i); % di

		generate(P);

		load(P.name)
		save("-append", "-7", "~/badger_data/fig9-data.mat", P.name)
		delete(P.name)
	endfor
endfunction


function plot_data()
	load ~/badger_data/fig9-data.mat % P par nvals vals

	figure(1)
	for i = 1:nvals
		subplot(3,2,i)

		graphs(eval(sprintf("d9_%i", i)), 13)
		legend off
		title(sprintf("alpha = %i", vals(i)))
	endfor
	print -dpng gfx/Figure9A.png

	figure(2)
	for i = 1:nvals
		subplot(3,2,i)

		graphs(eval(sprintf("d9_%i", i)), 12)
		legend off
		title(sprintf("alpha = %i", vals(i)))
	endfor
	print -dpng gfx/Figure9B.png
endfunction

