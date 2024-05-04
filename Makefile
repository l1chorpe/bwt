cpp_files := $(wildcard *.cpp)
h_files := $(wildcard *.h)
executable := bwt

$(executable): $(cpp_files) $(h_files)
	g++ -o $(executable) $(cpp_files)