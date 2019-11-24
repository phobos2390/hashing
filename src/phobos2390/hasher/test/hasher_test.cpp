#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include <catch2/catch.hpp>
#include <hasher/basic_hash.h>
#include <iostream>
#include <stdio.h>
#include <map>

TEST_CASE( "boileplate", "basic" ) 
{
    REQUIRE( 1 == 1 );
}

uint64_t collisions_test( uint64_t (*hash_func)(uint64_t input)
                        , std::map<uint64_t,uint64_t>& collisions
                        , uint64_t start
                        , uint64_t end)
{
    uint64_t total_collisions = 0;
    uint64_t max_collisions = 0;
    for(uint32_t i = start; i < end; i++)
    {
        uint64_t hashed = hash_long(i);
        if(collisions.find(hashed) == collisions.end())
        {
            collisions.emplace(hashed, 0);
        }
        else
        {
            std::cout << "Collision: " << std::hex << hashed << "(" << i << ")\n";
            fflush(stdout);
            ++total_collisions;
            ++collisions[hashed];
            if(collisions[hashed] > max_collisions)
            {
                max_collisions = collisions[hashed];
            }
        }
    }
    std::cout << "Max Collisions: " << max_collisions << "\n";
    fflush(stdout);
    return total_collisions;
}

TEST_CASE("integer 32 bit", "hash")
{
    uint64_t hash_value = 0x15162d8306ea2c7c;
    for(uint32_t i = 0; i < 0x40; i++)
    {
        hash_value = hash_int(hash_value);
        std::cout << "    0x" << std::hex << hash_value << ",\n";
    }
    std::cout << "\n";
    fflush(stdout);
}

TEST_CASE("integer 64 bit", "hash")
{
    uint64_t hash_value = 0x123123159796acca;
    for(uint32_t i = 0; i < 0x78; i++)
    {
        hash_value = hash_long(hash_value);
        std::cout << "    0x" << std::hex << hash_value << ",\n";
    }
    fflush(stdout);
}

TEST_CASE("exhaustive collisions", "hash collision")
{
    std::map<uint64_t, uint64_t> collisions;
    uint64_t total_collisions = 0;
    uint64_t max_collisions = 0;
    uint64_t hash_space = 0x10;// 0x100ff;//0x1ffff;//0xffffffffffffffff;
    for(uint32_t i = 0; i < hash_space; i++)
    {
        uint64_t hashed = hash_long(i);
        if(collisions.find(hashed) == collisions.end())
        {
            collisions.emplace(hashed, 0);
        }
        else
        {
//            std::cout << "Collision: " << std::hex << hashed << "(" << i << ")\n";
//            fflush(stdout);
            ++total_collisions;
            ++collisions[hashed];
            if(collisions[hashed] > max_collisions)
            {
                max_collisions = collisions[hashed];
            }
        }
    }
    REQUIRE(total_collisions <= 0x100000000);
    REQUIRE(max_collisions <= total_collisions);
}

uint64_t mod_exp_xor_hash(uint64_t input)
{
    return hash_long(hash_mod_exp(input) ^ input);
}

//TEST_CASE("mod_exp_hash", "example mod_exp_hash")
//{
//    std::map<uint64_t, uint64_t> collisions;
//    uint64_t total_collisions = collisions_test(&hash_mod_exp,collisions,0x0,0xffff);
//    REQUIRE(total_collisions == 0x0);
//}

TEST_CASE("mod_exp_hash", "example combined mod_exp_xor_hash")
{
    std::map<uint64_t, uint64_t> collisions;
    uint64_t total_collisions = collisions_test(&mod_exp_xor_hash,collisions,0x0,0x10);
    REQUIRE(total_collisions == 0x0);
}

void print_stats_for_hash_byte_seeds(uint8_t initial, uint64_t seed)
{
    std::map<uint8_t, uint8_t> collisions;
    uint8_t total_collisions = 0;
    for(uint16_t i = 0; i < 0x100; i++)
    {
        uint8_t hash = hash_byte_seed(i, initial, seed);
        if(collisions.find(hash) == collisions.end())
        {
            collisions.emplace(hash, 0);
        }
        else
        {
            ++total_collisions;
            ++collisions[hash];
        }
    }
    std::map<uint8_t, uint8_t> num_collisions;
    std::for_each( collisions.begin()
                 , collisions.end()
                 , [&num_collisions](std::pair<uint8_t, uint8_t> collision_pair)
    {
        if(num_collisions.find(collision_pair.second) == num_collisions.end())
        {
            num_collisions.emplace(collision_pair.second, 1);
        }
        else
        {
            ++num_collisions[collision_pair.second];
        }
    });
    std::cout << std::hex << "hash 8 stats (" << static_cast<uint16_t>(initial) << ", " << seed << ")\n";
    std::for_each( num_collisions.begin()
                 , num_collisions.end()
                 , [](std::pair<uint8_t, uint8_t> collision_pair)
    {
        std::cout << static_cast<uint16_t>(collision_pair.first)  << ": "
                  << static_cast<uint16_t>(collision_pair.second) << "\n";
    });
    std::cout << std::hex << "total collisions: " << static_cast<uint16_t>(total_collisions) << "\n";
}

void print_stats_for_hash_short_seeds(uint8_t initial, uint64_t seed)
{
    uint16_t hashes [c_bits_in_short];
    generate_short_hash_bytes(initial, seed, hashes);
    std::map<uint16_t, uint16_t> collisions;
    uint16_t total_collisions = 0;
    for(uint32_t i = 0; i < 0x10000; i++)
    {
        uint8_t hash = hash_short_seed(i, initial, seed);
        if(collisions.find(hash) == collisions.end())
        {
            collisions.emplace(hash, 0);
        }
        else
        {
            ++total_collisions;
            ++collisions[hash];
        }
    }
    std::map<uint16_t, uint16_t> num_collisions;
    std::for_each( collisions.begin()
                 , collisions.end()
                 , [&num_collisions](std::pair<uint16_t, uint16_t> collision_pair)
    {
        if(num_collisions.find(collision_pair.second) == num_collisions.end())
        {
            num_collisions.emplace(collision_pair.second, 1);
        }
        else
        {
            ++num_collisions[collision_pair.second];
        }
    });
    std::cout << std::hex << "hash 16 stats (" << static_cast<uint16_t>(initial) << ", " << seed << ")\n";
    std::cout << "Hash list: \n";
    for(uint32_t i = 0; i < c_bits_in_short; i++)
    {
        std::cout << "    0x" << hashes[i] << ",\n";
    }
    std::cout << "End Hash list: \n";
    std::for_each( num_collisions.begin()
                 , num_collisions.end()
                 , [](std::pair<uint16_t, uint16_t> collision_pair)
    {
        std::cout << static_cast<uint16_t>(collision_pair.first)  << ": "
                  << static_cast<uint16_t>(collision_pair.second) << "\n";
    });
    std::cout << std::hex << "total collisions: " << total_collisions << "\n";
}

void print_stats_for_hash_int_seeds(uint8_t initial, uint64_t seed)
{
    uint16_t hashes [c_bits_in_int];
    generate_int_hash_bytes(initial, seed, hashes);
    std::map<uint32_t, uint32_t> collisions;
    uint16_t total_collisions = 0;
    for(uint32_t i = 0; i < 0x100000000; i++)
    {
        uint8_t hash = hash_short_seed(i, initial, seed);
        if(collisions.find(hash) == collisions.end())
        {
            collisions.emplace(hash, 0);
        }
        else
        {
            ++total_collisions;
            ++collisions[hash];
        }
    }
    std::map<uint32_t, uint32_t> num_collisions;
    std::for_each( collisions.begin()
                 , collisions.end()
                 , [&num_collisions](std::pair<uint32_t, uint32_t> collision_pair)
    {
        if(num_collisions.find(collision_pair.second) == num_collisions.end())
        {
            num_collisions.emplace(collision_pair.second, 1);
        }
        else
        {
            ++num_collisions[collision_pair.second];
        }
    });
    std::cout << std::hex << "hash 32 stats (" << static_cast<uint16_t>(initial) << ", " << seed << ")\n";
    std::cout << "Hash list: \n";
    for(uint32_t i = 0; i < c_bits_in_int; i++)
    {
        std::cout << "    0x" << hashes[i] << ",\n";
    }
    std::cout << "End Hash list: \n";
    std::for_each( num_collisions.begin()
                 , num_collisions.end()
                 , [](std::pair<uint16_t, uint16_t> collision_pair)
    {
        std::cout << static_cast<uint16_t>(collision_pair.first)  << ": "
                  << static_cast<uint16_t>(collision_pair.second) << "\n";
    });
    std::cout << std::hex << "total collisions: " << total_collisions << "\n";
}

uint64_t gen_new_seed(uint8_t initial, uint64_t seed)
{
    uint64_t ret_seed = 0;
    uint8_t* seed_array = reinterpret_cast<uint8_t*>(&ret_seed);
    uint8_t current_seed = initial;
    for(uint32_t i = 0; i < sizeof(seed_array); i++)
    {
        seed_array[i] = hash_byte_seed(current_seed, initial, seed);
        current_seed = seed_array[i];
    }
    return ret_seed;
}

TEST_CASE("byte seeded hash", "initial byte seeded")
{    
    print_stats_for_hash_byte_seeds(0x5a, 0x0123456789abcdef);
    print_stats_for_hash_byte_seeds(0x32, 0x0123456789abcdef);
    print_stats_for_hash_byte_seeds(0x00, 0x0123456789abcdef);
    print_stats_for_hash_byte_seeds(0xaa, 0x0123456789abcdef);

    print_stats_for_hash_byte_seeds(0xaa, 0xfedcba9876543210);

    print_stats_for_hash_byte_seeds(0xaa, 0x74cf9b203d8e1a65);

    print_stats_for_hash_byte_seeds(0xaa, 0x20bc58317f9a64de);
    
    print_stats_for_hash_byte_seeds
        ( 0xaa
        , gen_new_seed(0xaa, 0x0123456789abcdef));

    print_stats_for_hash_byte_seeds
        ( 0xaa
        , gen_new_seed
            ( 0xaa
            , gen_new_seed(0xaa, 0x0123456789abcdef)));

    print_stats_for_hash_byte_seeds
        ( 0xaa
        , gen_new_seed
            ( 0xaa
            , gen_new_seed
                ( 0xaa
                , gen_new_seed(0xaa, 0x0123456789abcdef))));

    print_stats_for_hash_byte_seeds
        ( 0xaa
        , gen_new_seed
            ( 0xaa
            , gen_new_seed
                ( 0xaa
                , gen_new_seed(0xaa, 0x74cf9b203d8e1a65))));
}

TEST_CASE("short seeded hash", "initial short seeded")
{
    uint64_t initial = 0xaa;
//    uint64_t seed = 0x0123456789abcdef;
    uint64_t seed = 0x20bc58317f9a64de;
//    uint64_t seed = 0x74cf9b203d8e1a65;
    print_stats_for_hash_short_seeds(initial, seed);
}

