//
//  midiFile.hpp
//  midiVisualizer
//
//  Created by Jared Heddinger on 10/26/18.
//  Copyright © 2018 Jared Heddinger. All rights reserved.
//

#ifndef midiFile_h
#define midiFile_h

#include <stdio.h>
#include <fstream>
#include <list>
#include <vector>
#include "chunk.h"

class midiFile {
    //number of tracks (aka trackChunks)
    __int16_t numTracks;
    
    //sole headerChunk class
    headerChunk headerChunk;
    
public:
    //std::list of trackChunk objects
    std::list<trackChunk> trackChunks;
    
    //vector of input chars - not currently used
    std::vector<char> masterData;
    
public:
    //default initializer
    midiFile();
    
    //filestream initializer
    midiFile(std::fstream &fileIn);
    
    //prints all messages as they enter the engine
    void printMaster();
    
    //prints all messages according to their absolute time in the file
    void printSequentially();
    
    //outputs all the data to a given filename, keeps original formatting
    void save(std::string filename);
};

#endif /* midiFile_hpp */
