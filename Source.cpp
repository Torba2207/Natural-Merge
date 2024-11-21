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
	string outputFile = "sortedText.txt";

	int blockingFactor = B_FACTOR;
	int numOfBuffers = NUM_OF_BUFFERS;
	int rec=20;
	//generateRecords(inputFile, rec, true);
	//mergeSortWithBigBuffers(inputFile, outputFile, blockingFactor * numOfBuffers);
	/*while (choice != 5) {
		cout << "1. Random input" << endl;
		cout << "2. Manual input" << endl;
		cout << "3. Sort" << endl;
		cout << "4. Sort Output" << endl;
		cout << "5. Exit" << endl;
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
			mergeSortWithBigBuffers(inputFile, outputFile, blockingFactor * numOfBuffers);
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
			break;
		}
		default:
			break;
		}
		

		
	}*/
	return 0;
}