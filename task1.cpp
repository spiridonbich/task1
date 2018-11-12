#include "filter.hpp"
#include "io.hpp"

#include <iostream>
#include <stdexcept>
#include <chrono>

int main(int argc, char** argv)
{
    std::string ifile;
    if (argc != 2)
    {
        std::cout << "Please specify filename for input: ";
        std::cin >> ifile;
    }
    else
    {
        ifile = argv[1];
    }

    InputReader inp_reader(ifile);
    auto filter_width = inp_reader.getFilterWidth();
    auto values = inp_reader.getValues();
    auto values_amount = values.size();
    if (values_amount < filter_width)
    {
        throw std::logic_error("Not enough values for filter creation!");
    }

    Filter F(filter_width);
    std::vector<size_t> result_vals;
    result_vals.reserve(values_amount - filter_width);

    std::chrono::high_resolution_clock::time_point start_time = std::chrono::high_resolution_clock::now();
    for (size_t idx = 0; idx < filter_width - 1; ++idx)
    {
        F.push_back(values[idx]);
    }
    for (size_t idx = filter_width - 1; idx < values_amount; ++idx)
    {
        result_vals.push_back(F.push_back(values[idx]));
    }
    std::chrono::high_resolution_clock::time_point end_time = std::chrono::high_resolution_clock::now();

    for (auto a : result_vals)
    {
        std::cout << a << " ";
    }

    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time).count();
    std::ofstream dur("duration.out");
    dur << duration << " microsec" << std::endl;

    return 0;
}