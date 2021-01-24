/// @file hasher/test/tabulated_hasher_test.cpp

#include <hasher/tabulated_hasher.h>
#include <catch2/catch.hpp>
#include <hasher/tabulated_hasher.h> // Testing include guard
#include <map>

using namespace hasher;

TEST_CASE( "tabulated_hasher_test.stack", "tabulated_hasher_test.stack" )
{
    tabulated_hasher c;
}

TEST_CASE( "tabulated_hasher_test.hash_64_bit", "tabulated_hasher_test.hash_64_bit" )
{
    std::map<uint64_t, uint32_t> result_instances;
    
    tabulated_hasher c;
    c.set_output_size(sizeof(uint64_t));
    c.set_seed(0);
    for(size_t i = 0; i < 0xffff; i++)
    {
        uint64_t result = 0;
        std::vector<uint8_t> result_vec 
            = c.perform_hash(reinterpret_cast<uint8_t*>(&i),sizeof(i));
        std::memcpy(&result, result_vec.data(), result_vec.size());
        if(result_instances.find(i) == result_instances.end())
        {
            result_instances.emplace(i,1);
        }
        else
        {
            ++result_instances[i];
        }
    }
    REQUIRE(result_instances.size() == 0xffff);
    std::for_each(result_instances.begin()
                 ,result_instances.end()
                 ,[](const std::pair<uint64_t, uint32_t>& values)
    {
        REQUIRE(values.second <= 1);
    });
}

TEST_CASE( "tabulated_hasher_test.int_hash_map_collision_test", "tabulated_hasher_test.int_hash_map_collision_test")
{
    std::map<uint64_t, uint32_t> result_instances;
    tabulated_hasher c;
    c.set_output_size(1);
    uint64_t current=0;
    while(result_instances[current] < 10)
    {
        ++result_instances[current];
    }
}
