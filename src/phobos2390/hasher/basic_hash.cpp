#include <hasher/basic_hash.h>

uint64_t initial_hash = 0x655425abbbbeefad;

uint64_t byte_array [] = {
    0x2a3215072562f230,
    0xaaaacceeefffeeaa,
    0xff23251accc35062,
    0x023caaadedcce003,
    0x87321523cccace33,
    0x623ec8222313ccfe,
    0xa336229993ace211,
    0xb124325cf9938231,
};

uint64_t short_array [] = {
    // 0
    0x0123456789abcdef,
    0xfedcba9876543210,
    0x9caceef902311901,
    0xbcddda3308967657,
    // 4
    0x6553e21effceda98,
    0x8769156072637788,
    0x696cceeefaadce96,
    0x1456dea396965621,
    // 8
    0x879656eac96f11da,
    0xc086255c990758cb,
    0x96759a09c9700732,
    0x8665452cccabacae,
    // 12
    0x1aaaafecad973869,
    0x2526acfeefadc1f9,
    0x986954342512123a,
    0x6cbbbef999839888,
    // 16
};

uint64_t int_array [] = {
    // 0
    0x02512341adddaad3,
    0x9692aaccdeffa93c,
    0x0acdaaa969602341,
    0x88962096755798ca,
    // 4
    0x0878623690690234,
    0x111421311ca99865,
    0x9898267564544555,
    0x6740851264575721,
    // 8
    0x9706accdaffffec1,
    0x96565370c00cadca,
    0x62078909acefaa97,
    0x959583a975c7f9fa,
    // 12
    0x429870aa8a9a898a,
    0x5023458ccaaddef2,
    0xccceaffaea978759,
    0x6971fccca96931ac,
    // 16
    0x89620abbaecbba97,
    0x19892226389caac9,
    0x959197690865972c,
    0x9962202076010759,
    // 20
    0x55520761070aaacc,
    0x96019607162343ab,
    0x96696aca96730bbb,
    0xbac9969ca9520ce0,
    // 24
    0xeee0578eeeac9613,
    0x5195caaaac958659,
    0x196901ac06923fff,
    0x68810980232cacc2,
    // 28
    0x600707082807cca9,
    0x807596597238aa8b,
    0xbbac95967296ac17,
    0x07076aca0859ecaf,
    // 32
};

uint64_t long_array [] = {
    0x99a6a004724debcb,
    0x385e7594ccb0874f,
    0x5098facb8c4e0b06,
    0xf7b73aab50c5d28f,
    0x552e7c7810bb6159,
    0x956ddbc886af6077,
    0x4a2ccd851adb1800,
    0x9eae4b4ffdf7d2eb,
    0xa7e20fdb7d7c0dc2,
    0x5464cd2cf39f38c2,
    0x436adcc033098ba8,
    0xa47925fac218803d,
    0x191f2064ddff2418,
    0xe37a64df4f00feac,
    0x52edaafb06a5655a,
    0x33b654df91c5456d,
    0xbf48ba3994d241ba,
    0xa6f3c180d82abbe3,
    0xf13bdfa4bd7dc7d9,
    0xc991f68e5d4f838c,
    0x41970b9e9d5602a6,
    0xd564f41c09857a0f,
    0x8cbbd34608d040fc,
    0x69274cdefb713879,
    0xd006b4696b2c538d,
    0xcf432d16a6af574f,
    0x4d0a9a2a627256b1,
    0xcad7e7bf6a08e286,
    0x20637f562efa347a,
    0x004fafb5d579613d,
    0x90bf65af6e8e6e01,
    0x6dfc3a0b05ccd304,
    0xa2b3020ecb0a3f07,
    0xf37db7416bcdc5f0,
    0xda0c1fcda497da0b,
    0x6d9f3c78ed82fc18,
    0x00dfa1c28bbb9aa4,
    0xf021dfa623f092ca,
    0x4fde838726b0ca1b,
    0xbdc80f161ef63993,
    0xa1ed60a3c7037a15,
    0x51ce93e227478f5d,
    0xfeda596217925778,
    0x62fe9c4af6c3e2e7,
    0xd032c76e236903ff,
    0xccc41c0a194e6191,
    0xd0021f88184faa55,
    0xd36e19bcab784516,
    0x65728f613330db73,
    0x1b5c86e992d947ff,
    0x3625d5a4497ccca3,
    0x063ae96e5f63dbca,
    0x370dfd8fe9db1ae1,
    0xbe7b74c8e0c2010d,
    0x26be8dc61cb6d353,
    0x194e37fb020bd667,
    0x66c47667d586ab23,
    0x706c6965a654c2a9,
    0xb16e3e28210336cf,
    0xbe801e168bfcda65,
    0x25cb2bd2be1691cd,
    0x8a9e92fe3f4fe750,
    0xa2b9a9de1a244423,
    0x0a82bc52aca30aad,
};

uint64_t hash_byte(uint8_t byte_value)
{
    uint64_t hash_value = initial_hash;
    for(uint64_t i = 0; i < c_bits_in_byte; i++)
    {
        if((byte_value & 0x1) == 0)
        {
            hash_value = hash_value + byte_array[i];
        }
        byte_value = byte_value >> 1;
    }
    return hash_value;
}

uint64_t hash_short(uint16_t short_value)
{
    uint64_t hash_value = hash_byte(xor_divide(short_value));
    for(uint64_t i = 0; i < c_bits_in_byte * sizeof(uint16_t); i++)
    {
        if((short_value & 0x1) == 0)
        {
            hash_value = hash_value + short_array[i];
        }
        short_value = short_value >> 1;
    }
    return hash_value;
}


uint64_t hash_int(uint32_t int_value)
{
    uint64_t hash_value = hash_byte(xor_divide(int_value));
    for(uint64_t i = 0; i < c_bits_in_byte * sizeof(uint16_t); i++)
    {
        if((int_value & 0x1) == 0)
        {
            hash_value = hash_value + int_array[i];
        }
        int_value = int_value >> 1;
    }
    return hash_value;
}

uint64_t hash_long(uint64_t long_value)
{
    uint64_t hash_value = hash_byte(xor_divide(long_value));
    for(uint64_t i = 0; i < c_bits_in_byte * sizeof(uint16_t); i++)
    {
        if((long_value & 0x1) == 0)
        {
            hash_value = hash_value + long_array[i];
        }
        long_value = long_value >> 1;
    }
    return hash_value;
}

uint64_t mod_exp(uint64_t base, uint64_t exponent, uint64_t mod)
{
    uint64_t ret_val = base;
    if(exponent == 0)
    {
        ret_val = 1;
    }
    else
    {
        ret_val = mod_exp(base, exponent/2, mod);
        if(exponent % 2 == 0)
        {
            ret_val = (ret_val * ret_val) % mod;
        }
        else
        {
            ret_val = (base * ret_val * ret_val) % mod;
        }
    }
    return ret_val;
}

uint8_t  hash_byte_seed  (uint8_t byte,   uint8_t initial, uint64_t seed)
{
    uint8_t current_value = initial;
    uint8_t input_copy = byte;
    uint8_t* hashes = reinterpret_cast<uint8_t*>(&seed);
    for(uint32_t i = 0; i < c_bits_in_byte; i++)
    {
        if((input_copy & 0x1) == 0)
        {
            current_value = current_value + hashes[i];
        }
        input_copy = input_copy >> 1;
    }
    return current_value;
}

uint16_t hash_short_seed (uint16_t input, uint8_t initial, uint64_t seed)
{
    uint16_t hashes [c_bits_in_short];
    generate_short_hash_bytes(initial, seed, hashes);
    return hash_short_seed(input, hashes);
}

uint32_t hash_int_seed   (uint32_t input, uint8_t initial, uint64_t seed)
{
    uint32_t hashes [c_bits_in_int];
    generate_int_hash_bytes(initial, seed, hashes);
    return hash_int_seed(input, hashes);
}

uint64_t hash_long_seed  (uint64_t input, uint8_t initial, uint64_t seed)
{
    uint64_t hashes [c_bits_in_long];
    generate_long_hash_bytes(initial, seed, hashes);
    return hash_long_seed(input, hashes);
}

void generate_short_hash_bytes(uint8_t initial, uint64_t seed, uint16_t (&hashes)[c_bits_in_short])
{
    uint8_t current = xor_divide(xor_divide(xor_divide(seed)));
    for(uint32_t i = 0; i < c_bits_in_short; i++)
    {
        uint16_t next_byte_low = hash_byte_seed(current, current + i, seed);
        current = next_byte_low + 1;
        uint16_t next_byte_high = hash_byte_seed(current, current + i, seed);
        current = next_byte_high + i;
        hashes[i] = ((next_byte_high << static_cast<uint8_t>(c_bits_in_byte)) | next_byte_low);
    }
    uint16_t current_short = initial;
    for(uint32_t i = 0; i < c_bits_in_short; i++)
    {
        hashes[i] = hash_short_seed(current_short, hashes);
        current_short = hashes[i] * hashes[i];
    }
}

void generate_int_hash_bytes(uint8_t initial, uint64_t seed, uint32_t (&hashes)[c_bits_in_int])
{
    uint16_t current = initial;
    for(uint32_t i = 0; i < c_bits_in_short; i++)
    {
        uint32_t next_bytes_low = hash_short_seed(current, initial, seed);
        current = next_bytes_low + 1;
        uint32_t next_bytes_high = hash_short_seed(current, initial, seed);
        current = next_bytes_high + 1;
        hashes[i] = ((next_bytes_high << static_cast<uint8_t>(c_bits_in_short)) | next_bytes_low);
    }
}

void generate_long_hash_bytes(uint8_t initial, uint64_t seed, uint64_t (&hashes)[c_bits_in_long])
{
    uint32_t current = initial;
    for(uint32_t i = 0; i < c_bits_in_short; i++)
    {
        uint64_t next_bytes_low = hash_int_seed(current, initial, seed);
        current = next_bytes_low + 1;
        uint64_t next_bytes_high = hash_int_seed(current, initial, seed);
        current = next_bytes_high + 1;
        hashes[i] = ((next_bytes_high << static_cast<uint8_t>(c_bits_in_int)) | next_bytes_low);
    }
}

uint16_t hash_short_seed (uint16_t input, uint16_t (&hashes)[c_bits_in_short])
{
    uint16_t current_value = input;
    uint16_t input_copy = input;
    for(uint16_t i = 0; i < c_bits_in_short; i++)
    {
        if((input_copy & 0x1) == 0)
        {
            current_value = current_value + hashes[i];
        }
        input_copy = input_copy >> 1;
    }
    return current_value;
}

uint32_t hash_int_seed   (uint32_t input, uint32_t (&hashes)[c_bits_in_int])
{
    uint32_t current_value = input;
    uint32_t input_copy = input;
    for(uint32_t i = 0; i < c_bits_in_int; i++)
    {
        if((input_copy & 0x1) == 0)
        {
            current_value = current_value + hashes[i];
        }
        input_copy = input_copy >> 1;
    }
    return current_value;
}

uint64_t hash_long_seed  (uint64_t input, uint64_t (&hashes)[c_bits_in_long])
{
    uint64_t current_value = input;
    uint64_t input_copy = input;
    for(uint64_t i = 0; i < c_bits_in_long; i++)
    {
        if((input_copy & 0x1) == 0)
        {
            current_value = current_value + hashes[i];
        }
        input_copy = input_copy >> 1;
    }
    return current_value;
}

uint64_t hash_mod_exp(uint64_t value)
{
    return mod_exp(initial_hash, value + 1, 0xffffffffffffffff);
}

uint8_t xor_divide(uint16_t short_value)
{
    return ((short_value & 0xff00) >> 8) ^ (short_value & 0x00ff);
}

uint16_t xor_divide(uint32_t int_value)
{
    return ((int_value & 0xffff0000) >> 16) ^ (int_value & 0x0000ffff);
}

uint32_t xor_divide(uint64_t int_value)
{
    return ((int_value & 0xffffffff00000000) >> 32) ^ (int_value & 0x00000000ffffffff);
}
