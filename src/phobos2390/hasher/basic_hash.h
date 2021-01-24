#include <stdint.h>
#include <stddef.h>

const uint32_t c_bits_in_byte  = 8;
const uint32_t c_bits_in_short = c_bits_in_byte * sizeof(uint16_t);
const uint32_t c_bits_in_int   = c_bits_in_byte * sizeof(uint32_t);
const uint32_t c_bits_in_long  = c_bits_in_byte * sizeof(uint64_t);

uint64_t hash_byte  (uint8_t  byte);
uint64_t hash_short (uint16_t byte);
uint64_t hash_int   (uint32_t byte);
uint64_t hash_long  (uint64_t byte);

uint8_t  hash_byte_seed  (uint8_t  byte,  uint8_t initial, uint64_t seed);
uint16_t hash_short_seed (uint16_t input, uint8_t initial, uint64_t seed);
uint32_t hash_int_seed   (uint32_t input, uint8_t initial, uint64_t seed);
uint64_t hash_long_seed  (uint64_t input, uint8_t initial, uint64_t seed);

void generate_short_hash_bytes (uint8_t initial, uint64_t seed, uint16_t (&hashes)[c_bits_in_short]);
void generate_int_hash_bytes   (uint8_t initial, uint64_t seed, uint32_t (&hashes)[c_bits_in_int]);
void generate_long_hash_bytes  (uint8_t initial, uint64_t seed, uint64_t (&hashes)[c_bits_in_long]);

uint16_t hash_short_seed (uint16_t input, uint16_t (&hashes)[c_bits_in_short]);
uint32_t hash_int_seed   (uint32_t input, uint32_t (&hashes)[c_bits_in_int]);
uint64_t hash_long_seed  (uint64_t input, uint64_t (&hashes)[c_bits_in_long]);

uint8_t  xor_divide(uint16_t short_value);
uint16_t xor_divide(uint32_t short_value);
uint32_t xor_divide(uint64_t short_value);

uint64_t hash_mod_exp(uint64_t value);
