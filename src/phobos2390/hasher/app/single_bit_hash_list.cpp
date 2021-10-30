#include <cstdlib>
#include <iostream>
#include <sstream>
#include <string>
#include <hasher/tabulated_single_bit_hasher.h>
#include <hasher/tabulated_hasher.h>
#include <hasher/tabulated_block_hasher.h>
#include <algorithm>
#include <numeric>

using namespace std;

int main(int argc, char** argv) 
{
    hasher::tabulated_block_hasher block_hasher(8);
    hasher::tabulated_hasher tab_hasher;
    tab_hasher.set_byte_hasher(block_hasher);
    tab_hasher.set_seeder(block_hasher);
    tab_hasher.set_output_size(4);
    tab_hasher.set_seed(0);
    
    for(size_t i = 0; i < 0x100; i++)
//    for(size_t i = 0; i < 0x1; i++)
    {
        std::vector<uint8_t> input;
        input.push_back(i);
        std::vector<uint8_t> output = tab_hasher.perform_hash(input);
        std::cout << std::hex
                  << i 
                  << "," 
                  << std::accumulate(output.begin(), output.end(), "",[](std::string current, uint8_t next)
                  {
                      std::stringstream sstr;
                      sstr << current;
                      if(next < 0x10)
                      {
                          sstr << "0";
                      }
                      sstr << std::hex << static_cast<uint16_t>(next);
                      return sstr.str().c_str();
                  })
                  << ",\n";
    }
    return 0;
}

