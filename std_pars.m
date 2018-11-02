function P = std_pars(scenario = 1)
	% simulation setup
	P.runs = 200;	% number of times to run simulation (100 is probably good)
	P.icw = 0;		% Initial Conditions (ICs, 0=near DFE, 1=EE)
	P.icb = 0.2;	% initial G (0..1)
	P.inf = 0;		% infection type (0=DD, 1=FD)
	P.mt = 0;		% movement type (0=NN torus, 1=NN Edges, 2=NegExp, 3=Global)
	P.bias = 0;		% movement bias (0=step, 1=bias at c, 2=bias at K, 3=diffusion)
	P.T = 50;		% years of simulation
	P.recs = 200;	% num records to keep (usually either 200 or T)
	P.n = 10;		% no. groups = n x n

	% population
	P.c = 20;		% glass ceiling carrying capacity
	P.r = 1;		% intrinsic reproduction rate
	P.mu = 0.1;		% natural mortality rate
	P.nu = 0.1;		% disease induced mortality rate
	P.kappa = 0.1;	% movement rate
	P.alpha = 0.7;	% movement bias threshold
	P.phi = 1;		% NegExp movement parameter

	% infection parameters
	P.bw = 0.1;		% with groups rate
	P.bb = 0;		% between groups rate
	P.be = 0;		% environmental (background) rate
	P.pv = 0;		% vertical transmission probability

	% cull parameters
	P.cb = 1;		% cull between (0=no cull, 1=random p1, 2=p1 block, 3=inner core
	P.cw = 1;		% cull within (0=none, 1=continuous, 2=annual)
	P.cp = 1;		% cull pulse (duration time)
	P.cg = 0;		% cull gap (between cull)
	P.t0 = 10;		% cull start time
	P.cy = 20;		% cull duration
	P.p1 = 0.2;		% cull coverage
	P.p2 = 1;		% cull within groups
	P.pb = 0;		% fake a PE

	% optional switches
	P.dbg = 0;	% level of details
	P.dist = 0;	% also some summary stats

	if scenario == 2
		% badgers
		P.c = 20; P.r = 1; P.mu = 0.01; P.nu = 0.1; P.kappa = 0.1; P.alpha = 0.7; P.phi = 1;
		P.bw = 0.1; P.bb = 0; P.be = 0; P.pv = 0;
	elseif scenario == 3
		% stable
		P.c = 20; P.r = 1; P.mu = 0.01; P.nu = 0.1; P.kappa = 0.1; P.alpha = 0.7; P.phi = 1;
		P.bw = 0.1; P.bb = 0; P.be = 0; P.pv = 0;
	endif

endfunction

