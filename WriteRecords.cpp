#include "WriteRecords.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <limits>

#define MIN_VALUE -100.0
#define MAX_VALUE 100.0
using namespace std;

double generateRandomDouble(double min, double max) {
    return min + (double)(rand()) / RAND_MAX * (max - min);
}

vector<double> generateRandomRecord() {
    int numElements = rand() % 14 + 2; // quantity of numbers in records from 2 to 15
    vector<double> record;
    for (int i = 0; i < numElements; i++) {
        record.push_back(generateRandomDouble(MIN_VALUE, MAX_VALUE)); // generating and pushing back number to the record
    }
    return record;
}


void writeRecordUS(ofstream& file, const vector<double>& record, bool endlInTheEnd) {
    for (size_t i = 0; i < record.size(); i++) {
        file << record[i];
        if (i < record.size() - 1) file << " ";
    }
    //file << ",";
    //if(endlInTheEnd)
    file << endl;
}


vector<double> inputRecord() {
    int numElements;
    cout << "Quantity of numbers in record: ";
    cin >> numElements;
    if (numElements < 2 || numElements > 15) {
        cout << "Error: quantity can not be below 2 and more then 15." << endl;
        return {};
    }
    vector<double> record(numElements);
    cout << "Enter " << numElements << " numbers: ";
    for (int i = 0; i < numElements; i++) {
        cin >> record[i];
    }
    return record;
}


void generateRecords(const string& filename, int numRecords, bool randomMode) {
    ofstream file(filename);
    if (!file.is_open()) {
        cerr << "Error when trying to open this file" << endl;
        return;
    }

    for (int i = 0; i < numRecords; i++) {
        vector<double> record;
        if (randomMode) {
            record = generateRandomRecord();
        }
        else {
            record = inputRecord();
            if (record.empty()) {
                cout << "Invalid input. Continue..." << endl;
                continue;
            }
        }
        if(i==numRecords-1)
            writeRecordUS(file, record,false);
        else
            writeRecordUS(file, record, true);
    }
    file.close();
    cout << "Succesful writing to the file " << filename << endl;
}