# Divisors: $\tau$ and $\sigma$

$$\tau(n) = \#\{d : d \mid n\}, \qquad \sigma(n) = \sum_{d \mid n} d$$

How **many** divisors a number has, and what they **add up to**.

**Use:** counting problems that hinge on divisor structure, range queries over $\tau$ or
$\sigma$, "exactly $k$ divisors" constructions. **Needs:**
[divisibility](../divisibility/) · [prime factorization](../prime-factorization/)
**Next:** [Euler's totient](../euler-phi/) · [sieve](../sieve/)

**Tier: Essential.**

> **Where this lives.** [divisibility](../divisibility/) owns the $O(\sqrt n)$ pair loop
> and [prime factorization](../prime-factorization/) owns the exponent formula. This folder
> is the single place the two are compared, and the only place the **range** versions live.

---

## The formulas

From $n = p_1^{e_1}\cdots p_k^{e_k}$, a divisor is any exponent-wise smaller number, so you
pick each $f_i$ independently from $0..e_i$:

$$\boxed{\ \tau(n) = \prod_i (e_i + 1), \qquad \sigma(n) = \prod_i \frac{p_i^{\,e_i+1}-1}{p_i-1}\ }$$

The $\sigma$ product is one geometric sum per prime — $1 + p + p^2 + \cdots + p^{e}$ —
because expanding the product generates every divisor exactly once.

**Example.** $36 = 2^2\cdot3^2$, so $\tau(36) = 3\cdot3 = 9$ and
$\sigma(36) = (1{+}2{+}4)(1{+}3{+}9) = 7\cdot13 = 91$. Both are
[proved in prime-factorization](../prime-factorization/proofs.md).

**Both are multiplicative:** $\gcd(a,b)=1 \Rightarrow \tau(ab) = \tau(a)\tau(b)$, and the
same for $\sigma$. That is what makes the sieve versions below work.

> Coprimality is required. $\tau(2)\tau(2) = 4$ but $\tau(4) = 3$.

**$\tau(n)$ is odd exactly when $n$ is a perfect square** — divisors pair as
$d \leftrightarrow n/d$, and the only unpaired case is $d = n/d$. This is the single most
reused fact in the whole folder.

---

## Three ways to compute

**1. One $n$, by trial division — $O(\sqrt n)$.** Walk to $\sqrt n$ and count both members
of each pair.

```cpp
ll tau = 0;
for (ll d = 1; d <= n / d; ++d)          // NOT d*d <= n  (overflows)
    if (n % d == 0) tau += (d == n / d) ? 1 : 2;
```

**2. One $n$, from its factorization — $O(\sqrt n)$ to factor, then $O(k)$.** Same cost
here, but it is the only route once $n$ passes $10^{12}$ and you reach for
[Pollard's rho](../pollard-rho/).

**3. All $m \le n$ — $O(n\log n)$.** Each divisor visits its own multiples:

```cpp
for (int d = 1; d <= n; ++d)
    for (int m = d; m <= n; m += d)
        tau[m] += 1;                     // or sigma[m] += d
```

The inner loop runs $n/d$ times and $\sum_{d\le n} n/d = nH_n = \Theta(n\log n)$, so this
is **not** quadratic. It is the most reusable skeleton in the repo — swap the body for
$\sigma$, for smallest prime factor, for Möbius, for anything indexed by divisors.

| one $n \le 10^{12}$ | trial division | $O(\sqrt n)$ |
|---|---|---|
| one $n \le 9\times10^{18}$ | [Pollard's rho](../pollard-rho/), then the formula | $O(n^{1/4})$ |
| every $m \le 10^7$ | the divisor-visiting sieve | $O(n\log n)$ |
| every $m \le 10^7$, need factorizations too | [`spf` sieve](../sieve/) | $O(n)$ build |

---

## How big does $\tau$ get

Small. That is the point, and it is what makes "iterate over all divisors" affordable.

| $n \le 10^6$ | $\max\tau = 240$ | at $720720$ |
|---|---|---|
| $n \le 10^9$ | $\max\tau = 1344$ | at $735134400$ |
| $n \le 10^{18}$ | $\max\tau = 103680$ | at $897612484786617600$ |

So enumerating every divisor of a single $n \le 10^{18}$ is at most a hundred thousand
values — cheap, **once you have the factorization**. The record-holders are the *highly
composite* numbers: many small primes with descending exponents.

$\sigma$ grows differently. $\sigma(n) < n\ln\ln n$ up to a constant, so it is barely
superlinear, but it **overflows 32 bits almost immediately** — $\sigma$ over a range of
$10^7$ needs 64-bit accumulators.

---

## How to recognize it in a problem

Look for:

* **"number of divisors"**, "count the factors", "how many $d$ divide $n$"
* **"exactly $k$ divisors"** — this is a factorization-shape condition, not a search.
  $\tau(n)=2$ means prime; $\tau(n)=3$ means $p^2$; $\tau(n)=4$ means $p^3$ or $pq$
* **"perfect square"** phrased as a parity or toggle condition — the classic locker/bulb
  problems are $\tau(n)$ being odd
* **sum of divisors**, "sum of factors", perfect / abundant / deficient numbers
* a query asking for $\tau$ or $\sigma$ **over a range** → the $O(n\log n)$ sieve, once,
  globally, then prefix sums
* **"for each $i$, count multiples of $i$"** — the same skeleton read the other way
* pairing $d$ with $n/d$, or reconstructing $n$ from a list of its divisors
* counting pairs $(a,b)$ with $ab = n$ — that is $\tau(n)$, ordered

**The tell for the sieve skeleton.** Any time you would write "for each $x$, loop over its
divisors", invert it to "for each $d$, loop over its multiples". Same answers, $O(n\log n)$
instead of $O(n\sqrt n)$.

**Anti-pattern.** If the problem needs divisors of *one* huge number, do not sieve. If it
needs them for *every* number in a range, do not trial-divide each one.

---

## Complexity

| $\tau(n)$, $\sigma(n)$, all divisors of one $n$ | $O(\sqrt n)$ |
|---|---|
| from a known factorization | $O(k)$, with $k \le 15$ distinct primes |
| $\tau$ or $\sigma$ for all $m \le n$ | $O(n\log n)$, $O(n)$ space |
| the same with an `spf` table | $O(n)$ build, $O(\log m)$ per query |
| enumerating the divisors of one $n \le 10^{18}$ | $\le 103680$ values |

---

## Pitfalls

| | |
|---|---|
| `d * d <= n` overflows | `d <= n / d` |
| the perfect square counted twice | `if (d != n / d)` |
| $\sigma$ in an `int` | overflows before $n = 10^6$; use 64-bit |
| $\tau(ab) = \tau(a)\tau(b)$ without $\gcd(a,b)=1$ | $\tau(2)\tau(2)=4$, $\tau(4)=3$ |
| forgetting the leftover prime when factoring | the `if (n > 1)` line |
| assuming the pair loop emits divisors in order | `sort` if order matters |
| the $\sigma$ formula's division $\frac{p^{e+1}-1}{p-1}$ under a modulus | needs an inverse, or sum the powers directly |
| sieving per test case | build the table **once**, globally |
| $\tau(1) = 1$ and $\sigma(1) = 1$ | the empty product, not $0$ |
| calling a divisor count "the number of prime factors" | different quantity entirely |

---

[implementation.cpp](implementation.cpp) · [problems.md](problems.md)
