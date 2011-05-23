#ifndef bmpHdr_h
#define bmpHdr_h

#include <iostream>

using namespace std;

class bmpHdr
{
	public:
		char		magicNum[2];
		short		reserved[2];
		unsigned int	fileSize,
						offset;

		// The size of the DIB header
		unsigned int dibHdrSize;

		// Constructor
		bmpHdr();

		// Returns the value of headerSize
		int size();

		// Reads in header data from a file stream
		void readData(ifstream& file);

		// Returns header data as a char array
		const char* getData();

		// Prints the member variables to the console window
		void print();
	
	private:
		// The sum of the bytes of each public member variable
		unsigned int	headerSize;

		// The bytes of the public members put in the following
		// order:
		// magicNum, fileSize, reserved, offset
		char	data[14];

		// Resets the data array
		void dataReset();
};


/**************************************************************************
* FUNCTION DEFINITIONS													  *
**************************************************************************/
//_________________________________________________________________________
bmpHdr::bmpHdr()
{
	magicNum[0] =	'B';
	magicNum[1] =	'M';

	reserved[0] =	0;
	reserved[1] =	0;

	fileSize =	0;
	offset =	0;

	headerSize =	14;

	dibHdrSize = 0;

	dataReset();
}

//_________________________________________________________________________
void bmpHdr::dataReset()
{
	// Debug Code
//	cout << "reseting data..." << endl;

	// An index variable for data
	int index = 0;

	// A temporary pointer variable
	char *tempPtr;

	// Adds magicNum to data
	data[index] = magicNum[0];
	index++;

	data[index] = magicNum[1];
	index++;

	// Adds fileSize to data
	tempPtr = (char*)(&fileSize);
	for (int i = 0; i < sizeof(fileSize); i++)
	{
		data[index] = tempPtr[i];
		index++;
	}
	
	// Adds reserved to data
	tempPtr = (char*)(reserved);
	for (int i = 0; i < sizeof(reserved); i++)
	{
		data[index] = tempPtr[i];
		index++;
	}

	// Adds offset to data
	tempPtr = (char*)(&offset);
	for (int i = 0; i < sizeof(offset); i++)
	{
		data[index] = tempPtr[i];
		index++;
	}

	// Debug Code
/*	cout 	<< "final index: " << index << endl
		<< "data reset..." << endl << endl;
*/
	// Nullifies the temporary pointer
	tempPtr = NULL;
}

//_________________________________________________________________________
int bmpHdr::size()
{
	return headerSize;
}

//_________________________________________________________________________
const char* bmpHdr::getData()
{
	dataReset();

	return data;
}

//_________________________________________________________________________
void bmpHdr::readData(ifstream& file)
{
	// Magic number
	file.read((char*)magicNum, sizeof(magicNum));

	// File size
	file.read((char*)(&fileSize), sizeof(fileSize));

	// Reserved
	file.read((char*)(&reserved), sizeof(reserved));

	// Offset
	file.read((char*)(&offset), sizeof(offset));

	// DIB header size
	file.read((char*)(&dibHdrSize), sizeof(dibHdrSize));
}

//_________________________________________________________________________
void bmpHdr::print()
{
	cout	<< "=======================" << endl
			<< "Bitmap File Info Header" << endl
			<< "=======================" << endl
			<< "Header Size (bytes):    " << headerSize << endl
			<< "Magic #:                " << magicNum[0] << magicNum[1] << endl
			<< "File Size (bytes):      " << fileSize << endl
			<< "Reserved:               " << reserved[0] << " " << reserved[1]
			<< endl
			<< "Data Offset:            " << offset << endl;
}

#endif
