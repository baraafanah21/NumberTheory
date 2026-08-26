# Divisibility

$a$ **divides** $b$ — written $a \mid b$ — when $b$ is a whole number of copies of $a$:

$$a \mid b \quad\Longleftrightarrow\quad b = a\,k \ \text{ for some integer } k$$

Notice the definition uses **multiplication**, not division. No fractions appear anywhere.
In code the test is `b % a == 0`.

**You need this for:** finding divisors, sieves, perfect-square tricks, counting multiples.

**Before this:** nothing. **After this:** [gcd](../gcd-and-euclidean-algorithm/).

---

## The zero cases

These three catch everyone once. All follow straight from $b = ak$:

| Question | Answer | Why |
|---|---|---|
| $a \mid 0$ ? | **yes**, for every $a$ | $0 = a \times 0$ |
| $0 \mid 0$ ? | **yes** | $0 = 0 \times k$ |
| $0 \mid 5$ ? | **no** | $0 \times k$ is always $0$ |

And `b % 0` is undefined behaviour in C++ — guard it.

---

## The four facts

### 1. Linearity

$$\text{if } d \mid a \ \text{ and } \ d \mid b, \quad\text{then}\quad d \mid (a\,x + b\,y) \ \text{ for all integers } x, y$$

Anything dividing two numbers divides every combination of them. This is the single most
used fact in number theory — the Euclidean algorithm and the digit tests are both just
this.

### 2. The division algorithm

For any $a$ and any $b \neq 0$ there are **unique** integers $q$ and $r$ with

$$a = q\,b + r, \qquad 0 \le r < |b|$$

$q$ is the quotient, $r$ the remainder. Uniqueness is what makes "*the* remainder"
meaningful — and what makes `b % a == 0` a valid divisibility test.

> **C++ disagrees on signs.** The mathematical remainder is never negative, but C++ gives
> `-17 % 5 == -2` (the sign follows the dividend). The true remainder is $3$, since
> $-17 = (-4)\times 5 + 3$. Fix with `((a % b) + b) % b`.

### 3. Divisors come in pairs

If $d$ divides $n$, then so does $n/d$, and

$$d \times \frac{n}{d} = n \qquad\Longrightarrow\qquad \min\left(d,\ \frac{n}{d}\right) \le \sqrt{n}$$

So one of every pair is at most $\sqrt n$. **This is why you only loop to $\sqrt n$.**

Example, $n = 36$: &nbsp; $1\!\times\!36$, &nbsp; $2\!\times\!18$, &nbsp; $3\!\times\!12$,
&nbsp; $4\!\times\!9$, &nbsp; $6\!\times\!6$ — checking only $1$ to $6$ finds all nine
divisors.

### 4. Odd divisor count means perfect square

$$\tau(n) \text{ is odd} \quad\Longleftrightarrow\quad n \text{ is a perfect square}$$

($\tau(n)$ = how many positive divisors $n$ has.) Because divisors pair up, the count is
even — *unless* some divisor is its own partner, which happens exactly when $d = n/d$, i.e.
$n = d^2$.

---

## What you actually implement

### All divisors, in $O(\sqrt n)$

```cpp
for (ll d = 1; d <= n / d; ++d)          // NOT d*d <= n  -- that overflows
    if (n % d == 0) {
        use(d);
        if (d != n / d) use(n / d);      // guard: don't emit sqrt(n) twice
    }
```

Same loop counts $\tau(n)$ or sums $\sigma(n)$. Also gives primality: if no
$d \in [2, \sqrt n]$ divides $n$, then $n$ is prime.

### Everything up to $n$ at once, in $O(n\log n)$

Instead of asking each number for its divisors, let each divisor visit its multiples:

```cpp
for (int d = 1; d <= n; ++d)
    for (int m = d; m <= n; m += d)
        tau[m] += 1;                     // or sigma[m] += d
```

This looks quadratic and is not: the inner loop runs $n/d$ times, and

$$\sum_{d=1}^{n}\frac{n}{d} \;=\; n\left(1 + \frac12 + \frac13 + \cdots + \frac1n\right) \;=\; \Theta(n\log n)$$

**This "iterate over multiples" skeleton is the most reusable idea in the file.**

### Counting multiples — $O(1)$

$$\#\{\text{multiples of } d \text{ in } [1,n]\} = \Bigl\lfloor \frac{n}{d}\Bigr\rfloor$$

### Divisibility of numbers too big to parse

If the input arrives as a $10^5$-digit string, you cannot store it in any integer type.
You do not have to:

| Divisor | Test |
|---|---|
| $3$ or $9$ | the **digit sum** has the same divisibility |
| $11$ | the **alternating** digit sum ($+\,-\,+\,-\,\ldots$ from the right) |
| $2^j$ or $5^j$ | just the last $j$ digits |

---

## Complexity

| Task | Time | Space |
|---|---|---|
| test $a \mid b$ | $O(1)$ | $O(1)$ |
| all divisors of $n$ | $O(\sqrt n)$ | $O(\tau(n))$ |
| $\tau(n)$, $\sigma(n)$, primality of one $n$ | $O(\sqrt n)$ | $O(1)$ |
| $\tau(m)$ for **all** $m \le n$ | $O(n\log n)$ | $O(n)$ |
| digit test on a string | $O(\text{length})$ | $O(1)$ |

**Choosing:** one query with $n \le 10^{12}$ → $O(\sqrt n)$. Many queries with
$n \le 10^6$ → sieve once, then answer in $O(1)$.

---

## Common mistakes

| Mistake | Fix |
|---|---|
| `d * d <= n` overflows | `d <= n / d` |
| Perfect square counted twice | `if (d != n / d)` |
| `-17 % 5 == -2` used as an index | `((a % b) + b) % b` |
| `n % 0` | guard $b \neq 0$ |
| Divisors come out unsorted from the pair loop | `sort` if order matters |
| $\sigma$ overflowing an `int` | use 64-bit |
| Assuming $d \mid ab \Rightarrow d \mid a$ or $d \mid b$ | **false**: $6 \mid 4\times9$ but $6\nmid4$, $6\nmid9$ |

That last one is worth staring at. It only becomes true when $d$ is **prime** — and proving
that needs the [gcd concept](../gcd-and-euclidean-algorithm/), not this one.

---

## Files

- [proofs.md](proofs.md) — why $\sqrt n$ is enough, why the sieve isn't quadratic, why the
  digit tests work.
- [implementation.cpp](implementation.cpp) — the six functions that matter.
