#include <iostream>
#include <fstream>
#include "Decoder.h"
#include "BitReadBuffer.h"
#include "timer.h"

int main(int argc, char * argv[]) {
    start_timer ();
    std::string filename;
    if (argc > 1) {
        filename = argv[1];
    }

    if (filename.empty()) {
        auto input = BitReadBuffer(std::cin);
        Decoder decoder(input, std::cout);
        decoder.decode();
    } else {
        std::ifstream input_file(filename, std::ios::binary);
        auto input = BitReadBuffer(input_file);
        Decoder decoder(input, std::cout);
        decoder.decode();
        input_file.close();
    }
    double time = elapsed_time();
    std::cerr << "Expand processing time = " << time << std::endl;
}