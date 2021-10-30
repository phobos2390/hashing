/// @file hasher/test/tabulated_single_bit_hasher_test.cpp

#include <hasher/tabulated_single_bit_hasher.h>
#include <catch2/catch.hpp>
#include <hasher/tabulated_single_bit_hasher.h> // Testing include guard
#include <map>
#include <iostream>

using namespace hasher;

TEST_CASE( "tabulated_single_bit_hasher_test", "stack" )
{
    tabulated_single_bit_hasher c;
}

//void init_hit_map_single(std::map<uint8_t, size_t>& hit_map)
//{
//    for(uint16_t i = 0; i < 0x100; i++)
//    {
//        hit_map.emplace(i,0);
//    }
//}

void reset_hit_map_single(std::map<uint8_t, size_t>& hit_map)
{
    for(uint16_t i = 0; i < 0x100; i++)
    {
        hit_map[i] = 0;
    }
}

TEST_CASE( "tabulated_single_bit_hasher_test.basic_tabulation", "tabulated_single_bit_hasher_test.basic_tabulation" )
{
//    tabulated_single_bit_hasher c;
//    c.set_tab_function(index_multiplied_tabulation);
//    
//    for(size_t i = 0; i < sizeof(uint64_t); i++)
//    {
//        c.hash_rotate();
//        c.set_initial(c.perform_hash(i + 1));
//    }
//    
//    std::cout << "seed: " << std::hex << "0x" << c.get_hash_table_as_seed() << "\n";
//    
//    c.set_tab_function(standard_hash_tabulation);
//    
//    std::map<uint8_t, size_t> hit_map;
//    reset_hit_map_single(hit_map);
//    size_t collisions_before_resize = 2;//5;
//    uint8_t current = 0;
//    size_t cycle = 0;
//    uint8_t result = 0;
//    uint16_t mask = 0x1;
//    do
//    {
//        do
//        {
//            ++cycle;
//            result = c.perform_hash(current++);
//            ++hit_map[result & mask];
//            std::cout << "Result " << (result & mask) << "\n";
//        }
//        while(hit_map[result & mask] < collisions_before_resize);
//        std::cout << "Mask " << mask 
//                  << " Got to " << static_cast<uint16_t>(current )
//                  << " with " << cycle 
//                  << " cycles\n";
//        fflush(stdout);
//        reset_hit_map_single(hit_map);
//        cycle = 0;
//        current = 0;
//        mask = mask << 1 | 0x1;
//    }
//    while(mask < 0x1FF);
}
