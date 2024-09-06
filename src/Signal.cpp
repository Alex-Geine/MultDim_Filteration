#include "Signal.h"

// Signal class

// Default constructor
Signal::Signal(){};

// Default destructor
~Signal()
{
    DeleteDataArray();
};

// Copy constructor
Signal::Signal(conts Signal& sig)
{
    m_colomns = sig.m_colomns;
    m_strings = sig.m_strings;

    m_dataArray = new cmplx<double>*[m_strings];
    for (uint64_t i = 0; i < m_strings; ++i)
    {
        m_dataArray[i] = new cmplx<double>[m_colomns];
        for (uint64_t j = 0; j < m_colomns; ++j)
            m_dataArray[i][j] = sig.m_dataArray[i][j];
    }
};

// Move constructor
Signal::Signal(const Signal&& sig)
{
    m_colomns   = sig.m_colomns;
    m_strings   = sig.m_strings;
    m_dataArray = sig.m_dataArray;

    sig.m_dataArray = nullptr;
};

// Copy operator
Signal::Signal operator=(const Signal& sig)
{
    if (*this == sig)
        return *this;

    m_colomns = sig.m_colomns;
    m_strings = sig.m_strings;

    DeleteDataArray();

    m_dataArray = new cmplx<double>*[m_strings];
    for (uint64_t i = 0; i < m_strings; ++i)
    {
        m_dataArray[i] = new cmplx<double>[m_colomns];
        for (uint64_t j = 0; j < m_colomns; ++j)
            m_dataArray[i][j] = sig.m_dataArray[i][j];
    }

    return *this;
};

// Move operator
Signal::Signal operator=(const Signal&& sig)
{
    if (*this == sig)
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

// Get data array
cmplx<double>** Signal::GetDataArray()
{
    return m_dataArray;
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
