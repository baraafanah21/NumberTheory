// Divisibility -- CP templates
// C++17.  Compile: g++ -std=c++17 -O2 -o divisibility implementation.cpp
//
// Paste-ready. Every function states its complexity; see README.md for when to use which.

#include <algorithm>
#include <cmath>
#include <iostream>
#include <string>
#include <vector>

using ll = long long;

// ===========================================================================
// Basics
// ===========================================================================

// a | b.  The a == 0 branch is required: 0 | b only when b == 0, and b % 0 is UB.
// O(1)
bool divides(ll a, ll b) {
    if (a == 0) return b == 0;
    return b % a == 0;
}

// Floor division and its remainder. C++ truncates toward zero (-17 / 5 == -3), so the
// quotient needs correcting when the signs differ. floorMod is always in [0, |b|) for b > 0.
// O(1)
ll floorDiv(ll a, ll b) {
    ll q = a / b;
    if (a % b != 0 && ((a < 0) != (b < 0))) --q;
    return q;
}
ll floorMod(ll a, ll b) { return a - floorDiv(a, b) * b; }

struct Division { ll quotient, remainder; };

// The unique (q, r) with a = q*b + r and 0 <= r < |b|.  Not what % gives you.
// Precondition: b != 0.   O(1)
Division divisionAlgorithm(ll a, ll b) {
    ll magnitude = b < 0 ? -b : b;
    ll r = floorMod(a, magnitude);
    return {(a - r) / b, r};
}

// ===========================================================================
// Divisors of one number -- O(sqrt n)
// ===========================================================================

// All positive divisors, sorted.  Loop bound is d <= n/d, never d*d <= n (overflow).
// The d != n/d guard stops a perfect square being emitted twice.
// O(sqrt n) time, O(tau(n)) space.  Precondition: n >= 1.
std::vector<ll> divisors(ll n) {
    std::vector<ll> result;
    for (ll d = 1; d <= n / d; ++d)
        if (n % d == 0) {
            result.push_back(d);
            if (d != n / d) result.push_back(n / d);
        }
    std::sort(result.begin(), result.end());
    return result;
}

// tau(n): number of divisors.   O(sqrt n)
ll countDivisors(ll n) {
    ll count = 0;
    for (ll d = 1; d <= n / d; ++d)
        if (n % d == 0) count += (d == n / d) ? 1 : 2;
    return count;
}

// sigma(n): sum of divisors.   O(sqrt n)
ll sumDivisors(ll n) {
    ll total = 0;
    for (ll d = 1; d <= n / d; ++d)
        if (n % d == 0) {
            total += d;
            if (d != n / d) total += n / d;
        }
    return total;
}

// Integer-exact perfect square test. std::sqrt is only a starting guess (inexact past
// 2^53); the correction loops use n/root rather than root*root to avoid overflow.
// O(1)
bool isPerfectSquare(ll n) {
    if (n < 0) return false;
    if (n == 0) return true;
    ll root = static_cast<ll>(std::sqrt(static_cast<double>(n)));
    while (root > 1 && root > n / root) --root;
    while (root + 1 <= n / (root + 1)) ++root;
    return root * root == n;
}

// A composite always has a divisor in [2, sqrt(n)], so finding none proves primality.
// O(sqrt n)
bool isPrimeTrialDivision(ll n) {
    if (n < 2) return false;
    for (ll d = 2; d <= n / d; ++d)
        if (n % d == 0) return false;
    return true;
}

// ===========================================================================
// Counting multiples -- O(1)
// ===========================================================================

// How many multiples of d in [1, n].
ll countMultiples(ll d, ll n) { return n / d; }

// How many multiples of d in [L, R].
ll countMultiplesInRange(ll d, ll L, ll R) { return R / d - (L - 1) / d; }

// Sum of the multiples of d in [1, n].
ll sumMultiples(ll d, ll n) {
    ll k = n / d;
    return d * (k * (k + 1) / 2);
}

// ===========================================================================
// The divisor-block (floor) trick -- O(sqrt n)
// ===========================================================================

// sum over d = 1..n of floor(n / d), in O(sqrt n) instead of O(n).
//
// floor(n/d) takes only O(sqrt n) distinct values, so equal values are grouped into
// blocks and each block is added in one step. Swap the body to accumulate anything of
// the form f(floor(n/d)) * (block length).
// O(sqrt n)
ll sumOfFloorDivisions(ll n) {
    ll total = 0;
    for (ll l = 1, r; l <= n; l = r + 1) {
        r = n / (n / l);                     // largest r with n/r == n/l
        total += (r - l + 1) * (n / l);
    }
    return total;
}

// ===========================================================================
// Sieves -- every m <= n at once, O(n log n)
// ===========================================================================

// The "iterate over multiples" skeleton. Swap the body for sigma, mobius, spf, ...
// Total work is sum of n/d = n*H_n = Theta(n log n).
// O(n log n) time, O(n) space.
std::vector<int> divisorCountSieve(int n) {
    std::vector<int> tau(n + 1, 0);
    for (int d = 1; d <= n; ++d)
        for (int m = d; m <= n; m += d)
            ++tau[m];
    return tau;
}

// Same skeleton, adding d instead of 1. 64-bit accumulator: sigma(m) far exceeds m.
// O(n log n) time, O(n) space.
std::vector<ll> divisorSumSieve(int n) {
    std::vector<ll> sigma(n + 1, 0);
    for (int d = 1; d <= n; ++d)
        for (int m = d; m <= n; m += d)
            sigma[m] += d;
    return sigma;
}

// ===========================================================================
// Digit tests -- for numbers too large to parse
// ===========================================================================

ll digitSum(const std::string &s) {
    ll sum = 0;
    for (char c : s) sum += c - '0';
    return sum;
}

ll alternatingDigitSum(const std::string &s) {
    ll sum = 0;
    int sign = 1;
    for (int i = static_cast<int>(s.size()) - 1; i >= 0; --i) {
        sum += sign * (s[i] - '0');
        sign = -sign;
    }
    return sum;
}

bool divisibleBy3(const std::string &s)  { return digitSum(s) % 3 == 0; }
bool divisibleBy9(const std::string &s)  { return digitSum(s) % 9 == 0; }
bool divisibleBy11(const std::string &s) { return alternatingDigitSum(s) % 11 == 0; }

// ===========================================================================
// Demo / self-check
// ===========================================================================

void demoBasics() {
    std::cout << "--- zero rules ---\n";
    std::cout << "5|0 " << divides(5, 0) << "  0|0 " << divides(0, 0)
              << "  0|5 " << divides(0, 5) << "   [expect 1 1 0]\n";

    std::cout << "--- division algorithm vs C++ % ---\n";
    for (auto [a, b] : {std::pair<ll, ll>{17, 5}, {-17, 5}, {17, -5}, {-17, -5}}) {
        Division d = divisionAlgorithm(a, b);
        std::cout << a << " = " << d.quotient << "*" << b << " + " << d.remainder << "\n";
    }
    std::cout << "C++ gives -17 % 5 = " << (-17 % 5) << ", math wants 3\n";

    bool ok = true;
    for (ll a = -50; a <= 50; ++a)
        for (ll b = -12; b <= 12; ++b) {
            if (b == 0) continue;
            Division d = divisionAlgorithm(a, b);
            ll m = b < 0 ? -b : b;
            if (d.quotient * b + d.remainder != a || d.remainder < 0 || d.remainder >= m)
                ok = false;
        }
    std::cout << "0 <= r < |b| verified on all tested pairs: " << (ok ? "yes" : "NO") << "\n\n";
}

void demoDivisors() {
    std::cout << "--- divisors, O(sqrt n) ---\n";
    std::cout << "divisors(36): ";
    for (ll d : divisors(36)) std::cout << d << " ";
    std::cout << "  tau=" << countDivisors(36) << " sigma=" << sumDivisors(36)
              << "   [expect 9, 91]\n";

    bool matches = true;
    for (ll n = 1; n <= 500; ++n) {
        std::vector<ll> brute;
        for (ll d = 1; d <= n; ++d) if (n % d == 0) brute.push_back(d);
        if (brute != divisors(n)) matches = false;
    }
    std::cout << "sqrt-loop == brute force for n <= 500: " << (matches ? "yes" : "NO") << "\n";

    std::cout << "odd tau up to 50: ";
    for (ll n = 1; n <= 50; ++n) if (countDivisors(n) % 2) std::cout << n << " ";
    std::cout << "  [the squares]\n";

    bool parity = true;
    for (ll n = 1; n <= 2000; ++n)
        if ((countDivisors(n) % 2 == 1) != isPerfectSquare(n)) parity = false;
    std::cout << "tau odd <=> perfect square, n <= 2000: " << (parity ? "yes" : "NO") << "\n";

    std::cout << "tau(963761198400) = " << countDivisors(963761198400LL)
              << "   [~981k candidates, not 9.6e11]\n\n";
}

void demoCounting() {
    std::cout << "--- counting multiples, O(1) ---\n";
    std::cout << "multiples of 7 in [1,100]: " << countMultiples(7, 100)
              << ", their sum: " << sumMultiples(7, 100) << "   [expect 14, 735]\n";
    std::cout << "multiples of 3 in [10,50]: " << countMultiplesInRange(3, 10, 50)
              << "   [expect 13: 12,15,...,48]\n";

    // Inclusion-exclusion over multiples: divisible by 3 or 5 below 1000.
    std::cout << "sum of multiples of 3 or 5 below 1000: "
              << sumMultiples(3, 999) + sumMultiples(5, 999) - sumMultiples(15, 999)
              << "   [expect 233168]\n";

    // The floor trick must agree with the naive loop, and is O(sqrt n).
    bool ok = true;
    for (ll n = 1; n <= 300; ++n) {
        ll naive = 0;
        for (ll d = 1; d <= n; ++d) naive += n / d;
        if (naive != sumOfFloorDivisions(n)) ok = false;
    }
    std::cout << "divisor-block trick == naive loop for n <= 300: " << (ok ? "yes" : "NO")
              << "\n";
    std::cout << "sum of floor(1e12/d) for d=1..1e12 = " << sumOfFloorDivisions(1000000000000LL)
              << "   [O(sqrt n): 2 million steps, not 1e12]\n\n";
}

void demoSieve() {
    std::cout << "--- sieves, O(n log n) ---\n";
    const int N = 100000;
    std::vector<int> tau = divisorCountSieve(N);
    std::vector<ll> sigma = divisorSumSieve(N);

    bool agree = true;
    for (int m = 1; m <= 3000; ++m)
        if (tau[m] != countDivisors(m) || sigma[m] != sumDivisors(m)) agree = false;
    std::cout << "sieve == per-number for m <= 3000: " << (agree ? "yes" : "NO") << "\n";

    ll total = 0;
    for (int m = 1; m <= N; ++m) total += tau[m];
    std::cout << "average tau below " << N << ": " << static_cast<double>(total) / N
              << "   [~ln n = 11.5]\n";

    std::cout << "perfect numbers (sigma = 2n) up to " << N << ": ";
    for (int m = 1; m <= N; ++m) if (sigma[m] == 2LL * m) std::cout << m << " ";
    std::cout << "\n\n";
}

void demoDigits() {
    std::cout << "--- digit tests, for unparseable input ---\n";
    bool agree = true;
    for (int n = 1; n <= 20000; ++n) {
        std::string s = std::to_string(n);
        if (divisibleBy3(s) != (n % 3 == 0)) agree = false;
        if (divisibleBy9(s) != (n % 9 == 0)) agree = false;
        if (divisibleBy11(s) != (n % 11 == 0)) agree = false;
    }
    std::cout << "digit tests == % for n <= 20000: " << (agree ? "yes" : "NO") << "\n";

    std::string huge(100000, '7');   // 100000 sevens -- no integer type holds this
    std::cout << "a " << huge.size() << "-digit number: by 3? " << divisibleBy3(huge)
              << "  by 9? " << divisibleBy9(huge) << "  by 11? " << divisibleBy11(huge)
              << "\n\n";
}

void demoTrap() {
    std::cout << "--- the trap: d | ab does NOT give d | a or d | b ---\n";
    std::cout << "6 | 36 = 4*9 ? " << divides(6, 36) << "   but 6|4 ? " << divides(6, 4)
              << "  6|9 ? " << divides(6, 9) << "\n";
    std::cout << "Only true when d is prime (Euclid's lemma).\n";
}

int main() {
    demoBasics();
    demoDivisors();
    demoCounting();
    demoSieve();
    demoDigits();
    demoTrap();
    return 0;
}
