function fig10(gen=0)
	if !exist("~/badger_data/fig10-data.mat", "file") || gen == 12345
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
	P.p2 = 0.6;
	par = "alpha";
	vals = 0.3:0.2:1.1;
	nvals = length(vals);

	% create fresh file, removing old one if necessary
	save -7 ~/badger_data/fig10-data.mat P par nvals vals

	for i = 1:nvals
		P.alpha = vals(i);
		P.name = sprintf("d%i", i); % di

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
		subplot(3,2,i)

		graphs(eval(sprintf("d%i", i)), 13)
		legend off
		title(sprintf("alpha = %i", vals(i)))
	endfor
	print -dpng gfx/Figure10a.png

	figure(2)
	for i = 1:nvals
		subplot(3,2,i)

		graphs(eval(sprintf("d%i", i)), 12)
		legend off
		title(sprintf("alpha = %i", vals(i)))
	endfor
	print -dpng gfx/Figure10b.png
endfunction

