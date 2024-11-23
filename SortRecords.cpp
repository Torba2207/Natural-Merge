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
    //file << ",";
    //if(endlInTheEnd)
    file << endl;
}


vector<vector<double>> readRun(ifstream& file, vector<double>& currentRecord) {
    vector<vector<double>> run;
    vector<double> record, prev;
    if (!currentRecord.empty())
        record = currentRecord;
    else
        record = readRecord(file);
    
    prev = record;
    while (!file.eof() && calculateKey(prev) <= calculateKey(record)) {
        run.push_back(record);
        prev = record;
        record = readRecord(file);
        
    }
    currentRecord = record;
    return run;
}

void writeRun(ofstream& file, const vector<vector<double>>& run) {
    bool endlInTheEnd;
    for (size_t i = 0; i < run.size(); i++) {
        endlInTheEnd = (i < run.size() - 1?true:false);
        writeRecord(file, run[i], endlInTheEnd);
    }
}

void showTape(ifstream& file, vector<vector<double>>& currun, vector<double>& currentRec, bool opt6) {
    int currunid = 0;
    while (!file.eof()) {
        currun = readRun(file, currentRec);
        cout << "Run " << currunid << ": " << endl;
        if (opt6) {
            for (int i = 0; i < currun.size(); i++) {
                for (int j = 0; j < currun[i].size(); j++) {
                    cout << currun[i][j] << " ";
                }
                cout << "Key: " << calculateKey(currun[i]);
                cout << endl;
            }
        }
        currunid++;
    }
}


void distributeRuns(const string& inputFile, const string& tape1, const string& tape2, int& frp, int& fwp) {
    ifstream infile(inputFile);
    ofstream out1(tape1), out2(tape2);
    vector<double> currentRecord;
    vector<vector<double>> run;
    bool tapeWrite1 = true;
    while (!infile.eof()) {
        run = readRun(infile,currentRecord);
        writeRun(tapeWrite1 ? out1 : out2, run);
        tapeWrite1 = !tapeWrite1;
    }
    if (run.back() != currentRecord&&!currentRecord.empty())
        writeRecord(tapeWrite1 ? out1 : out2, currentRecord, false);
    infile.close();
    out1.close();
    out2.close();
    frp++;
    fwp += 2;
       
}

void mergeRuns(string& tape1, string& tape2, string& tape3,int& frp, int& fwp) {
    ifstream in1(tape1), in2(tape2);
    ofstream out3(tape3);
    vector<double> currentRecord1;
    vector<double> currentRecord2;

    vector<vector<double>>run1 = readRun(in1,currentRecord1);
    vector<vector<double>>run2 = readRun(in2, currentRecord2);

    while (!run1.empty() || !run2.empty()) {
        vector<vector<double>> mergedRun;
        while (!run1.empty() && !run2.empty()) {
            if (calculateKey(run1.front()) <= calculateKey(run2.front())) {
                mergedRun.push_back(run1.front());
                run1.erase(run1.begin());
            }
            else {
                mergedRun.push_back(run2.front());
                run2.erase(run2.begin());
            }
        }
        mergedRun.insert(mergedRun.end(), run1.begin(), run1.end());
        mergedRun.insert(mergedRun.end(), run2.begin(), run2.end());
        writeRun(out3, mergedRun);

        run1 = readRun(in1,currentRecord1);
        run2 = readRun(in2, currentRecord2);
    }

    in1.close();
    in2.close();
    out3.close();
    frp += 2;
    fwp++;
}





void naturalMergeSort(const string& inputFile, bool opt5, bool opt6) {
    string tape1 = "tape1.txt";
    string tape2 = "tape2.txt";
    string tape3 = "tape3.txt";
    int fileReads = 0;
    int fileWrites = 0;
    int* frp = &fileReads;
    int* fwp = &fileWrites;
    vector<double>currentRec;
    vector<vector<double>> currun;
    bool sorted;
    int phase = 1;

    distributeRuns(inputFile, tape1, tape2, *frp, *fwp);


    while (true) {
        cout << "Phase " << phase << ":\n";
        sorted = true;
        mergeRuns(tape1, tape2, tape3,*frp, *fwp);
        ifstream checkSorted(tape3);
        fileReads++;
        //readRun(checkSorted,currentRec);
        //if (!checkSorted.eof())
        //    sorted = false;
        vector<double> curr, prev;
        prev = readRecord(checkSorted);
        while (!checkSorted.eof()) {
            curr = readRecord(checkSorted);
            if (!curr.empty()&&calculateKey(curr) < calculateKey(prev)) {
                sorted = false;
                break;
            }
            prev = curr;
        }
        checkSorted.close();
        if (opt5 || opt6) {
            ifstream t1(tape1), t2(tape2), t3(tape3);
            cout << "Tape1: " << endl;
            showTape(t1, currun, currentRec, opt6);
            cout << "Tape2: " << endl;
            showTape(t2, currun, currentRec, opt6);
            cout << "Tape3: " << endl;
            showTape(t3, currun, currentRec, opt6);
            t1.close();
            t2.close();
            t3.close();
        }
        if (sorted) {
            break;
        }
        distributeRuns(tape3, tape1, tape2,*frp,*fwp);
        phase++;
    }
    cout << "File sorted successfuly!" << endl;
    cout << "Number of phases: " << phase << endl;
    cout << "Files were read " << fileReads << " times" << endl;
    cout << "Files were written " << fileWrites << " times" << endl;

    
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