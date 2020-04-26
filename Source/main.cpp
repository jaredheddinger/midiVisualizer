//
//  main.cpp
//  midiVisualizer
//
//  Created by Jared Heddinger on 10/18/18.
//  Copyright © 2018 Jared Heddinger. All rights reserved.
//

#include <fstream>
#include <iostream>
#include <string>
#include "chunk.h"
#include "event.h"
#include "midiFile.h"
#include "variableLength.h"

using namespace std;

int main(int argc, const char * argv[]) {
    string fileName;
    cout << "File: ";
    cin >> fileName;
    if (fileName == "d") {
        fileName = "Flight.mid";
    }
    fstream fileIn(fileName, fstream::in| fstream::binary);

    midiFile file(fileIn);
    

    cout << "Reading: " << fileName << "\n";
    bool run = true;
    while (run) {
        cout << "Action: ";
        char option;
        cin >> option;
        switch(option) {
            case 's': {
                cout << "Search Name: ";
                string searchTerm;
                cin >> searchTerm;
                cout << "Results: " << '\n';
                auto vector = file.trackChunks.back().eventsByName[searchTerm];
                for (int i = 0; i < vector.size(); ++i) {
                    cout << "   [" << i+1 << "] " << vector[i]->print() << "\n";
                }
                
                bool runSearch = 1;
                while (runSearch) {
                    cout << "Would you like to do anything with these?: ";
                    char newOption;
                    cin >> newOption;
                    switch (newOption) {
                        case 'n': {
                            runSearch = 0;
                            break;
                        }
                        case 'e': {
                            int num;
                            cout << "Which event?: ";
                            cin >> num;
                            auto temp = vector[num-1];
                            temp->editMessage();
    //                        int changeValue;
    //                        cin >> changeValue;
    //                        static_pointer_cast<KeySignature>(vector[num-1])->changeKey(changeValue);
                            cout << "Results: " << '\n';
                            for (int i = 0; i < vector.size(); ++i) {
                                cout << "   [" << i+1 << "] " << vector[i]->print() << "\n";
                            }
                            break;
                        }
                        default: {
                            cout << "Error\n";
                            break;
                        }
                    }
                }
                break;
            }
            case 'a': {
                cout << "Name the output file: ";
                std::string fileName;
                cin >> fileName;
                file.save(fileName);
                break;
            }
            case 'p': {
                file.printSequentially();
                break;
            }
            case 'o': file.printMaster(); break;
            case 'q': {
                run = 0;
                return 0;
            }
            default: {
                run = 0;
                break;
            }
        }
    }
    
//    file.printSequentially();
    
}
