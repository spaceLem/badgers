function fig2(gen=0, nvals = [26, 26])
	if !exist("~/badger_data/fig2-summary.mat", "file") || gen == 12345
		if !exist("~/badger_data/fig2-data.mat", "file") || gen == 12345
			tic; gen_data(nvals); toc;
		endif
		summarise_data()
	endif

	if gen == 1
		plot_data1()
	else
		plot_data()
	endif
endfunction


function gen_data(nvals);
	P = std_pars();
	P.T = 40;
	P.recs = 40; % only need a few recs

	% parameters of interest
	par = {"p1", "p2"};
	vals{1} = linspace(0, 1, nvals(1));   % p1
	vals{2} = linspace(0, 2.5, nvals(2)); % p2

	% create fresh file, removing old one if necessary
	save -7 ~/badger_data/fig2-data.mat P par nvals vals

	for j = 1:nvals(2); for i = 1:nvals(1)
		P.p1 = vals{1}(i);
		P.p2 = vals{2}(j);
		P.name = sprintf("d%i_%i", i, j); % di_j

		generate(P);

		load(P.name)
		save("-append", "-7", "~/badger_data/fig2-data.mat", P.name)
		delete(P.name)
	endfor; endfor

	% control (p=0)
	P.cb = P.cw = P.p1 = P.p2 = 0;
	P.name = "d0";

	generate(P);

	load(P.name)
	save("-append", "-7", "~/badger_data/fig2-data.mat", P.name)
	delete(P.name)
endfunction


function summarise_data()
	load ~/badger_data/fig2-data.mat % P par nvals vals

	% t = -10:29
	% take value right after cull is over
	tidx = floor((P.t0 + P.cy) * P.recs / P.T);

	Pi = G = N = zeros(nvals);

	Pi0 = mean(d0.data, 3)(2, tidx);
	G0  = mean(d0.data, 3)(4, tidx);

	for j = 1:nvals(2); for i = 1:nvals(1)
		data = eval(sprintf("d%i_%i.data", i, j));

		Pi(i, j) = mean(data, 3)(2, tidx) - Pi0;
		G(i, j)  = mean(data, 3)(4, tidx) - G0;
		N(i, j)  = mean(data, 3)(3, tidx);
	endfor; endfor

	% do interpolation
	x = vals{1};
	y = vals{2};
	[X Y] = meshgrid(x, y);

	% test with nvals(1) != nvals(2) to ensure we don't mix up G / G'
	xi = linspace(x(1), x(end), 100);
	yi = linspace(y(1), y(end), 101);
	[Xi Yi] = meshgrid(xi, yi);

	Pii = interp2(X, Y, Pi', Xi, Yi);
	Gi  = interp2(X, Y, G',  Xi, Yi);
	Ni  = interp2(X, Y, N',  Xi, Yi);

	Zi = 2*(Gi <= 0) .* (Ni > 2) + (Gi > 0);

	save -7 ~/badger_data/fig2-summary.mat P par nvals vals Pii Gi Ni xi yi Xi Yi Zi
endfunction


function plot_data()
	load ~/badger_data/fig2-summary.mat % P par nvals vals Pii Gi Ni xi yi Xi Yi Zi

	subplot(221)

	contourf(Xi, Yi, Pii);
	colorbar;
	xlabel("Coverage, p1");
	ylabel("Culling rate, p2");
	title("Pi");
	axis "square"

	hold on
	for p = 0.1:0.1:1;
		px = linspace(p / 2.5, 1, 101);
		py = p ./ px;
		plot(px, py, "r-", "LineWidth", 2)
	end
	hold off

	%--------------------------------------------------------------------------

	subplot(222)

	contourf(Xi, Yi, Gi, -.2:.1:.6)
	colorbar
	xlabel("Coverage, p1")
	ylabel("Culling rate, p2")
	title("G")
	axis "square"

	hold on
	for p = 0.1:0.1:1;
		px = linspace(p / 2.5, 1, 101);
		py = p ./ px;
		plot(px, py, "r-", "LineWidth", 2)
	end
	hold off

	%--------------------------------------------------------------------------

	subplot(223)

	contourf(Xi, Yi, Ni)
	colorbar
	xlabel("Coverage, p1")
	ylabel("Culling rate, p2")
	title("Popn size after cull")
	axis "square"

	%--------------------------------------------------------------------------

	subplot(224)

	contourf(Xi, Yi, Zi, [0 1 2], climits=[0 2])
	colorbar
	xlabel("Coverage, p1")
	ylabel("Culling rate, p2")
	title("Safe area to cull")
	axis "square"

	print -dpng gfx/Figure2.png
endfunction


function plot_data1()
	load ~/badger_data/fig2-summary.mat % G N Pi P par nvals vals

	subplot(111)

	contourf(Xi, Yi, Gi, -.2:.1:.6)
	colorbar
	xlabel("Coverage, p1")
	ylabel("Culling rate, p2")
	title("Pi groups")

	hold on
	for p = 0.1:0.1:1;
		px = linspace(p / 2.5, 1, 101);
		py = p ./ px;
		plot(px, py, "r-", "LineWidth", 2)
	end
	hold off

	print -dpng gfx/Figure2a.png
endfunction

