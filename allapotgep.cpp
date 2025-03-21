/**
 * \file allapotgep.cpp
 *
 * Ebben a fájlban kell megvalósítania az Allapotgep osztály
 * metódusait, valamint mindazon további osztályokat, melyek szükségesek
 * a feladat megvalósításához.
 *
 * Ezt a fájlt be kell adni (fel kell tölteni) a megoldással.
 */

#include <iostream>
#include <fstream>
#include <cstring>

#include "allapotgep.h"
#include "memtrace.h"

///KONFIG KEZELÉS LOGIKA
///

Configuration::Configuration(const char *filename){

    //Getting the file
    //
    std::ifstream file(filename);


    if (!file.is_open()) {
        throw "N6R1CW";
    }
    //Getting the number of states
    //
    char numberOfStates[3];
    file.getline(numberOfStates, 2);
    size = atoi(numberOfStates);

    //Filling up the state information
    //
    stateNameTable = new char*[size];
    for (int i = 0; i < size; ++i) {
        stateNameTable[i] = new char[21];  // 20 characters + 1 for '\0'
    }

    stateValidityTable = new bool[size];
    for (int i = 0; i < size; i++) {
        stateValidityTable[i] = ((char)file.get() == 'I' ? true : false);
        file.get();
        file.getline(stateNameTable[i], 20);
    }

    //Filling the stateTable
    //
    stateTable = new char**[size];
    for (int i = 0; i < size; ++i) {
        stateTable[i] = new char*[size];
        for (int j = 0; j < size; ++j) {
            stateTable[i][j] = new char[5];
        }
    }

    char lineBuffer[1024];
    for (int i = 0; i < size; ++i) {
        file.getline(lineBuffer,1023);
        std::istringstream stream(lineBuffer);

        for (int j = 0; j < size; ++j)
            stream >> stateTable[i][j];

    }

    file.close();
}
int Configuration::StateNameToIndex(const char* stateName) {
    for (int i = 0; i < size; i++) {
        if (!strcmp(stateName, stateNameTable[i]))
            return i;
    }
    return -1;
}
bool Configuration::IsStateValid(const char* stateName) {
    return stateValidityTable[this->StateNameToIndex(stateName)];
}
char* Configuration::NextState(const char* currentState, Bazis inputBase) {
    int stateIndex = this->StateNameToIndex(currentState);
    char baseToSearch = cast(inputBase);
    for (int i = 0; i < size; i++) {
        if(strchr(stateTable[stateIndex][i],baseToSearch) != nullptr) {
            return stateNameTable[i];
        }
    }
    return nullptr;
}
Configuration& Configuration::operator=(const Configuration& rhs) {
    if (this != &rhs) {
        size = rhs.size;
        stateNameTable = new char*[size];
        for (int i = 0; i < size; i++) {
            stateNameTable[i] = new char[21];
            strcpy(stateNameTable[i], rhs.stateNameTable[i]);
        }
        stateValidityTable = new bool[size];
        for (int i = 0; i < size; i++) {
            stateValidityTable[i] = rhs.stateValidityTable[i];
        }

        stateTable = new char**[size];
        for (int i = 0; i < size; i++) {
            stateTable[i] = new char*[size];
            for (int j = 0; j < size; j++) {
                stateTable[i][j] = new char[5];
                strcpy(stateTable[i][j], rhs.stateTable[i][j]);
            }
        }
    }
    return *this;
}
Configuration::~Configuration() {
    for (int i = 0; i < size; i++) {
        delete[] stateNameTable[i];
    }
    delete[] stateNameTable;
    delete[] stateValidityTable;

    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            delete[] stateTable[i][j];
        }
        delete[] stateTable[i];
    }
    delete[] stateTable;
}


///ÁLLAPOTGÉP LOGIKA
///

void Allapotgep::konfigural(const char *fajlnev) {
    config = Configuration(fajlnev);

    strcpy(currentState,config.GetBaseState());
    isValid = config.IsStateValid(currentState);

}
const char *Allapotgep::aktualisallapot() {
    return currentState;
}
bool Allapotgep::elfogad() {
    return isValid;
}
void Allapotgep::atmenet(Bazis b) {
    char* nextState = config.NextState(currentState, b);
    strcpy(currentState, nextState);
    isValid = config.IsStateValid(currentState);
}
bool Allapotgep::feldolgoz(const Bazis *b, int n) {
    for (int i = 0; i < n; i++) {
        this->atmenet(b[i]);
    }
    return isValid;
}
void Allapotgep::alaphelyzet() {
    strcpy(currentState,config.GetBaseState());
}
Allapotgep::~Allapotgep() = default;
