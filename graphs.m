function graphs(file=A, type=2, use_std_dev=1, annotate=2)
% plot the data from SI simulation
%
% file - the big file with all the numbers
% type - which parts to extract
%		 1..5:  SIN, G, SIN (single lines), [I, G, H_eff, D_eff], [I, G]
%		 7..9:  H, H_eff, rates, move, p_v
%		10..12: I, G, H, dispersal (inner core)
%		17..16: I, G, H, dispersal (boundary)
%		21..20: I, G, H, dispersal (peripheral area)
% use_std_dev - 0: ignore variance, 1: use std err, 2: use std dev
% annotate - 0: nothing, 1: draw labels and legends, 2: draw cull start/end

recs = file.parameters.recs;
runs = file.parameters.runs;
T = file.parameters.T;
t0 = file.parameters.t0;
t1 = file.parameters.t1;
c = file.parameters.c;
bw = file.parameters.bw;

t = (0:recs-1) * T / recs - t0;

file_mu = mean(file.data, 3)';

% set up error bars
if runs == 1
	use_std_dev = 0;
endif

switch use_std_dev
case 0
	% ignore variance, just show mean
	se = 0;
	errs = "";
case 1
	% calculate 95% CI of mean
	se = tinv(.975, runs) / sqrt(runs);
	errs = ", with 95% CI";
otherwise
	% just show the std dev
	se = 1;
	errs = ", with std dev";
endswitch
file_sigma = std(file.data, 0, 3)' * se;

% this takes file_me(:, x) and generates x as a variable to plot
vals = {"S", "I", "N", "G", ...
	"birth", "death", "infect", "move", ...
	"beta_eff", "vt", "prevalence", ...
	"I3", "G3", "infect3", "move3", ...
	"I2", "G2", "infect2", "move2", ...
	"I1", "G1", "infect1", "move1", ...
	"I0", "G0", "infect0", "move0"};

for i = 1:size(file_mu, 2)
	eval(sprintf("%s = file_mu(:, %i);", vals{i}, i));
	eval(sprintf("%sv = file_sigma(:, %i);", vals{i}, i));
endfor


switch type
case 1
	% populations S, I, N
	plot(t, [S I N], {"b-", "g-", "r-"}, "LineWidth", 2,
		 t, [S + Sv, S - Sv], "b-",
		 t, [I + Iv, I - Iv], "g-",
		 t, [N + Nv, N - Nv], "r-")

	maxval = max(N + Nv);
	title_str = "Global population numbers vs t";
	ystr = "Global mean number";
	lstr = {"Susceptible (S)", "Infected (I)", "Total popn (N)"};

case 2
	% proportion of infected groups G
	plot(t, G, "g-", "LineWidth", 2,
		 t, [G + Gv, G - Gv], "g-")

	maxval = 1;
	title_str = "Proportion of infected groups vs time";
	ystr = "Proportion of infected groups";
	lstr = {"Proportion of infected groups"};

case 3
	% populations S, I, N, with no error bars
	plot(t, [S I N], {"b-", "g-", "r-"}, "LineWidth", 2)

	maxval = max([S; I; N]);
	title_str = "Global population numbers vs time";
	ystr = "Global mean number";
	lstr = {"Susceptible (S)", "Infected (I)", "Total popn (N)"};

case 4
	% G, I, Heff and Deff, with no error bars
	plot(t, [G I/c beta_eff move], "LineWidth", 2)

	maxval = 1;
	title_str = "Global population numbers vs time";
	ystr = "Global mean number";
	lstr = {"Proportion of infected groups", "Infected (I) / c", "Effective transmission", "Dispersal"};

case 5
	% G, I, Heff and Deff, with no error bars
	plot(t, [G I/c], "LineWidth", 2)

	maxval = 1;
	title_str = "Global population numbers vs time";
	ystr = "Global mean number";
	lstr = {"Proportion of infected groups", "Infected (I) / c"};

case 6
	% show horizontal transmission
	plot(t, infect, "r-", "LineWidth", 2,
		 t, [infect + infectv, infect - infectv], "r-")

	maxval = max(infect + infectv);
	title_str = "Horizontal transmission vs time";
	ystr = "Global mean annual rate";
	lstr = {"Horizontal transmission rate"};

case 7
	% effective transmission rate
	plot(t, beta_eff, "r-", "LineWidth", 2,
		 t, [beta_eff + beta_effv, beta_eff - beta_effv], "r-")

	maxval = max(beta_eff + beta_effv);
	title_str = "Effective horizontal transmission vs time";
	ystr = "Global mean annual rate";
	lstr = {"Effective Transmission Rate"};

case 8
	% show the rates
	plot(t, [birth death infect move], {"b-", "r-", "g-", "c-"}, "LineWidth", 2,
		 t, [birth + birthv, birth - birthv], "b-",
		 t, [death + deathv, death - deathv], "r-",
		 t, [infect + infectv, infect - infectv], "g-",
		 t, [move + movev, move - movev], "c-")

	maxval = 1.1*max(cat(1, birth + birthv, death + deathv, infect + infectv, move + movev));
	title_str = "Global rates vs time";
	ystr = "Global mean annual rate";
	lstr = {"Birth rate", "Death rate", "Infection rate", "Dispersal rate"};

case 9
	% show dispersal
	plot(t, move, "r-", "LineWidth", 2,
		 t, [move + movev, move - movev], "r-")

	maxval = max(move + movev);
	title_str = "Dispersal vs time";
	ystr = "Global mean annual rate";
	lstr = {"Dispersal rate"};

case 10
	% show vertical transmission
	plot(t, vt, "g-",
		 t, [vt + vtv, vt - vtv], "g-");

	maxval = max(vt + vtv);
	title_str = "Vertical transmission rate vs time";
	ystr = "Global mean annual rate";
	lstr = {"Vertical transmission rate"};


case 11
	% G, dispersal and disease transmission
	plot(t, [G 0.2*infect move], {"g-", "r-", "c-"}, "LineWidth", 2)
		%t, [G + Gv, G - Gv], "g-")

	maxval = max([G; 0.2*infect; move]);
	title_str = "Rates";
	ystr = "Global mean proportion";
	lstr = {"Proportion of infected groups", "Disease Transmission", "Dispersal"};
case 12
	% G in and outside core
	plot(t, [G0 G1 G2 G3], {"g-", "r-", "m-", "b-"}, "LineWidth", 2)

	if use_std_dev
		hold on;
		plot(t, [G0 + G0v, G0 - G0v], "g-",
			 t, [G1 + G1v, G1 - G1v], "r-",
			 t, [G2 + G2v, G2 - G2v], "r-",
			 t, [G3 + G3v, G3 - G3v], "b-")
		hold off;
	endif

	maxval = 1;
	title_str = "Proportion of infected groups vs time";
	ystr = "G(t)";
	lstr = {"core", "boundary1", "boundary2", "periphery"};
case 13
	% I in and outside core
	plot(t, [I0 I1 I2 I3], {"g-", "r-", "m-", "b-"}, "LineWidth", 2,
		 t, 0*t, "k-")

	%maxval = max(cat(2, I0, I1, I2, I3));
	maxval = c/1.2;
	title_str = "Population numbers vs time";
	ystr = "I(t)";
	lstr = {"core", "boundary1", "boundary2", "periphery"};
case 14
	% dispersal in and outside core
	plot(t, [move0 move1 move2 move3], {"g-", "r-", "m-", "b-"}, "LineWidth", 2,
		 t, 0*t, "k-")

	maxval = max([move0; move1; move2; move3]);
	title_str = "Rate vs time";
	ystr = "Dispersal rate";
	lstr = {"core", "boundary1", "boundary2", "periphery"};
otherwise
	disp("set type 1--14 (you probably wanted 4...)")
	return
endswitch

axis([t(1), t(end), 0, 1.2*maxval])

if annotate == 0
	title("")
	xlabel("")
	ylabel("")
	legend("off")
else
	title(sprintf("%s%s", title_str, errs), "FontName", "Times New Roman", "FontSize", 12)
	xlabel("Time", "FontName", "Times New Roman", "FontSize", 12)
	ylabel(ystr, "FontName", "Times New Roman", "FontSize", 12)
	legend(lstr)
endif

if annotate == 2
	limits = axis;
	hold on
	plot([0 0], limits(3:4), "k--", (t1 - t0)*[1 1], limits(3:4), "k--")
	hold off
endif

endfunction

