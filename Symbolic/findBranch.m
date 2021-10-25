function branches = findBranch(location, maxlevel, targetLevel)
    branches = location;
    for i = 1:(maxlevel-targetLevel)
        temp = (location*(2^i):1:location*(2^i)+1);
        branches = cat(2, branches, temp);
    end
end