/// @file hasher/tabulated_single_bit_hasher.h

#ifndef HASHER_TABULATED_SINGLE_BIT_HASHER_H
#define HASHER_TABULATED_SINGLE_BIT_HASHER_H

#include <stdint.h>
#include <functional>
#include <hasher/I_byte_hasher.h>
#include <hasher/I_hash_seeder.h>

namespace hasher
{

uint8_t standard_hash_tabulation(size_t index, uint8_t bit, uint8_t tab_value);

uint8_t index_multiplied_tabulation(size_t index, uint8_t bit, uint8_t tab_value);
    
class tabulated_single_bit_hasher : public I_byte_hasher, public I_hash_seeder
{
public:
    typedef std::function<uint8_t( size_t /*bit_index*/
                                 , uint8_t /*bit*/
                                 , uint8_t /*tab_value*/)> tab_func;
    
    /// Creates class value
    tabulated_single_bit_hasher();

    /// Removes class value
    virtual ~tabulated_single_bit_hasher();
    
    void set_hash_table_as_seed(uint64_t seed);
    uint64_t get_hash_table_as_seed();
    virtual void set_seed(uint64_t seed);
    
    void set_tab_function(tab_func custom_tab_function);
    void set_initial(uint8_t initial);
    void set_hash_table_value(size_t index, uint8_t value);
    
    virtual uint8_t perform_hash(uint8_t input);
    virtual void generate_numbers(uint64_t seed, uint8_t* p_array, size_t array_size);
    
    void hash_rotate();
private:
    struct Impl;
    Impl* m_p_impl;
};

}

#endif /* HASHER_TABULATED_SINGLE_BIT_HASHER_H */
