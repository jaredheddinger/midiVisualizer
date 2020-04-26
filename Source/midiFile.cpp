//
//  midiFile.cpp
//  midiVisualizer
//
//  Created by Jared Heddinger on 10/26/18.
//  Copyright © 2018 Jared Heddinger. All rights reserved.
//

#include "midiFile.h"
#include "chunk.h"
#include "variableLength.h"
#include <fstream>
#include <iostream>
#include <cmath>

midiFile::midiFile() {}

midiFile::midiFile(std::fstream &fileIn) : headerChunk(fileIn) {
    numTracks = headerChunk.data[1];
    masterData = headerChunk.charCopy;
    for(int i = 0; i < numTracks; ++i) {
        trackChunks.emplace_back(fileIn);
    }
}

void midiFile::printMaster() {
    std::cout << '\n';
    int trackNum = 1;
    for (auto &track : trackChunks) {
        std::cout << "Track " << trackNum << "\n";
        for (auto event : track.events) {
//            for (int i = 0; i < event.chars.size(); ++i) {
//                std::cout << std::hex << int(event.chars[i]) << ' ';
//            }
            std::cout << event->print() << "\n";
//            for (auto c: event.chars) {
//                std::cout << std::hex << __uint8_t(c) << " ";
//            }
//            std::cout << '\n';
        }
        ++trackNum;
        std::cout << '\n';
    }
}

void midiFile::printSequentially() {
    std::cout << '\n' << std::string(80, '=') << '\n';
//    __int32_t max = trackChunks.front().events.back()->time;
//    auto t = trackChunks.front().events.back();
//    std::vector<std::list<Event*>::iterator> trackIterators;
//    std::vector<std::list<Event*>::iterator> trackEnds;
//    for (auto i = trackChunks.begin(); i != trackChunks.end(); ++i) {
//        std::list<Event*>::iterator t = i->events.begin();
//        trackIterators.push_back(t);
//        std::list<Event*>::iterator t2 = i->events.end();
//        trackEnds.push_back(t2);
//    }
    __int32_t max = std::max_element(trackChunks.begin(), trackChunks.end(),
                             [](const trackChunk& c1, const trackChunk& c2) {
                                 return c1.endTime < c2.endTime;
                             })->endTime;
    int maxSize = 1 + int(std::log10(max));
    for (int i = 0; i <= max; ++i) {
        int iSize = i == 0 ? 0 : (1 + int(std::log10(i)));
//        std::cout << i << std::string(maxSize - iSize, ' ') << '\n';
        bool printTicks = false;
//        while (running) {
        for (auto j = trackChunks.begin(); j != trackChunks.end(); ++j) {
            bool start = true;
            for (auto k = j->events.begin(); k != j->events.end(); ++k) {
                if ((*k)->absoluteTime == i) {
                    if (!printTicks) {
                        printTicks = true;
                        std::cout << i << std::string(maxSize - iSize, ' ') << '\n';
                    }
                    if (start) {
                        std::cout << std::string(2, ' ') << "Track " << std::distance(trackChunks.begin(), j)+1 << ":\n";
                        start = false;
                    }
                    std::cout << std::string(maxSize, ' ') << (*k)->print() << '\n';
                }
            }
        }
//        }
    }
}

void midiFile::save(std::string filename) {
    std::string file = filename + ".mid";
    std::fstream fileOut(file, std::fstream::out| std::fstream::binary);
    
    //headerTrack
    __uint8_t head[] = {'M','T','h','d'};
    __uint8_t track[] = {'M','T','r','k'};
    fileOut << std::hex << head;
    __uint8_t length[] = {0x00,0x00,0x00,0x06,0x00};
    for (auto c: length) {fileOut.put(c);}
    
    fileOut.put(__uint8_t(this->headerChunk.data[0]));
    fileOut.put(0x00);
    fileOut.put(__uint8_t(this->headerChunk.data[1]));
    fileOut.put((this->headerChunk.data[2] & 0x0000FF00) >> 8);
    fileOut.put(this->headerChunk.data[2] & 0x000000FF);
    
    //trackChunks
    for (auto beg = trackChunks.begin(); beg != trackChunks.end(); ++beg) {
        for (auto c: track) {fileOut.put(c);}
        fileOut.put(((*beg).length & 0xFF000000) >> 24);
        fileOut.put(((*beg).length & 0x00FF0000) >> 16);
        fileOut.put(((*beg).length & 0x0000FF00) >> 8);
        fileOut.put((*beg).length & 0x000000FF);
        __uint8_t status = 0;
        for (auto &event: (*beg).events) {
            if (status == 0xFF) {status = 0;}
            variableLengthChar(fileOut, event->time);
            status = event->getChars(fileOut, status);
//            break;
        }
    }
}
