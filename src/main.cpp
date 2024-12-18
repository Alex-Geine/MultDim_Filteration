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
}

// log scale for pixel data
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

// log scale for real data
void logScale(std::complex<double>** data, uint32_t col, uint32_t str)
{
    for (uint64_t i = 0; i < str; ++i)
        for (uint64_t j = 0; j < col; ++j)
            data[i][j] = std::log10(std::complex<double>(1,1) + data[i][j]);
}

// Function added some border on spectre for filtration window
void addWindowBorder(uint8_t* data, uint64_t col, uint64_t str, uint64_t x, uint64_t y, bool isInverse)
{
    uint64_t midCol = col / 2;
    uint64_t midStr = str / 2;

    if ((data == nullptr) || (midCol < y) || (midStr < x))
        return;

    uint8_t pixel = 255;
    if (isInverse)
        pixel = 0;

    uint64_t dx = midCol - x;    // center - border (strings)
    uint64_t dy = midStr - y;    // center - border (colomns)

    // strings
    for (uint64_t i = 0; i < midStr ; ++i)
    {
        if (i < y)   // < str
        {
            // left top
            data[i * col + x] = pixel;
            // right top
            data[i * col + midCol + dx] = pixel;
            // left bot
            data[(str - i) * col + x] = pixel;
            // right bot
            data[(str - i) * col + midCol + dx] = pixel;
        }
    }

    // colomns
    for (uint64_t i = 0; i < midCol; ++i)
    {
        if (i < x)    // < col
        {
            // left top
            data[y * col + i] = pixel;
            // right top
            data[(midStr + dy) * col + i] = pixel;
            // left bot
            data[y * col + col - i] = pixel;
            // right bot
            data[(midStr + dy) * col + col - i] = pixel;
        }
    }
}

// Moving spectre in the middle
void movingSpectreInTheMiddle(uint8_t* data, uint64_t col, uint64_t str)
{
    uint64_t midStr = str / 2;
    uint64_t midCol = col / 2;

    for (uint64_t i = 0; i < midStr; ++i)
    {
        for (uint64_t j = 0; j < midCol; ++j)
        {
            std::swap(data[i * col + j], data[(midStr + i) * col + midCol + j]); // Swap left top - right bot
            std::swap(data[(midStr + i) * col + j], data[i * col + midCol + j]); // Swap left bot - right top
        }
    }
}

void TestPicNew(double noizeLevel, double filterLevel, std::string logScaleAnsw);

void GaussPic(double noizeLevel, double filterLevel, std::string logScaleAnsw);

void NaturePic(double noizeLevel, double filterLevel, std::string logScaleAnsw, std::string filePath);

void NaturePicInterpol(double noizeLevel, double filterLevel, std::string logScaleAnsw, std::string filePath);

void TestNewFunc(double noiseLevel, double _filterLevel, std::string logScaleAnsw);

void TestInter()
{
    double weight = 50;
    TestSignal sig(weight);
    uint64_t col = sig.GetNumberOfColomns();
    uint64_t str = sig.GetNumberOfStrings();
    uint64_t nCol = col * 2;
    uint64_t nStr = str * 2;

    std::cout << "Start interpol" << std::endl;
    Signal* newSig = g_linInterpol(sig, nCol, nStr);

    delete newSig;
}

void TestSquareInterpol(uint64_t n, double w, double scale)
{
    std::cout << "Test square interpolation" << std::endl;
    uint32_t col         = n;
    uint32_t str         = n;
    uint64_t newCol      = n * scale;
    uint64_t newStr      = n * scale;
    double   weight      = w;

    uint8_t* pic     = new uint8_t[col * str * 3];
    uint8_t* data    = new uint8_t[col * str];
    uint8_t* picNew  = new uint8_t[newCol * newStr * 3];
    uint8_t* dataNew = new uint8_t[newCol * newStr];

    Signal* testSig         = new TestSignal(weight, n);
    Signal* interpol        = nullptr;

    // Gen Signal
    testSig->GetPicture(data, false);

    g_toGrayScaleOut(col, str, data, pic);
    g_safeImage(std::string("SquareSignal.bmp"), col, str, pic);

    interpol = g_linInterpolReal(*testSig, newCol, newStr);

    // Interpol Signal
    interpol->GetPicture(dataNew, false);

    g_toGrayScaleOut(newCol, newStr, dataNew, picNew);
    g_safeImage(std::string("SquareSignalInter.bmp"), newCol, newStr, picNew);

    delete[] pic;
    delete[] data;
    delete[] picNew;
    delete[] dataNew;

    delete testSig;
    delete interpol;
}

void GaussInterpolTest(uint64_t n, double scale)
{
    std::cout << "Gauss interpolation test: " << std::endl;

    uint32_t col         = n;
    uint32_t str         = n;
    uint32_t colNew      = n * scale;
    uint32_t strNew      = n * scale;

    uint32_t numberOfGauss = 3;
    std::cout << "Input number of gauss: " << std::endl;
    std::cin >> numberOfGauss;

    std::vector<double> x0(numberOfGauss);
    std::vector<double> y0(numberOfGauss);
    std::vector<double> ampl(numberOfGauss);
    std::vector<double> sigmaX(numberOfGauss);
    std::vector<double> sigmaY(numberOfGauss);

    for (uint32_t i = 0; i < numberOfGauss; ++i)
    {
        std::cout << "x0: " << std::endl;
        std::cin >> x0[i];
        std::cout << "y0: " << std::endl;
        std::cin >> y0[i];
        std::cout << "ampl: " << std::endl;
        std::cin >> ampl[i];
        std::cout << "SigmaX: " << std::endl;
        std::cin >> sigmaX[i];
        std::cout << "SigmaY: " << std::endl;
        std::cin >> sigmaY[i];
    }

    uint8_t* pic     = new uint8_t[col * str * 3];
    uint8_t* data    = new uint8_t[col * str];
    uint8_t* picNew  = new uint8_t[colNew * strNew * 3];
    uint8_t* dataNew = new uint8_t[colNew * strNew];

    Signal* testSig  = new GaussSignal(n, numberOfGauss, x0.data(), y0.data(), ampl.data(), sigmaX.data(), sigmaY.data());
    Signal* interpol = nullptr;

    // Gen Signal
    testSig->GetPicture(data, false);

    g_toGrayScaleOut(col, str, data, pic);
    g_safeImage(std::string("GaussSignal.bmp"), col, str, pic);

    interpol = g_linInterpolReal(*testSig, colNew, strNew);

    // Interpol Signal
    interpol->GetPicture(dataNew, false);

    g_toGrayScaleOut(colNew, strNew, dataNew, picNew);
    g_safeImage(std::string("GaussSignalInter.bmp"), colNew, strNew, picNew);

    delete[] pic;
    delete[] data;
    delete[] picNew;
    delete[] dataNew;

    delete testSig;
    delete interpol;
}

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
                  << "4. Test Inter Square" << std::endl
                  << "5. Test Inter Gauss" << std::endl
                  << "6. Exit" << std::endl;

       std::cin >> funk;

       if (funk == 6)
       {
           std::cout << "Exit..." << std::endl;
           return 0;
       }

       if (funk == 4)
       {
           uint64_t n = 0;
           double w = 0;
           double scale = 1;

           std::cout << "Input n, w, scale: " << std::endl;
           std::cin >> n >> w >> scale;

           TestSquareInterpol(n, w, scale);
           continue;
       }
       else if (funk == 5)
       {
           uint64_t n = 0;
           double scale = 1;
           std::cout << "Input n, scale: " << std::endl;
           std::cin >> n >> scale;

           GaussInterpolTest(n, scale);
           continue;
       }
       else
       {
           std::cout << "SNR (Db): " << std::endl;
           std::cin >> noiseLevel;
           std::cout << "Filter SNR (Db): " << std::endl;
           std::cin >> filterLevel;
           std::cout << "Log scale (y/n)?" << std::endl;
           std::cin >> logScaleAnsw;
       }

       if (funk == 1)
       {
            std::cout << "Testing new funcitonal..." << std::endl;
            TestNewFunc(noiseLevel, filterLevel, logScaleAnsw);
           //TestPicNew(noiseLevel, filterLevel, logScaleAnsw);
       }
       else if (funk == 2)
       {
           GaussPic(noiseLevel, filterLevel, logScaleAnsw);
       }
       else if (funk == 3)
       {
           std::string picType;
           std::string picPath("../img/gnature.bmp");
           std::cout << "Interpolation (y/n)?" << std::endl;
           std::cin >> interAnsw;
           std::cout << "Nature or cartoon (n/c)?" << std::endl;
           std::cin >> picType;

           if (picType == "c")
               picPath = "../img/not-gnature.bmp";

           if (interAnsw == "y")
               NaturePicInterpol(noiseLevel, filterLevel, logScaleAnsw, picPath);
           else
               NaturePic(noiseLevel, filterLevel, logScaleAnsw, picPath);
       }
       else
       {
           std::cout << "Unknown input... Retry" << std::endl;
           return 0;
       }
    }

    return 0;
};

void TestNewFunc(double noiseLevel, double _filterLevel, std::string logScaleAnsw)
{
    uint32_t col         = N;
    uint32_t str         = N;
    double   weight      = 1;
    double   noizeLevel  = noiseLevel;    // In Db
    double   filterLevel = _filterLevel;  // In Db
    uint64_t x = 0;                       // X coordinate for filtration window
    uint64_t y = 0;                       // Y coordinate for filtration window

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
    testSig->GetPicture(data, false);

    g_toGrayScaleOut(col, str, data, pic);
    g_safeImage(std::string("Signal.bmp"), col, str, pic);

    noiseSig = new Signal(*testSig);

    // Adding noize
    g_noizeSignal(*noiseSig, noizeLevel);
    noiseSig->GetPicture(data, false);

    g_toGrayScaleOut(col, str, data, pic);
    g_safeImage(std::string("NoizeSignal.bmp"), col, str, pic);

    std::cout << "SNR (square error): " << testSig->GetSquareError(*noiseSig) << " (Db)" << std::endl;
    std::cout << "SNR (pixel error): " << testSig->GetPixelError(*noiseSig) << " (Db)" << std::endl;

    // Get fft
    sig  = noiseSig->GetDataArray();
    spec = specture.GetDataArray();

    g_mfftDir(sig, spec, str, col, FT_DIRECT);

    filteredSpectre = g_squareFiltration(specture, filterLevel, x, y);

    // Out spectre
    Signal spectrePic(specture);
    std::cout << "Log scaling..." << std::endl;
    if (logScaleAnsw == "y")
    {
        sig = spectrePic.GetDataArray();
        logScale(sig, col, str);
    }

    spectrePic.GetPicture(data, false);

    std::cout << "Added border on spectre..." << std::endl;
    addWindowBorder(data, col, str, x, y, false);

    std::cout << "Move spec." << std::endl;
    movingSpectreInTheMiddle(data, col, str);
    
    g_toGrayScaleOut(col, str, data, pic);
    g_safeImage(std::string("Specture.bmp"), col, str, pic);

    // Out filtered spectre
    Signal FilSpecPic(*filteredSpectre);
    std::cout << "Log scaling filtered spec..." << std::endl;
    if (logScaleAnsw == "y")
    {
        sig = FilSpecPic.GetDataArray();
        logScale(sig, col, str);
    }

    FilSpecPic.GetPicture(data, false);

    std::cout << "Move filt spec." << std::endl;
    movingSpectreInTheMiddle(data, col, str);
    
    g_toGrayScaleOut(col, str, data, pic);
    g_safeImage(std::string("FilSpecture.bmp"), col, str, pic);

    // Get filt pic
    filSpec = filteredSpectre->GetDataArray();
    filPic = filteredPic.GetDataArray();

    g_mfftDir(filSpec, filPic, str, col, FT_INVERSE);

    filteredPic.GetPicture(data, false);

    g_toGrayScaleOut(col, str, data, pic);
    g_safeImage(std::string("Filtered.bmp"), col, str, pic);

    std::cout << "SNR (square error): " << testSig->GetSquareError(filteredPic) << " (Db)" << std::endl;
    std::cout << "SNR (pixel error): " << testSig->GetPixelError(filteredPic) << " (Db)" << std::endl;

    delete[] pic;
    delete[] data;

    delete testSig;
    delete noiseSig;
    delete filteredSpectre;
};

void GaussPic(double noiseLevel, double _filterLevel, std::string logScaleAnsw)
{
    uint32_t col         = N;
    uint32_t str         = N;
    double   noizeLevel  = noiseLevel;   // In Db
    double   filterLevel = _filterLevel;  // In Db
    uint64_t x = 0;                       // X coordinate for filtration window
    uint64_t y = 0;                       // Y coordinate for filtration window

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

    std::cout << "Gen signal" << std::endl;

    // Gen Signal
    testSig->GetPicture(data, false);

    g_toGrayScaleOut(col, str, data, pic);
    g_safeImage(std::string("Signal.bmp"), col, str, pic);

    noiseSig = new Signal(*testSig);

    std::cout << "Add noise..." << std::endl;
    // Adding noize
    g_noizeSignal(*noiseSig, noizeLevel);
    noiseSig->GetPicture(data, false);

    std::cout << "SNR (square error): " << testSig->GetSquareError(*noiseSig) << " (Db)" << std::endl;
    std::cout << "SNR (pixel error): " << testSig->GetPixelError(*noiseSig) << " (Db):" << std::endl;

    g_toGrayScaleOut(col, str, data, pic);
    g_safeImage(std::string("NoizeSignal.bmp"), col, str, pic);

    // Get fft
    sig  = noiseSig->GetDataArray();
    spec = specture.GetDataArray();

    g_mfftDir(sig, spec, str, col, FT_DIRECT);
    std::cout << "FFT is Ok!" << std::endl;

    filteredSpectre = g_squareFiltration(specture, filterLevel, x, y);

    // Out spectre
    Signal spectrePic(specture);
    std::cout << "Log scaling..." << std::endl;
    if (logScaleAnsw == "y")
    {
        sig = spectrePic.GetDataArray();
        logScale(sig, col, str);
    }

    spectrePic.GetPicture(data, false);

    std::cout << "Added border on spectre..." << std::endl;
    addWindowBorder(data, col, str, x, y, false);

    std::cout << "Move spec." << std::endl;
    movingSpectreInTheMiddle(data, col, str);

    g_toGrayScaleOut(col, str, data, pic);
    g_safeImage(std::string("Specture.bmp"), col, str, pic);

    // Out filtered spectre
    Signal FilSpecPic(*filteredSpectre);
    std::cout << "Log scaling filtered spec..." << std::endl;
    if (logScaleAnsw == "y")
    {
        sig = FilSpecPic.GetDataArray();
        logScale(sig, col, str);
    }

    FilSpecPic.GetPicture(data, false);

    std::cout << "Added border on spectre..." << std::endl;
    addWindowBorder(data, col, str, x, y, false);

    std::cout << "Move spec." << std::endl;
    movingSpectreInTheMiddle(data, col, str);

    g_toGrayScaleOut(col, str, data, pic);
    g_safeImage(std::string("FilSpecture.bmp"), col, str, pic);

    // Get filt pic
    filSpec = filteredSpectre->GetDataArray();
    filPic = filteredPic.GetDataArray();

    g_mfftDir(filSpec, filPic, str, col, FT_INVERSE);
    std::cout << "Inverce FFT is ready!" << std::endl;

    filteredPic.GetPicture(data, false);

    g_toGrayScaleOut(col, str, data, pic);
    g_safeImage(std::string("Filtered.bmp"), col, str, pic);

    std::cout << "SNR (square error): " << testSig->GetSquareError(filteredPic) << " (Db)" << std::endl;
    std::cout << "SNR (pixel error): " << testSig->GetPixelError(filteredPic) << " (Db):" << std::endl;

    delete[] pic;
    delete[] data;

    delete testSig;
    delete noiseSig;
    delete filteredSpectre;
};

void NaturePic(double noiseLevel, double _filterLevel, std::string logScaleAnsw, std::string filePath)
{
    uint32_t col         = 0;
    uint32_t str         = 0;
    uint32_t acCol       = 0;
    uint32_t acStr       = 0;
    double   noizeLevel  = noiseLevel;   // In Db
    double   filterLevel = _filterLevel;  // In Db
    uint64_t x = 0;                       // X coordinate for filtration window
    uint64_t y = 0;                       // Y coordinate for filtration window

    uint8_t* inPic = nullptr;

    std::cout << "loading..." << std::endl;

    g_loadImage(filePath, col, str, &inPic);

    std::cout << "load complete!" << std::endl;

    uint8_t* data     = new uint8_t[col * str];
    uint8_t* pic      = new uint8_t[col * str * 3];
    uint8_t* dataSpec = nullptr;
    uint8_t* picSpec  = nullptr;

    g_toGrayScaleIn(col, str, inPic, data);
    std::cout << "GrayScaling complete!" << std::endl;

    RealSignal* testSig = new RealSignal(data, col, str);
    RealSignal* noiseSig = nullptr;

    acCol = testSig->GetNumberOfColomns();
    acStr = testSig->GetNumberOfStrings();

    dataSpec = new uint8_t[acCol * acStr];
    picSpec  = new uint8_t[acCol * acStr * 3];
    std::cout << "Signal created!" << std::endl;
    Signal specture(acCol, acStr);
    Signal* filteredSpectre = nullptr;
    RealSignal* filteredPic = new RealSignal(data, col, str);

    std::complex<double>** sig     = nullptr;
    std::complex<double>** spec    = nullptr;
    std::complex<double>** filSpec = nullptr;
    std::complex<double>** filPic  = nullptr;

    // Gen Signal
    testSig->Resize();    // To pic
    testSig->GetPicture(data, false);

    g_toGrayScaleOut(col, str, data, pic);
    g_safeImage(std::string("Signal.bmp"), col, str, pic);

    testSig->Resize();    // To data

    noiseSig = new RealSignal(*testSig);

    // Adding noize
    g_noizeSignal(*noiseSig, noizeLevel);
    noiseSig->Resize();    // To pic
    noiseSig->GetPicture(data, false);

    testSig->Resize();    // To pic

    std::cout << "SNR: (square error): " << testSig->GetSquareError(*noiseSig) << " (Db)" << std::endl;
    std::cout << "SNR: (pixel error): " << testSig->GetPixelError(*noiseSig) << " (Db)" << std::endl;

    g_toGrayScaleOut(col, str, data, pic);
    g_safeImage(std::string("NoizeSignal.bmp"), col, str, pic);

    // Get fft
    noiseSig->Resize();    // To data
    sig  = noiseSig->GetDataArray();
    spec = specture.GetDataArray();

    std::cout << "SigCol: " << noiseSig->GetNumberOfColomns()
              << "SigStr: " << noiseSig->GetNumberOfStrings()
              << std::endl;
    std::cout << "SpecCol: " << specture.GetNumberOfColomns()
              << "SpecStr: " << specture.GetNumberOfStrings()
              << std::endl;

    if (g_mfftDir(sig, spec, acStr, acCol, FT_DIRECT))
        std::cout << "FFT is ok!" << std::endl;

    // Out spectre
    Signal spectrePic(specture);
    std::cout << "Log scaling..." << std::endl;
    if (logScaleAnsw == "y")
    {
        sig = spectrePic.GetDataArray();
        logScale(sig, acCol, acStr);
    }

    spectrePic.GetPicture(dataSpec, false);

    std::cout << "Added border on spectre..." << std::endl;
    addWindowBorder(dataSpec, acCol, acStr, x, y, false);

    std::cout << "Move spec." << std::endl;
    movingSpectreInTheMiddle(dataSpec, acCol, acStr);

    g_toGrayScaleOut(acCol, acStr, dataSpec, picSpec);
    g_safeImage(std::string("Specture.bmp"), acCol, acStr, picSpec);

    //std::cout << "Filter specture..." << std::endl;
    filteredSpectre = g_squareFiltration(specture, filterLevel, x, y);

    // Out spectre
    Signal spectreFilPic(*filteredSpectre);
    std::cout << "Log scaling..." << std::endl;
    if (logScaleAnsw == "y")
    {
        sig = spectreFilPic.GetDataArray();
        logScale(sig, acCol, acStr);
    }

    // Draw fil spec
    spectreFilPic.GetPicture(dataSpec, false);

    std::cout << "Added border on spectre..." << std::endl;
    addWindowBorder(dataSpec, acCol, acStr, x, y, false);

    std::cout << "Move spec." << std::endl;
    movingSpectreInTheMiddle(dataSpec, acCol, acStr);

    g_toGrayScaleOut(acCol, acStr, dataSpec, picSpec);
    g_safeImage(std::string("FilSpecture.bmp"), acCol, acStr, picSpec);

    // Get filt pic
    filSpec = filteredSpectre->GetDataArray();
    filPic = filteredPic->GetDataArray();

    if (g_mfftDir(filSpec, filPic, acStr, acCol, FT_INVERSE))
        std::cout << "Inv FFT is ok!" << std::endl;

    filteredPic->Resize();

    filteredPic->GetPicture(data, false);

    g_toGrayScaleOut(col, str, data, pic);
    g_safeImage(std::string("Filtered.bmp"), col, str, pic);

    std::cout << "SNR (square error): " << testSig->GetSquareError(*filteredPic) << " (Db)" << std::endl;
    std::cout << "SNR (pixel error): " << testSig->GetPixelError(*filteredPic) << " (Db)" << std::endl;

    testSig->Resize();
    filteredPic->Resize();

    delete[] inPic;
    delete[] data;
    delete[] pic;
    delete[] dataSpec;
    delete[] picSpec;
    delete testSig;
    delete noiseSig;
    delete filteredPic;
    delete filteredSpectre;
};

void NaturePicInterpol(double noiseLevel, double _filterLevel, std::string logScaleAnsw, std::string filePath)
{
    uint32_t col         = 0;
    uint32_t str         = 0;
    uint32_t acCol       = 0;
    uint32_t acStr       = 0;
    double   noizeLevel  = noiseLevel;   // In Db
    double   filterLevel = _filterLevel;  // In Db
    uint64_t x = 0;                       // X coordinate for filtration window
    uint64_t y = 0;                       // Y coordinate for filtration window

    uint8_t* inPic = nullptr;

    std::cout << "loading..." << std::endl;

    g_loadImage(filePath, col, str, &inPic);

    std::cout << "load complete!" << std::endl;

    uint8_t* data     = new uint8_t[col * str];
    uint8_t* pic      = new uint8_t[col * str * 3];
    uint8_t* dataSpec = nullptr;
    uint8_t* picSpec  = nullptr;

    g_toGrayScaleIn(col, str, inPic, data);
    std::cout << "GrayScaling complete!" << std::endl;

    RealSignal* testSig = new RealSignal(data, col, str);
    Signal* noiseSig = nullptr;

    acCol = testSig->GetNumberOfColomns();
    acStr = testSig->GetNumberOfStrings();

    dataSpec = new uint8_t[acCol * acStr];
    picSpec  = new uint8_t[acCol * acStr * 3];

    std::cout << "Signal created!" << std::endl;

    Signal specture(acCol, acStr);
    Signal* filteredSpectre = nullptr;
    Signal* filteredPic     = new Signal(acCol, acStr);
    Signal* interpol        = nullptr;
    Signal* invInterpol     = nullptr;

    std::complex<double>** sig     = nullptr;
    std::complex<double>** spec    = nullptr;
    std::complex<double>** filSpec = nullptr;
    std::complex<double>** filPic  = nullptr;

    // Gen Signal
    testSig->Resize();    // To pic
    testSig->GetPicture(data, false);

    g_toGrayScaleOut(col, str, data, pic);
    g_safeImage(std::string("Signal.bmp"), col, str, pic);

    //testSig->Resize(); // To data

    noiseSig = new Signal(*testSig);  // noise init pic

    // Adding noize
    g_noizeSignal(*noiseSig, noizeLevel);
    noiseSig->GetPicture(data, false);

    g_toGrayScaleOut(col, str, data, pic);
    g_safeImage(std::string("NoizeSignal.bmp"), col, str, pic);

    std::cout << "SNR: (square error): " << testSig->GetSquareError(*noiseSig) << " (Db)" << std::endl;
    std::cout << "SNR: (pixel error): " << testSig->GetPixelError(*noiseSig) << " (Db)" << std::endl;

    // Interpolation
//    testSig->Resize(); // To pic
    interpol = g_linInterpolReal(*noiseSig, acCol, acStr);

    // Get fft
    sig  = interpol->GetDataArray();
    spec = specture.GetDataArray();

    std::cout << "SigCol: " << interpol->GetNumberOfColomns()
              << "SigStr: " << interpol->GetNumberOfStrings()
              << std::endl;
    std::cout << "SpecCol: " << specture.GetNumberOfColomns()
              << "SpecStr: " << specture.GetNumberOfStrings()
              << std::endl;

    if (g_mfftDir(sig, spec, acStr, acCol, FT_DIRECT))
        std::cout << "FFT is ok!" << std::endl;

    filteredSpectre = g_squareFiltration(specture, filterLevel, x, y);

    // Out spectre
    Signal spectrePic(specture);
    std::cout << "Log scaling..." << std::endl;
    if (logScaleAnsw == "y")
    {
        sig = spectrePic.GetDataArray();
        logScale(sig, acCol, acStr);
    }

    spectrePic.GetPicture(dataSpec, false);

    std::cout << "Added border on spectre..." << std::endl;
    addWindowBorder(dataSpec, acCol, acStr, x, y, false);

    std::cout << "Move spec." << std::endl;
    movingSpectreInTheMiddle(dataSpec, acCol, acStr);

    g_toGrayScaleOut(acCol, acStr, dataSpec, picSpec);
    g_safeImage(std::string("Specture.bmp"), acCol, acStr, picSpec);

    // Out spectre
    Signal spectreFilPic(*filteredSpectre);
    std::cout << "Log scaling..." << std::endl;
    if (logScaleAnsw == "y")
    {
        sig = spectreFilPic.GetDataArray();
        logScale(sig, acCol, acStr);
    }

    // Draw fil spec
    spectreFilPic.GetPicture(dataSpec, false);

    std::cout << "Added border on spectre..." << std::endl;
    addWindowBorder(dataSpec, acCol, acStr, x, y, false);

    std::cout << "Move spec." << std::endl;
    movingSpectreInTheMiddle(dataSpec, acCol, acStr);

    g_toGrayScaleOut(acCol, acStr, dataSpec, picSpec);
    g_safeImage(std::string("FilSpecture.bmp"), acCol, acStr, picSpec);

    // Get filt pic
    filSpec = filteredSpectre->GetDataArray();
    filPic = filteredPic->GetDataArray();

    if (g_mfftDir(filSpec, filPic, acStr, acCol, FT_INVERSE))
        std::cout << "Inv FFT is ok!" << std::endl;

    invInterpol = g_linInterpolReal(*filteredPic, col, str);

    invInterpol->GetPicture(data, false);

    g_toGrayScaleOut(col, str, data, pic);
    g_safeImage(std::string("Filtered.bmp"), col, str, pic);

    std::cout << "SNR (square error): " << testSig->GetSquareError(*invInterpol) << " (Db)" << std::endl;
    std::cout << "SNR (pixel error): " << testSig->GetPixelError(*invInterpol) << " (Db)" << std::endl;

    testSig->Resize();  // to data

    delete[] inPic;
    delete[] data;
    delete[] pic;
    delete[] dataSpec;
    delete[] picSpec;
    delete testSig;
    delete noiseSig;
    delete filteredPic;
    delete filteredSpectre;
    delete interpol;
    delete invInterpol;
};