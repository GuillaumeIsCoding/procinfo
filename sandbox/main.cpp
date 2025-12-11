#include <include/enumerate_process.hpp>

#include <iostream>

#define SEPARATOR "==============================================="

int main(int argc, char** argv)
{
    std::cout << "Active process" << std::endl;

    std::cout << SEPARATOR << std::endl;

    auto processes = enumerate_process();

    int counter = 0;

    for (const auto& [pid, process] : processes)
    {
        counter++;

        std::cout << "PID " << pid << " :\n"
                  << process << SEPARATOR << std::endl;
    }

    std::cout << "Total of active process : " << counter << std::endl;

    return 0;
}