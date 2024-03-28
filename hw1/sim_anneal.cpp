#include "flowshop_scheduler.hpp"

#include <filesystem>
#include <iostream>
#include <random>
#include <vector>

int sim_anneal(const std::string data_file_name,
               const std::string log_file_name, double temperature,
               double cool_rate, int epoch_len, int max_iter) {

    FlowshopScheduler scheduler(data_file_name);

    std::ofstream outfile(log_file_name, std::ios::trunc);
    if (!outfile.is_open()) {
        std::cerr << "Error: Unable to open log file." << std::endl;
        exit(EXIT_FAILURE);
    }

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> dis(0.0, 1.0);
    std::uniform_int_distribution<int> dist(0, scheduler.GetNumJobs() - 1);

    std::vector<int> order = scheduler.GenerateRandomOrder();
    std::vector<int> pre_order = order;
    std::vector<int> best_order = order;
    int best_makespan = scheduler.CalculateMakespan(order);

    double initial_temperature = temperature, initial_iter = max_iter;
    scheduler.PrintOrderMakespan(order, "Initial solution");

    outfile << data_file_name << " " << max_iter << std::endl;
    outfile << scheduler.CalculateMakespan(order) << std::endl;

    // calculate sample size
    int sample_size = scheduler.GetNumJobs() * (scheduler.GetNumJobs() - 1) / 2;
    int sample_counter = 0;

    while (max_iter > 0) {
        std::vector<int> next_order = order;
        int next_makespan = scheduler.CalculateMakespan(next_order);

        bool sa_flag = true;
        int i = 0, j = 0, count = 0;

        // Simulated annealing
        while (sa_flag) {
            count = count + 1;
            i = dist(gen);
            j = dist(gen);
            while (i == j) {
                j = dist(gen);
            }

            sample_counter += 1;

            std::vector<int> new_order = order;
            std::swap(new_order[i], new_order[j]);

            int makespan = scheduler.CalculateMakespan(new_order);

            // if the new solution is better, then accept it
            // else, accept it with a probability
            if (makespan <= next_makespan) {
                next_order = new_order;
                next_makespan = makespan;
                if (makespan <= best_makespan) {
                    best_makespan = makespan;
                    best_order = new_order;
                }
                sa_flag = false;
            } else {
                double delta = static_cast<double>(next_makespan - makespan);
                double probability = exp(delta / temperature);
                double randomX = dis(gen);

                // accept worse solution with a probability
                if (randomX < probability) {
                    next_order = new_order;
                    next_makespan = makespan;
                    sa_flag = false;
                }
            }
            // if always can not find acceptable solution,then warm up
            if (count >= (initial_iter / 5)) {
                temperature = initial_temperature;
                count = 0;
            }
            // std::cout << count << std::endl;
        }

        order = next_order;
        // decrease temperature
        if (max_iter % epoch_len == 0) {
            temperature *= cool_rate;
        }

        if (sample_counter >= sample_size) {
            // std::cout << "One Iteration Done!" << std::endl;
            max_iter -= 1;
            sample_counter = 0;
        }
        outfile << scheduler.CalculateMakespan(next_order) << std::endl;
    }

    outfile.close();
    return best_makespan;
}

int main() {
    // sim_anneal("data_set/tai50_20_1.txt", "sa_test.txt", 1000, 0.85, 2, 50);

    int algo_runs = 20;

    for (const auto& entry : std::filesystem::directory_iterator("data_set")) {
        std::string data_file_name = entry.path().string();

        std::string log_file_name =
            "sa_log/log_" + entry.path().filename().string();
        std::string best_log_file_name =
            "sa_log/best_" + entry.path().filename().string();

        std::ofstream outfile(best_log_file_name, std::ios::trunc);
        if (!outfile.is_open()) {
            std::cerr << "Error: Unable to open best log file." << std::endl;
            exit(EXIT_FAILURE);
        }

        int iter_num = algo_runs;
        while (iter_num--) {
            std::cout << "--- " << log_file_name << " "
                      << "iter: " << iter_num << " ---" << std::endl;

            int search_result =
                sim_anneal(data_file_name, log_file_name, 1000, 0.85, 2, 500);
            outfile << search_result << std::endl;

            std::cout << "Best makespan: " << search_result << std::endl;
            std::cout << std::endl;
        }

        outfile.close();
    }

    return 0;
}
