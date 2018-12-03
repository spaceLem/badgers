function rename_files()

load ~/badger_data/fig7-data.mat

d7_0 = d8_0;

save -7 foo.mat P par nvals vals d7_0


for j = 1:nvals(2); for i = 1:nvals(1)
	n1 = sprintf("d8_%i_%i", i, j);
	n2 = sprintf("d7_%i_%i", i, j);
	eval(sprintf("%s = %s;", n2, n1));
	save("-append", "-7", "foo.mat", n2)
endfor; endfor

endfunction

