#include <iostream>
#include <fstream>
#include "bmpHdr.h"
#include "dibv3.h"

using namespace std;

int main()
{
	cout << "initializing file..." << endl;
	ofstream file;
	
	cout << "opening file..." << endl;
	file.open("test.data", ios::binary);

	cout << "initializing hdr..." << endl << endl;

	bmpHdr	hdr;
	hdr.print();
	cout << endl;

	cout << "initializing dib3Hdr..." << endl << endl;

	dibv3	dib3Hdr;
	dib3Hdr.print();
	cout << endl;

	cout << "closing file..." << endl;
	file.close();

	// Pauses consol before closing
	cout << "Press <ENTER> to continue..." << endl;
	cin.get();

	return 0;
}

