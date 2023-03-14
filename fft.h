#include <cmath>
#include <iostream>
#include <string>
#include <vector>

#include "complex.h"

using namespace std;

// class FFT {
//    public:
vector<Complex> fft(vector<Complex> x);
vector<Complex> ifft(vector<Complex> x);
vector<Complex> cconvolve(vector<Complex>, vector<Complex>);
vector<Complex> convolve(vector<Complex>, vector<Complex>);
// };