#include <cstdlib>
#include <iostream>
#include <sstream>
#include <string>
#include <hasher/tabulated_single_bit_hasher.h>
#include <hasher/tabulated_hasher.h>
#include <hasher/tabulated_block_hasher.h>
#include <algorithm>
#include <numeric>
#include <vector>
#include <fstream>

using namespace std;

bool get_dictionary_file(int argc, char** argv, char*& p_filename)
{
    bool found = false;
    for(int i = 0; i < argc; i++)
    {
        if( (0 == strncmp(argv[i], "-d", strlen("-d")))
         && ((i + 1) < argc))
        {
            p_filename = argv[i + 1];
            found = true;
        }
    }
    return found;
}

bool get_seed(int argc, char** argv, uint8_t*& p_seed, size_t& seed_size)
{
    bool found = false;
    for(int i = 0; i < argc; i++)
    {
        if((i + 1) < argc)
        {
            int j = i + 1;
            if(0 == strncmp(argv[i], "-s", strlen("-s")))
            {
                p_seed = reinterpret_cast<uint8_t*>(argv[j]);
                seed_size = strlen(argv[j]);
                found = true;
            }
            else if(0 == strncmp(argv[i], "-x", strlen("-x")))
            {
                uint8_t value[strlen(argv[j])];
                std::string current = "";
                memset(value, 0, sizeof(value));
                std::stringstream sstr;
                sstr << argv[j];
                seed_size = 0;
                while(std::getline(sstr, current, ':'))
                {
                    std::stringstream hex_convert;
                    hex_convert << std::hex << current;
                    hex_convert >> value[seed_size++];
                }
                std::memcpy(argv[j], value, seed_size);
                p_seed = reinterpret_cast<uint8_t*>(argv[j]);
                found = true;
            }
        }
    }
    return found;
}

uint64_t next_uint64_t(hasher::tabulated_hasher& tab_hasher, uint64_t& seed)
{
    tab_hasher.set_seed(seed);
    std::vector<uint8_t> hashed_value 
        = tab_hasher.perform_hash(reinterpret_cast<uint8_t*>(&seed), sizeof(seed));
    std::memcpy(&seed
               , hashed_value.data()
               , std::min(sizeof(seed), hashed_value.size()));
    tab_hasher.set_seed(seed);
    return seed;
}

uint64_t get_mask_from_maximum(uint64_t maximum)
{
    uint64_t mask = 0x1;
    while(mask < maximum)
    {
        mask = (mask << 0x1) | 0x1;
    }
    return mask;
}

uint64_t next_uint64_bounded(hasher::tabulated_hasher& tab_hasher, uint64_t& seed, uint64_t max)
{
    uint64_t next_64_bits = 0;
    bool not_found_yet = true;
    uint64_t mask = get_mask_from_maximum(max);
    while(not_found_yet)
    {
        next_64_bits = next_uint64_t(tab_hasher, seed) & mask;
        if(next_64_bits < max)
        {
            not_found_yet = false;
        }
    }
    return next_64_bits;
}

uint64_t get_word_count(std::iostream& input)
{
    uint64_t word_count = 0;
    std::string current;
    while(std::getline(input, current, '\n'))
    {
        ++word_count;
    }
    return word_count;
}

std::string get_line_in_file(std::iostream& input, uint64_t line_in_file)
{
    uint64_t line_count = 0;
    std::string current;
    input.clear(ios_base::eofbit);
    input.seekg(ios_base::beg);
    bool continuing = true;
    while(continuing)
    {
        if(std::getline(input, current, '\n'))
        {
            if(++line_count >= line_in_file)
            {
                continuing = false;
            }
        }
        else
        {
            continuing = false;
        }
    }
    return current;
}

int main(int argc, char** argv) 
{
    hasher::tabulated_block_hasher block_hasher(8);
    hasher::tabulated_hasher tab_hasher;
    tab_hasher.set_byte_hasher(block_hasher);
    tab_hasher.set_seeder(block_hasher);
    tab_hasher.set_output_size(8);
    tab_hasher.set_seed(0);
    
    uint8_t* p_seed = 0;
    size_t seed_size = 0;
    
    char* p_filename = 0;
    
    uint64_t dict_word_count = 0;
    uint64_t seed = 0;
    uint64_t sentences = 3;
    uint64_t min_sentence_word_count = 1;
    uint64_t max_sentence_word_count = 4;
    uint64_t min_number_count = 1;
    uint64_t max_number_count = 4; 
    uint64_t number_mask = 0xF;
    uint64_t number_count = 0x4;
    
    if(get_seed(argc, argv, p_seed, seed_size))
    {
        std::vector<uint8_t> hashed_value 
            = tab_hasher.perform_hash(p_seed, seed_size);
        std::memcpy(&seed
                   , hashed_value.data()
                   , std::min(sizeof(seed), hashed_value.size()));
        tab_hasher.set_seed(seed);
    }
    if(get_dictionary_file(argc, argv, p_filename))
    {
        std::fstream dict_file;
        dict_file.open(p_filename);
        dict_word_count = get_word_count(dict_file);
        
        std::string sentence_delim = "";
        std::string word_delim = "";
        
        for(uint64_t i = 0; i < sentences; i++)
        {
            std::cout << sentence_delim;
            word_delim = "";
            uint64_t words 
                = next_uint64_bounded( tab_hasher
                                     , seed
                                     ,  max_sentence_word_count 
                                      - min_sentence_word_count) 
                  + min_sentence_word_count;
            for(uint64_t j = 0; j < words; j++)
            {
                std::cout << word_delim;
                if((next_uint64_t(tab_hasher, seed) & number_mask) > number_count)
                {
                    uint64_t dict_position 
                        = next_uint64_bounded(tab_hasher, seed, dict_word_count);
                    std::cout << get_line_in_file(dict_file, dict_position);
                }
                else
                {
                    uint64_t number_count
                        = next_uint64_bounded( tab_hasher
                                             , seed
                                             ,  max_number_count
                                              - min_number_count) 
                          + min_number_count;
                    for(uint64_t n = 0; n < number_count; n++)
                    {
                        std::cout << std::dec << next_uint64_bounded(tab_hasher, seed, 10);
                    }
                }
                word_delim = "-";
            }
            sentence_delim = ".";
        }
        std::cout << "\n";
    }
    
    return 0;
}
