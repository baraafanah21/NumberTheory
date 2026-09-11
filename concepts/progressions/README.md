# Progressions

A sequence built by repeating one operation: **add** $d$ each step (arithmetic) or
**multiply** by $r$ each step (geometric).

$$3,\ 7,\ 11,\ 15 \quad (d=4) \qquad\qquad 3,\ 6,\ 12,\ 24 \quad (r=2)$$

**Use:** summing a sequence when $n$ is too large to loop. **Next:**
[divisibility](../divisibility/) · [fast power](../fast-power/)

---

## Key formulas

$a$ = first term, $n$ = number of terms, $S_n$ = sum of the first $n$.

| | Arithmetic | Geometric |
|---|---|---|
| $n$-th term | $a + (n-1)d$ | $a\,r^{\,n-1}$ |
| sum $S_n$ | $\dfrac{n}{2}\bigl(2a + (n-1)d\bigr)$ | $a\dfrac{r^{\,n}-1}{r-1}$ &nbsp;($r\neq1$), else $na$ |

$$\text{number of terms} = \frac{\text{last}-\text{first}}{d} + 1
\qquad\qquad 1+2+\cdots+n = \frac{n(n+1)}{2}$$

**The identity that keeps reappearing:**

$$\boxed{\ (a-b) \ \big|\ (a^n - b^n)\ }$$

so $(2^d-1) \mid (2^n-1)$ whenever $d \mid n$ — a composite exponent makes $2^n-1$
composite.

---

## What you implement

**AP sum, kept exact.** Exactly one of $n$ and $(2a+(n-1)d)$ is even, so halve *that* one:

```cpp
ll apSum(ll a, ll d, ll n) {
    ll bracket = 2*a + (n-1)*d;
    return (n % 2 == 0) ? (n/2) * bracket : n * (bracket/2);
}
```

**Sum of multiples of $d$ up to $n$** — an AP, so $O(1)$ with $k = \lfloor n/d\rfloor$:

$$\text{count} = k, \qquad \text{sum} = d\,\frac{k(k+1)}{2}$$

**Geometric sum mod $m$ — without dividing.** $\frac{r^n-1}{r-1}$ needs $(r-1)^{-1}$, which
exists only when $\gcd(r-1,m)=1$. Split instead, with $G(n) = 1+r+\cdots+r^{\,n-1}$:

$$G(2k) = G(k)\bigl(1+r^{\,k}\bigr), \qquad G(n) = G(n-1) + r^{\,n-1}\ (n \text{ odd})$$

Only $+$ and $\times$, so it is valid for **any** modulus.

---

## How to recognize it in a problem

Look for:

* **"sum of the first $n$"** with $n$ up to $10^{18}$ — a loop is impossible, so a closed
  form exists and the problem is about finding it
* evenly spaced values: consecutive integers, every $k$-th element, "all multiples of $d$"
* **"sum of all multiples of $d$ below $n$"** — an AP with $k = \lfloor n/d\rfloor$ terms,
  answered in $O(1)$
* a quantity that **doubles or halves** each step → geometric, not arithmetic
* expressions shaped like $2^n - 1$, $r^n - 1$, or "a number made of $n$ copies of a digit"
  → the identity $(a-b) \mid (a^n - b^n)$
* **"write $n$ as a sum of consecutive integers"** — solve $n = \frac{k(2a+k-1)}{2}$ over
  the divisors of $2n$
* triangular numbers, handshakes, pairs from $n$ items → $\frac{n(n+1)}{2}$
* an average or a mean stated as a constraint — multiply through and it becomes a sum

**The tell.** The input is a formula's worth of parameters, not an array. When $n$ is a
*bound* rather than a list length, you are being asked for a closed form.

**Anti-pattern.** If the terms are not evenly spaced and not in constant ratio, this is not
a progression — do not force it. And if the closed form needs a division under a modulus,
use the splitting recurrence above rather than reaching for an inverse that may not exist.

---

## Complexity

| AP term, AP sum, sum of multiples | $O(1)$ |
|---|---|
| GP sum, $r^n \bmod m$ | $O(\log n)$ |
| geometric sum $\bmod\ m$ | $O(\log^2 n)$ |

---

## Pitfalls

| | |
|---|---|
| dropping the $+1$ in the term count | steps $+1$ = terms |
| $a+nd$ for the $n$-th term | it is $a+(n-1)d$ |
| forgetting $r=1$ in the GP sum | division by zero |
| `n/2 * (...)` with odd $n$ | halve the even factor |
| `n*(n+1)` overflowing before `/2` | divide first |
| dividing by $r-1$ under a modulus | use the split above |
| `std::pow` for integer powers | returns `double`, wrong past $2^{53}$ |

---

[proofs.md](proofs.md) · [implementation.cpp](implementation.cpp) · [problems.md](problems.md)
