#include "BlockIO.h"

using namespace std;


BlockIO::BlockIO(const string& filename, int blockSize, bool writeMode) {
	this->fileName = filename;
	this->blockSize = blockSize;
	this->isWriteMode = writeMode;
	if(isWriteMode)
		filew.open(filename, ios::out);
	buffer.resize(blockSize);
	bufferPos = 0;
	cleanBuffer = true;
	filePosition = 0;
	fileRead = 0;
	fileWrite = 0;
}

BlockIO::~BlockIO() {
	if (this->isWriteMode) {
		flushBuffer();
		if (filew.is_open())
			filew.close();
	}
}

void BlockIO::flushBuffer() {
	
	if (!filew.is_open()) {
		cerr << "Bad File(flushhing)";
	}
	//file.seekp(filePosition);
	if (bufferPos > 0) {
		filew.write(buffer.data(), bufferPos);
		//filePosition += bufferPos;
		bufferPos = 0;
		fileWrite++;
	}
	//file.close();
}

void BlockIO::fillBuffer() {
	ifstream file(fileName);
	if (!file.is_open()) {
		cerr << "Bad File(filling)";
	}
	file.seekg(filePosition);
	file.read(buffer.data(), blockSize);
	if (file.gcount() < blockSize) {
		for (int i = file.gcount(); i < blockSize; i++)
			buffer[i] = '\0';
		
	}
	filePosition = file.tellg();
	fileRead++;
	file.close();
	cleanBuffer = false;
	

}


bool BlockIO::readRecord(vector<double>& record) {
	if (cleanBuffer) {
		bufferPos = 0;
		fillBuffer();
	}
	char c=0;
	stringstream recs;
	while (bufferPos < blockSize) {
		c = buffer[bufferPos];
		bufferPos++;
		if (c == '\n') {
			lastGoodLine = filePosition-blockSize+bufferPos;
			break;
		}
		if (c == '\0') {
			cleanBuffer = true;
			filePosition = lastGoodLine;
			eofdetected = true;
			return false;
			

		}
		recs.put(c);


		

	}

	//cout << bufferPos << " " << blockSize <<" "<<c<< endl;
	if (c == '\n') {
		double value;
		while (recs >> value)
			record.push_back(value);
	}
	else
		if (bufferPos >= blockSize) {
			filePosition = lastGoodLine;
			cleanBuffer = true;
			return true;
		}
	return true;

}



void BlockIO::writeRecord(const vector<double>& record, bool forceFlush) {
	stringstream recs;
	for (size_t i = 0; i < record.size(); i++)
		recs << record[i] << (i < record.size() - 1 ? " " : "\n");
	string data = recs.str();
	size_t dataSize = data.size();
	
	if (blockSize >= bufferPos + dataSize) {
		copy(data.begin(), data.begin() + dataSize, buffer.begin() + bufferPos);
		bufferPos += dataSize;
	}
	else
		if (blockSize < bufferPos + dataSize) {
			flushBuffer();
			copy(data.begin(), data.begin() + dataSize, buffer.begin() + bufferPos);
			bufferPos += dataSize;
		}

	


}


bool BlockIO::getEndOfFile() {
	return eofdetected;
}

void BlockIO::switchEof() {
	eofdetected = !eofdetected;
}

int BlockIO::getFileReads() {
	return this->fileRead;

}

int BlockIO::getFileWrites() {
	return this->fileWrite;
}