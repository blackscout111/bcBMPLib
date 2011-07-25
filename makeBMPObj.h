#ifndef makeBMPObj_h
#define makeBMPObj_h

#include <iostream>
#include <fstream>
#include <string>
#include <cstdio>
#include <cstdlib>
#include <cstring>

#include "bmp_headers.h"
#include "matrix.h"


using namespace std;

class makeBMPObj
{
	public:
		// Checks to see that the number of layers is 1, 3, or 4, and that all
		// of the layers have the same dimensions
		void checkLayerNumAndDim(matrix<int>* image, int numLayers);

		// Initializes and fills the color pallet
		char* fillColorPalette(int& paletteSize, dibv3& dibHdr);

		// Writes the data to the Bitmap file
		void writeBMPdata(matrix<int>* image,
						  int numLayers,
						  ofstream& image_file,
						  dibv3& dibHdr);
};


////////////////////////////////////////////////////////////////////////////////
// FUNCTION DEFINITIONS FOR makeBMPObj
////////////////////////////////////////////////////////////////////////////////

//______________________________________________________________________________
void makeBMPObj::checkLayerNumAndDim(matrix<int>* image, int numLayers)
{
	// Check layer number
	if ((numLayers == 1) || (numLayers == 3) || (numLayers == 4))
	{
		// Do nothing...
	}
	else
	{
		// Display error message and end program 
		cout	<< endl
				<< "bcBMPLib.h: In function "
				<< "'int makeBMP(matrix<int>* image, int numLayers, "
				<< "string image_name)'"
				<< "unsigned char disp= 0)'"
				<< endl
				<< "bcBMPLib.h: error: "
				<< "Layer number ("
				<< numLayers
				<< ") not allowed"
				<< endl << endl;

		exit(EXIT_FAILURE);
	}


	// Check layer dimensions
	for (char i = 0; i < numLayers; i++)
	{
		if (image[i].height() != image[0].height() ||
			image[i].width() != image[0].width())
		{
			cout	<< endl
					<< "bcBMPLib.h: In function "
					<< "'int makeBMP(matrix<int>* image, int numLayers, "
					<< "string image_name)'"
					<< "unsigned char disp= 0)'"
					<< endl
					<< "bcBMPLib.h: error: "
					<< "Dimension mismatch"
					<< endl << endl;

			exit(EXIT_FAILURE);
		}
	}
}


//______________________________________________________________________________
char* makeBMPObj::fillColorPalette(int& paletteSize, dibv3& dibHdr)
{
	char *palette = NULL;

	dibHdr.numPaletteColors = 256;
	dibHdr.numImportantColors = 256;
	paletteSize = 4 * dibHdr.numPaletteColors;

	// Allocates memory for palette
	palette = new char [paletteSize];

	// Fills palette
	char value = 0;
	for (int i = 0; i < paletteSize; i += 4)
	{
		char red = value,
			 green = value,
			 blue = value,
			 alpha = 255;

		palette[i] = blue;
		palette[i + 1] = green;
		palette[i + 2] = red;
		palette[i + 3] = alpha;

		value++;
	}

	return palette;
}



//______________________________________________________________________________
void makeBMPObj::writeBMPdata(matrix<int>* image,
							  int numLayers,
							  ofstream& image_file,
							  dibv3& dibHdr)
{
	// Number of bytes per image pixel row
	int rowsize = 4*((dibHdr.numBitsPerPixel * dibHdr.width + 31)/32);

	// A buffer for writing the pixel row data to the file
	char *buffer = new char [rowsize];

	// Loop to write the data rows (bottom to top)
	for (int row = dibHdr.height - 1; row >= 0; row--)
	{
		// Sets all values of buffer to zero
		memset(buffer,0,rowsize);

		// A variable to navigate through the buffer
		int bufpos = 0;

		// Fills buffer with column data (left to right)
		for (unsigned int col = 0; col < dibHdr.width; col++)
		{
			// Fills buffer with layer data deepest to
			// shallowest
			for (	int layer = numLayers - 1;
				layer >= 0;
				layer--)
			{
				buffer[bufpos] =
					(char)(image[layer](row,col));

				bufpos++;
			}
		}

		// Writes row data
		image_file.write(buffer,rowsize);	
	}

	// Free buffer memory
	delete[] buffer;
}



#endif
