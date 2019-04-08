#ifndef LZ_BITBUFFER_H
#define LZ_BITBUFFER_H

#include <ostream>
#include <cassert>
#include <cmath>

class BitWriteBuffer {
private:
    unsigned char writeBitBuffer = 0; //if not enough to construct a new byte, put bits into buffer
    unsigned char writeBitBuffers[1000]; // an array to hold constructed chars . write to file every 1000 char
    unsigned int currBuffInd = 0; // how many bits in the buffer
    unsigned int currBuffArrInd = 0; // how many bytes in the char array
    std::ostream& ostream;


public:
    long outputLength = 0;
    BitWriteBuffer(std::ostream &ostream);
    ~BitWriteBuffer();
    void insertIntoBuffer(uint16_t input, unsigned int lengthOfBits);
    void writeToFile(bool writePartialByte);
};


#endif //LZ_BITBUFFER_H
