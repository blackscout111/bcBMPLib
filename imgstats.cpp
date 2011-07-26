#include <iostream>
#include <string>

#include "matrixmath.h"
#include "bcBMPLib.h"

using namespace std;

int main(int argc, char **argv)
{
	// Image Name
	string img_name;

	// Checks to see the number of arguments is correct.  There should only be
	// one argument.  If it is "-h" display help.  Otherwise use it as the name
	// of the Bitmap.
	if (argc == 2)
	{
		if (argc == 2 && (strcmp(argv[1],"-h") == 0))
		{
			printf("\nimgstats [file name]\n\n");
			exit(EXIT_SUCCESS);
		}
		else
		{
			// Get the name of the image file
			img_name = argv[1];
		}

	}
	else
	{
		printf("Incorrect number of arguments.\n");
		printf("For help try imgstats -h\n");
		exit(EXIT_FAILURE);
	}


	// Calculate and display the image statistics
	bcBMPLib::imgstats(img_name);


	// Pause program before termination
	printf("Press <Enter> to continue...\n");
	cin.get();

	return EXIT_SUCCESS;
}
