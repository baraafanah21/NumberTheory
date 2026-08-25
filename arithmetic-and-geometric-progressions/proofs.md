# Why it works — Progressions

Only the proofs that change what you write.

---

## 1. The AP sum (P1)

> $$S_n = \frac{n}{2}\bigl(2a + (n-1)d\bigr) = \frac{n}{2}(a_1 + a_n)$$

Write the sum forwards and backwards and add columns. The $k$-th column is

$$a_k + a_{n+1-k} = \bigl[a + (k-1)d\bigr] + \bigl[a + (n-k)d\bigr] = 2a + (n-1)d$$

The $k$ cancels, so all $n$ columns are equal and $2S_n = n\bigl(2a+(n-1)d\bigr)$.

**Remember it as:** *number of terms × average of the two ends.* Both other forms follow.

**Justifies:** `apSum`, and $1+2+\cdots+n = \frac{n(n+1)}{2}$ with $a = d = 1$.

---

## 2. Why the integer AP sum is exact (and how to keep it that way)

> Exactly one of $n$ and $\bigl(2a + (n-1)d\bigr)$ is even.

$2a$ is even, so the bracket has the parity of $(n-1)d$. If $n$ is odd, $n-1$ is even, so
the bracket is even. If $n$ is even, $n$ is. Either way the product $n \cdot (\text{bracket})$
is divisible by $2$, so $S_n$ is an integer.

**Justifies:** halving *the even factor* before multiplying:

```cpp
return (n % 2 == 0) ? (n/2) * bracket : n * (bracket/2);
```

Writing `n/2 * bracket` unconditionally truncates for odd $n$; multiplying first risks
overflow. This version is exact and buys a bit of headroom.

---

## 3. The GP sum, and why $r = 1$ is a real case (P5, P6)

> $S_n = a\dfrac{r^{\,n}-1}{r-1}$ for $r \neq 1$; $S_n = na$ for $r = 1$.

Multiply $S_n = a + ar + \cdots + ar^{\,n-1}$ by $r$. That shifts every term one place, so
subtracting leaves only the ends — the sum **telescopes**:

$$rS_n - S_n = ar^{\,n} - a \qquad\Longrightarrow\qquad S_n(r-1) = a\bigl(r^{\,n}-1\bigr)$$

For $r \neq 1$, divide. For $r = 1$ the line reads $0 = 0$ and carries no information —
this is not a technicality, the derivation genuinely fails. Handle it directly: every term
equals $a$, so $S_n = na$.

**Justifies:** the `if (r == 1) return n * a;` branch. Constant sequences are common input.

**Corollary (P5):** the divisors of $p^k$ are $p^0, \dots, p^k$, a GP, so

$$\sigma(p^k) = \frac{p^{\,k+1}-1}{p-1}$$

**Corollary (P6):** $1 + 2 + \cdots + 2^k = 2^{k+1} - 1$ — a doubling GP is dominated by its
last term, which is why amortized doubling costs $O(n)$ total and why a $k$-bit unsigned
value maxes at $2^k - 1$.

---

## 4. $(a-b) \mid (a^n - b^n)$ (P3)

> $$a^n - b^n = (a-b)\sum_{i=0}^{n-1}a^i b^{\,n-1-i}$$

Let $T$ be that sum. Expanding $(a-b)T$ gives two sums with the same summand
$a^k b^{\,n-k}$, one running $k = 1..n$ and the other $k = 0..n-1$. Everything in
$1..n-1$ appears in both and cancels, leaving $a^n b^0 - a^0 b^n = a^n - b^n$. The cofactor
$T$ is a sum of products of integers, hence an integer.

**Check:** $a=5, b=2, n=3$: $T = 25+10+4 = 39$ and $3 \cdot 39 = 117 = 125-8$ ✓

**Justifies (P4):** if $n = de$ then $2^n - 1 = (2^d)^e - 1^e$, so $(2^d-1) \mid (2^n-1)$.
For composite $n$ pick $1 < d < n$: then $1 < 2^d-1 < 2^n-1$, a **proper** factor — so
$2^n-1$ is composite. Check the exponent before running any primality test on $2^n-1$.

*One direction only.* $n$ prime does **not** make $2^n-1$ prime: $2^{11}-1 = 2047 = 23\cdot89$.

**Justifies (P8):** the same argument in base 10 gives $d \mid n \Rightarrow R_d \mid R_n$
for repunits.

---

## 5. The division-free geometric sum (the modular algorithm)

> With $G(n) = \sum_{i=0}^{n-1} r^i$: $\quad G(2k) = G(k)\bigl(1+r^{\,k}\bigr)$ and
> $G(n) = G(n-1) + r^{\,n-1}$.

Split the $2k$ terms into two blocks of $k$. The second block is the first scaled by
$r^{\,k}$:

$$\sum_{i=k}^{2k-1} r^i = r^{\,k}\sum_{j=0}^{k-1}r^{\,j} = r^{\,k}G(k)$$

so $G(2k) = G(k) + r^{\,k}G(k)$. The odd case is just peeling the last term.

**Why this is the version you want.** The closed form needs $(r-1)^{-1} \bmod m$, which
**does not exist** unless $\gcd(r-1, m) = 1$. This recursion uses only $+$ and $\times$, so
it is valid for every modulus — including the composite ones problem setters choose
specifically to break the naive approach.

**Cost:** (a) halves $n$, (b) makes $n$ even so (a) applies next, so $O(\log n)$ calls each
doing one $O(\log n)$ power: $O(\log^2 n)$.

---

## 6. Recognizing an AP from its partial sums (P7)

> $(a_n)$ is an AP $\iff$ $S_n = An^2 + Bn$ (quadratic, **no constant term**), and then
> $d = 2A$, $a_1 = A + B$.

Forward: expand §1 to $S_n = \frac{d}{2}n^2 + (a - \frac d2)n$.

Backward: difference the partial sums. For $n \geq 2$,

$$a_n = S_n - S_{n-1} = A\bigl(n^2 - (n-1)^2\bigr) + B = 2An + (B-A)$$

which is linear in $n$, so the differences are constant ($= 2A$). It also holds at $n=1$,
since $S_1 = A + B$ matches.

**Why the constant term must vanish:** with $S_n = An^2+Bn+C$, differencing still gives
$a_n = 2An + (B-A)$ for $n \geq 2$, but $a_1 = S_1 = A+B+C$ while the formula predicts
$A+B$. They agree only if $C = 0$. So $S_n = n^2+n+5$ is *not* an AP's partial sums.

**Justifies:** reading $a$ and $d$ straight off a quadratic $S_n$ instead of solving for them.

---

## 7. Facts used without proof

Quick to check, nothing depends on the argument:

- **Three-term tests:** $x,y,z$ in AP $\iff 2y = x+z$; nonzero $x,y,z$ in GP $\iff y^2 = xz$.
  Both are just "the two consecutive steps agree", rearranged to avoid division.
  *In code, use these forms* — no floating point, no zero-divisor case. (The GP test needs
  the nonzero check separately: $1, 0, 5$ satisfies $y^2 = xz$ and is not a GP.)
- **Closure:** adding a constant to an AP keeps it an AP; scaling a GP keeps it a GP.
  Adding a constant to a GP does **not**.
- **Infinite GP:** converges iff $\lvert r\rvert < 1$, to $\frac{a}{1-r}$. Never relevant to
  integer problems.
