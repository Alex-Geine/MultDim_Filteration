#ifndef PICTURE_H
#define PICTURE_H

#include <Magick++.h> 
#include <iostream>
#include <cstdint>
#include <cstring>

using namespace Magick;

void g_initMagick(char **argv)
{
    InitializeMagick(*argv);
};

// Save image
bool g_safeImage(std::string name, uint32_t width, uint32_t height, uint8_t* data)
{
    Image image;

    image.read((size_t)width, (size_t)height, "BRG", CharPixel, (void*)data);
    image.write("Test.bmp");

    return true;
};

bool g_toGrayScaleIn(uint32_t width, uint32_t height, uint8_t* data, uint8_t* outData)
{
    if (data == nullptr || width == 0 || height == 0) return false;

    uint64_t counter = 0;
    //uint8_t* outData = new uint8_t[width * height];
    uint64_t size = width * height;
    double sum = 0;
    //std:: cout << "sum: " << (double)data[counter] * 0.299D  << " + " << (double)data[counter + 1] * 0.587D << " + "
    //<< " + " << (double)data[counter + 2] * 0.114D << std::endl;
    //sum = (double)data[counter] * 0.299D + (double)data[counter + 1] * 0.587D + (double)data[counter + 2] * 0.114D;

    //std::cout << sum << std::endl;
    //uint8_t bit = (uint8_t)sum;
    //std::cout << (uint32_t)bit << std::endl;
    
    //std::cout << "before: " <<std::endl;
    for (uint32_t i = 0; i < size; ++i)
    {
        sum = 0;
        sum = (double)data[counter] * 0.299D + (double)data[counter + 1] * 0.587D + (double)data[counter + 2] * 0.114D;
        //std::cout << sum << " ";
        outData[i] = sum;

        counter += 3;
    }   

    //std:: cout << std::endl;

    return true;
};

bool g_toGrayScaleOut(uint32_t width, uint32_t height, uint8_t* data, uint8_t* outData)
{
    if (data == nullptr || width == 0 || height == 0) return false;
 
    uint64_t counter = 0;
    //uint8_t* outData = new uint8_t[width * height * 3];

    for (uint32_t i = 0; i < width * height; ++i)
    {
        outData[counter]   = uint8_t((double)data[i]);
        outData[counter+1] = uint8_t((double)data[i]);
        outData[counter+2] = uint8_t((double)data[i]);
        counter += 3;
    }

    //*_outData = outData;

    return true;
};

// Load image
bool g_loadImage(std::string name, uint32_t& width, uint32_t& height, uint8_t** data)
{
    Image image;
    image.read(name.data());

    width  = image.columns();
    height = image.rows();

    char* pic = new char[width * height * 3];
    
    image.write(0, 0, image.columns(), image.rows(), "GRB", CharPixel, pic);

    std::cout << "Width: " << width << ", height: " << height << ", data: " << pic << std::endl;
    *data = (uint8_t*)pic;

    return true;
};

#endif    // PICTURE_H
