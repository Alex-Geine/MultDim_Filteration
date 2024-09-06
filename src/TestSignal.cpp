#include "Signal.h"

// Class TestSignal

// Default constructor
TestSignal::TestSignal(){};

// Default destructor
~TestSignal::TestSignal(){};

// Copy condtructor
TestSignal::TestSignal(const TestSignal& sig)
{

};

// Move constructor
TestSignal::TestSignal(const TestSignal& sig)
{

};

// Copy operator
TestSignal::TestSignal operator=(const TestSignal& sig)
{
    return *this;
};

// Move operator
TestSignal::TestSignal operator=(const TestSignal&& sig)
{
    return *this;
};

// Get number of colomns
uint64_t TestSignal::GetNumberOfColomns()
{
    return m_colomns;
};

// Get number of strings
uint64_t TestSignal::GetNumberOfStrings()
{
    return m_strings;
};

// Get data array
cmplx<double>** TestSignal::GetDataArray()
{
    return m_dataArray;
};

// TestSignal methods
TestSignal::TestSignal(double weight, double height)
{

};

