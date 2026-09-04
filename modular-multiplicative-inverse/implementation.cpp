// Modular Multiplicative Inverse -- g++ -std=c++17 -O2 implementation.cpp

#include <iostream>
#include <vector>

using ll = long long;

ll norm(ll x, ll m) { x %= m; return x < 0 ? x + m : x; }
ll mulMod(ll a, ll b, ll m) { return (ll)((__int128)a * b % m); }

ll powMod(ll base, ll exp, ll m) {
    ll r = 1 % m;
    base = norm(base, m);
    for (; exp > 0; exp >>= 1, base = mulMod(base, base, m))
        if (exp & 1) r = mulMod(r, base, m);
    return r;
}

ll extgcd(ll a, ll b, ll &x, ll &y) {
    if (b == 0) { x = 1; y = 0; return a; }
    ll x1, y1;
    ll g = extgcd(b, a % b, x1, y1);
    x = y1;
    y = x1 - (a / b) * y1;
    return g;
}

// ---------------------------------------------------------------- three ways

// a^-1 mod m for ANY m, or -1 when none exists.
//
// extgcd gives a*x + m*y = g.  If g == 1, reducing mod m kills the m*y term and leaves
// a*x = 1, so x is the inverse.  Both normalizations matter: the input may be negative,
// and extgcd returns a signed coefficient.
// O(log m)
ll modInverse(ll a, ll m) {
    if (m <= 1) return -1;
    ll x, y;
    if (extgcd(norm(a, m), m, x, y) != 1) return -1;
    return norm(x, m);
}

// a^-1 = a^(p-2) mod p, since a^(p-1) = 1 for prime p not dividing a.
// PRIME p ONLY -- on a composite modulus this returns a wrong value with no error.
// O(log p)
ll modInversePrime(ll a, ll p) { return powMod(a, p - 2, p); }

// inv[i] for i = 1..n mod prime p, in O(n).
// From p = q*i + r:  q*i + r = 0 (mod p)  =>  inv[i] = -q * inv[r], and r < i so
// inv[r] is already known.  Writing -q as (p - q) keeps everything non-negative.
// Requires p prime and n < p.
std::vector<ll> inverseTable(int n, ll p) {
    std::vector<ll> inv(n + 1, 0);
    if (n >= 1) inv[1] = 1;
    for (int i = 2; i <= n; ++i) inv[i] = (p - p / i) * inv[p % i] % p;
    return inv;
}

// ---------------------------------------------------------------- uses

// (a / b) mod m, done correctly.  Never write (a / b) % m -- integer division discards
// the remainder.  Returns -1 when b has no inverse.
ll divideMod(ll a, ll b, ll m) {
    ll ib = modInverse(b, m);
    return ib < 0 ? -1 : mulMod(norm(a, m), ib, m);
}

// C(n,k) mod a prime, O(1) per query after an O(n) build.
// One inversion, then walk down: inv((i-1)!) = inv(i!) * i.
struct Binomial {
    ll p;
    std::vector<ll> fact, invFact;

    Binomial(int n, ll prime) : p(prime), fact(n + 1), invFact(n + 1) {
        fact[0] = 1 % p;
        for (int i = 1; i <= n; ++i) fact[i] = mulMod(fact[i - 1], i, p);
        invFact[n] = powMod(fact[n], p - 2, p);
        for (int i = n; i >= 1; --i) invFact[i - 1] = mulMod(invFact[i], i, p);
    }

    ll choose(int n, int k) const {
        if (k < 0 || k > n) return 0;
        return mulMod(fact[n], mulMod(invFact[k], invFact[n - k], p), p);
    }
};

// ALL solutions of a*x = b (mod m): exactly gcd(a,m) of them when it divides b, else
// none.  Returning just one is the classic half-right answer.
// O(log m + g)
std::vector<ll> solveLinearCongruence(ll a, ll b, ll m) {
    std::vector<ll> out;
    if (m <= 0) return out;
    a = norm(a, m);
    b = norm(b, m);
    ll x, y;
    ll g = extgcd(a, m, x, y);
    if (b % g != 0) return out;
    ll step = m / g;
    ll first = norm(mulMod(norm(x, m), (b / g) % m, m), step);
    for (ll k = 0; k < g; ++k) out.push_back(first + k * step);
    return out;
}

// ---------------------------------------------------------------- demo

int main() {
    const ll P = 1000000007LL;

    std::cout << "what an inverse is\n";
    std::cout << "  3^-1 mod 11 = " << modInverse(3, 11) << "\tcheck 3 x that = "
              << 3 * modInverse(3, 11) % 11 << "\n";
    std::cout << "  5^-1 mod 12 = " << modInverse(5, 12) << "\tcheck 5 x that = "
              << 5 * modInverse(5, 12) % 12 << "\n";
    std::cout << "  2^-1 mod 6  = " << modInverse(2, 6) << "\t[-1: gcd(2,6)=2, none exists]\n";

    std::cout << "\ndividing under a modulus\n";
    std::cout << "  7 / 3 mod 11 = " << divideMod(7, 3, 11) << "\tcheck 3 x that = "
              << 3 * divideMod(7, 3, 11) % 11 << "\n";
    std::cout << "  the wrong way, (7/3) % 11 = " << (7 / 3) % 11
              << "\t<- discards the remainder\n";

    std::cout << "\nFermat needs a prime modulus\n";
    std::cout << "  mod 1e9+7: extgcd " << modInverse(3, P) << ", Fermat "
              << modInversePrime(3, P) << "\t[same]\n";
    std::cout << "  mod 12:    extgcd " << modInverse(5, 12) << ", Fermat "
              << powMod(5, 10, 12) << "\tcheck 5 x " << powMod(5, 10, 12) << " = "
              << 5 * powMod(5, 10, 12) % 12 << " -- should be 1, so WRONG\n";

    Binomial C(200000, P);
    std::cout << "\nbinomials mod p\n";
    std::cout << "  C(5,2) = " << C.choose(5, 2) << "   C(10,5) = " << C.choose(10, 5) << "\n";
    std::cout << "  C(200000,100000) mod 1e9+7 = " << C.choose(200000, 100000)
              << "\t(~60000 digits, O(1) per query)\n";

    std::cout << "\na*x = b (mod m), all solutions\n";
    const ll cases[][3] = {{3, 6, 9}, {4, 8, 12}, {2, 3, 4}};
    for (const auto &c : cases) {
        std::cout << "  " << c[0] << "x = " << c[1] << " (mod " << c[2] << "):  ";
        auto s = solveLinearCongruence(c[0], c[1], c[2]);
        if (s.empty()) std::cout << "no solution";
        for (ll v : s) std::cout << v << " ";
        std::cout << "\n";
    }

    bool invOk = true, tabOk = true, binOk = true, congOk = true;
    for (ll m = 2; m <= 60; ++m)
        for (ll a = 0; a < m; ++a) {
            ll r = modInverse(a, m);
            bool exists = false;
            for (ll x = 0; x < m; ++x) if (a * x % m == 1) exists = true;
            if ((r >= 0) != exists || (r >= 0 && a * r % m != 1)) invOk = false;
        }
    std::vector<ll> inv = inverseTable(5000, P);
    for (int i = 1; i <= 5000; ++i)
        if (mulMod(inv[i], i, P) != 1) tabOk = false;
    for (int n = 0; n <= 25; ++n) {
        ll row = 1;
        for (int k = 0; k <= n; ++k) {
            if (C.choose(n, k) != row % P) binOk = false;
            row = row * (n - k) / (k + 1);
        }
    }
    for (ll m = 1; m <= 40; ++m)
        for (ll a = 0; a < m; ++a)
            for (ll b = 0; b < m; ++b) {
                std::vector<ll> want;
                for (ll x = 0; x < m; ++x) if (norm(a * x - b, m) == 0) want.push_back(x);
                if (solveLinearCongruence(a, b, m) != want) congOk = false;
            }
    std::cout << "\nself-check: inverse vs brute force " << (invOk ? "ok" : "FAIL")
              << ", O(n) table " << (tabOk ? "ok" : "FAIL")
              << ", binomials vs Pascal " << (binOk ? "ok" : "FAIL")
              << ", congruences vs exhaustive " << (congOk ? "ok" : "FAIL") << "\n";
}
