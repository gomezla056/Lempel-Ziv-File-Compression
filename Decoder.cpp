#include <cmath>
#include <vector>
#include "Decoder.h"

Decoder::Decoder(BitReadBuffer &input, std::ostream &ostream) : input(input), ostream(ostream) {
    N = static_cast<unsigned int>(input.read(8));
    L = static_cast<unsigned int>(input.read(8));
    S = static_cast<unsigned int>(input.read(8));
    W = static_cast<unsigned int>(pow(2, N));
    F = static_cast<unsigned int>(pow(2, L));
}

void Decoder::decode() {
    std::vector<char> buffer; // a giant buffer to hold all the output
    do {
        // the first is always L, number of bits to encode match length
        uint16_t len = input.read(L);
        if (len >= 2) { // It is a match
            uint16_t offset = input.read(N);// next one is N, number of bits to encode window offset
            long begin = buffer.size() - offset; // get back into buffer to find matched chars
            for (uint16_t i = 0; i < len; i++) {
                buffer.push_back(buffer[begin + i]);
            }
        } else {
            uint16_t strlen = input.read(S); // next one is num of chars.
            if (strlen == 0) {
                break; // the end
            }
            for (uint16_t i = 0; i < strlen; i++) {
                auto c = static_cast<unsigned char>(input.read(8)); // push them to buffer one by one
                buffer.push_back(c);
            }
        }
    } while (true);

    ostream.write(buffer.data(), buffer.size());
}