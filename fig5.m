function fig5(gen=0, nvals = [26, 26, 5])
	if !exist("~/badger_data/fig5-summary.mat", "file") || gen == 12345
		if !exist("~/badger_data/fig5-data.mat", "file") || gen == 12345
			tic; gen_data(nvals); toc;
		endif
		summarise_data()
	endif

	plot_data()
endfunction


function gen_data(nvals)
	P = std_pars();
	P.recs = P.T = 30;

	% parameters of interest
	par = {"p1", "p2", "nu"};
	vals{1} = linspace(0, 1, nvals(1));   % p1
	vals{2} = linspace(0, 2.5, nvals(2)); % p2
	vals{3} = linspace(0, 0.5, nvals(3)); % nu

	% create fresh file, removing old one if necessary
	save -7 ~/badger_data/fig5-data.mat P par nvals vals

	for k = 1:nvals(3); for j = 1:nvals(2); for i = 1:nvals(1)
		P.p1 = vals{1}(i);
		P.p2 = vals{2}(j);
		P.nu = vals{3}(k);
		P.name = sprintf("d5_%i_%i_%i", i, j, k); % di_j_k

		generate(P);

		load(P.name)
		save("-append", "-7", "~/badger_data/fig5-data.mat", P.name)
		delete(P.name)
	endfor; endfor; endfor

	% control (p=0)
	P.cb = P.cw = P.p1 = P.p2 = 0;
	for k = 1:nvals(3)
		P.nu = vals{3}(k);
		P.name = sprintf("d5_0_%i", k);

		generate(P);

		load(P.name)
		save("-append", "-7", "~/badger_data/fig5-data.mat", P.name)
		delete(P.name)
	endfor

	if exist("~/badger_data/fig5-summary.mat", "file")
		delete ~/badger_data/fig5-summary.mat
	endif
endfunction


function summarise_data()
	load ~/badger_data/fig5-data.mat % P par nvals vals

	% t = -10:29
	tidx = floor((P.t0 + P.cy) * P.recs / P.T);

	Pi = G = N = zeros(nvals);

	for k = 1:nvals(3)
		data = eval(sprintf("d5_0_%i.data", k));

		Pi0 = mean(data, 3)(2, tidx);
		G0  = mean(data, 3)(4, tidx);

		for j = 1:nvals(2); for i = 1:nvals(1)
			data = eval(sprintf("d5_%i_%i_%i.data", i, j, k));

			Pi(i,j,k) = mean(data, 3)(2, tidx) - Pi0;
			G(i,j,k)  = mean(data, 3)(4, tidx) - G0;
			N(i,j,k)  = mean(data, 3)(3, tidx);
		endfor; endfor

	endfor


	x = vals{1};
	y = vals{2};
	[X Y] = meshgrid(x, y);
	xi = linspace(x(1), x(end), 100);
	yi = linspace(y(1), y(end), 101); % 50 / 51 ensures we don't mix up G / G'
	[Xi Yi] = meshgrid(xi, yi);

	for k = 1:nvals(3)
		Gi(:,:,k) = interp2(X, Y, G(:,:,k)', Xi, Yi);
		Ni(:,:,k) = interp2(X, Y, N(:,:,k)', Xi, Yi);
	endfor

	Zi = 2*(Gi <= 0) .* (Ni > 2 / P.n^2) + (Gi > 0);

	save -7 ~/badger_data/fig5-summary.mat G N Pi P par nvals vals Gi Ni Zi xi yi Xi Yi X Y
endfunction


function plot_data()
	load ~/badger_data/fig5-summary.mat % G N Pi P par nvals vals Gi Ni Zi xi yi Xi Yi X Y

	for k = 1:nvals(3)
		subplot(2,3,k)
		%Zi{k} = 2*(Gi{k} <= 0) .* (Ni{k} > 2 / P.n^2) + (Gi{k} > 0);

		contourf(Xi, Yi, Zi(:,:,k), [0 1 2])
		colorbar off
		xlabel("Coverage, p1")
		ylabel("Culling rate, p2")
		title(sprintf("nu=%i", vals{3}(k)))
	endfor

	print -dpng gfx/Figure5.png
endfunction

