#pragma once

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h" 

class Image {
public:
	unsigned char* image;
	int width, height, channels;
	Image(char *filename = NULL)
	{
		if (filename != NULL)
		{
			stbi_set_flip_vertically_on_load(false);
			image = stbi_load(filename, &width, &height, &channels, STBI_rgb);
		}
	}
};