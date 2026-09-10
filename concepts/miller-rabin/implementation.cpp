// Miller-Rabin Primality Test -- g++ -std=c++17 -O2 implementation.cpp

#include <chrono>
#include <iostream>
#include <vector>

using ll = long long;

// n reaches 9e18, so products reach 8e37 -- far past 64 bits.  Doing this in long long
// silently wraps and the whole test returns nonsense on exactly the inputs it exists for.
ll mulMod(ll a, ll b, ll m) { return (ll)((__int128)a * b % m); }

ll powMod(ll base, ll exp, ll m) {
    ll r = 1 % m;
    base %= m;
    for (; exp > 0; exp >>= 1, base = mulMod(base, base, m))
        if (exp & 1) r = mulMod(r, base, m);
    return r;
}

// ---------------------------------------------------------------- one base

// Does n pass the Miller-Rabin test for base a?
//
// Write n - 1 = d * 2^s with d odd, then square up the chain
//     a^d, a^2d, a^4d, ..., a^(n-1)
// If n is prime the chain ends at 1 (Fermat), and the entry before the first 1 must be
// -1, because modulo a prime the only square roots of 1 are +/-1.  So a prime satisfies
//     a^d = 1   or   a^(2^r * d) = -1  for some 0 <= r < s
// Failing both proves n composite -- a is then called a witness.
//
// Returns true for "passes" (possibly prime), false for "definitely composite".
// O(log n) modular multiplications
bool passesBase(ll n, ll a) {
    if (n % a == 0) return n == a;           // n is itself this small base

    ll d = n - 1;
    int s = 0;
    while (d % 2 == 0) { d /= 2; ++s; }

    ll x = powMod(a, d, n);
    if (x == 1 || x == n - 1) return true;   // this is the r = 0 case
    for (int r = 1; r < s; ++r) {
        x = mulMod(x, x, n);
        if (x == n - 1) return true;
    }
    return false;
}

// ---------------------------------------------------------------- the test

// Deterministic for every 64-bit n.
//
// The first 12 primes are a verified witness set for n < 3.18e23, which covers the whole
// range with room to spare.  Note the first 11 primes only reach 3.8e18 -- just BELOW
// 2^64, so dropping a base to save time makes the test wrong on part of long long.
// O(12 log n) multiplications -- about 750 for n near 1e18
bool isPrime(ll n) {
    if (n < 2) return false;
    for (ll p : {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37})
        if (n % p == 0) return n == p;       // clears the small primes and all evens

    for (ll a : {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37})
        if (!passesBase(n, a)) return false;
    return true;
}

// The plain Fermat test, for contrast only -- Carmichael numbers pass it for every base
// coprime to them, so it is not a primality test.
bool passesFermat(ll n, ll a) { return powMod(a, n - 1, n) == 1; }

// ---------------------------------------------------------------- demo

int main() {
    std::cout << "small numbers\n  primes up to 50: ";
    for (ll n = 2; n <= 50; ++n) if (isPrime(n)) std::cout << n << " ";
    std::cout << "\n";

    std::cout << "\nwhy the plain Fermat test is not enough\n";
    std::cout << "  561 = 3 x 11 x 17 is composite, yet it passes Fermat for base ";
    for (ll a : {2, 4, 5, 7}) if (passesFermat(561, a)) std::cout << a << " ";
    std::cout << "\n  Miller-Rabin says 561 is prime? " << isPrime(561) << "\t[0 -- caught]\n";
    std::cout << "  the first few Carmichael numbers: ";
    for (ll n = 3; n <= 3000; n += 2) {
        if (isPrime(n)) continue;
        bool allPass = true;
        for (ll a = 2; a < n && allPass; ++a) {
            ll g = n, b = a;
            while (b) { ll t = g % b; g = b; b = t; }
            if (g == 1 && !passesFermat(n, a)) allPass = false;
        }
        if (allPass) std::cout << n << " ";
    }
    std::cout << "\n";

    std::cout << "\nlarge numbers, where trial division cannot go\n";
    const ll cases[] = {1000000007LL, 1000000009LL, 999999999989LL,
                        4611686018427387847LL, 9223372036854775783LL,
                        1000000016000000063LL};
    for (ll n : cases)
        std::cout << "  " << n << "\t" << (isPrime(n) ? "prime" : "composite") << "\n";
    std::cout << "  (the last is 1000000007 x 1000000009 -- a product of two primes)\n";

    auto start = std::chrono::steady_clock::now();
    int count = 0;
    for (ll n = 9223372036854775000LL; n <= 9223372036854775783LL; ++n)
        if (isPrime(n)) ++count;
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(
                  std::chrono::steady_clock::now() - start).count();
    std::cout << "\n  tested 784 numbers near 2^63: found " << count << " primes in "
              << ms << " ms\n  (trial division would need ~3 billion steps each)\n";

    // ---- self-checks ----
    // A sieve gives ground truth on the small range.
    const int N = 2000000;
    std::vector<char> sieve(N + 1, 1);
    sieve[0] = sieve[1] = 0;
    for (int p = 2; (ll)p * p <= N; ++p)
        if (sieve[p])
            for (ll m = (ll)p * p; m <= N; m += p) sieve[m] = 0;

    bool sieveOk = true;
    for (int n = 0; n <= N; ++n)
        if (isPrime(n) != (bool)sieve[n]) sieveOk = false;

    // Products of two large primes must all come back composite.
    bool semiprimeOk = true;
    const ll bigPrimes[] = {1000000007LL, 1000000009LL, 998244353LL, 1000000021LL};
    for (ll p : bigPrimes)
        for (ll q : bigPrimes)
            if (isPrime(p * q)) semiprimeOk = false;

    // Carmichael numbers pass Fermat but must fail Miller-Rabin.
    bool carmichaelOk = true;
    for (ll n : {561, 1105, 1729, 2465, 2821, 6601, 8911, 41041, 62745, 162401})
        if (isPrime(n)) carmichaelOk = false;

    // Known large primes and their neighbours.
    bool bigOk = isPrime(9223372036854775783LL) && !isPrime(9223372036854775784LL) &&
                 isPrime(4611686018427387847LL) && !isPrime(1000000016000000063LL);

    std::cout << "\nself-check: matches a sieve for all n <= " << N << " "
              << (sieveOk ? "ok" : "FAIL")
              << ", semiprimes rejected " << (semiprimeOk ? "ok" : "FAIL")
              << ", Carmichael numbers rejected " << (carmichaelOk ? "ok" : "FAIL")
              << ", 63-bit cases " << (bigOk ? "ok" : "FAIL") << "\n";
}
