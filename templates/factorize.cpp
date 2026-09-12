// Trial division for one n up to ~1e12, and everything that follows from it.  O(sqrt n)
// Past 1e12 use pollard-rho.cpp instead.
#include <algorithm>
#include <utility>
#include <vector>
using ll = long long;

// (prime, exponent) pairs, increasing.  The trailing if is the line people forget.
std::vector<std::pair<ll, int>> factorize(ll n) {
    std::vector<std::pair<ll, int>> f;
    for (ll p = 2; p <= n / p; ++p)                 // p <= n/p, never p*p <= n
        if (n % p == 0) {
            int e = 0;
            while (n % p == 0) { n /= p; ++e; }
            f.push_back({p, e});
        }
    if (n > 1) f.push_back({n, 1});                 // a prime factor above sqrt is left
    return f;
}

ll countDivisors(ll n) {
    ll c = 1;
    for (auto &pe : factorize(n)) c *= pe.second + 1;
    return c;
}

// Summed term by term, so no division and no modular inverse is needed.
ll sumDivisors(ll n) {
    ll s = 1;
    for (auto &pe : factorize(n)) {
        ll term = 1, power = 1;
        for (int i = 0; i < pe.second; ++i) { power *= pe.first; term += power; }
        s *= term;
    }
    return s;
}

// Every divisor, sorted.  The pair loop emits them out of order.
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
