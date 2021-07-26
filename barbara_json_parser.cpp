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

#include <nlohmann/json.hpp>
using namespace std;
// for convenience
using json = nlohmann::json;

int main(int argc, char** argv)
{
	string filename = "test_data.json";

	if(argc>2)
	{
		filename = argv[1];
	}

	vector<string> unique_keys;

	// open the large json file
	cout << "opening json file: " << filename << endl;
	std::ifstream recordsfile(filename);

	// write then new json in raw and in TLV.. raw is for debugging
	std::ofstream rawfile(filename + ".processed.raw");
	std::fstream tlvfile;
	tlvfile.open(filename + ".processed.tvl", std::ios::out | std::ios::binary);

	// loop on each line
	std::string line;
	while (std::getline(recordsfile, line))
	{
		cout << "processing: " << line << endl;

		// json parse each line
		//auto j3 = json::parse(R"({"happy": true, "pi": 3.14, "Likes":true})");
		auto j3 = json::parse(line);
		std::cout << "Parsed: " << j3 << std::endl;
		json jnew;

		// write number of items in json record.. then serially add keys as int32 and values as strings or boolean, double, or integers.
		char number_of_params = j3.size();
		tlvfile<<number_of_params;

		// find unique keys and replace
		for (json::iterator it = j3.begin(); it != j3.end(); ++it)
		{
		  std::cout << "Processing: " << it.key() << " : " << it.value() << "\n"; // debugging
		  int new_key=-1;
		  for (int i=0; i<(int)unique_keys.size(); i++)
		  {
				  if(unique_keys[i]==it.key())
				  {
					  new_key=i+1;
					  break;
				  }
		  }
		  if(new_key==-1)
		  {
			  new_key=unique_keys.size()+1;
			  unique_keys.push_back(it.key());
			  cout << "adding new key: " << it.key() << std::endl;
		  }
		  jnew[new_key] = it.value();

		  // writing key in TLV
		  char typ = 1;// integer
		  char length = 4;

		  // writing key in TLV
		  tlvfile << typ << length;
		  tlvfile.write( reinterpret_cast<const char *>(&new_key), sizeof(new_key));

		  // writing value in TLV
		  if ( it.value().is_null() || !it.value().is_primitive() )
			  continue;
		  if (it.value().is_boolean())
		  {
			  typ = 2;// boolean
			  length = 1;
			  tlvfile << typ << length;
			  char value = it.value()==true?1:0;
			  void* data = &value;
			  tlvfile.write(reinterpret_cast<const char *>( data), 1);
		  }
		  else if (it.value().is_number_unsigned())
		  {
			  typ = 7;// uint
			  length = sizeof(unsigned int);
			  tlvfile << typ << length;
			  unsigned int value = it.value();
			  void* data = &value;
			  tlvfile.write(reinterpret_cast<const char *>( data), length);
		  }
		  else if (it.value().is_number_float())
		  {
			  typ = 6;// float
			  length = sizeof(float);
			  tlvfile << typ << length;
			  float value = it.value();
			  void* data = &value;
			  tlvfile.write( reinterpret_cast<const char *>(data), length);
		  }
		  else if (it.value().is_number_integer())
		  {
			  typ = 1;// integer
			  length = sizeof(int);
			  tlvfile << typ << length;
			  int value = it.value();
			  void* data = &value;
			  tlvfile.write( reinterpret_cast<const char *>(data), length);
		  }
		  else if (it.value().is_number())
		  {
			  typ = 5;// float
			  length = sizeof(float);
			  tlvfile << typ << length;
			  float value = it.value();
			  void* data = &value;
			  tlvfile.write( reinterpret_cast<const char *>(data), length);
		  }
		  else if (it.value().is_string())
		  {
			  typ = 4;// string
			  string value=it.value();
			  length = sizeof(char)*value.length();
			  tlvfile << typ << length;

			  tlvfile.write( reinterpret_cast<const char *>(value.c_str()), length);
		  }
		  else
		  {
			  cout << "streaming to TLV is not supported for type: " /*<< it.value().type() */<< endl;
		  }
		}

		rawfile << jnew << std::endl;

	// end of record work
	}
	recordsfile.close();
	tlvfile.close();

	// writing dictionary
	ofstream dictfile( filename + ".dict.txt");
	std::fstream tlvdict;
	tlvdict.open(filename + ".processed.tvl", std::ios::out | std::ios::binary);

	for (int i=0; i<(int)unique_keys.size(); i++)
	{
		if(i>0)
			dictfile << ", ";
		dictfile << unique_keys[i] << ":" << (i+1);// << std::endl;

		// writing dictionary record in TLV (string:index)
		// adding the key string
		char typ = 4;// string
		string value=unique_keys[i];
		char length = sizeof(char)*value.length();
	    tlvdict << typ << length;
	    tlvdict.write( reinterpret_cast<const char *>(value.c_str()), length);

	    // adding the key index to the TLV dictionary
	    typ = 1;// integer
	    int nValue=i+1;
	    length = sizeof(int);
	    tlvdict << typ << length;
	    tlvdict.write( reinterpret_cast<const char *>(&nValue), length);
	}

	dictfile << std::endl;

	dictfile.close();
	tlvdict.close();

	// end
	puts("Farewell (^_^)");
	return EXIT_SUCCESS;
}
