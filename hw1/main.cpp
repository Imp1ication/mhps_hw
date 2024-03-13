#include "flowshop_scheduler.hpp"

#include <iostream>
#include <vector>

int main() {
    FlowshopScheduler scheduler("test_data.txt");
    std::vector<int> order = {0, 2, 1, 3, 4};

    scheduler.PrintData();
    std::cout << std::endl;

    std::cout << "Order: ";
    for (auto i : order) {
        std::cout << i << " ";
    }
    std::cout << std::endl;

    // correct result: 68
    int result = scheduler.CalculateSchedule(order);
    std::cout << "Result: " << result << std::endl;

    return 0;
}
