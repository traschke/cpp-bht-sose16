#include "fixed_point.h"

#include <iostream>
#include <cassert>

// -----------------------------------------------------------------------------
// advanced assert
// -----------------------------------------------------------------------------

#define EXPECT_EQ(X, Y)                            \
{                                                     \
    auto x = X;                                       \
    auto y = Y;                                       \
    if ( !(x == y) ) {                                \
        std::cerr << std::boolalpha                   \
        << "! assertion failed in line " << __LINE__  \
        << ": expected " << x                         \
        << ", got " << y << std::endl;                \
    }                                                 \
}                                                     \

#define EXPECT_CLOSE(X, Y, D)                     \
{                                                     \
    auto x = X;                                       \
    auto y = Y;                                       \
    auto d = D;                                       \
    if ( abs(x-y) >= d   ) {                          \
        std::cerr << std::boolalpha                   \
        << "! assertion failed in line " << __LINE__  \
        << ": expected " << (x-y)                     \
        << " to be less than " << d << std::endl;     \
    }                                                 \
}                                                     \


template<int8_t Decimal, int8_t Fraction>
auto operator<<(std::ostream &os, fixed_point<Decimal, Fraction> const &f) -> std::ostream & {
    os << float(f);
    return os;
}

template<int8_t Decimal, int8_t Fraction>
int test() {
    const fixed_point<Decimal, Fraction> fp1(6.375f);
    const fixed_point<Decimal, Fraction> fp2(-4.0f);

    // -------------------------------------------------------------------------
    EXPECT_EQ(6, int(fp1));
    EXPECT_EQ(-4, int(fp2));

    EXPECT_EQ(fp1, abs(fp1));
    EXPECT_EQ(-fp2, abs(fp2));

    // -------------------------------------------------------------------------
    // comparison and ordering
    assert(fp1 == (fixed_point<Decimal, Fraction>(6.375)));
    assert(-fp1 == (fixed_point<Decimal, Fraction>(-6.375f)));
    assert(fp2 != fp1);
    assert(fp2 < fp1);
    assert(fp1 > fp2);
    assert(fp2 <= fp1);
    assert(fp1 >= fp2);

    // -------------------------------------------------------------------------
    // arithmetics
    EXPECT_EQ((fixed_point<Decimal, Fraction>(2.375f)), fp1 + fp2);
    EXPECT_EQ((fixed_point<Decimal, Fraction>(10.375f)), fp1 - fp2);
    EXPECT_EQ((fixed_point<Decimal, Fraction>(-25.5f)), fp1 * fp2);
    EXPECT_EQ((fixed_point<Decimal, Fraction>(-1.59375f)), fp1 / fp2);
    // -------------------------------------------------------------------------
    // arithmetics assignment
    fixed_point<Decimal, Fraction>
            fp3 = fp1;
    fp3 += fp2;
    EXPECT_EQ((fixed_point<Decimal, Fraction>(2.375f)), fp3);
    fp3 = fp1;
    fp3 -= fp2;
    EXPECT_EQ((fixed_point<Decimal, Fraction>(10.375f)), fp3);
    fp3 = fp1;
    fp3 *= fp2;
    EXPECT_EQ((fixed_point<Decimal, Fraction>(-25.5f)), fp3);
    fp3 = fp1;
    fp3 /= fp2;
    EXPECT_EQ((fixed_point<Decimal, Fraction>(-1.59375f)), fp3);

    // -------------------------------------------------------------------------
    // pre/post - increment/decrement
    fp3 = fp1;
    EXPECT_EQ((fixed_point<Decimal, Fraction>(7.375f)), ++fp3);
    fp3 = fp1;
    EXPECT_EQ((fixed_point<Decimal, Fraction>(5.375f)), --fp3);
    fp3 = fp1;
    EXPECT_EQ((fixed_point<Decimal, Fraction>(6.375f)), fp3++);
    EXPECT_EQ((fixed_point<Decimal, Fraction>(7.375f)), fp3);
    fp3 = fp1;
    EXPECT_EQ((fixed_point<Decimal, Fraction>(6.375f)), fp3--);
    EXPECT_EQ((fixed_point<Decimal, Fraction>(5.375f)), fp3);

    // -------------------------------------------------------------------------
    // trigonometric functions
    auto sinus1 = fixed_point<Decimal, Fraction>(std::sin(0.5f));
    auto sinus2 = sin((fixed_point<Decimal, Fraction>) (0.5f));

    auto cosine1 = fixed_point<Decimal, Fraction>(std::cos(0.5f));
    auto cosine2 = cos((fixed_point<Decimal, Fraction>) (0.5f));

    auto fpt = fixed_point<Decimal, Fraction>(0.01f);

    EXPECT_CLOSE(sinus1, sinus2, fpt);
    EXPECT_CLOSE(cosine1, cosine2, fpt);
}

int main() {
    test<16, 16>();
    test<8, 7>();
    test<24, 8>();
    //test<5, 3>();
    std::cout << "Test ended." << std::endl;
    return 0;
}
