/// @file phobos2390/hasher/tabulated_block_hasher.cpp

#include <hasher/tabulated_block_hasher.h>

static const size_t s_c_bits_in_a_byte = 0x8;

namespace hasher
{
struct tabulated_block_hasher::Impl
{
    size_t m_block_size_exponent;
    size_t m_block_size;
    size_t m_block_mask;
    size_t m_table_size;
    uint8_t* m_p_block_hash;
    uint8_t m_initial;
    
    Impl(size_t block_size)
    : m_block_size_exponent(block_size)
    , m_block_size(0x1 << m_block_size_exponent)
    , m_block_mask(m_block_size - 1)
    , m_table_size(m_block_size * s_c_bits_in_a_byte)
    , m_p_block_hash(new uint8_t[m_table_size])
    , m_initial(0)
    {
        uint8_t seed_array[sizeof(uint64_t)];
        memset(seed_array, 0, sizeof(seed_array));
        
    }

    virtual ~Impl()
    {
        delete [] m_p_block_hash;
        m_p_block_hash = 0;
    }
    
    uint8_t perform_hash(uint8_t input)
    {
        uint16_t current = (static_cast<uint16_t>(input) << 0x8) | input;
        uint8_t result = m_initial;
        for(size_t i = 0; i < s_c_bits_in_a_byte; i++)
        {
            result += get_entry(i, current & m_block_mask);
            current = current >> 1;
        }
        return result;
    }
    
    uint8_t& get_entry(size_t bit_index, size_t block_index)
    {
        return m_p_block_hash[(m_block_size * bit_index) + block_index];
    }
    
    void clear_table()
    {
        memset(m_p_block_hash, 0, m_table_size);
        m_initial = 0;
    }
    
    void set_seed(uint8_t* p_array, size_t array_size)
    {
        clear_table();
        uint8_t counter = 0;
        for(size_t i = 0; i < m_block_size; i++)
        {
            for(size_t j = 0; j < s_c_bits_in_a_byte; j++)
            {
                uint8_t new_value = (++counter + i) * (counter + j);
                get_entry(j, i) += new_value;//(++counter + i) * (counter + j);
            }
        }
        m_initial = ++counter;
        calculate_new_table();
        size_t row = 0;
        size_t column = 0;
        for(size_t i = 0; i < array_size; i++)
        {
            uint8_t current = (row << 0x4) + column + p_array[i] + get_entry(column, row);
            m_initial = perform_hash(current + m_initial);
            get_entry(column, row) = perform_hash(current + m_initial);
            row = (row + 1) % m_block_size;
            if(row == 0)
            {
                column = (column + 1) % s_c_bits_in_a_byte;
            }
            calculate_new_table();
        }
        calculate_new_table();
        m_initial = perform_hash(m_initial);
    }
    
    void calculate_new_table()
    {
        uint8_t current = perform_hash(m_initial);
        for (size_t i = 0; i < m_block_size; i++)
        {
            for (size_t j = 0; j < s_c_bits_in_a_byte; j++)
            {
                current = perform_hash(current + j + i + get_entry(j, i));
                get_entry(j, i) = current;
            }
        }
        m_initial = perform_hash(m_initial);
    }
    
    void generate_numbers_no_backup(uint64_t seed, uint8_t* p_array_start, size_t array_size)
    {
        set_seed(reinterpret_cast<uint8_t*>(&seed), sizeof(seed));
        for(size_t i = 0; i < array_size; i++)
        {
            calculate_new_table();
            p_array_start[i] = perform_hash(m_initial + i);
        }
    }
};

tabulated_block_hasher::tabulated_block_hasher(size_t block_size)
    :m_p_impl(new Impl(block_size))
{
}

tabulated_block_hasher::~tabulated_block_hasher()
{
    delete m_p_impl;
    m_p_impl = 0;
}

uint8_t tabulated_block_hasher::perform_hash(uint8_t input)
{
    return m_p_impl->perform_hash(input);
}

void tabulated_block_hasher::set_seed(uint64_t seed)
{
//    uint8_t generated_seed[m_p_impl->m_table_size];
//    generate_numbers(seed, generated_seed, sizeof(generated_seed));
    m_p_impl->set_seed(reinterpret_cast<uint8_t*>(&seed), sizeof(seed));//generated_seed, sizeof(generated_seed));
}

void tabulated_block_hasher::generate_numbers(uint64_t seed, uint8_t* p_array_start, size_t array_size)
{
    uint8_t backup_array[m_p_impl->m_table_size];
    memcpy(backup_array, m_p_impl->m_p_block_hash, m_p_impl->m_table_size);
    uint8_t backup_initial = m_p_impl->m_initial;
    m_p_impl->generate_numbers_no_backup(seed, p_array_start, array_size);
    memcpy(m_p_impl->m_p_block_hash, backup_array, m_p_impl->m_table_size);
    m_p_impl->m_initial = backup_initial;
}

}
