// Divisibility -- g++ -std=c++17 -O2 implementation.cpp

#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

using ll = long long;

// ---------------------------------------------------------------- the relation

// 0 divides only 0, and b % 0 is undefined behaviour -- hence the branch.  O(1)
bool divides(ll a, ll b) { return a == 0 ? b == 0 : b % a == 0; }

// The mathematical remainder, always in [0, |b|).  C++ gives the sign of the dividend,
// so -17 % 5 == -2 while the true remainder is 3.  O(1)
ll properMod(ll a, ll b) {
    ll r = a % b;
    return r < 0 ? r + (b < 0 ? -b : b) : r;
}

// ---------------------------------------------------------------- one number, O(sqrt n)

// Divisors pair as {d, n/d} around sqrt(n), so looping to sqrt(n) reaches all of them.
// Loop bound is d <= n/d, never d*d <= n (that overflows).  The d != n/d guard stops a
// perfect square being emitted twice.
// O(sqrt n) time, O(tau(n)) space.  Precondition: n >= 1.
std::vector<ll> divisors(ll n) {
    std::vector<ll> out;
    for (ll d = 1; d <= n / d; ++d)
        if (n % d == 0) {
            out.push_back(d);
            if (d != n / d) out.push_back(n / d);
        }
    std::sort(out.begin(), out.end());     // the pair loop emits them out of order
    return out;
}

// tau(n): how many divisors.  O(sqrt n)
ll countDivisors(ll n) {
    ll c = 0;
    for (ll d = 1; d <= n / d; ++d)
        if (n % d == 0) c += (d == n / d) ? 1 : 2;
    return c;
}

// sigma(n): sum of divisors.  O(sqrt n)
ll sumDivisors(ll n) {
    ll s = 0;
    for (ll d = 1; d <= n / d; ++d)
        if (n % d == 0) s += d + (d == n / d ? 0 : n / d);
    return s;
}

// A composite always has a divisor in [2, sqrt n], so finding none proves primality.
// O(sqrt n)
bool isPrime(ll n) {
    if (n < 2) return false;
    for (ll d = 2; d <= n / d; ++d)
        if (n % d == 0) return false;
    return true;
}

// ---------------------------------------------------------------- all m <= n, O(n log n)

// Each divisor visits its own multiples.  The inner loop runs n/d times and the sum of
// those is n*H_n, so this is NOT quadratic.  Swap the body for sigma, spf, mobius, ...
// O(n log n) time, O(n) space.
std::vector<int> divisorCountSieve(int n) {
    std::vector<int> tau(n + 1, 0);
    for (int d = 1; d <= n; ++d)
        for (int m = d; m <= n; m += d)
            ++tau[m];
    return tau;
}

// ---------------------------------------------------------------- digit tests

// 9 divides every 10^k - 1, so replacing each power of ten by 1 shifts n by a multiple
// of 9.  For 11 the weights alternate, since 11 divides 10^k - (-1)^k.
ll digitSum(const std::string &s) {
    ll t = 0;
    for (char c : s) t += c - '0';
    return t;
}

ll alternatingDigitSum(const std::string &s) {
    ll t = 0;
    int sign = 1;
    for (int i = (int)s.size() - 1; i >= 0; --i, sign = -sign) t += sign * (s[i] - '0');
    return t;
}

bool divisibleBy3(const std::string &s)  { return digitSum(s) % 3 == 0; }
bool divisibleBy9(const std::string &s)  { return digitSum(s) % 9 == 0; }
bool divisibleBy11(const std::string &s) { return alternatingDigitSum(s) % 11 == 0; }

// ---------------------------------------------------------------- demo

int main() {
    std::cout << "zero cases\n";
    std::cout << "  5|0 " << divides(5, 0) << "   0|0 " << divides(0, 0)
              << "   0|5 " << divides(0, 5) << "\t[1 1 0]\n";
    std::cout << "  C++ says -17 % 5 = " << (-17 % 5) << ", true remainder is "
              << properMod(-17, 5) << "\n";

    std::cout << "\ndivisors, O(sqrt n)\n  36: ";
    for (ll d : divisors(36)) std::cout << d << " ";
    std::cout << "\n  tau(36) = " << countDivisors(36) << ", sigma(36) = " << sumDivisors(36)
              << "\t[9, 91]\n";
    std::cout << "  97 prime? " << isPrime(97) << "   91 prime? " << isPrime(91)
              << "\t[1 0, since 91 = 7x13]\n";

    std::cout << "\nodd divisor count means perfect square\n  ";
    for (ll n = 1; n <= 50; ++n)
        if (countDivisors(n) % 2) std::cout << n << " ";
    std::cout << "\n";

    const int N = 100000;
    std::vector<int> tau = divisorCountSieve(N);
    ll total = 0;
    for (int m = 1; m <= N; ++m) total += tau[m];
    std::cout << "\nsieve, O(n log n)\n  tau[36] = " << tau[36]
              << "   average tau below " << N << " = " << (double)total / N
              << "\t[about ln n = 11.5]\n";

    std::string huge(100000, '7');
    std::cout << "\ndigit tests\n  12345: by 3? " << divisibleBy3("12345")
              << "  by 9? " << divisibleBy9("12345") << "\n";
    std::cout << "  a " << huge.size() << "-digit number: by 3? " << divisibleBy3(huge)
              << "  by 9? " << divisibleBy9(huge) << "  by 11? " << divisibleBy11(huge)
              << "\t(no integer type holds it)\n";

    bool loopOk = true, sieveOk = true, digitOk = true;
    for (ll n = 1; n <= 500; ++n) {
        std::vector<ll> brute;
        for (ll d = 1; d <= n; ++d) if (n % d == 0) brute.push_back(d);
        if (brute != divisors(n)) loopOk = false;
    }
    for (int m = 1; m <= 3000; ++m)
        if (tau[m] != countDivisors(m)) sieveOk = false;
    for (int n = 1; n <= 20000; ++n) {
        std::string s = std::to_string(n);
        if (divisibleBy3(s) != (n % 3 == 0) || divisibleBy9(s) != (n % 9 == 0) ||
            divisibleBy11(s) != (n % 11 == 0)) digitOk = false;
    }
    std::cout << "\nself-check: sqrt loop vs brute force " << (loopOk ? "ok" : "FAIL")
              << ", sieve vs per-number " << (sieveOk ? "ok" : "FAIL")
              << ", digit tests vs % " << (digitOk ? "ok" : "FAIL") << "\n";
}
