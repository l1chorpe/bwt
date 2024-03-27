#include "bwt.h"
#include "rle.h"

#include <cstring>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using std::cout;
using std::strcmp;
using std::string;
using std::vector;

vector<string>& getFileContent(const string& source);
void saveFileContent(const vector<string>& fileContent, const string& filename, const string& extension);


int main(int argc, char* argv[])
{
    if(argc == 4)
    {
        if(strcmp(argv[1], "-x") == 0)
        {
            if(strcmp(argv[2], "-r") == 0)
            {
                auto& content = getFileContent(argv[3]);
                RLE::decode(content);
                saveFileContent(content, argv[3], "");
            }
            else if(strcmp(argv[2], "-b") == 0)
            {
                auto& content = getFileContent(argv[3]);
                BWT::untransform(content);
                saveFileContent(content, argv[3], "");
            }
        }
    }
    if(argc == 3)
    {
        if(strcmp(argv[1], "-r") == 0)
        {
            auto& content = getFileContent(argv[2]);
            RLE::encode(content);
            saveFileContent(content, argv[2], ".rle");
        }
        else if(strcmp(argv[1], "-b") == 0)
        {
            auto& content = getFileContent(argv[2]);
            BWT::transform(content);
            saveFileContent(content, argv[2], ".bwt");
        }
        else if(strcmp(argv[1], "-x") == 0)
        {
            auto& content = getFileContent(argv[2]);
            BWT::untransform(content);
            RLE::decode(content);
            saveFileContent(content, argv[2], "");
        }
    }
    if(argc == 2)
    {
        auto& content = getFileContent(argv[1]);
        RLE::encode(content);
        BWT::transform(content);
        saveFileContent(content, argv[1], ".br");
    }
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
 * @brief Saves the data provided into a new file.
 * 
 * @param fileContent The content to save
 * @param filename The name of the file
 */
void saveFileContent(const vector<string>& fileContent, const string& filename, const string& extension)
{
    std::ofstream stream{filename.substr(0, filename.find('.')) + extension};
    for(const string& element : fileContent)
        stream << element << '\n';
}