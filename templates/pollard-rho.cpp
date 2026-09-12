// Factor one n up to 9e18.  Expected O(n^(1/4) log n).
// Needs: modpow.cpp, gcd.cpp, miller-rabin.cpp
#include <algorithm>
#include <cstdlib>
#include <map>
#include <random>
#include <utility>
#include <vector>
using ll = long long;

// One non-trivial factor of a COMPOSITE n.  Never call it on a prime: it will not stop.
// Brent's cycle detection, with the gcds batched 128 at a time.
ll pollardRho(ll n) {
    if (n % 2 == 0) return 2;
    static std::mt19937_64 rng(std::random_device{}());

    while (true) {
        ll c = (ll)(rng() % (unsigned long long)(n - 1)) + 1;
        auto f = [&](ll x) { return (ll)(((__int128)x * x + c) % n); };

        ll y = (ll)(rng() % (unsigned long long)n);
        ll x = 0, ys = 0, q = 1, g = 1;
        const int m = 128;

        for (ll r = 1; g == 1; r <<= 1) {
            x = y;
            for (ll i = 0; i < r; ++i) y = f(y);
            for (ll k = 0; k < r && g == 1; k += m) {
                ys = y;                              // batch start, kept for replay
                ll lim = std::min<ll>(m, r - k);
                for (ll i = 0; i < lim; ++i) {
                    y = f(y);
                    q = mulMod(q, std::llabs(x - y), n);
                }
                g = gcd(q, n);
            }
        }
        if (g == n) {                                // batch caught two primes -- replay
            g = 1;
            do { ys = f(ys); g = gcd(std::llabs(x - ys), n); } while (g == 1);
        }
        if (g != n) return g;
    }
}

void factorRec(ll n, std::map<ll, int> &out) {
    if (n == 1) return;
    if (isPrime(n)) { ++out[n]; return; }            // the base case that makes it stop
    ll d = pollardRho(n);
    factorRec(d, out);
    factorRec(n / d, out);
}

// Strip the small primes first: rho would waste its budget, and x^2+c cannot split n = 4.
std::vector<std::pair<ll, int>> factorizeBig(ll n) {
    std::map<ll, int> f;
    if (n <= 1) return {};
    for (ll p = 2; p < 100 && p * p <= n; ++p)
        while (n % p == 0) { ++f[p]; n /= p; }
    factorRec(n, f);
    return {f.begin(), f.end()};
}
