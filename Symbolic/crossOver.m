function [offSpring1, offSpring2] = crossOver(parent1, parent2, maxLevel)
maxCrossOverLimit = min(max(find(~cellfun(@isempty,parent1))),max(find(~cellfun(@isempty,parent2))));
offSpring1 = parent1;
offSpring2 = parent2;
maxCrossOverLevel = floor(log2(maxCrossOverLimit))+1;
while 1
crossOverLevel = randi(maxCrossOverLevel);
selection = 2^(crossOverLevel-1):2^(crossOverLevel)-1;
crossOverPoint1 = selection(randi(length(selection)));
crossOverPoint2 = selection(randi(length(selection)));
if ~isempty(parent1{crossOverPoint1}) && ~isempty(parent2{crossOverPoint2})
    if isnumeric(parent1{crossOverPoint1}) && isnumeric(parent2{crossOverPoint2})
        break
    elseif strcmp(parent1{crossOverPoint1},'x') && strcmp(parent2{crossOverPoint2},'x')
        break
    elseif isnumeric(parent1{crossOverPoint1}) && strcmp(parent2{crossOverPoint2},'x')
        break
    elseif isnumeric(parent2{crossOverPoint2}) && strcmp(parent1{crossOverPoint1},'x')
        break
    elseif ischar(parent1{crossOverPoint1}) && ischar(parent2{crossOverPoint2})
        break
    end
end
end
crossOverLocations1 = searchChildren(crossOverPoint1, maxLevel);
crossOverLocations2 = searchChildren(crossOverPoint2, maxLevel);
subHeap1 = parent1(crossOverLocations1);
subHeap2 = parent2(crossOverLocations2);
for i = 1:length(crossOverLocations1)
    offSpring1(crossOverLocations1(i)) = subHeap2(i);
    offSpring2(crossOverLocations2(i)) = subHeap1(i);
end
end