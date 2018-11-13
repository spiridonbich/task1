#pragma once

#include <stdexcept>
#include <vector>

class CyclIndex
{
public:
    CyclIndex(size_t value, size_t range)
    {
        if (0 == range)
        {
            throw std::logic_error("Range is zero!");
        }
        m_range = range;
        m_value = value % m_range;
    };
    void inc() { m_value = (m_value + 1) % m_range; };
    void dec() { m_value = (m_value - 1 < 0) ? m_range - 1 : m_value - 1; };
    size_t previous() const { return (m_value - 1 < 0) ? m_range - 1 : m_value - 1; };
    operator unsigned int() { return m_value; };

    CyclIndex() = delete;

private:
    size_t m_range;
    size_t m_value;
};

class CustDeque
{
public:
    typedef size_t ValueType;
    CustDeque(size_t max_amount) : m_max_amount(max_amount), m_size(0), m_begin(0, max_amount), m_end(0, max_amount), m_values(max_amount, 0) {};
    void push_back(ValueType value)
    {
        if (m_size >= m_max_amount)
        {
            throw std::logic_error("Deque is already full!");
        }
        m_values[m_end] = value; 
        m_end.inc();
        ++m_size;
    };
    void pop_back() { m_end.dec(); --m_size; };
    void pop_front() { m_begin.inc(); --m_size; };
    bool empty() { return (m_size == 0) ? true : false; };
    ValueType back() { return m_values[m_end.previous()]; };
    ValueType front() { return m_values[m_begin]; };
    size_t size() const { return m_size; };

    CustDeque() = delete;
    CustDeque& operator=(const CustDeque&) = delete;

private:
    size_t m_max_amount;
    size_t m_size;
    CyclIndex m_begin;
    CyclIndex m_end;
    std::vector<ValueType> m_values{};
};

class FilterEnhanced
{
public:
    typedef size_t ValueType;
    FilterEnhanced(size_t width);
    void add_max_candidate(ValueType value);
    void remove_max_candidate(ValueType value);
    ValueType getMax() { return m_deque.front(); };

    FilterEnhanced(const FilterEnhanced&) = delete;
    FilterEnhanced& operator=(const FilterEnhanced&) = delete;

private:
    size_t m_width;
    CustDeque m_deque;
};

FilterEnhanced::FilterEnhanced(size_t width) : m_width(width), m_deque(width) {};

void FilterEnhanced::add_max_candidate(ValueType value)
{
    while ( !m_deque.empty() && m_deque.back() < value )
    {
        m_deque.pop_back();
    }
    m_deque.push_back(value);
}

void FilterEnhanced::remove_max_candidate(ValueType value)
{
    if ( !m_deque.empty() && m_deque.front() == value)
    {
        m_deque.pop_front();
    }
}
