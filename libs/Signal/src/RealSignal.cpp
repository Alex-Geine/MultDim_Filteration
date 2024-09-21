#include "Signal.h"

// Class RealSignal

// Default constructor
RealSignal::RealSignal(){};

// Default destructor
RealSignal::~RealSignal(){};

// Copy constructor
RealSignal::RealSignal(const RealSignal& sig) : Signal(sig)
{
    m_actualColomns = sig.m_actualColomns;
    m_actualStrings = sig.m_actualStrings;
};

// Move constructor
RealSignal::RealSignal(RealSignal&& sig) : Signal(sig)
{
    m_actualColomns = sig.m_actualColomns;
    m_actualStrings = sig.m_actualStrings;
};

// Copy operator
RealSignal RealSignal::operator=(const RealSignal& sig)
{
    Signal::operator=(sig);

    m_actualColomns = sig.m_actualColomns;
    m_actualStrings = sig.m_actualStrings;

    return *this;
};

// Move operator
RealSignal RealSignal::operator=(RealSignal&& sig)
{
    Signal::operator=(sig);

    m_actualColomns = sig.m_actualColomns;
    m_actualStrings = sig.m_actualStrings;

    return *this;
};

// Constructor
RealSignal::RealSignal(uint8_t* dataArray, uint64_t colomns, uint64_t strings) : Signal(PowersOfTwo(colomns), PowersOfTwo(strings))
{
    if ((colomns != 0) && (strings != 0) && (dataArray != nullptr))
    {
        m_actualColomns = colomns;
        m_actualStrings = strings;
     
        uint64_t size = colomns * strings;
        
        colomns = Signal::GetNumberOfColomns();
        strings = Signal::GetNumberOfStrings();

        std::complex<double>** data = Signal::GetDataArray();

        for (uint64_t i = 0; i < size; ++i)        
            data[i / colomns][i % colomns].real(dataArray[i]);
    }
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

// Cheking powers of two
uint64_t RealSignal::PowersOfTwo(uint64_t num)
{
    uint64_t n = 2;

    while (num % n != num)
        n * 2;

    return n;
};

// Resize
void RealSignal::Resize()
{
    Signal::SetNumberOfColomns(m_actualColomns);
    Signal::SetNumberOfStrings(m_actualStrings);
};