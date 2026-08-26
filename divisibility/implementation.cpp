// Divisibility
// C++17.  Compile: g++ -std=c++17 -O2 -o divisibility implementation.cpp
//
// The six routines from this topic that actually appear in code.

#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

using ll = long long;

// ---------------------------------------------------------------------------
// 1. The relation
// ---------------------------------------------------------------------------

// Does a divide b?
// The a == 0 branch is required, not defensive: 0 divides only 0, and b % 0 is
// undefined behaviour.
// O(1)
bool divides(ll a, ll b) {
    if (a == 0) return b == 0;
    return b % a == 0;
}

// The mathematical remainder: always in [0, |b|).
// C++ gives the sign of the dividend, so -17 % 5 == -2 while the true remainder is 3.
// O(1)
ll properMod(ll a, ll b) {
    ll r = a % b;
    if (r < 0) r += (b < 0 ? -b : b);
    return r;
}

// ---------------------------------------------------------------------------
// 2. Divisors of one number -- O(sqrt n)
// ---------------------------------------------------------------------------

// All positive divisors of n, sorted.
//
// Divisors pair up as {d, n/d} around sqrt(n), so looping to sqrt(n) and emitting both
// members of each pair reaches all of them.
//   - loop bound is d <= n/d, never d*d <= n  (that overflows for large n)
//   - the d != n/d guard stops a perfect square being emitted twice
// O(sqrt n) time, O(number of divisors) space.  Precondition: n >= 1.
std::vector<ll> divisors(ll n) {
    std::vector<ll> result;
    for (ll d = 1; d <= n / d; ++d)
        if (n % d == 0) {
            result.push_back(d);
            if (d != n / d) result.push_back(n / d);
        }
    std::sort(result.begin(), result.end());   // the pair loop emits them out of order
    return result;
}

// tau(n): how many divisors n has.   O(sqrt n)
ll countDivisors(ll n) {
    ll count = 0;
    for (ll d = 1; d <= n / d; ++d)
        if (n % d == 0) count += (d == n / d) ? 1 : 2;
    return count;
}

// sigma(n): the sum of the divisors of n.   O(sqrt n)
ll sumDivisors(ll n) {
    ll total = 0;
    for (ll d = 1; d <= n / d; ++d)
        if (n % d == 0) {
            total += d;
            if (d != n / d) total += n / d;
        }
    return total;
}

// A composite n always has a divisor in [2, sqrt(n)], so finding none proves n is prime.
// O(sqrt n)
bool isPrime(ll n) {
    if (n < 2) return false;
    for (ll d = 2; d <= n / d; ++d)
        if (n % d == 0) return false;
    return true;
}

// ---------------------------------------------------------------------------
// 3. Every number up to n at once -- O(n log n)
// ---------------------------------------------------------------------------

// tau(m) for every m in [0, n].
//
// Instead of asking each number for its divisors, each divisor visits its own multiples.
// The inner loop runs n/d times, and the sum of n/d over all d is n*H_n = O(n log n) --
// so this is NOT quadratic despite how it looks.
//
// Swap the body for other functions: sigma[m] += d, smallest prime factor, and so on.
// O(n log n) time, O(n) space.
std::vector<int> divisorCountSieve(int n) {
    std::vector<int> tau(n + 1, 0);
    for (int d = 1; d <= n; ++d)
        for (int m = d; m <= n; m += d)
            ++tau[m];
    return tau;
}

// ---------------------------------------------------------------------------
// 4. Digit tests -- for numbers too large to parse
// ---------------------------------------------------------------------------

// 9 divides every 10^k - 1, so replacing each power of ten by 1 changes n only by a
// multiple of 9. Hence n and its digit sum have the same divisibility by 9 (and by 3).
ll digitSum(const std::string &s) {
    ll sum = 0;
    for (char c : s) sum += c - '0';
    return sum;
}

// For 11 the weights alternate, because 10 is one LESS than 11: 10^k behaves like (-1)^k.
ll alternatingDigitSum(const std::string &s) {
    ll sum = 0;
    int sign = 1;
    for (int i = (int)s.size() - 1; i >= 0; --i) {
        sum += sign * (s[i] - '0');
        sign = -sign;
    }
    return sum;
}

bool divisibleBy3(const std::string &s)  { return digitSum(s) % 3 == 0; }
bool divisibleBy9(const std::string &s)  { return digitSum(s) % 9 == 0; }
bool divisibleBy11(const std::string &s) { return alternatingDigitSum(s) % 11 == 0; }

// ---------------------------------------------------------------------------
// Demo
// ---------------------------------------------------------------------------

int main() {
    std::cout << "--- the zero cases ---\n";
    std::cout << "5 divides 0? " << divides(5, 0) << "   0 divides 0? " << divides(0, 0)
              << "   0 divides 5? " << divides(0, 5) << "   [1 1 0]\n";
    std::cout << "C++ says -17 % 5 = " << (-17 % 5) << ", the true remainder is "
              << properMod(-17, 5) << "\n";

    std::cout << "\n--- divisors, O(sqrt n) ---\n";
    std::cout << "divisors of 36: ";
    for (ll d : divisors(36)) std::cout << d << " ";
    std::cout << "\n  tau(36) = " << countDivisors(36) << ", sigma(36) = " << sumDivisors(36)
              << "   [9 and 91]\n";
    std::cout << "is 97 prime? " << isPrime(97) << "   is 91 prime? " << isPrime(91)
              << "   [1 0, since 91 = 7 x 13]\n";

    std::cout << "\n--- odd divisor count means perfect square ---\n";
    std::cout << "numbers up to 50 with an odd number of divisors: ";
    for (ll n = 1; n <= 50; ++n)
        if (countDivisors(n) % 2 == 1) std::cout << n << " ";
    std::cout << "\n  (these are exactly the perfect squares)\n";

    std::cout << "\n--- the sieve, O(n log n) ---\n";
    const int N = 100000;
    std::vector<int> tau = divisorCountSieve(N);
    std::cout << "tau[36] from the sieve = " << tau[36] << "\n";
    ll total = 0;
    for (int m = 1; m <= N; ++m) total += tau[m];
    std::cout << "average divisor count below " << N << " = " << (double)total / N
              << "   [about ln(n) = 11.5, so divisors are rare]\n";

    std::cout << "\n--- digit tests ---\n";
    std::cout << "12345: digit sum " << digitSum("12345") << " -> divisible by 3? "
              << divisibleBy3("12345") << ", by 9? " << divisibleBy9("12345") << "\n";
    std::string huge(100000, '7');
    std::cout << "a " << huge.size() << "-digit number: by 3? " << divisibleBy3(huge)
              << ", by 9? " << divisibleBy9(huge) << ", by 11? " << divisibleBy11(huge)
              << "   (no integer type could hold it)\n";

    // Self-checks
    bool sqrtLoopOk = true;
    for (ll n = 1; n <= 500; ++n) {
        std::vector<ll> brute;
        for (ll d = 1; d <= n; ++d) if (n % d == 0) brute.push_back(d);
        if (brute != divisors(n)) sqrtLoopOk = false;
    }
    bool sieveOk = true;
    for (int m = 1; m <= 3000; ++m)
        if (tau[m] != countDivisors(m)) sieveOk = false;
    bool digitsOk = true;
    for (int n = 1; n <= 20000; ++n) {
        std::string s = std::to_string(n);
        if (divisibleBy3(s) != (n % 3 == 0) || divisibleBy9(s) != (n % 9 == 0) ||
            divisibleBy11(s) != (n % 11 == 0)) digitsOk = false;
    }
    std::cout << "\nself-check: sqrt loop == brute force " << (sqrtLoopOk ? "ok" : "FAIL")
              << ", sieve == per-number " << (sieveOk ? "ok" : "FAIL")
              << ", digit tests == % " << (digitsOk ? "ok" : "FAIL") << "\n";
    return 0;
}
