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

void TestPicOld();

void TestPicNew();

void GaussPic();

int main(int argc,char **argv)
{
    InitializeMagick(*argv);

    GaussPic();

    return 0;
};


void TestPicOld()
{
    uint32_t col         = N;
    uint32_t str         = N;
    double   weight      = 1;
    double   noizeLevel  = 1;   // In Db
    double   filterLevel = -5;  // In Db
    uint8_t* pic         = new uint8_t[col * str * 3];

    Signal* testSig = new TestSignal(weight);
    Signal specture(col, str);
    Signal* filteredSpectre = nullptr;
    Signal filteredPic(col, str);

    uint8_t* data      = nullptr;
    uint8_t* noizeData = nullptr;
    uint8_t* s         = nullptr;
    uint8_t* specArray = nullptr;
    uint8_t* fPic      = nullptr;

    std::complex<double>** sig     = nullptr;
    std::complex<double>** spec    = nullptr;
    std::complex<double>** filSpec = nullptr;
    std::complex<double>** filPic  = nullptr;

    // Gen Signal
    data = testSig->GetPicture();

    g_toGrayScaleOut(col, str, data, pic);
    g_safeImage(std::string("Signal.bmp"), col, str, pic);

    // Adding noize
    g_noizeSignal(*testSig, noizeLevel);
    noizeData = testSig->GetPicture();

    g_toGrayScaleOut(col, str, noizeData, pic);
    g_safeImage(std::string("NoizeSignal.bmp"), col, str, pic);

    // Get fft

    sig  = testSig->GetDataArray();
    spec = specture.GetDataArray();

    g_mfftDir(sig, spec, str, col, FT_DIRECT);

    filteredSpectre = g_squareFiltration(specture, filterLevel);

    s         = specture.GetPicture();
    specArray = filteredSpectre->GetPicture();

    g_toGrayScaleOut(col, str, s, pic);
    g_safeImage(std::string("Specture.bmp"), col, str, pic);

    g_toGrayScaleOut(col, str, specArray, pic);
    g_safeImage(std::string("FilSpecture.bmp"), col, str, pic);


    // Get filt pic
    filSpec = filteredSpectre->GetDataArray();
    filPic = filteredPic.GetDataArray();

    g_mfftInv(filSpec, filPic, str, col, FT_INVERSE);

    fPic = filteredPic.GetPicture();

    g_toGrayScaleOut(col, str, fPic, pic);
    g_safeImage(std::string("Filtered.bmp"), col, str, pic);

    delete[] pic;
    delete[] data;
    delete[] noizeData;
    delete[] s;
    delete[] specArray;
    delete[] fPic;
};

void TestPicNew()
{
  uint32_t col         = N;
    uint32_t str         = N;
    double   weight      = 1;
    double   noizeLevel  = 1;   // In Db
    double   filterLevel = -5;  // In Db

    uint8_t* pic  = new uint8_t[col * str * 3];
    uint8_t* data = new uint8_t[col * str];

    Signal* testSig = new TestSignal(weight);
    Signal specture(col, str);
    Signal* filteredSpectre = nullptr;
    Signal filteredPic(col, str);

    std::complex<double>** sig     = nullptr;
    std::complex<double>** spec    = nullptr;
    std::complex<double>** filSpec = nullptr;
    std::complex<double>** filPic  = nullptr;

    // Gen Signal
    testSig->GetPicture(data);

    g_toGrayScaleOut(col, str, data, pic);
    g_safeImage(std::string("Signal.bmp"), col, str, pic);

    // Adding noize
    g_noizeSignal(*testSig, noizeLevel);
    testSig->GetPicture(data);

    g_toGrayScaleOut(col, str, data, pic);
    g_safeImage(std::string("NoizeSignal.bmp"), col, str, pic);

    // Get fft
    sig  = testSig->GetDataArray();
    spec = specture.GetDataArray();

    g_mfftDir(sig, spec, str, col, FT_DIRECT);

    filteredSpectre = g_squareFiltration(specture, filterLevel);

    // Out spectre
    specture.GetPicture(data);

    g_toGrayScaleOut(col, str, data, pic);
    g_safeImage(std::string("Specture.bmp"), col, str, pic);

    // Out filtered spectre
    filteredSpectre->GetPicture(data);

    g_toGrayScaleOut(col, str, data, pic);
    g_safeImage(std::string("FilSpecture.bmp"), col, str, pic);

    // Get filt pic
    filSpec = filteredSpectre->GetDataArray();
    filPic = filteredPic.GetDataArray();

    g_mfftInv(filSpec, filPic, str, col, FT_INVERSE);

    filteredPic.GetPicture(data);

    g_toGrayScaleOut(col, str, data, pic);
    g_safeImage(std::string("Filtered.bmp"), col, str, pic);

    delete[] pic;
    delete[] data;
};


void GaussPic()
{
    uint32_t col         = N;
    uint32_t str         = N;
    double   noizeLevel  = -50;   // In Db
    double   filterLevel = -5;  // In Db

    //uint32_t numberOfGauss = 3;
    //std::vector<double> x0{0.1, 0, -0.1};
    //std::vector<double> y0{0.1, 0, -0.1};
    //std::vector<double> ampl{1, 0.5, 1};
    //std::vector<double> sigmaX{0.1, 0.2, 0.3};
    //std::vector<double> sigmaY{0.1, 0.2, 0.3};

    uint32_t numberOfGauss = 1;
    std::vector<double> x0{0};
    std::vector<double> y0{0};
    std::vector<double> ampl{1};
    std::vector<double> sigmaX{1};
    std::vector<double> sigmaY{1};

    uint8_t* pic  = new uint8_t[col * str * 3];
    uint8_t* data = new uint8_t[col * str];

    Signal* testSig = new GaussSignal(numberOfGauss, x0.data(), y0.data(), ampl.data(), sigmaX.data(), sigmaY.data());
    Signal specture(col, str);
    Signal* filteredSpectre = nullptr;
    Signal filteredPic(col, str);

    std::complex<double>** sig     = nullptr;
    std::complex<double>** spec    = nullptr;
    std::complex<double>** filSpec = nullptr;
    std::complex<double>** filPic  = nullptr;

    // Gen Signal
    testSig->GetPicture(data);

    g_toGrayScaleOut(col, str, data, pic);
    g_safeImage(std::string("Signal.bmp"), col, str, pic);

    // Adding noize
    g_noizeSignal(*testSig, noizeLevel);
    testSig->GetPicture(data);

    g_toGrayScaleOut(col, str, data, pic);
    g_safeImage(std::string("NoizeSignal.bmp"), col, str, pic);

    // Get fft
    sig  = testSig->GetDataArray();
    spec = specture.GetDataArray();

    g_mfftDir(sig, spec, str, col, FT_DIRECT);

    filteredSpectre = g_squareFiltration(specture, filterLevel);

    // Out spectre
    specture.GetPicture(data);

    g_toGrayScaleOut(col, str, data, pic);
    g_safeImage(std::string("Specture.bmp"), col, str, pic);

    // Out filtered spectre
    filteredSpectre->GetPicture(data);

    g_toGrayScaleOut(col, str, data, pic);
    g_safeImage(std::string("FilSpecture.bmp"), col, str, pic);

    // Get filt pic
    filSpec = filteredSpectre->GetDataArray();
    filPic = filteredPic.GetDataArray();

    g_mfftInv(filSpec, filPic, str, col, FT_INVERSE);

    filteredPic.GetPicture(data);

    g_toGrayScaleOut(col, str, data, pic);
    g_safeImage(std::string("Filtered.bmp"), col, str, pic);

    delete[] pic;
    delete[] data;
};

