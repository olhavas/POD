//
// Created by olga on 30/06/19.
//

#ifndef POD_COMPLEX_H
#define POD_COMPLEX_H


#include <opencv2/core/core.hpp>
#include "opencv2/imgcodecs.hpp"
#include <opencv2/highgui/highgui.hpp>
#include "opencv2/opencv.hpp"
#include <iostream>
#include <cmath>



class Complex {
public:
    double r;
    double i;
    Complex();
    Complex(const Complex &v);
    Complex(const double &r, const double &i);
    void operator = (Complex v);
    void operator = (double v);
    Complex operator - (const Complex v);
    Complex operator + (const Complex v);
    Complex operator * (const Complex v);
    Complex operator * (double v);
    void operator *= (const Complex v);
    void operator *= (double v);
    Complex* operator += (const Complex v);
    Complex* operator -= (const Complex v);
    void operator /= (Complex v);
    bool operator == (Complex v);
    bool operator != (Complex v);
    double mag() const;
    double squaredSum();
    double angle();

    void setR(double r);

    void setI(double i);

};

std::ostream& operator<<(std::ostream& output, const Complex& v);

#endif //POD_COMPLEX_H
