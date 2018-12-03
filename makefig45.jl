using Plots
using MAT

pyplot()

function makefig45(fig::Int=4, S::Bool=false)

	if S
		file = "/Users/spacelem/badger_data/figS$fig-summary.mat"
	else
		file = "/Users/spacelem/badger_data/fig$fig-summary.mat"
	end #if

	vars = matread(file)

	par = vars["par"][3]
	nvals = convert(Array{Int}, vec(vars["nvals"]))
	vals3 = vec(vars["vals"][3])
	xi = vec(vars["xi"])
	yi = vec(vars["yi"])
	Gi = vars["Gi"]
	Ni = vars["Ni"]
	Zi = vars["Zi"]

	plt = plot(5, layout=5, clf=true, grid=false, label="", size=(710, .8*710))

	for i = 1:5
		contour!(xi, yi, Zi[:,:,i],
				title=string(par, " = ", round(vals3[i], sigdigits=2)),
				#titlefont=font(12, "Times New Roman"),
				subplot=i, fill=true, c=:viridis, ratio=0.4, clims=(0,2),
				colorbar=:none)

		#xaxis!("Cull coverage, p1", font(12, "Times New Roman"))
		#yaxis!("Removal rate, p2", font(12, "Times New Roman"))
	end #for

	display(plt)

	if S
		savefig("FigureS$fig.svg")
	else
		savefig("Figure$fig.svg")
	end #if
end #fn
