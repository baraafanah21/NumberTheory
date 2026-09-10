// Euler's Totient Function -- g++ -std=c++17 -O2 implementation.cpp

#include <iostream>
#include <numeric>
#include <string>
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

// ---------------------------------------------------------------- one value

// phi(n) = n * product of (1 - 1/p) over the distinct primes dividing n.
//
// Two details that are easy to get wrong:
//   - `while (n % p == 0) n /= p` strips the whole power, so each prime contributes its
//     factor ONCE (the exponent is already inside the leading n)
//   - `r -= r / p` applies (1 - 1/p) in integers.  r * (1 - 1/p) fails because 1/p
//     truncates to 0; the division here is exact, since p divides r at that moment.
// O(sqrt n)
ll phi(ll n) {
    if (n <= 0) return 0;
    ll r = n;
    for (ll p = 2; p <= n / p; ++p)
        if (n % p == 0) {
            while (n % p == 0) n /= p;
            r -= r / p;
        }
    if (n > 1) r -= r / n;          // one prime factor above sqrt is left over
    return r;
}

// ---------------------------------------------------------------- every value

// phi for every m in [0, n].  Same skeleton as the sieve: start each entry at itself,
// then let each prime scale down its own multiples.
// `phi[p] == p` doubles as a primality test -- nothing has scaled p down yet.
// O(n log log n) time, O(n) space.
std::vector<int> phiSieve(int n) {
    std::vector<int> phi(n + 1);
    for (int i = 0; i <= n; ++i) phi[i] = i;
    for (int p = 2; p <= n; ++p)
        if (phi[p] == p)
            for (int m = p; m <= n; m += p)
                phi[m] -= phi[m] / p;
    return phi;
}

// ---------------------------------------------------------------- uses

// a^-1 = a^(phi(m) - 1) mod m, valid whenever gcd(a,m) == 1.
//
// Generalizes the Fermat shortcut, which needed a prime modulus: there phi(p) = p-1, so
// this is exactly a^(p-2).  Costs a factorization to get phi(m), so extended Euclid is
// faster for a one-off inverse.
// Returns -1 when no inverse exists.  O(sqrt m + log m)
ll modInverseEuler(ll a, ll m) {
    if (m <= 1) return -1;
    a = norm(a, m);
    if (std::gcd(a, m) != 1) return -1;
    return powMod(a, phi(m) - 1, m);
}

// a^k mod m, with k given as a decimal STRING so it may have thousands of digits.
//
// Euler lets us reduce k modulo phi(m), but the plain reduction a^(k mod phi(m)) is only
// valid when gcd(a,m) == 1.  The safe form below holds for ALL a once k >= log2(m):
//     a^k = a^((k mod phi(m)) + phi(m))
// so we always add phi(m) back.  Costs nothing and removes the trap.
// Counterexample for the naive version: a=2, m=4, k=4 gives 2^4 = 0 mod 4, while
// 2^(4 mod phi(4)) = 2^0 = 1.
// O(len(k) + sqrt m + log m)
ll powHugeExponent(ll a, const std::string &k, ll m) {
    if (m == 1) return 0;
    ll ph = phi(m), reduced = 0;
    bool exceeds = false;
    for (char c : k) {
        reduced = reduced * 10 + (c - '0');
        if (reduced >= ph) { exceeds = true; reduced %= ph; }
    }
    if (exceeds) reduced += ph;
    return powMod(a, reduced, m);
}

// ---------------------------------------------------------------- demo

int main() {
    std::cout << "what phi counts\n";
    for (int n : {1, 6, 7, 9, 12}) {
        std::cout << "  phi(" << n << ") = " << phi(n) << "\tcoprime to it: ";
        for (int k = 1; k <= n; ++k) if (std::gcd(k, n) == 1) std::cout << k << " ";
        std::cout << "\n";
    }

    std::cout << "\nthe product formula\n";
    std::cout << "  phi(12) = 12(1-1/2)(1-1/3) = " << phi(12) << "\n";
    std::cout << "  phi(1e9+7) = " << phi(1000000007LL) << "\t[prime, so p-1]\n";
    std::cout << "  phi(2^20)  = " << phi(1 << 20) << "\t[= 2^20 - 2^19]\n";

    std::cout << "\nmultiplicative -- but only for coprime arguments\n";
    std::cout << "  phi(3)phi(4) = " << phi(3) * phi(4) << ", phi(12) = " << phi(12)
              << "\t[gcd(3,4)=1, so they agree]\n";
    std::cout << "  phi(2)phi(2) = " << phi(2) * phi(2) << ", phi(4)  = " << phi(4)
              << "\t[gcd(2,2)=2, so the rule does NOT apply]\n";

    std::cout << "\nEuler generalizes Fermat\n  a^phi(10) mod 10: ";
    for (int a : {3, 7, 9}) std::cout << a << "->" << powMod(a, phi(10), 10) << " ";
    std::cout << "\t[all 1, and 10 is composite]\n";
    std::cout << "  3^-1 mod 10 = " << modInverseEuler(3, 10) << "\tcheck 3 x that = "
              << 3 * modInverseEuler(3, 10) % 10 << "\n";
    std::cout << "  2^-1 mod 10 = " << modInverseEuler(2, 10) << "\t[gcd(2,10)=2, none]\n";

    std::cout << "\nshrinking a huge exponent\n";
    std::cout << "  7^(999...9, 1000 digits) mod 1e9+7 = "
              << powHugeExponent(7, std::string(1000, '9'), 1000000007LL) << "\n";
    std::cout << "  the trap: 2^4 mod 4 = " << powMod(2, 4, 4)
              << ", naive reduction gives " << powMod(2, 0, 4)
              << ", our function gives " << powHugeExponent(2, "4", 4) << "\t[correct]\n";

    const int N = 200000;
    std::vector<int> table = phiSieve(N);
    bool bruteOk = true, sieveOk = true, sumOk = true, eulerOk = true, hugeOk = true;
    for (int n = 1; n <= 2000; ++n) {
        int want = 0;
        for (int k = 1; k <= n; ++k) if (std::gcd(k, n) == 1) ++want;
        if (table[n] != want || phi(n) != want) bruteOk = false;
    }
    for (int n = 1; n <= N; ++n) if (table[n] != phi(n)) sieveOk = false;
    for (int n = 1; n <= 3000; ++n) {
        ll s = 0;
        for (int d = 1; d <= n; ++d) if (n % d == 0) s += table[d];
        if (s != n) sumOk = false;
    }
    for (int m = 2; m <= 300; ++m)
        for (int a = 1; a < m; ++a)
            if (std::gcd(a, m) == 1 && powMod(a, table[m], m) != 1) eulerOk = false;
    for (int m = 2; m <= 60; ++m)
        for (int a = 0; a < m; ++a)
            for (int k = 0; k <= 40; ++k)
                if (powHugeExponent(a, std::to_string(k), m) != powMod(a, k, m)) hugeOk = false;

    std::cout << "\nself-check: phi vs counting " << (bruteOk ? "ok" : "FAIL")
              << ", sieve == single " << (sieveOk ? "ok" : "FAIL")
              << ", sum over divisors == n " << (sumOk ? "ok" : "FAIL")
              << ", Euler's theorem " << (eulerOk ? "ok" : "FAIL")
              << ", huge exponent incl. gcd!=1 " << (hugeOk ? "ok" : "FAIL") << "\n";
}
