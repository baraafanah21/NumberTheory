# Sieve of Eratosthenes — Problems

Ten problems, hardest last. Every **check value** here was computed, not guessed — if your
code prints something else, the bug is yours. Answers at the bottom.

---

## By hand

**1. Count them.** How many primes are there below $100$? Cross them out by hand — it takes
two passes, and you will see why the third pass is unnecessary.

**2. Both optimisations, one fact.** Explain why the inner loop may start at $p^2$ and why
the outer loop may stop at $\sqrt n$. Both follow from a single statement about composites
— write that statement first, then derive each.

**3. Sizing it.** Roughly how many primes are below $10^7$, and how much memory does a
`vector<char>` sieve to $10^7$ need? Then say which of `vector<bool>` and `vector<char>`
you would pick, and what you are trading.

---

## With code

**4. Three counts.** Sieve to $10^7$ and report $\pi(10^6)$, $\pi(10^7)$, and the **sum**
of all primes $\le 10^6$. The third one overflows a 32-bit accumulator — notice it before
it bites.

**5. `spf` in action.** Build the smallest-prime-factor table to $10^7$ and factor
$9699690$ with it. How many table lookups does it take, and how many divisions would trial
division have taken?

**6. Two distinct primes.** Using `spf`, count the $n \le 10^6$ with exactly two distinct
prime factors (any exponents). Do it in $O(n\log n)$ total, not by factoring each $n$ from
scratch.

**7. Segmented.** Count the primes in $[10^{12},\ 10^{12}+10^6]$. You cannot sieve to
$10^{12}$ — sieve to $\sqrt{R} = 10^6$ instead, then cross out inside a window of size
$10^6+1$. Watch the starting index: $\max\bigl(p^2,\ \lceil L/p\rceil p\bigr)$.

**8. Prime gaps.** Find the largest gap between consecutive primes below $10^6$, and where
it starts. Gaps stay small — this is why "scan upward for the next prime" is a viable
algorithm.

---

## Harder

**9. The crucial break.** In the linear sieve, delete the `p > spf[i]` half of the break
condition and find the smallest $n$ that then gets crossed out twice. Prove that with the
break, every composite is crossed **exactly once** — as (smallest prime factor) × (the
rest).

**10. Goldbach, empirically.** For every even $n$ in $[4,10^5]$, find the **smallest** prime
$p$ with $n-p$ also prime. Report the largest such $p$ over the whole range. What does the
size of that answer tell you about the cost of a "try small primes first" search?

---

## Judge problems

| where | problem | what it drills |
|---|---|---|
| CF 26A · 900 | [Almost Prime](https://codeforces.com/problemset/problem/26/A) | problem 6 in miniature — count distinct prime divisors over a range |
| CF 230B · 1300 | [T-primes](https://codeforces.com/problemset/problem/230/B) | exactly three divisors means $p^2$: sieve to $10^6$, then test $\sqrt n$ |
| CF 271B · 1300 | [Prime Matrix](https://codeforces.com/problemset/problem/271/B) | sieve once globally, then "distance to the next prime" per cell |
| CF 385C · 1700 | [Bear and Prime Numbers](https://codeforces.com/problemset/problem/385/C) | the sieve skeleton plus prefix sums to answer range queries in $O(1)$ |
| CF 237C · 1600 | [Primes on Interval](https://codeforces.com/problemset/problem/237/C) | prime counts over sliding windows — prefix sums again, plus a binary search |
| LC 204 · Med | [Count Primes](https://leetcode.com/problems/count-primes/) | the plain sieve, nothing else |
| LC 1175 · Easy | [Prime Arrangements](https://leetcode.com/problems/prime-arrangements/) | $\pi(n)$, then $\pi(n)!\,(n-\pi(n))! \bmod 10^9+7$ |
| LC 2523 · Med | [Closest Prime Numbers in Range](https://leetcode.com/problems/closest-prime-numbers-in-range/) | a segmented-sieve mindset on a range up to $10^6$ |

---

## Answers

**1.** $\mathbf{25}$ primes below $100$. Crossing out multiples of $2$, $3$, $5$ and $7$ is
enough, since $11^2 = 121 > 100$ — that is the $\sqrt n$ cutoff in miniature.

**2.** The single fact: **every composite $m$ has a prime factor $\le \sqrt m$** — its
smallest one. Start at $p^2$: any multiple $kp$ with $k < p$ has a prime factor smaller
than $p$, so it was already crossed out. Stop at $\sqrt n$: a composite $m \le n$ is caught
by its smallest prime factor, which is at most $\sqrt m \le \sqrt n$.

**3.** About $n/\ln n \approx 620{,}000$ primes below $10^7$ (the true count is $664579$),
and a `vector<char>` costs one byte each — $10$ MB. `vector<bool>` packs to $1.25$ MB but
pays bit-masking on every access; `vector<char>` is the faster of the two, and you trade
$8\times$ the memory for it.

**4.** $\pi(10^6) = \mathbf{78498}$, $\pi(10^7) = \mathbf{664579}$, and the primes up to
$10^6$ sum to $\mathbf{37550402023}$ — about $8\times$ past the 32-bit limit.

**5.** $9699690 = 2\cdot3\cdot5\cdot7\cdot11\cdot13\cdot17\cdot19$, the product of the
first eight primes. With `spf` it is $8$ lookups (each step strips one prime and at least
halves $m$); trial division would run $p$ up to $\sqrt{9699690} \approx 3114$, i.e. a few
thousand divisions.

**6.** $\mathbf{288726}$ of the $n \le 10^6$ have exactly two distinct prime factors.
Cheapest route: one sieve pass that increments a counter for every $n$ at each prime
dividing it — $O(n\log\log n)$ — then count the entries equal to $2$.

**7.** $\mathbf{36249}$ primes in $[10^{12}, 10^{12}+10^6]$. Consistent with the density
estimate $10^6/\ln(10^{12}) \approx 36{,}200$. Memory is the window ($1$ MB), not the
range.

**8.** The largest gap below $10^6$ is $\mathbf{114}$, from $492113$ to $492227$. Gaps this
small are why scanning upward from $n$ for the next prime terminates fast — you expect to
test about $\ln n$ candidates.

**9.** Without the break, $i = 6$ pairs with $p = 3$ and writes $\mathrm{spf}[18]$ even
though $18$'s smallest prime factor is $2$ — and $\mathbf{12}$ is the smallest number
crossed twice ($i=6,p=2$ and $i=4,p=3$). With the break, the loop only pairs $i$ with
primes $p \le \mathrm{spf}[i]$, so the product $i\cdot p$ is written exactly once: at the
moment $p$ is its smallest prime factor and $i$ is the rest. Existence and uniqueness of
that decomposition is exactly unique factorization, which is what makes the sieve linear.

**10.** The largest "smallest prime" is $\mathbf{293}$, at $n = 63274$. So across $50{,}000$
even numbers, trying primes in increasing order never needed more than the first $62$ of
them — a search that looks unbounded is, in practice, a few dozen primality tests.

---

[README.md](README.md) · [proofs.md](proofs.md) · [implementation.cpp](implementation.cpp)
