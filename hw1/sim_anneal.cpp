#include "flowshop_scheduler.hpp"

#include <iostream>
#include <random>
#include <vector>
// #include <time.h>
// #include <limits.h>

void sim_anneal(const std::string data_file_name, double temperature,
                double cool_rate, int epoch_len, int max_iter) {

    FlowshopScheduler scheduler(data_file_name);

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> dis(0.0, 1.0);

    std::vector<int> order = scheduler.GenerateRandomOrder();
    scheduler.PrintOrderMakespan(order, "Initial solution");

    while (max_iter--) {
        std::vector<int> next_order = order;
        int next_makespan = scheduler.CalculateMakespan(next_order);

        // Simulated annealing
        for (int i = 0; i < scheduler.GetNumJobs() - 1; ++i) {
            for (int j = i + 1; j < scheduler.GetNumJobs(); ++j) {
                std::vector<int> new_order = order;
                std::swap(new_order[i], new_order[j]);

                int makespan = scheduler.CalculateMakespan(new_order);

                // if the new solution is better, then accept it
                // else, accept it with a probability
                if (makespan < next_makespan) {
                    next_order = new_order;
                    next_makespan = makespan;
                } else {
                    double delta =
                        static_cast<double>(next_makespan - makespan);
                    double probability = exp(delta / temperature);
                    double randomX = dis(gen);

                    // accept worse solution with a probability
                    if (randomX < probability) {
                        next_order = new_order;
                        next_makespan = makespan;
                    }
                }
            }
        }

        order = next_order;
        std::cout << "Makespan: " << next_makespan << std::endl;

        // decrease temperature
        if (max_iter % epoch_len == 0) {
            temperature *= cool_rate;
        }
    }

    scheduler.PrintOrderMakespan(order, "Best solution");
}

int main() {
    sim_anneal("test_data.txt", 1000, 0.85, 2, 50);
    return 0;
}
