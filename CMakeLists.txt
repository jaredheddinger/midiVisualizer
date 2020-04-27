cmake_minimum_required(VERSION 2.8.9)

project (midiVisualizer)

set(CMAKE_CXX_STANDARD 14)

add_executable(midiVisualizer Source/main.cpp Source/midiFile.cpp Source/chunk.cpp Source/event.cpp Source/variableLength.cpp)
