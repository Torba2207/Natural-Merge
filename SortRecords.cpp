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


vector<vector<double>> readRun(BlockIO& blockIO, vector<double>& currentRecord) {
    vector<vector<double>> run;
    vector<double> record, prev;
    if (!currentRecord.empty())
        record = currentRecord;
    else
        if (!blockIO.readRecord(record))
            return run;

    prev = record;
    bool rdrc = true;
    bool sameRecFlag = false;
    while (!blockIO.getEndOfFile() && rdrc && calculateKey(prev) <= calculateKey(record)) {
        if (!sameRecFlag)
            run.push_back(record);
        else
            sameRecFlag = false;
        prev = record;
        record.clear();
        rdrc = blockIO.readRecord(record);
        if (!rdrc || record.empty()) {
            record = prev;
            sameRecFlag = true;
        }

    }
    currentRecord = record;
    return run;
}

void writeRun(BlockIO& blockIO, const vector<vector<double>>& run) {
    bool forceFlush;
    for (size_t i = 0; i < run.size(); i++) {
        forceFlush = (i >= run.size() - 1 ? true : false);
        blockIO.writeRecord(run[i], forceFlush);
    }
}

void showTape(BlockIO& blockIO, vector<vector<double>>& currun, vector<double>& currentRec, bool opt6) {
    int currunid = 0;
    currentRec.clear();
    while (!blockIO.getEndOfFile()) {
        currun = readRun(blockIO, currentRec);
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
    //ifstream infile(inputFile);
    BlockIO inputTapeBlock(inputFile, BLOCK_SIZE, false);
    //ofstream out1(tape1), out2(tape2);
    BlockIO outTapeBlock1(tape1,BLOCK_SIZE,true);
    BlockIO outTapeBlock2(tape2, BLOCK_SIZE, true);
    vector<double> currentRecord;
    vector<vector<double>> run;
    bool tapeWrite1 = true;
    int writeCounter = 0;
    int runCounter = 0;
    while (!inputTapeBlock.getEndOfFile()) {
        run = readRun(inputTapeBlock, currentRecord);
        runCounter++;
        writeRun(tapeWrite1 ? outTapeBlock1 : outTapeBlock2, run);
        writeCounter++;
        tapeWrite1 = !tapeWrite1;
    }
    inputTapeBlock.switchEof();
    if (run.back() != currentRecord && !currentRecord.empty())
        tapeWrite1 ? outTapeBlock1.writeRecord(currentRecord,true) : outTapeBlock2.writeRecord(currentRecord,true);
    //infile.close();
    //out1.close();
    //out2.close();
    //cout << writeCounter << endl;
    cout<<"Runs in:"<<inputFile<<" " << runCounter << endl;
    frp+=inputTapeBlock.getFileReads();
    fwp = fwp+outTapeBlock1.getFileWrites()+outTapeBlock2.getFileWrites();

}

void mergeRuns(string& tape1, string& tape2, string& tape3, int& frp, int& fwp, bool& sorted) {

    //ifstream in1(tape1), in2(tape2);
    BlockIO inTape1(tape1, BLOCK_SIZE, false);
    BlockIO inTape2(tape2, BLOCK_SIZE, false);
    //ofstream out3(tape3);
    BlockIO outTape3(tape3, BLOCK_SIZE, true);
    vector<double> currentRecord1;
    vector<double> currentRecord2;
    //vector<double> lastRecordInMerge;

    vector<vector<double>>run1 = readRun(inTape1, currentRecord1);
    vector<vector<double>>run2 = readRun(inTape2, currentRecord2);

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
        writeRun(outTape3, mergedRun);

        run1 = readRun(inTape1, currentRecord1);
        run2 = readRun(inTape2, currentRecord2);
        if (!run1.empty()) {
            if (calculateKey(mergedRun[mergedRun.size() - 1]) > calculateKey(run1[0]))
                sorted = false;
        }
        if (!run2.empty()) {
            if (calculateKey(mergedRun[mergedRun.size() - 1]) > calculateKey(run2[0]))
                sorted = false;
        }
        
    }
    inTape1.switchEof();
    inTape2.switchEof();

    frp = frp+inTape1.getFileReads() + inTape2.getFileReads();
    fwp+=outTape3.getFileWrites();
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
        mergeRuns(tape1, tape2, tape3, *frp, *fwp, sorted);
        /*
        ifstream checkSorted(tape3);
        fileReads++;
        //readRun(checkSorted,currentRec);
        //if (!checkSorted.eof())
        //    sorted = false;
        vector<double> curr, prev;
        prev = readRecord(checkSorted);
        while (!checkSorted.eof()) {
            curr = readRecord(checkSorted);
            if (!curr.empty() && calculateKey(curr) < calculateKey(prev)) {
                sorted = false;
                break;
            }
            prev = curr;
        }
        checkSorted.close();
        */
        if (opt5 || opt6) {
            //ifstream t1(tape1), t2(tape2), t3(tape3);
            BlockIO t1(tape1, BLOCK_SIZE, false), t2(tape2, BLOCK_SIZE, false), t3(tape3, BLOCK_SIZE, false);
            cout << "Tape1: " << endl;
            showTape(t1, currun, currentRec, opt6);
            cout << "Tape2: " << endl;
            showTape(t2, currun, currentRec, opt6);
            cout << "Tape3: " << endl;
            showTape(t3, currun, currentRec, opt6);
            
        }
        if (sorted) {
            break;
        }
        distributeRuns(tape3, tape1, tape2, *frp, *fwp);
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