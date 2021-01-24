/// @file hasher/tabulated_single_bit_hasher.cpp

#include <hasher/tabulated_single_bit_hasher.h>
#include <string.h>

#include "tabulated_single_bit_hasher.h"

namespace hasher
{
uint8_t standard_hash_tabulation(size_t index, uint8_t bit, uint8_t tab_value)
{
    uint8_t result = 0;
    if(bit == 1)
    {
        result = tab_value;
    }
    return result;
}

uint8_t index_multiplied_tabulation(size_t index, uint8_t bit, uint8_t tab_value)
{
    uint8_t result = 0;
    if(bit == 1)
    {
        result = tab_value + index + 1;
    }
    else
    {
        result = -tab_value - index;
    }
    result *= result;
    return result;
}
    
struct tabulated_single_bit_hasher::Impl
{
public:
    Impl()
    :m_tab_func(standard_hash_tabulation)
    ,m_initial(0)
    ,m_tab_value()
    {
        memset(m_tab_value,0,sizeof(m_tab_value));
    }
    
    virtual ~Impl(){}
    
    tab_func m_tab_func;
    uint8_t m_initial;
    uint8_t m_tab_value[8];
};

tabulated_single_bit_hasher::tabulated_single_bit_hasher()
    :m_p_impl(new Impl)
{
}

tabulated_single_bit_hasher::~tabulated_single_bit_hasher()
{
    delete m_p_impl;
    m_p_impl = 0;
}

void tabulated_single_bit_hasher::set_hash_table_as_seed(uint64_t seed)
{
    memcpy(m_p_impl->m_tab_value,&seed,sizeof(seed));
}

uint64_t tabulated_single_bit_hasher::get_hash_table_as_seed()
{
    uint64_t hash_as_seed = 0;
    memcpy(&hash_as_seed,m_p_impl->m_tab_value,sizeof(hash_as_seed));
    return hash_as_seed;
}

void tabulated_single_bit_hasher::set_tab_function(tab_func custom_tab_function)
{
    m_p_impl->m_tab_func = custom_tab_function;
}

void tabulated_single_bit_hasher::set_initial(uint8_t initial)
{
    m_p_impl->m_initial = initial;
}

void tabulated_single_bit_hasher::set_hash_table_value(size_t index, uint8_t value)
{
    m_p_impl->m_tab_value[index] = value;
}

uint8_t tabulated_single_bit_hasher::perform_hash(uint8_t input)
{
    uint8_t result = m_p_impl->m_initial;
    uint8_t copy_input = input;
    for(size_t i = 0; i < sizeof(m_p_impl->m_tab_value); i++)
    {
        result += m_p_impl->m_tab_func(i, (copy_input & 0x1), m_p_impl->m_tab_value[i]);
        copy_input = copy_input >> 1;
    }
    return result;
}

void tabulated_single_bit_hasher::generate_numbers(uint64_t seed, uint8_t* p_array, size_t array_size)
{
    uint64_t old_seed = get_hash_table_as_seed();
    set_hash_table_as_seed(seed);
    uint8_t new_initial = 0;
    for(size_t i = 0; i < sizeof(seed); i++)
    {
        new_initial += m_p_impl->m_tab_value[i];
    }
    uint8_t old_initial = m_p_impl->m_initial;
    m_p_impl->m_initial = new_initial;

    uint8_t current = new_initial;
    
    for(size_t i = 0; i < array_size; i++)
    {
        current = perform_hash(current);
        hash_rotate();
        m_p_impl->m_tab_value[0] = current;
        p_array[i] = current;
    }
    
    m_p_impl->m_initial = old_initial;
    set_hash_table_as_seed(old_seed);
}

void tabulated_single_bit_hasher::hash_rotate()
{
    uint8_t new_smallest = perform_hash(m_p_impl->m_initial);
    uint8_t new_array[sizeof(m_p_impl->m_tab_value)];
    for(size_t i = 0; i < sizeof(new_array); i++)
    {
        new_array[(i + 1)%(sizeof(new_array))] = m_p_impl->m_tab_value[i];
    }
    new_array[0] = new_smallest;
    memcpy(m_p_impl->m_tab_value, new_array, sizeof(new_array));
}

void tabulated_single_bit_hasher::set_seed(uint64_t seed)
{
    set_hash_table_as_seed(seed);
    uint8_t* p_seed = reinterpret_cast<uint8_t*>(&seed);
    m_p_impl->m_initial = 0;
    for(size_t i = 0; i < sizeof(seed); i++)
    {
        m_p_impl->m_initial += p_seed[i] + i;
        m_p_impl->m_initial = m_p_impl->m_initial * m_p_impl->m_initial;
    }
    m_p_impl->m_initial = perform_hash(m_p_impl->m_initial);
    uint8_t count = 1;//m_p_impl->m_initial;
    do
    {
        uint8_t new_array [sizeof(m_p_impl->m_tab_value)];
        generate_numbers(get_hash_table_as_seed(), new_array, sizeof(new_array));
        memcpy(m_p_impl->m_tab_value, new_array, sizeof(new_array));
        --count;
    }
    while(count > 0);
}

}
