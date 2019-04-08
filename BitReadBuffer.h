#ifndef LZ_BITREADBUFFER_H
#define LZ_BITREADBUFFER_H

#include <cstdint>
#include <istream>

class BitReadBuffer {
private:
    uint32_t readBitBuffer = 0;// a place to hold available bits
    unsigned int currBuffInd = 0; //how many available bits in buffer already
    std::istream& istream;

public:
    BitReadBuffer(std::istream &istream);
    uint16_t read(unsigned int lengthOfBits);
};

#endif //LZ_BITREADBUFFER_H
