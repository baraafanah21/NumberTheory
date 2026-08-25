// Arithmetic and Geometric Progressions -- CP templates
// C++17.  Compile: g++ -std=c++17 -O2 -o progressions implementation.cpp

#include <iostream>
#include <vector>

using ll = long long;
using lll = __int128;      // GCC/Clang: exact 63x63 products before reducing/comparing

// ===========================================================================
// Helpers
// ===========================================================================

ll floorDiv(ll x, ll y) {
    ll q = x / y;
    if (x % y != 0 && ((x < 0) != (y < 0))) --q;
    return q;
}

// % takes the sign of the dividend: -3 % 5 == -3. Normalize before using as an index.
ll norm(ll x, ll mod) { x %= mod; return x < 0 ? x + mod : x; }

// Exact for any modulus below 2^63.
ll mulMod(ll a, ll b, ll mod) { return (ll)((lll)a * b % mod); }

// ===========================================================================
// Arithmetic progressions
// ===========================================================================

ll apTerm(ll a, ll d, ll n) { return a + (n - 1) * d; }

// S = n*(2a + (n-1)d)/2, kept exact.
// Exactly one factor is even (2a is even, so the bracket has the parity of (n-1)d), so
// halve THAT one. Exact, and one bit safer against overflow than multiplying first.
// O(1)
ll apSum(ll a, ll d, ll n) {
    if (n <= 0) return 0;
    ll bracket = 2 * a + (n - 1) * d;
    return (n % 2 == 0) ? (n / 2) * bracket : n * (bracket / 2);
}

// Terms from `first` stepping by d without passing `last`. The count of STEPS plus one.
// Returns 0 when last is behind first. Handles descending progressions (d < 0).
// Precondition: d != 0.   O(1)
ll apCount(ll first, ll last, ll d) {
    ll steps = floorDiv(last - first, d);
    return steps < 0 ? 0 : steps + 1;
}

// Sum of the multiples of d in [1, n] / in [L, R].   O(1)
ll sumOfMultiplesUpTo(ll d, ll n) {
    if (n < d) return 0;
    return d * apSum(1, 1, n / d);          // apSum(1,1,k) == k(k+1)/2
}
ll sumOfMultiplesInRange(ll d, ll L, ll R) {
    return sumOfMultiplesUpTo(d, R) - sumOfMultiplesUpTo(d, L - 1);
}

// Constant differences. Subtraction only -- no division, no zero case.
// Length <= 2 is trivially an AP.   O(n)
bool isArithmetic(const std::vector<ll> &v) {
    if (v.size() <= 2) return true;
    ll d = v[1] - v[0];
    for (std::size_t i = 2; i < v.size(); ++i)
        if (v[i] - v[i - 1] != d) return false;
    return true;
}

// ===========================================================================
// Geometric progressions
// ===========================================================================

// Integer power. Never use std::pow: it returns a double and is wrong past 2^53.
// O(log exp)
ll ipow(ll base, ll exp) {
    ll result = 1;
    while (exp > 0) {
        if (exp & 1) result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

ll gpTerm(ll a, ll r, ll n) { return a * ipow(r, n - 1); }

// r == 1 is a REAL case, not a corner case: the closed form divides by zero.
// For r != 1 the division is exact because (r-1) | (r^n - 1).
// Precondition: r^n fits in 64 bits.   O(log n)
ll gpSum(ll a, ll r, ll n) {
    if (n <= 0) return 0;
    if (r == 1) return n * a;
    return a * ((ipow(r, n) - 1) / (r - 1));
}

// base^exp mod m.   O(log exp)
ll powerMod(ll base, ll exp, ll mod) {
    ll result = 1 % mod;
    base = norm(base, mod);
    while (exp > 0) {
        if (exp & 1) result = mulMod(result, base, mod);
        base = mulMod(base, base, mod);
        exp >>= 1;
    }
    return result;
}

// 1 + r + ... + r^(n-1)  (mod m), using only + and *.
//
// The closed form needs (r-1)^-1 mod m, which DOES NOT EXIST unless gcd(r-1,m) = 1.
// This splits instead:  G(2k) = G(k)*(1 + r^k),  G(n) = G(n-1) + r^(n-1) for odd n.
// Valid for every modulus, including composite ones.
// O(log^2 n) time, O(log n) stack.
ll geometricSumMod(ll r, ll n, ll mod) {
    if (n <= 0) return 0;
    if (n % 2 == 1)
        return (geometricSumMod(r, n - 1, mod) + powerMod(r, n - 1, mod)) % mod;
    ll half = geometricSumMod(r, n / 2, mod);
    return mulMod(half, (1 + powerMod(r, n / 2, mod)) % mod, mod);
}

// Constant ratios, tested as v[i]^2 == v[i-1]*v[i+1] -- cross-multiplied, so no division
// and no floating point. Products go through __int128 because two 64-bit terms overflow.
// Zero terms are rejected first: 1, 0, 5 satisfies the equation but is not a GP.
// O(n)
bool isGeometric(const std::vector<ll> &v) {
    for (ll x : v) if (x == 0) return false;
    if (v.size() <= 2) return true;
    for (std::size_t i = 1; i + 1 < v.size(); ++i)
        if ((lll)v[i] * v[i] != (lll)v[i - 1] * v[i + 1]) return false;
    return true;
}

// ===========================================================================
// Number-theoretic consequences
// ===========================================================================

// sigma(p^k) = 1 + p + ... + p^k: the divisors of a prime power form a GP.
ll sigmaPrimePower(ll p, ll k) { return gpSum(1, p, k + 1); }

// If n is composite, returns the nontrivial factor 2^d - 1 of 2^n - 1, without ever
// forming 2^n - 1. Returns 0 when n is prime (the test says nothing then:
// 2^11 - 1 = 23 * 89).
// O(sqrt n)
ll mersenneFactorFromExponent(ll n) {
    if (n <= 1) return 0;
    for (ll d = 2; d * d <= n; ++d)
        if (n % d == 0) return ipow(2, d) - 1;
    return 0;
}

// ===========================================================================
// Demo / self-check
// ===========================================================================

void demoAp() {
    std::cout << "--- AP ---\n";
    std::cout << "a_10 of (5,8,11,...) = " << apTerm(5, 3, 10)
              << "   S_10 = " << apSum(5, 3, 10) << "   [32, 185]\n";
    std::cout << "terms in 7,11,...,99 = " << apCount(7, 99, 4)
              << "   descending 20,17,...,2 = " << apCount(20, 2, -3) << "   [24, 7]\n";
    std::cout << "1+2+...+1e9 = " << apSum(1, 1, 1000000000LL)
              << "   [exact: 500000000500000000]\n";
    std::cout << "sum multiples of 7 up to 100 = " << sumOfMultiplesUpTo(7, 100)
              << "   in [10,50] of 3 = " << sumOfMultiplesInRange(3, 10, 50)
              << "   [735, 390]\n";
    std::cout << "3-or-5 below 1000 = "
              << sumOfMultiplesUpTo(3, 999) + sumOfMultiplesUpTo(5, 999)
                 - sumOfMultiplesUpTo(15, 999) << "   [233168]\n";
    std::cout << "isArithmetic{2,5,8,11}=" << isArithmetic({2, 5, 8, 11})
              << " {2,5,8,12}=" << isArithmetic({2, 5, 8, 12}) << "   [1 0]\n\n";
}

void demoGp() {
    std::cout << "--- GP ---\n";
    std::cout << "a_5 of (3,6,12,...) = " << gpTerm(3, 2, 5)
              << "   S_5 = " << gpSum(3, 2, 5) << "   [48, 93]\n";
    std::cout << "S_4 of (4,4,4,4) = " << gpSum(4, 1, 4) << " (r==1 branch)"
              << "   S_4 of (1,-2,4,-8) = " << gpSum(1, -2, 4) << "   [16, -5]\n";
    std::cout << "1+2+4+...+2^19 = " << gpSum(1, 2, 20) << " = 2^20-1\n";
    std::cout << "isGeometric{3,6,12,24}=" << isGeometric({3, 6, 12, 24})
              << " {1,0,5}=" << isGeometric({1, 0, 5}) << " (zero term)   [1 0]\n\n";
}

void demoMod() {
    std::cout << "--- geometric sums mod m ---\n";
    const ll MOD = 1000000007LL;
    std::cout << "1+3+9+27+81 mod 1e9+7 = " << geometricSumMod(3, 5, MOD) << "   [121]\n";
    std::cout << "sum of 2^i for i < 1e18, mod 1e9+7 = "
              << geometricSumMod(2, 1000000000000000000LL, MOD) << "\n";
    // The point: r-1 = 3 is NOT invertible mod 9, so the closed form is unusable here.
    std::cout << "1+4+16+64 mod 9 = " << geometricSumMod(4, 4, 9)
              << "   [4; gcd(r-1,9)=3, so no modular inverse exists]\n\n";
}

void demoNumberTheory() {
    std::cout << "--- consequences ---\n";
    std::cout << "sigma(2^3)=" << sigmaPrimePower(2, 3)
              << "  sigma(3^2)=" << sigmaPrimePower(3, 2) << "   [15, 13]\n";
    for (ll n : {6, 11, 15}) {
        ll f = mersenneFactorFromExponent(n);
        std::cout << "2^" << n << "-1: ";
        if (f) std::cout << "composite, divisible by " << f << "\n";
        else   std::cout << "exponent prime -- inconclusive (2^11-1 = 23*89)\n";
    }
}

int main() {
    demoAp();
    demoGp();
    demoMod();
    demoNumberTheory();
    return 0;
}
