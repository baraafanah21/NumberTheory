// Unique Factorization -- g++ -std=c++17 -O2 implementation.cpp
//
// The theorem is a proof, not an algorithm.  What it gives you here is the guarantee
// that the canonical form below is THE factorization, so every formula built on it is
// well defined.

#include <algorithm>
#include <iostream>
#include <numeric>
#include <vector>

using ll = long long;

struct Factor { ll prime; int exp; };

// n = p1^e1 * p2^e2 * ..., primes increasing, every exponent >= 1.
//
// `while (n % p == 0)` strips the whole power, so each prime appears once with its full
// exponent.  The trailing `if (n > 1)` is the line people forget: whatever survives the
// loop is either 1 or a single prime above the original sqrt(n).
//
// n shrinks as factors come out, so the bound falls too -- that is what keeps this
// O(sqrt n) overall rather than per prime.
// O(sqrt n).  Precondition: n >= 1.
std::vector<Factor> factorize(ll n) {
    std::vector<Factor> f;
    for (ll p = 2; p <= n / p; ++p)          // p <= n/p, never p*p <= n (overflow)
        if (n % p == 0) {
            int e = 0;
            while (n % p == 0) { n /= p; ++e; }
            f.push_back({p, e});
        }
    if (n > 1) f.push_back({n, 1});
    return f;
}

// ---------------------------------------------------------------- what it unlocks

// tau = product of (e+1): a divisor picks any exponent 0..e, independently per prime.
ll countDivisors(ll n) {
    ll t = 1;
    for (const Factor &f : factorize(n)) t *= f.exp + 1;
    return t;
}

// sigma = product of (1 + p + ... + p^e), each factor a geometric series.
ll sumDivisors(ll n) {
    ll s = 1;
    for (const Factor &f : factorize(n)) {
        ll term = 1, pw = 1;
        for (int i = 0; i < f.exp; ++i) { pw *= f.prime; term += pw; }
        s *= term;
    }
    return s;
}

// phi = n * product of (1 - 1/p) over distinct primes.
ll totient(ll n) {
    ll r = n;
    for (const Factor &f : factorize(n)) r -= r / f.prime;
    return r;
}

// Every divisor, built by taking each prime to every allowed exponent -- the theorem
// made literal: divisors correspond to exponent vectors below (e_1,...,e_k).
// O(sqrt n + tau(n) log tau(n))
std::vector<ll> allDivisors(ll n) {
    std::vector<ll> ds{1};
    for (const Factor &f : factorize(n)) {
        std::size_t before = ds.size();
        ll pw = 1;
        for (int e = 1; e <= f.exp; ++e) {
            pw *= f.prime;
            for (std::size_t i = 0; i < before; ++i) ds.push_back(ds[i] * pw);
        }
    }
    std::sort(ds.begin(), ds.end());
    return ds;
}

// n is a perfect k-th power exactly when every exponent is divisible by k.
bool isPerfectPower(ll n, int k) {
    if (n < 1) return false;
    for (const Factor &f : factorize(n))
        if (f.exp % k != 0) return false;
    return true;
}

// ---------------------------------------------------------------- demo

int main() {
    std::cout << "canonical form\n";
    for (ll n : {1LL, 60LL, 97LL, 1024LL, 999999999989LL}) {
        std::cout << "  " << n << " = ";
        auto f = factorize(n);
        if (f.empty()) std::cout << "(empty product)";
        for (std::size_t i = 0; i < f.size(); ++i) {
            std::cout << f[i].prime;
            if (f[i].exp > 1) std::cout << "^" << f[i].exp;
            if (i + 1 < f.size()) std::cout << " x ";
        }
        std::cout << "\n";
    }
    std::cout << "  (1 is the empty product, and 1 is NOT prime -- else uniqueness fails)\n";

    std::cout << "\neverything below comes from the exponents of 60 = 2^2 x 3 x 5\n";
    std::cout << "  tau   = (2+1)(1+1)(1+1)      = " << countDivisors(60) << "\n";
    std::cout << "  sigma = (1+2+4)(1+3)(1+5)    = " << sumDivisors(60) << "\n";
    std::cout << "  phi   = 60(1-1/2)(1-1/3)(1-1/5) = " << totient(60) << "\n";
    std::cout << "  divisors: ";
    for (ll d : allDivisors(60)) std::cout << d << " ";
    std::cout << "\n";

    std::cout << "\nperfect powers are exponent divisibility\n";
    for (ll m : {36LL, 64LL, 72LL})
        std::cout << "  " << m << ": square? " << isPerfectPower(m, 2)
                  << "  cube? " << isPerfectPower(m, 3) << "\n";

    bool rebuildOk = true, orderOk = true, primeOk = true, divOk = true, formulaOk = true;
    for (ll m = 1; m <= 20000; ++m) {
        auto f = factorize(m);
        ll product = 1;
        for (const Factor &x : f) {
            for (int i = 0; i < x.exp; ++i) product *= x.prime;
            if (x.exp < 1) orderOk = false;
        }
        if (product != m) rebuildOk = false;
        for (std::size_t i = 1; i < f.size(); ++i)
            if (f[i].prime <= f[i - 1].prime) orderOk = false;
        for (const Factor &x : f) {                       // every factor must BE prime
            bool p = x.prime >= 2;
            for (ll d = 2; d <= x.prime / d && p; ++d)
                if (x.prime % d == 0) p = false;
            if (!p) primeOk = false;
        }
    }
    for (ll m = 1; m <= 3000; ++m) {
        std::vector<ll> want;
        ll tau = 0, sigma = 0, phi = 0;
        for (ll d = 1; d <= m; ++d)
            if (m % d == 0) { want.push_back(d); ++tau; sigma += d; }
        for (ll k = 1; k <= m; ++k) if (std::gcd(k, m) == 1) ++phi;
        if (allDivisors(m) != want) divOk = false;
        if (countDivisors(m) != tau || sumDivisors(m) != sigma || totient(m) != phi)
            formulaOk = false;
    }
    std::cout << "\nself-check: factors multiply back " << (rebuildOk ? "ok" : "FAIL")
              << ", canonical order " << (orderOk ? "ok" : "FAIL")
              << ", every factor prime " << (primeOk ? "ok" : "FAIL")
              << ", divisor list " << (divOk ? "ok" : "FAIL")
              << ", tau/sigma/phi " << (formulaOk ? "ok" : "FAIL") << "\n";
}
