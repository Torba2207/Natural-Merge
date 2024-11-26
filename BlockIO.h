#pragma once
#include <vector>
#include <fstream>
#include <string>
#include <algorithm>
#include <limits>
#include <queue>
#include <cmath>
#include <sstream>
#include <iostream>
#include <cstdio>
#include <algorithm>

#define RECORD_MAX_SIZE 15


using namespace std;

class BlockIO {
private:
    string fileName; // Plik do odczytu lub zapisu
    vector<char> buffer; // Bufor blokowy
    int blockSize; // Rozmiar bloku w bajtach
    int bufferPos; // Pozycja w buforze
    bool isWriteMode; // Tryb: true - zapis, false - odczyt
    //bool isResized;
    int filePosition;
    bool cleanBuffer;
    //double currentLowest;
    int lastGoodLine;
    bool eofdetected=false;
    ofstream filew;
    int fileRead;
    int fileWrite;

    void flushBuffer();
    void fillBuffer();

    


public:
    BlockIO(const string& filename, int blockSize, bool writeMode);
    ~BlockIO();
    bool readRecord(vector<double>& record);
    void writeRecord(const vector<double>& record, bool forceFlush);
    bool getEndOfFile();

    void switchEof();
    int getFileReads();
    int getFileWrites();

};

