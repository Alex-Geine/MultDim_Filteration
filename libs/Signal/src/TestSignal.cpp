 #include "Signal.h"

// Class TestSignal

// Default constructor
TestSignal::TestSignal(){};

// Default destructor
TestSignal::~TestSignal(){};

// Copy condtructor
TestSignal::TestSignal(const TestSignal& sig) : Signal(sig){};

// Move constructor
TestSignal::TestSignal(TestSignal&& sig) : Signal(sig){};

// Copy operator
TestSignal TestSignal::operator=(const TestSignal& sig)
{
    Signal::operator=(sig);

    return *this;
};

// Move operator
TestSignal TestSignal::operator=(TestSignal&& sig)
{
    Signal::operator=(sig);

    return *this;
};

// Constructor
TestSignal::TestSignal(double weight) : Signal(N ,N)
{
    if ((weight != 0) && (weight <= 100))
    {
        std::complex<double>** data = GetDataArray();

        double                 left  = -0.5;
        double                 rigth = 0.5;
        double                 top   = 0.5;
        double                 bot   = -0.5;

        uint64_t               idxMax  = uint64_t(double(N) * weight / (double)100);
        uint64_t               idxMin  = N - idxMax;
        uint64_t               idyMax  = idxMax;
        uint64_t               idyMin  = idxMin;

        for (uint64_t i = idxMin; i < idxMax; ++i)
            for (uint64_t j = idyMin; j < idyMax; ++j)
                data[i][j].real(1);
    }
};

