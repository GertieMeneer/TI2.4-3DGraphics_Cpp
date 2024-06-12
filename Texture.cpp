#include "Texture.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

Texture::Texture(const std::string& fileName)
{
    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load(fileName.c_str(), &width, &height, nullptr, 4); // Get width, height, and color components
    if (!data)
    {
        printf("%s -> %s\n", fileName.c_str(), stbi_failure_reason());
        exit(0);
    }
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    stbi_image_free(data);

    colorData.assign(data, data + (width * height * 4));
}

void Texture::bind()
{
    glBindTexture(GL_TEXTURE_2D, id);
}

glm::vec3 Texture::getColor(int x, int y)
{
    if (x >= 0 && x < width && y >= 0 && y < height)
    {
        int index = (x + y * width) * 4;
        float r = colorData[index] / 255.0f;
        float g = colorData[index + 1] / 255.0f;
        float b = colorData[index + 2] / 255.0f;
        return glm::vec3(r, g, b);
    }
    return glm::vec3(0.0f);
}
