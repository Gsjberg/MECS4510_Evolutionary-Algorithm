function [] = main()
%Set parameters
clear
clc
rng('shuffle')
operators = {'+', '-', '.*', './', 'cos', 'sin'};
constant = {'x', 'cons'};
data = csvread("data.csv");
data_training = data(1:length(data)/2, :);
data_validation = data(length(data)/2:end, :);
evl = 3000;
run = 4;
maxlevel = 4;
%randomsearch(operators, constant, data_training, evl, run, maxlevel);
hillclimber(operators, constant, data_training, evl, run, maxlevel);
%GA(operators, constant, data_training, evl, run, maxlevel);
%GA_diversity(operators, constant, data_training, data_validation, evl, run, maxlevel)
end