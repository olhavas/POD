//
// Created by olga on 30/06/19.
//

#include "Complex.h"


Complex::Complex()
{
    this->r = 0;
    this->i = 0;
}
Complex::Complex(const Complex &v) : r(v.r), i(v.i)
{

}
Complex::Complex(const double &r, const double &i=0)
{
    this->r = r;
    this->i = i;
}
void Complex::operator=(Complex v)
{
    r = v.r;
    i = v.i;
}


void Complex::operator=(double v)
{
    r = v;
    i = v;
}

Complex Complex::operator*(double v)
{
    return Complex(r*v, i*v);
}

Complex Complex::operator+(const Complex v)
{
    return Complex(r+v.r, i+v.i);
}

Complex Complex::operator-(const Complex v)
{
    return Complex(r-v.r, i-v.i);
}

Complex Complex::operator*(const Complex v)
{
    return Complex(r*v.r - i*v.i,
                     r*v.i + i*v.r);
}

Complex* Complex::operator+=(const Complex v)
{
    r += v.r;
    i += v.i;
    return this;
}

Complex* Complex::operator-=(const Complex v)
{
    r -= v.r;
    i -= v.i;
    return this;
}

void Complex::operator*=(const Complex v)
{
    r *= v.r;
    i *= v.i;
}

void Complex::operator*=(double v)
{
    r *= v;
    i *= v;
}

double Complex::mag() const {
    return sqrt(r*r + i*i);
}

double Complex::squaredSum()
{
    return r*r + i*i;
}

double Complex::angle()
{
    double a = atan2(r, i);
    if(a < 0) {
        a = (M_PI * 2) + a;
    }
    return a;
}

bool Complex::operator==(Complex v) {
    return (this->r == v.r && this->i == v.i);

}

bool Complex::operator != (Complex v)
{
    return (this->r != v.r || this->i != v.i);
}

std::ostream& operator<<(std::ostream& output, const Complex& v) {
    output.setf(std::ios::fixed, std::ios::floatfield);
    output.precision(2);
    if(abs(v.i) > 0.001){
        if(v.i < 0)
            output  << v.r << " - " << abs(v.i)<<"i";
        else
            output  << v.r << " + " << abs(v.i)<<"i";
    }else{
        if (abs(v.r) < 0.001)
            output << abs(v.r);
        else
            output << v.r;
    }
    return output;
}