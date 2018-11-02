function x = generate(P)
	cmd = despace(sprintf("nice ./SIspatial %s %s", P.name,
				num2str([P.runs P.icw P.icb P.inf P.mt P.bias P.T P.recs ...
					P.n P.c P.r P.mu P.nu P.kappa P.alpha P.phi P.bw P.bb P.be P.pv ...
					P.cb P.cw P.cp P.cg P.t0 P.cy P.p1 P.p2 P.pb P.dbg P.dist])));

	x = system(cmd);
	disp("")
endfunction

