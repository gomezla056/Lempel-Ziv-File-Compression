#include "BitWriteBuffer.h"

//constructor; receive an input stream to decode
BitWriteBuffer::BitWriteBuffer(std::ostream &ostream) : ostream(ostream) {}

void BitWriteBuffer::insertIntoBuffer(uint16_t input, unsigned int lengthOfBits) {
/*  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *
 *  express input in certain num of bits, then put them into buffer to construct a new byte(a char)
 *
 *  uint16_t input: force input to use 16 bits
 *  unsigned int lengthOfBits:
 *
 *  writeBitBuffer will be updated after new byte is constructed or just be added in more bits
 *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  */

    // input has to be able to be expressed by 2^ lengthOfBits.
    assert(input < pow(2, lengthOfBits));


    unsigned int remainingSpace = 8 - currBuffInd; // remainingSpace = how many bits can current writeBitBuffer hold
    unsigned int remainigLengthOfBits = lengthOfBits; // remainigLengthOfBits = how many bits we need to insert now
    unsigned int currInput = 0;

    // how many bits the input is using, get rid of leading 0s
    input = input << (16 - lengthOfBits);

    // if total bits is more than 8, we need to put a new char into writeBitBuffers array
    while (remainigLengthOfBits >= remainingSpace) {

        //currInput = the most significant bits we can put into the buffer
        currInput = input >> (8 - remainingSpace);
        currInput >>= 8; //fill up currInput with leading 0s
        //remove what we want to insert into buffer from input
        input = input << remainingSpace;

        // fill up writeBitBuffer by OR
        writeBitBuffer = writeBitBuffer | (unsigned char)currInput; // force currInput to be 8 bits
        writeBitBuffers[currBuffArrInd] = writeBitBuffer;

        // update everything
        remainigLengthOfBits  = remainigLengthOfBits - remainingSpace;
        writeBitBuffer = 0;
        currBuffInd = 0;
        remainingSpace = 8;
        currBuffArrInd++;

        //write to file when buffer array is full;
        if (currBuffArrInd == 1000) {
            writeToFile(false);
        }
    }

    // if total bits is less than 8 or we finished insertion
    if (remainigLengthOfBits < remainingSpace && remainigLengthOfBits != 0) {
        currInput = input >> (8 - remainingSpace);
        currInput >>= 8;
        writeBitBuffer = writeBitBuffer | (unsigned char)currInput;
        currBuffInd = currBuffInd + remainigLengthOfBits;
    }

}

void BitWriteBuffer::writeToFile(bool writePartialByte) {
/*  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *
 *  put the content of current buffer array into output stream
 *
 *  bool writePartialByte: if there is remaining bits in the buffer
 *
 *  output stream and currBuffInd will be updated. writeBitBuffer will be cleared.
 *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  */

    ostream.write(reinterpret_cast<const char *>(writeBitBuffers), currBuffArrInd);
    outputLength += currBuffArrInd;
    currBuffArrInd = 0;
    // if these are the last bits of the whole stream
    if (writePartialByte && currBuffInd > 0) {
        ostream.write(reinterpret_cast<const char *>(&writeBitBuffer), 1);
        writeBitBuffer = 0;
        currBuffInd = 0;
        outputLength += 1;
    }
}

BitWriteBuffer::~BitWriteBuffer() {
    //if there are several bits remaining in the buffer
    writeToFile(true);

}



