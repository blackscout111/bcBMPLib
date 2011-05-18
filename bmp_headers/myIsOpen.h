#ifndef myIsOpen_h
#define myIsOpen_h

#include <iostream>
#include <fstream>
#include <string>

using namespace std;

// Checks to see if an fstream file is open under 'fileName'

void myIsOpen(fstream& file, const string& fileName)
{
	if (!file.is_open())
	{
		cout	<< "ERROR: Failed to open "
			<< fileName
			<< endl
			<< "Press <ENTER> to continue...";
		
		cin.get();
		cin.get();

		exit(-1);
	}
}


void myIsOpen(ifstream& file, const string& fileName)
{
	if (!file.is_open())
	{
		cout	<< "ERROR: Failed to open "
			<< fileName
			<< endl
			<< "Press <ENTER> to continue...";
		
		cin.get();
		cin.get();

		exit(-1);
	}
}

void myIsOpen(ofstream& file, const string& fileName)
{
	if (!file.is_open())
	{
		cout	<< "ERROR: Failed to open "
			<< fileName
			<< "!"
			<< endl
			<< "Press <ENTER> to continue...";
		
		cin.get();
		cin.get();
		exit(-1);
	}
}


#endif
