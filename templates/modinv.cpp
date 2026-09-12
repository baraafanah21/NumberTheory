// Modular inverse and linear congruences.  Needs: modpow.cpp, extgcd.cpp
#include <vector>
using ll = long long;

// Any modulus.  Returns -1 when gcd(a,m) != 1, i.e. no inverse exists.  O(log m)
ll modInverse(ll a, ll m) {
    ll x, y;
    if (extgcd(norm(a, m), m, x, y) != 1) return -1;
    return norm(x, m);                              // extgcd may return x < 0
}

// PRIME modulus only.  On a composite m this returns a wrong number silently.  O(log p)
ll modInversePrime(ll a, ll p) { return powMod(a, p - 2, p); }

// inv[1..n] modulo a prime p, all of them.  O(n)
std::vector<ll> inverseTable(int n, ll p) {
    std::vector<ll> inv(n + 1, 1);
    for (int i = 2; i <= n; ++i) inv[i] = (p - p / i) * inv[p % i] % p;
    return inv;
}

// All solutions of a*x = b (mod m): exactly gcd(a,m) of them, or none.
// Returning just one when there are g is the classic half-right answer.  O(log m + g)
std::vector<ll> solveLinearCongruence(ll a, ll b, ll m) {
    std::vector<ll> out;
    if (m <= 0) return out;
    a = norm(a, m);
    b = norm(b, m);
    ll x, y, g = extgcd(a, m, x, y);
    if (b % g != 0) return out;
    ll step = m / g;
    ll first = norm(mulMod(norm(x, m), (b / g) % m, m), step);
    for (ll k = 0; k < g; ++k) out.push_back(first + k * step);
    return out;
}
