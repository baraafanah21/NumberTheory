# Divisibility

$$a \mid b \quad\Longleftrightarrow\quad b = a\,k \ \text{ for some integer } k$$

Defined by **multiplication**, not division — no fractions appear anywhere. In code:
`b % a == 0`.

**Use:** divisors, sieves, perfect-square tricks. **Next:**
[gcd](../gcd-and-euclidean-algorithm/)

---

## The zero cases

| $a \mid 0$ | **true** for every $a$ | $0 = a\cdot0$ |
|---|---|---|
| $0 \mid 0$ | **true** | $0 = 0\cdot k$ |
| $0 \mid b,\ b\neq0$ | **false** | $0\cdot k$ is always $0$ |

And `b % 0` is undefined behaviour — guard it.

---

## Key facts

**Linearity.** $d \mid a$ and $d \mid b$ $\Rightarrow$ $d \mid (ax+by)$ for all $x,y$.
The workhorse of the whole subject.

**Division algorithm.** For $b \neq 0$ there are **unique** $q, r$ with

$$a = qb + r, \qquad 0 \le r < |b|$$

Uniqueness is what makes `b % a == 0` an if-and-only-if test. Note C++ disagrees on
signs: `-17 % 5 == -2`, while the true remainder is $3$. Fix with `((a % b) + b) % b`.

**Divisors pair up.** If $d \mid n$ then $n/d \mid n$, and $\min(d,\ n/d) \le \sqrt n$ —
so a loop to $\sqrt n$ meets every pair. For $n=36$: $1{\times}36$, $2{\times}18$,
$3{\times}12$, $4{\times}9$, $6{\times}6$.

**$\tau(n)$ is odd $\iff n$ is a perfect square** — pairs contribute $2$ each, unless some
divisor is its own partner ($d = n/d$).

**Trap.** $d \mid ab$ does **not** give $d \mid a$ or $d \mid b$: $6 \mid 4{\times}9$ but
$6$ divides neither. It needs $d$ **prime** — see
[gcd](../gcd-and-euclidean-algorithm/).

---

## The two loops

```cpp
// every divisor of n -- O(sqrt n)
for (ll d = 1; d <= n / d; ++d)          // NOT d*d <= n  (overflows)
    if (n % d == 0) {
        use(d);
        if (d != n / d) use(n / d);      // don't emit sqrt(n) twice
    }

// tau/sigma for all m <= n -- O(n log n).  Each divisor visits its own multiples.
for (int d = 1; d <= n; ++d)
    for (int m = d; m <= n; m += d)
        tau[m] += 1;                     // or sigma[m] += d
```

The second looks quadratic and is not: the inner loop runs $n/d$ times, and
$\sum_{d\le n} n/d = n H_n = \Theta(n\log n)$. **This skeleton is the most reusable idea
here.**

**Counting multiples** of $d$ in $[1,n]$ is $\lfloor n/d\rfloor$ — $O(1)$, no loop.

**Digit tests**, for numbers too big to parse: digit sum for $3$ and $9$, alternating digit
sum for $11$, last $j$ digits for $2^j$ and $5^j$.

---

## Complexity

| test $a \mid b$ | $O(1)$ |
|---|---|
| divisors, $\tau$, $\sigma$, primality of one $n$ | $O(\sqrt n)$ |
| $\tau(m)$ for **all** $m \le n$ | $O(n\log n)$, $O(n)$ space |
| digit test on a string | $O(\text{length})$ |

One query with $n \le 10^{12}$ → $O(\sqrt n)$. Many queries with $n \le 10^6$ → sieve once.

---

## Pitfalls

| | |
|---|---|
| `d * d <= n` overflows | `d <= n / d` |
| perfect square counted twice | `if (d != n / d)` |
| `-17 % 5 == -2` used as an index | `((a % b) + b) % b` |
| `n % 0` | guard $b \neq 0$ |
| the pair loop emits unsorted | `sort` if order matters |
| $\sigma$ overflowing an `int` | use 64-bit |
| $d \mid ab \Rightarrow d \mid a$ or $d \mid b$ | false unless $d$ is prime |

---

[proofs.md](proofs.md) · [implementation.cpp](implementation.cpp)
