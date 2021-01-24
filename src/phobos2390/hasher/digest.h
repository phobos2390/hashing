#include <stdint.h>
#include <stddef.h>

class Digest
{
public:
    Digest(size_t num_bytes);
    Digest(uint8_t* value, size_t num_bytes);
    Digest(Digest& copy);
    ~Digest();
    
    Digest operator|(Digest& other);
    Digest operator^(Digest& other);
    Digest operator<<(Digest& other);
    Digest operator>>(Digest& other);
private:
    struct Impl;
    Impl* m_p_impl;
};