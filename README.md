# jason-packer

Build testing file:
g++ -o testfile-creator barbara_json_parser-testfile-creator.o -O2 -g -Wall -fmessage-length=0 -std=c++11 -Wsign-compare


Building main tool:
git clone  https://github.com/smartphoneworld/jason-packer.git
cd jason-packer
make

Run:
./barbara_json_parser test_data.json


