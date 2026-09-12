// Discrete log: smallest x with g^x = b (mod m).  Needs: modpow.cpp
// Baby-step giant-step, O(sqrt m) time and memory.  Returns -1 if no solution.
#include <cmath>
#include <numeric>
#include <unordered_map>
using ll = long long;

ll discreteLog(ll g, ll b, ll m) {
    if (m <= 0) return -1;
    g = norm(g, m);
    b = norm(b, m);
    if (m == 1) return 0;
    if (std::gcd(g, m) != 1) return -1;              // the standard version needs this
    if (b == 1 % m) return 0;

    ll n = (ll)std::sqrt((double)m) + 1;
    while (n * n < m) ++n;

    std::unordered_map<ll, ll> baby;
    baby.reserve(n * 2);
    ll v = b;
    for (ll j = 0; j < n; ++j, v = mulMod(v, g, m))
        baby[v] = j;                                 // keep the LARGEST j per key

    ll stride = powMod(g, n, m), cur = 1 % m;
    for (ll i = 1; i <= n; ++i) {                    // ascending i -> first hit is smallest x
        cur = mulMod(cur, stride, m);
        auto it = baby.find(cur);
        if (it != baby.end()) return i * n - it->second;
    }
    return -1;
}
