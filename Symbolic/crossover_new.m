function [child1, child2] = crossover_new(parent1, parent2, maxLevel)
    %%%%%%% Specification %%%%%%%%%%%%%%
    % The crossover location has to be non-empty for both parents;
    % Crossover level can't be the top level;
    %%%%%%% Let's Go! %%%%%%%%%%%%%%%
    % Create two operator collections for both parents
    operatorA = cell(1, maxLevel-2);
    operatorB = cell(1, maxLevel-2);
    % Put operator indices into respective collections
    % For every floor except for the first and last ones
    for i = 2:(maxLevel-1)
        % For every element in each floor
        flooropA = [];
        flooropB = [];
        for j = 2^(i-1):2^i-1
            % Make sure the location containing operators
            if ~isempty(parent1{j}) && ~strcmp(parent1{j}, 'x')
                flooropA(end+1) = j;
            end
            if ~isempty(parent2{j}) && ~strcmp(parent2{j}, 'x')
                flooropB(end+1) = j;
            end
        end
        operatorA{i} = flooropA;
        operatorB{i} = flooropB;
    end
    % Randomly selector a level
    targetLevel = randi(maxLevel-2)+1;
    % Randomly pick a location 
    locationA = operatorA{targetLevel}(randi(length(parent1{targetLevel})));
    locationB = operatorB{targetLevel}(randi(length(parent2{targetLevel})));
    % Swap the subheap starting from the selected location
    branchA = findBranch(locationA, maxLevel, targetLevel);
    branchB = findBranch(locationB, maxLevel, targetLevel);
    child1 = parent1;
    child2 = parent2;
    for k = 1:length(branchA)
        temp = child1{branchA(k)};
        child1{branchA(k)} = child2{branchB(k)};
        child2{branchB(k)} = temp;
    end
end