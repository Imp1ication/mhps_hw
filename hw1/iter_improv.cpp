#include "flowshop_scheduler.hpp"

#include <algorithm>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <vector>

int iter_improv(const std::string data_file_name,
                const std::string log_file_name, int max_iter) {
    FlowshopScheduler scheduler(data_file_name);

    std::ofstream outfile(log_file_name, std::ios::trunc);
    if (!outfile.is_open()) {
        std::cerr << "Error: Unable to open log file." << std::endl;
        exit(EXIT_FAILURE);
    }

    // generate initial solution randomly
    std::vector<int> order = scheduler.GenerateRandomOrder();

    scheduler.PrintOrderMakespan(order, "Initial solution");
    std::cout << std::endl;

    outfile << data_file_name << " " << max_iter << std::endl;
    outfile << scheduler.CalculateMakespan(order) << std::endl;

    int restart_counter = 0;
    int restart_threshold = 10;
    // Iterative improvement

    std::vector<int> hist_best_order = order;
    int hist_best_makespan = scheduler.CalculateMakespan(hist_best_order);

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

                if (makespan <= hist_best_makespan) {
                    hist_best_order = new_order;
                    hist_best_makespan = makespan;
                }
            }
        }

        if (best_order == order) {
            restart_counter = restart_threshold;
        } else if (best_makespan == scheduler.CalculateMakespan(order)) {
            restart_counter += 1;
        } else {
            order = best_order;
        }

        if (restart_counter == restart_threshold) {
            order = scheduler.GenerateRandomOrder();
            restart_counter = 0;
        }

        // std::cout << "Makespan: " << best_makespan << std::endl;
        outfile << best_makespan << std::endl;
    }

    // scheduler.PrintOrderMakespan(hist_best_order, "Best solution");
    outfile.close();

    return hist_best_makespan;
}

int main() {
    int algo_runs = 20;

    for (const auto& entry : std::filesystem::directory_iterator("data_set")) {
        std::string data_file_name = entry.path().string();

        std::string log_file_name =
            "ii_log/log_" + entry.path().filename().string();
        std::string best_log_file_name =
            "ii_log/best_" + entry.path().filename().string();

        std::ofstream outfile(best_log_file_name, std::ios::trunc);
        if (!outfile.is_open()) {
            std::cerr << "Error: Unable to open best log file." << std::endl;
            exit(EXIT_FAILURE);
        }

        int iter_num = algo_runs;
        while (iter_num--) {
            std::cout << "--- " << log_file_name << " "
                      << "iter: " << iter_num << " ---" << std::endl;

            int search_result = iter_improv(data_file_name, log_file_name, 500);
            outfile << search_result << std::endl;

            std::cout << "Best makespan: " << search_result << std::endl;
            std::cout << std::endl;
        }

        outfile.close();
    }

    return 0;
}
