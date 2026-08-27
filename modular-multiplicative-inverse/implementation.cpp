// Modular Multiplicative Inverse
// C++17.  Compile: g++ -std=c++17 -O2 -o modinverse implementation.cpp
//
// Four ways to get an inverse, plus the two things you use them for:
// dividing under a modulus, and binomial coefficients mod p.

#include <iostream>
#include <vector>

using ll = long long;

// (a * b) % mod, exact for any mod below 2^63. __int128 holds the product first.
ll mulMod(ll a, ll b, ll mod) { return (ll)((__int128)a * b % mod); }

// Least non-negative residue: '%' can return a negative value in C++.
ll norm(ll x, ll m) { x %= m; return x < 0 ? x + m : x; }

// From the extended Euclid concept: returns g = gcd(a,b) with a*x + b*y == g.
ll extgcd(ll a, ll b, ll &x, ll &y) {
    if (b == 0) { x = 1; y = 0; return a; }
    ll x1, y1;
    ll g = extgcd(b, a % b, x1, y1);
    x = y1;
    y = x1 - (a / b) * y1;
    return g;
}

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

// ---------------------------------------------------------------------------
// Method 1: extended Euclid -- works for ANY modulus
// ---------------------------------------------------------------------------

// a^-1 mod m, or -1 when no inverse exists.
//
// extgcd(a, m) gives a*x + m*y = g. If g == 1, reducing mod m kills the m*y term and
// leaves a*x = 1, so x is the inverse. If g != 1 there is no inverse at all.
//
// Both normalizations are needed: the input may be negative, and extgcd returns a
// signed coefficient.
// O(log m)
ll modInverse(ll a, ll m) {
    if (m <= 1) return -1;
    ll x, y;
    ll g = extgcd(norm(a, m), m, x, y);
    if (g != 1) return -1;
    return norm(x, m);
}

// ---------------------------------------------------------------------------
// Method 2: Fermat -- PRIME modulus only
// ---------------------------------------------------------------------------

// a^-1 = a^(p-2) mod p, because a^(p-1) = 1 when p is prime and p does not divide a.
//
// On a composite modulus this does NOT fail -- it returns a wrong number with no
// warning. Use modInverse above unless you know the modulus is prime.
// O(log p)
ll modInversePrime(ll a, ll p) { return powerMod(a, p - 2, p); }

// ---------------------------------------------------------------------------
// Method 3: every inverse from 1 to n at once -- O(n), prime modulus
// ---------------------------------------------------------------------------

// Writing p = q*i + r gives q*i + r = 0 (mod p), and multiplying by inv(i)*inv(r)
// rearranges to inv(i) = -q * inv(r). Since r < i, inv(r) is already known.
// Writing -q as (p - q) keeps everything non-negative.
//
// Requires p prime and n < p.
// O(n) time, O(n) space.
std::vector<ll> inverseTable(int n, ll p) {
    std::vector<ll> inv(n + 1, 0);
    if (n >= 1) inv[1] = 1;
    for (int i = 2; i <= n; ++i)
        inv[i] = (p - p / i) * inv[p % i] % p;
    return inv;
}

// ---------------------------------------------------------------------------
// Use 1: dividing under a modulus
// ---------------------------------------------------------------------------

// (a / b) mod m, done correctly. Returns -1 when b has no inverse.
// Never write (a / b) % m: integer division discards the remainder and the answer
// is simply wrong.
ll divideMod(ll a, ll b, ll m) {
    ll invB = modInverse(b, m);
    if (invB < 0) return -1;
    return mulMod(norm(a, m), invB, m);
}

// ---------------------------------------------------------------------------
// Use 2: binomial coefficients mod a prime
// ---------------------------------------------------------------------------

// C(n, k) = n! / (k! (n-k)!), so mod p it is n! * inv(k!) * inv((n-k)!).
// Precompute factorials and their inverses once, then every query is O(1).
struct Binomial {
    ll p;
    std::vector<ll> fact, invFact;

    Binomial(int n, ll prime) : p(prime), fact(n + 1), invFact(n + 1) {
        fact[0] = 1 % p;
        for (int i = 1; i <= n; ++i) fact[i] = mulMod(fact[i - 1], i, p);

        // One inversion, then walk down: inv((i-1)!) = inv(i!) * i.
        invFact[n] = powerMod(fact[n], p - 2, p);
        for (int i = n; i >= 1; --i) invFact[i - 1] = mulMod(invFact[i], i, p);
    }

    ll choose(int n, int k) const {
        if (k < 0 || k > n) return 0;
        return mulMod(fact[n], mulMod(invFact[k], invFact[n - k], p), p);
    }
};

// ---------------------------------------------------------------------------
// Use 3: a*x = b (mod m) when a is not invertible
// ---------------------------------------------------------------------------

// ALL solutions mod m. There are exactly g = gcd(a, m) of them when g divides b,
// and none otherwise. Returning just one is the classic half-right answer.
// O(log m + g)
std::vector<ll> solveLinearCongruence(ll a, ll b, ll m) {
    std::vector<ll> answers;
    if (m <= 0) return answers;
    a = norm(a, m);
    b = norm(b, m);
    ll x, y;
    ll g = extgcd(a, m, x, y);
    if (b % g != 0) return answers;                 // unsolvable

    ll step = m / g;
    ll first = norm(mulMod(norm(x, m), (b / g) % m, m), step);
    for (ll k = 0; k < g; ++k) answers.push_back(first + k * step);
    return answers;
}

// ---------------------------------------------------------------------------
// Demo
// ---------------------------------------------------------------------------

int main() {
    const ll P = 1000000007LL;

    std::cout << "--- what an inverse is ---\n";
    std::cout << "3^-1 mod 11 = " << modInverse(3, 11) << "    check: 3 x "
              << modInverse(3, 11) << " = " << 3 * modInverse(3, 11) % 11 << " mod 11\n";
    std::cout << "5^-1 mod 12 = " << modInverse(5, 12) << "    check: 5 x "
              << modInverse(5, 12) << " = " << 5 * modInverse(5, 12) % 12 << " mod 12\n";
    std::cout << "2^-1 mod 6  = " << modInverse(2, 6)
              << "   (-1: gcd(2,6) = 2, so no inverse exists)\n";

    std::cout << "\n--- dividing under a modulus ---\n";
    std::cout << "7 / 3 mod 11 = " << divideMod(7, 3, 11) << "    check: 3 x "
              << divideMod(7, 3, 11) << " = " << 3 * divideMod(7, 3, 11) % 11
              << " mod 11, which is 7\n";
    std::cout << "the wrong way, (7 / 3) % 11 = " << (7 / 3) % 11 << "  <- discards the remainder\n";

    std::cout << "\n--- Fermat works only for prime moduli ---\n";
    std::cout << "mod 1e9+7 (prime):  extgcd gives 3^-1 = " << modInverse(3, P)
              << ", Fermat gives " << modInversePrime(3, P) << "   (same)\n";
    std::cout << "mod 12 (composite): extgcd gives 5^-1 = " << modInverse(5, 12)
              << ", Fermat gives " << powerMod(5, 10, 12) << "\n";
    std::cout << "  check Fermat's answer: 5 x " << powerMod(5, 10, 12) << " = "
              << 5 * powerMod(5, 10, 12) % 12 << " mod 12 -- should be 1, so it is WRONG\n";

    std::cout << "\n--- binomial coefficients mod p ---\n";
    Binomial C(200000, P);
    std::cout << "C(5, 2)          = " << C.choose(5, 2) << "   [10]\n";
    std::cout << "C(10, 5)         = " << C.choose(10, 5) << "   [252]\n";
    std::cout << "C(200000, 100000) mod 1e9+7 = " << C.choose(200000, 100000)
              << "   (a number with ~60000 digits, in O(1) per query)\n";

    std::cout << "\n--- a*x = b (mod m), all solutions ---\n";
    const ll cases[][3] = {{3, 6, 9}, {4, 8, 12}, {2, 3, 4}};
    for (const auto &c : cases) {
        std::vector<ll> sol = solveLinearCongruence(c[0], c[1], c[2]);
        std::cout << c[0] << "x = " << c[1] << " (mod " << c[2] << "):  ";
        if (sol.empty()) std::cout << "no solution";
        else for (ll s : sol) std::cout << s << " ";
        std::cout << "\n";
    }

    // ---- self-checks ----
    bool invOk = true;
    for (ll m = 2; m <= 60; ++m)
        for (ll a = 0; a < m; ++a) {
            ll r = modInverse(a, m);
            bool shouldExist = false;
            for (ll x = 0; x < m; ++x) if (a * x % m == 1) shouldExist = true;
            if ((r >= 0) != shouldExist) invOk = false;
            if (r >= 0 && a * r % m != 1) invOk = false;
        }

    bool tableOk = true;
    std::vector<ll> inv = inverseTable(5000, P);
    for (int i = 1; i <= 5000; ++i)
        if (mulMod(inv[i], i, P) != 1) tableOk = false;

    bool binomOk = true;
    for (int n = 0; n <= 25; ++n) {
        ll row = 1;                                  // Pascal's triangle, computed plainly
        for (int k = 0; k <= n; ++k) {
            if (C.choose(n, k) != row % P) binomOk = false;
            row = row * (n - k) / (k + 1);
        }
    }

    bool congOk = true;
    for (ll m = 1; m <= 40; ++m)
        for (ll a = 0; a < m; ++a)
            for (ll b = 0; b < m; ++b) {
                std::vector<ll> got = solveLinearCongruence(a, b, m);
                std::vector<ll> want;
                for (ll x = 0; x < m; ++x) if (norm(a * x - b, m) == 0) want.push_back(x);
                if (got != want) congOk = false;
            }

    std::cout << "\nself-check: inverse existence and value vs brute force "
              << (invOk ? "ok" : "FAIL") << ", O(n) table " << (tableOk ? "ok" : "FAIL")
              << ", binomials vs Pascal " << (binomOk ? "ok" : "FAIL")
              << ", congruences vs exhaustive " << (congOk ? "ok" : "FAIL") << "\n";
    return 0;
}
