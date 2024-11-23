#include "WriteRecords.h"
#include "SortRecords.h"
#include <iostream>
#include <ctime>
#define B_FACTOR 5
#define NUM_OF_BUFFERS 1

using namespace std;




int main() {
	srand(time(0));
	int choice = 0;
	string inputFile = "text.txt";
	string outputFile = "tape3.txt";

	int blockingFactor = B_FACTOR;
	int numOfBuffers = NUM_OF_BUFFERS;
	int rec=100;
	//distributeRuns(inputFile, "tape1.txt", "tape2.txt");
	//generateRecords(inputFile, rec, true);
	//mergeSortWithBigBuffers(inputFile, outputFile, blockingFactor * numOfBuffers);
	
	while (choice != 7) {
		cout << "1. Random input" << endl;
		cout << "2. Manual input" << endl;
		cout << "3. Sort" << endl;
		cout << "4. Sort Output" << endl;
		cout << "5. Sort with showing tapes and number of runs" << endl;
		cout << "6. Sort with showing tapes and runs" << endl;
		cout << "7. Exit" << endl;;
		cin >> choice;
		switch (choice)
		{
		case 1: {
			cout << "Enter num of records: ";
			
			cin >> rec;
			generateRecords(inputFile, rec, true);
			break;
		}
		case 2: {
			cout << "Enter num of records: ";
			cin >> rec;
			generateRecords(inputFile, rec, false);
			break;
		}
		case 3: {
			naturalMergeSort(inputFile, false,false);
			break;
		}
		case 4: {
			ifstream opf(outputFile);
			string line;
			int counter = 0;
			if (opf.is_open()) {
				while (getline(opf, line)) {
					cout << line << endl;
					counter++;
				}
				opf.close();
				cout << counter << " Records" << endl;
			}
			break;
		}
		case 5: {
			naturalMergeSort(inputFile, true, false);
			break;
		}

		case 6: {
			naturalMergeSort(inputFile, false, true);
			break;
		}
		
		case 7: {
			cout << "Exiting..." << endl;
			break;
		}
		default:
			break;
		}
		

		
	}
	return 0;
}