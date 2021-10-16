function error = eror_cal(heap,data)
    fh = str2func(equation_gen(heap));
    y = fh(data(:,1));
    error = sum(y - data(:,2)) / length(y);
end

function equation = equation_gen(heap)
    equation = heap;
    for i = fliplr(1:floor(length(heap)/2))
        if ~isempty(equation{i})
            if strcmp(equation{i}, "sin") || strcmp(equation{i}, "cos")
                equation{i} = strcat('(', num2str(equation{i}), '(', num2str(equation{2*i}), '.*', num2str(equation{2*i+1}), ')', ')');
            else
                equation{i} = strcat('(', num2str(equation{i*2}), num2str(equation{i}), num2str(equation{i*2+1}), ')');   
            end
        end     
    end
    equation = strcat('@(x)', equation{1});
end