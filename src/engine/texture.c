#include "texture.h"
#include <glad/glad.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

struct Result textureCreate(const char *path, const char *samplerName, struct Texture *dest)
{
    glGenTextures(1, &dest->id);
    glBindTexture(GL_TEXTURE_2D, dest->id);

    // bunlari her defasinda yapmak gerekiyor obur turlu yapinca olusturulan texture etki etmiyo
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    stbi_set_flip_vertically_on_load(TRUE);

    int width, height, nrChannels;
    unsigned char *data = stbi_load(path, &width, &height, &nrChannels, STBI_rgb_alpha);

    dest->width = width;
    dest->height = height;
    dest->hasAlpha = nrChannels == 4;
    dest->path = path;
    dest->samplerName = samplerName;

    if (data)
    {

        //  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, dest->hasAlpha ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, data);
        // glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        stbi_image_free(data);
        return mErr(format("Failed to load texture: %s\n", path));
    }

    stbi_image_free(data);
}

void textureDestroy(struct Texture texture)
{
    glDeleteTextures(1, &texture.id);
}