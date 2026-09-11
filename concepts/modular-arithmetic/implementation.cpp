// Modular Arithmetic -- g++ -std=c++17 -O2 implementation.cpp

#include <iostream>
#include <string>
#include <vector>

using ll = long long;

// ---------------------------------------------------------------- the primitives

// The mathematical residue, always in [0, m).  C++ '%' truncates toward zero, so it
// follows the sign of the dividend: -17 % 5 == -2, not 3.  Harmless in a comparison,
// fatal as an array index or a hash key.  Normalize in ONE place and use it everywhere.
// O(1)
ll norm(ll x, ll m) {
    x %= m;
    return x < 0 ? x + m : x;
}

// a*b mod m without overflow.  'a * b % m' evaluates a*b FIRST, so it wraps whenever the
// product passes 9.22e18 -- that is any m above ~3e9.  __int128 holds the product, then
// the reduction brings it back.  This is the most common silent wrong answer in the
// subject: no crash, no warning, just a different number.
// O(1)
ll mulMod(ll a, ll b, ll m) { return (ll)((__int128)a * b % m); }

// Both operands are assumed already reduced into [0, m).  Addition can reach 2m, which is
// why the test is a subtraction and not a '%': cheaper, and it cannot overflow while
// m < 4.6e18.
// O(1)
ll addMod(ll a, ll b, ll m) {
    ll s = a + b;
    return s >= m ? s - m : s;
}

// Subtraction is where the negative-remainder trap actually bites, since a < b is common.
// O(1)
ll subMod(ll a, ll b, ll m) {
    ll d = a - b;
    return d < 0 ? d + m : d;
}

// ---------------------------------------------------------------- uses

// Product of a whole array mod m, reducing at every step so nothing ever exceeds m^2.
// Never build the true product first -- that is the point of the concept.
// O(n)
ll productMod(const std::vector<ll> &v, ll m) {
    ll r = 1 % m;                       // 1 % m, not 1: the modulus may be 1
    for (ll x : v) r = mulMod(r, norm(x, m), m);
    return r;
}

// Sum of an array mod m.  A bare sum of 1e18-sized values overflows after nine terms.
// O(n)
ll sumMod(const std::vector<ll> &v, ll m) {
    ll r = 0;
    for (ll x : v) r = addMod(r, norm(x, m), m);
    return r;
}

// A decimal string reduced mod m -- for inputs far too large to parse into any integer
// type.  Horner's rule, reducing after every digit.
// O(len)
ll modOfBigDecimal(const std::string &s, ll m) {
    ll r = 0;
    for (char c : s)
        if (c >= '0' && c <= '9') r = addMod(mulMod(r, 10 % m, m), (c - '0') % m, m);
    return r;
}

// Cancellation, done correctly.  From a*c = b*c (mod m) you may NOT conclude a = b (mod m):
// 2*3 = 2*0 (mod 6) while 3 != 0 (mod 6).  What survives is the same statement modulo
// m / gcd(c, m).  This function returns that reduced modulus.
// O(log m)
ll cancelModulus(ll c, ll m) {
    ll a = c < 0 ? -c : c, b = m;
    while (b) { ll t = a % b; a = b; b = t; }   // a = gcd(|c|, m)
    return m / a;
}

// ---------------------------------------------------------------- demo

int main() {
    const ll M = 1000000007;

    std::cout << "negative remainders\n";
    std::cout << "  -17 % 5        = " << (-17 % 5) << "\t[C++]\n";
    std::cout << "  norm(-17, 5)   = " << norm(-17, 5) << "\t[the residue]\n";
    std::cout << "  subMod(3,7,10) = " << subMod(3, 7, 10) << "\t[a < b, still in range]\n";

    std::cout << "\noverflow\n";
    ll big = 3037000500LL;                       // just above sqrt(2^63)
    std::cout << "  big*big % M, naive = " << (big * big % M) << "\t[wrapped]\n";
    std::cout << "  mulMod(big,big,M)  = " << mulMod(big, big, M) << "\t[correct]\n";

    std::cout << "\nreduce at every step\n";
    std::cout << "  product{123456789, 987654321, 555555555} mod M = "
              << productMod({123456789, 987654321, 555555555}, M) << "\n";
    std::cout << "  a 60-digit number mod 97 = "
              << modOfBigDecimal("123456789012345678901234567890"
                                 "123456789012345678901234567890", 97)
              << "\t[never parsed as an integer]\n";

    std::cout << "\ncancellation is not free\n";
    std::cout << "  2*3 = 2*0 (mod 6), yet 3 != 0 (mod 6)\n";
    std::cout << "  cancelling 2 is only legal mod " << cancelModulus(2, 6)
              << "\t[m / gcd(c,m)]\n";

    // self-check: the congruence rules against direct computation on small values
    bool ok = true;
    for (ll m = 1; m <= 40; ++m)
        for (ll a = -60; a <= 60; ++a) {
            ll na = norm(a, m);
            if (na < 0 || na >= m || (a - na) % m != 0) ok = false;
            for (ll b = -60; b <= 60; ++b) {
                ll nb = norm(b, m);
                if (addMod(na, nb, m) != norm(a + b, m)) ok = false;
                if (subMod(na, nb, m) != norm(a - b, m)) ok = false;
                if (mulMod(na, nb, m) != norm(a * b, m)) ok = false;
            }
        }

    // self-check: mulMod against __int128 at a modulus where 'a * b % m' cannot work
    bool wide = true;
    const ll HUGE_M = 4000000000000000037LL;
    for (ll a = 1; a <= 2000; ++a) {
        ll x = HUGE_M - a * 1000003, y = HUGE_M - a * 7919;
        if (mulMod(x, y, HUGE_M) != (ll)((__int128)x * y % HUGE_M)) wide = false;
    }

    std::cout << "\nself-check: +,-,* vs direct " << (ok ? "ok" : "FAIL")
              << ", mulMod at m ~ 4e18 " << (wide ? "ok" : "FAIL") << "\n";
}
