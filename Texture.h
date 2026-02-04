#pragma once

#include <vector>
using std::vector;
class Texture
{
public:
    std::vector<std::string> faces;

    unsigned int loadCubeMap();
    
};

