function y = despace(x)

while findstr(x, "  ")
	x = strrep(x, "  ", " ");
endwhile
y = x;
