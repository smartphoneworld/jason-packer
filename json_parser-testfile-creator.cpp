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

int main(int argc, char** argv) {
        string filename = "test_data.json";
	int records = 100;
	if(argc>2)
	{
		filename = argv[1];
		records = atoi(argv[2]);
	}

	// creating a test file
	cout << "Creating a big test file: " << filename << ", Records: " << records << endl;
	std::ofstream testfile(filename);
	for(int t=0; t<records; t++)
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
