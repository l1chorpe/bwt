files := $(wildcard *.cpp)
executable := bwt

$(executable): $(files)
	g++ -o $(executable) $(files)