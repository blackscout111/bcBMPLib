#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <cstdio>
#include <cstdlib>
#include <cstring>

#include "../myIsOpen.h"
#include "../bcBMPLib/bcBMPLib.h"

using namespace std;

// Reads in image layer data
void readLayerData(matrix<int>& data, const string& file_name);

int main()
{
	// Name of the image
	string image_name;

	// Layered image data
	// (red= layer 1, green= layer 2, blue= layer 3)
	// (*Layer 0 contains the dimensions of the array)
	matrix <int>	*image;

	// Number of layers to the image
	int	numLayers;

	// Array of layer data file names
	string	*layerName;

	// Prompts the user for the number of image layers	
	cout	<< "======================================================="
		<< endl
		<< "Enter the nuber of image layers: ";

	cin	>> numLayers;

	cout	<< endl;


	// Prompts the user for the names of the data files containing
	// layer data
	if (numLayers > 0)
	{
		// Allocates memory for layerName
		layerName = new string[numLayers];

		// Gets layer names
		cout	<< "Enter layer file names" << endl;
		for (int i = 0; i < numLayers; i++)
		{
			cin >> layerName[i];
		}
	}
	else
	{
		// Pauses console before closing
		cout << "Press <ENTER> to continue...";
		cin.get();
		cin.get();

		return -1;
	}
	cout << endl;

	// Allocates memory for image layers
	image = new matrix <int> [numLayers];


	// Prompts user for name of new image name
	cout << "Name of new image file (must be bitmap file): ";
	cin >> image_name;
	cout << endl;


	// Reads in layer data
	for (int i = 0; i < numLayers; i++)
	{
		// Reads in data
		readLayerData(image[i],layerName[i]);
	}

	// Makes and saves image
	makeBMP(image, numLayers, image_name);

	// Pauses console before closing
	cout << "Press <ENTER> to continue...";
	cin.get();
	cin.get();

	return 0;
}

/////////////////////////////////////////////////////////////////////////
// Function Definitions											       //
/////////////////////////////////////////////////////////////////////////

void readLayerData(matrix<int>& data, const string& file_name)
{
	// Number of rows and columns of the image data
	int	numRows = 0,
		numCols = 0;

	// Data file variable
	ifstream file;

	// Open data_file and check to see if it is open	
	file.open(file_name.c_str());
	myIsOpen(file,file_name);

	// Get the number of rows
	file >> numRows;

	// Get the number of columns
	file >> numCols;

	// Changes height of layer if needed
	if (data.height() < numRows)
	{
		data.grow(0, (numRows - data.height()), 0, 0);
	}

	// Changes width of layer if needed
	if (data.width() < numCols)
	{
		data.grow(0, 0, 0, (numCols - data.width()));
	}

	// Reads in data from file
	for (int r = 0; r < data.height(); r++)
	{
		for (int c = 0; c < data.width(); c++)
		{
			file >> data(r,c);
		}
	}
}
