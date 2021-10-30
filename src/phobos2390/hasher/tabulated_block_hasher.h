/// @file phobos2390/hasher/tabulated_block_hasher.h

#ifndef HASHER_TABULATED_BLOCK_HASHER_H
#define HASHER_TABULATED_BLOCK_HASHER_H

#include <hasher/I_byte_hasher.h>
#include <hasher/I_hash_seeder.h>

namespace hasher
{
class tabulated_block_hasher: public I_byte_hasher, public I_hash_seeder
{
public:
    /// Creates class value
    tabulated_block_hasher(size_t block_size);

    /// Removes class value
    virtual ~tabulated_block_hasher();
    
    virtual uint8_t perform_hash(uint8_t input);

    virtual void set_seed(uint64_t seed);
    
    virtual void generate_numbers(uint64_t seed, uint8_t* p_array_start, size_t array_size);
    
private:
    struct Impl;
    Impl* m_p_impl;
};

}

#endif /* HASHER_TABULATED_BLOCK_HASHER_H */
