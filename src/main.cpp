#include "Signal.h"
#include "Picture.h"

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
    uint32_t col = N;
    uint32_t str = N;
    double weight = 10;
    
    TestSignal testSig(weight);
    
    uint8_t* data = testSig.GetPictureArray();
    uint8_t* pic  = new uint8_t[col * str * 3];
    
    printData(data, col, str);

    g_toGrayScaleOut(col, str, data, pic);
    g_safeImage(std::string("f"), col, str, pic);

     //std::string name("not-gnature.bmp");
     //uint32_t width  = 0;
     //uint32_t height = 0;
     //uint8_t* data = nullptr;
//
     //g_loadImage(name, width, height, &data);
     //if ((data == nullptr) || (width == 0) || (height == 0))
     //{
     //     std::cout << "Error!" << std::endl;
     //     return 0;
     //}
     //else
     //{
     //   uint8_t* outData = new uint8_t[width * height];
     //   uint8_t* grayscale = new uint8_t[width * height * 3];
//
     //   g_toGrayScaleIn(width, height, data, outData);
     //   g_toGrayScaleOut(width, height, outData, grayscale);
     //   g_safeImage(std::string("f"), width, height, grayscale);
//
     //}
    return 0;
};
