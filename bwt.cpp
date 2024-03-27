#include "bwt.h"

#include <algorithm>
#include <iostream>

using std::string;
using std::vector;


/**
 * @brief Aplies the BWT in-place on each string in fileContent.
 * 
 * @param fileContent A list of words
 */
void BWT::transform(vector<string>& fileContent)
{
    for(string& element : fileContent)
    {
        // Prepare the string for the transform
        element += '$';
        // Create a "matrix" with all the rotations
        vector<string> matrix{};
        for(int i = 0; i < element.length(); ++i)
        {
            matrix.push_back(element);
            std::rotate(element.begin(), element.begin() + 1, element.end());
        }

        // Sort, get the last column of the "matrix", and replace the original string
        std::sort(matrix.begin(), matrix.end());
        string bwt{};
        for(const string& line : matrix)
        {
            bwt.push_back(line.back());
        }
        element = bwt;
    }
}

void BWT::untransform(vector<string>& fileContent)
{
    for(string& element : fileContent)
    {
        const std::size_t element_size = element.size();
        vector<string> matrix(element_size, "");

        for(std::size_t i = 0; i < element_size; ++i)
        {
            for(std::size_t j = 0; j < element_size; ++j)
            {
                matrix[j] = element[j] + matrix[j];
            }
            std::sort(matrix.begin(), matrix.end());
        }

        for(const string& result : matrix)
        {
            if(result.back() == '$')
            {
                element = result;
                break;
            }
        }
    }
}