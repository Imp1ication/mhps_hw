#ifndef FLOWSHOP_SCHEDULER_HPP
#define FLOWSHOP_SCHEDULER_HPP

#include <fstream>
#include <iostream>
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

    int CalculateSchedule(const std::vector<int>& permutation) {
        if (permutation.size() != static_cast<size_t>(num_jobs_)) {
            std::cerr
                << "Error: Permutation size does not match number of jobs."
                << std::endl;
            return -1;
        }

        std::vector<int> machine_end_times(num_machines_, 0);

        for (int job : permutation) {
            if (job < 0 || job >= num_jobs_) {
                std::cerr << "Error: Job " << job << " is not valid."
                          << std::endl;
                return -1;
            }

            for (int machine = 0; machine < num_machines_; ++machine) {
                int job_times = processing_times_[machine][job];

                if (machine == 0) {
                    machine_end_times[machine] += job_times;
                } else if (machine_end_times[machine - 1] >
                           machine_end_times[machine]) {
                    machine_end_times[machine] =
                        machine_end_times[machine - 1] + job_times;
                } else {
                    machine_end_times[machine] += job_times;
                }
            }
        }

        return machine_end_times[num_machines_ - 1];
    }
};

#endif // FLOWSHOP_SCHEDULER_HPP
