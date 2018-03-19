#include "Image.h"

//Define required for stb_image
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

using std::string;
Image::Image(string filename)
{
    if (filename.length() > 0)
    {
        char * fn = new char[filename.length()+1];
        strcpy(fn, filename.c_str());
        stbi_set_flip_vertically_on_load(false);
        image = stbi_load(fn, &width, &height, &channels, STBI_rgb);
    }
}
