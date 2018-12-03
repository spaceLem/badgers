function fig7(gen=0, nvals = [51 4])
	if !exist("~/badger_data/fig7-summary.mat", "file") || gen == 12345
		if !exist("~/badger_data/fig7-data.mat", "file") || gen == 12345
			tic; gen_data(nvals); toc;
		endif
		summarise_data()
	endif

	plot_data()
endfunction


function gen_data(nvals)
	P = std_pars();
	P.recs = P.T;

	% parameters of interest
	par = {"cy", "p1", "p"};
	vals{1} = linspace(0, 20, nvals(1));
	vals{2} = [0.1 0.2 0.5 1.0];
	%P.cp = vals{1}(end);

	nvals = [length(vals{1}), length(vals{2})];

	% create fresh file, removing old one if necessary
	save -7 ~/badger_data/fig7-data.mat P par nvals vals

	for j = 1:nvals(2); for i = 1:nvals(1)
		P.cy = vals{1}(i);
		P.p1 = vals{2}(j);
		P.p2 = 0.7 / P.p1;
		P.name = sprintf("d7_%i_%i", i, j); % di_j

		generate(P);

		load(P.name);
		save("-append", "-7", "~/badger_data/fig7-data.mat", P.name);
		delete(P.name);
	endfor; endfor

	% control (p=0)
	P.cb = P.cw = P.p1 = P.p2 = P.cy = 0;
	P.name = "d7_0";

	generate(P);

	load(P.name)
	save("-append", "-7", "~/badger_data/fig7-data.mat", P.name);
	delete(P.name)
endfunction


function summarise_data()
	load ~/badger_data/fig7-data.mat % P par nvals vals

	Pi = G = Pi_se = G_se = zeros(nvals);

	se = tinv(.975, P.runs) / sqrt(P.runs);

	Pi0 = d7_0.data(2, end);
	G0  = d7_0.data(4, end);

	for j = 1:nvals(2); for i = 1:nvals(1)
		data_mu    = mean(eval(sprintf("d7_%i_%i.data", i, j)), 3);
		data_sigma = std(eval(sprintf("d7_%i_%i.data", i, j)), 0, 3);

		Pi(i,j)    = data_mu(2, end) - Pi0;
		G(i,j)     = data_mu(4, end) - G0;
		Pi_se(i,j) = data_sigma(2, end) * se;
		G_se(i,j)  = data_sigma(4, end) * se;
	endfor; endfor

	save -7 ~/badger_data/fig7-summary.mat Pi G Pi_se G_se P par nvals vals
endfunction


function plot_data()
	load ~/badger_data/fig7-summary.mat % Pi G Pi_se G_se P par nvals vals

	h1 = plot(vals{1}, Pi, "LineWidth", 2,
			%vals{1}, [Pi + Pi_se],
			%vals{1}, [Pi - Pi_se],
			vals{1}, 0*vals{1}, "k-");

	%palette = viridis(4);
	%for i = 1:12: set(h1(i), "color", palette(mod(i-1,4)+1,:)); endfor


	xlabel("Duration of cull")
	ylabel("Pi")
	title("");

	for j = 1:nvals(2)
		leg{j} = sprintf("p1=%i, p2=%i",
			vals{2}(j), 0.7 / vals{2}(j));
	endfor
	legend(leg)

	print -dpng gfx/Figure7.png
endfunction


