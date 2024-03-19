#include "flowshop_scheduler.hpp"

#include <iostream>
#include <vector>

using namespace std;

int main() {
    FlowshopScheduler scheduler("test_data.txt");
    std::vector<int> order = scheduler.GenerateRandomOrder();

    std::cout << "---PrintData---" << std::endl;
    scheduler.PrintData();
    std::cout << std::endl;

    std::cout << "---PrintOrderMakespan---" << std::endl;
    scheduler.PrintOrderMakespan(order, "Your order name");
    std::cout << std::endl;

    std::cout << "------" << std::endl;
    std::cout << "Jobs: " << scheduler.GetNumJobs() << std::endl;
    std::cout << "Machines: " << scheduler.GetNumMachines() << std::endl;
    std::cout << "CalculateMakespan: " << scheduler.CalculateMakespan(order)
              << std::endl;

    // correct result: 68
    int result = scheduler.CalculateMakespan(order);
    std::cout << "Result: " << result << std::endl;

    return 0;
}
