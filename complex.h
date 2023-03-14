#ifndef COMPLEX_H
#define COMPLEX_H

#include <cmath>
#include <iostream>
#include <string>

using namespace std;

class Complex {
   public:
    Complex();
    Complex(double real, double imag);
    string toString();
    double abs();
    double phase();
    Complex plus(Complex b);
    Complex minus(Complex b);
    Complex times(Complex b);
    Complex scale(double alpha);
    Complex conjugate();
    Complex reciprocal();
    double real();
    double imag();
    Complex divides(Complex b);
    Complex cexp();
    Complex csin();
    Complex ccos();
    Complex ctan();
    Complex plus(Complex a, Complex b);

    double re;
    double im;
};
#endif