function fig9(gen=0, nvals = [51 4 3])
	if !exist("~/badger_data/fig9-summary.mat", "file") || gen == 12345
		if !exist("~/badger_data/fig9-data.mat", "file") || gen == 12345
			tic; gen_data(nvals); toc;
		endif
		summarise_data()
	endif

	plot_data()
endfunction


function gen_data(nvals)
	P = std_pars();
	P.T = 50;
	P.recs = 40;
	P.runs = 500;

	% parameters of interest
	par = {"cy", "p1", "p"};
	vals{1} = linspace(0, 20, nvals(1));
	vals{2} = [0.1 0.2 0.5 1.0];
	vals{3} = [0.2 0.6, 0.7];
	%P.cp = vals{1}(end);

	nvals = [length(vals{1}), length(vals{2}), length(vals{3})];

	% create fresh file, removing old one if necessary
	save -7 ~/badger_data/fig9-data.mat P par nvals vals

	for k = 1:nvals(3); for j = 1:nvals(2); for i = 1:nvals(1)
		P.cy = vals{1}(i);
		P.p1 = vals{2}(j);
		P.p2 = vals{3}(k) / P.p1;
		P.name = sprintf("d%i_%i_%i", i, j, k); % di_j_k

		generate(P);

		load(P.name);
		save("-append", "-7", "~/badger_data/fig9-data.mat", P.name);
		delete(P.name);
	endfor; endfor; endfor

	% control (p=0)
	P.cb = P.cw = P.p1 = P.p2 = P.cy = 0;
	P.name = "d0";

	generate(P);

	load(P.name)
	save("-append", "-7", "~/badger_data/fig9-data.mat", P.name);
	delete(P.name)
endfunction


function summarise_data()
	load ~/badger_data/fig9-data.mat % P par nvals vals

	Pi = G = zeros(nvals);

	Pi0 = d0.data(2, end);
	G0  = d0.data(4, end);

	for k = 1:nvals(3); for j = 1:nvals(2); for i = 1:nvals(1)
		data = mean(eval(sprintf("d%i_%i_%i.data", i,j,k)), 3);
		Pi(i,j,k) = data(2, end) - Pi0;
		G(i,j,k)  = data(4, end) - G0;
	endfor; endfor; endfor

	save -7 ~/badger_data/fig9-summary.mat Pi G P par nvals vals
endfunction


function plot_data()
	load ~/badger_data/fig9-summary.mat % Pi G P par nvals vals

	for k = 1:nvals(3)
		subplot(nvals(3), 1, k)

		plot(vals{1}, Pi(:,:,k), "LineWidth", 2,
			 vals{1}, 0*vals{1}, "k-")

		xlabel("Duration of cull")
		ylabel("Pi")
		title(sprintf("p=%i", vals{3}(k)));

		for j = 1:nvals(2)
			leg{j} = sprintf("p1=%i, p2=%i",
				vals{2}(j), vals{3}(k) / vals{2}(j));
		endfor
		legend(leg)
	endfor
	print -dpng gfx/Figure10.png
endfunction


