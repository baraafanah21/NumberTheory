# Arithmetic and Geometric Progressions

**Use it for:** closed-form sums when $n$ is too big to loop, counting/summing multiples,
geometric sums under a modulus, complexity analysis, and anything shaped $a^n - 1$.

**Prereq:** none. **Used by:** $\sigma(n)$, Mersenne numbers, sieve complexity, binary
exponentiation.

---

## 1. Formulas

**AP** — constant difference $d$:

$$a_n = a + (n-1)d, \qquad S_n = \frac{n}{2}\bigl(2a + (n-1)d\bigr) = \frac{n}{2}(a_1 + a_n)$$

$$\text{number of terms} = \frac{\text{last} - \text{first}}{d} + 1 \qquad \textbf{(the +1 is the classic bug)}$$

**GP** — constant ratio $r$:

$$a_n = a\,r^{\,n-1}, \qquad S_n = a\,\frac{r^{\,n} - 1}{r - 1} \ \ (r \neq 1), \qquad S_n = na \ \ (r = 1)$$

**The identity that matters most:**

$$a^n - b^n = (a-b)\sum_{i=0}^{n-1}a^i b^{\,n-1-i} \qquad\Longrightarrow\qquad (a-b) \mid (a^n - b^n)$$

---

## 2. Facts you actually use

| # | Fact | Where it shows up |
|---|---|---|
| **P1** | $1 + 2 + \cdots + n = \frac{n(n+1)}{2}$ | everywhere; nested-loop complexity |
| **P2** | Multiples of $d$ in $[1,n]$: $\lfloor n/d\rfloor$ of them, sum $d\frac{k(k+1)}{2}$ | "sum everything divisible by…" |
| **P3** | $(a-b) \mid (a^n - b^n)$ | $2^n-1$, repunits, $a^n-1$ factoring |
| **P4** | $d \mid n \Rightarrow (2^d-1) \mid (2^n-1)$; so $n$ composite $\Rightarrow 2^n-1$ composite | Mersenne. **Converse false:** $2^{11}-1 = 23\cdot 89$ |
| **P5** | $\sigma(p^k) = 1 + p + \cdots + p^k = \frac{p^{k+1}-1}{p-1}$ | divisor sums from a factorization |
| **P6** | $1 + 2 + 4 + \cdots + 2^k = 2^{k+1}-1 < 2\cdot 2^k$ | amortized doubling; $n + \frac n2 + \frac n4 < 2n$ |
| **P7** | $S_n$ of an AP is quadratic in $n$ **with no constant term** | recognize an AP from its partial sums |
| **P8** | Repunit $R_n = \frac{10^n-1}{9}$; $d \mid n \Rightarrow R_d \mid R_n$ | repunit problems |

**Two complexity sums to recognize instantly:**

$$1 + 2 + \cdots + n = \Theta(n^2) \qquad\text{vs}\qquad n + \tfrac n2 + \tfrac n4 + \cdots < 2n = \Theta(n)$$

Telling these apart is often the entire complexity analysis.

---

## 3. Algorithms

| Task | Time | Space |
|---|---|---|
| AP term / AP sum | $O(1)$ | $O(1)$ |
| GP term / GP sum (exact) | $O(\log n)$ | $O(1)$ |
| $r^{\,n} \bmod m$ | $O(\log n)$ | $O(1)$ |
| $\sum_{i<n} r^i \bmod m$ | $O(\log^2 n)$ | $O(\log n)$ |
| detect AP / GP in an array | $O(n)$ | $O(1)$ |
| sum of multiples in $[L,R]$ | $O(1)$ | $O(1)$ |

### The exact-integer AP sum

`n/2 * bracket` truncates when $n$ is odd. But **exactly one of $n$ and $(2a+(n-1)d)$ is
even** — $2a$ is even, so the bracket has the parity of $(n-1)d$. Test and halve the even
one:

```cpp
ll apSum(ll a, ll d, ll n) {
    if (n <= 0) return 0;
    ll bracket = 2*a + (n-1)*d;
    return (n % 2 == 0) ? (n/2) * bracket : n * (bracket/2);
}
```

Exact *and* one bit safer against overflow. `1 + 2 + ... + 1e9` comes out right.

### Geometric sum mod $m$, without a modular inverse

**You cannot** use $(r^n-1)\cdot(r-1)^{-1}$: $r-1$ need not be invertible mod $m$ (only
when $\gcd(r-1,m)=1$). Composite moduli break it. Split the sum instead — with
$G(n) = 1 + r + \cdots + r^{\,n-1}$:

$$G(2k) = G(k)\bigl(1 + r^{\,k}\bigr), \qquad G(n) = G(n-1) + r^{\,n-1} \ (n \text{ odd})$$

```cpp
ll geometricSumMod(ll r, ll n, ll mod) {
    if (n <= 0) return 0;
    if (n & 1) return (geometricSumMod(r, n-1, mod) + powerMod(r, n-1, mod)) % mod;
    ll half = geometricSumMod(r, n/2, mod);
    return mulMod(half, (1 + powerMod(r, n/2, mod)) % mod, mod);
}
```

Only `+` and `*`, so it is valid for **any** modulus. $O(\log^2 n)$.

---

## 4. Patterns

**Huge $n$ + regular sequence = closed form.** If $n \leq 10^{18}$ the problem is telling
you no loop exists. Find the formula.

**Counting/summing multiples.** $\lfloor n/d\rfloor$ and $d\frac{k(k+1)}{2}$, $O(1)$.
Combine with inclusion–exclusion: multiples of 3 or 5 = (3) + (5) − (15).

**Anything shaped $a^n - 1$ or $2^n - 1$ or $11\ldots1$.** That is P3 in disguise.
Before primality-testing $2^n-1$, check whether $n$ is composite (P4) — it settles the
question for free.

**Prime-power divisor sums.** Once you have $n = \prod p_i^{e_i}$, each factor's
contribution to $\sigma(n)$ is a GP sum (P5). That is how $\sigma$ is actually computed.

**Recognize an AP from data.** Constant first differences → AP. Constant *second*
differences → quadratic → by P7 it is the partial-sum sequence of an AP, and you can read
$d = 2A$, $a_1 = A + B$ straight off $S_n = An^2 + Bn$.

**Telescoping.** If you can write the summand as $f(i+1) - f(i)$, the sum is
$f(n) - f(0)$. The GP sum proof is exactly this.

**Detect a GP without floating point.** Test $a_{i+1}a_{i-1} = a_i^2$ by cross-multiplying
(in `__int128` if values are large), and reject zero terms separately — $1, 0, 5$ satisfies
the equation but is not a GP.

---

## 5. Pitfalls

| # | Mistake | Fix |
|---|---|---|
| 1 | Dropping the $+1$ in the term count | steps $+1$ = terms |
| 2 | Using $a + nd$ for the $n$-th term | it is $a + (n-1)d$ |
| 3 | Forgetting the $r = 1$ case in the GP sum | division by zero; constant sequences are common input |
| 4 | `n/2 * (...)` truncating when $n$ is odd | halve the even factor (§3) |
| 5 | `n * (n+1)` overflowing before the `/2` | divide first, or use `__int128` |
| 6 | Dividing by $r-1$ under a modulus | use the recursion in §3 |
| 7 | `std::pow` for integer powers | returns `double`, wrong past $2^{53}$; write binary exponentiation |
| 8 | Ratios in `double` to detect a GP | cross-multiply in integers |
| 9 | Negative intermediates before `%` | `((x % m) + m) % m` |
| 10 | Assuming $2^p-1$ is prime when $p$ is | $2^{11}-1 = 23\cdot89$ |

---

## 6. Code

[implementation.cpp](implementation.cpp):

| Function | Purpose |
|---|---|
| `apTerm`, `apSum`, `apCount` | AP closed forms, overflow-aware |
| `gpTerm`, `gpSum` | GP closed forms, `r == 1` handled |
| `powerMod`, `mulMod` | binary exponentiation, safe for any 63-bit modulus |
| `geometricSumMod` | division-free, works for composite moduli |
| `sumOfMultiplesUpTo`, `sumOfMultiplesInRange` | P2 |
| `isArithmetic`, `isGeometric` | $O(n)$ detection, no floating point |
| `sigmaPrimePower` | P5 |
| `mersenneFactorFromExponent` | P4 — a free factor when the exponent is composite |

---

## 7. Why it works

[proofs.md](proofs.md) — short: the two sum formulas, why the integer AP sum is exact, why
$(a-b) \mid (a^n-b^n)$, and why the division-free geometric sum recursion is correct.
