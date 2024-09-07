#include "Signal.h"

// Signal class

// Default constructor
Signal::Signal(){};

// Constructor with zeor data
Signal::Signal(uint64_t colomns, uint64_t strings) : m_colomns(colomns), m_strings(strings)
{
    if ((m_colomns != 0) && (m_strings != 0))
    {
        m_dataArray = new std::complex<double>*[m_strings];

        for (uint64_t i = 0; i < m_strings; ++i)
            m_dataArray[i] = new std::complex<double>[m_colomns];
    }
};

// Default destructor
Signal::~Signal()
{
    DeleteDataArray();
};

// Copy constructor
Signal::Signal(const Signal& sig)
{
    m_colomns = sig.m_colomns;
    m_strings = sig.m_strings;

    m_dataArray = new std::complex<double>*[m_strings];
    for (uint64_t i = 0; i < m_strings; ++i)
    {
        m_dataArray[i] = new std::complex<double>[m_colomns];
        for (uint64_t j = 0; j < m_colomns; ++j)
            m_dataArray[i][j] = sig.m_dataArray[i][j];
    }
};

// Move constructor
Signal::Signal(Signal&& sig)
{
    m_colomns   = sig.m_colomns;
    m_strings   = sig.m_strings;
    m_dataArray = sig.m_dataArray;

    sig.m_dataArray = nullptr;
};

// Copy operator
Signal Signal::operator=(const Signal& sig)
{
    if (this == &sig)
        return *this;

    m_colomns = sig.m_colomns;
    m_strings = sig.m_strings;

    DeleteDataArray();

    m_dataArray = new std::complex<double>*[m_strings];
    for (uint64_t i = 0; i < m_strings; ++i)
    {
        m_dataArray[i] = new std::complex<double>[m_colomns];
        for (uint64_t j = 0; j < m_colomns; ++j)
            m_dataArray[i][j] = sig.m_dataArray[i][j];
    }

    return *this;
};

// Move operator
Signal Signal::operator=(Signal&& sig)
{
    if (this == &sig)
        return *this;

    m_colomns = sig.m_colomns;
    m_strings = sig.m_strings;

    DeleteDataArray();

    m_dataArray     = sig.m_dataArray;
    sig.m_dataArray = nullptr;

    return *this;
};

// Get number of colomns
uint64_t Signal::GetNumberOfColomns()
{
    return m_colomns;
};

// Get number of strings
uint64_t Signal::GetNumberOfStrings()
{
    return m_strings;
};

// Set number of colomns
void Signal::SetNumberOfColomns(uint64_t colomns)
{
    m_colomns = colomns;
}

// Set number of strings
void Signal::SetNumberOfStrings(uint64_t strings)
{
    m_strings = strings;
}

// Get data array
std::complex<double>** Signal::GetDataArray()
{
    return m_dataArray;
};

// Get energy of signal
double Signal::GetEnergy()
{
    double energy = 0;

    for (uint64_t i = 0; i < m_strings; ++i)
    {
        for (uint64_t j = 0; j < m_colomns; ++j)
        {
            energy += m_dataArray[i][j].real() * m_dataArray[i][j].real();
            energy += m_dataArray[i][j].imag() * m_dataArray[i][j].imag();
        }
    }

    return energy;
};

// Delete data array function
void Signal::DeleteDataArray()
{
    if (m_dataArray != nullptr)
        for (uint64_t i = 0; i < m_strings; ++i)
            if (m_dataArray[i] != nullptr)
                delete[] m_dataArray[i];

    delete[] m_dataArray;

    return;
};
