#pragma once

#include <string>
#include <vector>
#include <fstream>
#include <stdexcept>

class InputReader
{
public:
    InputReader(const std::string& file_name);
    size_t getFilterWidth() const { return m_filter_width; };
    std::vector<int> getValues();

    InputReader(const InputReader&) = delete;
    InputReader& operator=(const InputReader&) = delete;

private:
    std::ifstream m_input_file;
    size_t m_values_amount;
    size_t m_filter_width;
};

InputReader::InputReader(const std::string& file_name)
: m_input_file(file_name)
, m_values_amount(0)
, m_filter_width(0)
{
    m_input_file >> m_values_amount >> m_filter_width;
    if ((m_values_amount == 0) || (m_filter_width == 0))
    {
        throw std::logic_error("Amount of values or filter width is 0!");
    }
};

std::vector<int> InputReader::getValues()
{
    std::vector<int> result;
    result.reserve(m_values_amount);
    for (size_t idx = 0; idx < m_values_amount; ++idx)
    {
        auto ivalue = 0;
        m_input_file >> ivalue;
        result.push_back(ivalue);
    }
    return result;
};
