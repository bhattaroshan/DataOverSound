
#include "fft.h"

vector<Complex> fft(vector<Complex> x) {
    int n = x.size();
    // cout << "my size is " << n << endl;
    if (n == 1) {
        return {x[0]};
    }

    if (n % 2 != 0) return {};  //WARNING!!! error must be power of 2, this line probably has a bug this line only checks if n is odd or even

    vector<Complex> even;
    // Complex* even = new Complex[n / 2];

    for (int k = 0; k < n / 2; k++) {
        // even[k] = x[2 * k];
        even.push_back(x[2 * k]);
    }

    vector<Complex> q = fft(even);

    vector<Complex> odd = even;

    for (int k = 0; k < n / 2; k++) {
        odd[k] = x[2 * k + 1];
    }
    vector<Complex> r = fft(odd);

    vector<Complex> y(n);

    for (int k = 0; k < n / 2; k++) {
        double kth = -2 * k * M_PI / n;
        Complex wk = Complex(cos(kth), sin(kth));
        y[k] = q[k].plus(wk.times(r[k]));
        y[k + n / 2] = q[k].minus(wk.times(r[k]));
    }
    return y;
}

// Complex* ifft(Complex* x) {
//     int n = sizeof(x) / sizeof(Complex);
//     Complex* y = new Complex[n];
//     for (int i = 0; i < n; i++) {
//         y[i] = x[i].conjugate();
//     }
//     y = fft(y);

//     for (int i = 0; i < n; i++) {
//         y[i] = y[i].conjugate();
//     }

//     for (int i = 0; i < n; i++) {
//         y[i] = y[i].scale(1.0 / n);
//     }
//     return y;
// }

// Complex* cconvolve(Complex* x, Complex* y) {
//     int xlen = sizeof(x) / sizeof(Complex);
//     int ylen = sizeof(y) / sizeof(Complex);

//     if (xlen != ylen) return NULL;  //Dimensions doesn't match

//     int n = xlen;

//     Complex* a = fft(x);
//     Complex* b = fft(y);

//     Complex* c = new Complex[n];

//     for (int i = 0; i < n; i++) {
//         c[i] = a[i].times(b[i]);
//     }

//     return ifft(c);
// }

// Complex* convolve(Complex* x, Complex* y) {
//     int xlen = sizeof(x) / sizeof(Complex);
//     int ylen = sizeof(y) / sizeof(Complex);
//     Complex ZERO = Complex(0, 0);
//     Complex* a = new Complex[2 * xlen];
//     for (int i = 0; i < xlen; i++) a[i] = x[i];
//     for (int i = xlen; i < 2 * xlen; i++) a[i] = ZERO;

//     Complex* b = new Complex[2 * ylen];
//     for (int i = 0; i < ylen; i++) b[i] = y[i];
//     for (int i = ylen; i < 2 * ylen; i++) b[i] = ZERO;
//     return cconvolve(a, b);
// }