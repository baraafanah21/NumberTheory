// Euler's Totient Function
// C++17.  Compile: g++ -std=c++17 -O2 -o totient implementation.cpp
//
// phi(n) counts the numbers in [1, n] coprime to n.
// Four routines: one value, all values, Euler's inverse, and exponent reduction.

#include <iostream>
#include <numeric>
#include <vector>

using ll = long long;

ll mulMod(ll a, ll b, ll mod) { return (ll)((__int128)a * b % mod); }

ll powerMod(ll base, ll exp, ll mod) {
    ll result = 1 % mod;
    base %= mod;
    if (base < 0) base += mod;
    while (exp > 0) {
        if (exp & 1) result = mulMod(result, base, mod);
        base = mulMod(base, base, mod);
        exp >>= 1;
    }
    return result;
}

// ---------------------------------------------------------------------------
// 1. phi of a single number -- O(sqrt n)
// ---------------------------------------------------------------------------

// Applies phi(n) = n * product over distinct primes p | n of (1 - 1/p), while
// trial-dividing to find those primes.
//
// Two details that are easy to get wrong:
//   - `while (n % p == 0) n /= p;` strips the whole power, so each prime contributes
//     its factor exactly ONCE (the exponent is already inside the leading n)
//   - `result -= result / p` is how you multiply by (1 - 1/p) in integers. Writing
//     result * (1 - 1/p) fails, because 1/p truncates to 0. The division is exact,
//     since p divides result at that moment.
// O(sqrt n)
ll phi(ll n) {
    if (n <= 0) return 0;
    ll result = n;
    for (ll p = 2; p <= n / p; ++p)
        if (n % p == 0) {
            while (n % p == 0) n /= p;
            result -= result / p;
        }
    if (n > 1) result -= result / n;    // one prime factor larger than sqrt is left over
    return result;
}

// ---------------------------------------------------------------------------
// 2. phi for every number up to n -- O(n log log n)
// ---------------------------------------------------------------------------

// Same skeleton as the sieve. Start each entry at itself, then let each prime scale
// down all of its multiples.
//
// The test `phi[p] == p` doubles as a primality test: nothing has scaled p down yet.
// O(n log log n) time, O(n) space.
std::vector<int> phiSieve(int n) {
    std::vector<int> phi(n + 1);
    for (int i = 0; i <= n; ++i) phi[i] = i;
    for (int p = 2; p <= n; ++p)
        if (phi[p] == p)                       // p is prime
            for (int m = p; m <= n; m += p)
                phi[m] -= phi[m] / p;
    return phi;
}

// ---------------------------------------------------------------------------
// 3. Euler's theorem: an inverse for ANY modulus
// ---------------------------------------------------------------------------

// a^-1 = a^(phi(m) - 1) mod m, valid whenever gcd(a, m) == 1.
//
// This generalizes the Fermat shortcut, which needed a prime modulus: there
// phi(p) = p - 1, so a^(phi(p)-1) is exactly a^(p-2).
//
// It does cost a factorization to get phi(m). Extended Euclid is faster for a one-off
// inverse; use this when phi(m) is already known.
// Returns -1 when no inverse exists.
// O(sqrt m + log m)
ll modInverseEuler(ll a, ll m) {
    if (m <= 1) return -1;
    a %= m;
    if (a < 0) a += m;
    if (std::gcd(a, m) != 1) return -1;
    return powerMod(a, phi(m) - 1, m);
}

// ---------------------------------------------------------------------------
// 4. Shrinking a huge exponent
// ---------------------------------------------------------------------------

// a^k mod m where k is given as a decimal STRING, so it may have thousands of digits.
//
// Euler's theorem lets us reduce k modulo phi(m). But the plain reduction
//     a^k = a^(k mod phi(m))
// is only valid when gcd(a, m) == 1. The safe form below holds for ALL a once
// k >= log2(m):
//     a^k = a^((k mod phi(m)) + phi(m))
// so we always add phi(m) back. That costs nothing and removes the trap entirely.
//
// Counterexample for the naive version: a=2, m=4, k=4 gives 2^4 = 0 mod 4, but
// 2^(4 mod phi(4)) = 2^0 = 1.
// O(len(k) + sqrt(m) + log m)
ll powerHugeExponent(ll a, const std::string &k, ll m) {
    if (m == 1) return 0;
    ll ph = phi(m);
    ll reduced = 0;
    bool exceeds = false;                       // did k already exceed phi(m)?
    for (char c : k) {
        reduced = reduced * 10 + (c - '0');
        if (reduced >= ph) { exceeds = true; reduced %= ph; }
    }
    if (exceeds) reduced += ph;                 // the safe +phi(m) correction
    return powerMod(a, reduced, m);
}

// ---------------------------------------------------------------------------
// Demo
// ---------------------------------------------------------------------------

int main() {
    std::cout << "--- what phi counts ---\n";
    for (int n : {1, 6, 7, 9, 12}) {
        std::cout << "phi(" << n << ") = " << phi(n) << "   coprime to it: ";
        for (int k = 1; k <= n; ++k) if (std::gcd(k, n) == 1) std::cout << k << " ";
        std::cout << "\n";
    }

    std::cout << "\n--- the product formula ---\n";
    std::cout << "phi(12) = 12 * (1 - 1/2) * (1 - 1/3) = " << phi(12) << "\n";
    std::cout << "phi(1000000007) = " << phi(1000000007LL) << "   (a prime, so p - 1)\n";
    std::cout << "phi(2^20) = " << phi(1 << 20) << "   (= 2^20 - 2^19)\n";

    std::cout << "\n--- multiplicative, but only for coprime arguments ---\n";
    std::cout << "phi(3)*phi(4) = " << phi(3) * phi(4) << " and phi(12) = " << phi(12)
              << "   (gcd(3,4)=1, so they agree)\n";
    std::cout << "phi(2)*phi(2) = " << phi(2) * phi(2) << " but phi(4) = " << phi(4)
              << "   (gcd(2,2)=2, so the rule does NOT apply)\n";

    std::cout << "\n--- Euler's theorem generalizes Fermat ---\n";
    std::cout << "a^phi(m) mod m, for m = 10 (composite): ";
    for (int a : {3, 7, 9}) std::cout << a << "->" << powerMod(a, phi(10), 10) << " ";
    std::cout << "  (all 1)\n";
    std::cout << "inverse of 3 mod 10 via Euler = " << modInverseEuler(3, 10)
              << "   check: 3 x " << modInverseEuler(3, 10) << " = "
              << 3 * modInverseEuler(3, 10) % 10 << " mod 10\n";
    std::cout << "inverse of 2 mod 10 = " << modInverseEuler(2, 10)
              << "   (-1: gcd(2,10)=2, no inverse)\n";

    std::cout << "\n--- shrinking a huge exponent ---\n";
    std::string bigExp(1000, '9');              // a 1000-digit exponent
    std::cout << "7^(999...9, 1000 digits) mod 1000000007 = "
              << powerHugeExponent(7, bigExp, 1000000007LL) << "\n";
    std::cout << "the trap: 2^4 mod 4 = " << powerMod(2, 4, 4)
              << ", but reducing 4 mod phi(4)=2 gives 2^0 = " << powerMod(2, 0, 4)
              << "   -- our function returns " << powerHugeExponent(2, "4", 4)
              << ", which is correct\n";

    std::cout << "\n--- sum of phi over divisors equals n ---\n";
    std::cout << "n = 12:  ";
    ll total = 0;
    for (int d = 1; d <= 12; ++d) if (12 % d == 0) { std::cout << phi(d) << " "; total += phi(d); }
    std::cout << " sum = " << total << "\n";

    // ---- self-checks ----
    const int N = 200000;
    std::vector<int> table = phiSieve(N);

    bool bruteOk = true;
    for (int n = 1; n <= 2000; ++n) {
        int want = 0;
        for (int k = 1; k <= n; ++k) if (std::gcd(k, n) == 1) ++want;
        if (table[n] != want || phi(n) != want) bruteOk = false;
    }

    bool sieveMatchesSingle = true;
    for (int n = 1; n <= N; ++n)
        if (table[n] != phi(n)) sieveMatchesSingle = false;

    bool divisorSumOk = true;
    for (int n = 1; n <= 3000; ++n) {
        ll s = 0;
        for (int d = 1; d <= n; ++d) if (n % d == 0) s += table[d];
        if (s != n) divisorSumOk = false;
    }

    bool eulerOk = true;
    for (int m = 2; m <= 300; ++m)
        for (int a = 1; a < m; ++a)
            if (std::gcd(a, m) == 1 && powerMod(a, table[m], m) != 1) eulerOk = false;

    bool hugeExpOk = true;
    for (int m = 2; m <= 60; ++m)
        for (int a = 0; a < m; ++a)
            for (int k = 0; k <= 40; ++k) {
                ll want = powerMod(a, k, m);
                if (powerHugeExponent(a, std::to_string(k), m) != want) hugeExpOk = false;
            }

    std::cout << "\nself-check: phi vs counting " << (bruteOk ? "ok" : "FAIL")
              << ", sieve == single " << (sieveMatchesSingle ? "ok" : "FAIL")
              << ", sum over divisors == n " << (divisorSumOk ? "ok" : "FAIL")
              << ", Euler's theorem " << (eulerOk ? "ok" : "FAIL")
              << ", huge-exponent form (incl. gcd != 1) " << (hugeExpOk ? "ok" : "FAIL")
              << "\n";
    return 0;
}
