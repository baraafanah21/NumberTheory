// Arithmetic and Geometric Progressions
// C++17.  Compile: g++ -std=c++17 -O2 -o progressions implementation.cpp
//
// The five functions from this topic that actually appear in code.

#include <iostream>

using ll = long long;

// ---------------------------------------------------------------------------
// 1. Arithmetic progression
// ---------------------------------------------------------------------------

// The n-th term of a, a+d, a+2d, ...      a_n = a + (n-1)d
// O(1)
ll apTerm(ll a, ll d, ll n) {
    return a + (n - 1) * d;
}

// Sum of the first n terms:  S = n*(2a + (n-1)d) / 2
//
// The division must stay exact. Exactly one of n and (2a + (n-1)d) is even
// (proofs.md section 2), so halve THAT one before multiplying.
//   n/2 * bracket        -> wrong when n is odd
//   n * bracket / 2      -> right, but can overflow first
// O(1)
ll apSum(ll a, ll d, ll n) {
    if (n <= 0) return 0;
    ll bracket = 2 * a + (n - 1) * d;
    return (n % 2 == 0) ? (n / 2) * bracket : n * (bracket / 2);
}

// Sum of every multiple of d in [1, n].
// The multiples are d, 2d, ..., kd with k = n/d, so the sum is d * k(k+1)/2.
// O(1)
ll sumOfMultiples(ll d, ll n) {
    if (n < d) return 0;
    ll k = n / d;
    return d * apSum(1, 1, k);        // apSum(1,1,k) is k(k+1)/2
}

// ---------------------------------------------------------------------------
// 2. Geometric progression
// ---------------------------------------------------------------------------

// base^exp for integers, by repeated squaring.
// Never use std::pow: it returns a double and is wrong past 2^53.
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

// Sum of the first n terms of a, ar, ar^2, ...
//
// r == 1 is a REAL case, not a corner case: the closed form divides by r-1 = 0.
// When r != 1 the division is exact, because (r-1) always divides (r^n - 1).
// Precondition: r^n fits in 64 bits.
// O(log n)
ll gpSum(ll a, ll r, ll n) {
    if (n <= 0) return 0;
    if (r == 1) return n * a;
    return a * ((ipow(r, n) - 1) / (r - 1));
}

// ---------------------------------------------------------------------------
// 3. Working modulo m
// ---------------------------------------------------------------------------

// (a * b) % mod, exact for any mod below 2^63.
// __int128 is a GCC/Clang extension; it holds the product before reducing.
ll mulMod(ll a, ll b, ll mod) {
    return (ll)((__int128)a * b % mod);
}

// base^exp % mod, by repeated squaring.   O(log exp)
ll powerMod(ll base, ll exp, ll mod) {
    ll result = 1 % mod;
    base %= mod;
    if (base < 0) base += mod;
    while (exp > 0) {
        if (exp & 1) result = mulMod(result, base, mod);
        base = mulMod(base, base, mod);
        exp >>= 1;
    }
    return result;
}

// (1 + r + r^2 + ... + r^(n-1)) % mod, using only + and *.
//
// The closed form would need to divide by r-1, i.e. multiply by (r-1)^-1 -- and that
// inverse only exists when gcd(r-1, mod) == 1. Splitting the sum avoids division
// entirely, so this is correct for EVERY modulus:
//     G(2k) = G(k) * (1 + r^k)          both halves are the same, one scaled by r^k
//     G(n)  = G(n-1) + r^(n-1)          n odd: peel off the last term
// O(log^2 n)
ll geometricSumMod(ll r, ll n, ll mod) {
    if (n <= 0) return 0;
    if (n % 2 == 1)
        return (geometricSumMod(r, n - 1, mod) + powerMod(r, n - 1, mod)) % mod;
    ll half = geometricSumMod(r, n / 2, mod);
    return mulMod(half, (1 + powerMod(r, n / 2, mod)) % mod, mod);
}

// ---------------------------------------------------------------------------
// Demo
// ---------------------------------------------------------------------------

int main() {
    std::cout << "--- arithmetic ---\n";
    std::cout << "10th term of 5, 8, 11, ... = " << apTerm(5, 3, 10) << "   [32]\n";
    std::cout << "sum of its first 10 terms  = " << apSum(5, 3, 10) << "   [185]\n";
    std::cout << "1 + 2 + ... + 1000000000   = " << apSum(1, 1, 1000000000LL)
              << "   [exact, no overflow]\n";
    std::cout << "sum of multiples of 7 up to 100 = " << sumOfMultiples(7, 100) << "   [735]\n";
    std::cout << "sum of multiples of 3 or 5 below 1000 = "
              << sumOfMultiples(3, 999) + sumOfMultiples(5, 999) - sumOfMultiples(15, 999)
              << "   [233168, by inclusion-exclusion]\n";

    std::cout << "\n--- geometric ---\n";
    std::cout << "sum of 3, 6, 12, 24, 48 = " << gpSum(3, 2, 5) << "   [93]\n";
    std::cout << "sum of 4, 4, 4, 4       = " << gpSum(4, 1, 4) << "   [16, the r == 1 branch]\n";
    std::cout << "1 + 2 + 4 + ... + 2^19  = " << gpSum(1, 2, 20) << "   [2^20 - 1]\n";

    std::cout << "\n--- modulo m ---\n";
    const ll MOD = 1000000007LL;
    std::cout << "1 + 3 + 9 + 27 + 81 mod 1e9+7 = " << geometricSumMod(3, 5, MOD) << "   [121]\n";
    std::cout << "sum of 2^i for i < 10^18, mod 1e9+7 = "
              << geometricSumMod(2, 1000000000000000000LL, MOD) << "\n";
    std::cout << "1 + 4 + 16 + 64 mod 9 = " << geometricSumMod(4, 4, 9)
              << "   [4 -- and note gcd(r-1, 9) = 3, so the closed form is unusable here]\n";

    // Self-check: the fast method must agree with a plain loop.
    bool ok = true;
    for (ll r = 2; r <= 6; ++r)
        for (ll n = 0; n <= 40; ++n)
            for (ll m : {7, 9, 12, 1000000007}) {
                ll slow = 0, p = 1 % m;
                for (ll i = 0; i < n; ++i) { slow = (slow + p) % m; p = p * r % m; }
                if (slow != geometricSumMod(r, n, m)) ok = false;
            }
    std::cout << "\ngeometricSumMod matches a plain loop (incl. composite moduli): "
              << (ok ? "yes" : "NO") << "\n";
    return 0;
}
