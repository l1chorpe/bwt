#pragma once

#include <string>
#include <vector>


class RLE final
{
    private:
        RLE();

    public:
        static void encode(std::vector<std::string>& fileContent);
        static void decode(std::vector<std::string>& fileContent);
};