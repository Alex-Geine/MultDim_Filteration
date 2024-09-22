#include "Signal.h"
#include "Picture.h"

void printData(uint8_t* data, uint32_t col, uint32_t str)
{
    std::cout << "printing data..." << std::endl;
    uint64_t counter = 0;
    for (uint64_t i = 0; i < str; ++i)
    {
        for (uint64_t j = 0; j < col; ++j)
        {
            std::cout << (uint32_t)data[counter] << " ";
            counter++;
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
};

int main(int argc,char **argv)
{
    InitializeMagick(*argv);
    uint32_t col = N;
    uint32_t str = N;
    double weight = 5;

    TestSignal testSig(weight);

    // Gen Signal
    uint8_t* data = testSig.GetPicture();
    uint8_t* pic  = new uint8_t[col * str * 3];

    //printData(data, col, str);

    g_toGrayScaleOut(col, str, data, pic);
    //printData(pic, col * 3, str);

    g_safeImage(std::string("Signal.bmp"), col, str, pic);

    // Get fft
    Signal specture(col, str);

    std::complex<double>** sig  = testSig.GetDataArray();
    std::complex<double>** spec = specture.GetDataArray();

    g_mfft(sig, spec, str, col, FT_DIRECT);

    uint8_t* specArray = specture.GetPicture();
    //printData(specArray, col, str);

    g_toGrayScaleOut(col, str, specArray, pic);
    g_safeImage(std::string("Specture.bmp"), col, str, pic);

    return 0;
};
