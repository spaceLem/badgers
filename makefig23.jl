using Plots
using MAT

function makefig23(fig::Int=2)

	vars = matread("data/fig2-summary.mat")

	xi = vec(vars["xi"])
	yi = vec(vars["yi"])
	Gi = vars["Gi"]
	Ni = vars["Ni"]
	Pii = vars["Pii"]

	Z = 2 * (Gi .<= 0) .* (Ni .> 1) + (Gi .> 0)

	if fig == 2

		p1 = contour(xi, yi, Gi,
			aspect_ratio=0.4, fill=true, c=:viridis, levels=-.3:.1:.6)
		xaxis!("Cull coverage, p1")
		yaxis!("Removal rate, p2")

		# -----

		p2 = contour(xi, yi, Pii,
			aspect_ratio=0.4, fill=true, c=:viridis, levels=-4:4)
		xaxis!("Cull coverage, p1")
		yaxis!("Removal rate, p2")

		# ----

		p3 = contour(xi, yi, Gi,
			aspect_ratio=0.4, fill=true, c=:viridis, levels=-.3:.1:.6)
		xaxis!("Cull coverage, p1")
		yaxis!("Removal rate, p2")

		for q = .1:.1:.9
			p = linspace(q/2.5, 1, 101);
			qx = q ./ p
			plot!(p, qx, c=:red, grid=false, label="")
		end

		# -----

		p4 = contour(xi, yi, Pii,
			aspect_ratio=0.4, fill=true, c=:viridis, levels=-4:4)
		xaxis!("Cull coverage, p1")
		yaxis!("Removal rate, p2")

		for q = .1:.1:.9
			p = linspace(q/2.5, 1, 101);
			qx = q ./ p
			plot!(p, qx, c=:red, grid=false, label="")
		end

		# ----

		plot(p1,p2,p3,p4, layout=(2,2), size=(710, .8*710))
		#savefig("Figure2a.svg")

	else

		p5 = contour(xi, yi, Ni,
			aspect_ratio=0.4, fill=true, c=:viridis)
		xaxis!("Cull coverage, p1")
		yaxis!("Removal rate, p2")

		# -----

		p6 = contour(xi, yi, Z,
			aspect_ratio=0.4, fill=true, c=:viridis, colorbar=:none)
		xaxis!("Cull coverage, p1")
		yaxis!("Removal rate, p2")

		# ----

		plot(p5, p6, layout=2, size=(710, .4*710))
		#savefig("Figure2b.svg")

	end
end
