#include <Magick++.h> 
#include <iostream> 
#include "Picture.h"

using namespace std; 
using namespace Magick; 

void printData(uint8_t* data, uint32_t col, uint32_t str)
{
     std::cout << "printing data..." << std::endl;
        for (uint64_t i = 0; i < col * str; ++i)
        {
           std::cout << (uint32_t)data[i] << " ";
        }
        std::cout << std::endl;
};

int main(int argc,char **argv) 
{ 
    
     InitializeMagick(*argv);
     std::string name("../img/gnature.bmp");
     uint32_t width  = 0;
     uint32_t height = 0;
     uint8_t* data = nullptr;
     
     std::cout << "before..." << std::endl;
     
     if(!g_loadImage(name, width, height, &data))
     {
          std::cout << "load data failed" << std::endl;
          return 0;
     };
     std::cout << "after loading..." << std::endl;
     if ((data == nullptr) || (width == 0) || (height == 0))
     {
          std::cout << "Error!" << std::endl;
          return 0;
     }
     else
     {
        //printData(data, width / 10, height / 10);
        uint8_t* outData = new uint8_t[height * width];
        uint8_t* grayscale = new uint8_t[height * width * 3];
        g_toGrayScaleIn(width, height, data, outData);
        //printData(outData, width, height);
        
        g_toGrayScaleOut(width, height, outData, grayscale);
        //printData(grayscale, width, height);

        g_safeImage(std::string("f"), width, height, grayscale);
        //g_safeImage(std::string("f"), width, height, outData);
     }
    //Image image;
    //try { 
    // Read a file into image object 
    //image.read( "not-nature.bmp" );
    //image.type( GrayscaleType );
    //image.write("not-gnature.bmp");

    // Crop the image to specified size (width, height, xOffset, yOffset)
    //image.crop( Geometry(100,100, 100, 100) );

    // Write the image to a file 
    //image.write( "logo.png" ); 
    //} 
    //catch( Exception &error_ ) 
    //{ 
    //    cout << "Caught exception: " << error_.what() << endl; 
    //    return 1; 
    //} 
  return 0; 
} 
