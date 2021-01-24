/// @file hasher/tabulated_hasher.cpp

#include <hasher/tabulated_hasher.h>
#include <algorithm>
#include <hasher/tabulated_single_bit_hasher.h>

#include "tabulated_single_bit_hasher.h"
#include "I_byte_hasher.h"

namespace hasher
{

struct tabulated_hasher::Impl
{
public:
    tabulated_single_bit_hasher m_default_hasher;
    I_byte_hasher* m_p_hasher;
    I_hash_seeder* m_p_seeder;
    std::vector<uint64_t> m_seeds;
    uint64_t m_current_seed;
    
    Impl()
    : m_default_hasher()
    , m_p_hasher(&m_default_hasher)
    , m_p_seeder(&m_default_hasher)
    , m_seeds()
    {
        m_default_hasher.set_tab_function(index_multiplied_tabulation);
        m_default_hasher.set_seed(0);
    }
    
    virtual ~Impl(){}
};

tabulated_hasher::tabulated_hasher()
    :m_p_impl(new Impl)
{
    set_output_size(1);
}

tabulated_hasher::~tabulated_hasher()
{
    delete m_p_impl;
    m_p_impl = 0;
}

void tabulated_hasher::set_output_size(size_t bytes)
{
    while(m_p_impl->m_seeds.size() < bytes)
    {
        m_p_impl->m_seeds.emplace_back(m_p_impl->m_seeds.size());
    }
    while(m_p_impl->m_seeds.size() > bytes)
    {
        m_p_impl->m_seeds.pop_back();
    }
    set_seed(m_p_impl->m_current_seed);
}

void tabulated_hasher::set_bit_width(size_t bits)
{
    
}

void tabulated_hasher::set_byte_hasher(I_byte_hasher& hasher)
{
    m_p_impl->m_p_hasher = &hasher;
}

void tabulated_hasher::set_seeder(I_hash_seeder& seeder)
{
    m_p_impl->m_p_seeder = &seeder;
}

void tabulated_hasher::set_seed(uint64_t seed)
{
    m_p_impl->m_current_seed = seed;
    uint64_t seed_list [m_p_impl->m_seeds.size()];
    if(m_p_impl->m_p_seeder != 0)
    {
        m_p_impl->m_p_seeder->generate_numbers( seed + sizeof(seed_list)
                                              , reinterpret_cast<uint8_t*>(seed_list)
                                              , sizeof(seed_list));
    }
    for(size_t i = 0; i < m_p_impl->m_seeds.size(); i++)
    {
        if(m_p_impl->m_p_hasher != 0)
        {
            uint64_t new_seed = seed_list[i];
            m_p_impl->m_p_seeder->generate_numbers
                ( seed_list[i]
                , reinterpret_cast<uint8_t*>(&new_seed)
                , sizeof(new_seed));
            m_p_impl->m_seeds[i] = new_seed;
        }
    }
    m_p_impl->m_p_hasher->set_seed(seed);
}

std::vector<uint8_t> tabulated_hasher::perform_hash(std::vector<uint8_t> input)
{
    std::vector<uint8_t> ret_vector;
    ret_vector.resize(m_p_impl->m_seeds.size(), 0);
    uint64_t backup_seed = m_p_impl->m_current_seed;
    if((m_p_impl->m_p_hasher != 0) && (m_p_impl->m_p_seeder != 0))
    {
        std::for_each( input.begin()
                     , input.end()
                     , [this, &ret_vector](uint8_t& current)
        {
            size_t ret_vector_index = 0;
            std::for_each(m_p_impl->m_seeds.begin()
                         ,m_p_impl->m_seeds.end()
                         ,[this, current, &ret_vector_index, &ret_vector](uint64_t& seed)
            {
                m_p_impl->m_p_hasher->set_seed(seed + m_p_impl->m_current_seed);
                uint8_t hashed_value = m_p_impl->m_p_hasher->perform_hash(current);
                ret_vector[ret_vector_index++] 
                    += hashed_value;
            });
            uint64_t new_seed = 0;
            m_p_impl->m_p_seeder->generate_numbers
                ( m_p_impl->m_current_seed
                , reinterpret_cast<uint8_t*>(&new_seed)
                , sizeof(new_seed));
            m_p_impl->m_current_seed = new_seed;
        });
    }
    m_p_impl->m_current_seed = backup_seed;
    return ret_vector;
}

std::vector<uint8_t> tabulated_hasher::perform_hash(uint8_t* p_input, size_t input_size)
{
    std::vector<uint8_t> input_vector;
    input_vector.assign(p_input,p_input + input_size);
    return perform_hash(input_vector);
}

}
