#include <catch2/catch.hpp>
#include <hasher/digest.h>

TEST_CASE( "digest_test", "stack" ) 
{
    Digest d(1);
    REQUIRE( 1 == 1 );
}

TEST_CASE("hamming code", "hamming code implementation test")
{
    uint8_t message [] = {
        0b00101011,
        0b10101110
    };
    uint8_t error_flags [] = {
        0b11101000,
        0b10000000
    };
    uint8_t parity_bit_check = 0;
    for(uint32_t i = 0; i < sizeof(message); i++)
    {
        uint8_t flag = 0b10000000;
        for(uint32_t j = 0; j < 8; j++)
        {
            if(message[i] & flag)
            {
                parity_bit_check ^= (j + i*8);
            }
            flag = flag >> 1;
        }
    }
    REQUIRE(static_cast<uint16_t>(parity_bit_check) == 10);
}