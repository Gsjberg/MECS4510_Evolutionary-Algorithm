function heap = heapgenerator(operators, constant, maxlevel)
heapSize = 2^maxlevel - 1;
heap = cell([heapSize,1]);
opSize = 2^(maxlevel - 2) - 1;
opQuene = [1];
while ~isempty(opQuene)
    currentIndex = opQuene(1);
    
    heap(currentIndex) = operators(randi(length(operators)));
   
    if currentIndex <= opSize
       if rand < 0.5
          opQuene(end+1) = currentIndex*2; 
       end
       if rand < 0.5
          opQuene(end+1) = currentIndex*2 + 1;
       end
    end  
    opQuene(1) = [];
end
% find existing operator locations
opLocations = find(~cellfun(@isempty, heap));
consLocations = [];
% Collect all the constant locations into array "consLocation"
for i = 1:length(opLocations)
    if isempty(heap{opLocations(i)*2})
        consLocations(end+1) = opLocations(i) * 2;
    end
    if isempty(heap{opLocations(i)*2 + 1})
        consLocations(end+1) = opLocations(i) * 2 + 1;
    end
end
% Assign constants into respective locations
for j = 1:length(consLocations)
    heap(consLocations(j)) = constant(randi(length(constant)));
end
%Assign constant value into "C"
for k = 1:length(heap)
    if strcmp(heap{k}, 'cons')
        min = -10;
        max = 10;
        r = (max - min).*rand(500, 1) + min;
        heap{k} = r(randi(500));
    end
end
end

