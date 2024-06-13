#include "Texture.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

Texture::Texture(const std::string& fileName)
{
    glGenTextures(1, &id);      //number of textures, array where names are stored
    glBindTexture(GL_TEXTURE_2D, id);       //target, name of texture

    unsigned char* data = stbi_load(fileName.c_str(), &width, &height, nullptr, 4); //load image data
    if (!data)  //if data from image could not load
    {
        printf("%s -> %s\n", fileName.c_str(), stbi_failure_reason());
        exit(0);
    }
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);     //2D texture image
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);       //use linear interpolation when sampling pixels smaller
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);       //use linear interpolation when sampling pixels bigger
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);       //set wrap for horizontal texture coordinate
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);       //set wrap for vertical texture coordinate
    stbi_image_free(data);      //free memory

    colorData.assign(data, data + (width * height * 4));        //init vector with rgba information
}

void Texture::bind()
{
    glBindTexture(GL_TEXTURE_2D, id);       //target, name of texture
}
