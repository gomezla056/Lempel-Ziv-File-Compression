#ifndef LZ_DECODER_H
#define LZ_DECODER_H
#include "BitReadBuffer.h"

class Decoder {
private:
    unsigned int N; //number of bits to encode window offset
    unsigned int L; //number of bits to encode match length
    unsigned int S; //number of bits to encode length of literal string
    unsigned int W; //the window size, W, which is 2^N
    unsigned int F; //the lookahead buffer size, F, which is 2^L-1.

    BitReadBuffer &input;
    std::ostream &ostream;

public:
    Decoder(BitReadBuffer &input, std::ostream &ostream);

    void decode();
};


#endif //LZ_DECODER_H
