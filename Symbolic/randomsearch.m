function [] = randomsearch(operators, constant, data_training, evl, run, maxlevel)
    parfor i = 1:run
        fileID = fopen(strcat('randomsearch_', num2str(i), '.txt'), 'w');
        current_best = inf;
        for j = 1:evl
            heap = heapgenerator(operators, constant, maxlevel);
            error = eror_cal(heap, data_training);
            if error < current_best
                current_best = error;
            end
            %fprintf('%d %8.4f\n', j, current_best);
            fprintf(fileID, '%d %8.4f %8.4f\n', j, error, current_best);
        end
        fclose(fileID);
    end
end