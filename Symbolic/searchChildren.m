function operateLocations = searchChildren(operatPoint, maxLevel)
searchQueue = [operatPoint];
operateLocations = [operatPoint];
while ~isempty(searchQueue)
    currentIndex = searchQueue(1);
    if currentIndex*2+1 <= 2^maxLevel - 1
        operateLocations(end+1) = currentIndex*2;
        operateLocations(end+1) = currentIndex*2+1;
        searchQueue(end+1) = currentIndex*2;
        searchQueue(end+1) = currentIndex*2+1;
    end
    searchQueue(1) = [];
end
end