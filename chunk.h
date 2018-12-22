//
//  chunk.hpp
//  midiVisualizer
//
//  Created by Jared Heddinger on 10/26/18.
//  Copyright © 2018 Jared Heddinger. All rights reserved.
//

#ifndef chunk_h
#define chunk_h

#include <stdio.h>
#include <vector>
#include <list>
#include <fstream>
#include "event.h"
#include <unordered_map>



class Chunk {
public:
    //four char cstring - "MThd"
    char type[4];
    
    //length, in messages, of chunk
    __int32_t length;
    
    //master copy of chars, currently unused
    std::vector<char> charCopy;
    
    //default initializer, currently public
    Chunk();
    
    //helper function to get chars while adding them to copy
    char get(std::fstream &fileIn);
};

class headerChunk : public Chunk {
public:
    //default initializer
    headerChunk();
    
    //filestream initializer
    headerChunk(std::fstream &fileIn);
    
    //data object storing 
    __int16_t data[3];
};

class trackChunk : public Chunk {
public:
    __int8_t runningStatus;
    __int32_t endTime = 0;
    std::list<Event*> events;
//    std::unordered_map<__int32_t, std::shared_ptr<Event>> eventsByTime;
    std::unordered_map<std::string, std::vector<std::shared_ptr<Event>>> eventsByName;
    
    
    trackChunk();
    trackChunk(std::fstream &fileIn);
//    ~trackChunk();
    bool eventFactory(std::fstream &fileIn);
    void addEvent(Event*);
    
};

#endif /* chunk_hpp */
