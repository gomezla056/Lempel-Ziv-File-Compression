#ifndef LZ_ENCODER_H
#define LZ_ENCODER_H

#include <vector>
#include <iostream>
#include <deque>
#include <unordered_map>
#include "BitWriteBuffer.h"

class Encoder {
private:

    //use unsign to make sure auto-filling leading bits will be 0s.
    unsigned int N; //number of bits to encode window offset
    unsigned int L; //number of bits to encode match length
    unsigned int S; //number of bits to encode length of literal string
    unsigned int W; //the window size, W, which is 2^N
    unsigned int F; //the lookahead buffer size, F, which is 2^L-1.
    unsigned int maxStrLength; // 2^S-1

    std::istream &istream;
    BitWriteBuffer &output;

    std::deque<char> window;
    std::vector<unsigned char> charsToBeEncoded;// store the unmatched single chars
    std::unordered_map<std::string, std::vector<long>> map; // store every adjacent two chars as key, their absolute indexes as value
    unsigned long shiftCount = 0;// how far the window has shifted

    long continueEncode();
    void encodeCurrTriple();
    void shiftWindow(long length);
    void insertIntoMap(long length);

public:
    Encoder(unsigned int N, unsigned int L, unsigned int S, std::istream &istream, BitWriteBuffer &output);
    void encode();
    ~Encoder();
};


#endif //LZ_ENCODER_H
