// Divisors: tau and sigma -- g++ -std=c++17 -O2 implementation.cpp

#include <algorithm>
#include <cmath>
#include <iostream>
#include <utility>
#include <vector>

using ll = long long;

// ---------------------------------------------------------------- one number

// Every divisor d <= sqrt(n) is paired with n/d >= sqrt(n), so one loop to sqrt(n) meets
// every pair.  The d == n/d case is the square root itself and must not be counted twice.
// 'd <= n / d' rather than 'd * d <= n': the product overflows near 3e9.
// O(sqrt n)
ll countDivisors(ll n) {
    if (n <= 0) return 0;
    ll c = 0;
    for (ll d = 1; d <= n / d; ++d)
        if (n % d == 0) c += (d == n / d) ? 1 : 2;
    return c;
}

// Same loop, adding instead of counting.  Returns 64-bit because sigma outgrows int well
// before n = 1e6.
// O(sqrt n)
ll sumDivisors(ll n) {
    if (n <= 0) return 0;
    ll s = 0;
    for (ll d = 1; d <= n / d; ++d)
        if (n % d == 0) {
            s += d;
            if (d != n / d) s += n / d;
        }
    return s;
}

// Every divisor, sorted.  The pair loop emits them out of order, so the sort is not
// optional if the caller indexes the result.
// O(sqrt n + tau log tau)
std::vector<ll> allDivisors(ll n) {
    std::vector<ll> v;
    for (ll d = 1; d <= n / d; ++d)
        if (n % d == 0) {
            v.push_back(d);
            if (d != n / d) v.push_back(n / d);
        }
    std::sort(v.begin(), v.end());
    return v;
}

// ---------------------------------------------------------------- from a factorization

// tau = prod (e_i + 1): a divisor picks each exponent independently from 0..e_i.
// The route to use once n is past 1e12 and the factorization comes from Pollard's rho.
// O(k)
ll tauFromFactors(const std::vector<std::pair<ll, int>> &f) {
    ll c = 1;
    for (auto &pe : f) c *= pe.second + 1;
    return c;
}

// sigma = prod (1 + p + ... + p^e), one geometric sum per prime.  Summed term by term
// rather than as (p^(e+1)-1)/(p-1), so no division and no modular inverse is needed.
// O(sum of exponents)
ll sigmaFromFactors(const std::vector<std::pair<ll, int>> &f) {
    ll s = 1;
    for (auto &pe : f) {
        ll term = 1, power = 1;
        for (int i = 0; i < pe.second; ++i) { power *= pe.first; term += power; }
        s *= term;
    }
    return s;
}

// O(sqrt n)
std::vector<std::pair<ll, int>> factorize(ll n) {
    std::vector<std::pair<ll, int>> f;
    for (ll p = 2; p <= n / p; ++p)
        if (n % p == 0) {
            int e = 0;
            while (n % p == 0) { n /= p; ++e; }
            f.push_back({p, e});
        }
    if (n > 1) f.push_back({n, 1});      // the prime factor above sqrt(n) that is left
    return f;
}

// ---------------------------------------------------------------- whole ranges

// The skeleton worth memorizing: instead of asking each m for its divisors, let each d
// visit its own multiples.  The inner loop runs n/d times and sum(n/d) = n*H_n, so this is
// O(n log n), not quadratic.  Swap the body for sigma, spf, mobius -- anything indexed by
// divisors.
// O(n log n) time, O(n) space
std::vector<int> tauUpTo(int n) {
    std::vector<int> tau(n + 1, 0);
    for (int d = 1; d <= n; ++d)
        for (int m = d; m <= n; m += d)
            ++tau[m];
    return tau;
}

// Same skeleton, accumulating d.  64-bit: sigma over a range of 1e6 already passes 2^31.
// O(n log n)
std::vector<ll> sigmaUpTo(int n) {
    std::vector<ll> sig(n + 1, 0);
    for (ll d = 1; d <= n; ++d)
        for (ll m = d; m <= n; m += d)
            sig[m] += d;
    return sig;
}

// tau(n) is odd exactly when n is a perfect square: divisors pair as d <-> n/d, and only
// d == n/d is unpaired.  The locker / bulb-toggling problems are this fact in disguise.
// O(1)
bool hasOddDivisorCount(ll n) {
    ll r = (ll)std::sqrt((double)n);
    while (r > 0 && r > n / r) --r;              // correct the double
    while ((r + 1) <= n / (r + 1)) ++r;
    return r * r == n;
}

// ---------------------------------------------------------------- demo

int main() {
    std::cout << "one number\n";
    std::cout << "  tau(36)   = " << countDivisors(36) << "\t[2^2 * 3^2 -> 3*3]\n";
    std::cout << "  sigma(36) = " << sumDivisors(36) << "\t[(1+2+4)(1+3+9) = 7*13]\n";
    std::cout << "  sigma(28) = " << sumDivisors(28) << "\t[= 2*28, a perfect number]\n";
    std::cout << "  divisors(36) = ";
    for (ll d : allDivisors(36)) std::cout << d << " ";
    std::cout << "\n";

    std::cout << "\nfrom the factorization\n";
    auto f = factorize(735134400);
    std::cout << "  735134400 = ";
    for (auto &pe : f) std::cout << pe.first << "^" << pe.second << " ";
    std::cout << "\n  tau = " << tauFromFactors(f) << "\t[the record below 1e9]\n";

    std::cout << "\nwhole ranges\n";
    auto tau = tauUpTo(1000000);
    int best = 0, arg = 1;
    for (int i = 1; i <= 1000000; ++i)
        if (tau[i] > best) { best = tau[i]; arg = i; }
    std::cout << "  max tau below 1e6 = " << best << " at " << arg << "\n";
    auto sig = sigmaUpTo(100);
    std::cout << "  sigma(100) = " << sig[100] << "   sigma(12) = " << sig[12] << "\n";

    std::cout << "\nodd divisor count = perfect square\n";
    std::cout << "  ";
    for (ll n = 1; n <= 20; ++n)
        if (hasOddDivisorCount(n)) std::cout << n << " ";
    std::cout << "\t[exactly the squares]\n";

    // self-check: the sqrt(n) routines against the O(n log n) tables
    bool ok = true;
    for (int n = 1; n <= 100000; ++n) {
        if (countDivisors(n) != tau[n]) ok = false;
        if ((countDivisors(n) % 2 == 1) != hasOddDivisorCount(n)) ok = false;
    }
    auto sigAll = sigmaUpTo(100000);
    for (int n = 1; n <= 100000; ++n)
        if (sumDivisors(n) != sigAll[n]) ok = false;

    // self-check: the factorization formulas against the loops
    bool fk = true;
    for (ll n = 1; n <= 5000; ++n) {
        auto g = factorize(n);
        if (tauFromFactors(g) != countDivisors(n)) fk = false;
        if (sigmaFromFactors(g) != sumDivisors(n)) fk = false;
    }

    std::cout << "\nself-check: sqrt loops vs sieve tables " << (ok ? "ok" : "FAIL")
              << ", factorization formulas " << (fk ? "ok" : "FAIL") << "\n";
}
