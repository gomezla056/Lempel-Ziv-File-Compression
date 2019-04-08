#include <cmath>

#include "Encoder.h"

int findMatchedLen(const std::deque<char> &buffer, long index1, long index2, long maxLen) {
    int currMatchedLen = 0;
    while (currMatchedLen < maxLen) {
        // outside of the window
        if (index2 + currMatchedLen >= buffer.size()) {
            break;
        }
        if (buffer[index1 + currMatchedLen] == buffer[index2 + currMatchedLen]) {
            currMatchedLen += 1;
        } else {
            break;
        }
    }
    return currMatchedLen;
}

// constructor
Encoder::Encoder(unsigned int N, unsigned int L, unsigned int S, std::istream &istream, BitWriteBuffer &output)
        : N(N), L(L), S(S), istream(istream), output(output) {
    W = static_cast<unsigned int>(pow(2, N));
    F = static_cast<unsigned int>(pow(2, L) - 1);
    maxStrLength = static_cast<unsigned int>(pow(2, S) - 1);
}

Encoder::~Encoder(){
    std::cerr << "output =  " << output.outputLength << std::endl;
    std::cerr << "total =  " << shiftCount << std::endl;
    std::cerr << "compression savings = " << 1-((double)output.outputLength/(double)shiftCount) << std::endl;
}

void Encoder::encode() {
    //put options(in byte) into output stream
    output.insertIntoBuffer(static_cast<uint16_t>(N), 8);
    output.insertIntoBuffer(static_cast<uint16_t>(L), 8);
    output.insertIntoBuffer(static_cast<uint16_t>(S), 8);

    int offset = 0; // offset = current windows starting point - absolute starting points

    // initialize the window, it has nothing at the beginning, so put 0.
    for (int i = 0; i < W - F; ++i) {
        window.push_back(0);
    }

    // put chars into the window, chars' length is the look ahead buffer's length
    char c;
    for (int i = 0; i < F && istream.get(c); ++i) {
        window.push_back(c);
    }

    //while we are able to move the window, look ahead and check matches!
    while (W - F < window.size()) {
        //return how many chars we encoded
        long windowShiftLength = continueEncode();
        // shift window to new starting place
        shiftWindow(windowShiftLength);
        offset += windowShiftLength;
    }
    //just in case there is some remaining chars
    encodeCurrTriple();
    //encode the ending (0, 0)
    output.insertIntoBuffer(0, L);
    output.insertIntoBuffer(0, N);
}

long Encoder::continueEncode() {
    //initialize everything
    long longestPrefixIndex = -1;
    long longestPrefixLen = 0;
    long lookaheadBufferStartIndex = W - F; //index relative to the start of the window

    if (lookaheadBufferStartIndex < window.size() - 1) { // not the last one
        // construct a key of two chars
        auto key = std::string(&window[lookaheadBufferStartIndex], 2);
        auto it = map.find(key);
        if (it != map.end()) {
            //if key is in the map, get the value. Value is a vector of absolute index representing the location
            const std::vector<long> &candidates = it->second;
            // check every location one by one backwards. (find the nearest match first)
            for (long i = candidates.size() - 1; i >= 0; i--) {
                auto candidate = candidates[i];
                long relativeIndex = candidate - shiftCount; // relativeIndex = index in the window
                if (relativeIndex < 0) {
                    break;// being negative means being outside of the window
                }
                // prefixLen = how many chars matched. We need the longest one
                long prefixLen = findMatchedLen(window, relativeIndex, lookaheadBufferStartIndex, F);
                if (prefixLen > longestPrefixLen) {
                    longestPrefixLen = prefixLen;
                    longestPrefixIndex = relativeIndex;
                }
            }
        }
    }

    if (longestPrefixLen >= 2) {
        // if there is a match, we need to encode 1. curr triple(charsToBeEncoded) 2. current match
        encodeCurrTriple();

        long offset = lookaheadBufferStartIndex - longestPrefixIndex;
        output.insertIntoBuffer(static_cast<uint16_t>(longestPrefixLen), L);
        output.insertIntoBuffer(static_cast<uint16_t>(offset), N);
        return longestPrefixLen;
    } else {
        // if there is no match, we update curr triple(charsToBeEncoded)
        if (charsToBeEncoded.size() == maxStrLength) {
            encodeCurrTriple();
        }
        charsToBeEncoded.push_back(static_cast<unsigned char>(window[lookaheadBufferStartIndex]));
        return 1;
    }
}

void Encoder::encodeCurrTriple() {
    if (!charsToBeEncoded.empty()) {
        output.insertIntoBuffer(0, L);
        output.insertIntoBuffer(static_cast<uint16_t>(charsToBeEncoded.size()), S);
        for (unsigned char c : charsToBeEncoded) {
            output.insertIntoBuffer(static_cast<uint16_t>(c), 8);
        }

        charsToBeEncoded.clear();
    }
}

void Encoder::shiftWindow(long length) {
    char c;
    for (long i = 0; i < length; ++i) {
        window.pop_front();
        if (istream.get(c)) {
            window.push_back(c);
        }
    }
    shiftCount += length;//shiftCount will record how many steps the window has shifted
    insertIntoMap(length);
}

void Encoder::insertIntoMap(long length) {

    long lookaheadBufferStartIndex = W - F;
    //insert this string into the map. Key is every two adjacent chars.
    for (long i = 0; i < length; i++) {
        auto index = lookaheadBufferStartIndex - i - 1; // index relative to the window
        auto key = std::string(&window[index], 2);
        auto absoluteIndex = shiftCount + index; // (index + how far the window has shifted ) =  the absolute index
        auto value = std::vector<long>();
        value.push_back(absoluteIndex);
        auto ret = map.insert(std::pair<std::string, std::vector<long>>(key, value));
        if (!ret.second) { // if key already exists
            ret.first->second.push_back(absoluteIndex);
        }
    }
}