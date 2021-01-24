/// @file hasher/test/tabulated_double_bit_block_hasher_test.cpp

#include <hasher/tabulated_double_bit_block_hasher.h>
#include <catch2/catch.hpp>
#include <hasher/tabulated_double_bit_block_hasher.h> // Testing include guard
#include <iostream>
#include <map>

using namespace hasher;

TEST_CASE( "tabulated_double_bit_block_hasher_test", "stack" )
{
    tabulated_double_bit_block_hasher c;
}

TEST_CASE( "tabulated_double_bit_block_hasher_test.seed_this", "tabulated_double_bit_block_hasher_test.seed_this")
{
    tabulated_double_bit_block_hasher c;
    c.seed_table(0, c);
    uint8_t collision[0x100];
    memset(collision, 0, sizeof(collision));
    for(size_t i = 0; i < sizeof(collision); i++)
    {
        uint16_t index = c.perform_hash(i);
        uint16_t collision_value = ++collision[index];
        std::cout << std::hex << "0x" << i 
                              << ": 0x" << index 
                              << "(" << collision_value << ")\n";
    }
    fflush(stdout);
}

void init_hit_map(std::map<uint8_t, size_t>& hit_map)
{
    for(uint16_t i = 0; i < 0x100; i++)
    {
        hit_map.emplace(i,0);
    }
}

void reset_hit_map(std::map<uint8_t, size_t>& hit_map)
{
    for(uint16_t i = 0; i < 0x100; i++)
    {
        hit_map[i] = 0;
    }
}

TEST_CASE( "tabulated_double_bit_block_hasher_test.seeded_hash_table", "tabulated_double_bit_block_hasher_test.seeded_hash_table")
{
    tabulated_double_bit_block_hasher c;
    for(size_t i = 0; i < 255; i++)
    {
        c.seed_table(i, c);
        std::map<uint8_t, size_t> hit_map;
        reset_hit_map(hit_map);
        size_t collisions_before_resize = 5;
        uint8_t current = 0;
        size_t cycle = 0;
        uint8_t result = 0;
        uint16_t mask = 0x1;
//        std::cout << "seed " << i << "\n";
        do
        {
            do
            {
                ++cycle;
                result = c.perform_hash(current++);
                ++hit_map[result & mask];
    //            std::cout << "Result " << (result & mask) << "\n";
            }
            while(hit_map[result & mask] <= collisions_before_resize);
//            std::cout << "Mask " << mask 
//                      << " Got to " << static_cast<uint16_t>(current )
//                      << " with " << cycle 
//                      << " cycles\n";
//            fflush(stdout);
            reset_hit_map(hit_map);
            cycle = 0;
            current = 0;
            mask = mask << 1 | 0x1;
        }
        while(mask < 0x1FF);
    }
}

TEST_CASE("tabulated_double_bit_block_hasher_test.test_seed_reseed", "tabulated_double_bit_block_hasher_test.test_seed_reseed")
{
    tabulated_double_bit_block_hasher a;
    tabulated_double_bit_block_hasher b;
    a.set_seed(0);
    b.set_seed(0);
    b.set_seed(0);
    for(size_t i = 0; i < 0x100; i++)
    {
        REQUIRE(a.perform_hash(i) == b.perform_hash(i));
    }
}