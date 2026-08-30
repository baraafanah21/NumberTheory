// Primitive Roots and Discrete Logarithm
// C++17.  Compile: g++ -std=c++17 -O2 -o primitiveroots implementation.cpp
//
// Three routines: multiplicative order, finding a primitive root, and baby-step
// giant-step for the discrete logarithm.

#include <cmath>
#include <iostream>
#include <numeric>
#include <unordered_map>
#include <vector>

using ll = long long;

ll mulMod(ll a, ll b, ll mod) { return (ll)((__int128)a * b % mod); }
ll norm(ll x, ll m) { x %= m; return x < 0 ? x + m : x; }

ll powerMod(ll base, ll exp, ll mod) {
    ll result = 1 % mod;
    base = norm(base, mod);
    while (exp > 0) {
        if (exp & 1) result = mulMod(result, base, mod);
        base = mulMod(base, base, mod);
        exp >>= 1;
    }
    return result;
}

// The distinct primes dividing n (exponents are irrelevant for everything below).
// O(sqrt n)
std::vector<ll> distinctPrimeFactors(ll n) {
    std::vector<ll> primes;
    for (ll p = 2; p <= n / p; ++p)
        if (n % p == 0) {
            primes.push_back(p);
            while (n % p == 0) n /= p;
        }
    if (n > 1) primes.push_back(n);
    return primes;
}

// phi(n), from the product formula.   O(sqrt n)
ll phi(ll n) {
    ll result = n;
    for (ll p : distinctPrimeFactors(n)) result -= result / p;
    return result;
}

// ---------------------------------------------------------------------------
// 1. Multiplicative order
// ---------------------------------------------------------------------------

// The smallest k >= 1 with a^k = 1 (mod m). Returns -1 when gcd(a,m) != 1, where no
// such k exists at all.
//
// The order always DIVIDES phi(m), so we never search: start at phi(m), which is
// guaranteed to work, and strip prime factors out for as long as it still works.
// O(sqrt m) to factor, then O(log^2 m)
ll multiplicativeOrder(ll a, ll m) {
    if (m <= 0 || std::gcd(norm(a, m), m) != 1) return -1;
    ll order = phi(m);
    for (ll q : distinctPrimeFactors(order))
        while (order % q == 0 && powerMod(a, order / q, m) == 1)
            order /= q;
    return order;
}

// ---------------------------------------------------------------------------
// 2. Primitive root
// ---------------------------------------------------------------------------

// The smallest primitive root modulo a PRIME p, or -1 if p is not usable.
//
// g is a primitive root iff its order is p-1. Testing every exponent below p-1 would
// cost O(p); it is enough to rule out the maximal proper divisors (p-1)/q, one per
// prime q dividing p-1. That is a handful of exponentiations.
//
// The answer is nearly always small -- for p = 1e9+7 it is 5.
// O(sqrt p) to factor p-1, then a few O(log p) tests per candidate
ll findPrimitiveRoot(ll p) {
    if (p == 2) return 1;
    if (p < 2) return -1;
    ll target = p - 1;
    std::vector<ll> qs = distinctPrimeFactors(target);
    for (ll g = 2; g < p; ++g) {
        bool ok = true;
        for (ll q : qs)
            if (powerMod(g, target / q, p) == 1) { ok = false; break; }
        if (ok) return g;
    }
    return -1;
}

// Does a primitive root exist mod m? Only for m = 1, 2, 4, p^k, 2p^k with p an odd prime.
// (Classification cited, not proved -- see proofs.md section 3.)
bool hasPrimitiveRoot(ll m) {
    if (m == 1 || m == 2 || m == 4) return true;
    if (m % 2 == 0) m /= 2;
    if (m % 2 == 0) return false;                  // divisible by 4 and bigger than 4
    std::vector<ll> ps = distinctPrimeFactors(m);
    return ps.size() == 1;                          // an odd prime power
}

// ---------------------------------------------------------------------------
// 3. Discrete logarithm -- baby-step giant-step
// ---------------------------------------------------------------------------

// The SMALLEST x >= 0 with g^x = b (mod m), or -1 if there is none.
// Precondition: gcd(g, m) == 1.
//
// Write x = i*n - j with n = ceil(sqrt(m)), 1 <= i <= n, 0 <= j < n. Then
//     g^(i*n - j) = b   <=>   (g^n)^i = b * g^j
// and the two sides depend on separate variables, so we tabulate one and look up the
// other -- meet in the middle.
//
// Storing the LARGEST j per key (later writes overwrite) and scanning i ascending is
// what makes the first hit the smallest x, not just some x.
// O(sqrt m) time and O(sqrt m) memory
ll discreteLog(ll g, ll b, ll m) {
    if (m <= 0) return -1;
    g = norm(g, m);
    b = norm(b, m);
    if (m == 1) return 0;
    if (std::gcd(g, m) != 1) return -1;
    if (b == 1 % m) return 0;

    ll n = (ll)std::sqrt((double)m) + 1;
    while (n * n < m) ++n;                          // n = ceil(sqrt(m)), exactly

    std::unordered_map<ll, ll> babySteps;
    ll value = b;
    for (ll j = 0; j < n; ++j) {
        babySteps[value] = j;                       // overwrite: keep the largest j
        value = mulMod(value, g, m);
    }

    ll giantStride = powerMod(g, n, m);
    ll current = 1 % m;
    for (ll i = 1; i <= n; ++i) {
        current = mulMod(current, giantStride, m);  // (g^n)^i
        auto it = babySteps.find(current);
        if (it != babySteps.end()) {
            ll x = i * n - it->second;
            if (x >= 0) return x;
        }
    }
    return -1;
}

// ---------------------------------------------------------------------------
// Demo
// ---------------------------------------------------------------------------

int main() {
    std::cout << "--- order: how soon a^k returns to 1 ---\n";
    std::cout << "modulo 7 (phi = 6):\n";
    for (ll a = 1; a <= 6; ++a) {
        std::cout << "  a = " << a << ":  ";
        ll v = 1;
        for (int k = 1; k <= 6; ++k) { v = v * a % 7; std::cout << v << " "; if (v == 1) break; }
        std::cout << "  order = " << multiplicativeOrder(a, 7) << "\n";
    }
    std::cout << "  every order divides 6, as it must\n";
    std::cout << "order of 2 mod 8 = " << multiplicativeOrder(2, 8)
              << "   (-1: gcd(2,8) != 1, so 2^k never reaches 1)\n";

    std::cout << "\n--- primitive roots ---\n";
    for (ll p : {2, 3, 7, 11, 101}) {
        ll g = findPrimitiveRoot(p);
        std::cout << "  smallest primitive root mod " << p << " = " << g
                  << "   (there are phi(p-1) = " << phi(p - 1) << " of them)\n";
    }
    std::cout << "  mod 1000000007: " << findPrimitiveRoot(1000000007LL) << "\n";

    std::cout << "\n  a primitive root generates everything -- g = 3 mod 7:\n    ";
    for (int k = 0; k < 6; ++k) std::cout << powerMod(3, k, 7) << " ";
    std::cout << "  <- all six nonzero residues\n";

    std::cout << "\n  not every modulus has one: ";
    for (ll m : {8, 12, 15, 16}) std::cout << m << "->" << (hasPrimitiveRoot(m) ? "yes" : "no") << " ";
    std::cout << "\n";

    std::cout << "\n--- discrete logarithm ---\n";
    std::cout << "  solve 3^x = 5 (mod 7):  x = " << discreteLog(3, 5, 7)
              << "   check 3^" << discreteLog(3, 5, 7) << " mod 7 = "
              << powerMod(3, discreteLog(3, 5, 7), 7) << "\n";
    std::cout << "  solve 2^x = 3 (mod 5):  x = " << discreteLog(2, 3, 5) << "\n";
    std::cout << "  solve 2^x = 3 (mod 7):  x = " << discreteLog(2, 3, 7)
              << "   (-1: 2 has order 3 mod 7, so its powers are only {1,2,4})\n";

    ll bigMod = 1000000007LL, gg = 5;
    ll secret = 123456789LL;
    ll target = powerMod(gg, secret, bigMod);
    ll recovered = discreteLog(gg, target, bigMod);
    std::cout << "  5^x = " << target << " (mod 1e9+7):  x = " << recovered << "\n";
    std::cout << "    check: 5^" << recovered << " mod 1e9+7 = "
              << powerMod(gg, recovered, bigMod) << "\n";
    std::cout << "    (computing 5^x took ~30 multiplications; recovering x took ~31623)\n";

    // ---- self-checks ----
    bool orderOk = true;
    for (ll m = 2; m <= 200; ++m)
        for (ll a = 1; a < m; ++a) {
            if (std::gcd(a, m) != 1) { if (multiplicativeOrder(a, m) != -1) orderOk = false; continue; }
            ll want = 0, v = 1;
            for (ll k = 1; k <= m; ++k) { v = v * a % m; if (v == 1) { want = k; break; } }
            if (multiplicativeOrder(a, m) != want) orderOk = false;
        }

    bool rootOk = true;
    for (ll p : {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 97, 101, 997}) {
        ll g = findPrimitiveRoot(p);
        if (multiplicativeOrder(g, p) != p - 1) rootOk = false;
        for (ll c = 2; c < g; ++c)                       // it must be the SMALLEST
            if (multiplicativeOrder(c, p) == p - 1) rootOk = false;
    }

    bool countOk = true;
    for (ll p : {7, 11, 13, 17, 19, 23, 31, 101}) {
        ll found = 0;
        for (ll a = 1; a < p; ++a) if (multiplicativeOrder(a, p) == p - 1) ++found;
        if (found != phi(p - 1)) countOk = false;
    }

    bool logOk = true;
    for (ll m = 2; m <= 120; ++m)
        for (ll g = 1; g < m; ++g) {
            if (std::gcd(g, m) != 1) continue;
            for (ll b = 0; b < m; ++b) {
                ll got = discreteLog(g, b, m);
                ll want = -1;
                for (ll x = 0; x < m + 2; ++x)
                    if (powerMod(g, x, m) == b) { want = x; break; }
                if (got != want) logOk = false;          // also checks it is the SMALLEST
            }
        }

    std::cout << "\nself-check: order vs brute force " << (orderOk ? "ok" : "FAIL")
              << ", primitive root is smallest and correct " << (rootOk ? "ok" : "FAIL")
              << ", count == phi(p-1) " << (countOk ? "ok" : "FAIL")
              << ", discrete log finds smallest x " << (logOk ? "ok" : "FAIL") << "\n";
    return 0;
}
