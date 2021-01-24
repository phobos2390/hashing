/// @file hasher/tabulated_double_bit_block_hasher.h

#ifndef HASHER_TABULATED_DOUBLE_BIT_BLOCK_HASHER_H
#define HASHER_TABULATED_DOUBLE_BIT_BLOCK_HASHER_H

#include <hasher/I_byte_hasher.h>
#include <hasher/I_hash_seeder.h>

namespace hasher
{

class tabulated_double_bit_block_hasher : public I_byte_hasher, public I_hash_seeder
{
public:
    /// Creates class value
    tabulated_double_bit_block_hasher();

    /// Removes class value
    virtual ~tabulated_double_bit_block_hasher();
    
    void seed_table(uint64_t seed, I_hash_seeder& seeder);
    void set_seeder(I_hash_seeder& seeder);
    virtual void generate_numbers(uint64_t seed, uint8_t* p_array_start, size_t array_size);
    
    void set_initial(uint8_t initial);
    
    void set_seed(uint64_t seed);
    
    virtual uint8_t perform_hash(uint8_t input);
private:
    struct Impl;
    Impl* m_p_impl;
};

}

#endif /* HASHER_TABULATED_DOUBLE_BIT_BLOCK_HASHER_H */
