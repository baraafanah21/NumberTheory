// Sieve of Eratosthenes
// C++17.  Compile: g++ -std=c++17 -O2 -o sieve implementation.cpp
//
// Four routines: the plain sieve, the smallest-prime-factor sieve, factorization
// using it, and a segmented sieve for ranges too large to hold in memory.

#include <algorithm>
#include <iostream>
#include <vector>

using ll = long long;

// ---------------------------------------------------------------------------
// 1. The plain sieve
// ---------------------------------------------------------------------------

// isPrime[m] for every m in [0, n].
//
// Two things make it fast, and both are consequences of one fact: a composite is always
// caught by its SMALLEST prime factor.
//   - inner loop starts at p*p, because any smaller multiple k*p (k < p) has a prime
//     factor below p and was crossed out on an earlier pass
//   - outer loop stops at sqrt(n), because a composite m <= n has a prime factor
//     <= sqrt(m) <= sqrt(n)
//
// vector<char> rather than vector<bool>: the latter is a packed bit-array, which saves
// memory but costs speed. Swap it if memory is the binding constraint.
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

// ---------------------------------------------------------------------------
// 2. Smallest prime factor -- the version worth building
// ---------------------------------------------------------------------------

// spf[m] = the smallest prime dividing m, for every m in [2, n].
//
// This is the linear sieve. The 'p > spf[i]' break is the whole trick: it forces every
// composite to be produced only as (smallest prime factor) x (the rest), so each one is
// written exactly once. See proofs.md section 3.
//
// primes comes out sorted as a free by-product.
// O(n) time, O(n) space.
std::vector<int> smallestPrimeFactorSieve(int n, std::vector<int> &primes) {
    std::vector<int> spf(n + 1, 0);
    primes.clear();
    for (int i = 2; i <= n; ++i) {
        if (spf[i] == 0) {                    // nothing marked i, so i is prime
            spf[i] = i;
            primes.push_back(i);
        }
        for (int p : primes) {
            if (p > spf[i] || (ll)i * p > n) break;
            spf[i * p] = p;
        }
    }
    return spf;
}

// ---------------------------------------------------------------------------
// 3. Factorization using the table
// ---------------------------------------------------------------------------

// The prime factorization of m as (prime, exponent) pairs, in increasing order.
//
// Each division at least halves m, so this runs in at most log2(m) steps -- about 23
// for m up to 10^7, versus ~3000 for trial division.
// Precondition: 1 <= m <= n, with spf built for n.
// O(log m)
std::vector<std::pair<int, int>> factorize(int m, const std::vector<int> &spf) {
    std::vector<std::pair<int, int>> factors;
    while (m > 1) {
        int p = spf[m], e = 0;
        while (m % p == 0) { m /= p; ++e; }
        factors.push_back({p, e});
    }
    return factors;
}

// Number of divisors, straight from the factorization: tau = product of (e_i + 1).
ll countDivisors(int m, const std::vector<int> &spf) {
    ll tau = 1;
    for (auto [p, e] : factorize(m, spf)) { (void)p; tau *= e + 1; }
    return tau;
}

// ---------------------------------------------------------------------------
// 4. Segmented sieve -- for ranges too big to sieve directly
// ---------------------------------------------------------------------------

// The primes in [lo, hi], where hi may be up to about 10^12.
//
// Any composite in the window has a prime factor <= sqrt(hi), so sieving the small
// primes up to sqrt(hi) is enough to cross out everything in the window. Memory is the
// window size plus sqrt(hi) -- not hi.
// O((hi - lo) log log hi + sqrt(hi)) time.
std::vector<ll> primesInRange(ll lo, ll hi) {
    if (hi < 2 || hi < lo) return {};
    lo = std::max(lo, 2LL);

    int limit = 1;
    while ((ll)(limit + 1) * (limit + 1) <= hi) ++limit;      // limit = floor(sqrt(hi))
    std::vector<char> smallIsPrime = sieve(limit);

    std::vector<char> inWindow(hi - lo + 1, 1);
    for (int p = 2; p <= limit; ++p) {
        if (!smallIsPrime[p]) continue;
        // first multiple of p inside the window, but never below p*p
        ll start = std::max((ll)p * p, ((lo + p - 1) / p) * (ll)p);
        for (ll m = start; m <= hi; m += p) inWindow[m - lo] = 0;
    }

    std::vector<ll> result;
    for (ll m = lo; m <= hi; ++m)
        if (inWindow[m - lo]) result.push_back(m);
    return result;
}

// ---------------------------------------------------------------------------
// Demo
// ---------------------------------------------------------------------------

int main() {
    std::cout << "--- the plain sieve ---\n";
    std::vector<char> isPrime = sieve(50);
    std::cout << "primes up to 50: ";
    for (int i = 2; i <= 50; ++i) if (isPrime[i]) std::cout << i << " ";
    std::cout << "\n";

    const int N = 10000000;
    std::vector<char> big = sieve(N);
    ll count = 0;
    for (int i = 2; i <= N; ++i) count += big[i];
    std::cout << "primes below 10^7: " << count
              << "   (the estimate n/ln n gives 620421 -- it always undercounts a little)\n";

    std::cout << "\n--- smallest prime factor ---\n";
    std::vector<int> primes;
    std::vector<int> spf = smallestPrimeFactorSieve(1000000, primes);
    std::cout << "the linear sieve found " << primes.size() << " primes below 10^6\n";
    std::cout << "spf[84] = " << spf[84] << ", spf[97] = " << spf[97]
              << "   (97 is prime, so spf equals itself)\n";

    std::cout << "\n--- factorization in O(log m) ---\n";
    for (int m : {84, 1000000, 999983}) {
        std::cout << m << " = ";
        auto f = factorize(m, spf);
        for (std::size_t i = 0; i < f.size(); ++i) {
            std::cout << f[i].first;
            if (f[i].second > 1) std::cout << "^" << f[i].second;
            if (i + 1 < f.size()) std::cout << " x ";
        }
        std::cout << "   (tau = " << countDivisors(m, spf) << ")\n";
    }

    std::cout << "\n--- segmented sieve ---\n";
    auto rangePrimes = primesInRange(1000000000000LL, 1000000000100LL);
    std::cout << "primes in [10^12, 10^12 + 100]: ";
    for (ll p : rangePrimes) std::cout << p << " ";
    std::cout << "\n  (a direct sieve would need a 10^12-entry array)\n";

    // ---- self-checks ----
    bool trialOk = true;
    for (int m = 0; m <= 5000; ++m) {
        bool want = m >= 2;
        for (int d = 2; d <= m / d && want; ++d) if (m % d == 0) want = false;
        if ((bool)big[m] != want) trialOk = false;
    }

    bool spfOk = true;
    for (int m = 2; m <= 200000; ++m) {
        int want = 0;
        for (int d = 2; d <= m / d; ++d) if (m % d == 0) { want = d; break; }
        if (want == 0) want = m;                      // m is prime
        if (spf[m] != want) spfOk = false;
    }

    bool factorOk = true;
    for (int m = 1; m <= 200000; ++m) {
        ll product = 1;
        for (auto [p, e] : factorize(m, spf))
            for (int k = 0; k < e; ++k) product *= p;
        if (product != m) factorOk = false;
    }

    bool linearMatchesPlain = true;
    for (int m = 2; m <= 1000000; ++m)
        if ((spf[m] == m) != (bool)big[m]) linearMatchesPlain = false;

    bool segmentedOk = true;
    {
        auto seg = primesInRange(2, 5000);
        std::vector<ll> want;
        for (int m = 2; m <= 5000; ++m) if (big[m]) want.push_back(m);
        if (seg != want) segmentedOk = false;
        auto seg2 = primesInRange(4000, 5000);
        std::vector<ll> want2;
        for (int m = 4000; m <= 5000; ++m) if (big[m]) want2.push_back(m);
        if (seg2 != want2) segmentedOk = false;
    }

    std::cout << "\nself-check: sieve vs trial division " << (trialOk ? "ok" : "FAIL")
              << ", spf vs brute force " << (spfOk ? "ok" : "FAIL")
              << ", factorizations multiply back " << (factorOk ? "ok" : "FAIL")
              << ", linear == plain " << (linearMatchesPlain ? "ok" : "FAIL")
              << ", segmented == plain " << (segmentedOk ? "ok" : "FAIL") << "\n";
    return 0;
}
