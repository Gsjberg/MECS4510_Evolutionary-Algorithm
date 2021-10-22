function [] = hillclimber(operators, constant, data_training, evl, run, maxlevel)
    parfor i = 1:run
        fileID = fopen(strcat('hillclimber_', num2str(i), '.txt'), 'w');
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
                new_heap = mutation(heap_collection{n});
                new_error = eror_cal(new_heap, data_training);
                while ismember(new_error, error_collection)
                    new_heap = mutation(new_heap);
                    new_error = eror_cal(new_heap, data_training);
                end
                heap_collection{end+1} = new_heap;
                error_collection(end+1) = new_error;
            end
            temp = error_collection;
            [~, I] = sort(temp);
            heap_collection = heap_collection(I(1:10));
            error_collection = error_collection(I(1:10));
            current_best = error_collection(1);
            %fprintf('%d %8.4f\n', j, current_best);
            fprintf(fileID, '%d %8.4f\n', j, current_best);
        end
        fclose(fileID);
    end
end