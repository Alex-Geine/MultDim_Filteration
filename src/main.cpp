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

void logScale(uint8_t* data, uint64_t col, uint64_t str)
{
    uint64_t size = col * str;
    uint8_t max = data[0];

    for (uint64_t i = 1; i < size; ++i)
        if (data[i] > max)
            max = data[i];

    for (uint64_t i = 0; i < size; ++i)
       data[i] = 255 * std::log2(1 + data[i] * data[i]) / std::log2(1 + max * max);
}

void TestPicNew(double noizeLevel, double filterLevel, std::string logScaleAnsw);

void GaussPic(double noizeLevel, double filterLevel, std::string logScaleAnsw);

void NaturePic(double noizeLevel, double filterLevel, std::string logScaleAnsw, std::string interAnsw);

int main(int argc,char **argv)
{
    InitializeMagick(*argv);

    while (1)
    {
        uint64_t funk        = 0;
        double   noiseLevel  = 0;
        double   filterLevel = 0;

        std::string logScaleAnsw;
        std::string interAnsw;

        std::cout << "Please type the number of func: " << std::endl
                  << "1. TestSignal" << std::endl
                  << "2. GaussSignal" << std::endl
                  << "3. RealPicture" << std::endl
                  << "4. Exit" << std::endl;

       std::cin >> funk;

       if (funk == 4)
       {
           std::cout << "Exit..." << std::endl;
           return 0;
       }

       std::cout << "SNR (Db): " << std::endl;
       std::cin >> noiseLevel;
       std::cout << "Filter SNR (Db): " << std::endl;
       std::cin >> filterLevel;
       std::cout << "Log scale (y/n)?" << std::endl;
       std::cin >> logScaleAnsw;

       if (funk == 1)
       {
           TestPicNew(noiseLevel, filterLevel, logScaleAnsw);
       }
       else if (funk == 2)
       {
           GaussPic(noiseLevel, filterLevel, logScaleAnsw);
       }
       else if (funk == 3)
       {
           std::cout << "Interpolation (y/n)?" << std::endl;
           std::cin >> interAnsw;
           NaturePic(noiseLevel, filterLevel, logScaleAnsw, interAnsw);
       }
       else
       {
           std::cout << "Unknown input... Retry" << std::endl;
           return 0;
       }
    }

    return 0;
};

void TestPicNew(double noiseLevel, double _filterLevel, std::string logScaleAnsw)
{
    uint32_t col         = N;
    uint32_t str         = N;
    double   weight      = 1;
    double   noizeLevel  = noiseLevel;   // In Db
    double   filterLevel = _filterLevel;  // In Db

    uint8_t* pic  = new uint8_t[col * str * 3];
    uint8_t* data = new uint8_t[col * str];

    Signal* testSig         = new TestSignal(weight);
    Signal* noiseSig        = nullptr;
    Signal  specture(col, str);
    Signal* filteredSpectre = nullptr;
    Signal  filteredPic(col, str);

    std::complex<double>** sig     = nullptr;
    std::complex<double>** spec    = nullptr;
    std::complex<double>** filSpec = nullptr;
    std::complex<double>** filPic  = nullptr;

    // Gen Signal
    testSig->GetPicture(data, true);

    g_toGrayScaleOut(col, str, data, pic);
    g_safeImage(std::string("Signal.bmp"), col, str, pic);

    noiseSig = new Signal(*testSig);

    // Adding noize
    g_noizeSignal(*noiseSig, noizeLevel);
    noiseSig->GetPicture(data, true);

    g_toGrayScaleOut(col, str, data, pic);
    g_safeImage(std::string("NoizeSignal.bmp"), col, str, pic);

    std::cout << "SNR (square error): " << testSig->GetSquareError(*noiseSig) << " (Db)" << std::endl;
    std::cout << "SNR (pixel error): " << testSig->GetPixelError(*noiseSig) << " (Db)" << std::endl;

    // Get fft
    sig  = noiseSig->GetDataArray();
    spec = specture.GetDataArray();

    g_mfftDir(sig, spec, str, col, FT_DIRECT);

    filteredSpectre = g_squareFiltration(specture, filterLevel);

    // Out spectre
    specture.GetPicture(data, true);
    if (logScaleAnsw == "y")
        logScale(data, col, str);

    g_toGrayScaleOut(col, str, data, pic);
    g_safeImage(std::string("Specture.bmp"), col, str, pic);

    // Out filtered spectre
    filteredSpectre->GetPicture(data, true);
    if (logScaleAnsw == "y")
        logScale(data, col, str);

    g_toGrayScaleOut(col, str, data, pic);
    g_safeImage(std::string("FilSpecture.bmp"), col, str, pic);

    // Get filt pic
    filSpec = filteredSpectre->GetDataArray();
    filPic = filteredPic.GetDataArray();

    g_mfftDir(filSpec, filPic, str, col, FT_INVERSE);

    filteredPic.GetPicture(data, true);

    g_toGrayScaleOut(col, str, data, pic);
    g_safeImage(std::string("Filtered.bmp"), col, str, pic);

    std::cout << "SNR (square error): " << testSig->GetSquareError(filteredPic) << " (Db)" << std::endl;
    std::cout << "SNR (pixel error): " << testSig->GetPixelError(filteredPic) << " (Db)" << std::endl;

    delete[] pic;
    delete[] data;
};

void GaussPic(double noiseLevel, double _filterLevel, std::string logScaleAnsw)
{
    uint32_t col         = N;
    uint32_t str         = N;
    double   noizeLevel  = noiseLevel;   // In Db
    double   filterLevel = _filterLevel;  // In Db

    uint32_t numberOfGauss = 3;
    std::vector<double> x0{-0.2, 0, -0.4};
    std::vector<double> y0{0.2, 0, 0.3};
    std::vector<double> ampl{1, 1, 1};
    std::vector<double> sigmaX{0.01, 0.02, 0.03};
    std::vector<double> sigmaY{0.01, 0.02, 0.03};

    uint8_t* pic  = new uint8_t[col * str * 3];
    uint8_t* data = new uint8_t[col * str];

    Signal* testSig = new GaussSignal(numberOfGauss, x0.data(), y0.data(), ampl.data(), sigmaX.data(), sigmaY.data());
    Signal* noiseSig = nullptr;
    Signal specture(col, str);
    Signal* filteredSpectre = nullptr;
    Signal filteredPic(col, str);

    std::complex<double>** sig     = nullptr;
    std::complex<double>** spec    = nullptr;
    std::complex<double>** filSpec = nullptr;
    std::complex<double>** filPic  = nullptr;

    // Gen Signal
    testSig->GetPicture(data, true);

    g_toGrayScaleOut(col, str, data, pic);
    g_safeImage(std::string("Signal.bmp"), col, str, pic);

    noiseSig = new Signal(*testSig);

    // Adding noize
    g_noizeSignal(*noiseSig, noizeLevel);
    noiseSig->GetPicture(data, true);

    std::cout << "SNR (square error): " << testSig->GetSquareError(*noiseSig) << " (Db)" << std::endl;
    std::cout << "SNR (pixel error): " << testSig->GetSquareError(*noiseSig) << " (Db):" << std::endl;

    g_toGrayScaleOut(col, str, data, pic);
    g_safeImage(std::string("NoizeSignal.bmp"), col, str, pic);

    // Get fft
    sig  = testSig->GetDataArray();
    spec = specture.GetDataArray();

    g_mfftDir(sig, spec, str, col, FT_DIRECT);

    filteredSpectre = g_squareFiltration(specture, filterLevel);

    // Out spectre
    specture.GetPicture(data, true);
    if (logScaleAnsw == "y")
        logScale(data, col, str);

    g_toGrayScaleOut(col, str, data, pic);
    g_safeImage(std::string("Specture.bmp"), col, str, pic);

    // Out filtered spectre
    filteredSpectre->GetPicture(data, true);
    if (logScaleAnsw == "y")
        logScale(data, col, str);

    g_toGrayScaleOut(col, str, data, pic);
    g_safeImage(std::string("FilSpecture.bmp"), col, str, pic);

    // Get filt pic
    filSpec = filteredSpectre->GetDataArray();
    filPic = filteredPic.GetDataArray();

    g_mfftDir(filSpec, filPic, str, col, FT_INVERSE);

    filteredPic.GetPicture(data, true);

    g_toGrayScaleOut(col, str, data, pic);
    g_safeImage(std::string("Filtered.bmp"), col, str, pic);

    std::cout << "SNR (square error): " << testSig->GetSquareError(filteredPic) << " (Db)" << std::endl;
    std::cout << "SNR (pixel error): " << testSig->GetSquareError(filteredPic) << " (Db):" << std::endl;

    delete[] pic;
    delete[] data;
};


void NaturePic(double noiseLevel, double _filterLevel, std::string logScaleAnsw, std::string interAnsw)
{
    uint32_t col         = 0;
    uint32_t str         = 0;
    uint32_t acCol       = 0;
    uint32_t acStr       = 0;
    double   noizeLevel  = noiseLevel;   // In Db
    double   filterLevel = _filterLevel;  // In Db

    uint8_t* inPic = nullptr;

    std::cout << "loading..." << std::endl;

    g_loadImage(std::string("../img/gnature.bmp"), col, str, &inPic);

    std::cout << "load complete!" << std::endl;

    uint8_t* data     = new uint8_t[col * str];
    uint8_t* pic      = new uint8_t[col * str * 3];
    uint8_t* dataSpec = nullptr;
    uint8_t* picSpec  = nullptr;

    std::cout << "GrayScaling..." << std::endl;
    g_toGrayScaleIn(col, str, inPic, data);
    std::cout << "GrayScaling complete!" << std::endl;

    std::cout << "Creating signal..." << std::endl;
    std::cout << "Col: " << col << ", str: " << str << std::endl;
    RealSignal* testSig = new RealSignal(data, col, str);

    acCol = testSig->GetNumberOfColomns();
    acStr = testSig->GetNumberOfStrings();

    dataSpec = new uint8_t[acCol * acStr];
    picSpec  = new uint8_t[acCol * acStr * 3];
    std::cout << "Signal created!" << std::endl;
    Signal specture(acCol, acStr);
    Signal* filteredSpectre = nullptr;
    Signal filteredPic(acCol, acStr);

    std::complex<double>** sig     = nullptr;
    std::complex<double>** spec    = nullptr;
    std::complex<double>** filSpec = nullptr;
    std::complex<double>** filPic  = nullptr;

    // Gen Signal
    testSig->Resize();
    testSig->GetPicture(data, false);

    g_toGrayScaleOut(col, str, data, pic);
    g_safeImage(std::string("Signal.bmp"), col, str, pic);

    // Adding noize
    testSig->Resize();
    g_noizeSignal(*testSig, noizeLevel);
    testSig->Resize();
    testSig->GetPicture(data, false);

    g_toGrayScaleOut(col, str, data, pic);
    g_safeImage(std::string("NoizeSignal.bmp"), col, str, pic);

    // Get fft
    testSig->Resize();
    sig  = testSig->GetDataArray();
    spec = specture.GetDataArray();

    std::cout << "SigCol: " << testSig->GetNumberOfColomns()
              << "SigStr: " << testSig->GetNumberOfStrings()
              << std::endl;
    std::cout << "SpecCol: " << specture.GetNumberOfColomns()
              << "SpecStr: " << specture.GetNumberOfStrings()
              << std::endl;

    if (g_mfftDir(sig, spec, acStr, acCol, FT_DIRECT))
        std::cout << "FFT is ok!" << std::endl;

    // Draw spec
    specture.GetPicture(dataSpec, false);
    if (logScaleAnsw == "y")
        logScale(dataSpec, acCol, acStr);

    g_toGrayScaleOut(acCol, acStr, dataSpec, picSpec);
    g_safeImage(std::string("Specture.bmp"), acCol, acStr, picSpec);

    std::cout << "Filter specture..." << std::endl;
    filteredSpectre = g_squareFiltration(specture, filterLevel);

    // Draw fil spec
    filteredSpectre->GetPicture(dataSpec, false);
    if (logScaleAnsw == "y")
        logScale(dataSpec, acCol, acStr);

    g_toGrayScaleOut(acCol, acStr, dataSpec, picSpec);
    g_safeImage(std::string("FilSpecture.bmp"), acCol, acStr, picSpec);

    // Get filt pic
    filSpec = filteredSpectre->GetDataArray();
    filPic = filteredPic.GetDataArray();

    if (g_mfftDir(filSpec, filPic, acStr, acCol, FT_INVERSE))
        std::cout << "Inv FFT is ok!" << std::endl;

    filteredPic.GetPicture(dataSpec, false);

    g_toGrayScaleOut(acCol, acStr, dataSpec, picSpec);
    g_safeImage(std::string("Filtered.bmp"), acCol, acStr, picSpec);

    std::cout << "SNR (square error): " << testSig->GetSquareError(filteredPic) << " in Db." << std::endl;
    std::cout << "SNR (pixel error): " << testSig->GetPixelError(filteredPic) << " in Db." << std::endl;

    delete[] inPic;
    delete[] pic;
    delete[] data;
};
