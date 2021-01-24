/// @file hasher/tabulated_hasher.h

#ifndef HASHER_TABULATED_HASHER_H
#define HASHER_TABULATED_HASHER_H

#include <stdint.h>
#include <vector>
#include <hasher/I_hash_seeder.h>
#include <hasher/I_byte_hasher.h>

namespace hasher
{

class tabulated_hasher
{
public:
    /// Creates class value
    tabulated_hasher();

    /// Removes class value
    virtual ~tabulated_hasher();
    
    void set_output_size(size_t bytes);

    void set_bit_width(size_t bits);
    
    void set_seed(uint64_t seed);
    
    void set_byte_hasher(I_byte_hasher& hasher);
    
    void set_seeder(I_hash_seeder& seeder);
    
    std::vector<uint8_t> perform_hash(std::vector<uint8_t> input);
    
    std::vector<uint8_t> perform_hash(uint8_t* p_input, size_t input_size);
private:
    struct Impl;
    Impl* m_p_impl;
};

}

#endif /* HASHER_TABULATED_HASHER_H */
