function [] = hillclimber(operators, constant, data_training, evl, run, maxlevel)
    parfor i = 1:run
        fileID = fopen(strcat('hillclimber_', num2str(i), '.csv'), 'w');
        current_best = inf;
        heap_collection = {};
        error_collection = [];
        %Generate 10 initial heaps
        for k = 1:10
            new_heap = heapgenerator(operators, constant, maxlevel);
            heap_collection{end+1} = new_heap;
            new_error = eror_cal(new_heap, data_training);
            error_collection(end+1) = new_error;
        end
        for j = 1:evl
            %Generate 10 more mutated heaps
            for n = 1:10
                new_heap = mutation(heap_collection{n}, maxlevel);
                heap_collection{end+1} = new_heap;
                new_error = eror_cal(new_heap, data_training);
                error_collection(end+1) = new_error;
            end
            temp = error_collection;
            [~, I] = sort(temp);
            heap_collection = heap_collection{I(1:10)};
            current_best = temp(1);
            fprintf(fileID, '%d %8.4f\n', i, current_best);
        end
    end
end