#ifndef bcBMPLib_h
#define bcBMPLib_h

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <string>

#include "bmp_headers.h"
#include "readBMPObj.h"
#include "makeBMPObj.h"
#include "matrix.h"

using namespace std;


////////////////////////////////////////////////////////////////////////////////
// This namespace contains useful Bitmap manipulation and analysis functions
////////////////////////////////////////////////////////////////////////////////
namespace bcBMPLib
{

//______________________________________________________________________________
// Creates a Bitmap image with data contained in an array of matrices. The
// number of layers, 'numLayers' should be equal to 1,3, or 4.
// 
// -	If the number of layers is 1, the image created will be black and white
//		and have a bitdepth of 8.
// -	If the number of layers is 3, the image created will be RGB with a
// 		bitdepth of 24.  The red, green, and blue layers will be created from
// 		the 0th,1st, and 2nd indexed matrices in the array respectively.
// -	If the number of layers is 4, the image created will be a RGB with a
//		bitdepth of 32.  The red, green, blue, and alpha layers will be created
// 		from the 0th,1st, and 2nd indexed matrices in the array respectively.
void makeBMP(matrix<int>* image, int numLayers, string image_name)
{
	// BMP reading object that contains important functions
	makeBMPObj makeObj;

	// File Header
	bmpHdr	hdr;

	// DIB Header
	dibv3	dibHdr;

	// Color Palette
	int	paletteSize = 0;
	char	*palette = NULL;

	// Image file stream
	ofstream	image_file;

	// Checks to make sure that the number of layers is 1, 3, or 4, and that
	// the layers all have the same dimensions
	makeObj.checkLayerNumAndDim(image, numLayers);

	// Gets the image dimensions
	dibHdr.height = image[0].height();
	dibHdr.width = image[0].width();

	// Calculates number of bits/pixel
	dibHdr.numBitsPerPixel = numLayers*sizeof(char)*8;

	// Initializes and fills the color palette if single layered image
	if (numLayers == 1)
	{
		palette = makeObj.fillColorPalette(paletteSize, dibHdr);
	}

	// Calculates and sets the total header size/palette offset
	hdr.offset = hdr.size() + dibHdr.size() + paletteSize;

	// Calculates raw data size
	dibHdr.rawDataSize = dibHdr.height * 4 *
						 ((dibHdr.numBitsPerPixel * dibHdr.width + 31)/32);

	// Calculates file size
	hdr.fileSize = hdr.offset + paletteSize * sizeof(char) + dibHdr.rawDataSize;

	// Opens image file and checks to see if its open
	image_file.open(image_name.c_str(),
			ios::out | ios::binary | ios::app);
	if (!image_file.is_open())
		{
			cerr	<< endl
					<< "bcBMPLib.h: In function "
					<< "'int makeBMP(matrix<int>* image, int numLayers, "
					<< "string image_name)'"
					<< "unsigned char disp= 0)'"
					<< endl
					<< "bcBMPLib.h: error: "
					<< "Failed to open "
					<< image_name 
					<< endl << endl;

			std::exit(EXIT_FAILURE);
		}	

	// Writes bitmap file info header to image file
	image_file.write(hdr.getData(),hdr.size());

	// Writes DIB info header to image file
	image_file.write(dibHdr.getData(),dibHdr.size());

	// Writes palette to file
	if (dibHdr.numPaletteColors > 0)
	{
		image_file.write(palette, paletteSize * sizeof(char));
	}

	// Frees palette memory
	delete[] palette;

	// Write data to image file
	makeObj.writeBMPdata(image, numLayers, image_file, dibHdr);

	// Closes image file
	image_file.close();
}




//______________________________________________________________________________
// Reads in a Bitmap image with name 'image_name' into an array of matrices
// pointed to by 'image'.  Returns the length of the array (0 if error).
// *	Image is an array of image layers (as matrices)
//		(layers are as follows: 0=red, 1=green, 2=blue, 3=transparency)
// *	The value of 'disp' determines whether or not the image file's header
// 		information will be displayed (0 = display, 1 = don't display)
int readBMP(matrix<int>*& image, string image_name, bool disp= false)
{
	// BMP reading object that contains important functions
	readBMPObj readObj;

	// Image file stream
	ifstream	image_file;
	
	// Important image variables
	int	width = 0,		// width of image in pixels
		height = 0,		// height of image in pixels
		bitspp = 0,		// Number of bits per pixel
		plength = 0;	// Number palette colors

	// Color Palette
	palette_elem	*palette = NULL;

	// The number of required image layers
	// (1 for a black and white image, 3 for a color, 4 for color with
	// transparency)
	int	numLayers = 0;

	// Checks to see if the value of disp is allowed
	if ((disp != 0) && (disp != 1))
	{
		// Display error message and terminates the program
		cerr	<< endl
				<< "bcBMPLib.h: In function "
				<< "'int readBMP(matrix<int>*& image, string image_name, "
				<< "unsigned char disp= 0)'"
				<< endl
				<< "bcBMPLib.h: error: "
				<< "disp= " << (unsigned int)disp << " is out of bounds!"
				<< endl
				<< "disp can be 0 or 1"
				<< endl << endl;

		// Exits program
		std::exit(EXIT_FAILURE);
	}	

	// Opens bmp file in binary mode and checks to see if it is open
	image_file.open(image_name.c_str(),
			ios::in | ios::binary);
	if (!image_file.is_open())
		{
			cerr	<< endl
					<< "bcBMPLib.h: In function "
					<< "'int readBMP(matrix<int>*& image, string image_name, "
					<< "unsigned char disp= 0)'"
					<< endl
					<< "bcBMPLib.h: error: "
					<< "Failed to open "
					<< image_name 
					<< endl << endl;

			std::exit(EXIT_FAILURE);
		}	
	
	// Reads in file header information
	// (gets the values of the important image variables)
	readObj.readBMPHdrInfo(width, height, bitspp, image_file, plength, disp);

	// Determine the needed number of image layers.  If necessary read in color
	// palette.
	numLayers = readObj.getNumLayers(bitspp, image_file, palette, plength);

	// Reads in image pixel data
	image = readObj.readPxlData(width,
								height,
								numLayers,
								bitspp,
								image_file,
								palette,
								plength);

	return numLayers;
}




//______________________________________________________________________________
// Calculates useful statistics of the Bitmap file named "img_name" and displays
// them in the terminal window
void imgstats(string img_name)
{
	// Image
	matrix<int>* img = NULL;

	// Number of image layers
	int nlayers = 0;

	// Pointer to the layer name array
	string* layerNamePtr = NULL;

	// Layer name arrays
	string	layer1 = "Gray",
			layers3[3] = {"Red","Green","Blue"},
			layers4[4] = {"Red","Green","Blue", "Alpha"};

	// Descriptive statistics about image
	double	*max = NULL,				// Maximum pixel value
			*min = NULL,				// Minimum pixel value
			*range = NULL,				// max - min
			*mean = NULL,				// Mean pixel value
			*stdev = NULL,				// The population standard deviation of
										// pixel values
			*std_err = NULL,			// The standard error of pixel values
			*stdev_divRange = NULL,		// The stdev divided by range
			*stdev_divMean = NULL;		// The stdev divided bye mean


	// Read in image and display header information
	nlayers = readBMP(img, img_name, true);

	// Allocate memory for image statistics
	max = new double [nlayers];
	min = new double [nlayers];
	range = new double [nlayers];
	mean = new double [nlayers];
	stdev = new double [nlayers];
	std_err = new double [nlayers];
	stdev_divRange = new double [nlayers];
	stdev_divMean = new double [nlayers];

	// Set layerNamePtr to point to the correct layer name array
	switch (nlayers)
	{
		case 1:
			layerNamePtr = &layer1;
			break;

		case 3:
			layerNamePtr = layers3;
			break;

		case 4:
			layerNamePtr = layers4;
			break;
	}

	// Calculate and display layer statistics
	for (int n = 0; n < nlayers; n++)
	{
		max[n] = max2d(img[n]);
		min[n] = min2d(img[n]);
		range[n] = max[n] - min[n];
		mean[n] = mean2d(img[n]);
		stdev[n] = stdev2d(img[n], 0);
		std_err[n] = stdev[n]/
			sqrt((double)(img[n].height()*img[n].width()));
		stdev_divRange[n] = stdev[n]/range[n];
		stdev_divMean[n] = stdev[n]/mean[n];

		cout	<< "==================="
				<< endl
				<< layerNamePtr[n]
				<< " Layer"
				<< endl
				<< "==================="
				<< endl
				<< "Max:                " << max[n] << endl
				<< "Min:                " << min[n] << endl
				<< "Range:              " << range[n] << endl
				<< "Mean:               " << mean[n] << endl
				<< "Standard Deviation: " << stdev[n] << endl
				<< "Standard Error:     " << std_err[n] << endl
				<< "StDev/Range:        " << stdev_divRange[n] << endl
				<< "StDev/Mean:         " << stdev_divMean[n] << endl
				<< endl;
	}

	// Free memory of image statistic variables
	delete[] max;
	delete[] min;
	delete[] range;
	delete[] mean;
	delete[] stdev;
	delete[] std_err;
	delete[] stdev_divRange;
	delete[] stdev_divMean;

	// Free memory of image
	delete[] img;
}




};
////////////////////////////////////////////////////////////////////////////////
// End namespace bcBMPLib
////////////////////////////////////////////////////////////////////////////////


#endif
