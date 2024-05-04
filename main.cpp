#include "bwt.h"
#include "rle.h"
#include "ArgException.h"
#include "FileException.h"

#include <bitset>
#include <cstring>
#include <cctype>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using std::strcmp;
using std::string;
using std::vector;

typedef std::bitset<3> Flags;

vector<string>& getFileContent(const string& source);
bool isEmpty(const vector<string>& v);
void saveFileContent(const vector<string>& fileContent, const string& filename, const string& extension = "");
Flags handleCLIArgs(int argc, char* argv[]);
void handleData(int dataPos, char* argv[], Flags flags = Flags{0b011});
void displayUsage(std::ostream& displayStream);



int main(int argc, char* argv[])
{
    try
    {
        Flags cmd_flags = handleCLIArgs(argc, argv);
        if(cmd_flags.any())
            handleData(argc - 1, argv, cmd_flags);
        else
            // This is reached only if -h/--help is used, hence using cout instead of cerr
            displayUsage(std::cout);
        
    }
    catch(const ArgException& e)
    {
        std::cerr << "Error: " << e.what() << "\n\n";
        displayUsage(std::cerr);
        return EXIT_FAILURE;
    }
    catch(const FileException& e)
    {
        std::cerr << "Error: " << e.what() << '\n';
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

/**
 * @brief Extracts each word from a given file.
 * 
 * @param source The name of the file
 * 
 * @return vector<string>& The list of words
 * 
 * @exception FileReadException Thrown when a file can't be read.
 * @exception EmptyFileException Thrown when a file is empty.
 */
vector<string>& getFileContent(const string& source)
{
    std::ifstream stream{source};
    if(!stream)
        throw FileReadException();

    string element{};
    static vector<string> content{};
    
    while(std::getline(stream, element))
        content.push_back(element);
    
    stream.close();

    if(isEmpty(content))
        throw EmptyFileException();
    
    return content;
}

/**
 * @brief Checks if a vector of strings is empty or contains only whitespace.
 * 
 * @param v The vector to check
 * @return true If it's empty or contains only whitespace
 * @return false Otherwise
 */
bool isEmpty(const vector<string>& v)
{
    if(v.empty())
        return true;
    
    // If at least one character is not whitespace, the file is not empty
    for(const string& s : v)
        for(char c : s)
            if(!std::isspace(c))
                return false;

    return true;
}

/**
 * @brief Saves the data provided into a new file with the given name and extension. The default is no extension.
 * 
 * @param fileContent The content to save
 * @param filename The name of the file
 * @param extension The extension for the file
 */
void saveFileContent(const vector<string>& fileContent, const string& filename, const string& extension)
{
    // Strip the extensions before saving the file
    const string name = filename.substr(0, filename.find(".br"))
                                .substr(0, filename.find(".bwt"))
                                .substr(0, filename.find(".rle"));
    std::ofstream stream{name + extension};
    for(const string& element : fileContent)
        stream << element << '\n';
    stream.close();

    if(!stream)
        throw FileWriteException();
}

/**
 * @brief Checks the CLI arguments, and passes the data to handleData and returns true, or returns false if the args
 * aren't valid.
 * 
 * @param argc The argument count
 * @param argv The arguments themselves
 * 
 * @return The flags set in the command line
 * 
 * @exception MissingArgsException Thrown when the program is called without arguments.
 * @exception InvalidArgCountException Thrown when the program is called with more than 4 arguments.
 * @exception InvalidArgsException Thrown when invalid flags have been set.
 */
Flags handleCLIArgs(int argc, char* argv[])
{
    // Throw if no or more than 4 command line args are given
    if(argc == 1)
        throw MissingArgsException();
    if(argc > 5)
        throw InvalidArgCountException();

    // Avoid unwanted behavior/longer code by handling the simplest case in isolation
    // Meaning that anytime there's only one argument, it's treated as a file name, except for "-h"/"--help"
    if(argc == 2)
    {
        if(strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0)
            return {0b000};
        
        return {0b011};
    }
    
    // Set the flags accordingly regardless of the order of the operators
    // It doesn't catch invalid mixes of operators like -xb -br but I don't care for now
    Flags flags{0b000};
    for(int i = 1; i < argc - 1; ++i)
    {
        // Individual flags
        if(strcmp(argv[i], "-x") == 0 || strcmp(argv[i], "--extract") == 0)
            flags.set(2);
        if(strcmp(argv[i], "-b") == 0 || strcmp(argv[i], "--bwt") == 0)
            flags.set(1);
        if(strcmp(argv[i], "-r") == 0 || strcmp(argv[i], "--rle") == 0)
            flags.set(0);

        // Grouped flags
        if(strcmp(argv[i], "-br") == 0)
        {
            flags.set(1);
            flags.set(0);
        }
        if(strcmp(argv[i], "-xb") == 0)
        {
            flags.set(2);
            flags.set(1);
        }
        if(strcmp(argv[i], "-xr") == 0)
        {
            flags.set(2);
            flags.set(0);
        }
        if(strcmp(argv[i], "-xbr") == 0)
        {
            flags.set(2);
            flags.set(1);
            flags.set(0);
        }
    }

    // If flags is still 0, the CLI args are invalid
    if(flags.any())
        return flags;

    throw InvalidArgsException();
}

/**
 * @brief Compresses or decompresses the data according to the flags; left bit: compress/decompress, middle bit: BWT
 * off/on, right bit: RLE off/on. Default is compression with BWT and RLE.
 * 
 * @param dataPos The position of the filename in argv
 * @param argv The CLI arguments
 * @param flags Flags set to represent what operations to do on the data
 */
void handleData(int dataPos, char* argv[], Flags flags)
{
    // The nesting order is important: when compressing, BWT is done before RLE and inversely for decompression
    auto& content = getFileContent(argv[dataPos]);
    if(!flags[2])
    {
        if(flags[1])
            BWT::transform(content);
        if(flags[0])
            RLE::encode(content);
        
        if(flags[1] && flags[0])
            saveFileContent(content, argv[dataPos], ".br");
        else if(flags[1])
            saveFileContent(content, argv[dataPos], ".bwt");
        else
            saveFileContent(content, argv[dataPos], ".rle");
    }
    else
    {
        // Safeguard for when -x is used alone
        if(!flags[1] && !flags[0])
            flags |= 0b111;
        
        if(flags[0])
            RLE::decode(content);
        if(flags[1])
            BWT::untransform(content);
        saveFileContent(content, argv[dataPos]);
    }
}

/**
 * @brief Outputs the usage "manual" of this program.
 * 
 * @param displayStream The stream which should be output to 
 */
void displayUsage(std::ostream& displayStream)
{
    displayStream << "Usage: bwt [filename] <options>\n\n"

                  << "Options:\n"
                  << "    -b, --bwt: Restricts the compression/decompression to BWT.\n\n"
                  << "    -h, --help: Displays this page.\n\n"
                  << "    -r, --rle: Restricts the compression/decompression to RLE.\n\n"
                  << "    -x, --extract: Extracts the data from the compressed file. If no other options are used, it uses"
                     " BWT and RLE by default.\n\n"

                  << "The program compresses when used without -x. Flags can be shortened by grouping them with a single"
                     " hyphen:\n"
                  << "'bwt -x -r' can be shortened to 'bwt -xr'.\n";
}