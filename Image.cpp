#include "Image.h"

//Define required for stb_image
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

Image::Image(char * filename)
{
    if (filename != NULL)
    {
        stbi_set_flip_vertically_on_load(false);
        image = stbi_load(filename, &width, &height, &channels, STBI_rgb);
    }
}
