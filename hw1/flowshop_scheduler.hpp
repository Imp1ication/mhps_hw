#ifndef FLOWSHOP_SCHEDULER_HPP
#define FLOWSHOP_SCHEDULER_HPP

#include <algorithm>
#include <fstream>
#include <iostream>
#include <random>
#include <string>
#include <vector>

class FlowshopScheduler {
  private:
    int num_jobs_;
    int num_machines_;
    std::string data_name_;
    int** processing_times_;

    void ReadDataFromFile(const std::string& file_name) {
        std::ifstream file(file_name);
        if (!file.is_open()) {
            std::cerr << "Error: Unable to open file " << file_name
                      << std::endl;
            exit(EXIT_FAILURE);
        }

        file >> num_jobs_ >> num_machines_ >> data_name_;

        // Allocate memory and read processing times
        processing_times_ = new int*[num_machines_];
        for (int machine = 0; machine < num_machines_; ++machine) {
            processing_times_[machine] = new int[num_jobs_];
            for (int job = 0; job < num_jobs_; ++job) {
                file >> processing_times_[machine][job];
            }
        }

        file.close();
    }

  public:
    FlowshopScheduler(const std::string& file_name) {
        ReadDataFromFile(file_name);
    }

    ~FlowshopScheduler() {
        if (!processing_times_)
            return;

        for (int i = 0; i < num_machines_; ++i) {
            delete[] processing_times_[i];
        }

        delete[] processing_times_;
    }

    void PrintData() {
        std::cout << "Data name: " << data_name_ << std::endl;
        std::cout << "Number of jobs: " << num_jobs_ << std::endl;
        std::cout << "Number of machines: " << num_machines_ << std::endl;

        std::cout << std::endl;

        std::cout << "Processing times:" << std::endl;
        for (int machine = 0; machine < num_machines_; ++machine) {
            for (int job = 0; job < num_jobs_; ++job) {
                std::cout << processing_times_[machine][job] << " ";
            }
            std::cout << std::endl;
        }
    }
    void PrintOrderMakespan(const std::vector<int>& order,
                            const std::string title = "Order") {
        std::cout << title << ": ";
        for (auto i : order) {
            std::cout << i << " ";
        }
        std::cout << std::endl;

        std::cout << "Makespan: " << CalculateMakespan(order) << std::endl;
    }

    std::vector<int> GenerateRandomOrder() {
        std::random_device rd;
        std::mt19937 gen(rd());

        std::vector<int> order(num_jobs_);
        for (int i = 0; i < num_jobs_; ++i) {
            order[i] = i;
        }
        shuffle(order.begin(), order.end(), gen);

        return order;
    }

    int GetNumJobs() const { return num_jobs_; }
    int GetNumMachines() const { return num_machines_; }

    int CalculateMakespan(const std::vector<int>& permutation) {
        std::vector<int> machine_end_times(num_machines_, 0);

        for (int job : permutation) {
            // Check if job is valid
            if (job < 0 || job >= num_jobs_) {
                std::cerr << "Error: Job " << job << " is not valid."
                          << std::endl;
                return -1;
            }

            // Calculate each machine's end time for the current job.
            for (int machine = 0; machine < num_machines_; ++machine) {
                int job_times = processing_times_[machine][job];

                if (machine == 0) {
                    machine_end_times[machine] += job_times;
                } else {
                    machine_end_times[machine] =
                        std::max(machine_end_times[machine - 1],
                                 machine_end_times[machine]) +
                        job_times;
                }
            }
        }

        return machine_end_times[num_machines_ - 1];
    }
};

#endif // FLOWSHOP_SCHEDULER_HPP
