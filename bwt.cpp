#include "bwt.h"

#include <algorithm>
#include <iostream>

using std::string;
using std::vector;


/**
 * @brief Aplies the BWT in-place on each string in fileContent.
 * 
 * @param fileContent A list of strings
 */
void BWT::transform(vector<string>& fileContent)
{
    for(string& element : fileContent)
    {
        // Prepare the string for the transformation
        element += '$';
        // Create a matrix with all the rotations of the strings
        vector<string> matrix{};
        for(int i = 0; i < element.length(); ++i)
        {
            matrix.push_back(element);
            std::rotate(element.begin(), element.begin() + 1, element.end());
        }

        // Sort, get the last column of the matrix, and replace the original string
        std::sort(matrix.begin(), matrix.end());
        string bwt{};
        for(const string& line : matrix)
        {
            bwt.push_back(line.back());
        }
        element = bwt;
    }
}

/**
 * @brief Reverses BWT in-place for each string in fileContent.
 * 
 * @param fileContent A list of strings "encoded" with BWT
 */
void BWT::untransform(vector<string>& fileContent)
{
    // For each string, make an nxn matrix (where n is the length of the string) and add the i-th character to the i-th
    // *row* (not column) of the matrix. Sort and repeat until each string in the matrix is of length n
    for(string& element : fileContent)
    {
        const std::size_t element_size = element.size();
        vector<string> matrix(element_size, "");

        for(std::size_t i = 0; i < element_size; ++i)
        {
            for(std::size_t j = 0; j < element_size; ++j)
                matrix[j] = element[j] + matrix[j];

            std::sort(matrix.begin(), matrix.end());
        }

        // Use the only element that ends with '$'
        for(const string& result : matrix)
        {
            if(result.back() == '$')
            {
                element = result.substr(0, result.length() - 1);
                break;
            }
        }
    }
}