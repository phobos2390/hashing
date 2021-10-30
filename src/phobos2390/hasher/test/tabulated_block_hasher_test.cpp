/// @file phobos2390/hasher/test/tabulated_block_hasher_test.cpp

#include <hasher/tabulated_block_hasher.h>
#include <catch2/catch.hpp>
#include <hasher/tabulated_block_hasher.h> // Testing include guard

#include <iostream>

using namespace hasher;

TEST_CASE( "tabulated_block_hasher_test", "stack" )
{
    tabulated_block_hasher c1(1);
    tabulated_block_hasher c2(2);
    tabulated_block_hasher c3(3);
    tabulated_block_hasher c4(4);
    tabulated_block_hasher c5(5);
    tabulated_block_hasher c6(6);
    tabulated_block_hasher c7(7);
    tabulated_block_hasher c8(8);
}

TEST_CASE( "tabulated_block_hasher_test.verify_zero", "verify_zero" )
{
    tabulated_block_hasher c(8);
    
    size_t key = 0;
    size_t range_size = 0x200;
    uint8_t values[range_size];
    c.generate_numbers(key, values, sizeof(values));
    bool found_value = false;
    std::cout << std::hex;
    for(size_t i = 0; i < range_size; i++)
    {
        if(values[i] == 0x00)
        {
            found_value = true;
        }
        if(values[i] < 0x10)
        {
            std::cout << "0";
        }
        std::cout << static_cast<uint16_t>(values[i]) << " ";
    }
    std::cout << "\n";
    REQUIRE(found_value);
}

TEST_CASE( "tabulated_block_hasher_test.verify_zero_other", "verify_zero_other" )
{
    tabulated_block_hasher c(8);
    
    size_t key = 2;
    size_t range_size = 0x200;
    uint8_t values[range_size];
    c.generate_numbers(key, values, sizeof(values));
    bool found_value = false;
    std::cout << std::hex;
    for(size_t i = 0; i < range_size; i++)
    {
        if(values[i] == 0x00)
        {
            found_value = true;
        }
        if(values[i] < 0x10)
        {
            std::cout << "0";
        }
        std::cout << static_cast<uint16_t>(values[i]) << " ";
    }
    std::cout << "\n";
    REQUIRE(found_value);
}