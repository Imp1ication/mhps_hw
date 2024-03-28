#include "flowshop_scheduler.hpp"

#include <filesystem>
#include <iostream>
#include <vector>

using namespace std;
int tabu_search(const std::string data_file_name,
                const std::string log_file_name, int max_iter) {
    FlowshopScheduler scheduler(data_file_name);

    std::ofstream outfile(log_file_name, std::ios::trunc);
    if (!outfile.is_open()) {
        std::cerr << "Error: Unable to open log file." << std::endl;
        exit(EXIT_FAILURE);
    }

    std::vector<int> order = scheduler.GenerateRandomOrder();
    int spend_time = scheduler.CalculateMakespan(order);

    std::vector<int> best_order = order;

    outfile << data_file_name << " " << max_iter << std::endl;
    outfile << scheduler.CalculateMakespan(order) << std::endl;

    // set tabu tenure
    int tabu_tenure = 10;
    int no_update_limit = 50;
    std::vector<std::vector<int>> tabu_list(tabu_tenure, {-1, -1});

    int no_update_count = 0;
    int step_count = 0; // to determine update which value in tabu list
    int tabu_count = 0; // if the best 5 moves are tabu, stop the algorithm
    int best_time = 0;  // store the best time in while loop
    int test_time = 0;
    int best_swap_i = -1; // save the best swap
    int best_swap_j = -1;
    int tabu_flag = 0;

    std::vector<int> hist_best_order = order;
    int hist_best_makespan = scheduler.CalculateMakespan(hist_best_order);

    while (max_iter--) {
        best_time = spend_time * 2;

        // search for all swap pairs
        for (int i = 0; i < scheduler.GetNumJobs() - 1; i++) {
            for (int j = i + 1; j < scheduler.GetNumJobs(); j++) {
                std::swap(order[i], order[j]);
                test_time = scheduler.CalculateMakespan(order);
                if ((test_time < best_time)) {
                    // check whether the target swap is in the tabu list
                    for (int k = 0; k < tabu_tenure; k++) {
                        if (tabu_list[k][0] == std::min(order[i], order[j]) &&
                            tabu_list[k][1] == std::max(order[i], order[j])) {
                            tabu_flag = 1;
                            break;
                        }
                    }

                    if (tabu_flag == 1) {
                        tabu_count += 1;
                    } else {
                        best_swap_i = i;
                        best_swap_j = j;
                        best_time = test_time;
                        // if any swap not in the tabu list is better than any
                        // swap in it, bypass the tabu count check
                        tabu_count = 0;
                    }
                    tabu_flag = 0;
                }
                std::swap(order[i], order[j]);
            }
        }

        std::swap(order[best_swap_i], order[best_swap_j]);

        if (best_time < spend_time) {
            no_update_count = 0;
            for (int i = 0; i < scheduler.GetNumJobs(); i++) {
                best_order[i] = order[i];
            }
            spend_time = best_time;
        } else {
            spend_time = best_time;
            no_update_count += 1;
        }

        outfile << best_time << std::endl;

        if (best_time < hist_best_makespan) {
            hist_best_order = order;
            hist_best_makespan = best_time;
        }

        tabu_list[step_count % tabu_tenure][0] =
            std::min(order[best_swap_i], order[best_swap_j]);
        tabu_list[step_count % tabu_tenure][1] =
            std::max(order[best_swap_i], order[best_swap_j]);
        step_count += 1;

        // restart tabu search
        if (no_update_count >= no_update_limit || tabu_count >= tabu_tenure) {
            if (no_update_count >= no_update_limit) {
                std::cout << "No update" << std::endl;
            }

            if (tabu_count >= tabu_tenure) {
                std::cout << "Tabu tenure" << std::endl;
            }

            no_update_count = 0;
            tabu_count = 0;
            order = scheduler.GenerateRandomOrder();
            spend_time = scheduler.CalculateMakespan(order);
        }
    }

    outfile.close();
    return hist_best_makespan;
}

int main() {
    // tabu_search("tai100_20_1.txt", "tabu_test.txt", 500);

    int iter_threshold = 20;

    for (const auto& entry : std::filesystem::directory_iterator("data_set"))
    {
        std::string data_file_name = entry.path().string();

        std::string log_file_name =
            "ts_log/log_" + entry.path().filename().string();
        std::string best_log_file_name =
            "ts_log/best_" + entry.path().filename().string();

        std::ofstream outfile(best_log_file_name, std::ios::trunc);
        if (!outfile.is_open()) {
            std::cerr << "Error: Unable to open best log file." << std::endl;
            exit(EXIT_FAILURE);
        }

        int iter_num = iter_threshold;
        while (iter_num--) {
            std::cout << "--- " << log_file_name << " "
                      << "iter: " << iter_num << " ---" << std::endl;

            int search_result = tabu_search(data_file_name, log_file_name,
            500); outfile << search_result << std::endl;

            std::cout << "Best makespan: " << search_result << std::endl;
            std::cout << std::endl;
        }

        outfile.close();
    }

    return 0;
}
