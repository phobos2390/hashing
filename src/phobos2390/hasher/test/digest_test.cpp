#include <catch2/catch.hpp>
#include <hasher/digest.h>

TEST_CASE( "digest_test", "stack" ) 
{
    Digest d(1);
    REQUIRE( 1 == 1 );
}