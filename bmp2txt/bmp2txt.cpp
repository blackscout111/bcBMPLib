#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iomanip>
#include <string>

#include "bcBMPLib.h"

using namespace std;



////////////////////////////////////////////////////////////////////////////////
// Main Program
////////////////////////////////////////////////////////////////////////////////
int main()
{
	// Image file name
	string		image_name;

	// Image pixel data
	// (a pointer to an array of matrix<int>'s)
	// (layers are as follows: 0=red, 1=green, 2=blue, 3=transparency)
	matrix <int>	*image;

	// The number of required image layers
	// (1 for a black and white image, 3 for a color, 4 for color with
	// transparency)
	size_t	numLayers = 0;

	// Text file stream
	ofstream	*txt_file;

	// Text file save-to directory
	string		saveDir = "C:\\Users\\cliffb\\Desktop\\";


	// Prompts the user for the name of the image file
	cout	<< "================================================"
			<< endl
			<< "Enter the image file name (must be bitmap file): ";
	cin		>> image_name;
	cout	<< endl;

	// Reads in image from file
	numLayers = bcBMPLib::readBMP(image,image_name,true);

	// Allocates memory for output file streams
	txt_file = new ofstream [numLayers];

	// Opens output file(s)
	if (numLayers == 1)
	{
		txt_file[0].open((image_name + ".txt").c_str());
	}
	else if (numLayers == 3)
	{
		txt_file[0].open((image_name + "_r.txt").c_str());
		txt_file[1].open((image_name + "_g.txt").c_str());
		txt_file[2].open((image_name + "_b.txt").c_str());
	}
	else if (numLayers == 4)
	{
		txt_file[0].open((image_name + "_r.txt").c_str());
		txt_file[1].open((image_name + "_g.txt").c_str());
		txt_file[2].open((image_name + "_b.txt").c_str());
		txt_file[3].open((image_name + "_a.txt").c_str());
	}
	else
	{
		// Display error message
		cout	<< "ERROR: Could not open text files"
				<< endl;

		// Frees the memory of the file streams and the image
		delete[] image;
		delete[] txt_file;

		// Terminates the program
		cout << "Press <ENTER> to terminate the program...";
		cin.get();
		cin.get();
		exit(EXIT_FAILURE);
	}


	// Writes data to output file(s)
	for (size_t layer = 0; layer < numLayers; layer++)
	{
		// Writes dimensions of image to file(s)
		txt_file[layer]	<< image[layer].height() << "\t"
						<< image[layer].width() << "\r\n";

		// Writes data of image layers to file(s)
		for (size_t row = 0; row < image[layer].height(); row++)
		{
			for (size_t col = 0; col < image[layer].width(); col++)
			{
				txt_file[layer] << setw(4) << image[layer](row,col);
			}
			txt_file[layer] << "\r\n";
		}
	}

	// Frees the memory of the file streams and the image
	delete[] image;
	delete[] txt_file;

	// Pauses consol before closing
	cout	<< "Press <ENTER> to continue...";
	cin.get();
	cin.get();

	return EXIT_SUCCESS;
}
