# Arithmetic and Geometric Progressions

A **progression** is a sequence built by repeating one operation.

| Type | Rule | Example |
|---|---|---|
| **Arithmetic (AP)** | keep **adding** $d$ | $3,\ 7,\ 11,\ 15,\ 19$ &nbsp; ($d = 4$) |
| **Geometric (GP)** | keep **multiplying** by $r$ | $3,\ 6,\ 12,\ 24,\ 48$ &nbsp; ($r = 2$) |

**You need this for:** summing such a sequence *without looping*. When a problem says
$n \le 10^{18}$, a loop is impossible and a formula must exist.

**Before this:** nothing. **After this:** [divisibility](../divisibility/).

---

## Key formulas

Everywhere below:

- $a$ = the first term
- $n$ = how many terms
- $S_n$ = the sum of those $n$ terms

### Arithmetic progression (common difference $d$)

$$a_n = a + (n-1)\,d$$

$$S_n \;=\; \frac{n}{2}\Bigl(2a + (n-1)d\Bigr) \;=\; \underbrace{n}_{\text{how many}} \times \underbrace{\frac{a_1 + a_n}{2}}_{\text{average term}}$$

Number of terms from first to last:

$$n \;=\; \frac{\text{last} - \text{first}}{d} \;+\; 1$$

> The $+1$ is the most common bug in this whole topic. $\frac{\text{last}-\text{first}}{d}$
> counts the **steps**; the number of **terms** is one more.

**Special case worth memorizing:**

$$1 + 2 + 3 + \cdots + n = \frac{n(n+1)}{2}$$

### Geometric progression (common ratio $r$)

$$a_n = a\,r^{\,n-1}$$

$$S_n = a\cdot\frac{r^{\,n} - 1}{r - 1} \quad (r \neq 1), \qquad\qquad S_n = n\,a \quad (r = 1)$$

> The $r = 1$ case is not a technicality — the formula divides by $r-1 = 0$. Constant
> sequences appear as input all the time, so handle it or crash.

### One identity that keeps coming back

$$\boxed{\,(a - b)\ \big|\ (a^n - b^n)\,}$$

For example $a = 2$, $b = 1$: $\ 1 \mid (2^n - 1)$, and more usefully
$(2^d - 1) \mid (2^n - 1)$ whenever $d \mid n$.

---

## What you actually implement

Only three things from this topic show up in code.

### 1. AP sum in $O(1)$, kept exact

$S_n = \frac{n(2a+(n-1)d)}{2}$ has a division by $2$. In integer arithmetic that
truncates — unless you halve the factor that is *actually even*.

**Exactly one of $n$ and $(2a+(n-1)d)$ is even** (proved in [proofs.md](proofs.md) §2), so:

```cpp
ll apSum(ll a, ll d, ll n) {
    if (n <= 0) return 0;
    ll bracket = 2*a + (n-1)*d;
    return (n % 2 == 0) ? (n/2) * bracket    // n is even
                        : n * (bracket/2);   // then the bracket is even
}
```

Exact, and one bit safer against overflow than multiplying first.

### 2. Sum of multiples — $O(1)$ instead of a loop

The multiples of $d$ in $[1, n]$ are $d, 2d, \dots, kd$ with $k = \lfloor n/d\rfloor$ —
an AP. So:

$$\text{count} = \Bigl\lfloor \frac{n}{d} \Bigr\rfloor, \qquad
\text{sum} = d\cdot\frac{k(k+1)}{2}$$

### 3. Geometric sum modulo $m$ — without dividing

You **cannot** compute $\frac{r^n-1}{r-1} \bmod m$ in general: dividing under a modulus
needs $(r-1)^{-1}$, which only exists when $\gcd(r-1, m) = 1$. Split the sum instead.

With $G(n) = 1 + r + r^2 + \cdots + r^{\,n-1}$:

$$G(2k) = G(k)\cdot\bigl(1 + r^{\,k}\bigr), \qquad\qquad G(n) = G(n-1) + r^{\,n-1}\ \ (n\text{ odd})$$

Only $+$ and $\times$, so it works for **any** modulus. $O(\log^2 n)$.

---

## Complexity

| Task | Time |
|---|---|
| AP term, AP sum, sum of multiples | $O(1)$ |
| GP term, GP sum, $r^n \bmod m$ | $O(\log n)$ |
| geometric sum $\bmod\ m$ | $O(\log^2 n)$ |

---

## Common mistakes

| Mistake | Fix |
|---|---|
| Forgetting the $+1$ in the term count | steps $+\,1$ = terms |
| Writing $a + nd$ for the $n$-th term | it is $a + (n-1)d$ |
| Forgetting the $r = 1$ case of the GP sum | division by zero |
| `n/2 * (...)` when $n$ is odd | halve the even factor |
| `n * (n+1)` overflowing before `/2` | divide first |
| Dividing by $r-1$ under a modulus | use the split above |
| `std::pow` for integer powers | returns `double`, wrong past $2^{53}$ |

---

## Files

- [proofs.md](proofs.md) — where every formula above comes from.
- [implementation.cpp](implementation.cpp) — the five functions that matter.
