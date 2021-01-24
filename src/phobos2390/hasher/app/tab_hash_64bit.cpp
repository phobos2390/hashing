#include <hasher/tabulated_hasher.h>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <numeric>

int main(int argc, char** argv)
{
    std::vector<uint8_t> input;
    hasher::tabulated_hasher hash_tabulator;
    hash_tabulator.set_output_size(sizeof(uint64_t));
    if(argc > 1)
    {
        input.assign(argv[1], argv[1] + strlen(argv[1]));
        std::string str_input = argv[1];
    }
    else
    {
        std::stringstream sstr;
        std::string str_input = "";
        while(std::getline(std::cin, str_input))
        {
            sstr << str_input << "\n";
        }
        for(uint32_t i = 0; i < sstr.str().length(); i++)
        {
            char c = sstr.str().c_str()[i];
            input.push_back(c);
        }
    }
    hash_tabulator.set_seed(std::accumulate(input.begin(), input.end(), 0));
    std::vector<uint8_t> result = hash_tabulator.perform_hash(input);
    std::cout << std::hex;
    std::for_each(result.begin()
                 ,result.end()
    ,[](uint8_t& result_value)
    {
        if(result_value < 0x10)
        {
            std::cout << "0";
        }
        std::cout << static_cast<uint16_t>(result_value);
    });
    return 0;
}
