// Primitive Roots and Discrete Logarithm -- g++ -std=c++17 -O2 implementation.cpp

#include <cmath>
#include <iostream>
#include <numeric>
#include <unordered_map>
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

// The distinct primes dividing n -- exponents are irrelevant to everything below.
// O(sqrt n)
std::vector<ll> distinctPrimeFactors(ll n) {
    std::vector<ll> ps;
    for (ll p = 2; p <= n / p; ++p)
        if (n % p == 0) {
            ps.push_back(p);
            while (n % p == 0) n /= p;
        }
    if (n > 1) ps.push_back(n);
    return ps;
}

ll phi(ll n) {
    ll r = n;
    for (ll p : distinctPrimeFactors(n)) r -= r / p;
    return r;
}

// ---------------------------------------------------------------- order

// Smallest k >= 1 with a^k = 1 (mod m); -1 when gcd(a,m) != 1, where no such k exists.
//
// The order always DIVIDES phi(m), so we never search: start at phi(m), which is
// guaranteed to work, and strip prime factors while it still works.
// O(sqrt m) to factor, then O(log^2 m)
ll multiplicativeOrder(ll a, ll m) {
    if (m <= 0 || std::gcd(norm(a, m), m) != 1) return -1;
    ll order = phi(m);
    for (ll q : distinctPrimeFactors(order))
        while (order % q == 0 && powMod(a, order / q, m) == 1)
            order /= q;
    return order;
}

// ---------------------------------------------------------------- primitive root

// The smallest primitive root modulo a PRIME p.
//
// g qualifies iff its order is p-1.  Testing every exponent below p-1 costs O(p); it is
// enough to rule out the maximal proper divisors (p-1)/q, one per prime q dividing p-1.
// The answer is nearly always small -- for p = 1e9+7 it is 5.
// O(sqrt p) to factor p-1, then a few O(log p) tests per candidate
ll findPrimitiveRoot(ll p) {
    if (p == 2) return 1;
    if (p < 2) return -1;
    std::vector<ll> qs = distinctPrimeFactors(p - 1);
    for (ll g = 2; g < p; ++g) {
        bool ok = true;
        for (ll q : qs)
            if (powMod(g, (p - 1) / q, p) == 1) { ok = false; break; }
        if (ok) return g;
    }
    return -1;
}

// Does a primitive root exist mod m?  Only for m = 1, 2, 4, p^k, 2p^k with p an odd
// prime.  (Classification cited, not proved -- see proofs.md section 3.)
bool hasPrimitiveRoot(ll m) {
    if (m == 1 || m == 2 || m == 4) return true;
    if (m % 2 == 0) m /= 2;
    if (m % 2 == 0) return false;                   // was divisible by 4 and > 4
    return distinctPrimeFactors(m).size() == 1;     // an odd prime power
}

// ---------------------------------------------------------------- discrete log

// The SMALLEST x >= 0 with g^x = b (mod m), or -1.  Precondition: gcd(g,m) == 1.
//
// Write x = i*n - j with n = ceil(sqrt m), 1 <= i <= n, 0 <= j < n.  Then
//     g^(i*n - j) = b   <=>   (g^n)^i = b * g^j
// and each side involves one variable, so we tabulate one and look up the other.
//
// Storing the LARGEST j per key (later writes overwrite) and scanning i ascending is
// what makes the first hit the smallest x, not just some x.
// O(sqrt m) time and memory
ll discreteLog(ll g, ll b, ll m) {
    if (m <= 0) return -1;
    g = norm(g, m);
    b = norm(b, m);
    if (m == 1) return 0;
    if (std::gcd(g, m) != 1) return -1;
    if (b == 1 % m) return 0;

    ll n = (ll)std::sqrt((double)m) + 1;
    while (n * n < m) ++n;                          // exactly ceil(sqrt m)

    std::unordered_map<ll, ll> baby;
    baby.reserve(n * 2);
    ll v = b;
    for (ll j = 0; j < n; ++j, v = mulMod(v, g, m))
        baby[v] = j;                                // overwrite: keep the largest j

    ll stride = powMod(g, n, m), cur = 1 % m;
    for (ll i = 1; i <= n; ++i) {
        cur = mulMod(cur, stride, m);               // (g^n)^i
        auto it = baby.find(cur);
        if (it != baby.end()) return i * n - it->second;
    }
    return -1;
}

// ---------------------------------------------------------------- demo

int main() {
    std::cout << "order -- how soon a^k returns to 1, modulo 7 (phi = 6)\n";
    for (ll a = 1; a <= 6; ++a) {
        std::cout << "  a = " << a << ":  ";
        ll v = 1;
        for (int k = 1; k <= 6; ++k) { v = v * a % 7; std::cout << v << " "; if (v == 1) break; }
        std::cout << "\torder " << multiplicativeOrder(a, 7) << "\n";
    }
    std::cout << "  every order divides 6, as it must\n";
    std::cout << "  order of 2 mod 8 = " << multiplicativeOrder(2, 8)
              << "\t[-1: gcd(2,8)!=1, so 2^k never reaches 1]\n";

    std::cout << "\nprimitive roots\n";
    for (ll p : {2, 3, 7, 11, 101})
        std::cout << "  mod " << p << ": smallest is " << findPrimitiveRoot(p)
                  << ", and there are phi(p-1) = " << phi(p - 1) << " of them\n";
    std::cout << "  mod 1000000007: " << findPrimitiveRoot(1000000007LL) << "\n";
    std::cout << "  g = 3 generates everything mod 7:  ";
    for (int k = 0; k < 6; ++k) std::cout << powMod(3, k, 7) << " ";
    std::cout << "\n  not every modulus has one: ";
    for (ll m : {8, 12, 15, 16}) std::cout << m << "->" << (hasPrimitiveRoot(m) ? "yes " : "no ");
    std::cout << "\n";

    std::cout << "\ndiscrete logarithm\n";
    std::cout << "  3^x = 5 (mod 7):  x = " << discreteLog(3, 5, 7) << "\tcheck 3^x = "
              << powMod(3, discreteLog(3, 5, 7), 7) << "\n";
    std::cout << "  2^x = 3 (mod 7):  x = " << discreteLog(2, 3, 7)
              << "\t[-1: 2 has order 3, so its powers are only {1,2,4}]\n";

    const ll M = 1000000007LL, G = 5, secret = 123456789LL;
    ll target = powMod(G, secret, M), recovered = discreteLog(G, target, M);
    std::cout << "  5^x = " << target << " (mod 1e9+7):  x = " << recovered << "\n";
    std::cout << "  computing 5^x took ~30 multiplications; recovering x took ~31623\n";

    bool orderOk = true, rootOk = true, countOk = true, logOk = true;
    for (ll m = 2; m <= 200; ++m)
        for (ll a = 1; a < m; ++a) {
            if (std::gcd(a, m) != 1) {
                if (multiplicativeOrder(a, m) != -1) orderOk = false;
                continue;
            }
            ll want = 0, v = 1;
            for (ll k = 1; k <= m; ++k) { v = v * a % m; if (v == 1) { want = k; break; } }
            if (multiplicativeOrder(a, m) != want) orderOk = false;
        }
    for (ll p : {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 97, 101, 997}) {
        ll g = findPrimitiveRoot(p);
        if (multiplicativeOrder(g, p) != p - 1) rootOk = false;
        for (ll c = 2; c < g; ++c)                        // it must be the SMALLEST
            if (multiplicativeOrder(c, p) == p - 1) rootOk = false;
    }
    for (ll p : {7, 11, 13, 17, 19, 23, 31, 101}) {
        ll found = 0;
        for (ll a = 1; a < p; ++a) if (multiplicativeOrder(a, p) == p - 1) ++found;
        if (found != phi(p - 1)) countOk = false;
    }
    for (ll m = 2; m <= 120; ++m)
        for (ll g = 1; g < m; ++g) {
            if (std::gcd(g, m) != 1) continue;
            for (ll b = 0; b < m; ++b) {
                ll want = -1;
                for (ll x = 0; x < m + 2; ++x) if (powMod(g, x, m) == b) { want = x; break; }
                if (discreteLog(g, b, m) != want) logOk = false;   // also checks minimality
            }
        }
    std::cout << "\nself-check: order vs brute force " << (orderOk ? "ok" : "FAIL")
              << ", primitive root smallest and correct " << (rootOk ? "ok" : "FAIL")
              << ", count == phi(p-1) " << (countOk ? "ok" : "FAIL")
              << ", discrete log finds smallest x " << (logOk ? "ok" : "FAIL") << "\n";
}
