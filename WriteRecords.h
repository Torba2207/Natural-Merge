#pragma once
#include <vector>
#include <string>
#include <fstream>


//Generation of a random real number
double generateRandomDouble(double min, double max); 


//Generation of a random record
std::vector<double> generateRandomRecord();

//Write the record to the file
void writeRecordUS(std::ofstream& file, const std::vector<double>& record, bool endlInTheEnd);

//Manual input of data for a new record
std::vector<double> inputRecord();


//General function for generation and writing records to the file
void generateRecords(const std::string& filename, int numRecords, bool randomMode);