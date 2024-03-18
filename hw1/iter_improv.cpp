#include "flowshop_scheduler.hpp"

#include <algorithm>
#include <iostream>
#include <vector>

void iter_improv(const std::string data_file_name, int max_iter) {
    FlowshopScheduler scheduler(data_file_name);

    // generate initial solution randomly
    std::vector<int> order = scheduler.GenerateRandomOrder();

    scheduler.PrintOrderMakespan(order, "Initial solution");
    std::cout << std::endl;

    // Iterative improvement
    while (max_iter--) {
        std::vector<int> best_order = order;
        int best_makespan = scheduler.CalculateMakespan(best_order);

        // swap two elements and find the best solution
        for (int i = 0; i < scheduler.GetNumJobs() - 1; ++i) {
            for (int j = i + 1; j < scheduler.GetNumJobs(); ++j) {
                std::vector<int> new_order = order;
                std::swap(new_order[i], new_order[j]);

                int makespan = scheduler.CalculateMakespan(new_order);
                if (makespan <= best_makespan) {
                    best_order = new_order;
                    best_makespan = makespan;
                }
            }
        }

        // if no better solution, then break
        if (best_order == order) {
            break;
        } else {
            order = best_order;
        }

        std::cout << "Makespan: " << best_makespan << std::endl;
    }

    scheduler.PrintOrderMakespan(order, "Best solution");
}

int main() {
    iter_improv("test_data.txt", 50);
    return 0;
}
