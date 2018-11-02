function testvar(n1=200, n2=100, v=2)

P = std_pars();
%P.cb = 3;

P.n = 10;
P.runs = n1;
P.name = "d1";
tic; generate(P); toc;
load d1

P.n = 20;
P.runs = n2;
P.name = "d2";
tic; generate(P); toc;
load d2

v1 = d1.data(v, :,:);
v2 = d2.data(v, :,:);

v1se = std(v1, 0, 3) / sqrt(d1.parameters.runs);
v2se = std(v2, 0, 3) / sqrt(d2.parameters.runs);

plot([v1se; v2se]', "LineWidth", 2);

xlabel "Time"
ylabel "Std Error"
legend(sprintf("n=10, %i runs", d1.parameters.runs),
		sprintf("n=20, %i runs", d2.parameters.runs));

endfunction

