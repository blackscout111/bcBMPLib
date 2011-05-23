#ifndef dibv3_h
#define dibv3_h

#include <iostream>
using namespace std;

class dibv3
{
	public:
		unsigned int	width,
						height,
						compressionType,
						rawDataSize,
						horizontalRes,
						verticalRes,
						numPaletteColors,
						numImportantColors;

		unsigned short	numColorPlanes,
						numBitsPerPixel;

		// Constructor
		dibv3();

		// Returns the value of headerSize
		int size();

		// Reads in header data from a file stream
		// (To be used write after the file info header is read)
		void readData(ifstream& file);

		// Returns header data as a char array
		const char* getData();

		// Prints the member variables to the console window
		void print();
	
	private:
		unsigned int	headerSize;
		char	data[40];

		// Resets the data array
		void dataReset();
};


/**************************************************************************
* FUNCTION DEFINITIONS													  *
**************************************************************************/
//_________________________________________________________________________
dibv3::dibv3()
{
	width = 0;
	height = 0;

	numColorPlanes = 1;
	numBitsPerPixel = 0;

	compressionType = 0;
	rawDataSize = 0;
	horizontalRes = 0;
	verticalRes = 0;
	numPaletteColors = 0;
	numImportantColors = 0;

	headerSize =	40;
	dataReset();
}

//_________________________________________________________________________
void dibv3::dataReset()
{
	// Debug Code
//	cout << "reseting data..." << endl;

	// An index variable for data
	int index = 0;

	// A temporary pointer variable
	char *tempPtr;

	// Adds headerSize to data
	tempPtr = (char*)(&headerSize);
	for (int i = 0; i < sizeof(headerSize); i++)
	{
		data[index] = tempPtr[i];
		index++;
	}
	
	// Adds width to data
	tempPtr = (char*)(&width);
	for (int i = 0; i < sizeof(width); i++)
	{
		data[index] = tempPtr[i];
		index++;
	}

	// Adds height to data
	tempPtr = (char*)(&height);
	for (int i = 0; i < sizeof(height); i++)
	{
		data[index] = tempPtr[i];
		index++;
	}
	
	// Adds numColorPlanes to data
	tempPtr = (char*)(&numColorPlanes);
	for (int i = 0; i < sizeof(numColorPlanes); i++)
	{
		data[index] = tempPtr[i];
		index++;
	}

	// Adds numBitsPerPixel to data
	tempPtr = (char*)(&numBitsPerPixel);
	for (int i = 0; i < sizeof(numBitsPerPixel); i++)
	{
		data[index] = tempPtr[i];
		index++;
	}
	
	// Adds compressionType to data
	tempPtr = (char*)(&compressionType);
	for (int i = 0; i < sizeof(compressionType); i++)
	{
		data[index] = tempPtr[i];
		index++;
	}

	// Adds rawDataSize to data
	tempPtr = (char*)(&rawDataSize);
	for (int i = 0; i < sizeof(rawDataSize); i++)
	{
		data[index] = tempPtr[i];
		index++;
	}
	
	// Adds horizontalRes to data
	tempPtr = (char*)(&horizontalRes);
	for (int i = 0; i < sizeof(horizontalRes); i++)
	{
		data[index] = tempPtr[i];
		index++;
	}

	// Adds verticalRes to data
	tempPtr = (char*)(&verticalRes);
	for (int i = 0; i < sizeof(verticalRes); i++)
	{
		data[index] = tempPtr[i];
		index++;
	}

	// Adds numPaletteColors to data
	tempPtr = (char*)(&numPaletteColors);
	for (int i = 0; i < sizeof(numPaletteColors); i++)
	{
		data[index] = tempPtr[i];
		index++;
	}

	// Adds numImportantColors to data
	tempPtr = (char*)(&numImportantColors);
	for (int i = 0; i < sizeof(numImportantColors); i++)
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
int dibv3::size()
{
	return headerSize;
}

//_________________________________________________________________________
void dibv3::readData(ifstream& file)
{
	// Reads in the rest of the header
	file.read((char*)(&width),
			sizeof(width));
	file.read((char*)(&height),
			sizeof(height));
	file.read((char*)(&numColorPlanes),
			sizeof(numColorPlanes));
	file.read((char*)(&numBitsPerPixel),
			sizeof(numBitsPerPixel));
	file.read((char*)(&compressionType),
			sizeof(compressionType));
	file.read((char*)(&rawDataSize),
			sizeof(rawDataSize));
	file.read((char*)(&horizontalRes),
			sizeof(horizontalRes));
	file.read((char*)(&verticalRes),
			sizeof(verticalRes));
	file.read((char*)(&numPaletteColors),
			sizeof(numPaletteColors));
	file.read((char*)(&numImportantColors),
			sizeof(numImportantColors));
}


//_________________________________________________________________________
const char* dibv3::getData()
{
	dataReset();

	return data;
}

//_________________________________________________________________________
void dibv3::print()
{
	cout	<< "======================" << endl
			<< "DIB Info Header" << endl
			<< "======================" << endl
			<< "Header Size:           " << headerSize << endl
			<< "Image Width:           " << width << endl
			<< "Image Height:          " << height << endl
			<< "# of Color Planes:     " << numColorPlanes << endl
			<< "# of Bits Per Pixel:   " << numBitsPerPixel << endl
			<< "Compression Type:      " << compressionType << endl
			<< "Raw Data Size:         " << rawDataSize << endl
			<< "Horizontal Resolution: " << horizontalRes << endl
			<< "Vertical Resolution:   " << verticalRes << endl
			<< "# of Palette Colors:   " << numPaletteColors << endl
			<< "# of Important Colors: " << numImportantColors << endl;
}

#endif
