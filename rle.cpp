#include "rle.h"

#include <cctype>
#include <cstddef>

using std::string;
using std::vector;


/**
 * @brief Applies RLE in-place on fileContent.
 * 
 * @param fileContent A list of words
 */
void RLE::encode(vector<string>& fileContent)
{
    for(string& element : fileContent)
    {
        // Count how many times each character is repeated
        string rle_str = "";
        std::size_t prev_char_index = 0;
        for(std::size_t i = 1; i < element.length(); ++i)
        {
            if(element[prev_char_index] != element[i])
            {
                // Don't put 1 in front of lone characters to reduce space
                if(i - prev_char_index > 1)
                    rle_str += std::to_string(i - prev_char_index);
                rle_str += element[prev_char_index];
                prev_char_index = i;
            }
        }
        // Clean up the last iteration
        element = rle_str + std::to_string(element.length() - prev_char_index) + element[prev_char_index];
    }
}

void RLE::decode(vector<string>& fileContent)
{
    for(string& element : fileContent)
    {
        string number = "", result = "";
        for(char c : element)
        {
            if(std::isdigit(c))
            {
                number += c;
            }
            else
            {
                if(number.compare("") != 0)
                    result += string(std::stoi(number), c);
                else
                    result += c;
                number = "";
            }
        }
        element = result.substr(0, result.length() - 1);
    }
}