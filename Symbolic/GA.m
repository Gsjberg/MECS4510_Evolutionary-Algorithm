function [] = GA(operators, constant, data_training, evl, run, maxlevel)
    populationsize = 20;
    parfor i = 1:run
        fileID = fopen(strcat('hillclimber_', num2str(i), '.txt'), 'w');
        heap_collection = {};
        error_collection = [];
        %Generate 20 initial heaps, the population size can vary by
        %changing the variable above
        %The population size has to be able to be divided by 4
        for k = 1:populationsize 
            new_heap = heapgenerator(operators, constant, maxlevel);
            heap_collection{end+1} = new_heap;
            new_error = eror_cal(new_heap, data_training);
            error_collection(end+1) = new_error;
        end

        for j = 1:evl
            %Generate 20 more crossovered heaps, the population size can vary
            %Randomly select 10 groups of parents
            for n = 1:populationsize/2
                %We get 2 more memebers every round
                parentsID = randperm(populationsize, 2);
                parent1  = heap_collection{parentsID(1)};
                parent2  = heap_collection{parentsID(2)};
                [child1, child2] = crossover(parent1, parent2, maxlevel);
                %Mutate the newly generated members
                child1 = mutation(child1);
                child2 = mutation(child2);
                %Put the newly generated offsprings to the collection pool
                heap_collection{end+1} = child1;
                heap_collection{end+1} = child2;
                %Put their respective errors into the collection pool
                new_error = eror_cal(child1, data_training);
                error_collection(end+1) = new_error;
                new_error = eror_cal(child2, data_training);
                error_collection(end+1) = new_error;
            end
            temp = error_collection;
            [~, I] = sort(temp);
            heap_collection = heap_collection(I(1:populationsize));
            error_collection = error_collection(I(1:populationsize));
            %It doesn't mmatter if we compare current best and the
            %universal best
            current_best = error_collection(1);
            %fprintf('%d %8.4f\n', j, current_best);
            fprintf(fileID, '%d %8.4f\n', j, current_best);
        end
    end
end