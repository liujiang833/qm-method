CXXFLAGS= -g -std=c++17

all: qmdev

qmdev: main.o qm.o
	$(CXX) $(CXXFLAGS) qm.o main.o -o qmdev

qm.o: qm.cpp qm.h
	$(CXX) $(CXXFLAGS) qm.cpp -c -o qm.o

main.o: main.cpp qm.h
	$(CXX) $(CXXFLAGS) main.cpp -c -o main.o 

