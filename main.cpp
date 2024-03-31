#include "bwt.h"
#include "rle.h"

#include <bitset>
#include <cstring>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using std::cout;
using std::strcmp;
using std::string;
using std::vector;

typedef std::bitset<3> Flags;

vector<string>& getFileContent(const string& source);
void saveFileContent(const vector<string>& fileContent, const string& filename, const string& extension = "");
bool handleCLIArgs(int argc, char* argv[]);
void handleData(int dataPos, char* argv[], Flags flags = Flags{0b011});



int main(int argc, char* argv[])
{
    handleCLIArgs(argc, argv);
    return 0;
}

/**
 * @brief Extracts each word from a given file.
 * 
 * @param source The name of the file
 * @return vector<string>& The list of words
 */
vector<string>& getFileContent(const string& source)
{
    std::ifstream stream{source};
    string element{};
    static vector<string> content{};
    
    while(std::getline(stream, element))
        content.push_back(element);
    
    return content;
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
}

/**
 * @brief Checks the CLI arguments, and passes the data to handleData and returns true, or returns false if the args
 * aren't valid.
 * 
 * @param argc The argument count
 * @param argv The arguments themselves
 * 
 * @return True if successful, false otherwise
 */
bool handleCLIArgs(int argc, char* argv[])
{
    // Avoid unwanted behavior/longer code by handling the simplest case in isolation
    if(argc == 2)
    {
        handleData(1, argv);
        return true;
    }
    
    // Set the flags accordingly regardless of the order of the operators
    // It doesn't catch repeating operators but I think it's okay
    Flags flags{0b000};
    for(int i = 1; i < argc - 1; ++i)
    {
        if(strcmp(argv[i], "-x") == 0 || strcmp(argv[i], "--extract") == 0)
            flags.set(2);
        if(strcmp(argv[i], "-b") == 0 || strcmp(argv[i], "--bwt") == 0)
            flags.set(1);
        if(strcmp(argv[i], "-r") == 0 || strcmp(argv[i], "--rle") == 0)
            flags.set(0);

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
    {
        handleData(argc - 1, argv, flags);
        return true;
    }

    return false;
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