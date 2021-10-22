% A few options of functions
a = '@(x) sin(x)';
b = '@(x) sin(x) + 1';
c = '@(x) x^2+3';
fh = str2func(a); % Having a function ready to plug in x
x = -10+(10+10)*rand(1000, 1);% Generate 1000 x
y = fh(x);
coordiante = ones(1000, 2);
coordiante(:,1) = x;
coordiante(:,2) = y;
dlmwrite('sample_data', coordiante, 'precision', '%.4');


