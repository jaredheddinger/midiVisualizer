//
//  event.hpp
//  midiVisualizer
//
//  Created by Jared Heddinger on 10/18/18.
//  Copyright © 2018 Jared Heddinger. All rights reserved.
//

#ifndef event_h
#define event_h

#include <stdio.h>
#include <fstream>
#include <vector>
#include <string>
#include <array>

std::string noteString(__int8_t input);

class Event {
protected:
    __int8_t status;
    __int32_t time;
    std::vector<char> chars;
    std::string name;
public:
    friend class midiFile;
    friend class Chunk;
    __int32_t absoluteTime;
//    friend class chunk;
    Event();
    Event(std::fstream &fileIn, __int8_t currentStatus);
    virtual ~Event() = default;
    virtual std::string print();
    char get(std::fstream &fileIn);
    std::string getName();
    __int32_t getTime();
    __int32_t getAbsoluteTime();
    virtual void changeData(__int32_t input);
    virtual void editMessage();
    char getChars(std::fstream &fileOut, char status);
//    virtual char outputChars(std::fstream &fileOut, __uint8_t status);
};

    class MIDIEvent : public Event {
    protected:
        __int8_t channel;
    public:
        MIDIEvent();
        MIDIEvent(std::fstream &fileIn);
        virtual std::string print();
    };

        class NoteOff : public MIDIEvent {
        private:
            __int8_t note;
            __int8_t velocity;
        public:
            NoteOff(__int32_t time, std::fstream &fileIn, __int8_t channel);
            std::string print();
        };

        class NoteOn : public MIDIEvent {
        private:
            __int8_t note;
            __int8_t velocity;
        public:
            NoteOn(__int32_t time, std::fstream &fileIn, __int8_t channel);
            std::string print();
        };

        class PolyphonicKeyPressure : public MIDIEvent {
        private:
            __int8_t note;
            __int8_t pressure;
        public:
            PolyphonicKeyPressure(__int32_t time, std::fstream &fileIn, __int8_t channel);
            std::string print();
        };

        class ControlChange : public MIDIEvent {
        private:
            __int8_t controllerNumber;
            __int8_t value;
            std::string controllerName;
        public:
            ControlChange(__int32_t time, std::fstream &fileIn, __int8_t channel);
            std::string print();
            void editMessage();
        };

        class ProgramChange : public MIDIEvent {
        private:
            __int8_t programNumber;
            std::string programName;
        public:
            ProgramChange(__int32_t time, std::fstream &fileIn, __int8_t channel);
            std::string print();
        };

        class ChannelPressure : public MIDIEvent {
        private:
            __int8_t pressureValue;
        public:
            ChannelPressure(__int32_t time, std::fstream &fileIn, __int8_t channel);
            std::string print();
        };

        class PitchWheel : public MIDIEvent {
        private:
            __int8_t leastSig;
            __int8_t mostSig;
        public:
            PitchWheel(__int32_t time, std::fstream &fileIn, __int8_t channel);
            std::string print();
        };

    class SysCommonEvent : public Event {
        SysCommonEvent(std::fstream &fileIn);
    };

    class MetaEvent : public Event {
    protected:
        __int8_t metaByte;
        __int32_t length;
    public:
        MetaEvent();
        MetaEvent(std::fstream &fileIn);
        virtual std::string print();
    };

        class SequenceNumber : public MetaEvent {
        private:
            __int8_t sequence1;
            __int8_t sequence2;
        public:
            SequenceNumber(__int32_t time, std::fstream &fileIn);
            std::string print();
        };

        class TrackName : public MetaEvent {
            private:
            std::vector<char> stream;
            std::string text;
            public:
            TrackName(__int32_t time, std::fstream &fileIn);
            std::string print();
        };

        class MIDIPort : public MetaEvent {
        private:
            __int8_t port;
        public:
            MIDIPort(__int32_t time, std::fstream &fileIn);
            std::string print();
        };

        class EndOfTrack : public MetaEvent {
        public:
            __int32_t time;
        
            EndOfTrack(__int32_t time);
            std::string print();
        };

        class SetTempo : public MetaEvent {
        private:
            __uint16_t BPM;
            __int32_t msPerQuarter;
        public:
            SetTempo(__int32_t time, std::fstream &fileIn);
            std::string print();
        };

        class TimeSignature : public MetaEvent {
        private:
            __int8_t numerator;
            __int8_t denominator;
            __int8_t clocks;
            __int8_t notesInQuarter;
        public:
            TimeSignature(__int32_t time, std::fstream &fileIn);
            std::string print();
        };

        class KeySignature : public MetaEvent {
        private:
            __int8_t sharps;
            __int8_t minor;
        public:
            KeySignature(__int32_t time, std::fstream &fileIn);
            void changeData(__int32_t numSharps);
            void changeKey(__int8_t numSharps);
            std::string print();
            void editMessage();
//            char outputChars(std::fstream &fileOut, __uint8_t status);
        };



#endif /* event_hpp */
