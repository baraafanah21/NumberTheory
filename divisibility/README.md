# Divisibility

**Use it for:** enumerating divisors, sieves, perfect-square tests, counting multiples,
divisibility of numbers too big to parse.

**Prereq:** none. **Used by:** gcd/Euclid, primes, and essentially everything else.

---

## 1. The relation

$a \mid b$ means $b = ak$ for some integer $k$. In code: `b % a == 0`.

**Zero rules.** These cause real WAs, so memorize them:

| Case | Value | Reason |
|---|---|---|
| $a \mid 0$ | **true**, every $a$ | $0 = a \cdot 0$ |
| $0 \mid 0$ | **true** | $0 = 0 \cdot k$ |
| $0 \mid b$, $b \neq 0$ | **false** | $0 \cdot k$ is always $0$ |
| `b % 0` | **UB / crash** | guard before dividing |

---

## 2. Facts you actually use

| # | Fact | Where it shows up |
|---|---|---|
| **D1** | $d \mid a$ and $d \mid b$ $\Rightarrow$ $d \mid (ax + by)$ | Everywhere. Euclid's step, digit tests |
| **D2** | Division algorithm: unique $q, r$ with $a = qb + r$, $0 \leq r < \lvert b \rvert$ | Makes `%` well defined; C++ disagrees on signs |
| **D3** | Divisors pair as $\{d,\ n/d\}$ around $\sqrt n$ | Why enumeration is $O(\sqrt n)$ |
| **D4** | $\tau(n)$ is odd $\iff$ $n$ is a perfect square | Locker/switch problems, instantly |
| **D5** | Multiples of $d$ in $[1,n]$: $\lfloor n/d \rfloor$ of them, sum $d\frac{k(k+1)}{2}$ | $O(1)$ instead of a loop |
| **D6** | $n = \prod p_i^{e_i}$ $\Rightarrow$ $\tau(n) = \prod(e_i+1)$, $\sigma(n) = \prod\frac{p_i^{e_i+1}-1}{p_i-1}$ | When you already have the factorization |
| **D7** | **Trap:** $d \mid ab$ does **not** give $d \mid a$ or $d \mid b$ | $6 \mid 4\cdot 9$, but $6\nmid 4$, $6 \nmid 9$. Needs $d$ prime |

**Digit tests** — for numbers arriving as strings:

| Divisor | Test |
|---|---|
| $2^j$, $5^j$ | last $j$ digits only |
| $3$, $9$ | digit sum |
| $11$ | alternating digit sum |

---

## 3. Algorithms

| Task | Method | Time | Space |
|---|---|---|---|
| test $a \mid b$ | `b % a == 0` | $O(1)$ | $O(1)$ |
| floored div/mod | correct the truncation | $O(1)$ | $O(1)$ |
| all divisors of $n$ | loop `d <= n/d`, emit $d$ and $n/d$ | $O(\sqrt n)$ | $O(\tau(n))$ |
| $\tau(n)$, $\sigma(n)$ for one $n$ | same loop, no storage | $O(\sqrt n)$ | $O(1)$ |
| primality of one $n$ | trial division to $\sqrt n$ | $O(\sqrt n)$ | $O(1)$ |
| $\tau(m)$, $\sigma(m)$ for **all** $m \leq n$ | multiples sieve | $O(n\log n)$ | $O(n)$ |
| divisibility of a digit string | digit sum / alternating | $O(\text{len})$ | $O(1)$ |

**Choosing between them:** one query with $n \leq 10^{12}$ → $O(\sqrt n)$. Many queries with
$n \leq 10^6$ → sieve once, answer in $O(1)$.

**The two loops to know by heart:**

```cpp
// every divisor of n, O(sqrt n)
for (ll d = 1; d <= n / d; ++d)          // NOT d*d <= n  (overflow)
    if (n % d == 0) {
        use(d);
        if (d != n / d) use(n / d);      // guard the perfect square
    }

// tau/sigma for all m <= n, O(n log n)  -- the "iterate over multiples" skeleton
for (int d = 1; d <= n; ++d)
    for (int m = d; m <= n; m += d)
        tau[m] += 1;                     // or sigma[m] += d
```

That second skeleton is worth more than anything else in this file. It computes $\tau$,
$\sigma$, smallest prime factor, Möbius, "count multiples in range", and more — all in
$O(n\log n)$ because $\sum_d n/d = n H_n$.

---

## 4. Patterns

**Odd divisor count → perfect square.** Any problem about toggling every $k$-th item, or
about an odd number of divisors, is D4. Cell $m$ is toggled once per divisor, so it ends
flipped iff $\tau(m)$ is odd iff $m$ is a square. Turns a simulation into $\lfloor\sqrt n\rfloor$.

**Count multiples, don't iterate them.** "How many numbers in $[1,n]$ are divisible by $d$"
is $\lfloor n/d \rfloor$, $O(1)$. For a range, $\lfloor R/d \rfloor - \lfloor (L-1)/d \rfloor$.
Divisible by $a$ **or** $b$ → inclusion–exclusion with $\mathrm{lcm}(a,b)$.

**Swap the summation order.** $\sum_{m \leq n}\sum_{d \mid m} f(d)$ becomes
$\sum_{d \leq n} f(d)\lfloor n/d \rfloor$. Same idea as the sieve skeleton, used
algebraically — routinely turns $O(n\sqrt n)$ into $O(n \log n)$.

**Divisor-block / floor trick.** $\lfloor n/d \rfloor$ takes only $O(\sqrt n)$ distinct
values. Group equal values into blocks to evaluate $\sum_{d=1}^{n} \lfloor n/d\rfloor$
in $O(\sqrt n)$ instead of $O(n)$:

```cpp
for (ll l = 1, r; l <= n; l = r + 1) {
    r = n / (n / l);              // largest r with n/r == n/l
    ans += (r - l + 1) * (n / l); // the whole block at once
}
```

**Huge numbers as strings.** Input with $10^5$ digits cannot be parsed. Digit sum handles
$3$ and $9$, alternating sum handles $11$, last $j$ digits handle $2^j$ and $5^j$.

**"Same remainder" → subtract.** $a \equiv b \pmod d$ iff $d \mid (a-b)$. Eliminates the
unknown remainder and hands the problem to a gcd.

---

## 5. Pitfalls

| # | Mistake | Fix |
|---|---|---|
| 1 | `d * d <= n` overflows near $9\cdot10^{18}$ | `d <= n / d` |
| 2 | Perfect square emitted twice | `if (d != n / d)` |
| 3 | `-17 % 5 == -2` in C++, used as an index | `((a % b) + b) % b` |
| 4 | `n % 0` | guard $b \neq 0$ |
| 5 | `(int)sqrt(n)` off by one near squares | integer-correct it, or use `d <= n/d` |
| 6 | Divisors come out **unsorted** from the pairing loop | `sort` if order matters |
| 7 | $\sigma$ overflowing an `int` array | 64-bit accumulator |
| 8 | Assuming $d \mid ab \Rightarrow d \mid a$ or $d \mid b$ | Only true for prime $d$ (D7) |
| 9 | Forgetting $a \mid 0$ is true | check the zero table above |
| 10 | Floating point divisibility (`n/d*d == n` on doubles) | integer `%` only |

---

## 6. Code

[implementation.cpp](implementation.cpp) — all of it, compiling and verified:

| Function | Purpose |
|---|---|
| `divides(a, b)` | zero-safe divisibility test |
| `floorDiv`, `floorMod` | mathematical (non-truncating) division |
| `divisionAlgorithm(a, b)` | the unique $(q, r)$ with $0 \leq r < \lvert b \rvert$ |
| `divisors(n)` | all divisors, sorted, $O(\sqrt n)$ |
| `countDivisors(n)`, `sumDivisors(n)` | $\tau$, $\sigma$ for one $n$ |
| `isPerfectSquare(n)` | integer-exact, no `sqrt` rounding bug |
| `isPrimeTrialDivision(n)` | $O(\sqrt n)$ |
| `divisorCountSieve(n)`, `divisorSumSieve(n)` | $\tau$, $\sigma$ for all $m \leq n$ |
| `divisibleBy3/9/11(string)` | digit tests for unparseable numbers |

---

## 7. Why it works

[proofs.md](proofs.md) — kept short, and only where a proof changes what you write:
why $\sqrt n$ enumeration is complete, why the sieve is $O(n\log n)$ and not $O(n^2)$,
why `b % a == 0` is a valid test, why the digit tests are correct, and why D7 fails.
