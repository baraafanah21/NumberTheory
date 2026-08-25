// Divisibility
// C++17
//
// Every routine here implements a result stated in README.md and proved in proofs.md.
// Cross-references are given as "Theorem N" (proofs.md) and "D-k" (README.md).
//
// Compile: g++ -std=c++17 -O2 -o divisibility implementation.cpp

#include <algorithm>
#include <cmath>
#include <iostream>
#include <string>
#include <vector>

using ll = long long;

// ---------------------------------------------------------------------------
// The relation itself
// ---------------------------------------------------------------------------

// Does a divide b?   (Definition; Theorem 8 justifies using the remainder to decide it)
//
// The a == 0 branch is not a hack: 0 | b holds exactly when b == 0 (Theorem 1e), and
// evaluating b % 0 would be undefined behaviour. Every other case is a zero remainder.
// Time O(1), Space O(1).
bool divides(ll a, ll b) {
    if (a == 0) return b == 0;
    return b % a == 0;
}

// ---------------------------------------------------------------------------
// The division algorithm
// ---------------------------------------------------------------------------

// Floor division: the largest q with q*b <= a. C++ '/' truncates toward zero, so
// -17 / 5 == -3, whereas the floor is -4. Correct downward when the signs differ and
// the division was inexact.
// Time O(1), Space O(1).
ll floorDiv(ll a, ll b) {
    ll q = a / b;
    if (a % b != 0 && ((a < 0) != (b < 0))) --q;
    return q;
}

// Remainder matching floorDiv: always has the sign of b, and is 0 <= r < |b| for b > 0.
// Time O(1), Space O(1).
ll floorMod(ll a, ll b) {
    return a - floorDiv(a, b) * b;
}

struct Division {
    ll quotient;
    ll remainder;
};

// The division algorithm of Theorem 7: the UNIQUE pair (q, r) with
//     a = q*b + r    and    0 <= r < |b|
//
// Note this is not what C++ gives you: (-17) % 5 is -2, but the mathematical remainder
// is 3. Reducing modulo |b| first pins r into [0, |b|), then q falls out exactly.
// Precondition: b != 0.
// Time O(1), Space O(1).
Division divisionAlgorithm(ll a, ll b) {
    ll magnitude = b < 0 ? -b : b;
    ll r = floorMod(a, magnitude);     // 0 <= r < |b|
    ll q = (a - r) / b;                // exact: b divides a - r by construction
    return {q, r};
}

// ---------------------------------------------------------------------------
// Divisors of a single number
// ---------------------------------------------------------------------------

// All positive divisors of n, sorted ascending.   (Theorem 9 / D8)
//
// Divisors pair up as {d, n/d} around sqrt(n), so walking d only as far as sqrt(n) and
// emitting both members of each pair reaches every divisor. The d != n/d guard stops the
// square root of a perfect square being emitted twice.
//
// The loop condition is d <= n / d, never d * d <= n: the latter overflows for n near the
// top of the type long before the loop would end.
//
// Precondition: n >= 1.
// Time O(sqrt(n)), Space O(tau(n)).
std::vector<ll> divisors(ll n) {
    std::vector<ll> result;
    for (ll d = 1; d <= n / d; ++d) {
        if (n % d == 0) {
            result.push_back(d);
            if (d != n / d) result.push_back(n / d);
        }
    }
    std::sort(result.begin(), result.end());   // the pairing emits them out of order
    return result;
}

// tau(n): how many positive divisors n has.   (Theorem 9)
// Same loop as above without storing anything.
// Precondition: n >= 1.
// Time O(sqrt(n)), Space O(1).
ll countDivisors(ll n) {
    ll count = 0;
    for (ll d = 1; d <= n / d; ++d) {
        if (n % d == 0) {
            count += (d == n / d) ? 1 : 2;
        }
    }
    return count;
}

// sigma(n): the sum of the positive divisors of n.   (Theorem 12 gives the closed form,
// but that needs the factorization; this needs only the pairing.)
// Precondition: n >= 1, and the sum fits in a signed 64-bit integer.
// Time O(sqrt(n)), Space O(1).
ll sumDivisors(ll n) {
    ll total = 0;
    for (ll d = 1; d <= n / d; ++d) {
        if (n % d == 0) {
            total += d;
            if (d != n / d) total += n / d;
        }
    }
    return total;
}

// Is n a perfect square? By Theorem 10 this is equivalent to tau(n) being odd -- but
// when you are not already inside a divisor loop, checking directly is far cheaper.
//
// std::sqrt on a double is inexact past 2^53, so its result is only a starting guess;
// the two correction loops fix it using integer comparisons only (note n / root rather
// than root * root, to avoid overflow). Each loop runs O(1) times in practice.
// Time O(1), Space O(1).
bool isPerfectSquare(ll n) {
    if (n < 0) return false;
    if (n == 0) return true;
    ll root = static_cast<ll>(std::sqrt(static_cast<double>(n)));
    while (root > 1 && root > n / root) --root;
    while (root + 1 <= n / (root + 1)) ++root;
    return root * root == n;
}

// Primality by trial division.   (Theorem 11)
// A composite n always has a divisor in [2, sqrt(n)], so finding none proves primality.
// Precondition: none; handles n < 2.
// Time O(sqrt(n)), Space O(1).
bool isPrimeTrialDivision(ll n) {
    if (n < 2) return false;
    for (ll d = 2; d <= n / d; ++d) {
        if (n % d == 0) return false;
    }
    return true;
}

// ---------------------------------------------------------------------------
// Sieves: every m <= n at once
// ---------------------------------------------------------------------------

// tau(m) for every m in [0, n], by iterating over multiples instead of over divisors.
//
// The inner loop bumps tau[m] once for each divisor d of m, so each divisor is counted
// exactly once. Total work is sum over d of floor(n/d) = Theta(n log n) by Theorem 13 --
// the harmonic sum is what keeps this from being quadratic.
//
// Time O(n log n), Space O(n).
std::vector<int> divisorCountSieve(int n) {
    std::vector<int> tau(n + 1, 0);
    for (int d = 1; d <= n; ++d) {
        for (int m = d; m <= n; m += d) {
            ++tau[m];
        }
    }
    return tau;
}

// sigma(m) for every m in [0, n]. Same skeleton, adding d rather than 1.
// The accumulator is 64-bit: sigma(m) can be several times m, so an int array sized for
// the input is not sized for the output.
// Time O(n log n), Space O(n).
std::vector<ll> divisorSumSieve(int n) {
    std::vector<ll> sigma(n + 1, 0);
    for (int d = 1; d <= n; ++d) {
        for (int m = d; m <= n; m += d) {
            sigma[m] += d;
        }
    }
    return sigma;
}

// ---------------------------------------------------------------------------
// Digit-based divisibility tests (D10 / Theorem 14)
//
// These take a STRING, which is the point: they work on numbers far too large to hold in
// any integer type, because they never form the number at all.
// ---------------------------------------------------------------------------

// Sum of the decimal digits.
// Time O(len), Space O(1).
long long digitSum(const std::string &digits) {
    long long sum = 0;
    for (char c : digits) sum += c - '0';
    return sum;
}

// Alternating digit sum, with a plus sign on the units digit.
// Time O(len), Space O(1).
long long alternatingDigitSum(const std::string &digits) {
    long long sum = 0;
    int sign = 1;
    for (int i = static_cast<int>(digits.size()) - 1; i >= 0; --i) {
        sum += sign * (digits[i] - '0');
        sign = -sign;
    }
    return sum;
}

// 9 | n  <=>  9 | digitSum(n), because 9 divides 10^k - 1 for every k.
bool divisibleBy9(const std::string &digits) { return digitSum(digits) % 9 == 0; }

// 3 | n  <=>  3 | digitSum(n), since 3 | 9 and transitivity carries the argument through.
bool divisibleBy3(const std::string &digits) { return digitSum(digits) % 3 == 0; }

// 11 | n <=>  11 | alternatingDigitSum(n), because 11 divides 10^k - (-1)^k.
bool divisibleBy11(const std::string &digits) { return alternatingDigitSum(digits) % 11 == 0; }

// ---------------------------------------------------------------------------
// Demonstration
// ---------------------------------------------------------------------------

void demoRelation() {
    std::cout << "--- The relation, including the zero cases ---\n";
    std::cout << "3 | 12 ?  " << (divides(3, 12) ? "yes" : "no") << "   [expect yes]\n";
    std::cout << "5 | 12 ?  " << (divides(5, 12) ? "yes" : "no") << "   [expect no]\n";
    std::cout << "7 | -21 ? " << (divides(7, -21) ? "yes" : "no")
              << "   [expect yes -- sign is irrelevant]\n";
    std::cout << "5 | 0 ?   " << (divides(5, 0) ? "yes" : "no")
              << "   [expect yes -- everything divides 0]\n";
    std::cout << "0 | 0 ?   " << (divides(0, 0) ? "yes" : "no")
              << "   [expect yes -- 0 = 0*k]\n";
    std::cout << "0 | 5 ?   " << (divides(0, 5) ? "yes" : "no")
              << "   [expect no -- 0 divides only itself]\n\n";
}

void demoDivisionAlgorithm() {
    std::cout << "--- The division algorithm (Theorem 7) ---\n";

    const ll cases[][2] = {{17, 5}, {-17, 5}, {17, -5}, {-17, -5}};
    for (const auto &c : cases) {
        Division d = divisionAlgorithm(c[0], c[1]);
        std::cout << "a = " << c[0] << ", b = " << c[1] << "  ->  q = " << d.quotient
                  << ", r = " << d.remainder << "   check: " << d.quotient << "*" << c[1]
                  << " + " << d.remainder << " = " << d.quotient * c[1] + d.remainder
                  << "\n";
    }

    std::cout << "contrast with C++: -17 / 5 = " << (-17 / 5) << ", -17 % 5 = " << (-17 % 5)
              << "   [truncates toward zero; the math remainder is 3]\n";

    // Verify the defining property over a range, including negative divisors.
    bool ok = true;
    for (ll a = -50; a <= 50; ++a) {
        for (ll b = -12; b <= 12; ++b) {
            if (b == 0) continue;
            Division d = divisionAlgorithm(a, b);
            ll magnitude = b < 0 ? -b : b;
            if (d.quotient * b + d.remainder != a) ok = false;
            if (d.remainder < 0 || d.remainder >= magnitude) ok = false;
        }
    }
    std::cout << "a = qb + r with 0 <= r < |b| holds for all tested a, b: "
              << (ok ? "yes" : "NO") << "\n\n";
}

void demoDivisors() {
    std::cout << "--- Divisors and the sqrt(n) pairing ---\n";

    std::cout << "divisors of 36: ";
    for (ll d : divisors(36)) std::cout << d << " ";
    std::cout << "\n  tau(36) = " << countDivisors(36) << ", sigma(36) = " << sumDivisors(36)
              << "   [expect 9 and 91]\n";

    std::cout << "divisors of 12: ";
    for (ll d : divisors(12)) std::cout << d << " ";
    std::cout << "\n  tau(12) = " << countDivisors(12) << ", sigma(12) = " << sumDivisors(12)
              << "   [expect 6 and 28 -- sigma(12) > 2*12, so 12 is abundant]\n";

    // Theorem 10: tau is odd exactly for perfect squares.
    std::cout << "n with odd tau(n) up to 50: ";
    for (ll n = 1; n <= 50; ++n) {
        if (countDivisors(n) % 2 == 1) std::cout << n << " ";
    }
    std::cout << "  [expect the perfect squares]\n";

    bool parityHolds = true;
    for (ll n = 1; n <= 2000; ++n) {
        if ((countDivisors(n) % 2 == 1) != isPerfectSquare(n)) parityHolds = false;
    }
    std::cout << "tau(n) odd <=> n is a perfect square, for n <= 2000: "
              << (parityHolds ? "yes" : "NO") << "\n";

    // The pairing loop must agree with a brute-force scan of every candidate.
    bool matchesBruteForce = true;
    for (ll n = 1; n <= 500; ++n) {
        std::vector<ll> brute;
        for (ll d = 1; d <= n; ++d) {
            if (n % d == 0) brute.push_back(d);
        }
        if (brute != divisors(n)) matchesBruteForce = false;
    }
    std::cout << "O(sqrt n) enumeration == O(n) brute force, for n <= 500: "
              << (matchesBruteForce ? "yes" : "NO") << "\n";

    // A large input, to show the sqrt bound is what makes this feasible.
    ll big = 963761198400LL;   // a highly composite number
    std::cout << "tau(963761198400) = " << countDivisors(big)
              << "   [found by checking only ~981000 candidates, not 9.6e11]\n\n";
}

void demoSieve() {
    std::cout << "--- Sieves: every m <= n at once (Theorem 13) ---\n";

    const int N = 100000;
    std::vector<int> tau = divisorCountSieve(N);
    std::vector<ll> sigma = divisorSumSieve(N);

    std::cout << "tau[36] = " << tau[36] << ", sigma[36] = " << sigma[36]
              << "   [expect 9 and 91]\n";

    // The sieve must agree with the per-number routine.
    bool agree = true;
    for (int m = 1; m <= 3000; ++m) {
        if (tau[m] != countDivisors(m) || sigma[m] != sumDivisors(m)) agree = false;
    }
    std::cout << "sieve == per-number computation for m <= 3000: " << (agree ? "yes" : "NO")
              << "\n";

    // Corollary 13.1: the average number of divisors is about ln(n).
    ll totalDivisors = 0;
    for (int m = 1; m <= N; ++m) totalDivisors += tau[m];
    std::cout << "average tau(m) for m <= " << N << " is "
              << static_cast<double>(totalDivisors) / N << "   [ln(" << N
              << ") = 11.5 -- divisors are rare on average]\n";

    // The perfect numbers fall out of sigma: sigma(n) == 2n.
    std::cout << "perfect numbers up to " << N << ": ";
    for (int m = 1; m <= N; ++m) {
        if (sigma[m] == 2LL * m) std::cout << m << " ";
    }
    std::cout << "\n\n";
}

void demoDigitTests() {
    std::cout << "--- Digit tests (Theorem 14), on numbers too large to parse ---\n";

    std::cout << "12345: digit sum " << digitSum("12345") << " -> divisible by 3? "
              << (divisibleBy3("12345") ? "yes" : "no") << ", by 9? "
              << (divisibleBy9("12345") ? "yes" : "no") << "   [expect yes, no]\n";

    std::cout << "918082: alternating sum " << alternatingDigitSum("918082")
              << " -> divisible by 11? " << (divisibleBy11("918082") ? "yes" : "no")
              << "   [expect yes]\n";

    // Cross-check the string tests against ordinary arithmetic on values that do fit.
    bool agree = true;
    for (int n = 1; n <= 20000; ++n) {
        std::string s = std::to_string(n);
        if (divisibleBy3(s) != (n % 3 == 0)) agree = false;
        if (divisibleBy9(s) != (n % 9 == 0)) agree = false;
        if (divisibleBy11(s) != (n % 11 == 0)) agree = false;
    }
    std::cout << "digit tests agree with % for all n <= 20000: " << (agree ? "yes" : "NO")
              << "\n";

    // The actual payoff: a 90-digit number, far beyond any integer type.
    std::string huge = "123456789012345678901234567890123456789012345678901234567890"
                       "123456789012345678901234567890";
    std::cout << "a " << huge.size() << "-digit number: divisible by 3? "
              << (divisibleBy3(huge) ? "yes" : "no") << ", by 9? "
              << (divisibleBy9(huge) ? "yes" : "no") << ", by 11? "
              << (divisibleBy11(huge) ? "yes" : "no")
              << "\n  (digit sum " << digitSum(huge) << ", alternating sum "
              << alternatingDigitSum(huge) << ")\n\n";
}

void demoNonImplication() {
    std::cout << "--- What divisibility does NOT give (Theorem 15) ---\n";
    std::cout << "6 | 4*9 = 36 ?  " << (divides(6, 36) ? "yes" : "no") << "\n";
    std::cout << "  but 6 | 4 ?   " << (divides(6, 4) ? "yes" : "no") << "\n";
    std::cout << "  and 6 | 9 ?   " << (divides(6, 9) ? "yes" : "no") << "\n";
    std::cout << "6 = 2*3 is split across the two factors, so neither contains it.\n";
    std::cout << "The implication needs the divisor to be PRIME -- that is Euclid's lemma,\n";
    std::cout << "which requires Bezout, not divisibility alone. See the GCD concept.\n";
}

int main() {
    demoRelation();
    demoDivisionAlgorithm();
    demoDivisors();
    demoSieve();
    demoDigitTests();
    demoNonImplication();
    return 0;
}
