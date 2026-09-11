// Fast Power (Binary Exponentiation) -- g++ -std=c++17 -O2 implementation.cpp

#include <array>
#include <iostream>
#include <string>
#include <vector>

using ll = long long;

ll norm(ll x, ll m) { x %= m; return x < 0 ? x + m : x; }
ll mulMod(ll a, ll b, ll m) { return (ll)((__int128)a * b % m); }

// ---------------------------------------------------------------- the loop

// Walk the bits of exp from the bottom.  'base' squares every step, so after k steps it
// holds a^(2^k); multiply it into the answer exactly when bit k of exp is set.
// The invariant r * base^exp is constant, and the loop ends at exp == 0.
// O(log exp) modular multiplications -- at most ~120 for any 64-bit exponent.
ll powMod(ll base, ll exp, ll m) {
    if (exp < 0) return -1;             // a negative exponent is an inverse, not a power
    ll r = 1 % m;                       // 1 % m, not 1: the modulus may be 1
    base = norm(base, m);
    while (exp > 0) {
        if (exp & 1) r = mulMod(r, base, m);
        base = mulMod(base, base, m);
        exp >>= 1;
    }
    return r;
}

// The same loop with no modulus.  Correct only while the true value fits -- 2^62 is
// already the ceiling, so this is for small bases and small exponents only.
// O(log exp)
ll ipow(ll base, ll exp) {
    ll r = 1;
    while (exp > 0) {
        if (exp & 1) r *= base;
        base *= base;
        exp >>= 1;
    }
    return r;
}

// Multiplication by repeated doubling: the identical loop with + where * was.  This is how
// you compute a*b mod m on a compiler with no __int128.
// O(log b)
ll mulModByDoubling(ll a, ll b, ll m) {
    ll r = 0;
    a = norm(a, m);
    b = norm(b, m);
    while (b > 0) {
        if (b & 1) r = (r + a) % m;
        a = (a + a) % m;
        b >>= 1;
    }
    return r;
}

// ---------------------------------------------------------------- uses

// An exponent too large to hold in any integer type, arriving as decimal digits.
// Horner on the exponent: a^(10k + d) = (a^k)^10 * a^d.
// O(len * log 10) multiplications
ll powHugeExponent(ll base, const std::string &digits, ll m) {
    ll r = 1 % m;
    for (char c : digits) {
        if (c < '0' || c > '9') continue;
        r = powMod(r, 10, m);
        r = mulMod(r, powMod(base, c - '0', m), m);
    }
    return r;
}

// Inverse modulo a PRIME, via Fermat: a^(p-1) = 1, so a^(p-2) is the inverse.
// Silently wrong on a composite modulus -- see ../modular-inverse/.
// O(log p)
ll modInversePrime(ll a, ll p) { return powMod(a, p - 2, p); }

// 2x2 matrix power, the standard upgrade.  Same loop, matrix multiply instead of mulMod.
// Every entry is reduced at every step; skipping that is the usual bug.
using Mat = std::array<std::array<ll, 2>, 2>;

Mat matMul(const Mat &A, const Mat &B, ll m) {
    Mat C{};
    for (int i = 0; i < 2; ++i)
        for (int k = 0; k < 2; ++k) {
            if (A[i][k] == 0) continue;
            for (int j = 0; j < 2; ++j)
                C[i][j] = (C[i][j] + mulMod(A[i][k], B[k][j], m)) % m;
        }
    return C;
}

// O(2^3 log n)
Mat matPow(Mat A, ll n, ll m) {
    Mat r{{{1 % m, 0}, {0, 1 % m}}};    // the identity
    while (n > 0) {
        if (n & 1) r = matMul(r, A, m);
        A = matMul(A, A, m);
        n >>= 1;
    }
    return r;
}

// F(0)=0, F(1)=1.  [[1,1],[1,0]]^n has F(n) in the top-right corner.
// O(log n) -- so n = 1e18 is instant.
ll fibonacci(ll n, ll m) {
    Mat base{{{1, 1}, {1, 0}}};
    return matPow(base, n, m)[0][1];
}

// ---------------------------------------------------------------- demo

int main() {
    const ll M = 1000000007;

    std::cout << "the basics\n";
    std::cout << "  2^10 mod 1000    = " << powMod(2, 10, 1000) << "\t[1024 -> 24]\n";
    std::cout << "  3^100 mod M      = " << powMod(3, 100, M) << "\n";
    std::cout << "  2^62             = " << ipow(2, 62) << "\t[no modulus, at the ceiling]\n";
    std::cout << "  anything mod 1   = " << powMod(7, 5, 1) << "\t[1 % m, not 1]\n";
    std::cout << "  (-3)^3 mod 7     = " << powMod(-3, 3, 7) << "\t[base normalized first]\n";

    std::cout << "\nbig exponents\n";
    std::cout << "  7^(10^18) mod M  = " << powMod(7, 1000000000000000000LL, M) << "\n";
    std::cout << "  5^(60 digits) mod M = "
              << powHugeExponent(5, "123456789012345678901234567890"
                                    "123456789012345678901234567890", M)
              << "\t[exponent never parsed]\n";

    std::cout << "\nwhere the modulus is big\n";
    const ll HUGE_M = 4000000000000000037LL;
    std::cout << "  3^(10^9) mod 4e18 = " << powMod(3, 1000000000LL, HUGE_M)
              << "\t[needs mulMod, not *]\n";

    std::cout << "\nsame loop, other operations\n";
    std::cout << "  Fermat inverse of 3 mod M = " << modInversePrime(3, M)
              << "   check 3*inv = " << mulMod(3, modInversePrime(3, M), M) << "\n";
    std::cout << "  F(10)  = " << fibonacci(10, M) << "\t[55]\n";
    std::cout << "  F(90)  = " << fibonacci(90, M) << "\t[2880067194370816120 mod M]\n";
    std::cout << "  F(10^18) mod M = " << fibonacci(1000000000000000000LL, M) << "\n";

    // self-check: powMod against naive repeated multiplication
    bool ok = true;
    for (ll m = 1; m <= 50; ++m)
        for (ll a = -20; a <= 20; ++a)
            for (ll e = 0; e <= 30; ++e) {
                ll want = 1 % m;
                for (ll i = 0; i < e; ++i) want = (want * norm(a, m)) % m;
                if (powMod(a, e, m) != want) ok = false;
            }

    // self-check: the doubling multiply agrees with __int128
    bool dbl = true;
    for (ll i = 1; i <= 3000; ++i) {
        ll x = HUGE_M - i * 1000003, y = HUGE_M - i * 7919;
        if (mulModByDoubling(x, y, HUGE_M) != mulMod(x, y, HUGE_M)) dbl = false;
    }

    // self-check: matrix Fibonacci against the iterative sequence
    bool fib = true;
    ll p = 0, q = 1;
    for (ll n = 0; n <= 500; ++n) {
        if (fibonacci(n, M) != p) fib = false;
        ll t = (p + q) % M;
        p = q;
        q = t;
    }

    std::cout << "\nself-check: powMod vs naive " << (ok ? "ok" : "FAIL")
              << ", doubling multiply " << (dbl ? "ok" : "FAIL")
              << ", matrix Fibonacci " << (fib ? "ok" : "FAIL") << "\n";
}
