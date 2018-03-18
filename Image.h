#ifndef IMAGE_H
#define IMAGE_H

#include <cstddef>

class Image {
public:
    unsigned char * image;
    int width, height, channels;
    Image(char * filename = NULL);
};

#endif
