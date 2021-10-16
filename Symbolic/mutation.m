function heapMu = mutation(heap, maxlevel)
    temp = heap;
    constRow = temp(2^(maxlevel-1):end);
    mutationIndex = randi(2^maxlevel);
    % Ensure the selected node is not empty
    while isempty(constRow(mutationIndex))
        mutationIndex = randi(2^maxlevel);
    end
    if rand > 0.5
        temp(mutationIndex) = abs(temp(mutationIndex) - 0.1);
    else
        temp(mutationIndex) = strcat('(0.01.*x+', num2str(temp(mutationIndex)),')');
    end
    heapMu = temp;
end