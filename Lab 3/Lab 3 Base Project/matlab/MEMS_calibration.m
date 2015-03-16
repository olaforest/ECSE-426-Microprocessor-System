n = 1000;
ones_col = ones(n, 1);
zeros_col = zeros(n, 1);

W1 = [csvread('position1.csv') ones_col];
W2 = [csvread('position2.csv') ones_col];
W3 = [csvread('position3.csv') ones_col];
W4 = [csvread('position4.csv') ones_col];
W5 = [csvread('position5.csv') ones_col];
W6 = [csvread('position6.csv') ones_col];
W = [W1; W2; W3; W4; W5; W6];

Y1 = [zeros_col zeros_col ones_col];
Y2 = [zeros_col zeros_col -ones_col];
Y3 = [zeros_col ones_col zeros_col];
Y4 = [zeros_col -ones_col zeros_col];
Y5 = [ones_col zeros_col zeros_col];
Y6 = [-ones_col zeros_col zeros_col];
Y = [Y1; Y2; Y3; Y4; Y5; Y6];

X = (W' * W) \ W' * Y;