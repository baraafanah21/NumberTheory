// Unique Factorization (Fundamental Theorem of Arithmetic)
// C++17.  Compile: g++ -std=c++17 -O2 -o factorization implementation.cpp
//
// The theorem itself is a proof, not an algorithm. What it gives you in code is the
// guarantee that the canonical form below is THE factorization -- so every formula
// derived from it is well defined.

#include <algorithm>
#include <iostream>
#include <numeric>
#include <vector>

using ll = long long;

// A prime and how many times it appears.
struct Factor { ll prime; int exponent; };

// ---------------------------------------------------------------------------
// 1. The canonical form
// ---------------------------------------------------------------------------

// n = p1^e1 * p2^e2 * ... with p1 < p2 < ..., each exponent >= 1.
//
// Two details:
//   - `while (n % p == 0)` strips the whole power before moving on, so each prime
//     appears once in the output with its full exponent
//   - the trailing `if (n > 1)` is the line people forget. After the loop whatever is
//     left is either 1 or a single prime larger than the original sqrt(n).
//
// n shrinks as factors are removed, so the loop bound falls too -- that is what keeps
// this O(sqrt n) overall rather than O(sqrt n) per prime.
// Precondition: n >= 1.   O(sqrt n)
std::vector<Factor> factorize(ll n) {
    std::vector<Factor> factors;
    for (ll p = 2; p <= n / p; ++p)          // p <= n/p, never p*p <= n (overflow)
        if (n % p == 0) {
            int e = 0;
            while (n % p == 0) { n /= p; ++e; }
            factors.push_back({p, e});
        }
    if (n > 1) factors.push_back({n, 1});
    return factors;
}

// Multiply a canonical form back out. Used to check factorize against its own input.
ll rebuild(const std::vector<Factor> &factors) {
    ll n = 1;
    for (const Factor &f : factors)
        for (int i = 0; i < f.exponent; ++i) n *= f.prime;
    return n;
}

// ---------------------------------------------------------------------------
// 2. What the theorem unlocks
// ---------------------------------------------------------------------------

// tau(n) = product of (e_i + 1): a divisor picks any exponent from 0..e_i, independently.
ll countDivisors(ll n) {
    ll tau = 1;
    for (const Factor &f : factorize(n)) tau *= f.exponent + 1;
    return tau;
}

// sigma(n) = product of (1 + p + ... + p^e), each factor a geometric series.
ll sumDivisors(ll n) {
    ll sigma = 1;
    for (const Factor &f : factorize(n)) {
        ll term = 1, power = 1;
        for (int i = 0; i < f.exponent; ++i) { power *= f.prime; term += power; }
        sigma *= term;
    }
    return sigma;
}

// phi(n) = n * product over distinct primes of (1 - 1/p).
ll totient(ll n) {
    ll result = n;
    for (const Factor &f : factorize(n)) result -= result / f.prime;
    return result;
}

// Every divisor of n, built by taking each prime to every allowed exponent.
// This is the theorem made literal: divisors <-> exponent vectors below (e_1,...,e_k).
// O(sqrt n + number of divisors)
std::vector<ll> allDivisors(ll n) {
    std::vector<ll> divisors{1};
    for (const Factor &f : factorize(n)) {
        std::size_t sizeBefore = divisors.size();
        ll power = 1;
        for (int e = 1; e <= f.exponent; ++e) {
            power *= f.prime;
            for (std::size_t i = 0; i < sizeBefore; ++i)
                divisors.push_back(divisors[i] * power);
        }
    }
    std::sort(divisors.begin(), divisors.end());
    return divisors;
}

// n is a perfect k-th power exactly when every exponent is divisible by k.
bool isPerfectPower(ll n, int k) {
    if (n < 1) return false;
    for (const Factor &f : factorize(n))
        if (f.exponent % k != 0) return false;
    return true;
}

// ---------------------------------------------------------------------------
// 3. gcd and lcm by exponents -- for understanding, NOT for computing
// ---------------------------------------------------------------------------

// gcd takes the min exponent of each prime, lcm takes the max.
// Correct, and far slower than the Euclidean algorithm because it must factor first.
// Included only to make the min/max characterization concrete.
ll gcdByFactorization(ll a, ll b) {
    ll g = 1;
    for (const Factor &fa : factorize(a))
        for (const Factor &fb : factorize(b))
            if (fa.prime == fb.prime)
                for (int i = 0; i < std::min(fa.exponent, fb.exponent); ++i)
                    g *= fa.prime;
    return g;
}

// ---------------------------------------------------------------------------
// Demo
// ---------------------------------------------------------------------------

void print(ll n, const std::vector<Factor> &f) {
    std::cout << n << " = ";
    for (std::size_t i = 0; i < f.size(); ++i) {
        std::cout << f[i].prime;
        if (f[i].exponent > 1) std::cout << "^" << f[i].exponent;
        if (i + 1 < f.size()) std::cout << " x ";
    }
    if (f.empty()) std::cout << "(empty product)";
    std::cout << "\n";
}

int main() {
    std::cout << "--- the canonical form ---\n";
    for (ll n : {1LL, 60LL, 97LL, 1024LL, 999999999989LL}) print(n, factorize(n));
    std::cout << "(1 factors as the empty product -- and 1 is NOT prime, or uniqueness"
                 " would fail)\n";

    std::cout << "\n--- what it unlocks, all from the exponents ---\n";
    ll n = 60;                                  // 2^2 x 3 x 5
    std::cout << "n = 60 = 2^2 x 3 x 5\n";
    std::cout << "  tau   = (2+1)(1+1)(1+1) = " << countDivisors(n) << "\n";
    std::cout << "  sigma = (1+2+4)(1+3)(1+5) = " << sumDivisors(n) << "\n";
    std::cout << "  phi   = 60 (1-1/2)(1-1/3)(1-1/5) = " << totient(n) << "\n";
    std::cout << "  divisors: ";
    for (ll d : allDivisors(n)) std::cout << d << " ";
    std::cout << "\n";

    std::cout << "\n--- perfect powers are exponent divisibility ---\n";
    for (ll m : {36LL, 64LL, 72LL})
        std::cout << "  " << m << ": square? " << isPerfectPower(m, 2)
                  << "  cube? " << isPerfectPower(m, 3) << "\n";

    std::cout << "\n--- gcd by min-exponent (correct, but slow) ---\n";
    std::cout << "  gcd(48, 180) by factorization = " << gcdByFactorization(48, 180)
              << ", by Euclid = " << std::gcd(48, 180) << "\n";
    std::cout << "  48 = 2^4 x 3, 180 = 2^2 x 3^2 x 5  ->  min gives 2^2 x 3 = 12\n";

    // ---- self-checks ----
    bool rebuildOk = true, sortedOk = true;
    for (ll m = 1; m <= 20000; ++m) {
        std::vector<Factor> f = factorize(m);
        if (rebuild(f) != m) rebuildOk = false;
        for (std::size_t i = 1; i < f.size(); ++i)
            if (f[i].prime <= f[i - 1].prime) sortedOk = false;
        for (const Factor &x : f) if (x.exponent < 1) sortedOk = false;
    }

    // Every factor reported must actually be prime -- otherwise it is not a
    // factorization into primes at all.
    bool allPrime = true;
    for (ll m = 2; m <= 20000; ++m)
        for (const Factor &f : factorize(m)) {
            bool prime = f.prime >= 2;
            for (ll d = 2; d <= f.prime / d && prime; ++d)
                if (f.prime % d == 0) prime = false;
            if (!prime) allPrime = false;
        }

    // The divisor list built from exponents must match a direct scan.
    bool divisorsOk = true;
    for (ll m = 1; m <= 3000; ++m) {
        std::vector<ll> want;
        for (ll d = 1; d <= m; ++d) if (m % d == 0) want.push_back(d);
        if (allDivisors(m) != want) divisorsOk = false;
    }

    // The formulas must match direct computation.
    bool formulasOk = true;
    for (ll m = 1; m <= 5000; ++m) {
        ll tau = 0, sigma = 0, phi = 0;
        for (ll d = 1; d <= m; ++d) if (m % d == 0) { ++tau; sigma += d; }
        for (ll k = 1; k <= m; ++k) if (std::gcd(k, m) == 1) ++phi;
        if (countDivisors(m) != tau || sumDivisors(m) != sigma || totient(m) != phi)
            formulasOk = false;
    }

    // min-exponent gcd must agree with Euclid.
    bool gcdOk = true;
    for (ll a = 1; a <= 300; ++a)
        for (ll b = 1; b <= 300; ++b)
            if (gcdByFactorization(a, b) != std::gcd(a, b)) gcdOk = false;

    std::cout << "\nself-check: factors multiply back " << (rebuildOk ? "ok" : "FAIL")
              << ", canonical order " << (sortedOk ? "ok" : "FAIL")
              << ", every factor is prime " << (allPrime ? "ok" : "FAIL")
              << ", divisor list " << (divisorsOk ? "ok" : "FAIL")
              << ", tau/sigma/phi formulas " << (formulasOk ? "ok" : "FAIL")
              << ", min-exponent gcd " << (gcdOk ? "ok" : "FAIL") << "\n";
    return 0;
}
