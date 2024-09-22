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

        uint64_t l = uint64_t(double(N) * weight / (double)100);

        std::cout << "l: " << l << ", N: " << N <<  std::endl;

        uint64_t               idxMax  = l + (N - l) / 2;
        uint64_t               idxMin  = (N - l) / 2;
        uint64_t               idyMax  = idxMax;
        uint64_t               idyMin  = idxMin;

        std::cout << "idxMax: " << idxMax << ", idxMin: " << idxMin << ", idYMax: " << idyMax << ", idyMin: " << idyMin << std::endl;

        for (uint64_t i = idxMin; i < idxMax; ++i)
            for (uint64_t j = idyMin; j < idyMax; ++j)
                data[i][j] = std::complex<double>{255, 0};

        //for (uint64_t i = 0; i < N * N; ++i)
        //     std::cout << std::abs(data[i / N][i % N]) << " " << std::endl;

    }
};

