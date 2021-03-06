//
// Created by Timo and Khaled on 18.04.2016.
//

#ifndef CPP_BHT_SOSE16_A02_FIXED_POINT_H
#define CPP_BHT_SOSE16_A02_FIXED_POINT_H


#include <cstdint>
#include <cmath>

// numbertrait for always using the (s)mallest (p)ossible (d)atatype => spd
template<int bitSize>
struct use_spd{ using type = typename use_spd<bitSize + 1>::type; };

template<>
struct use_spd <8> { using type = int8_t; };

template<>
struct use_spd <16> { using type = int16_t; };

template<>
struct use_spd <32> { using type = int32_t; };

template<>
struct use_spd <64> { using type = int64_t; };

template <int8_t Decimal, int8_t Fraction>
class fixed_point {
    using impl_type = typename use_spd<Decimal + Fraction>::type;
    using doubled_impl_type = typename  use_spd<2*(Decimal + Fraction)>::type;

public:
    fixed_point(float x) {
        q = x * std::pow(2, Fraction);
    }

    explicit operator int() const {
        return q / std::pow(2, Fraction);
    }

    explicit operator float() const {
        return q / std::pow(2, Fraction);
    }

    template <int8_t DecimalF, int8_t FractionF>
    friend fixed_point<DecimalF, FractionF> abs(fixed_point<DecimalF, FractionF> fixedPoint);

    fixed_point operator+() const {
        fixed_point temp(*this);
        return temp;
    }

    fixed_point operator-() const {
        fixed_point temp(*this);
        temp.q = -temp.q;
        return temp;
    }

    fixed_point operator+(fixed_point rhs) const {
        fixed_point<Decimal, Fraction> temp(*this);
        temp.q += rhs.q;
        return temp;
    }

    fixed_point operator-(fixed_point rhs) const {
        fixed_point<Decimal, Fraction> temp = *this;
        temp.q -= rhs.q;
        return temp;
    }

    fixed_point operator*(fixed_point rhs) const {
        fixed_point<Decimal, Fraction> tempFixed(*this);
        doubled_impl_type temp = ((doubled_impl_type)q * (doubled_impl_type)rhs.q) >> Fraction;
        impl_type result = (impl_type)temp;

        tempFixed.q = result;

        return tempFixed;
    }

    fixed_point operator/(fixed_point rhs) const {
        fixed_point<Decimal, Fraction> tempFixed(*this);

        impl_type result;
        doubled_impl_type temp;

        temp = (doubled_impl_type) q << Fraction;

        if ((temp >= 0 && rhs.q >= 0) || (temp < 0 && rhs.q < 0)) {
            temp += rhs.q / 2;
        } else {
            temp -= rhs.q / 2;
        }

        result = (impl_type)(temp / rhs.q);

        tempFixed.q = result;

        return tempFixed;
    }

    fixed_point &operator+=(fixed_point rhs) {
        q += rhs.q;
        return *this;
    }

    fixed_point &operator-=(fixed_point rhs) {
        q -= rhs.q;
        return *this;
    }

    fixed_point &operator*=(fixed_point rhs) {
        doubled_impl_type temp = ((doubled_impl_type)q * (doubled_impl_type)rhs.q) >> Fraction;
        q = (impl_type) temp;
        return *this;
    }

    fixed_point &operator/=(fixed_point rhs) {
        impl_type result;
        doubled_impl_type temp;

        temp = (doubled_impl_type) q << Fraction;

        if ((temp >= 0 && rhs.q >= 0) || (temp < 0 && rhs.q < 0)) {
            temp += rhs.q / 2;
        } else {
            temp -= rhs.q / 2;
        }

        result = (impl_type)(temp / rhs.q);

        q = result;

        return *this;
    }

    bool operator==(fixed_point rhs) const {
        return q == rhs.q;
    }

    bool operator!=(fixed_point rhs) const {
        return q != rhs.q;
    }

    bool operator<(fixed_point rhs) const {
        return q < rhs.q;
    }

    bool operator>(fixed_point rhs) const {
        return q > rhs.q;
    }

    bool operator<=(fixed_point rhs) const {
        return q <= rhs.q;
    }

    bool operator>=(fixed_point rhs) const {
        return q >= rhs.q;
    }

    fixed_point& operator++() {
        q += std::pow(2, Fraction);
        return *this;
    }

    fixed_point operator++(int) {
        fixed_point<Decimal, Fraction> temp = *this;
        q += std::pow(2, Fraction);
        return temp;
    }

    fixed_point& operator--() {
        q -= std::pow(2, Fraction);
        return *this;
    }

    fixed_point operator--(int) {
        fixed_point<Decimal, Fraction> temp = *this;
        q -= std::pow(2, Fraction);
        return temp;
    }

private:
    impl_type q;
};

template <int8_t DecimalF, int8_t FractionF>
fixed_point<DecimalF, FractionF> abs(fixed_point<DecimalF, FractionF> fixedPoint) {
    if (fixedPoint >= 0) {
        return fixedPoint;
    } else {
        return -fixedPoint;
    }
};

template <int8_t Decimal, int8_t Fraction>
fixed_point<Decimal, Fraction> sin(fixed_point<Decimal, Fraction> x) {
    return x - (x*x*x) / 6 + (x*x*x*x*x) / 120 - (x*x*x*x*x*x*x) / 5040;
};

template <int8_t Decimal, int8_t Fraction>
fixed_point<Decimal, Fraction> cos(fixed_point<Decimal, Fraction> x) {
    return fixed_point<Decimal, Fraction>(1.f) - (x*x) / 2 + (x*x*x*x) / 24 - (x*x*x*x*x*x*x) / 720;
};

#endif //CPP_BHT_SOSE16_A02_FIXED_POINT_H
