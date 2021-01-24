/// @file hasher/tabulated_double_bit_block_hasher.cpp

#include <hasher/tabulated_double_bit_block_hasher.h>
#include <string.h>

namespace hasher
{

struct tabulated_double_bit_block_hasher::Impl
{
public:
    uint64_t m_seed;
    uint8_t m_initial;
    uint8_t m_hash_table[8][2*2];
    I_hash_seeder* m_p_seeder;

    Impl()
        : m_seed(0)
        , m_initial(0)
        , m_hash_table()
        , m_p_seeder(0)
    {
        memset(m_hash_table, 0, sizeof(m_hash_table));
    }
    virtual ~Impl(){}    
};

tabulated_double_bit_block_hasher::tabulated_double_bit_block_hasher()
    :m_p_impl(new Impl)
{
}

tabulated_double_bit_block_hasher::~tabulated_double_bit_block_hasher()
{
    delete m_p_impl;
    m_p_impl = 0;
}

void tabulated_double_bit_block_hasher::seed_table(uint64_t seed, I_hash_seeder& seeder)
{
    uint8_t new_hash_table[sizeof(m_p_impl->m_hash_table)];
    memset(m_p_impl->m_hash_table, 0, sizeof(m_p_impl->m_hash_table));
    memset(new_hash_table, 0, sizeof(new_hash_table));
    seeder.generate_numbers( seed
                           , new_hash_table
                           , sizeof(m_p_impl->m_hash_table));
    memcpy(m_p_impl->m_hash_table, new_hash_table, sizeof(new_hash_table));
    uint8_t initial = m_p_impl->m_hash_table[7][3];
    uint8_t* p_array = reinterpret_cast<uint8_t*>(&seed);
    for(size_t i = 0; i < sizeof(uint64_t); i++)
    {
        initial += p_array[i] + i;
        initial = initial * initial;
    }
    m_p_impl->m_initial = perform_hash(initial);
}

void tabulated_double_bit_block_hasher::set_seeder(I_hash_seeder& seeder)
{
    m_p_impl->m_p_seeder = &seeder;
}

void tabulated_double_bit_block_hasher::generate_numbers( uint64_t seed
                                                        , uint8_t* p_array
                                                        , size_t array_size)
{
    uint64_t current_seed = seed;
    uint8_t current = 0;
    uint8_t old_hash_table[8][2*2];
    uint8_t old_initial = m_p_impl->m_initial;
    memcpy(old_hash_table,m_p_impl->m_hash_table,sizeof(m_p_impl->m_hash_table));
    if((m_p_impl->m_p_seeder != this) && (m_p_impl->m_p_seeder != 0))
    {
        m_p_impl->m_p_seeder->generate_numbers( current_seed
                                              , reinterpret_cast<uint8_t*>(&current_seed)
                                              , sizeof(current_seed));
        m_p_impl->m_p_seeder->generate_numbers( current_seed, p_array, array_size);
        m_p_impl->m_p_seeder->generate_numbers( current_seed
                                              , reinterpret_cast<uint8_t*>(&current_seed)
                                              , sizeof(current_seed));
        m_p_impl->m_p_seeder->generate_numbers( current_seed
                                              , reinterpret_cast<uint8_t*>(m_p_impl->m_hash_table)
                                              , sizeof(m_p_impl->m_hash_table));
        m_p_impl->m_p_seeder->generate_numbers( current_seed
                                              , reinterpret_cast<uint8_t*>(&current_seed)
                                              , sizeof(current_seed));
    }
    else
    {
        uint8_t* p_seed = reinterpret_cast<uint8_t*>(&current_seed);
        for(size_t i = 0; i < sizeof(current_seed); i++)
        {
            current += p_seed[i] + i;
            current = current * current;
            for(size_t j = 0; j < 4; j++)
            {
                uint8_t hash_table_value = p_seed[i] + m_p_impl->m_hash_table[i][j] + j + 1;
                hash_table_value = hash_table_value * hash_table_value;
                m_p_impl->m_hash_table[i][j] += hash_table_value;
            }
        }
    }
    m_p_impl->m_initial = current;
    current = current * current;
    for(size_t i = 0; i < array_size; i++)
    {
        current += perform_hash(current + p_array[i] + i);
        current = current * current;
        p_array[i] = current;
    }
    memcpy(m_p_impl->m_hash_table, old_hash_table, sizeof(old_hash_table));
    m_p_impl->m_initial = old_initial;
}

void tabulated_double_bit_block_hasher::set_initial(uint8_t initial)
{
    m_p_impl->m_initial = initial;
}

uint8_t tabulated_double_bit_block_hasher::perform_hash(uint8_t input)
{
    uint16_t copy_input = static_cast<uint16_t>(input) << 8 | input;
    uint8_t result = m_p_impl->m_initial;
    for(size_t i = 0; i < 8; i++)
    {
        result += m_p_impl->m_hash_table[i][copy_input & 0x3];
        copy_input = copy_input >> 1;
    }
    return result;
}

void tabulated_double_bit_block_hasher::set_seed(uint64_t seed)
{
    seed_table(seed, *this);
    uint8_t* p_seed = reinterpret_cast<uint8_t*>(&seed);
    m_p_impl->m_initial = 0;
    for(size_t i = 0; i < sizeof(seed); i++)
    {
        m_p_impl->m_initial += p_seed[i] + i;
        m_p_impl->m_initial = m_p_impl->m_initial * m_p_impl->m_initial;
    }
    m_p_impl->m_initial = perform_hash(m_p_impl->m_initial);
}

}
