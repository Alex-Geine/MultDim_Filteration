#ifndef SIGNAL_H
#define SIGNAL_H

#include <iostream>
#include <complex>

// Base class for signals representation
class Signal
{
    private:    // variables

    uint64_t        m_colomns   = 0;          //!< Number of colomns in matrix
    uint64_t        m_strings   = 0;          //!< Number of strings in matrix
    std::complex<double>** m_dataArray = nullptr;    //!< Array with points

    private:    // methods

    // Delete data array function
    void DeleteDataArray();

    public:

    // Delault constructor
    Signal();

    // Default destructor
    ~Signal();

    // Copy constructor
    Signal(const Signal& sig);

    // Move constructor
    Signal(Signal&& sig);

    // Copy operator
    Signal operator=(const Signal& sig);

    // Move operator
    Signal operator=(Signal&& sig);

    // Get number of colomns
    uint64_t GetNumberOfColomns();

    // Get number of strings
    uint64_t GetNumberOfStrings();

    // Get data array
    std::complex<double>** GetDataArray();
};

// Class witch representation Gauss signal
class GaussSignal : public Signal
{
    private:    // variables

    uint64_t        m_colomns       = 0;            //!< Number of colomns in matrix
    uint64_t        m_strings       = 0;            //!< Number of strings in matrix
    std::complex<double>** m_dataArray     = nullptr;      //!< Array with points

    uint64_t        m_numberOfGauss = 0;            //!< Number of Gauss functions in signal
    double*         m_x0Array       = nullptr;      //!< Array with x0 values of centers of functions
    double*         m_y0Array       = nullptr;      //!< Array with y0 values of centers of functions
    double*         m_amplArray     = nullptr;      //!< Array with values of amplitude of functions
    double*         m_sigmaXArray   = nullptr;      //!< Array with values of xSigma of function
    double*         m_sigmaYArray   = nullptr;      //!< Array with values of ySigma of function

    private:    // methods

    // Delete data array function
    void DeleteDataArray();

    // Delete other arrays
    void DeleteOtherArrays();

    // Gauss function
    double Gauss(uint64_t idFunc);

    public:

    // Delault constructor
    GaussSignal();

    // Default destructor
    ~GaussSignal();

    // Copy constructor
    GaussSignal(const GaussSignal& sig);

    // Move constructor
    GaussSignal(GaussSignal&& sig);

    // Copy operator
    GaussSignal operator=(const GaussSignal& sig);

    // Move operator
    GaussSignal operator=(GaussSignal&& sig);

    // Get number of colomns
    uint64_t GetNumberOfColomns();

    // Get number of strings
    uint64_t GetNumberOfStrings();

    // Get data array
    std::complex<double>** GetDataArray();

    // GaussSignal methods

    // Constructor
    GaussSignal( uint64_t colomns, uint64_t strings, uint64_t numberOfGauss,
                 double* x0Array, double* y0Array, double* amplArray,
                 double* sigmaXArray, double* sigmaYArray );
};

// Class whitch representation test signal (square)
class TestSignal : public Signal
{
    private:    // variables

    uint64_t        m_colomns   = 0;          //!< Number of colomns in matrix
    uint64_t        m_strings   = 0;          //!< Number of strings in matrix
    std::complex<double>** m_dataArray = nullptr;    //!< Array with points

    double          weight      = 0;          //!< Weight value for square signal
    double          heighth     = 0;          //!< Height value for squate signal

    private:    // methods

    // Delete data array function
    void DeleteDataArray();

    public:

    // Delault constructor
    TestSignal();

    // Default destructor
    ~TestSignal();

    // Copy constructor
    TestSignal(const TestSignal& sig);

    // Move constructor
    TestSignal(TestSignal&& sig);

    // Copy operator
    TestSignal operator=(const TestSignal& sig);

    // Move operator
    TestSignal operator=(TestSignal&& sig);

    // Get number of colomns
    uint64_t GetNumberOfColomns();

    // Get number of strings
    uint64_t GetNumberOfStrings();

    // Get data array
    std::complex<double>** GetDataArray();

    // Test signal methods

    // Constructor
    TestSignal(double weight, double height);
};

class RealSignal : public Signal
{
    private:    // variables

    uint64_t        m_colomns       = 0;          //!< Number of colomns in matrix
    uint64_t        m_strings       = 0;          //!< Number of strings in matrix
    std::complex<double>** m_dataArray     = nullptr;    //!< Array with points

    uint64_t        m_actualColomns = 0;          //!< Number of colomns of resized signal
    uint64_t        m_actualStrings = 0;          //!< Number of strings of resized signal

    private:    // methods

    // Delete data array function
    void DeleteDataArray();

    public:

    // Delault constructor
    RealSignal();

    // Default destructor
    ~RealSignal();

    // Copy constructor
    RealSignal(const RealSignal& sig);

    // Move constructor
    RealSignal(RealSignal&& sig);

    // Copy operator
    RealSignal operator=(const RealSignal& sig);

    // Move operator
    RealSignal operator=(RealSignal&& sig);

    // Get number of colomns
    uint64_t GetNumberOfColomns();

    // Get number of strings
    uint64_t GetNumberOfStrings();

    // Get data array
    std::complex<double>** GetDataArray();

    // Real signal methods

    // Constructor
    RealSignal(std::complex<double>** dataArray, uint64_t colomns, uint64_t strings);

    // Get number of actual colomns
    uint64_t GetActualNumberOfColomns();

    // Get number of actual strings
    uint64_t GetActualNumberOfStrings();
};

// Multidimensional Furie
bool g_MFFT(std::complex<double>** inData, std::complex<double>** outData, uint64_t size, bool flag);

// FFT
bool g_FFT(std::complex<double>* inData, std::complex<double>* outData, uint64_t size, bool flag);

// Noize function
bool g_NoizeSignal(const Signal& sig, double Db);

// Filtration function
bool g_SquareFiltration(const Signal& sig, double Db);

#endif    // SIGNAL_H
