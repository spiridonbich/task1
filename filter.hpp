#pragma once

#include <stdexcept>
#include <set>
#include <deque>

template <typename ValueType>
class CustomKey
{
public:
    CustomKey(ValueType val) : m_key_value(val), m_amount_keys(1) {};

    bool operator<(const CustomKey& rhs) const
    {
        return (rhs.m_key_value > m_key_value) ? true : false;
    };

    ValueType getKey() const { return m_key_value; };
    size_t getAmount() const { return m_amount_keys; };
    void incrementAmount() { ++m_amount_keys; };
    void decrementAmount() { --m_amount_keys; };

private:
    ValueType           m_key_value{};
    size_t              m_amount_keys{};
};

class Filter
{
public:
    Filter(size_t width);

    // Returns 0 when the amount of elements of the filter after insertion is less than
    // the width of the filter, return maximum of values in the filter otherwise
    size_t push_back(size_t value);

    Filter(const Filter&) = delete;
    Filter& operator=(const Filter&) = delete;

private:
    void add_element(size_t value);
    void remove_element(size_t value);

    // I do not have another choice:
    // both std::set stores unique entities and std::multiset 
    // cannot delete just a single occurance of the entity
    using set_t = std::set<CustomKey<size_t>>;

    size_t              m_width;
    set_t               m_set{};
    std::deque<size_t>  m_deque{};
};

Filter::Filter(size_t width)
: m_width(width)
{
    // erroneous case
    if (0 == m_width)
    {
        throw std::logic_error("Filter has zero length!");
    }
};

void Filter::add_element(size_t value)
{
    // store the value in sorted manner
    auto ins_result = m_set.insert(value);
    // if insert successful - nothing to do
    if (true == ins_result.second)
    {
        return;
    }
    // if insertion failed - lets deal with it
    // this is an element we already have in a set
    auto key_to_inc_amount = *ins_result.first;
    // we change it
    key_to_inc_amount.incrementAmount();
    // we delete old value
    m_set.erase(value);
    // we insert new value
    m_set.insert(key_to_inc_amount);
}

void Filter::remove_element(size_t value)
{
    // finding it in ordered storage
    auto it = m_set.find(value);
    // erroneous behaviour - we should have this item there by definition
    if (m_set.end() == it)
    {
        throw std::logic_error("Missing an element in set which should be there!");
    }
    // this is an element we already have in a set
    auto key_to_dec_amount = *it;
    if (0 == key_to_dec_amount.getAmount())
    {
        throw std::logic_error("Erroneous amount (0) of elements!");
    }
    // we have exactly one element
    if (1 == key_to_dec_amount.getAmount())
    {
        m_set.erase(key_to_dec_amount);
        return;
    }
    // we have several elements like it
    key_to_dec_amount.decrementAmount();
    // we delete old value - this is tricky but should work
    // as soon as the key value is the same
    m_set.erase(value);
    // we insert new value
    m_set.insert(key_to_dec_amount);
}

size_t Filter::push_back(size_t value)
{
    // most simple case
    if (1 == m_width)
    {
        return value;
    }
    // store the value first in deque
    m_deque.push_back(value);
    // if the filter is underflown
    if (m_width > m_deque.size())
    {
        // store it in ordered manner
        add_element(value);
        return 0;
    }
    // filter is full
    if (m_width == m_deque.size())
    {
        // store it in ordered manner
        add_element(value);
        return (*m_set.crbegin()).getKey();
    }
    // now the filter is overflown and we need to deal with it
    // getting value to be removed from the filter
    auto value_to_remove = m_deque.front();
    // removing from deque
    m_deque.pop_front();
    // removing it
    remove_element(value_to_remove);
    // store new one
    add_element(value);
    // and the maximum value is rbegin
    return (*m_set.crbegin()).getKey();
}
