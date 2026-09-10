// Sieve of Eratosthenes -- g++ -std=c++17 -O2 implementation.cpp

#include <algorithm>
#include <iostream>
#include <vector>

using ll = long long;

// ---------------------------------------------------------------- plain sieve

// isPrime[m] for every m in [0, n].
//
// Both speedups follow from one fact: a composite is always caught by its SMALLEST prime
// factor.  So the inner loop may start at p*p (any smaller multiple k*p, k < p, has a
// smaller prime factor and is already gone), and the outer loop may stop at sqrt(n).
//
// vector<char> not vector<bool>: the latter is a packed bit-array -- less memory, more
// time.  Swap if memory is the binding constraint.
// O(n log log n) time, O(n) space.
std::vector<char> sieve(int n) {
    std::vector<char> isPrime(n + 1, 1);
    if (n >= 0) isPrime[0] = 0;
    if (n >= 1) isPrime[1] = 0;
    for (int p = 2; (ll)p * p <= n; ++p)
        if (isPrime[p])
            for (ll m = (ll)p * p; m <= n; m += p)
                isPrime[m] = 0;
    return isPrime;
}

// ---------------------------------------------------------------- smallest prime factor

// spf[m] = the smallest prime dividing m, for every m in [2, n].  The linear sieve.
//
// The `p > spf[i]` break is the whole trick: it forces every composite to be produced
// only as (smallest prime factor) x (the rest), so each is written exactly once.
// The sorted prime list comes out as a by-product.
// O(n) time, O(n) space.
std::vector<int> spfSieve(int n, std::vector<int> &primes) {
    std::vector<int> spf(n + 1, 0);
    primes.clear();
    for (int i = 2; i <= n; ++i) {
        if (spf[i] == 0) { spf[i] = i; primes.push_back(i); }
        for (int p : primes) {
            if (p > spf[i] || (ll)i * p > n) break;
            spf[i * p] = p;
        }
    }
    return spf;
}

// Prime factorization as (prime, exponent) pairs, increasing.
// Each division at least halves m, so at most log2(m) steps -- about 23 at m = 10^7,
// versus ~3000 for trial division.
// Precondition: 1 <= m <= n, spf built for n.  O(log m)
std::vector<std::pair<int, int>> factorize(int m, const std::vector<int> &spf) {
    std::vector<std::pair<int, int>> f;
    while (m > 1) {
        int p = spf[m], e = 0;
        while (m % p == 0) { m /= p; ++e; }
        f.push_back({p, e});
    }
    return f;
}

// tau straight from the factorization: product of (e_i + 1).
ll countDivisors(int m, const std::vector<int> &spf) {
    ll t = 1;
    for (auto [p, e] : factorize(m, spf)) { (void)p; t *= e + 1; }
    return t;
}

// ---------------------------------------------------------------- segmented

// Primes in [lo, hi], where hi may reach about 10^12.
//
// Any composite in the window has a prime factor <= sqrt(hi), so sieving the small
// primes is enough to clear it.  Memory is the WINDOW plus sqrt(hi), not hi.
// O((hi - lo) log log hi + sqrt hi)
std::vector<ll> primesInRange(ll lo, ll hi) {
    if (hi < 2 || hi < lo) return {};
    lo = std::max(lo, 2LL);

    int limit = 1;
    while ((ll)(limit + 1) * (limit + 1) <= hi) ++limit;      // floor(sqrt(hi))
    std::vector<char> small = sieve(limit);

    std::vector<char> window(hi - lo + 1, 1);
    for (int p = 2; p <= limit; ++p) {
        if (!small[p]) continue;
        ll start = std::max((ll)p * p, ((lo + p - 1) / p) * (ll)p);
        for (ll m = start; m <= hi; m += p) window[m - lo] = 0;
    }

    std::vector<ll> out;
    for (ll m = lo; m <= hi; ++m)
        if (window[m - lo]) out.push_back(m);
    return out;
}

// ---------------------------------------------------------------- demo

int main() {
    std::cout << "plain sieve\n  primes up to 50: ";
    std::vector<char> small = sieve(50);
    for (int i = 2; i <= 50; ++i) if (small[i]) std::cout << i << " ";

    const int N = 10000000;
    std::vector<char> big = sieve(N);
    ll count = 0;
    for (int i = 2; i <= N; ++i) count += big[i];
    std::cout << "\n  primes below 10^7: " << count
              << "\t[n/ln n estimates 620421 -- it always undercounts]\n";

    std::vector<int> primes;
    std::vector<int> spf = spfSieve(1000000, primes);
    std::cout << "\nsmallest prime factor\n  linear sieve found " << primes.size()
              << " primes below 10^6\n";
    std::cout << "  spf[84] = " << spf[84] << ", spf[97] = " << spf[97]
              << "\t[97 is prime, so spf is itself]\n";

    std::cout << "\nfactorization in O(log m)\n";
    for (int m : {84, 1000000, 999983}) {
        std::cout << "  " << m << " = ";
        auto f = factorize(m, spf);
        for (std::size_t i = 0; i < f.size(); ++i) {
            std::cout << f[i].first;
            if (f[i].second > 1) std::cout << "^" << f[i].second;
            if (i + 1 < f.size()) std::cout << " x ";
        }
        std::cout << "\t(tau = " << countDivisors(m, spf) << ")\n";
    }

    std::cout << "\nsegmented sieve\n  primes in [10^12, 10^12 + 100]: ";
    for (ll p : primesInRange(1000000000000LL, 1000000000100LL)) std::cout << p << " ";
    std::cout << "\n  (a direct sieve would need a 10^12-entry array)\n";

    bool trialOk = true, spfOk = true, factorOk = true, linearOk = true, segOk = true;
    for (int m = 0; m <= 5000; ++m) {
        bool want = m >= 2;
        for (int d = 2; d <= m / d && want; ++d) if (m % d == 0) want = false;
        if ((bool)big[m] != want) trialOk = false;
    }
    for (int m = 2; m <= 200000; ++m) {
        int want = m;
        for (int d = 2; d <= m / d; ++d) if (m % d == 0) { want = d; break; }
        if (spf[m] != want) spfOk = false;
    }
    for (int m = 1; m <= 200000; ++m) {
        ll product = 1;
        for (auto [p, e] : factorize(m, spf))
            for (int k = 0; k < e; ++k) product *= p;
        if (product != m) factorOk = false;
    }
    for (int m = 2; m <= 1000000; ++m)
        if ((spf[m] == m) != (bool)big[m]) linearOk = false;
    {
        std::vector<ll> want;
        for (int m = 4000; m <= 5000; ++m) if (big[m]) want.push_back(m);
        if (primesInRange(4000, 5000) != want) segOk = false;
    }
    std::cout << "\nself-check: sieve vs trial division " << (trialOk ? "ok" : "FAIL")
              << ", spf vs brute force " << (spfOk ? "ok" : "FAIL")
              << ", factorizations multiply back " << (factorOk ? "ok" : "FAIL")
              << ", linear == plain " << (linearOk ? "ok" : "FAIL")
              << ", segmented == plain " << (segOk ? "ok" : "FAIL") << "\n";
}
