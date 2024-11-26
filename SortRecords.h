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
#include "BlockIO.h"
#define BLOCK_SIZE 1000


double calculateKey(const std::vector<double>& record);
//void readToBuffer(std::ifstream& file, int blockSize, char* buffer);
std::vector<double> readRecord(std::ifstream& file);
void writeRecord(std::ofstream& file, const std::vector<double>& record, bool endlInTheEnd);
std::vector<std::vector<double>> readRun(BlockIO& blockIO, std::vector<double>& currentRec);
void writeRun(BlockIO& blockIO, const std::vector<std::vector<double>>& run);
void showTape(BlockIO& blockIO, std::vector<std::vector<double>>& currun, std::vector<double>& currentRec, bool opt6);
void distributeRuns(const std::string& inputFile, const std::string& tape1, const std::string& tape2, int& frp, int& fwp);
void mergeRuns(const std::string& tape1, const std::string& tape2, const std::string& tape3, int& frp, int& fwp, bool& sorted);
void naturalMergeSort(const std::string& inputFile, bool opt5, bool opt6); //TODO

