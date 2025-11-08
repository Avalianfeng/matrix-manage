#include<iostream>
#include<cmath>
#ifndef DECIMAL_TO_FRACTION_HPP
#define DECIMAL_TO_FRACTION_HPP
class fraction {
public:
    int numerator;
    int denominator;
    fraction() : numerator(0), denominator(1) {}
    fraction(int num, int denom) : numerator(num), denominator(denom) {}
    double to_double() const {
        return static_cast<double>(numerator) / denominator;
    }
    std::string to_string() const {
        return std::to_string(numerator) + "/" + std::to_string(denominator);
    }
};
void decimalToFraction(double num,fraction &frac);

#endif
