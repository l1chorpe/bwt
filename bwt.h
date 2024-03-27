#pragma once

#include <string>
#include <vector>


class BWT final
{
    private:
        BWT();
    
    public:
        static void transform(std::vector<std::string>& fileContent);
        static void untransform(std::vector<std::string>& fileContent);
};