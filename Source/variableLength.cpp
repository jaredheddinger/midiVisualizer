//
//  variableLength.cpp
//  midiVisualizer
//
//  Created by Jared Heddinger on 10/26/18.
//  Copyright © 2018 Jared Heddinger. All rights reserved.
//

#include "variableLength.h"
#include <fstream>
#include <vector>

__int32_t variableLengthNum(std::fstream &fileIn) {
    __uint32_t output = 0;
    __uint8_t c = fileIn.get();
    while (c >> 7) {
        output <<= 7;
        output += (c & 0x7F);
        c = fileIn.get();
    }
    output <<= 7;
    output += (c & 0x7F);
    return output;
}

void variableLengthChar(std::fstream &fileOut, __int32_t input) {
    if (input < 128) {
        fileOut.put(input);
        return;
    }
    __uint32_t mask = 0x0FE00000;
    __uint8_t shift = 21;
    __uint8_t output;
    std::vector<__uint8_t> t;
    while (mask) {
        output = (input & mask) >> shift;
        mask >>= 7;
        shift -= 7;
        if (output) {
            if (mask) {
                output += 128;
            }
            t.push_back(output);
            fileOut.put(output);
        }
    }
    if (t.empty()) {
        t.push_back(0);
        fileOut.put(0);
    }
}
