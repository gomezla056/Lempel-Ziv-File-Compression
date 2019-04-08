#include "BitReadBuffer.h"

//constructor; receive an input stream to decode
BitReadBuffer::BitReadBuffer(std::istream &istream) : istream(istream) {}

uint16_t BitReadBuffer::read(unsigned int lengthOfBits) {
/*  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *
 *  Read specific num of bits from input stream
 *
 *  unsigned int lengthOfBits : how many bits we want to get from input stream
 *
 *  return an int. Use 16 bits since the largest num of bits to be used is 14
 *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  */

    uint16_t result = 0;

    // curr buffer doesn't have enough bits
    while(currBuffInd <= lengthOfBits) {
        // read one byte from istream
        char temp;
        istream.read(&temp, 1);
        // convert it to an int
        uint32_t tempInt = static_cast<uint32_t>(static_cast<unsigned char>(temp));//possible to have more than 16 bits in the buffer, use 32 bits
        //shift(shift left 24 to the most significant then shift right currBuffInd to correct location
        tempInt = tempInt << (24 - currBuffInd);
        // then OR to put it into readBitBuffer
        readBitBuffer = readBitBuffer | tempInt;
        //increase currBuffInd by one byte(8 bits)
        currBuffInd = currBuffInd + 8;
    }

    //now currBuffInd >= lengthOfBits, we can get bits from buffer directly, and use the most significant bits
    result = (readBitBuffer >> (32 - lengthOfBits));
    currBuffInd = currBuffInd - lengthOfBits;
    readBitBuffer = readBitBuffer << lengthOfBits;

    return result;
}
