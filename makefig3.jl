using Plots
using MAT

pyplot()

function makefig3()

	vars = matread("/Users/spacelem/badger_data/fig3-summary.mat")

	xi = vec(vars["xi"])
	yi = vec(vars["yi"])
	Gi = vars["Gi"]
	Ni = vars["Ni"]
	Pii = vars["Pii"]
	Zi = vars["Zi"]

	#Zi = 2 * (Gi .<= 0) .* (Ni .> 0.02) + (Gi .> 0)

	p1 = contourf(xi, yi, Gi,
				  aspect_ratio=0.4, c=:viridis, levels=-0.2:0.1:0.8)
	#contour!(xi, yi, Gi, aspect_ratio=0.4, lc=[:black], levels=-0.2:0.1:0.8)
	xaxis!("Cull coverage, p1")
	yaxis!("Removal rate, p2")

	for q = .1:.1:.9
		p = range(q/2.5, stop=1.0, length=101);
		qx = q ./ p
		plot!(p, qx, c=:red, grid=false, label="")
	end

	# -----

	p2 = contourf(xi, yi, Pii,
				  aspect_ratio=0.4, c=:viridis, levels=-3:4)
	#contour!(xi, yi, Pii, aspect_ratio=0.4, lc=[:black], levels=-3:4)
	xaxis!("Cull coverage, p1")
	yaxis!("Removal rate, p2")

	for q = .1:.1:.9
		p = range(q/2.5, stop=1.0, length=101);
		qx = q ./ p
		plot!(p, qx, c=:red, grid=false, label="")
	end

	# ----

	p3 = contourf(xi, yi, Ni,
				  aspect_ratio=0.4, c=:viridis, levels = 0:2:20)
	#contour!(xi, yi, Ni, aspect_ratio=0.4, lc=[:black], levels = 0:2:20)

	xaxis!("Cull coverage, p1")
	yaxis!("Removal rate, p2")

	xaxis!("Cull coverage, p1")
	yaxis!("Removal rate, p2")

	# -----

	p4 = contourf(xi, yi, Zi,
				  aspect_ratio=0.4, c=:viridis)#, colorbar=:none)
	#contour!(xi, yi, Zi, aspect_ratio=0.4, lc=[:black])#, colorbar=:none)
	xaxis!("Cull coverage, p1")
	yaxis!("Removal rate, p2")

	# ----

	plot(p1,p2,p3,p4, layout=(2,2), size=(710, .8*710))
	#savefig("Figure2a.svg")
end
