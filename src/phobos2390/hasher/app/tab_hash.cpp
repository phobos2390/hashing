#include <hasher/tabulated_hasher.h>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <numeric>


void set_base_64_codex(char* codex, size_t codex_size)
{
    size_t iterator = 0;
    for(char n = '0'; n <= '9'; n++)
    {
        codex[iterator++] = n;
    }
    for(char l = 'a'; l <= 'z'; l++)
    {
        codex[iterator++] = l;
    }
    for(char u = 'A'; u <= 'Z'; u++)
    {
        codex[iterator++] = u;
    }
    codex[iterator++] = '-';
    codex[iterator++] = '_';
}

void print_base64(std::vector<uint8_t>& input)
{
    std::vector<uint8_t>::iterator iter = input.begin();
    
}

int main(int argc, char** argv)
{
    std::vector<uint8_t> input;
    hasher::tabulated_hasher hash_tabulator;
    uint64_t output_size = sizeof(uint64_t);
    if(argc > 2)
    {
        input.assign(argv[2], argv[2] + strlen(argv[2]));
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
            input.push_back(sstr.str().c_str()[i]);
        }
    }
    if(argc > 1)
    {
        output_size = 0;
        // argv[1] output size in bits
        std::string output_size_str = argv[1];
        std::stringstream sstr;
        sstr << output_size_str;
        sstr >> output_size;
        output_size = output_size / 8;
    }
    hash_tabulator.set_output_size(output_size);
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