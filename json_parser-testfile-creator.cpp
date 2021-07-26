//============================================================================
// Name        : barbara_json_parser.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C, Ansi-style
//============================================================================

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

using namespace std;

int main(void) {

	string filename = "test_data.json";
	// creating a test file
	cout << "Creating a big test file: " << filename << endl;
	std::ofstream testfile(filename);
	for(int t=0; t<100000; t++)
	{
		int rnd = rand() % 100; // 100 unqiue keys
		string record = "{\"happy\": true, \"pi\": 3.14, \"Likes\":false, \"key"+to_string(rnd)+"\":\"Key test value\"}";
		testfile << record << endl;
		if(t % 1000 == 0)
			cout << ".";
	}
	testfile.close();
	cout << endl << "test file created: " << filename << endl;
	return EXIT_SUCCESS;
}
