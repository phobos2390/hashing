#include <hasher/digest.h>

struct Digest::Impl
{
    Impl(size_t /*num_bytes*/){}
    Impl(uint8_t* /*value*/, size_t /*num_bytes*/){}
    Impl(Digest& /*copy*/){}
    ~Impl(){}
};

Digest::Digest(size_t /*num_bytes*/){}
Digest::Digest(uint8_t* /*value*/, size_t /*num_bytes*/){}
Digest::Digest(Digest& /*copy*/){}
Digest::~Digest(){}

Digest Digest::operator|(Digest& other)
{
    return other;
}

Digest Digest::operator^(Digest& other)
{
    return other;
}

Digest Digest::operator<<(Digest& other)
{
    return other;
}

Digest Digest::operator>>(Digest& other)
{
    return other;
}
