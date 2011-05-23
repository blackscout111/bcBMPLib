#ifndef readBMPObj_h
#define readBMPObj_h

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <string>

#include "dibv3.h"
#include "bmpHdr.h"
#include "matrix.h"

using namespace std;



//______________________________________________________________________________
// Pallet entry structure
struct palette_elem
{
	unsigned char red, green, blue, trans;
} typedef palette_elem;



//______________________________________________________________________________
// This class contains all of the image reading functions required by the
// function readBMP
class readBMPObj
{
	public:

		// Reads in the Bitmap file information header and the DIB header.
		void readBMPHdrInfo(int& width,
							int& height,
							int& bitspp,
							ifstream& image_file,
							int& plength,
							unsigned char disp);

		// Reads in color palette and returns the needed number of layers
		// *** Only call this function after calling readBMPHdrInfo(...)
		int readColorPalette(ifstream& image_file,
							 palette_elem*& palette,
							 int plength);

		// If necessary, read in color palette and determine number of layers
		// (numLayers) needed by looking at indexed colors of the color palette
		// ***	'numLayers' will be > 0 if the color palette is read in.
		int getNumLayers(int bitspp,
						 ifstream& image_file,
						 palette_elem*& palette,
						 int& plength);

		// Reads in pixel data from image file and fills buffer matrix with it
		unsigned char** fillBuff(int width,
								 int height,
								 int bitspp,
								 ifstream& image_file);

		// The following 7 'fromBuff2Img functions read data from a 2D buffer
		// matrix (who's rows correspond to the image rows) into the image
		// pointer assuming the bitmap file has a bitdepth equal to the number
		// at the end of the function name.
		// *** 'image' is a matrix array of length 3
		void fromBuff2Img1(int width,
						   int height,
						   int numLayers,
						   matrix<int>* image,
						   palette_elem* palette,
						   int plength,
						   unsigned char** buffer);

		void fromBuff2Img2(int width,
						   int height,
						   int numLayers,
						   matrix<int>* image,
						   palette_elem* palette,
						   int plength,
						   unsigned char** buffer);

		void fromBuff2Img4(int width,
						   int height,
						   int numLayers,
						   matrix<int>* image,
						   palette_elem* palette,
						   int plength,
						   unsigned char** buffer);

		void fromBuff2Img8(int width,
						   int height, int numLayers,
						   matrix<int>* image,
						   palette_elem* palette,
						   int plength,
						   unsigned char** buffer);

		void fromBuff2Img16(int width,
							int height,
							matrix<int>* image,
							unsigned char** buffer);

		void fromBuff2Img24(int width,
							int height,
							matrix<int>* image,
							unsigned char** buffer);

		void fromBuff2Img32(int width,
							int height,
							matrix<int>* image,
							unsigned char** buffer);

		// Reads in pixel data from the Bitmap and returns a pointer to an array
		// of matrices which correspond to the image layers
		matrix<int>* readPxlData(int width,
								 int height,
								 int numLayers,
								 int bitspp,
								 ifstream& image_file,
								 palette_elem* palette,
								 int plength);
};



////////////////////////////////////////////////////////////////////////////////
// FUNCTION DEFINITIONS FOR readBMPObj
////////////////////////////////////////////////////////////////////////////////

//______________________________________________________________________________
void readBMPObj::readBMPHdrInfo(int& width,
								int& height,
								int& bitspp,
								ifstream& image_file,
								int& plength,
								unsigned char disp)
{
	// File info header
	bmpHdr	hdr;

	// Pointer to the DIB header object
	void*	dibHdrPtr;

	// Reads in file info header
	hdr.readData(image_file);

	// Prints the file info header
	if (disp == 1)
	{
		hdr.print();
		cout << endl;
	}

	// Determins version of and reads in DIB header
	switch (hdr.dibHdrSize)
	{
		// If version 3
		case 40:
			
			// Allocate memory for DIB header
			dibHdrPtr = new dibv3;

			// Read is DIB header data
			((dibv3*)dibHdrPtr)->readData(image_file);

			// Print DIB header data
			if (disp == 1)
			{
				((dibv3*)dibHdrPtr)->print();
				cout << endl;
			}

			// Gets important image variables from DIB header
			width = ((dibv3*)dibHdrPtr)->width;
			height = ((dibv3*)dibHdrPtr)->height;
			plength = ((dibv3*)dibHdrPtr)->numPaletteColors;
			bitspp = ((dibv3*)dibHdrPtr)->numBitsPerPixel;

			// Free DIB header memory
			delete ((dibv3*)dibHdrPtr);

			break;

		// If version not supported
		default:

			// Display error message
			cout	<< endl
					<< "bcBMPLib.h: In function "
					<< "'int readBMP(matrix<int>*& image, string image_name, "
					<< "unsigned char disp= 0)'"
					<< endl
					<< "bcBMPLib.h: error: "
					<< "DIB header version not supported (size= "
					<< hdr.dibHdrSize << ")"
					<< endl << endl;

			image_file.close();

			exit(-1);
	}
}



//______________________________________________________________________________
int readBMPObj::readColorPalette(ifstream& image_file,
								palette_elem*& palette,
								int plength)
{
	int numLayers = 1;

	// Allocates memory for palette
	palette = new palette_elem[plength];

	// Fill palette
	for (int i = 0; i < plength; i++)
	{
		image_file.read((char*)(&palette[i].blue),sizeof(palette[i].blue));
		image_file.read((char*)(&palette[i].green),sizeof(palette[i].green));
		image_file.read((char*)(&palette[i].red),sizeof(palette[i].red));
		image_file.read((char*)(&palette[i].trans),sizeof(palette[i].trans));

		// Check for RGB color
		if (numLayers < 3)
		{
			if ((palette[i].red != palette[i].green) ||
				(palette[i].red != palette[i].blue) ||
				(palette[i].green != palette[i].blue))
			{
					numLayers = 3;
			}
		}

		// Check for alpha layer
		if (numLayers < 4)
		{
			if (palette[i].trans != 255)
			{
				numLayers = 4;
			}
		}
	}

	return numLayers;
}



//______________________________________________________________________________
int readBMPObj::getNumLayers(int bitspp,
							 ifstream& image_file,
							 palette_elem*& palette,
							 int& plength)
{
	// Number of image layers
	int numLayers = 0;

	// If there are entries in the color palette then read in the color palette.
	if (plength != 0)
	{
		numLayers = readColorPalette(image_file, palette, plength);
		
		if (bitspp > 8)
		{
			cout	<< endl
					<< "bcBMPLib.h: In function "
					<< "'int readBMP(matrix<int>*& image, string image_name, "
					<< "unsigned char disp= 0)'"
					<< endl
					<< "bcBMPLib.h: error: "
					<< "WARNING: Unnecessary color palette"
					<< endl << endl;
		}
	}


	// Calculates number of layers needed
	if (bitspp <= 8)
	{
		// Check for a color palette of default length
		// *	If plength doesn't equal 0 then the color palette would have
		// 		already been read in and the number of layers would already have
		// 		been calculated
		if (plength == 0)
		{
			switch (bitspp)
			{
				case 1:
					plength = 2;
					break;

				case 2:
					plength = 4;
					break;

				case 4:
					plength = 16;
					break;

				case 8:
					plength = 256;
					break;
			}

			numLayers = readColorPalette(image_file, palette, plength);
		}
	}
	else if ((bitspp == 16) || (bitspp == 24))
	{
		numLayers = 3;
	}
	else if (bitspp == 32)
	{
		numLayers = 4;
	}
	else
	{
			cout	<< endl
					<< "bcBMPLib.h: In function "
					<< "'int readBMP(matrix<int>*& image, string image_name, "
					<< "unsigned char disp= 0)'"
					<< endl
					<< "bcBMPLib.h: error: "
					<< "Bitdepth out of range"
					<< endl << endl;
	}

	return numLayers;
}



//______________________________________________________________________________
unsigned char** readBMPObj::fillBuff(int width,
									 int height,
									 int bitspp,
									 ifstream& image_file)
{
	// A buffer for the pixel data
	unsigned char** buffer = NULL;

	// Number of bytes per image pixel data row
	int	rowSize = 0;
	
	// Number of padding bytes for each image pixel data row
	int	numPad = 0;

	// Calculates rowsize
	rowSize = 4*((bitspp * width + 31)/32);

	// Calculates number of padded bytes
	numPad = rowSize - (bitspp * width + 7)/8;

	// Alocates memory for and fills buffer with image data
	buffer = new unsigned char* [height];
	for (int row = (height - 1); row >= 0; row--)
	{
		buffer[row] = new unsigned char [rowSize - numPad];
		image_file.read((char*)buffer[row], (rowSize - numPad));
		image_file.seekg(numPad, ios::cur);
	}

	// Closes the image file stream(s)
	image_file.close();

	return buffer;
}



//______________________________________________________________________________
void readBMPObj::fromBuff2Img1(int width,
							   int height,
							   int numLayers,
							   matrix<int>* image,
							   palette_elem* palette,
							   int plength,
							   unsigned char** buffer)
{
	for (int row = 0; row < height; row++)
	{
		for (int col = 0; col < width; col += 8)
		{
			int value[8];

			// Get the two values per buffer bit
			// ('col' is divided by 8 for the buffer index)
			for (int i = 0; i < 8; i++)
			{
				value[i] = (0x01 && (buffer[row][col>>3] >> (7 - i)));

				// Check to make sure the pixel value is in the range of the
				// color palette.  Display error message and use index 0 if out
				// of range.
				if (value[i] >= plength)
				{
					cout	<< endl
							<< "bcBMPLib.h: In function "
							<< "'int readBMP(matrix<int>*& image, string "
							<< "image_name, unsigned char disp= 0)'"
							<< endl
							<< "bcBMPLib.h: error: "
							<< "Pixel value out of palette range, using 0"
							<< endl << endl;
					value[i] = 0;
				}

				switch (numLayers)
				{
					case 1:
						// Reads in the grayscale value
						// (the red, green, and blue values are all the same)
						image[0](row,(col + i)) = palette[value[i]].red;
						break;

					case 3:
						// Reads the blue value
						image[2](row,(col + i)) = palette[value[i]].blue;

						// Reads the green value
						image[1](row,(col + i)) = palette[value[i]].green;

						// Reads the red value
						image[0](row,(col + i)) = palette[value[i]].red;
						break;

					case 4:
						// Reads the alpha value
						image[3](row,(col + i)) = palette[value[i]].trans;

						// Reads the blue value
						image[2](row,(col + i)) = palette[value[i]].blue;

						// Reads the green value
						image[1](row,(col + i)) = palette[value[i]].green;

						// Reads the red value
						image[0](row,(col + i)) = palette[value[i]].red;
						break;
				}
			}
		}

		// Frees buffer memory for the current row
		delete[] buffer[row];
	}

	// Frees Buffer memory
	delete[] buffer;
}



//______________________________________________________________________________
void readBMPObj::fromBuff2Img2(int width,
							   int height,
							   int numLayers,
							   matrix<int>* image,
							   palette_elem* palette,
							   int plength,
							   unsigned char** buffer)
{
	for (int row = 0; row < height; row++)
	{
		for (int col = 0; col < width; col += 4)
		{
			int value[4];

			// Get the two values per buffer bit
			// ('col' is divided by 4 for the buffer index)
			for (int i = 0; i < 4; i++)
			{
				value[i] = (0x03 && (buffer[row][col>>2] >> (6 - 2*i)));

				// Check to make sure the pixel value is in the range of the
				// color palette.  Display error message and use index 0 if out
				// of range.
				if (value[i] >= plength)
				{
					cout	<< endl
							<< "bcBMPLib.h: In function "
							<< "'int readBMP(matrix<int>*& image, string"
							<< " image_name, unsigned char disp= 0)'"
							<< endl
							<< "bcBMPLib.h: error: "
							<< "Pixel value out of palette range, using 0"
							<< endl << endl;
					value[i] = 0;
				}

				switch (numLayers)
				{
					case 1:
						// Reads in the grayscale value
						// (the red, green, and blue values are all the same)
						image[0](row,(col + i)) = palette[value[i]].red;
						break;

					case 3:
						// Reads the blue value
						image[2](row,(col + i)) = palette[value[i]].blue;

						// Reads the green value
						image[1](row,(col + i)) = palette[value[i]].green;

						// Reads the red value
						image[0](row,(col + i)) = palette[value[i]].red;
						break;

					case 4:
						// Reads the alpha value
						image[3](row,(col + i)) = palette[value[i]].trans;

						// Reads the blue value
						image[2](row,(col + i)) = palette[value[i]].blue;

						// Reads the green value
						image[1](row,(col + i)) = palette[value[i]].green;

						// Reads the red value
						image[0](row,(col + i)) = palette[value[i]].red;
						break;
				}
			}
		}

		// Frees buffer memory for the current row
		delete[] buffer[row];
	}

	// Frees Buffer memory
	delete[] buffer;
}



//______________________________________________________________________________
void readBMPObj::fromBuff2Img4(int width,
							   int height,
							   int numLayers,
							   matrix<int>* image,
							   palette_elem* palette,
							   int plength,
							   unsigned char** buffer)
{
	for (int row = 0; row < height; row++)
	{
		for (int col = 0; col < width; col += 2)
		{
			int value[2];

			// Get the two values per buffer bit
			// ('col' is divided by 2 for the buffer index)
			for (int i = 0; i < 2; i++)
			{
				value[i] = 0x0F && (buffer[row][col>>1] >> (4 - 4*i));

				// Check to make sure the pixel value is in the range of the
				// color palette.  Display error message and use index 0 if out
				// of range.
				if (value[i] >= plength)
				{
					cout	<< endl
							<< "bcBMPLib.h: In function "
							<< "'int readBMP(matrix<int>*& image, string "
							<< "image_name, unsigned char disp= 0)'"
							<< endl
							<< "bcBMPLib.h: error: "
							<< "Pixel value out of palette range, using 0"
							<< endl << endl;
					value[i] = 0;
				}

				switch (numLayers)
				{
					case 1:
						// Reads in the grayscale value
						// (the red, green, and blue values are all the same)
						image[0](row,(col + i)) = palette[value[i]].red;
						break;

					case 3:
						// Reads the blue value
						image[2](row,(col + i)) = palette[value[i]].blue;

						// Reads the green value
						image[1](row,(col + i)) = palette[value[i]].green;

						// Reads the red value
						image[0](row,(col + i)) = palette[value[i]].red;
						break;

					case 4:
						// Reads the alpha value
						image[3](row,(col + i)) = palette[value[i]].trans;

						// Reads the blue value
						image[2](row,(col + i)) = palette[value[i]].blue;

						// Reads the green value
						image[1](row,(col + i)) = palette[value[i]].green;

						// Reads the red value
						image[0](row,(col + i)) = palette[value[i]].red;
						break;
				}
			}
		}

		// Frees buffer memory for the current row
		delete[] buffer[row];
	}

	// Frees Buffer memory
	delete[] buffer;
}


//______________________________________________________________________________
void readBMPObj::fromBuff2Img8(int width,
							   int height, int numLayers,
							   matrix<int>* image,
							   palette_elem* palette,
							   int plength,
							   unsigned char** buffer)
{
	for (int row = 0; row < height; row++)
	{
		for (int col = 0; col < width; col++)
		{
			// Check to make sure the pixel value is in the range of the color
			// palette.  Display error message and use index 0 if out of range.
			int value = buffer[row][col];
			if (value >= plength)
			{
				cout	<< endl
						<< "bcBMPLib.h: In function "
						<< "'int readBMP(matrix<int>*& image, string "
						<< "image_name, unsigned char disp= 0)'"
						<< endl
						<< "bcBMPLib.h: error: "
						<< "Pixel value out of palette range, using 0"
						<< endl << endl;
				value = 0;
			}

			switch (numLayers)
			{
				case 1:
					// Reads in the grayscale value
					// (the red, green, and blue values are all the same)
					image[0](row,col) = palette[value].red;
					break;

				case 3:
					// Reads the blue value
					image[2](row,col) = palette[value].blue;

					// Reads the green value
					image[1](row,col) = palette[value].green;

					// Reads the red value
					image[0](row,col) = palette[value].red;
					break;

				case 4:
					// Reads the alpha value
					image[3](row,col) = palette[value].trans;

					// Reads the blue value
					image[2](row,col) = palette[value].blue;

					// Reads the green value
					image[1](row,col) = palette[value].green;

					// Reads the red value
					image[0](row,col) = palette[value].red;
					break;
			}
		}

		// Frees buffer memory for the current row
		delete[] buffer[row];
	}

	// Frees Buffer memory
	delete[] buffer;
}



//______________________________________________________________________________
void readBMPObj::fromBuff2Img16(int width,
								int height,
								matrix<int>* image,
								unsigned char** buffer)
{
	for (int row = 0; row < height; row++)
	{
		for (int col = 0; col < width; col++)
		{
			// Reads the blue value (5 bits)
			image[2](row,col) =
				buffer[row][2*col] & (char)0x1f;

			// Reads the green value (6 bits)
			image[1](row,col) =
				buffer[row][2*col + 1] & (char)0x03;
			image[1](row,col) <<= 3;
			image[1](row,col) |= (buffer[row][2*col] >> 5);

			// Reads the red value (5 bits)
			image[0](row,col) =
				(char)0x1f & (buffer[row][2*col + 1] >> 2);
		}

		// Frees buffer memory for the current row
		delete[] buffer[row];
	}

	// Frees Buffer memory
	delete[] buffer;
}



//______________________________________________________________________________
void readBMPObj::fromBuff2Img24(int width,
								int height,
								matrix<int>* image,
								unsigned char** buffer)
{
	for (int row = 0; row < height; row++)
	{
		for (int col = 0; col < width; col++)
		{
			for (int layer = 0; layer < 3; layer++)
			{
				image[layer](row,col) =
					buffer[row][2 + 3*col - layer];
			}
		}

		// Frees buffer memory for the current row
		delete[] buffer[row];
	}

	// Frees Buffer memory
	delete[] buffer;
}



//______________________________________________________________________________
void readBMPObj::fromBuff2Img32(int width,
								int height,
								matrix<int>* image,
								unsigned char** buffer)
{
	for (int row = 0; row < height; row++)
	{
		for (int col = 0; col < width; col++)
		{
			for (int layer = 0; layer < 4; layer++)
			{
				image[layer](row,col) =
					buffer[row][3 + 4*col - layer];
			}
		}

		// Frees buffer memory for the current row
		delete[] buffer[row];
	}

	// Frees Buffer memory
	delete[] buffer;
}




//______________________________________________________________________________
matrix<int>* readBMPObj::readPxlData(int width,
									 int height,
									 int numLayers,
									 int bitspp,
									 ifstream& image_file,
									 palette_elem* palette,
									 int plength)
{
	// Pointer to image
	matrix<int>* image = NULL;

	// Temporary 2D data buffer
	unsigned char** buffer = NULL;

	// Allocates memory for image
	image = new matrix <int> [numLayers];
	
	// Resizes image variable and fills with zeros
	for (int layer = 0; layer < numLayers; layer++)
	{
		image[layer](0,0) = 0;
		image[layer].grow(0, (height - 1),
				      	  0, (width - 1),
				      	  0);
	}

	// Fills buffer with pixel image data
	buffer = fillBuff(width, height, bitspp, image_file);

	// Reads from buffer into image
	switch (bitspp)
	{
		// For 1-bit Images
		case 1:
			fromBuff2Img1(width, height, numLayers,
						  image, palette, plength, buffer);
			break;

		// For 2-bit Images
		case 2:
			fromBuff2Img2(width, height, numLayers,
						  image, palette, plength, buffer);
			break;

		// For 4-bit Images
		case 4:
			fromBuff2Img4(width, height, numLayers,
						  image, palette, plength, buffer);
			break;

		// For 8-bit Images
		case 8:
			fromBuff2Img8(width, height, numLayers,
						  image, palette, plength, buffer);
			break;

		// For 16-bit Images
		case 16:
			fromBuff2Img16(width, height, image, buffer);
			break;

		// For 24-bit Images
		case 24:
			fromBuff2Img24(width, height, image, buffer);
			break;

		// For 32-bit Images
		case 32:
			fromBuff2Img32(width, height, image, buffer);
			break;

		default:
			// Display error message
			cout	<< endl
					<< "bcBMPLib.h: In function "
					<< "'int readBMP(matrix<int>*& image, string image_name, "
					<< "unsigned char disp= 0)'"
					<< endl
					<< "bcBMPLib.h: error: "

					<< "bitdepth (" 
					<< bitspp
					<< ") not supported"
					<< endl;

			// Closes the image file stream
			image_file.close();

			// Frees palette memory
			delete[] palette;

			// Frees the memory of the file streams and the image
			delete[] image;

			// Terminates the program
			exit(-1);
	}

	// Frees palette memory if necessary
	if (plength > 0) delete[] palette;

	return image;
}



#endif
