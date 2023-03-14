#include "complex.h"

Complex::Complex() {
}

Complex::Complex(double real, double imag) {
    re = real;
    im = imag;
}

string Complex::toString() {
    if (im == 0) return to_string(re) + "";
    if (re == 0) return to_string(im) + "i";
    if (im < 0) return to_string(re) + "-" + to_string(-im) + "i";
    return to_string(re) + " + " + "i";
}

double Complex::abs() {
    return hypot(re, im);
}

double Complex::phase() {
    return atan2(im, re);
}

Complex Complex::plus(Complex b) {
    Complex a = *this;
    double real = a.re + b.re;
    double imag = a.im + b.im;
    return Complex(real, imag);
}

Complex Complex::minus(Complex b) {
    Complex a = *this;
    double real = a.re - b.re;
    double imag = a.im - b.im;
    return Complex(real, imag);
}

Complex Complex::times(Complex b) {
    Complex a = *this;
    double real = a.re * b.re - a.im * b.im;
    double imag = a.re * b.im + a.im * b.re;
    return Complex(real, imag);
}

Complex Complex::scale(double alpha) {
    return Complex(alpha * re, alpha * im);
}

Complex Complex::conjugate() {
    return Complex(re, -im);
}

Complex Complex::reciprocal() {
    double scale = re * re + im * im;
    return Complex(re / scale, -im / scale);
}
double Complex::real() {
    return re;
}

double Complex::imag() {
    return im;
}

Complex Complex::divides(Complex b) {
    Complex a = *this;
    return a.times(b.reciprocal());
}

Complex Complex::cexp() {
    return Complex(exp(re) * cos(im), exp(re) * sin(im));
}

Complex Complex::csin() {
    return Complex(sin(re) * cosh(im), cos(re) * sinh(im));
}

Complex Complex::ccos() {
    return Complex(cos(re) * cosh(im), -sin(re) * sinh(im));
}

Complex Complex::ctan() {
    return csin().divides(ccos());
}

Complex Complex::plus(Complex a, Complex b) {
    double real = a.re + b.re;
    double imag = a.im + b.im;
    Complex sum = Complex(real, imag);
    return sum;
}