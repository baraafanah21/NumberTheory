// Arithmetic and Geometric Progressions
// C++17
//
// Every routine here implements a result stated in README.md and proved in proofs.md.
// Cross-references are given as "Theorem N" (proofs.md) and "AP-k / GP-k" (README.md).
//
// Compile: g++ -std=c++17 -O2 -o progressions implementation.cpp

#include <iostream>
#include <vector>

using ll = long long;

// __int128 is a GCC/Clang extension. It is used only in two places where a product of
// two 63-bit values must be formed exactly before being reduced or compared.
using lll = __int128;

// ---------------------------------------------------------------------------
// Small helpers
// ---------------------------------------------------------------------------

// Floor division. C++ truncates toward zero, so -7 / 2 == -3, but we want -4.
// Time O(1), Space O(1).
ll floorDiv(ll x, ll y) {
    ll q = x / y;
    if (x % y != 0 && ((x < 0) != (y < 0))) --q;
    return q;
}

// Least nonnegative residue. C++ '%' takes the sign of the dividend: -3 % 5 == -3.
// Time O(1), Space O(1).
ll norm(ll x, ll mod) {
    x %= mod;
    if (x < 0) x += mod;
    return x;
}

// (a * b) mod m, exact for any modulus below 2^63.
// Time O(1), Space O(1).
ll mulMod(ll a, ll b, ll mod) {
    return static_cast<ll>(static_cast<lll>(a) * b % mod);
}

// ---------------------------------------------------------------------------
// Arithmetic progressions
// ---------------------------------------------------------------------------

// n-th term of the AP a, a+d, a+2d, ...   (Theorem 1)
// Precondition: n >= 1.
// Time O(1), Space O(1).
ll apTerm(ll a, ll d, ll n) {
    return a + (n - 1) * d;
}

// Sum of the first n terms: S = n * (2a + (n-1)d) / 2.   (Theorem 2)
//
// The division must stay exact in integer arithmetic. By Corollary 2.2 exactly one of
// the two factors is even, so we halve *that* one before multiplying. This is both exact
// and one bit safer against overflow than multiplying first.
//
// Precondition: n >= 0, and the true sum fits in a signed 64-bit integer.
// Time O(1), Space O(1).
ll apSum(ll a, ll d, ll n) {
    if (n <= 0) return 0;
    ll bracket = 2 * a + (n - 1) * d;   // even whenever n is odd
    if (n % 2 == 0) return (n / 2) * bracket;
    return n * (bracket / 2);
}

// Number of terms of the AP starting at `first` with step `d` that do not pass `last`
// (in the direction d points). Returns 0 when `last` lies behind `first`.
//
// This is the "+1" formula: the count of terms is the count of steps plus one.
// Precondition: d != 0.
// Time O(1), Space O(1).
ll apCount(ll first, ll last, ll d) {
    ll steps = floorDiv(last - first, d);
    return steps < 0 ? 0 : steps + 1;
}

// Sum of all multiples of d in [1, n].   (Theorem 8 / AP-6)
// The multiples are d, 2d, ..., kd with k = floor(n/d), so the sum is d * k(k+1)/2.
// Precondition: d >= 1, n >= 0.
// Time O(1), Space O(1).
ll sumOfMultiplesUpTo(ll d, ll n) {
    if (n < d) return 0;
    ll k = n / d;
    return d * apSum(1, 1, k);          // apSum(1,1,k) == k(k+1)/2
}

// Sum of all multiples of d in [L, R].   (Corollary 8.1)
// Precondition: d >= 1, 1 <= L <= R.
// Time O(1), Space O(1).
ll sumOfMultiplesInRange(ll d, ll L, ll R) {
    return sumOfMultiplesUpTo(d, R) - sumOfMultiplesUpTo(d, L - 1);
}

// Is the sequence an AP?   (Theorem 4 / AP-1)
// Uses subtraction only: no division, no floating point, no zero-divisor case.
// Sequences of length <= 2 are APs by definition.
// Time O(n), Space O(1).
bool isArithmetic(const std::vector<ll> &v) {
    if (v.size() <= 2) return true;
    ll d = v[1] - v[0];
    for (std::size_t i = 2; i < v.size(); ++i) {
        if (v[i] - v[i - 1] != d) return false;
    }
    return true;
}

// ---------------------------------------------------------------------------
// Geometric progressions
// ---------------------------------------------------------------------------

// base^exp over the integers, by binary exponentiation.
// Precondition: exp >= 0 and the result fits in a signed 64-bit integer.
// Never use std::pow here: it returns a double and rounds silently past 2^53.
// Time O(log exp), Space O(1).
ll ipow(ll base, ll exp) {
    ll result = 1;
    while (exp > 0) {
        if (exp & 1) result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

// n-th term of the GP a, ar, ar^2, ...   (Theorem 10)
// Precondition: n >= 1, result fits in 64 bits.
// Time O(log n), Space O(1).
ll gpTerm(ll a, ll r, ll n) {
    return a * ipow(r, n - 1);
}

// Sum of the first n terms of a GP.   (Theorem 11)
//
//   r == 1 : every term equals a, so the sum is n*a. The closed form would divide by 0,
//            so this case must be handled separately -- it is not a rare corner case.
//   r != 1 : a * (r^n - 1) / (r - 1). The division is exact over the integers because
//            (r - 1) | (r^n - 1) by Theorem 12.
//
// Precondition: n >= 0, r != 0, and r^n fits in 64 bits (so n must be small).
// Time O(log n), Space O(1).
ll gpSum(ll a, ll r, ll n) {
    if (n <= 0) return 0;
    if (r == 1) return n * a;
    return a * ((ipow(r, n) - 1) / (r - 1));
}

// base^exp mod m, by binary exponentiation.
// Precondition: exp >= 0, mod >= 1.
// Time O(log exp), Space O(1).
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

// G(n) = 1 + r + r^2 + ... + r^(n-1)  (mod m), using only + and *.   (Theorem 20)
//
// We cannot compute (r^n - 1) / (r - 1) modulo m in general, because r - 1 need not be
// invertible mod m. Instead we split the sum:
//
//     G(2k) = G(k) * (1 + r^k)      the second half is the first half scaled by r^k
//     G(n)  = G(n-1) + r^(n-1)      n odd: peel off the last term
//
// Precondition: n >= 0, mod >= 1.
// Time O(log^2 n) multiplications, Space O(log n) recursion stack.
ll geometricSumMod(ll r, ll n, ll mod) {
    if (n <= 0) return 0;
    if (n % 2 == 1) {
        return (geometricSumMod(r, n - 1, mod) + powerMod(r, n - 1, mod)) % mod;
    }
    ll half = geometricSumMod(r, n / 2, mod);
    ll factor = (1 + powerMod(r, n / 2, mod)) % mod;
    return mulMod(half, factor, mod);
}

// Is the sequence a GP?   (Theorem 15 / GP-1)
//
// Tests v[i]^2 == v[i-1]*v[i+1] by cross-multiplication -- exact integers, no division,
// no floating-point ratios. Products are formed in __int128 because two 64-bit terms can
// overflow. Zero terms are rejected first: 1, 0, 5 satisfies the equation but is no GP,
// which is why Theorem 15 requires nonzero terms.
// Time O(n), Space O(1).
bool isGeometric(const std::vector<ll> &v) {
    for (ll x : v) {
        if (x == 0) return false;
    }
    if (v.size() <= 2) return true;
    for (std::size_t i = 1; i + 1 < v.size(); ++i) {
        lll square = static_cast<lll>(v[i]) * v[i];
        lll cross  = static_cast<lll>(v[i - 1]) * v[i + 1];
        if (square != cross) return false;
    }
    return true;
}

// ---------------------------------------------------------------------------
// Number-theoretic consequences of the GP sum
// ---------------------------------------------------------------------------

// sigma(p^k) = 1 + p + ... + p^k, the sum of the divisors of a prime power. (Theorem 14)
// The divisors of p^k are exactly p^0, ..., p^k -- a GP of k+1 terms with ratio p.
// Precondition: p >= 2 prime, k >= 0, result fits in 64 bits.
// Time O(log k), Space O(1).
ll sigmaPrimePower(ll p, ll k) {
    return gpSum(1, p, k + 1);
}

// If n is composite, returns a nontrivial divisor 2^d - 1 of 2^n - 1, proving that
// 2^n - 1 is composite without ever computing 2^n - 1.   (Theorem 13 / GP-6)
// Returns 0 when n is prime or n <= 1, where the test says nothing.
//
// Note the direction: n prime is necessary for 2^n - 1 to be prime, never sufficient
// (2^11 - 1 = 2047 = 23 * 89).
// Time O(sqrt(n)), Space O(1).
ll mersenneFactorFromExponent(ll n) {
    if (n <= 1) return 0;
    for (ll d = 2; d * d <= n; ++d) {
        if (n % d == 0) return ipow(2, d) - 1;   // 1 < d < n, so 1 < 2^d - 1 < 2^n - 1
    }
    return 0;                                     // n is prime
}

// ---------------------------------------------------------------------------
// Demonstration
// ---------------------------------------------------------------------------

void demoArithmetic() {
    std::cout << "--- Arithmetic progressions ---\n";

    // 5, 8, 11, 14, ...  (a = 5, d = 3)
    std::cout << "a_10 of (5, 8, 11, ...) = " << apTerm(5, 3, 10) << "   [expect 32]\n";
    std::cout << "S_10 of (5, 8, 11, ...) = " << apSum(5, 3, 10) << "   [expect 185]\n";

    // The classic off-by-one: 7, 11, 15, ..., 99
    std::cout << "terms in 7, 11, ..., 99  = " << apCount(7, 99, 4) << "   [expect 24]\n";

    // 1 + 2 + ... + n stays exact for large n thanks to halving the even factor.
    std::cout << "1 + 2 + ... + 1000000000 = " << apSum(1, 1, 1000000000LL)
              << "   [expect 500000000500000000]\n";

    // A descending progression: 20, 17, 14, ..., 2
    std::cout << "terms in 20, 17, ..., 2  = " << apCount(20, 2, -3) << "   [expect 7]\n";

    // AP-6: multiples form an AP.
    std::cout << "sum of multiples of 7 up to 100 = " << sumOfMultiplesUpTo(7, 100)
              << "   [expect 735]\n";
    std::cout << "sum of multiples of 3 in [10, 50] = " << sumOfMultiplesInRange(3, 10, 50)
              << "   [expect 390]\n";

    // Inclusion-exclusion over three APs: multiples of 3 or 5 below 1000.
    ll byThree = sumOfMultiplesUpTo(3, 999);
    ll byFive  = sumOfMultiplesUpTo(5, 999);
    ll byBoth  = sumOfMultiplesUpTo(15, 999);
    std::cout << "sum of multiples of 3 or 5 below 1000 = " << byThree + byFive - byBoth
              << "   [expect 233168]\n";

    std::cout << "is (2, 5, 8, 11) an AP? " << (isArithmetic({2, 5, 8, 11}) ? "yes" : "no")
              << "   [expect yes]\n";
    std::cout << "is (2, 5, 8, 12) an AP? " << (isArithmetic({2, 5, 8, 12}) ? "yes" : "no")
              << "   [expect no]\n\n";
}

void demoGeometric() {
    std::cout << "--- Geometric progressions ---\n";

    // 3, 6, 12, 24, 48  (a = 3, r = 2)
    std::cout << "a_5 of (3, 6, 12, ...) = " << gpTerm(3, 2, 5) << "   [expect 48]\n";
    std::cout << "S_5 of (3, 6, 12, ...) = " << gpSum(3, 2, 5) << "   [expect 93]\n";

    // The r == 1 case: the closed form would divide by zero.
    std::cout << "S_4 of (4, 4, 4, 4)    = " << gpSum(4, 1, 4) << "   [expect 16]\n";

    // Alternating GP: 1, -2, 4, -8  -> 1 - 2 + 4 - 8 = -5
    std::cout << "S_4 of (1, -2, 4, -8)  = " << gpSum(1, -2, 4) << "   [expect -5]\n";

    // GP-9: a doubling GP is dominated by its last term.
    std::cout << "1 + 2 + 4 + ... + 2^19 = " << gpSum(1, 2, 20)
              << "   [expect 1048575 = 2^20 - 1]\n";

    std::cout << "is (3, 6, 12, 24) a GP? " << (isGeometric({3, 6, 12, 24}) ? "yes" : "no")
              << "   [expect yes]\n";
    std::cout << "is (1, 0, 5) a GP?      " << (isGeometric({1, 0, 5}) ? "yes" : "no")
              << "   [expect no -- zero term]\n\n";
}

void demoModular() {
    std::cout << "--- Geometric sums modulo m ---\n";

    const ll MOD = 1000000007LL;
    std::cout << "(1 + 3 + 9 + 27 + 81) mod 1e9+7 = " << geometricSumMod(3, 5, MOD)
              << "   [expect 121]\n";

    // A huge exponent: 1 + 2 + ... + 2^(10^18 - 1) mod 1e9+7. No loop could do this.
    std::cout << "sum of 2^i, i < 10^18, mod 1e9+7 = "
              << geometricSumMod(2, 1000000000000000000LL, MOD) << "\n";

    // The point of the division-free method: r - 1 = 3 is NOT invertible modulo 9,
    // so (r^n - 1)/(r - 1) mod 9 cannot be computed with a modular inverse. The
    // recursion still works: 1 + 4 + 16 + 64 = 85, and 85 mod 9 = 4.
    std::cout << "(1 + 4 + 16 + 64) mod 9 = " << geometricSumMod(4, 4, 9)
              << "   [expect 4, and gcd(r-1, 9) = 3 != 1]\n\n";
}

void demoNumberTheory() {
    std::cout << "--- Number-theoretic consequences ---\n";

    // Theorem 14: sum of divisors of a prime power is a GP sum.
    std::cout << "sigma(2^3) = " << sigmaPrimePower(2, 3) << "   [expect 15 = 1+2+4+8]\n";
    std::cout << "sigma(3^2) = " << sigmaPrimePower(3, 2) << "   [expect 13 = 1+3+9]\n";

    // Theorem 13: a composite exponent hands us a factor for free.
    for (ll n : {6, 11, 15}) {
        ll factor = mersenneFactorFromExponent(n);
        std::cout << "2^" << n << " - 1: ";
        if (factor != 0) {
            std::cout << "composite, divisible by " << factor << "\n";
        } else {
            std::cout << "exponent is prime -- test needed (2^11-1 = 23 * 89)\n";
        }
    }
    std::cout << "\n";
}

int main() {
    demoArithmetic();
    demoGeometric();
    demoModular();
    demoNumberTheory();
    return 0;
}
