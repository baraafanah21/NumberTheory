// Pollard's Rho Factorization -- g++ -std=c++17 -O2 implementation.cpp

#include <algorithm>
#include <chrono>
#include <cstdlib>
#include <iostream>
#include <map>
#include <random>
#include <utility>
#include <vector>

using ll = long long;

// ---------------------------------------------------------------- borrowed

// Same three functions as ../miller-rabin/, unchanged.  Rho needs a
// primality test to know when a piece is finished, and the modulus here reaches 9e18, so
// every product still has to go through __int128.
ll mulMod(ll a, ll b, ll m) { return (ll)((__int128)a * b % m); }

ll powMod(ll base, ll exp, ll m) {
    ll r = 1 % m;
    base %= m;
    for (; exp > 0; exp >>= 1, base = mulMod(base, base, m))
        if (exp & 1) r = mulMod(r, base, m);
    return r;
}

bool passesBase(ll n, ll a) {
    if (n % a == 0) return n == a;
    ll d = n - 1;
    int s = 0;
    while (d % 2 == 0) { d /= 2; ++s; }
    ll x = powMod(a, d, n);
    if (x == 1 || x == n - 1) return true;
    for (int r = 1; r < s; ++r) {
        x = mulMod(x, x, n);
        if (x == n - 1) return true;
    }
    return false;
}

// Deterministic for every 64-bit n.
bool isPrime(ll n) {
    if (n < 2) return false;
    for (ll p : {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37})
        if (n % p == 0) return n == p;
    for (ll a : {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37})
        if (!passesBase(n, a)) return false;
    return true;
}

ll gcd(ll a, ll b) {
    while (b) { ll r = a % b; a = b; b = r; }
    return a;
}

// ---------------------------------------------------------------- one split

// Find ONE non-trivial factor of a composite n -- not necessarily prime, not necessarily
// the smallest.  Never call it on a prime: there is nothing to find and it will not stop.
//
// The idea.  Iterate f(x) = x^2 + c (mod n) from a random start.  We cannot see the
// factor p, but the same sequence read mod p collides after about sqrt(p) steps -- the
// birthday bound -- and p <= sqrt(n), so that is n^(1/4) steps.  A collision mod p means
//     x = y (mod p)   but usually   x != y (mod n)
// and then gcd(|x - y|, n) is a proper divisor.  We detect it without ever knowing p.
//
// Two speedups, both standard:
//   * Brent's cycle detection -- compare against a checkpoint x held for r = 1, 2, 4, ...
//     steps, instead of Floyd's two pointers.  Same guarantee, one f() per step.
//   * batched gcd -- multiply 128 differences together (mod n) and take one gcd.  A gcd
//     costs far more than a multiply, so this is most of the speed.  If a batch returns n
//     it may have collected two different factors at once, so we replay that batch one
//     step at a time; that path is rare and cheap.
//
// Expected O(n^(1/4) log n)
ll pollardRho(ll n) {
    if (n % 2 == 0) return 2;

    // Fixed seed: the demo below must print the same thing on every run.  A real library
    // would seed from the clock -- the algorithm is correct for any start.
    static std::mt19937_64 rng(9876543210987654321ULL);

    while (true) {                                   // retry until a start splits n
        ll c = (ll)(rng() % (unsigned long long)(n - 1)) + 1;
        auto f = [&](ll x) { return (ll)(((__int128)x * x + c) % n); };

        ll y = (ll)(rng() % (unsigned long long)n);
        ll x = 0, ys = 0, q = 1, g = 1;
        const int m = 128;                           // gcd batch size

        for (ll r = 1; g == 1; r <<= 1) {
            x = y;
            for (ll i = 0; i < r; ++i) y = f(y);     // advance to the next checkpoint
            for (ll k = 0; k < r && g == 1; k += m) {
                ys = y;                              // start of this batch, kept for replay
                ll lim = std::min<ll>(m, r - k);
                for (ll i = 0; i < lim; ++i) {
                    y = f(y);
                    q = mulMod(q, std::llabs(x - y), n);
                }
                g = gcd(q, n);                       // one gcd per 128 steps
            }
        }

        if (g == n) {                                // batch overshot -- redo it slowly
            g = 1;
            do {
                ys = f(ys);
                g = gcd(std::llabs(x - ys), n);
            } while (g == 1);
        }

        if (g != n) return g;                        // 1 < g < n: a genuine factor
        // g == n even step by step: this (start, c) pair is degenerate.  Pick another.
    }
}

// ---------------------------------------------------------------- full factorization

// Split n recursively until every piece is prime.  Miller-Rabin is the base case: it is
// what tells the recursion when to stop.
void factorRec(ll n, std::map<ll, int> &out) {
    if (n == 1) return;
    if (isPrime(n)) { ++out[n]; return; }
    ll d = pollardRho(n);
    factorRec(d, out);
    factorRec(n / d, out);
}

// Canonical form, as (prime, exponent) pairs in increasing order.  Same contract as
// ../prime-factorization/, but for n up to 9e18 instead of 1e12.
//
// The small primes are stripped by trial division first.  Two reasons: rho spends its
// n^(1/4) budget looking for a factor a single division would have found, and f(x) = x^2+c
// cannot split n = 4 at all.  Expected O(n^(1/4) log n)
std::vector<std::pair<ll, int>> factorize(ll n) {
    std::map<ll, int> f;
    if (n <= 1) return {};
    for (ll p = 2; p < 100 && p * p <= n; ++p)
        while (n % p == 0) { ++f[p]; n /= p; }
    factorRec(n, f);
    return {f.begin(), f.end()};
}

// ---------------------------------------------------------------- payoff

// tau(n) -- now reachable for n up to 9e18, where the O(sqrt n) loop was not.  O(n^(1/4))
ll countDivisors(ll n) {
    ll t = 1;
    for (auto [p, e] : factorize(n)) { (void)p; t *= e + 1; }
    return t;
}

// Every divisor of n, unsorted.  Grows each partial product by one prime power at a time.
std::vector<ll> allDivisors(ll n) {
    std::vector<ll> d = {1};
    for (auto [p, e] : factorize(n)) {
        size_t sz = d.size();
        ll pk = 1;
        for (int i = 0; i < e; ++i) {
            pk *= p;
            for (size_t j = 0; j < sz; ++j) d.push_back(d[j] * pk);
        }
    }
    return d;
}

// ---------------------------------------------------------------- demo

static std::string render(const std::vector<std::pair<ll, int>> &f) {
    std::string s;
    for (size_t i = 0; i < f.size(); ++i) {
        if (i) s += " x ";
        s += std::to_string(f[i].first);
        if (f[i].second > 1) s += "^" + std::to_string(f[i].second);
    }
    return s.empty() ? "1" : s;
}

int main() {
    std::cout << "numbers trial division cannot reach\n";
    const ll cases[] = {1000000016000000063LL,      // 1000000007 x 1000000009
                        9223372036854775807LL,      // 2^63 - 1
                        999999943999999559LL,       // 999999937 x 1000000007
                        1000000000000000000LL,      // 10^18
                        4611686018427387903LL,      // 2^62 - 1
                        1000000000000000009LL};     // prime
    for (ll n : cases)
        std::cout << "  " << n << " = " << render(factorize(n)) << "\n";

    std::cout << "\nwhy this is not trial division\n";
    ll hard = 999999943999999559LL;
    auto start = std::chrono::steady_clock::now();
    auto f = factorize(hard);
    auto us = std::chrono::duration_cast<std::chrono::microseconds>(
                  std::chrono::steady_clock::now() - start).count();
    std::cout << "  " << hard << " = " << render(f) << " in " << us << " us\n";
    std::cout << "  its smallest prime factor is 999999937, so trial division needs\n"
              << "  ~1e9 divisions -- about a second, against the microseconds above\n";

    std::cout << "\nthe payoff: divisor counts past 1e12\n";
    for (ll n : {963761198400LL, 1000000000000000000LL, 897612484786617600LL})
        std::cout << "  tau(" << n << ") = " << countDivisors(n) << "\n";

    start = std::chrono::steady_clock::now();
    std::mt19937_64 rng(2024);
    int done = 0;
    for (int i = 0; i < 200; ++i) {
        ll n = (ll)(rng() % 9000000000000000000ULL) + 1000000000LL;
        factorize(n);
        ++done;
    }
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(
                  std::chrono::steady_clock::now() - start).count();
    std::cout << "\n  factored " << done << " random numbers up to 9e18 in " << ms << " ms\n";

    // ---- self-checks ----
    // 1. Against trial division on a small range: same answer every time.
    bool smallOk = true;
    for (ll n = 2; n <= 20000; ++n) {
        std::vector<std::pair<ll, int>> want;
        ll m = n;
        for (ll p = 2; p * p <= m; ++p)
            if (m % p == 0) {
                int e = 0;
                while (m % p == 0) { m /= p; ++e; }
                want.push_back({p, e});
            }
        if (m > 1) want.push_back({m, 1});
        if (factorize(n) != want) smallOk = false;
    }

    // 2. On big inputs there is nothing to compare against, so check the definition:
    //    the factors are prime, and they multiply back to n.
    bool bigOk = true;
    std::mt19937_64 check(7);
    for (int i = 0; i < 300; ++i) {
        ll n = (ll)(check() % 9000000000000000000ULL) + 2;
        __int128 prod = 1;
        for (auto [p, e] : factorize(n)) {
            if (!isPrime(p)) bigOk = false;
            for (int j = 0; j < e; ++j) prod *= p;
        }
        if (prod != (__int128)n) bigOk = false;
    }

    // 3. Semiprimes of two large primes -- the worst case, and the one rho exists for.
    bool semiprimeOk = true;
    const ll bigPrimes[] = {999999937LL, 1000000007LL, 1000000009LL, 998244353LL,
                            2147483647LL, 3037000493LL};
    for (ll p : bigPrimes)
        for (ll q : bigPrimes) {
            if ((__int128)p * q > (__int128)9000000000000000000LL) continue;
            auto g = factorize(p * q);
            ll want = 0;
            for (auto [r, e] : g) want += e;
            if (want != 2) semiprimeOk = false;
            if (p == q && (g.size() != 1 || g[0] != std::make_pair(p, 2))) semiprimeOk = false;
            if (p != q && (g.size() != 2 || g[0].first != std::min(p, q) ||
                           g[1].first != std::max(p, q))) semiprimeOk = false;
        }

    // 4. Prime powers, which have only one factor to find and no gcd to separate.
    bool powerOk = true;
    for (ll p : {2LL, 3LL, 1000003LL, 2147483647LL})
        for (int e = 1; e <= 4; ++e) {
            __int128 v = 1;
            for (int i = 0; i < e; ++i) v *= p;
            if (v > (__int128)9000000000000000000LL) continue;
            auto g = factorize((ll)v);
            if (g.size() != 1 || g[0] != std::make_pair(p, e)) powerOk = false;
        }

    // 5. The divisor list must have exactly tau(n) entries and every one must divide n.
    bool divisorOk = true;
    for (ll n : {963761198400LL, 1000000016000000063LL, 1000000000000000000LL}) {
        auto d = allDivisors(n);
        if ((ll)d.size() != countDivisors(n)) divisorOk = false;
        for (ll x : d) if (n % x != 0) divisorOk = false;
    }

    std::cout << "\nself-check: matches trial division for n <= 20000 "
              << (smallOk ? "ok" : "FAIL")
              << ", 300 random n up to 9e18 reassemble " << (bigOk ? "ok" : "FAIL")
              << ",\n            large semiprimes split " << (semiprimeOk ? "ok" : "FAIL")
              << ", prime powers " << (powerOk ? "ok" : "FAIL")
              << ", divisor lists " << (divisorOk ? "ok" : "FAIL") << "\n";
}
