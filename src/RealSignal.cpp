#include "Signal.h"

// Class RealSignal

// Default constructor
RealSignal::RealSignal(){};

// Default destructor
RealSignal::~RealSignal(){};

// Copy constructor
RealSignal::RealSignal(const RealSignal& sig)
{

};

// Move constructor
RealSignal::RealSignal(RealSignal&& sig)
{

};

// Copy operator
RealSignal RealSignal::operator=(const RealSignal& sig)
{
    return *this;
};

// Move operator
RealSignal RealSignal::operator=(RealSignal&& sig)
{
    return *this;
};

// Get number of colomns
uint64_t RealSignal::GetNumberOfColomns()
{
    return m_colomns;
};

// Get number of strings
uint64_t RealSignal::GetNumberOfStrings()
{
    return m_strings;
};

// Get data array
std::complex<double>** RealSignal::GetDataArray()
{
    return m_dataArray;
};

// RealSignal methods

// Constructor
RealSignal::RealSignal(std::complex<double>** dataArray, uint64_t colomns, uint64_t strings)
{

};

// Get nnumber of actual colomns
uint64_t RealSignal::GetActualNumberOfColomns()
{
    return m_actualColomns;
};

// Get number of actual strings
uint64_t RealSignal::GetActualNumberOfStrings()
{
    return m_actualStrings;
};

