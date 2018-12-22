//
//  chunk.cpp
//  midiVisualizer
//
//  Created by Jared Heddinger on 10/26/18.
//  Copyright © 2018 Jared Heddinger. All rights reserved.
//

#include "chunk.h"
#include "event.h"
#include "variableLength.h"
#include <vector>
#include <fstream>
#include <cassert>
#include <cstring>
#include <iostream>

Chunk::Chunk() : type(), length(0) {}

char Chunk::get(std::fstream &fileIn) {
    charCopy.push_back(fileIn.get());
    return charCopy.back();
}

headerChunk::headerChunk() {}
headerChunk::headerChunk(std::fstream &fileIn) {
//    length = 6;
    for(int i = 0; i < 4; ++i) {
        type[i] = get(fileIn);
    }
    
    for(int i = 0; i < 4; ++i) {
        length += get(fileIn);
        if (i != 3) { length <<= 8; }
    }
    
    for(int i = 0; i < 2; ++i) {
        if (i == 1) { data[0] = fileIn.get(); }
        else { get(fileIn); }
    }
    
    data[1] = 0;
    for(int i = 0; i < 2; ++i) {
        data[1] += get(fileIn);
        if (i != 1) { data[1] <<= 8; }
    }
    
    data[2] = 0;
    __uint8_t c = 0;
    for(int i = 0; i < 2; ++i) {
        c = get(fileIn);
        data[2] += c;
        if (i != 1) { data[2] <<= 8; }
    }
}

trackChunk::trackChunk() {}
trackChunk::trackChunk(std::fstream &fileIn) {
    for(int i = 0; i < 4; ++i) {
        type[i] = get(fileIn);
    }
    for(int i = 0; i < 4; ++i) {
        length += get(fileIn);
        if (i != 3) { length <<= 8; }
    }
    
    bool keepReading = true;
    while(keepReading) {
        keepReading = eventFactory(fileIn);
    }
//    std::cout << "done\n";
}

//trackChunk::~trackChunk() {
//    for (auto item : events) {
//        delete item;
//    }
//}

void trackChunk::addEvent(Event *input) {
    auto pointer = std::shared_ptr<Event>(input);
    auto temp = std::vector<std::shared_ptr<Event>> {pointer};
    auto pair = eventsByName.insert({input->getName(), temp});
    if (!pair.second) {
        pair.first->second.push_back(pointer);
    }
    
}

bool trackChunk::eventFactory(std::fstream &fileIn) {
    if (fileIn.eof()) {
        return false;
    }
    __int32_t time = variableLengthNum(fileIn);
    __uint8_t status = fileIn.peek();
    if (!(status >> 7)) {
        status = runningStatus;
    }
    else {
        fileIn.get();
    }
    switch(status>>4) {
        case 0x08: events.push_back(new NoteOff(time, fileIn, status&0x0F)); break;
        case 0x09: events.push_back(new NoteOn(time, fileIn, status&0x0F)); break;
        case 0x0A: events.push_back(new PolyphonicKeyPressure(time, fileIn,  status&0x0F)); break;
        case 0x0B: events.push_back(new ControlChange(time, fileIn, status&0x0F)); break;
        case 0x0C: events.push_back(new ProgramChange(time, fileIn, status&0x0F)); break;
        case 0x0D: events.push_back(new ChannelPressure(time, fileIn, status&0x0F)); break;
        case 0x0E: events.push_back(new PitchWheel(time, fileIn, status&0x0F)); break;
        case 0x0F: {
            if ((status & 0xF) != 0xF) {
//                events.push_back(SysCommonEvent());
                std::cout << "SysCommon\n";
            }
            else {
                char c = fileIn.get();
                switch (c) {
                    case 0x00: events.push_back(new SequenceNumber(time, fileIn)); break;
                    case 0x03: events.push_back(new TrackName(time, fileIn)); break;
                    case 0x21: events.push_back(new MIDIPort(time, fileIn)); break;
                    case 0x2F: events.push_back(new EndOfTrack(time)); fileIn.get(); return false;
                    case 0x51: events.push_back(new SetTempo(time, fileIn)); break;;
                    case 0x58: events.push_back(new TimeSignature(time, fileIn)); break;
                    case 0x59: events.push_back(new KeySignature(time, fileIn)); break;
                    default: std::cout << std::hex << int(c) << " ErrMeta\n"; break;
                }
            }
            break;
        }
        default: std::cout << int(status) << " ErrStatus\n";
    }
    if (events.size() > 1) {
        int thisTime = events.back()->getTime();
        int prevTime = (*(----events.end()))->getAbsoluteTime();
        events.back()->absoluteTime = prevTime + thisTime;
    }
    addEvent(events.back());
    runningStatus = status;
    endTime = std::max(events.back()->getAbsoluteTime(), endTime);
    return true;
}
