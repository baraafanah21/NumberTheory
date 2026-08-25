# Arithmetic and Geometric Progressions

**Position in the hierarchy:** foundational tool layer. No prerequisites beyond integer
arithmetic.

**Concepts that depend on it:** sum of divisors $\sigma(n)$, Mersenne numbers, repunits,
counting multiples in a range, complexity analysis of sieves, binary exponentiation,
modular geometric sums.

---

## 1. Definition

A **sequence** is an ordered list of numbers $a_1, a_2, a_3, \dots$ A **progression** is a
sequence built by repeating one fixed operation.

**Arithmetic progression (AP).** A sequence in which the difference between consecutive
terms is constant:

$$a_{n+1} - a_n = d \qquad \text{for every } n$$

$d$ is the **common difference**. Example: $3, 7, 11, 15, 19$ with $d = 4$.

**Geometric progression (GP).** A sequence in which the *ratio* between consecutive terms
is constant:

$$\frac{a_{n+1}}{a_n} = r \qquad \text{for every } n \quad (\text{all terms nonzero})$$

$r$ is the **common ratio**. Example: $3, 6, 12, 24, 48$ with $r = 2$.

The two definitions are the same statement with $+$ replaced by $\times$. This is not a
coincidence and it is worth taking seriously — see GP-4 in section 4.

> **Note on the "all terms nonzero" clause.** A GP is usually defined with $a_1 \neq 0$
> and $r \neq 0$. If a term were $0$, every later term would be $0$ and the ratio would be
> undefined. Some texts allow $0, 0, 0, \dots$; this repository does not, because the
> ratio is what carries all the information.

---

## 2. Intuition

**An AP is repeated addition. A GP is repeated multiplication.**

Picture walking along a number line.

- In an AP you take steps of the *same size* every time. Your position after $n$ steps is
  a straight line in $n$. Growth is **linear**.
- In a GP you *scale* your position by the same factor every time. Your position after
  $n$ steps is exponential in $n$. Growth is **exponential**.

That single difference explains everything else:

| | AP | GP |
|---|---|---|
| step operation | add $d$ | multiply by $r$ |
| general term | $a + (n-1)d$ — linear in $n$ | $a r^{\,n-1}$ — exponential in $n$ |
| sum of $n$ terms | quadratic in $n$ | again exponential in $n$ |
| middle term is | the **average** of neighbours | the **geometric mean** of neighbours |
| grows like | $1, 2, 3, 4, 5$ | $1, 2, 4, 8, 16$ |

**Why the AP sum has a shortcut.** Write $1 + 2 + \cdots + 100$ forwards and backwards,
one under the other:

$$
\begin{aligned}
S &= \phantom{100 \;+\;\;} 1 + \phantom{9}2 + \cdots + \phantom{9}99 + 100\\
S &= 100 + \phantom{0}99 + \cdots + \phantom{99}2 + \phantom{10}1\\[4pt]
2S &= \underbrace{101 + 101 + \cdots + 101 + 101}_{100 \text{ equal terms}} = 100 \cdot 101
\end{aligned}
$$

Every vertical pair sums to the same value, because as you move right the top row gains
$d$ and the bottom row loses $d$. You counted the sum twice, so $S = \frac{100 \cdot 101}{2} = 5050$.
The pairing works for *any* AP — that is the whole content of the formula.

**Why the GP sum has a shortcut.** Let $S = 1 + r + r^2 + \cdots + r^{\,n-1}$. Multiplying
by $r$ shifts the whole thing one place:

$$
\begin{aligned}
rS &= \phantom{1 + {}} r + r^2 + \cdots + r^{\,n-1} + r^{\,n}\\
S &= 1 + r + r^2 + \cdots + r^{\,n-1}\\[4pt]
rS - S &= r^{\,n} - 1
\end{aligned}
$$

Everything in the middle cancels — the sum **telescopes** — leaving
$S(r - 1) = r^{\,n} - 1$. This cancellation is the single most useful identity in this
file: it is what makes $2^n - 1$ factorable, and it is why $\sigma(p^k)$ has a closed form.

---

## 3. Mathematical Formulation

Throughout, $a$ (or $a_1$) is the first term and $n \geq 1$ is the number of terms.

### Arithmetic progression

$$
\begin{aligned}
\text{General term} \qquad & a_n = a + (n-1)d\\[4pt]
\text{Number of terms} \qquad & n = \frac{a_n - a}{d} + 1 \qquad (d \neq 0)\\[4pt]
\text{Sum of } n \text{ terms} \qquad & S_n = \frac{n}{2}\bigl(2a + (n-1)d\bigr)
                                            = \frac{n}{2}(a_1 + a_n)\\[4pt]
\text{Special case} \qquad & 1 + 2 + \cdots + n = \frac{n(n+1)}{2}
\end{aligned}
$$

The second form, $S_n = n \cdot \frac{\text{first} + \text{last}}{2}$, is the one to
remember: **number of terms times the average term**. Both other formulas follow from it.

### Geometric progression

$$
\begin{aligned}
\text{General term} \qquad & a_n = a\, r^{\,n-1}\\[4pt]
\text{Sum of } n \text{ terms} \qquad & S_n = a \cdot \frac{r^{\,n} - 1}{r - 1}
   \quad \text{if } r \neq 1, \qquad S_n = na \quad \text{if } r = 1\\[4pt]
\text{Infinite sum} \qquad & S_\infty = \frac{a}{1 - r} \quad \text{if } |r| < 1
   \ \ (\text{converges only then})\\[4pt]
\text{Key identity} \qquad & r^{\,n} - 1 = (r - 1)\bigl(r^{\,n-1} + r^{\,n-2} + \cdots + r + 1\bigr)\\[4pt]
\text{General identity} \qquad & a^n - b^n = (a - b)\sum_{i=0}^{n-1} a^i b^{\,n-1-i}
\end{aligned}
$$

The $r \neq 1$ split is a real case distinction, not a technicality — dividing by $r - 1$
when $r = 1$ is division by zero. Handle it in code, always.

### Means

For three consecutive terms $x, y, z$:

$$
\begin{aligned}
\text{AP} \iff & \ y = \frac{x + z}{2} && y \text{ is the arithmetic mean}\\[4pt]
\text{GP} \iff & \ y^2 = xz && y \text{ is the geometric mean, } |y| = \sqrt{xz}
\end{aligned}
$$

---

## 4. Properties

Proofs for all of these are in [proofs.md](proofs.md); here is what each one *says* and
why it is believable.

### AP properties

**AP-1 (Middle term / three-term test).** $x, y, z$ are in AP $\iff 2y = x + z$.
*Why:* both sides say "the step up equals the step down", i.e. $y - x = z - y$. This is
the cheapest way to test three numbers, and it avoids division entirely.

**AP-2 (Equidistant pairs).** $a_k + a_{n+1-k}$ is the same for every $k$.
*Why:* moving one index right adds $d$, moving the partner one index left subtracts $d$.
This is exactly the Gauss pairing from section 2.

**AP-3 (Closure).** If $(a_n)$ is an AP with difference $d$, then $(a_n + c)$ is an AP
with difference $d$, and $(c\,a_n)$ is an AP with difference $cd$.
*Why:* shifting or scaling a linear function keeps it linear.

**AP-4 (Evenly spaced subsequence).** Taking every $k$-th term of an AP gives an AP with
difference $kd$. *Why:* you are just taking $k$ steps at a time.

**AP-5 (Sum is quadratic, with no constant term).**

$$S_n = \frac{d}{2}n^2 + \Bigl(a - \frac{d}{2}\Bigr)n$$

Conversely, if $S_n = An^2 + Bn$ for all $n \geq 1$, the underlying sequence is an AP with
$d = 2A$ and $a_1 = A + B$.
*Why it matters:* if a problem hands you a quadratic partial-sum formula, you instantly
know you are looking at an AP, and you can read off $a$ and $d$. The **absent constant
term** is the part people forget: $S_n = n^2 + n + 5$ is not the partial-sum sequence of
any AP, because $S$ of zero terms must be $0$.

**AP-6 (Multiples form an AP).** For a fixed $d > 0$, the multiples of $d$ inside
$[1, n]$ are $d, 2d, \dots, kd$ where $k = \lfloor n/d \rfloor$. There are
$\lfloor n/d \rfloor$ of them and their sum is

$$d \cdot \frac{k(k+1)}{2}$$

*Why it matters:* this is the single most-used consequence of APs in number theory. Every
"sum all numbers divisible by …" problem is this line.

**AP-7 (Residues of an AP).** Modulo $m$, an AP is periodic:
$a_n \equiv a + (n-1)d \pmod m$. The terms hit **every** residue class mod $m$ if and only
if $\gcd(d, m) = 1$; otherwise they only ever land in residues congruent to $a$ modulo
$\gcd(d, m)$.
*Depends on:* modular arithmetic and Bézout's identity. Stated here because it is the
bridge from progressions to congruences — and the reason Dirichlet's theorem on primes in
an AP requires $\gcd(a, d) = 1$.

**AP-8 (Sum of $n$ consecutive integers).** The sum of $n$ consecutive integers is
divisible by $n$ **iff $n$ is odd**.
*Why:* the sum is $n \times (\text{average})$, and the average of an even count of
consecutive integers is a half-integer. A classic contest observation.

### GP properties

**GP-1 (Middle term / three-term test).** Nonzero $x, y, z$ are in GP $\iff y^2 = xz$.
*Why:* both sides say $y/x = z/y$. Prefer the cross-multiplied form in code — it uses no
division and no floating point.

**GP-2 (Equidistant pairs).** $a_k \cdot a_{n+1-k}$ is the same for every $k$. The
multiplicative twin of AP-2.

**GP-3 (Closure).** If $(a_n)$ is a GP with ratio $r$, then $(c\,a_n)$ is a GP with ratio
$r$, and $(a_n^t)$ is a GP with ratio $r^t$. Note the asymmetry with AP-3: *adding* a
constant to a GP destroys it.

**GP-4 (AP $\leftrightarrow$ GP bridge).** If $(a_n)$ is a GP of positive terms, then
$(\log a_n)$ is an AP, and conversely. *Why:* logarithms turn multiplication into
addition, which is exactly the difference between the two definitions. This is why
"exponential growth looks linear on a log scale".

**GP-5 (Divisibility identity).** For integers $a, b$ and $n \geq 1$:

$$(a - b) \mid (a^n - b^n)$$

*Why:* the factorization in section 3 exhibits the quotient explicitly, and that quotient
is an integer because it is a sum of products of integers. **This is the main
number-theoretic payoff of GPs.**

**GP-6 (Mersenne consequence).** If $d \mid n$ then $(2^d - 1) \mid (2^n - 1)$. Hence if
$n$ is composite, $2^n - 1$ is composite. *Why:* write $n = de$, then
$2^n - 1 = (2^d)^e - 1^e$ and apply GP-5 with $a = 2^d$, $b = 1$. So $2^n - 1$ can only be
prime when $n$ is prime — the necessary condition behind Mersenne primes. (The converse
fails: $2^{11} - 1 = 2047 = 23 \cdot 89$.)

**GP-7 (Sum of divisors of a prime power).** For prime $p$ and $k \geq 0$:

$$\sigma(p^k) = 1 + p + p^2 + \cdots + p^k = \frac{p^{\,k+1} - 1}{p - 1}$$

*Why:* the divisors of $p^k$ are exactly $p^0, p^1, \dots, p^k$ — a GP — so summing them
is a GP sum. *Depends on:* unique factorization, to know that list is complete.

**GP-8 (Convergence).** $a + ar + ar^2 + \cdots$ converges iff $|r| < 1$, to
$\dfrac{a}{1 - r}$. *Why:* $r^{\,n} \to 0$ exactly when $|r| < 1$. *Depends on:* limits.

**GP-9 (Doubling bound).** $1 + 2 + 4 + \cdots + 2^k = 2^{\,k+1} - 1 < 2 \cdot 2^k$.
*Why it matters:* a GP with $r = 2$ is dominated by its last term. This is the amortized
analysis of a doubling array (total copying work $< 2n$), and it is why a $k$-bit unsigned
integer maxes out at $2^k - 1$.

---

## 5. Examples

**AP, general term and sum.** $a = 5$, $d = 3$: $\ 5, 8, 11, 14, 17, \dots$

$$a_{10} = 5 + 9 \cdot 3 = 32, \qquad S_{10} = \tfrac{10}{2}(5 + 32) = 5 \cdot 37 = 185$$

**AP, number of terms.** How many terms in $7, 11, 15, \dots, 99$?

$$n = \frac{99 - 7}{4} + 1 = \frac{92}{4} + 1 = 23 + 1 = 24$$

Forgetting the $+1$ is the classic error.

**AP-6 in action.** Sum of all multiples of $7$ up to $100$: $k = \lfloor 100/7 \rfloor = 14$,

$$7 \cdot \frac{14 \cdot 15}{2} = 7 \cdot 105 = 735$$

**AP-8 in action.** $4 + 5 + 6 + 7 + 8 = 30$ and $5 \mid 30$ ($n = 5$, odd), whereas
$4 + 5 + 6 + 7 = 22$ and $4 \nmid 22$ ($n = 4$, even).

**GP, general term and sum.** $a = 3$, $r = 2$: $\ 3, 6, 12, 24, 48, \dots$

$$a_5 = 3 \cdot 2^4 = 48, \qquad S_5 = \frac{3(2^5 - 1)}{2 - 1} = 3 \cdot 31 = 93$$

**GP with $r = 1$.** $4, 4, 4, 4$: the formula would divide by $0$; the answer is
$S_4 = 4 \cdot 4 = 16$.

**GP-5 in action.** $n = 3$, $a = 5$, $b = 2$: $\ 5^3 - 2^3 = 125 - 8 = 117$, and
$5 - 2 = 3$ divides $117 = 3 \cdot 39$. The identity even names the quotient:
$5^2 + 5 \cdot 2 + 2^2 = 25 + 10 + 4 = 39$. ✓

**GP-6 in action.** $n = 6 = 2 \cdot 3$, so both $2^2 - 1 = 3$ and $2^3 - 1 = 7$ must
divide $2^6 - 1 = 63$. Indeed $63 = 3 \cdot 7 \cdot 3$. ✓

**GP-7 in action.** $\sigma(2^3) = 1 + 2 + 4 + 8 = 15 = \frac{2^4 - 1}{2 - 1}$ ✓ and
$\sigma(3^2) = 1 + 3 + 9 = 13 = \frac{3^3 - 1}{3 - 1} = \frac{26}{2}$ ✓

**Infinite GP.** $1 + \frac12 + \frac14 + \cdots = \frac{1}{1 - 1/2} = 2$. With $r = 2$
the series diverges, which is why $1 + 2 + 4 + \cdots$ has no finite value.

---

## 6. Algorithm

### 6.1 Closed-form evaluation — $O(1)$

Both general terms and both sums have closed forms, so the naive $O(n)$ loop is never
needed for exact small values. The only real work is **avoiding overflow and integer
division error**.

**The integer-division trap.** $S = \frac{n(2a + (n-1)d)}{2}$ must stay exact. In integer
arithmetic `n/2 * bracket` truncates when $n$ is odd. The fix rests on a small fact:

> Exactly one of $n$ and $\bigl(2a + (n-1)d\bigr)$ is even.

Indeed $2a$ is even, so the bracket has the parity of $(n-1)d$. If $n$ is odd then $n - 1$
is even and the bracket is even; if $n$ is even then $n$ itself is. So: **test the parity
and halve whichever factor is even, before multiplying.** This both keeps the result exact
and halves the magnitude, buying one bit of headroom against overflow.

### 6.2 Powers and geometric sums modulo $m$ — $O(\log n)$

$a\,r^{\,n-1} \bmod m$ needs **binary exponentiation**: square the base while halving the
exponent, multiplying the answer in whenever the current bit is set. $O(\log n)$
multiplications, $O(1)$ extra space.

The geometric *sum* mod $m$ is the subtle one. You **cannot** just compute
$(r^{\,n} - 1)\cdot(r-1)^{-1} \bmod m$ in general, because $r - 1$ need not be invertible
mod $m$ (it is invertible only when $\gcd(r-1, m) = 1$). The division-free method splits
the sum in half instead. Write $G(n) = 1 + r + \cdots + r^{\,n-1}$. Then:

$$
\begin{aligned}
G(0) &= 0\\[4pt]
G(2k) &= G(k)\bigl(1 + r^{\,k}\bigr) && \text{split into two blocks of } k \text{ terms}\\[4pt]
G(n) &= G(n-1) + r^{\,n-1} && n \text{ odd: peel one term}
\end{aligned}
$$

**Why the even case works:** the last $k$ terms are
$r^{\,k} + r^{\,k+1} + \cdots + r^{\,2k-1} = r^{\,k}\bigl(1 + r + \cdots + r^{\,k-1}\bigr) = r^{\,k}G(k)$.
So the whole sum is $G(k) + r^{\,k}G(k)$.

- **Time:** $O(\log^2 n)$ — $O(\log n)$ recursion depth, each level computing one power in
  $O(\log n)$. (An $O(\log n)$ variant exists by carrying $r^{\,k}$ alongside the sum; the
  version above is written for clarity.)
- **Space:** $O(\log n)$ for the recursion stack.

### 6.3 Detecting a progression in an array — $O(n)$ time, $O(1)$ space

Scan once and check the three-term condition on every window:

- AP: $a_{i+1} - a_i = a_1 - a_0$ for all $i$. Use subtraction, never division.
- GP: $a_{i+1}a_{i-1} = a_i^2$ for all interior $i$, plus all terms nonzero. Use the
  cross-multiplied form (GP-1) so there is no floating point and no rounding.

Sequences of length $\leq 2$ are trivially both.

### 6.4 Summing multiples in a range — $O(1)$

By AP-6, with $F(n) = d \cdot \frac{k(k+1)}{2}$ where $k = \lfloor n/d \rfloor$ being the
sum of multiples of $d$ in $[1, n]$, the sum of multiples of $d$ in $[L, R]$ is
$F(R) - F(L-1)$. Combine several $d$ with inclusion–exclusion (using $\mathrm{lcm}$ for
the overlaps — see the LCM concept).

**Complexity summary**

| Operation | Time | Space |
|---|---|---|
| AP term / AP sum (exact) | $O(1)$ | $O(1)$ |
| GP term (exact, small) | $O(\log n)$ | $O(1)$ |
| GP sum (exact, $r \neq 1$) | $O(\log n)$ | $O(1)$ |
| $r^{\,n} \bmod m$ | $O(\log n)$ | $O(1)$ |
| geometric sum mod $m$ | $O(\log^2 n)$ | $O(\log n)$ |
| detect AP / GP in array | $O(n)$ | $O(1)$ |
| sum of multiples in $[L, R]$ | $O(1)$ | $O(1)$ |

---

## 7. Problem-Solving

### How to recognize it

- **Explicit wording:** "each day he saves 5 more than the previous day" (AP), "the
  population doubles every hour" (GP), "each step costs twice the last" (GP).
- **A constant difference or ratio in sample data.** Always compute the first few
  differences; if they are constant you have an AP, if the *ratios* are constant, a GP. If
  the *second* differences are constant you have a quadratic — which by AP-5 is the
  partial-sum sequence of an AP.
- **$n$ is huge ($10^{18}$) but the sequence is regular.** That is a closed-form problem by
  construction: no loop will finish, so a formula must exist.
- **Sums of the form $\sum r^i$ appear inside a modular problem.** Reach for §6.2.
- **Anything of the shape $x^n - y^n$, $2^n - 1$, $\underbrace{11\cdots1}$.** That is GP-5
  wearing a costume.

### Common patterns

**Counting and summing multiples.** $\lfloor n/d \rfloor$ multiples, sum
$d \cdot \frac{k(k+1)}{2}$. Combine with inclusion–exclusion: multiples of 3 or 5 below
$n$ = (multiples of 3) + (multiples of 5) − (multiples of 15).

**Repunits.** $R_n = \underbrace{11\cdots1}_{n} = \dfrac{10^n - 1}{9}$, a GP sum. By GP-5,
$d \mid n$ implies $R_d \mid R_n$. So $R_n$ can only be prime if $n$ is prime.

**Mersenne / factoring $a^n \pm b^n$.** Before testing $2^n - 1$ for primality, check
whether $n$ is composite — GP-6 settles it instantly. Likewise $a^n - 1$ is always
divisible by $a - 1$, so $a^n - 1$ is prime only if $a = 2$.

**Prime-power divisor sums.** When you factor $n = p_1^{e_1}\cdots p_k^{e_k}$, each
factor's contribution to $\sigma(n)$ is a GP sum (GP-7). This is the computational core of
$\sigma$.

**Telescoping.** The GP sum proof is a telescoping argument. The same trick collapses many
sums: if you can write the summand as $f(i+1) - f(i)$, the sum is $f(n) - f(0)$.

**Complexity arguments.** $1 + 2 + \cdots + n = \Theta(n^2)$ (nested loops);
$n + \frac{n}{2} + \frac{n}{4} + \cdots < 2n$ (GP-9, doubling arrays and
divide-and-conquer merge costs). Recognizing which of the two you are summing is often the
whole complexity analysis.

**Binary representation.** $k$ set bits give $2^k - 1$; a value $< 2^k$ needs $k$ bits.

### Typical tricks

- **Pair from both ends** (AP-2) instead of manipulating the formula.
- **Cross-multiply instead of dividing** (GP-1) to stay in exact integers.
- **Reverse the formula.** Given $S_n$ and asked for $n$, solve the quadratic
  $\frac{d}{2}n^2 + \bigl(a - \frac{d}{2}\bigr)n - S = 0$ and take the positive root, then
  verify with integers — never trust the floating-point root alone.
- **Halve the even factor first** (§6.1) to keep integer sums exact and delay overflow.
- **Split a sum at a parity or a residue class.** The odd terms of an AP form an AP with
  difference $2d$ (AP-4).

### Important observations

- An AP is determined by any two terms; a GP by any two terms (up to the sign of $r$ when
  the index gap is even).
- $d$ may be negative or zero; $r$ may be negative (alternating signs), a fraction, or
  $1$. Test all of these.
- Sums grow fast: $n = 10^9$ with $d = 1$ already gives $S \approx 5 \cdot 10^{17}$, near
  the `long long` ceiling. Estimate the magnitude *before* choosing a type.

---

## 8. Common Mistakes

**Conceptual**

1. **Dropping the $+1$ in the term count.** $\frac{\text{last} - \text{first}}{d}$ is the
   number of *steps*; the number of *terms* is one more. $7, 11, \dots, 99$ has $24$
   terms, not $23$.
2. **Using $a + nd$ for the $n$-th term.** It is $a + (n-1)d$; the first term takes zero
   steps. Same off-by-one, different disguise.
3. **Forgetting the $r = 1$ case** in the GP sum. It is division by zero, not a rare
   corner case — constant sequences are common inputs.
4. **Assuming an infinite GP has a finite sum.** It converges only for $|r| < 1$.
5. **Confusing difference with ratio.** $2, 4, 8$ is a GP with $r = 2$, not an AP with
   $d = 2$.
6. **Thinking adding a constant to a GP keeps it a GP.** It does not (contrast AP-3 with
   GP-3): $1, 2, 4$ is a GP but $2, 3, 5$ is not.
7. **Reading $S_n = An^2 + Bn + C$ with $C \neq 0$ as an AP's partial sums.** AP-5
   requires $C = 0$.
8. **Assuming $2^p - 1$ is prime whenever $p$ is prime.** GP-6 gives only the *necessary*
   direction: $2^{11} - 1 = 2047 = 23 \cdot 89$.

**Implementation**

9. **Integer-division truncation:** writing `n / 2 * (2*a + (n-1)*d)` when $n$ is odd.
   Halve the even factor — see §6.1.
10. **Overflow before the division.** `n * (n+1)` overflows `long long` around
    $n \approx 3 \cdot 10^9$ even when $\frac{n(n+1)}{2}$ would fit. Divide first, or use
    a wider type.
11. **Dividing by $r - 1$ under a modulus.** Only valid when $\gcd(r - 1, m) = 1$. Use the
    division-free recursion of §6.2 instead. Composite moduli like $10^9 + 8$ break the
    naive approach.
12. **Detecting a GP with floating-point ratios.** `a[i+1]/a[i]` in `double` loses
    precision and misjudges large integers. Cross-multiply (GP-1) — and mind that the
    products themselves can overflow.
13. **Naive `pow()` from `<cmath>` for integer powers.** It returns a `double`, silently
    rounds, and is wrong past $2^{53}$. Write integer binary exponentiation.
14. **Negative $d$ or $r$ breaking loop bounds.** A "count the terms until we pass $L$"
    loop assumes an increasing sequence. Handle descending progressions explicitly.
15. **Taking a modulus of a negative intermediate.** In C++ `%` follows the sign of the
    dividend, so `-3 % 5 == -3`. Normalize with `((x % m) + m) % m`.

---

## Files

- [README.md](README.md) — this file: the concept.
- [proofs.md](proofs.md) — rigorous proofs of every property above.
- [implementation.cpp](implementation.cpp) — C++17 implementation with complexities.
