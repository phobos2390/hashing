/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   I_byte_hasher.h
 * Author: James
 *
 * Created on October 11, 2020, 12:17 PM
 */

#include <stdint.h>

#ifndef I_BYTE_HASHER_H
#define I_BYTE_HASHER_H

class I_byte_hasher
{
public:
    I_byte_hasher(){}
    virtual ~I_byte_hasher(){}
    virtual uint8_t perform_hash(uint8_t input)=0;
    virtual void set_seed(uint64_t seed)=0;
};

#endif /* I_BYTE_HASHER_H */

