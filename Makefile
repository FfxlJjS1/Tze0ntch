CXX			= g++
CXXFLAGS	= -g -c -Wall
LDFLAGS		= 
LDLIBS		= 
OBJECTS		= 
EXECUTABLE	= 

SRC_PATH = ./src

OBJECTS = main.o DbController.o SemanticWeb.o Node.o NetInterface.o Iis.o RecursionParser.o Interpretator.o

Release: $(OBJECTS)
	$(CXX) $(LDFLAGS) $^ -o $@ $(LDLIBS)

Debug: 
	$(CXX) $(LDFLAGS) $^ -o $@ $(LDLIBS)


main.o: main.h main.cpp
	$(CXX) -c -o main.o $^

Iis.o: Iis.h Iis.cpp
	$(CXX) -c -o Iis.o $^

Interpretator.o: Interpretator.h Interpretator.cpp
	$(CXX) -c -o Interpretator.o $^

RecursionParser.o: RecursionParser.h RecursionParser.cpp
	$(CXX) -c -o RecursionParser.o $^

NetInterface.o: NetInterface.h
	$(CXX) -c -o NetInterface.o $^

DbController.o: DbController.h DbController.cpp
	$(CXX) -c -o DbController.o $^

SemanticWeb.o: SemanticWeb.h SemanticWeb.cpp
	$(CXX) -c -o SemanticWe.o $^

Node.o: Node.h Node.cpp
	$(CXX) -c -o Node.o $^


.PHONY clean
clean:
	rm -f $(OBJECTS) $(EXECUTABLE)
	@echo Clean done!