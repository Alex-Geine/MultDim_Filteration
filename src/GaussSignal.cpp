#include <Signal.h>

// class GaussSignal

// Default constructor
GaussSignal::GaussSignal(){};

// Default destructor
GaussSignal::~GaussSignal(){};

// Copy constructor
GaussSignal::GaussSignal(const GaussSignal& sig)
{

};

// Move constructor
GaussSignal::GaussSignal(GaussSignal&& sig)
{

};

// Copy operator
GaussSignal GaussSignal::operator=(const GaussSignal& sig)
{
    return *this;
};

// Move operator
GaussSignal GaussSignal::operator=(GaussSignal&& sig)
{
    return *this;
};

// Get number of colomns
uint64_t GaussSignal::GetNumberOfColomns()
{
    return m_colomns;
};

// Get number if strings
uint64_t GaussSignal::GetNumberOfStrings()
{
    return m_strings;
};

// Get data array
std::complex<double>** GaussSignal::GetDataArray()
{
    return m_dataArray;
};

// GaussSignal methods

// Constructor
GaussSignal::GaussSignal ( uint64_t colomns, uint64_t strings, uint64_t numberOfGauss,
                           double* x0Array, double* y0Array,  double* amplArray,
                           double* sigmaXArray, double* sigmaYArray )
{

};

// Delete other arrays
void GaussSignal::DeleteOtherArrays()
{
    if (m_x0Array != nullptr)
        delete[] m_x0Array;

    if (m_y0Array != nullptr)
        delete[] m_y0Array;

    if (m_amplArray != nullptr)
        delete[] m_amplArray;

    if (m_sigmaXArray != nullptr)
        delete[] m_sigmaXArray;

    if (m_sigmaYArray != nullptr)
        delete[] m_sigmaYArray;
};

// Gauss function
double GaussSignal::Gauss(uint64_t idFunc)
{
    return 0;
};


