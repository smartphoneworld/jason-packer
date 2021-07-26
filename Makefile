CXXFLAGS =	-O2 -g -Wall -fmessage-length=0 -std=c++11 -Wsign-compare

OBJS =		barbara_json_parser.o

LIBS =

TARGET =	barbara_json_parser

$(TARGET):	$(OBJS)
	$(CXX) -o $(TARGET) $(OBJS) $(LIBS)

all:	$(TARGET)

clean:
	rm -f $(OBJS) $(TARGET)
