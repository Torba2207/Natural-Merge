#include "SortRecords.h"
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

using namespace std;


// Function to calculate the key for sorting a record: difference between max and min
double calculateKey(const vector<double>& record) {
    auto minMax = minmax_element(record.begin(), record.end());
    return *minMax.second - *minMax.first;  // max - min
}

// Function to read records from a file into a vector
vector<double> readRecord(ifstream& file) {
    vector<double> record;
    double value;
    while (file >> value) {
        record.push_back(value);
        if (file.peek() == '\n' || file.eof()) break;  // End of record
    }
    return record;
}

// Function to write a record to a file
void writeRecord(ofstream& file, const vector<double>& record, bool endlInTheEnd) {
    for (size_t i = 0; i < record.size(); ++i) {
        file << record[i];
        if (i < record.size() - 1) file << " ";
    }
    file << ",";
    if(endlInTheEnd)
        file << endl;
}


vector<vector<double>> readRun(ifstream& file) {
    vector<vector<double>> run;
    vector<double> record, prev;
    record = readRecord(file);
    
    prev = record;
    while (!file.eof() && calculateKey(prev) <= calculateKey(record)) {
        run.push_back(record);
        prev = record;
        record = readRecord(file);
        
    }
}

void writeRun(ofstream& file, vector<vector<double>>& run) {
    bool endlInTheEnd;
    for (size_t i = 0; i < run.size(); i++) {
        endlInTheEnd = i < run.size() - 2;
        writeRecord(file, run[i], endlInTheEnd);
    }
}

// Function to read 'nb' records into memory and sort them
/*void createRun(ifstream& inputFile, ofstream& runFile, int nb) {
    vector<vector<double>> buffer;
    for (int i = 0; i < nb; ++i) {
        if (inputFile.eof()) break;
        vector<double> record = readRecord(inputFile);
        buffer.push_back(record);
    }

    // Sort the buffer by the difference between max and min
    sort(buffer.begin(), buffer.end(), [](const vector<double>& a, const vector<double>& b) {
        return calculateKey(a) < calculateKey(b);
        });

    // Write sorted records to the run file
    for (int i = 0; i < buffer.size();i++) {
        if(i==buffer.size()-1)
            writeRecord(runFile, buffer[i], false);
        else
            writeRecord(runFile, buffer[i], true);
    }
}*/

// Function to merge runs using a priority queue
/*void mergeRuns(const vector<string>& runFiles, const string& sortedFile, int nb) {
    ofstream output(sortedFile);
    vector<ifstream> runStreams(runFiles.size());
    priority_queue<pair<double, pair<int, int>>> minHeap;  // {key, {run_index, record_index}}

    // Open all the run files
    for (int i = 0; i < runFiles.size(); ++i) {
        runStreams[i].open(runFiles[i]);
        vector<double> firstRecord = readRecord(runStreams[i]);
        if (!firstRecord.empty()) {
            minHeap.push({ calculateKey(firstRecord), {i, 0} });
        }
    }

    // Process the runs by merging
    while (!minHeap.empty()) {
        auto top = minHeap.top();
        minHeap.pop();
        int runIndex = top.second.first;
        int recordIndex = top.second.second;

        // Read the record from the corresponding run file
        vector<double> record = readRecord(runStreams[runIndex]);

        // Write the record to the output file
        writeRecord(output, record,true);

        // If there are more records in the same run file, push the next record to the heap
        if (!record.empty()) {
            minHeap.push({ calculateKey(record), {runIndex, recordIndex + 1} });
        }
    }

    // Close all run streams
    for (auto& runStream : runStreams) {
        runStream.close();
    }

    output.close();
}*/

// Function to perform sorting with big buffers
/*void mergeSortWithBigBuffers(const string& inputFile, const string& sortedFile, int nb) {
    ifstream input(inputFile);
    if (!input.is_open()) {
        cerr << "Error opening input file!" << endl;
        return;
    }

    vector<string> runFiles;
    int runCount = 0;

    // Stage 1: Create runs
    while (!input.eof()) {
        stringstream runFileName;
        runFileName << "run_" << runCount++ << ".txt";
        ofstream runFile(runFileName.str());

        createRun(input, runFile, nb);

        runFiles.push_back(runFileName.str());
    }

    input.close();

    // Stage 2: Merge runs
    
    while (runFiles.size() > 1) {
        vector<string> newRunFiles;

        for (size_t i = 0; i < runFiles.size(); i += nb) {
            // Merge `nb` runs into one
            vector<string> runsToMerge;
            for (size_t j = i; j < i + nb && j < runFiles.size(); ++j) {
                runsToMerge.push_back(runFiles[j]);
            }

            stringstream mergedRunFile;
            mergedRunFile << "merged_run_" << newRunFiles.size() << ".txt";
            mergeRuns(runsToMerge, mergedRunFile.str(), nb);
            newRunFiles.push_back(mergedRunFile.str());
        }

        // Remove old run files
        for (const string& runFile : runFiles) {
            remove(runFile.c_str());
        }

        runFiles = newRunFiles;
    }

    // Final sorted file
    if (rename(runFiles[0].c_str(), sortedFile.c_str()) != 0) {
        cout << "ERROR" << endl;
    }
    else {
        cout << "Success" << endl;
    }
}*/