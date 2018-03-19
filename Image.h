#ifndef IMAGE_H
#define IMAGE_H

#include <cstddef>
#include <string>

using std::string;

class Image {
public:
    unsigned char * image;
    int width, height, channels;
    Image(string filename = "");
};

#endif
