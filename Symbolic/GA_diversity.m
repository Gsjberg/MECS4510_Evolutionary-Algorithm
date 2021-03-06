% Size of some important containers
% Dotplot: N * populationsize
% Validation Error: N*1

function [] = GA_diversity(operators, constant, data_training, data_validation, evl, run, maxlevel)
    % Initialization of some parameters and containers
    populationsize = 40;
    %parfor i = 1:run
        fileID = fopen(strcat('GA_diversity_', num2str(5), '.txt'), 'w');
        heap_collection = cell(1, populationsize);
        error_collection = ones(1, populationsize);
        error_validation = ones(1, populationsize);
        Dotplot = ones(evl, populationsize);
        %Generate 20 initial heaps, the population size can vary by
        %changing the variable above
        %The population size has to be able to be divided by 4
        for k = 1:populationsize 
            new_heap = heapgenerator(operators, constant, maxlevel);
            heap_collection{k} = new_heap;
            new_error = eror_cal(new_heap, data_training);
            error_collection(k) = new_error;
        end

        % WE don't need a whole new set of new members
        % Crossover and replace their parent
        for j = 1:evl
            for n = 1:populationsize/2
                %We get 2 more memebers every round
                parentsID = randperm(populationsize, 2);
                parent1  = heap_collection{parentsID(1)};
                parent2  = heap_collection{parentsID(2)};
                [child1, child2] = crossOver(parent1, parent2, maxlevel);
                %Mutate the newly generated members
                child1 = mutation(child1);
                child2 = mutation(child2);
                %We are gonna see if the two children are similar to their
                %parents (adding diversity)
                p1c1 = similarity(parent1, child1, data_training);
                p2c2 = similarity(parent2, child2, data_training);
                p1c2 = similarity(parent1, child2, data_training);
                p2c1 = similarity(parent2, child1, data_training);
                if p1c1+p2c2 < p1c2 + p2c1
                    if eror_cal(child1, data_training) < eror_cal(parent1, data_training)
                        heap_collection{parentsID(1)} = child1;
                    end
                    if eror_cal(child2, data_training) < eror_cal(parent2, data_training)
                        heap_collection{parentsID(2)} = child2;
                    end
                else
                    if eror_cal(child1, data_training) < eror_cal(parent2, data_training)
                        heap_collection{parentsID(2)} = child1;
                    end
                    if eror_cal(child2, data_training) < eror_cal(parent1, data_training)
                        heap_collection{parentsID(1)} = child2;
                    end
                end
                % Now we have a new collection of heaps of population size
                % Let's calculate errors!
                for m = 1:populationsize
                    error_collection(m) = eror_cal(heap_collection{m}, data_training);
                    error_validation(m) = eror_cal(heap_collection{m}, data_validation);
                end
            end
            Dotplot(j,:) = error_collection;
            [best_error, P] = min(error_collection);
            %fprintf('%d %8.4f\n', j, best_error);
            fprintf(fileID, '%d %8.4f %8.4f \n', j, best_error, min(error_validation));
        end
        equation_best = heap_collection{P};
        for gsj = 1:length(equation_best)
            fprintf(fileID, '%s ', num2str(equation_best{gsj}));
        end
        fclose(fileID);
        writematrix(Dotplot);
    %end
end