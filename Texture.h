#ifndef TEXTURE_H
#define TEXTURE_H

#include <glad/glad.h>
#include <string>

class Texture {
public:
    unsigned int ID;
    std::string type;
    std::string path;

    // Constructor
    Texture(const char* imagePath, std::string texType);

    // Activate the texture
    void Bind(unsigned int unit);
};

#endif