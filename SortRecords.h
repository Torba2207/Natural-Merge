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



double calculateKey(const std::vector<double>& record);
std::vector<double> readRecord(std::ifstream& file);
void writeRecord(std::ofstream& file, const std::vector<double>& record, bool endlInTheEnd);
std::vector<std::vector<double>> readRun(std::ifstream& file);
void writeRun(std::ofstream& file, const std::vector<std::vector<double>>& run);
void distributeRuns(const std::string& inputFile, const std::string& tape2); //TODO
//void createRun(std::ifstream& inputFile, std::ofstream& runFile, int nb);
void mergeRuns(const std::vector<std::string>& runFiles, const std::string& sortedFile, int nb); //TODO
//void mergeSortWithBigBuffers(const std::string& inputFile, const std::string& sortedFile, int nb);
void naturalMergeSort(const std::string& inputFile, const std::string& outputFile); //TODO


