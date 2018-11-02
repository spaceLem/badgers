function compare(x, y)

recs  = x.parameters.recs;
runs  = x.parameters.runs;
T     = x.parameters.T;
t0    = x.parameters.t0;
t1    = x.parameters.t1;
c     = x.parameters.c;
bw    = x.parameters.bw;
p1    = x.parameters.p1;
p2    = x.parameters.p2;
alpha = x.parameters.alpha;
cy    = x.parameters.cull_years;

t = (0:recs-1) * T / recs - t0;

N  = mean(x.data, 3)'(:, 3) / c;
Gx = mean(x.data, 3)'(:, 4);
Gy = mean(y.data, 3)'(:, 4);
Pi = Gx - Gy;

% note: t1 - t0 = cy + t0 unless cg > 0
tidx = ceil((t1 - t0) * recs / T);
Pi(tidx)

maxval = 1.1 * max([Gx; Gy; Pi]);
minval = 1.1 * min([Gx; Gy; Pi; 0]);

plot(t, [Gx Gy Pi N], "LineWidth", 2,
     t, 0*t, "k-",
	 [0 0], [minval maxval], "k--",
	 (t1-t0)*[1 1], [minval maxval], "k--");

xlabel("Time");
ylabel("G");
title(sprintf("alpha=%i: p1=%i, p2=%i, p=%i", a, p1, p2, p1*p2));
axis([t(1) t(end) minval maxval]);

legend("cull", "no cull", "Pi-G", "N/c", "location", "northeastoutside");


