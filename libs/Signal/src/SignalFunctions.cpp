#include "Signal.h"
#include <vector>

// Signal functions

// FFT
bool g_fft(std::complex<double>* inData, std::complex<double>* outData, uint64_t size, uint64_t flag)
{
    // Prepare data
    uint64_t logN = std::log2(size);

    // Parameters error check:
    if ((inData == nullptr) || (outData == nullptr))       return false;
    if ((size > 16384) || (size < 1))                      return false;
    if (!NUMBER_IS_2_POW_K(size))                          return false;
    if ((logN < 2) || (logN > 14))                         return false;
    if ((flag != FT_DIRECT) && (flag != FT_INVERSE))       return false;

    // Output data
    if (inData != outData)
        for (uint64_t i = 0; i < size; ++i)
            outData[i] = inData[i];

    uint64_t  i, j, n, k, io, ie, in, nn;
    double    ru, iu, rtp, itp, rtq, itq, rw, iw, sr;

    static const double Rcoef[14] =
    {  -1.0000000000000000D,  0.0000000000000000D,  0.7071067811865475D,
        0.9238795325112867D,  0.9807852804032304D,  0.9951847266721969D,
        0.9987954562051724D,  0.9996988186962042D,  0.9999247018391445D,
        0.9999811752826011D,  0.9999952938095761D,  0.9999988234517018D,
        0.9999997058628822D,  0.9999999264657178D
    };
    static const double Icoef[14] =
    {   0.0000000000000000D, -1.0000000000000000D, -0.7071067811865474D,
       -0.3826834323650897D, -0.1950903220161282D, -0.0980171403295606D,
       -0.0490676743274180D, -0.0245412285229122D, -0.0122715382857199D,
       -0.0061358846491544D, -0.0030679567629659D, -0.0015339801862847D,
       -0.0007669903187427D, -0.0003834951875714D
    };

    nn = size >> 1;
    ie = size;

    for (n = 1; n <= logN; ++n)
    {
        rw = Rcoef[logN - n];
        iw = Icoef[logN - n];

        if (flag == FT_INVERSE)
            iw = -iw;

        in = ie >> 1;
        ru = 1.0D;
        iu = 0.0D;

        for (j = 0; j < in; ++j)
        {
            for (i = j; i < size; i += ie)
            {
                io       = i + in;
                rtp      = outData[i].real()  + outData[io].real();
                itp      = outData[i].imag()  + outData[io].imag();
                rtq      = outData[i].real()  - outData[io].real();
                itq      = outData[i].imag()  - outData[io].imag();

                outData[io].real(rtq * ru - itq * iu);
                outData[io].imag(itq * ru + rtq * iu);
                outData[i].real(rtp);
                outData[i].imag(itp);
            }

            sr = ru;
            ru = ru * rw - iu * iw;
            iu = iu * rw + sr * iw;
        }

        ie >>= 1;
    }

    for (j = i = 1; i < size; ++i)
    {
        if (i < j)
        {
            io       = i - 1;
            in       = j - 1;
            rtp      = outData[in].real();
            itp      = outData[in].imag();

            outData[in] = outData[io];
            outData[io].real(rtp);
            outData[io].imag(itp);
        }

        k = nn;

        while (k < j)
        {
            j   = j - k;
            k >>= 1;
        }

        j = j + k;
    }

    if (flag == FT_DIRECT)
       return true;

    rw = 1.0D / size;

    for (i = 0; i < size; ++i)
        outData[i] *= rw;

    return true;
};

// Multidimensional FFT
bool g_mfftDir(std::complex<double>** inData, std::complex<double>** outData, uint64_t strings, uint64_t colomns, uint64_t flag)
{
    if (inData  == nullptr)   return false;
    if (outData == nullptr)   return false;
    if (strings == 0)         return false;
    if (colomns == 0)         return false;
    uint64_t i;
    uint64_t j;

    std::complex<double>* data = new std::complex<double>[colomns];
    //std::vector<cd> data(colomns);
    bool res;
    bool inverse = false;

    // Colomns
    for (i = 0; i < strings; ++i)
    {
        for (j = 0; j < colomns; ++j)
            data[j] = inData[i][j];

        res = g_fft(data, data, colomns, flag);
        //fft(data, )

        if ( !res )
        {
            delete[] data;

            return false;
        }

        for (j = 0; j < colomns; ++j)
           outData[i][j] = data[j];
    }

    delete[] data;
    data = new std::complex<double>[strings];

    // Strings
    for (j = 0; j < colomns; ++j)
    {
        for (uint64_t i = 0; i < strings; ++i)
            data[i] = outData[i][j];

        res = g_fft(data, data, strings, flag);

        if ( !res )
        {
            delete[] data;

            return false;
        }

        for (i = 0; i < strings; ++i)
            outData[i][j] = data[i];
    }

    delete[] data;

    return true;
};

// Random generator
double g_random()
{
    double res = 0;

    for (uint64_t i = 0; i < 12; ++i)
        res += -1. + 2 * (double)std::rand() / RAND_MAX;

    return res;
};

// Noize function
bool g_noizeSignal(Signal& sig, double Db)
{
    uint64_t colomns = sig.GetNumberOfColomns();
    uint64_t strings = sig.GetNumberOfStrings();

    if ((colomns == 0) || (strings == 0))
        return false;

    Signal noise(colomns, strings);
    std::complex<double>** noiseData = noise.GetDataArray();
    std::complex<double>** sigData   = sig.GetDataArray();

    std::complex<double>   deletedValue    = sigData[0][0];

    // Delete first point
    sigData[0][0].real(0);
    sigData[0][0].imag(0);

    for (uint64_t i = 0; i < strings; ++i)
        for (uint64_t j = 0; j < colomns; ++j)
            noiseData[i][j].real(g_random());

    double energySig   = sig.GetEnergy();
    double energyNoise = noise.GetEnergy();
    double koef        = 1. / std::pow(10, Db / 10);

    std::cout << "koef: " << koef << ", Db: " << Db << std::endl;

    for (uint64_t i = 0; i < strings; ++i)
         for (uint64_t j = 0; j < colomns; ++j)
             sigData[i][j].real(sigData[i][j].real() + koef * noiseData[i][j].real());

   sigData[0][0] = deletedValue;

    return true;
};

// Filtration function
Signal* g_squareFiltration(Signal& sig, double Db)
{
    if (Db < 0)
        Db = 0;

    std::cout << "Start filtering.." << std::endl;
    Signal* filteredSignal = new Signal(sig);

    std::complex<double>** realSigData     = sig.GetDataArray();
    std::complex<double>** filteredSigData = filteredSignal->GetDataArray();
    std::complex<double>   deletedValue    = realSigData[0][0];

    // Delete first point
    realSigData[0][0].real(0);
    realSigData[0][0].imag(0);
    filteredSigData[0][0].real(0);
    filteredSigData[0][0].imag(0);

    double   persent = 0.01;
    uint64_t strings = sig.GetNumberOfStrings();
    uint64_t colomns = sig.GetNumberOfColomns();
    uint64_t centIdx = colomns / 2;
    uint64_t centIdy = strings / 2;
    uint64_t idx     = centIdx;
    uint64_t idy     = centIdy;
    uint64_t dIdx    = (double)colomns / 2. * persent;
    uint64_t dIdy    = (double)strings / 2. * persent;

    if (dIdx == 0)
        dIdx = 1;
    if (dIdy == 0)
        dIdy = 1;

    double energyRealSig = sig.GetEnergy();
    double energyFiltSig = filteredSignal->GetEnergy();

    uint64_t newIdx = 0;
    uint64_t newIdy = 0;

    double curLevel = 10 * std::log10(energyRealSig / energyFiltSig);

    std::cout << "Db: " << Db << ", curLevel: " << curLevel << std::endl;

    // Processing filtration
    while (Db > curLevel)
    {
        if ((idx <= dIdx) && (idy <= dIdy))
        {
            realSigData[0][0]     = deletedValue;
            filteredSigData[0][0] = deletedValue;

            return filteredSignal;
        }

        newIdx = idx - dIdx;
        newIdy = idy - dIdy;

        // Colomns
        for (uint64_t i = 0; i < strings; ++i)
        {
            for (uint64_t j = newIdx; j <= idx; ++j)
            {
                // Left
                filteredSigData[i][j].real(0);
                filteredSigData[i][j].imag(0);

                //Right
                filteredSigData[i][centIdx + (centIdx - j)].real(0);
                filteredSigData[i][centIdx + (centIdx - j)].imag(0);
            }
        }

        // Strings
        for (uint64_t i = 0; i < colomns; ++i)
        {
            for (uint64_t j = newIdy; j <= idy; ++j)
            {
                // Bot
                filteredSigData[j][i].real(0);
                filteredSigData[j][i].imag(0);

                // Top
                filteredSigData[centIdy + (centIdy - j)][i].real(0);
                filteredSigData[centIdy + (centIdy - j)][i].imag(0);
            }
        }

        idx -= dIdx;
        idy -= dIdy;

        energyRealSig = sig.GetEnergy();
        energyFiltSig = filteredSignal->GetEnergy();

        curLevel = 10 * std::log10(energyRealSig / energyFiltSig);
    }

    realSigData[0][0]     = deletedValue;
    filteredSigData[0][0] = deletedValue;

    return filteredSignal;
};
