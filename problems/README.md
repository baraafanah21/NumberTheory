# Problems

Every problem here is one already referenced from a concept folder in this repository.
Nothing was invented, and nothing is marked solved — the `done` column is yours to fill
in as you go.

**How to use this.** Pick a rating you can mostly clear, do five problems at it, then
move up. The `concept` column tells you which folder to reread when you get stuck, and
the `key idea` column is deliberately a spoiler — cover it until you have tried.

Where a key idea says *problem 4* or *problem 7*, it means that numbered exercise in the
linked concept's own `problems.md`, not an entry in this table.

When a problem teaches you something worth keeping, write it up with
[TEMPLATE.md](TEMPLATE.md) and drop the file in this directory. Do not write up problems
that went smoothly; the notes are for the ones that did not.

| | |
|---|---|
| unique problems | 67 |
| Codeforces, rated | 35 |
| LeetCode | 31 |
| other judges | 1 |

---

## Codeforces, by rating

### 800

| done | id | problem | concept | key idea |
|---|---|---|---|---|
| [ ] | CF 664A | [Complicated GCD](https://codeforces.com/problemset/problem/664/A) | [gcd](../concepts/gcd/) | $\gcd$ of every integer in $[a,b]$ — consecutive integers are coprime |
| [ ] | CF 913A | [Modular Exponentiation](https://codeforces.com/problemset/problem/913/A) | [fast-power](../concepts/fast-power/) | $2^n$ against $m$, where $n \le 10^8$ makes the naive loop the trap |
| [ ] | CF 1194A | [Remove a Progression](https://codeforces.com/problemset/problem/1194/A) | [progressions](../concepts/progressions/) | the survivors of the removal process are an AP with $d=2$ — noticing that *is* the problem |
| [ ] | CF 1325A | [EhAb AnD gCd](https://codeforces.com/problemset/problem/1325/A) | [gcd](../concepts/gcd/) | construct $a,b$ with $\gcd + \mathrm{lcm}$ given |
| [ ] | CF 1537A | [Arithmetic Array](https://codeforces.com/problemset/problem/1537/A) | [progressions](../concepts/progressions/) | mean of an array as a sum condition |
| [ ] | CF 2034A | [King Keykhosrow's Mystery](https://codeforces.com/problemset/problem/2034/A) | [gcd](../concepts/gcd/) | smallest $m$ with $m \bmod a = m \bmod b$ |

### 900

| done | id | problem | concept | key idea |
|---|---|---|---|---|
| [ ] | CF 26A | [Almost Prime](https://codeforces.com/problemset/problem/26/A) | [divisibility](../concepts/divisibility/) · [divisors](../concepts/divisors/) · [sieve](../concepts/sieve/) | count $n \le 3000$ with exactly two distinct prime divisors |

### 1000

*Nothing at this rating yet — a gap worth filling.*

### 1100

| done | id | problem | concept | key idea |
|---|---|---|---|---|
| [ ] | CF 630J | [Divisibility](https://codeforces.com/problemset/problem/630/J) | [divisibility](../concepts/divisibility/) | count $m \le 10^{18}$ divisible by all of $2..10$ — one lcm, then $\lfloor n/d\rfloor$ |
| [ ] | CF 1033B | [Square Difference](https://codeforces.com/problemset/problem/1033/B) | [miller-rabin](../concepts/miller-rabin/) | $a^2-b^2 = (a-b)(a+b)$ is prime only if $a-b=1$ and $a+b$ is prime — with $a \le 10^{11}$, that primality test is the whole problem |
| [ ] | CF 1327A | [Sum of Odd Integers](https://codeforces.com/problemset/problem/1327/A) | [fast-power](../concepts/fast-power/) · [progressions](../concepts/progressions/) | a constructive check where the power is the cheap part |

### 1200

*Nothing at this rating yet — a gap worth filling.*

### 1300

| done | id | problem | concept | key idea |
|---|---|---|---|---|
| [ ] | CF 230B | [T-primes](https://codeforces.com/problemset/problem/230/B) | [sieve](../concepts/sieve/) | exactly three divisors means $p^2$: sieve to $10^6$, then test $\sqrt n$ |
| [ ] | CF 271B | [Prime Matrix](https://codeforces.com/problemset/problem/271/B) | [sieve](../concepts/sieve/) | sieve once globally, then "distance to the next prime" per cell |
| [ ] | CF 876B | [Divisiblity of Differences](https://codeforces.com/problemset/problem/876/B) | [gcd](../concepts/gcd/) | problem 4's pattern: equal differences mod $m$ means bucket by remainder |

### 1400

| done | id | problem | concept | key idea |
|---|---|---|---|---|
| [ ] | CF 284A | [Cows and Primitive Roots](https://codeforces.com/problemset/problem/284/A) | [primitive-roots](../concepts/primitive-roots/) | count the primitive roots mod $p$ — problem 9, as a judge task |
| [ ] | CF 679A | [Bear and Prime 100](https://codeforces.com/problemset/problem/679/A) | [miller-rabin](../concepts/miller-rabin/) | interactive: decide primality of a hidden $n \le 100$ from divisibility queries — deterministic-witness thinking in miniature |
| [ ] | CF 762A | [k-th divisor](https://codeforces.com/problemset/problem/762/A) | [divisibility](../concepts/divisibility/) · [divisors](../concepts/divisors/) | the $O(\sqrt n)$ pair loop, and the $d = n/d$ edge case |

### 1500

*Nothing at this rating yet — a gap worth filling.*

### 1600+

| done | id | rating | problem | concept | key idea |
|---|---|---|---|---|---|
| [ ] | CF 237C | 1600 | [Primes on Interval](https://codeforces.com/problemset/problem/237/C) | [sieve](../concepts/sieve/) | prime counts over sliding windows — prefix sums again, plus a binary search |
| [ ] | CF 495B | 1600 | [Modular Equations](https://codeforces.com/problemset/problem/495/B) | [modular-inverse](../concepts/modular-inverse/) | solve $n \bmod x = k$ for $x$ — a congruence read the other way round |
| [ ] | CF 1165D | 1600 | [Almost All Divisors](https://codeforces.com/problemset/problem/1165/D) | [divisors](../concepts/divisors/) · [prime-factorization](../concepts/prime-factorization/) | reconstruct $n$ from its divisor list — pure $d \leftrightarrow n/d$ pairing |
| [ ] | CF 1458A | 1600 | [Row GCD](https://codeforces.com/problemset/problem/1458/A) | [gcd](../concepts/gcd/) | $\gcd(a_1+b_j,\dots,a_n+b_j) = \gcd(a_1+b_j,\ a_2-a_1,\ \dots)$ — gcd of differences again |
| [ ] | CF 1514C | 1600 | [Product 1 Modulo N](https://codeforces.com/problemset/problem/1514/C) | [modular-inverse](../concepts/modular-inverse/) | pair each unit with its inverse; the leftovers are the self-inverse ones — problem 2 taken seriously |
| [ ] | CF 385C | 1700 | [Bear and Prime Numbers](https://codeforces.com/problemset/problem/385/C) | [sieve](../concepts/sieve/) | the sieve skeleton plus prefix sums to answer range queries in $O(1)$ |
| [ ] | CF 1114C | 1700 | [Trailing Loves (or L'oeufs?)](https://codeforces.com/problemset/problem/1114/C) | [prime-factorization](../concepts/prime-factorization/) | problem 2 in an arbitrary base — factor $b$, apply Legendre per prime, take the min |
| [ ] | CF 1228C | 1700 | [Primes and Multiplication](https://codeforces.com/problemset/problem/1228/C) | [prime-factorization](../concepts/prime-factorization/) | exponent counting over a whole range, mod $10^9+7$ |
| [ ] | CF 1512G | 1700 | [Short Task](https://codeforces.com/problemset/problem/1512/G) | [divisibility](../concepts/divisibility/) · [divisors](../concepts/divisors/) | the problem-5 skeleton with `sigma[m] += d` |
| [ ] | CF 7C | 1800 | [Line](https://codeforces.com/problemset/problem/7/C) | [extended-euclid](../concepts/extended-euclid/) | $Ax+By+C=0$ in integers: solvability is $\gcd(A,B) \mid C$, then scale — problems 2 and 3 at $2\cdot10^9$ scale |
| [ ] | CF 300C | 1800 | [Beautiful Numbers](https://codeforces.com/problemset/problem/300/C) | [modular-inverse](../concepts/modular-inverse/) | $\binom nk \bmod p$ over many $k$ — problem 5 with a time limit |
| [ ] | CF 687B | 1800 | [Remainders Game](https://codeforces.com/problemset/problem/687/B) | [CRT](../concepts/chinese-remainder-theorem/) | when do $x \bmod c_i$ determine $x \bmod k$ — the answer is per prime power of $k$ |
| [ ] | CF 1225D | 1800 | [Power Products](https://codeforces.com/problemset/problem/1225/D) | [prime-factorization](../concepts/prime-factorization/) | reduce every exponent mod $k$ — pairs match iff the residue vectors are complementary |
| [ ] | CF 776E | 2100 | [The Holmes Children](https://codeforces.com/problemset/problem/776/E) | [euler-phi](../concepts/euler-phi/) | built directly on $\sum_{d\mid n}\varphi(d) = n$ — problem 10, as a judge task |
| [ ] | CF 919E | 2100 | [Congruence Equation](https://codeforces.com/problemset/problem/919/E) | [CRT](../concepts/chinese-remainder-theorem/) | $n\,a^n \equiv b \pmod p$: $a^n$ has period $p-1$ and $n$ lives mod $p$, so combine the two moduli |
| [ ] | CF 1500B | 2200 | [Two chandeliers](https://codeforces.com/problemset/problem/1500/B) | [CRT](../concepts/chinese-remainder-theorem/) | count coincidences of two cycles up to day $t$, then binary search $t$ |
| [ ] | CF 1106F | 2400 | [Lunar New Year and a Recursive Sequence](https://codeforces.com/problemset/problem/1106/F) | [primitive-roots](../concepts/primitive-roots/) | take discrete logs base $3$ mod $998244353$, turning a product recurrence into a linear one on exponents mod $p-1$; matrix-power it, then solve a linear congruence and exponentiate back |
| [ ] | CF 1305F | 2500 | [Kuroni and the Punishment](https://codeforces.com/problemset/problem/1305/F) | [pollard-rho](../concepts/pollard-rho/) | make every element share a prime, moving each by $\pm1$: sample elements at random, factor the values around them, then test each prime that turns up |
| [ ] | CF 906D | 2700 | [Power Tower](https://codeforces.com/problemset/problem/906/D) | [euler-phi](../concepts/euler-phi/) | a tower of exponents mod $m$: recurse with the safe form of problem 6, since $\gcd$ is never guaranteed |

---

## LeetCode, by difficulty

LeetCode does not publish a numeric rating, so these are kept on their own scale rather
than forced into the buckets above.

### Easy

| done | id | problem | concept | key idea |
|---|---|---|---|---|
| [ ] | LC 507 | [Perfect Number](https://leetcode.com/problems/perfect-number/) | [divisors](../concepts/divisors/) | problem 7, one value at a time |
| [ ] | LC 914 | [X of a Kind in a Deck of Cards](https://leetcode.com/problems/x-of-a-kind-in-a-deck-of-cards/) | [gcd](../concepts/gcd/) | fold $\gcd$ over counts, then test $\ge 2$ |
| [ ] | LC 1071 | [Greatest Common Divisor of Strings](https://leetcode.com/problems/greatest-common-divisor-of-strings/) | [gcd](../concepts/gcd/) | Euclid transplanted to strings; the answer's length is $\gcd$ of the lengths |
| [ ] | LC 1175 | [Prime Arrangements](https://leetcode.com/problems/prime-arrangements/) | [sieve](../concepts/sieve/) | $\pi(n)$, then $\pi(n)!\,(n-\pi(n))! \bmod 10^9+7$ |
| [ ] | LC 1952 | [Three Divisors](https://leetcode.com/problems/three-divisors/) | [divisibility](../concepts/divisibility/) · [divisors](../concepts/divisors/) | problem 4 as a judge task |
| [ ] | LC 2427 | [Number of Common Factors](https://leetcode.com/problems/number-of-common-factors/) | [divisibility](../concepts/divisibility/) · [divisors](../concepts/divisors/) | common divisors are the divisors of the gcd |

### Medium

| done | id | problem | concept | key idea |
|---|---|---|---|---|
| [ ] | LC 50 | [Pow(x, n)](https://leetcode.com/problems/powx-n/) | [fast-power](../concepts/fast-power/) · [modular-arithmetic](../concepts/modular-arithmetic/) | the loop with no modulus, plus the negative-exponent case of problem 4 |
| [ ] | LC 172 | [Factorial Trailing Zeroes](https://leetcode.com/problems/factorial-trailing-zeroes/) | [prime-factorization](../concepts/prime-factorization/) | problem 2 as a judge task |
| [ ] | LC 204 | [Count Primes](https://leetcode.com/problems/count-primes/) | [sieve](../concepts/sieve/) | the plain sieve, nothing else |
| [ ] | LC 365 | [Water and Jug Problem](https://leetcode.com/problems/water-and-jug-problem/) | [extended-euclid](../concepts/extended-euclid/) · [gcd](../concepts/gcd/) | Bézout as a reachability test |
| [ ] | LC 372 | [Super Pow](https://leetcode.com/problems/super-pow/) | [euler-phi](../concepts/euler-phi/) · [fast-power](../concepts/fast-power/) · [modular-arithmetic](../concepts/modular-arithmetic/) | exponent arrives as a digit array and the modulus $1337 = 7\cdot191$ is composite — exactly where the naive $\varphi$ reduction is wrong |
| [ ] | LC 413 | [Arithmetic Slices](https://leetcode.com/problems/arithmetic-slices/) | [progressions](../concepts/progressions/) | counting APs inside an array; the run of length $L$ contributes $\binom{L-1}{2}$ |
| [ ] | LC 866 | [Prime Palindrome](https://leetcode.com/problems/prime-palindrome/) | [miller-rabin](../concepts/miller-rabin/) | generate candidates cleverly, test each one — the test is the inner loop |
| [ ] | LC 1015 | [Smallest Integer Divisible by K](https://leetcode.com/problems/smallest-integer-divisible-by-k/) | [modular-arithmetic](../concepts/modular-arithmetic/) · [primitive-roots](../concepts/primitive-roots/) | build $1, 11, 111, \dots$ **as residues** — the true numbers overflow immediately |
| [ ] | LC 1201 | [Ugly Number III](https://leetcode.com/problems/ugly-number-iii/) | [CRT](../concepts/chinese-remainder-theorem/) · [divisibility](../concepts/divisibility/) | three periods, inclusion–exclusion, binary search |
| [ ] | LC 1390 | [Four Divisors](https://leetcode.com/problems/four-divisors/) | [divisors](../concepts/divisors/) · [pollard-rho](../concepts/pollard-rho/) | problem 6 as a judge task, and it wants $\sigma$ of those $n$ too |
| [ ] | LC 1447 | [Simplified Fractions](https://leetcode.com/problems/simplified-fractions/) | [euler-phi](../concepts/euler-phi/) · [gcd](../concepts/gcd/) | the coprime-pair count of problem 7, small enough to brute force |
| [ ] | LC 1492 | [The kth Factor of n](https://leetcode.com/problems/the-kth-factor-of-n/) | [divisibility](../concepts/divisibility/) · [divisors](../concepts/divisors/) | divisors in $O(\sqrt n)$, sorted, without double-counting $\sqrt n$ |
| [ ] | LC 1922 | [Count Good Numbers](https://leetcode.com/problems/count-good-numbers/) | [fast-power](../concepts/fast-power/) · [modular-arithmetic](../concepts/modular-arithmetic/) · [modular-inverse](../concepts/modular-inverse/) · [progressions](../concepts/progressions/) | $5^{\lceil n/2\rceil}4^{\lfloor n/2\rfloor} \bmod 10^9+7$ with $n \le 10^{15}$ |
| [ ] | LC 2400 | [Number of Ways to Reach a Position After Exactly k Steps](https://leetcode.com/problems/number-of-ways-to-reach-a-position-after-exactly-k-steps/) | [modular-inverse](../concepts/modular-inverse/) | one binomial mod $10^9+7$ |
| [ ] | LC 2507 | [Smallest Value After Replacing With Sum of Prime Factors](https://leetcode.com/problems/smallest-value-after-replacing-with-sum-of-prime-factors/) | [pollard-rho](../concepts/pollard-rho/) | factor, sum, repeat — a fixed-point loop over factorizations |
| [ ] | LC 2521 | [Distinct Prime Factors of Product of Array](https://leetcode.com/problems/distinct-prime-factors-of-product-of-array/) | [prime-factorization](../concepts/prime-factorization/) | the union of prime supports — no products needed |
| [ ] | LC 2523 | [Closest Prime Numbers in Range](https://leetcode.com/problems/closest-prime-numbers-in-range/) | [miller-rabin](../concepts/miller-rabin/) · [sieve](../concepts/sieve/) | at $10^6$ a sieve wins; useful as the contrast case for "which tool" |
| [ ] | LC 2550 | [Count Collisions of Monkeys on a Polygon](https://leetcode.com/problems/count-collisions-of-monkeys-on-a-polygon/) | [fast-power](../concepts/fast-power/) | $2^n - 2 \bmod 10^9+7$ — the subtraction is where people lose it, since the result can go negative |
| [ ] | LC 2572 | [Count the Number of Square-Free Subsets](https://leetcode.com/problems/count-the-number-of-square-free-subsets/) | [prime-factorization](../concepts/prime-factorization/) | problem 8 plus a bitmask over the primes below 30 |

### Hard

| done | id | problem | concept | key idea |
|---|---|---|---|---|
| [ ] | LC 829 | [Consecutive Numbers Sum](https://leetcode.com/problems/consecutive-numbers-sum/) | [progressions](../concepts/progressions/) | write $n$ as an AP with $d=1$ — solve $n = \frac{k(2a+k-1)}{2}$ for $k$ |
| [ ] | LC 878 | [Nth Magical Number](https://leetcode.com/problems/nth-magical-number/) | [CRT](../concepts/chinese-remainder-theorem/) | two periodic conditions merged through $\mathrm{lcm}$, then counted |
| [ ] | LC 952 | [Largest Component Size by Common Factor](https://leetcode.com/problems/largest-component-size-by-common-factor/) | [pollard-rho](../concepts/pollard-rho/) | the same pipeline — factor every value, union by shared prime — at a scale where a sieve suffices |
| [ ] | LC 1250 | [Check If It Is a Good Array](https://leetcode.com/problems/check-if-it-is-a-good-array/) | [extended-euclid](../concepts/extended-euclid/) | Bézout for $n$ numbers — the reachable set is the multiples of the whole array's gcd |
| [ ] | LC 1735 | [Count Ways to Make Array With Product](https://leetcode.com/problems/count-ways-to-make-array-with-product/) | [modular-inverse](../concepts/modular-inverse/) · [prime-factorization](../concepts/prime-factorization/) | stars-and-bars binomials mod $p$ — needs the whole factorial + inverse-factorial setup |
| [ ] | LC 2543 | [Check if Point Is Reachable](https://leetcode.com/problems/check-if-point-is-reachable/) | [extended-euclid](../concepts/extended-euclid/) | run the moves backwards; the invariant is the gcd, and it must be a power of two |

---

## Other judges

| done | id | problem | concept | key idea |
|---|---|---|---|---|
| [ ] | Library Checker | [Factorize](https://judge.yosupo.jp/problem/factorize) | [pollard-rho](../concepts/pollard-rho/) | the algorithm by itself, at $n < 2^{62}$ — the cleanest place to check your implementation |

---

## Problems appearing under more than one concept

These drill a crossover and are worth doing early — they are where two ideas meet.

| problem | concepts |
|---|---|
| [Ugly Number III](https://leetcode.com/problems/ugly-number-iii/) | [CRT](../concepts/chinese-remainder-theorem/) · [divisibility](../concepts/divisibility/) |
| [Almost Prime](https://codeforces.com/problemset/problem/26/A) | [divisibility](../concepts/divisibility/) · [divisors](../concepts/divisors/) · [sieve](../concepts/sieve/) |
| [k-th divisor](https://codeforces.com/problemset/problem/762/A) | [divisibility](../concepts/divisibility/) · [divisors](../concepts/divisors/) |
| [Short Task](https://codeforces.com/problemset/problem/1512/G) | [divisibility](../concepts/divisibility/) · [divisors](../concepts/divisors/) |
| [Three Divisors](https://leetcode.com/problems/three-divisors/) | [divisibility](../concepts/divisibility/) · [divisors](../concepts/divisors/) |
| [The kth Factor of n](https://leetcode.com/problems/the-kth-factor-of-n/) | [divisibility](../concepts/divisibility/) · [divisors](../concepts/divisors/) |
| [Number of Common Factors](https://leetcode.com/problems/number-of-common-factors/) | [divisibility](../concepts/divisibility/) · [divisors](../concepts/divisors/) |
| [Almost All Divisors](https://codeforces.com/problemset/problem/1165/D) | [divisors](../concepts/divisors/) · [prime-factorization](../concepts/prime-factorization/) |
| [Four Divisors](https://leetcode.com/problems/four-divisors/) | [divisors](../concepts/divisors/) · [pollard-rho](../concepts/pollard-rho/) |
| [Super Pow](https://leetcode.com/problems/super-pow/) | [euler-phi](../concepts/euler-phi/) · [fast-power](../concepts/fast-power/) · [modular-arithmetic](../concepts/modular-arithmetic/) |
| [Simplified Fractions](https://leetcode.com/problems/simplified-fractions/) | [euler-phi](../concepts/euler-phi/) · [gcd](../concepts/gcd/) |
| [Water and Jug Problem](https://leetcode.com/problems/water-and-jug-problem/) | [extended-euclid](../concepts/extended-euclid/) · [gcd](../concepts/gcd/) |
| [Pow(x, n)](https://leetcode.com/problems/powx-n/) | [fast-power](../concepts/fast-power/) · [modular-arithmetic](../concepts/modular-arithmetic/) |
| [Count Good Numbers](https://leetcode.com/problems/count-good-numbers/) | [fast-power](../concepts/fast-power/) · [modular-arithmetic](../concepts/modular-arithmetic/) · [modular-inverse](../concepts/modular-inverse/) · [progressions](../concepts/progressions/) |
| [Sum of Odd Integers](https://codeforces.com/problemset/problem/1327/A) | [fast-power](../concepts/fast-power/) · [progressions](../concepts/progressions/) |
| [Closest Prime Numbers in Range](https://leetcode.com/problems/closest-prime-numbers-in-range/) | [miller-rabin](../concepts/miller-rabin/) · [sieve](../concepts/sieve/) |
| [Smallest Integer Divisible by K](https://leetcode.com/problems/smallest-integer-divisible-by-k/) | [modular-arithmetic](../concepts/modular-arithmetic/) · [primitive-roots](../concepts/primitive-roots/) |
| [Count Ways to Make Array With Product](https://leetcode.com/problems/count-ways-to-make-array-with-product/) | [modular-inverse](../concepts/modular-inverse/) · [prime-factorization](../concepts/prime-factorization/) |

---

[repository root](../README.md) · [concepts](../concepts/) · [template](TEMPLATE.md)
