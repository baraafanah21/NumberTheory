// Progressions -- g++ -std=c++17 -O2 implementation.cpp

#include <iostream>

using ll = long long;

ll norm(ll x, ll m) { x %= m; return x < 0 ? x + m : x; }
ll mulMod(ll a, ll b, ll m) { return (ll)((__int128)a * b % m); }

// ---------------------------------------------------------------- arithmetic

// n-th term.  O(1)
ll apTerm(ll a, ll d, ll n) { return a + (n - 1) * d; }

// Sum of the first n terms, exact in integers.
// Exactly one of n and (2a + (n-1)d) is even -- halve that one, so the division is
// exact and one factor shrinks before the multiply.
// O(1)
ll apSum(ll a, ll d, ll n) {
    if (n <= 0) return 0;
    ll bracket = 2 * a + (n - 1) * d;
    return (n % 2 == 0) ? (n / 2) * bracket : n * (bracket / 2);
}

// Sum of every multiple of d in [1, n].  They are d, 2d, ..., kd with k = n/d.  O(1)
ll sumOfMultiples(ll d, ll n) {
    return n < d ? 0 : d * apSum(1, 1, n / d);
}

// ---------------------------------------------------------------- geometric

// base^exp over the integers.  std::pow returns a double and is wrong past 2^53.
// O(log exp)
ll ipow(ll base, ll exp) {
    ll r = 1;
    for (; exp > 0; exp >>= 1, base *= base)
        if (exp & 1) r *= base;
    return r;
}

// Sum of the first n terms.  r == 1 is a real case: the closed form divides by r-1 = 0.
// For r != 1 the division is exact, since (r-1) always divides (r^n - 1).
// O(log n)
ll gpSum(ll a, ll r, ll n) {
    if (n <= 0) return 0;
    if (r == 1) return n * a;
    return a * ((ipow(r, n) - 1) / (r - 1));
}

// ---------------------------------------------------------------- modular

// base^exp mod m.  O(log exp)
ll powMod(ll base, ll exp, ll m) {
    ll r = 1 % m;
    base = norm(base, m);
    for (; exp > 0; exp >>= 1, base = mulMod(base, base, m))
        if (exp & 1) r = mulMod(r, base, m);
    return r;
}

// (1 + r + ... + r^(n-1)) mod m, using only + and *.
//
// The closed form needs (r-1)^-1, which exists only when gcd(r-1, m) = 1.  Splitting
// avoids division entirely, so this is correct for EVERY modulus:
//     G(2k) = G(k) * (1 + r^k)      the two halves differ by a factor of r^k
//     G(n)  = G(n-1) + r^(n-1)      n odd: peel the last term
// O(log^2 n)
ll geoSumMod(ll r, ll n, ll m) {
    if (n <= 0) return 0;
    if (n & 1) return (geoSumMod(r, n - 1, m) + powMod(r, n - 1, m)) % m;
    ll half = geoSumMod(r, n / 2, m);
    return mulMod(half, (1 + powMod(r, n / 2, m)) % m, m);
}

// ---------------------------------------------------------------- demo

int main() {
    std::cout << "arithmetic\n";
    std::cout << "  a_10 of 5,8,11,...  = " << apTerm(5, 3, 10) << "\t[32]\n";
    std::cout << "  S_10                = " << apSum(5, 3, 10) << "\t[185]\n";
    std::cout << "  1+2+...+1e9         = " << apSum(1, 1, 1000000000LL) << "\t[exact]\n";
    std::cout << "  multiples of 7 <=100= " << sumOfMultiples(7, 100) << "\t[735]\n";
    std::cout << "  3 or 5 below 1000   = "
              << sumOfMultiples(3, 999) + sumOfMultiples(5, 999) - sumOfMultiples(15, 999)
              << "\t[233168, inclusion-exclusion]\n";

    std::cout << "\ngeometric\n";
    std::cout << "  3+6+12+24+48        = " << gpSum(3, 2, 5) << "\t[93]\n";
    std::cout << "  4+4+4+4             = " << gpSum(4, 1, 4) << "\t[16, the r==1 branch]\n";
    std::cout << "  1+2+...+2^19        = " << gpSum(1, 2, 20) << "\t[2^20 - 1]\n";

    std::cout << "\nmodular\n";
    const ll MOD = 1000000007LL;
    std::cout << "  1+3+9+27+81 mod 1e9+7 = " << geoSumMod(3, 5, MOD) << "\t[121]\n";
    std::cout << "  sum 2^i, i<1e18       = " << geoSumMod(2, 1000000000000000000LL, MOD) << "\n";
    std::cout << "  1+4+16+64 mod 9       = " << geoSumMod(4, 4, 9)
              << "\t[4; gcd(r-1,9)=3, so the closed form is unusable]\n";

    bool ok = true;
    for (ll r = 2; r <= 6; ++r)
        for (ll n = 0; n <= 40; ++n)
            for (ll m : {7, 9, 12, 1000000007}) {
                ll slow = 0, p = 1 % m;
                for (ll i = 0; i < n; ++i) { slow = (slow + p) % m; p = p * r % m; }
                if (slow != geoSumMod(r, n, m)) ok = false;
            }
    std::cout << "\nself-check: geoSumMod vs plain loop (composite moduli included) "
              << (ok ? "ok" : "FAIL") << "\n";
}
