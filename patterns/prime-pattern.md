# Pattern: which prime tool

> *"Something about primes — but the constraints decide the algorithm, not the wording."*

This pattern is a dispatch table. Almost every prime problem is answered by the same three
tools, and picking the wrong one is the only real mistake available.

---

## Recognition clues

* the words **prime**, composite, "has no divisors other than", "cannot be written as a
  product"
* **"count the primes in $[l, r]$"**, repeated over queries
* **"is $n$ prime"** with $n$ enormous
* smallest or largest prime factor
* a number that is prime only in one algebraic case — $a^2-b^2$, $2^n-1$, $n^2+n+41$
* **"exactly three divisors"** → $p^2$; **"exactly two"** → prime
* needing to factor many numbers quickly

---

## Key observation

**Count the numbers you must answer for, not how big they are.** That single question picks
the tool:

| how many | how big | tool | cost |
|---|---|---|---|
| many | $\le 10^7$ | [sieve](../concepts/sieve/) | $O(n\log\log n)$ once, $O(1)$ per query |
| many, need factorizations | $\le 10^7$ | [`spf` sieve](../concepts/sieve/) | $O(n)$ once, $O(\log m)$ each |
| a window | $r \le 10^{12}$, $r-l$ small | segmented sieve | memory is the window, not the range |
| one | $\le 10^{18}$ | [Miller–Rabin](../concepts/miller-rabin/) | $\sim750$ multiplications |
| one, need its factors | $\le 9\times10^{18}$ | [Pollard's rho](../concepts/pollard-rho/) | $O(n^{1/4})$ |

The supporting fact behind all of them: **a composite $m$ always has a prime factor
$\le \sqrt m$**. That is why the sieve stops at $\sqrt n$, why trial division stops at
$\sqrt n$, and why the segmented sieve only needs the small primes.

There are about $n/\ln n$ primes below $n$ — exactly $78{,}498$ below $10^6$ and
$664{,}579$ below $10^7$ — so the list is always cheap to store.

---

## Relevant concepts

[sieve](../concepts/sieve/) · [miller-rabin](../concepts/miller-rabin/) ·
[pollard-rho](../concepts/pollard-rho/) ·
[prime-factorization](../concepts/prime-factorization/)

---

## Typical approach

1. Read the constraints before the statement. They choose the tool.
2. Many small numbers? Sieve **once, globally**, outside the test-case loop. This is the
   single most common performance mistake in multi-test problems.
3. Need factorizations too? Build `spf` instead of a boolean array. Same cost, far more
   useful.
4. A window high up the number line? Segmented sieve: small primes to $\sqrt r$, then a
   boolean window of size $r-l+1$.
5. One large number? Miller–Rabin with the fixed 12-base witness set. Deterministic, no
   randomness, no seed.
6. Need the factors of one large number? Strip small primes by trial division, then
   Pollard's rho with Miller–Rabin as the stopping rule.
7. Range queries on prime counts? Sieve, then prefix sums, then $O(1)$ per query.

---

## Complexity

| plain sieve | $O(n\log\log n)$, $O(n)$ space |
|---|---|
| linear sieve with `spf` | $O(n)$ |
| segmented sieve over $[l,r]$ | $O((r-l)\log\log r + \sqrt r)$ |
| Miller–Rabin, one $n$ | $O(\log n)$ multiplications, $\times 12$ bases |
| Pollard's rho, one split | $O(n^{1/4})$ expected |
| trial division, for contrast | $O(\sqrt n)$ — $10^9$ steps at $n=10^{18}$ |

---

## Common traps

| | |
|---|---|
| sieving inside the test-case loop | build it once, globally |
| sieving $10^{12}$ directly | that is the segmented version |
| `p * p` overflowing `int` in the sieve | cast to `ll`, or loop `p <= n / p` |
| forgetting $0$ and $1$ are not prime | set both false explicitly |
| a plain **Fermat** test | Carmichael numbers ($561$, $1105$, $1729$) pass every base |
| two or three random Miller–Rabin bases | use the deterministic 12-base set |
| `a * b % n` in Miller–Rabin without `__int128` | overflows for $n > 2^{31}$ |
| calling Pollard's rho on a prime | it never returns — test primality first |
| reaching for rho below $10^{12}$ | trial division is simpler and fast enough |
| `vector<bool>` for speed | it is a packed bit array; `vector<char>` is faster |

---

## Example problems here

| where | problem | the shape |
|---|---|---|
| CF 26A · 900 | [Almost Prime](https://codeforces.com/problemset/problem/26/A) | count distinct prime divisors over a range |
| CF 230B · 1300 | [T-primes](https://codeforces.com/problemset/problem/230/B) | exactly three divisors means $p^2$ |
| CF 271B · 1300 | [Prime Matrix](https://codeforces.com/problemset/problem/271/B) | sieve once, then distance to the next prime per cell |
| CF 237C · 1600 | [Primes on Interval](https://codeforces.com/problemset/problem/237/C) | prime counts over sliding windows, prefix sums plus binary search |
| CF 385C · 1700 | [Bear and Prime Numbers](https://codeforces.com/problemset/problem/385/C) | sieve skeleton plus prefix sums for $O(1)$ range queries |
| CF 1033B · 1100 | [Square Difference](https://codeforces.com/problemset/problem/1033/B) | $a^2-b^2$ is prime only if $a-b=1$; at $10^{11}$ that test is the problem |
| CF 679A · 1400 | [Bear and Prime 100](https://codeforces.com/problemset/problem/679/A) | interactive witness thinking in miniature |
| LC 204 · Med | [Count Primes](https://leetcode.com/problems/count-primes/) | the plain sieve, nothing else |
| LC 1175 · Easy | [Prime Arrangements](https://leetcode.com/problems/prime-arrangements/) | $\pi(n)$, then a factorial product mod $10^9+7$ |
| LC 866 · Med | [Prime Palindrome](https://leetcode.com/problems/prime-palindrome/) | generate candidates cleverly, test each |
| LC 2523 · Med | [Closest Primes in Range](https://leetcode.com/problems/closest-prime-numbers-in-range/) | at $10^6$ a sieve wins — the contrast case |

---

[patterns index](README.md) · [concepts](../concepts/) · [problems](../problems/README.md)
