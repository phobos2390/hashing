/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   I_hash_seeder.h
 * Author: James
 *
 * Created on October 11, 2020, 12:25 PM
 */

#include <stdint.h>
#include <cstring>

#ifndef I_HASH_SEEDER_H
#define I_HASH_SEEDER_H

class I_hash_seeder
{
public:
    I_hash_seeder(){}
    virtual ~I_hash_seeder(){}
    virtual void generate_numbers(uint64_t seed, uint8_t* p_array_start, size_t array_size)=0;
};

#endif /* I_HASH_SEEDER_H */

