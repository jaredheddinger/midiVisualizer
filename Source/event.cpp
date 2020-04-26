//
//  event.cpp
//  midiVisualizer
//
//  Created by Jared Heddinger on 10/18/18.
//  Copyright © 2018 Jared Heddinger. All rights reserved.
//

#include "event.h"
#include "variableLength.h"
#include <iostream>
#include <array>
#include <string>
#include <sstream>
#include <cmath>

std::vector<std::string> noteNames = {"C","C#","D","Eb","E","F","F#","G","Ab","A","Bb","B"};
std::string noteString(__int8_t input) {
    std::stringstream output;
    output << noteNames[input%12] << (input/12)-1;
    return output.str();
}

Event::Event() {}
Event::Event(std::fstream &fileIn, __int8_t currentStatus) {
    name = "ERRORNAME";
}
std::string Event::getName() {
    return this->name;
}
std::string Event::print() {
    return "";
}
char Event::get(std::fstream &fileIn) {
    chars.push_back(fileIn.get());
    return chars.back();
}
void Event::changeData(__int32_t input) {
    return;
}
void Event::editMessage() {
    return;
}

__int32_t Event::getTime() {
    return time;
}

__int32_t Event::getAbsoluteTime() {
    return absoluteTime;
}

char Event::getChars(std::fstream &fileOut, char status) {
    int i = 0;
    if (chars[0] == status) {
        i = 1;
    }
    for (; i < chars.size(); ++i) {
        fileOut.put(chars[i]);
    }
    return chars[0];
}

MIDIEvent::MIDIEvent() {}
MIDIEvent::MIDIEvent(std::fstream &fileIn) {
    name = "ERRORNAME - MIDIEVENT";
}
std::string MIDIEvent::print() {
    return name;
}

NoteOff::NoteOff(__int32_t time, std::fstream &fileIn, __int8_t channel) {
    name = "NoteOff";
    this->time = time;
    this->channel = channel;
    this->absoluteTime = 0;
    chars.push_back(0x80 + channel);
    note = get(fileIn);
    velocity = get(fileIn);
    velocity = 0;
}
std::string NoteOff::print() {
    std::stringstream output;
    output << name << ": " << noteString(note) << " velocity: "
    << std::to_string(velocity);
    return output.str();
}

NoteOn::NoteOn(__int32_t time, std::fstream &fileIn, __int8_t channel) {
    name = "NoteOn";
    this->time = time;
    this->channel = channel;
    this->absoluteTime = 0;
    chars.push_back(0x90 + channel);
    note = get(fileIn);
    velocity = get(fileIn);
}
std::string NoteOn::print() {
    std::stringstream output;
    output << name << ": " << noteString(note) << " velocity: "
    << int(velocity);
    return output.str();
}

PolyphonicKeyPressure::PolyphonicKeyPressure(__int32_t time, std::fstream &fileIn, __int8_t channel) {
    name = "PolyphonicKeyPressure";
    this->time = time;
    this->channel = channel;
    this->absoluteTime = 0;
    chars.push_back(0xA0 + channel);
    note = get(fileIn);
    pressure = get(fileIn);
}
std::string PolyphonicKeyPressure::print() {
    return name;
}

ControlChange::ControlChange(__int32_t time, std::fstream &fileIn, __int8_t channel) {
    name = "ControllerChange";
    this->time = time;
    this->channel = channel;
    this->absoluteTime = 0;
    chars.push_back(0xB0 + channel);
    controllerNumber = get(fileIn);
    switch (controllerNumber) {
        case 7: {controllerName = "Track Volume"; break;}
        case 10: {controllerName = "Pan"; break;}
        case 64: {controllerName = "Sustain Pedal"; break;}
        case 91: {controllerName = "Effect 1 Depth"; break;}
        case 93: {controllerName = "Effect 3 Depth"; break;}
        case 121: {controllerName = "Reset Controllers"; break;}
        default: {controllerName = std::to_string(controllerNumber); break;}
    }
    value = get(fileIn);
}
std::string ControlChange::print() {
    std::stringstream output;
    output << "Controller: " << controllerName << ", Value = " << int(value);
    return output.str();
}
void ControlChange::editMessage() {
    std::cout << "Value? ";
    int value;
    std::cin >> value;
    this->value = value;
}

ProgramChange::ProgramChange(__int32_t time, std::fstream &fileIn, __int8_t channel) {
    name = "ProgramChange";
    this->time = time;
    this->channel = channel;
    this->absoluteTime = 0;
    chars.push_back(0xC0 + channel);
    programNumber = get(fileIn);
    switch (programNumber) {
        case 0: {programName = "Piano"; break;}
        default: {programName = std::to_string(programNumber); break;}
    }
}
std::string ProgramChange::print() {
    std::stringstream output;
    output << name << ": " << "Program " << programName;
    return output.str();
}

ChannelPressure::ChannelPressure(__int32_t time, std::fstream &fileIn, __int8_t channel) {
    name = "ChannelPressure";
    this->time = time;
    this->channel = channel;
    this->absoluteTime = 0;
    chars.push_back(0xD0 + channel);
    pressureValue = get(fileIn);
}
std::string ChannelPressure::print() {
    return name;
}

PitchWheel::PitchWheel(__int32_t time, std::fstream &fileIn, __int8_t channel) {
    name = "PitchWheel";
    this->time = time;
    this->channel = channel;
    this->absoluteTime = 0;
    chars.push_back(0xE0 + channel);
    leastSig = get(fileIn);
    mostSig = get(fileIn);
}
std::string PitchWheel::print() {
    return name;
}

MetaEvent::MetaEvent() {
//    this->length = variableLengthNum
    this->status = 0xFF;
    chars.push_back(0xFF);
}
MetaEvent::MetaEvent(std::fstream &fileIn) {
    name = "ERRORNAME - METAEVENT";
}
std::string MetaEvent::print() {
    return name;
}

SequenceNumber::SequenceNumber(__int32_t time, std::fstream &fileIn) {
    this->time = time;
    this->metaByte = 0x00;
    this->absoluteTime = 0;
    length = get(fileIn);
        //    this->length = variableLengthNum(fileIn);
    name = "SequenceNumber";
    sequence1 = get(fileIn);
    sequence2 = get(fileIn);
}
std::string SequenceNumber::print() {
    return name;
}

TrackName::TrackName(__int32_t time, std::fstream &fileIn) {
    this->time = time;
    this->metaByte = 0x00;
    this->absoluteTime = 0;
    length = get(fileIn);
        //    this->length = variableLengthNum(fileIn);
    name = "TrackName";
    for (int i = 0; i < length; ++i) {
        stream.push_back(get(fileIn));
    }
    text = std::string(stream.begin(), stream.end());
}
std::string TrackName::print() {
    std::stringstream output;
    output << "TrackName" << ": " << text;
    return output.str();
}

MIDIPort::MIDIPort(__int32_t time, std::fstream &fileIn) {
    this->time = time;
    this->metaByte = 0x21;
    this->absoluteTime = 0;
    chars.push_back(0x21);
    length = get(fileIn);
        //    this->length = variableLengthNum(fileIn);
    name = "MIDIPort";
    port = get(fileIn);
}
std::string MIDIPort::print() {
    return name;
}

EndOfTrack::EndOfTrack(__int32_t time) {
    chars.push_back(0x2F);
    chars.push_back(0x00);
    this->absoluteTime = 0;
    this->time = time;
    name = "EndOfTrack";
}
std::string EndOfTrack::print() {
    return name;
}

SetTempo::SetTempo(__int32_t time, std::fstream &fileIn) {
    this->time = time;
    this->metaByte = 0x51;
    this->absoluteTime = 0;
    chars.push_back(0x51);
    length = get(fileIn);
        //    this->length = variableLengthNum(fileIn);
    name = "SetTempo";
    msPerQuarter = 0;
    msPerQuarter += get(fileIn);
    msPerQuarter <<= 8;
    msPerQuarter += get(fileIn);
    msPerQuarter <<= 8;
    msPerQuarter += get(fileIn);
    BPM = 60000000 / msPerQuarter;
}
std::string SetTempo::print() {
    std::stringstream output;
    output << "Tempo: " << BPM << " BPM";
    return output.str();
}

TimeSignature::TimeSignature(__int32_t time, std::fstream &fileIn) {
    this->time = time;
    this->metaByte = 0x58;
    this->absoluteTime = 0;
    chars.push_back(0x58);
    length = get(fileIn);
//    this->length = variableLengthNum(fileIn);
    name = "TimeSignature";
    numerator = get(fileIn);
    denominator = get(fileIn);
    clocks = get(fileIn);
    notesInQuarter = get(fileIn);
}
std::string TimeSignature::print() {
    std::stringstream output;
    output << name << ": " << int(numerator) << "/" << pow(2, denominator) << " ("
        << int(clocks) << " clocks per metronome click, " << int(notesInQuarter)
        << " notes in quarter note)";
    return output.str();
}

KeySignature::KeySignature(__int32_t time, std::fstream &fileIn) {
    this->time = time;
    this->metaByte = 0x59;
    this->absoluteTime = 0;
    chars.push_back(0x59);
    length = get(fileIn);
        //    this->length = variableLengthNum(fileIn);
    name = "KeySignature";
    sharps = get(fileIn);
    minor = get(fileIn);
}
std::string KeySignature::print() {
    std::stringstream output;
    output << name << ": " << noteNames[(7*(int(sharps)+12))%12] << " " << (minor ? "minor" : "major");
    return output.str();
}
void KeySignature::changeData(__int32_t numSharps) {
    sharps = numSharps;
}
void KeySignature::changeKey(__int8_t numSharps) {
    sharps = numSharps;
}
void KeySignature::editMessage() {
    std::cout << "How many sharps in the new Key Signature? ";
    int numSharps;
    std::cin >> numSharps;
    std::cout << "Major/Minor? ";
    std::string min;
    std::cin >> min;
    transform(min.begin(), min.end(), min.begin(), ::tolower);
    sharps = numSharps;
    minor = min == "minor" ? 1 : 0;
    chars[3] = numSharps;
    chars[4] = minor;
}
