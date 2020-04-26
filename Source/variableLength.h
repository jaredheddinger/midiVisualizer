//
//  variableLength.hpp
//  midiVisualizer
//
//  Created by Jared Heddinger on 10/26/18.
//  Copyright © 2018 Jared Heddinger. All rights reserved.
//

#ifndef variableLength_h
#define variableLength_h

#include <stdio.h>
#include <fstream>

__int32_t variableLengthNum(std::fstream &fileIn);
void variableLengthChar(std::fstream &fileOut, __int32_t input);

#endif /* variableLength_hpp */
