#include <iostream>
#include <fstream>
#include "Encoder.h"
#include "timer.h"

int main(int argc, char * argv[]) {
    start_timer ();
    // default option values
    long N = 11; //number of bits to encode window offset
    long L = 4; //number of bits to encode match length
    long S = 3; // number of bits to encode length of literal string
    std::string filename;

    //convert input option(str) to long
    for (int i = 1; i < argc; ++i) {
        if (argv[i][0] == '-') {
            switch (argv[i][1]) {
                case 'N':
                    N = strtol(&argv[i][3], nullptr, 10);
                    if (N < 9 || N > 14) {
                        fprintf(stderr, "Invalid N: %s\n", &argv[i][2]);
                    }
                    break;
                case 'L':
                    L = strtol(&argv[i][3], nullptr, 10);
                    if (L < 3 || L > 4) {
                        fprintf(stderr, "Invalid L: %s\n", &argv[i][2]);
                    }
                    break;
                case 'S':
                    S = strtol(&argv[i][3], nullptr, 10);
                    if (S < 1 || S > 5) {
                        fprintf(stderr, "Invalid S: %s\n", &argv[i][2]);
                    }
                    break;
                default:
                    break;
            }
        } else {
            filename = argv[i];
        }
    }

    // information to stderr
    std::cerr << "N: " << N << std::endl;
    std::cerr << "L: " << L << std::endl;
    std::cerr << "S: " << S << std::endl;
    BitWriteBuffer output(std::cout);

    if (filename.empty()) {
        Encoder encoder(static_cast<unsigned int>(N),
                        static_cast<unsigned int>(L),
                        static_cast<unsigned int>(S),
                        std::cin,
                        output);
        encoder.encode();
    } else {
        std::ifstream input_file(filename, std::ios::binary);
        Encoder encoder(static_cast<unsigned int>(N),
                        static_cast<unsigned int>(L),
                        static_cast<unsigned int>(S),
                        input_file,
                        output);
        encoder.encode();
        input_file.close();
    }

    double time = elapsed_time();
    std::cerr << "Compression processing time = " << time << std::endl;
}