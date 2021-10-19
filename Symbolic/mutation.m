function heapMu = mutation(heap)
    temp = heap;
    mutationIndex = [];
    mutationO = [];
    operators = {'+', '-', '.*', './', 'cos', 'sin'};
    % get the locations of constants excluding "x"
    for i = 1:length(temp)
        if isa(temp{i}, 'double') && ~isempty(temp{i})
            mutationIndex(end+1) = i;
        end
    end
    % get the locations of operators
    for ii = 1:length(temp)
        if isa(temp{ii}, 'char') && ~strcmp(temp{ii}, 'x') && ~contains(temp{ii},'0.01.*x') 
            mutationO(end+1) = ii;
        end
    end
    % Consider the scenario where there is no "cons" in the heap
    if isempty(mutationIndex)
        for j = 1: length(temp)
            if strcmp(temp{j}, 'x')
                % Find all the locations containing "x"
                mutationIndex(end + 1) = j;
            end
        end
        % Consider the scenario where there are all 0.01.*x
        if isempty(mutationIndex)
            for alpha = 1: length(temp)
                if ~isa(temp{alpha}, 'char')
                    continue;
                end
                if contains(temp{alpha},'0.01.*x') 
                    mutationIndex(end+1) = alpha;
                end 
            end
        end
        muLocation = mutationIndex(randi(length(mutationIndex)));
        min = -10;
        max = 10;
        r = (max - min).*rand(500, 1) + min;
        temp{muLocation} = r(randi(500));
    else
        % randomly select a constant location to mutate
        muLocation = mutationIndex(randi(length(mutationIndex)));
        muLocationO = mutationO(randi(length(mutationO)));
        if rand < 0.3 
            min = -10;
            max = 10;
            r = (max - min).*rand(500, 1) + min;
            temp{muLocation} = temp{muLocation} + r(randi(500));
        else          
            temp{muLocation} = strcat('(0.01.*x+', num2str(temp{muLocation}),')');
        end
        if rand > 0.5
            temp(muLocationO) = operators(randi(length(operators)));
        end
    end
    heapMu = temp;
end