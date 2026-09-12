// Primes in [lo, hi] with hi up to ~1e12.  Memory is the window plus sqrt(hi), not hi.
// O((hi - lo) log log hi + sqrt hi).  Needs: sieve.cpp
#include <algorithm>
#include <vector>
using ll = long long;

std::vector<ll> primesInRange(ll lo, ll hi) {
    if (hi < 2 || hi < lo) return {};
    lo = std::max(lo, 2LL);

    int limit = 1;
    while ((ll)(limit + 1) * (limit + 1) <= hi) ++limit;   // floor(sqrt(hi))
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
