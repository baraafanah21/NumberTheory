# Proofs — Arithmetic and Geometric Progressions

Every theorem here corresponds to a property stated in [README.md](README.md).

**Notation.** An AP has first term $a$, common difference $d$, and terms
$a_n = a + (n-1)d$. A GP has first term $a \neq 0$, common ratio $r \neq 0$, and terms
$a_n = a\,r^{\,n-1}$. $S_n$ denotes the sum of the first $n$ terms. Indices start at $1$.

**Dependencies used in this file.** Only elementary integer arithmetic and induction,
except where explicitly flagged: Theorem 14 uses unique factorization, Theorem 17 uses
logarithms, Theorem 19 uses limits, and Theorem 21 uses Bézout's identity. Each such
dependency is restated at the theorem itself.

---

## Part I — Arithmetic Progressions

### Theorem 1 (General term)

> If $a_1 = a$ and $a_{n+1} = a_n + d$ for all $n \geq 1$, then $a_n = a + (n-1)d$.

**What it says.** The recursive description ("keep adding $d$") and the closed form
("start at $a$, take $n-1$ steps") describe the same sequence. This is what lets us
evaluate the $n$-th term in $O(1)$ instead of looping.

**Proof (induction on $n$).**

*Base case* $n = 1$: the formula gives $a + (1-1)d = a + 0 = a = a_1$. ✓

*Inductive step.* Assume $a_k = a + (k-1)d$ for some $k \geq 1$. Then by the recurrence,

$$a_{k+1} = a_k + d = \bigl[a + (k-1)d\bigr] + d = a + (k-1)d + d = a + kd = a + \bigl((k+1) - 1\bigr)d$$

which is the formula at $n = k+1$. ✓

By induction the formula holds for all $n \geq 1$. $\blacksquare$

**Key step.** The whole step is $(k-1)d + d = kd$, i.e. one more addition of $d$ is one
more step. The $n-1$ (rather than $n$) is forced by the base case: the first term has
taken *no* steps.

---

### Theorem 2 (Sum of an AP)

> $$S_n = \frac{n}{2}\bigl(2a + (n-1)d\bigr) = \frac{n}{2}(a_1 + a_n)$$

**What it says.** The sum equals the number of terms times the average of the first and
last term. The two forms are equal because
$a_1 + a_n = a + \bigl(a + (n-1)d\bigr) = 2a + (n-1)d$.

**Proof A (reversal — the Gauss pairing, made rigorous).**

Write the sum forwards and backwards:

$$
\begin{aligned}
S_n &= a_1 + a_2 + \cdots + a_{n-1} + a_n\\
S_n &= a_n + a_{n-1} + \cdots + a_2 + a_1
\end{aligned}
$$

Add the two lines term by term. The $k$-th column is $a_k + a_{n+1-k}$. By Theorem 1,

$$a_k + a_{n+1-k} = \bigl[a + (k-1)d\bigr] + \bigl[a + (n-k)d\bigr] = 2a + \bigl(k - 1 + n - k\bigr)d = 2a + (n-1)d$$

The $k$ cancels, so **every column has the same value** $2a + (n-1)d$ — this is exactly
Theorem 3 below. There are $n$ columns, so

$$2S_n = n\bigl(2a + (n-1)d\bigr) \quad \Longrightarrow \quad S_n = \frac{n}{2}\bigl(2a + (n-1)d\bigr) \qquad \blacksquare$$

**Proof B (induction),** for readers who want no reliance on the "$\cdots$" notation.

*Base* $n = 1$: $\ \frac12(2a + 0) = a = S_1$. ✓

*Step.* Assume $S_k = \frac{k}{2}\bigl(2a + (k-1)d\bigr)$. Then

$$
\begin{aligned}
S_{k+1} &= S_k + a_{k+1}\\
        &= \frac{k}{2}\bigl(2a + (k-1)d\bigr) + a + kd\\
        &= \frac{2ak + k(k-1)d + 2a + 2kd}{2}\\
        &= \frac{2a(k+1) + kd\bigl((k-1) + 2\bigr)}{2}\\
        &= \frac{2a(k+1) + k(k+1)d}{2}\\
        &= \frac{k+1}{2}\bigl(2a + kd\bigr)
\end{aligned}
$$

which is the formula at $n = k+1$. ✓ $\blacksquare$

**Key step.** In Proof A, that $k$ disappears from $a_k + a_{n+1-k}$. In Proof B, the
factorization $k(k-1)d + 2kd = k(k+1)d$.

**Corollary 2.1.** Taking $a = 1$, $d = 1$:

$$1 + 2 + \cdots + n = \frac{n(n+1)}{2}$$

**Corollary 2.2.** $S_n$ is an integer whenever $a, d, n$ are integers — even though the
formula shows a division by $2$.

*Proof.* $2a$ is even, so $2a + (n-1)d$ has the same parity as $(n-1)d$. If $n$ is odd,
$n-1$ is even, so the bracket is even. If $n$ is even, $n$ is even. In either case at
least one factor of the product $n\bigl(2a + (n-1)d\bigr)$ is even, so the product is
divisible by $2$. $\blacksquare$

*This corollary is the justification for the "halve the even factor" technique in
[README.md §6.1](README.md#61-closed-form-evaluation--o1).*

---

### Theorem 3 (Equidistant pairs — AP-2)

> For an AP of $n$ terms, $a_k + a_{n+1-k}$ is independent of $k$.

Proved inside Proof A of Theorem 2: the value is $2a + (n-1)d = a_1 + a_n$ for every $k$.
$\blacksquare$

**Intuition.** As the left index moves one step right the term gains $d$; the right index
moves one step left and loses $d$. The two changes cancel exactly.

---

### Theorem 4 (Three-term characterization — AP-1)

> Numbers $x, y, z$ (in that order) form an AP $\iff 2y = x + z$.

**Proof.** By definition $x, y, z$ is an AP iff the two consecutive differences agree:

$$y - x = z - y$$

Add $y + x$ to both sides: $2y = x + z$. Every step is reversible (adding a quantity to
both sides of an equation is invertible), so the two statements are equivalent.
$\blacksquare$

**Why the additive form matters.** $2y = x + z$ uses only addition, so it is exact in
integer arithmetic and never divides by zero — unlike testing $y - x = z - y$ through a
computed ratio.

---

### Theorem 5 (Closure under shift and scale — AP-3)

> If $(a_n)$ is an AP with difference $d$, then $(a_n + c)$ is an AP with difference $d$,
> and $(c\,a_n)$ is an AP with difference $cd$.

**Proof.** Direct computation of consecutive differences.

$$
\begin{aligned}
(a_{n+1} + c) - (a_n + c) &= a_{n+1} - a_n = d && \text{constant, so an AP}\\
c\,a_{n+1} - c\,a_n &= c(a_{n+1} - a_n) = cd && \text{constant, so an AP}
\end{aligned}
$$

$\blacksquare$

**Remark.** Note that $c$ may be $0$ in the second statement, giving the constant sequence
$0, 0, \dots$, which is an AP with $d = 0$. This is why the degenerate $d = 0$ case must
be allowed by the definition.

---

### Theorem 6 (Evenly spaced subsequence — AP-4)

> If $(a_n)$ is an AP with difference $d$, then for fixed $k \geq 1$ and any starting index
> $i$, the sequence $b_j = a_{\,i + (j-1)k}$ is an AP with difference $kd$.

**Proof.** Using Theorem 1,

$$
\begin{aligned}
b_{j+1} - b_j &= a_{\,i + jk} - a_{\,i + (j-1)k}\\
&= \bigl[a + (i + jk - 1)d\bigr] - \bigl[a + (i + jk - k - 1)d\bigr]\\
&= kd
\end{aligned}
$$

which is constant. $\blacksquare$

**Consequence used in problems.** The odd-indexed terms of an AP form an AP with
difference $2d$; so do the even-indexed ones.

---

### Theorem 7 (Partial sums are quadratic with zero constant term — AP-5)

> **(a)** If $(a_n)$ is an AP, then $S_n = An^2 + Bn$ with $A = \frac{d}{2}$ and
> $B = a - \frac{d}{2}$.
> **(b)** Conversely, if a sequence's partial sums satisfy $S_n = An^2 + Bn$ for all
> $n \geq 1$, then the sequence is an AP with $d = 2A$ and $a_1 = A + B$.

**What it says.** "AP" and "quadratic partial sums with no constant term" are the same
condition. Part (b) is the useful direction in problems: a quadratic $S_n$ lets you read
off $a$ and $d$ immediately.

**Proof of (a).** Expand Theorem 2:

$$S_n = \frac{n}{2}\bigl(2a + (n-1)d\bigr) = an + \frac{d\,n(n-1)}{2} = \frac{d}{2}n^2 + \Bigl(a - \frac{d}{2}\Bigr)n$$

so $A = \frac{d}{2}$, $B = a - \frac{d}{2}$, and the constant term is $0$. $\blacksquare$

**Proof of (b).** Recover the terms by differencing the partial sums. For $n \geq 2$,

$$
\begin{aligned}
a_n = S_n - S_{n-1} &= \bigl[An^2 + Bn\bigr] - \bigl[A(n-1)^2 + B(n-1)\bigr]\\
&= A\bigl(n^2 - (n-1)^2\bigr) + B\bigl(n - (n-1)\bigr)\\
&= A(2n - 1) + B\\
&= 2An + (B - A)
\end{aligned}
$$

and for $n = 1$, $a_1 = S_1 = A + B$, which agrees with the formula
$2A \cdot 1 + (B - A) = A + B$. So $a_n = 2An + (B - A)$ for **all** $n \geq 1$. Then

$$a_{n+1} - a_n = 2A(n+1) - 2An = 2A$$

is constant, so the sequence is an AP with $d = 2A$, and $a_1 = A + B$. $\blacksquare$

**Why the constant term must vanish.** Suppose $S_n = An^2 + Bn + C$. Differencing still
gives $a_n = 2An + (B - A)$ for $n \geq 2$, but now $a_1 = S_1 = A + B + C$, whereas the
formula predicts $A + B$. These agree only if $C = 0$. So a nonzero constant term makes
the first term inconsistent with the rest, and the sequence fails to be an AP (it is an AP
from the second term onward). $\blacksquare$

---

### Theorem 8 (Multiples in a range — AP-6)

> Let $d \geq 1$ and $n \geq 0$ be integers. The multiples of $d$ in $[1, n]$ are exactly
> $d, 2d, \dots, kd$ where $k = \lfloor n/d \rfloor$. There are $k$ of them and their sum
> is $d \cdot \frac{k(k+1)}{2}$.

**Proof.** A positive multiple of $d$ has the form $jd$ with $j \geq 1$. Then

$$jd \leq n \iff j \leq \frac{n}{d} \iff j \leq \Bigl\lfloor \frac{n}{d} \Bigr\rfloor$$

The last equivalence holds because $j$ is an integer: an integer is $\leq n/d$ iff it is
$\leq$ the greatest integer not exceeding $n/d$. So the valid $j$ are exactly
$1, 2, \dots, k$ with $k = \lfloor n/d \rfloor$, giving $k$ multiples.

Their sum is

$$\sum_{j=1}^{k} jd = d\sum_{j=1}^{k} j = d \cdot \frac{k(k+1)}{2}$$

by Corollary 2.1. $\blacksquare$

**Corollary 8.1 (range version).** With $F(n) = d \cdot \frac{k(k+1)}{2}$,
$k = \lfloor n/d \rfloor$, the sum of multiples of $d$ in $[L, R]$ is $F(R) - F(L-1)$,
since the multiples in $[1, L-1]$ are precisely those counted by $F(R)$ that fall below
$L$.

---

### Theorem 9 (Sum of $n$ consecutive integers — AP-8)

> The sum of $n$ consecutive integers is divisible by $n$ **iff $n$ is odd**.

**Proof.** Let the integers be $a, a+1, \dots, a+n-1$ — an AP with $d = 1$. By Theorem 2,

$$S = \frac{n}{2}\bigl(2a + n - 1\bigr)$$

*Case $n$ odd.* Then $n - 1$ is even, so $2a + n - 1$ is even; write $2a + n - 1 = 2t$
with $t$ an integer. Then $S = nt$, so $n \mid S$. ✓

*Case $n$ even.* Then $n - 1$ is odd, so $2a + n - 1$ is odd; call it $u$. Then
$S = \frac{n}{2}u$ with $\frac{n}{2}$ an integer. Suppose for contradiction that
$n \mid S$, say $S = nm$. Then $\frac{n}{2}u = nm$, and dividing by $\frac{n}{2} \neq 0$
gives $u = 2m$ — so $u$ is even, contradicting that $u$ is odd. Hence $n \nmid S$.
$\blacksquare$

**Intuition.** The sum is $n \times (\text{average})$. For an odd count of consecutive
integers the average is the middle integer; for an even count it is a half-integer, and
that stray half is exactly what blocks divisibility.

---

## Part II — Geometric Progressions

### Theorem 10 (General term)

> If $a_1 = a$ and $a_{n+1} = r\,a_n$ for all $n \geq 1$, then $a_n = a\,r^{\,n-1}$.

**Proof (induction on $n$).**

*Base* $n = 1$: $\ a\,r^0 = a \cdot 1 = a = a_1$. ✓

*Step.* Assume $a_k = a\,r^{\,k-1}$. Then

$$a_{k+1} = r\,a_k = r \cdot a\,r^{\,k-1} = a\,r^{\,k} = a\,r^{\,(k+1)-1}$$

which is the formula at $n = k+1$. ✓

$\blacksquare$

**Key step.** $r \cdot r^{\,k-1} = r^{\,k}$. This is Theorem 1 with addition replaced by
multiplication throughout — the same induction, in a different operation.

---

### Theorem 11 (Sum of a GP)

> For $r \neq 1$: $\ S_n = a \cdot \dfrac{r^{\,n} - 1}{r - 1}$. For $r = 1$: $\ S_n = na$.

**Proof (telescoping).** Let $S_n = a + ar + ar^2 + \cdots + ar^{\,n-1}$. Multiply by $r$:

$$
\begin{aligned}
rS_n &= \phantom{a + {}} ar + ar^2 + ar^3 + \cdots + ar^{\,n-1} + ar^{\,n}\\
S_n &= a + ar + ar^2 + \cdots + ar^{\,n-1}
\end{aligned}
$$

Subtract the second line from the first. Every term $ar^i$ with $1 \leq i \leq n-1$
appears in both lines and cancels; only $ar^{\,n}$ (present only in $rS_n$) and $a$
(present only in $S_n$) survive:

$$
\begin{aligned}
rS_n - S_n &= ar^{\,n} - a\\
S_n(r - 1) &= a\bigl(r^{\,n} - 1\bigr)
\end{aligned}
$$

If $r \neq 1$ then $r - 1 \neq 0$ and we may divide:
$S_n = a \cdot \frac{r^{\,n} - 1}{r - 1}$. $\blacksquare$

**The $r = 1$ case.** The line $S_n(r - 1) = a(r^{\,n} - 1)$ remains true but reads
$0 = 0$, carrying no information — division is illegal. Instead, $r = 1$ makes every term
equal to $a$, so directly $S_n = na$. $\blacksquare$

**Key step.** The cancellation. Writing the two lines with aligned columns makes it
visible: multiplying by $r$ shifts the sequence one position, so the two sums overlap in
all but their end terms. This *telescoping* is the reusable idea — not the formula.

**Remark (why this is the most important identity here).** Setting $a = 1$ and clearing
the denominator gives

$$r^{\,n} - 1 = (r - 1)\bigl(r^{\,n-1} + r^{\,n-2} + \cdots + r + 1\bigr)$$

a **factorization**, valid over the integers. Theorems 12, 13 and 14 are all consequences.

---

### Theorem 12 (Divisibility identity — GP-5)

> For all integers $a, b$ and every integer $n \geq 1$:
> $$a^n - b^n = (a - b)\sum_{i=0}^{n-1} a^i b^{\,n-1-i}$$
> and therefore $(a - b) \mid (a^n - b^n)$.

**What it says.** The difference of two $n$-th powers always has $a - b$ as a factor, and
the identity names the cofactor explicitly.

**Proof (direct, by expanding and telescoping).** Let
$T = \sum_{i=0}^{n-1} a^i b^{\,n-1-i}$. Multiply out:

$$(a - b)T = \sum_{i=0}^{n-1} a^{\,i+1} b^{\,n-1-i} \; - \; \sum_{i=0}^{n-1} a^i b^{\,n-i}$$

Reindex the first sum with $j = i + 1$ (so $j$ runs from $1$ to $n$, and
$n - 1 - i = n - j$):

$$(a - b)T = \sum_{j=1}^{n} a^j b^{\,n-j} \; - \; \sum_{i=0}^{n-1} a^i b^{\,n-i}$$

The two sums have the *same* summand $a^k b^{\,n-k}$; the first runs over
$k = 1, \dots, n$, the second over $k = 0, \dots, n-1$. Every $k$ in $1, \dots, n-1$
occurs in both and cancels. What remains is the $k = n$ term of the first sum minus the
$k = 0$ term of the second:

$$(a - b)T = a^n b^0 - a^0 b^n = a^n - b^n$$

Since $a, b$ are integers, $T$ is a sum of products of integers, hence an integer.
Therefore $a^n - b^n = (a-b)T$ with $T \in \mathbb{Z}$, which is precisely the statement
$(a - b) \mid (a^n - b^n)$. $\blacksquare$

**Key step.** The reindexing that makes the two sums differ only at their endpoints — the
same telescoping as Theorem 11, now with two variables.

**Sanity check.** $n = 3$, $a = 5$, $b = 2$: $\ T = 5^2 + 5 \cdot 2 + 2^2 = 39$, and
$(5-2) \cdot 39 = 117 = 125 - 8$. ✓

---

### Theorem 13 (Mersenne consequence — GP-6)

> **(a)** If $d \mid n$ then $(2^d - 1) \mid (2^n - 1)$.
> **(b)** If $n > 1$ is composite, then $2^n - 1$ is composite.

**Proof of (a).** Write $n = de$ with $e$ a positive integer. Then

$$2^n - 1 = \bigl(2^d\bigr)^e - 1^e$$

Apply Theorem 12 with $a = 2^d$, $b = 1$, exponent $e$:
$(2^d - 1) \mid \bigl((2^d)^e - 1^e\bigr) = 2^n - 1$. $\blacksquare$

**Proof of (b).** Let $n$ be composite, so it has a divisor $d$ with $1 < d < n$. By part
(a), $2^d - 1$ divides $2^n - 1$. To conclude compositeness we must check the factor is
*proper* — that is, neither $1$ nor $2^n - 1$:

- Since $d > 1$, we have $2^d - 1 \geq 2^2 - 1 = 3 > 1$.
- Since $d < n$, and $x \mapsto 2^x - 1$ is strictly increasing, $2^d - 1 < 2^n - 1$.

So $2^n - 1$ has a divisor strictly between $1$ and itself, hence is composite.
$\blacksquare$

**What this does and does not give.** It gives a *necessary* condition: $2^n - 1$ prime
$\Rightarrow n$ prime. The converse is false — $n = 11$ is prime but
$2^{11} - 1 = 2047 = 23 \cdot 89$.

**Remark (repunits).** The identical argument with base $10$ shows $d \mid n$ implies
$R_d \mid R_n$, where $R_n = \frac{10^n - 1}{9}$ is the repunit of $n$ ones. So a repunit
can only be prime when its length is prime.

---

### Theorem 14 (Sum of divisors of a prime power — GP-7)

> For a prime $p$ and integer $k \geq 0$:
> $$\sigma(p^k) = 1 + p + p^2 + \cdots + p^k = \frac{p^{\,k+1} - 1}{p - 1}$$

**Depends on:** unique factorization (the fundamental theorem of arithmetic).

**Proof.** Two steps: identify the divisors, then sum them.

*Step 1 — the divisors of $p^k$ are exactly $p^0, p^1, \dots, p^k$.*
Each $p^i$ with $0 \leq i \leq k$ divides $p^k$, since $p^k = p^i \cdot p^{\,k-i}$ and
$k - i \geq 0$. Conversely, let $m \mid p^k$ with $m > 0$. By unique factorization, the
prime factorization of $m$ uses only primes appearing in $p^k$, so $m = p^i$ for some
$i \geq 0$; and $m \leq p^k$ forces $i \leq k$. So the list is complete and has no repeats
(the $p^i$ are distinct because $p \geq 2$).

*Step 2 — sum them.* The list $p^0, p^1, \dots, p^k$ is a GP with $a = 1$, ratio $r = p$,
and $k+1$ terms. Since $p \geq 2 \neq 1$, Theorem 11 applies:

$$\sigma(p^k) = 1 \cdot \frac{p^{\,k+1} - 1}{p - 1}$$

$\blacksquare$

**Check.** $\sigma(2^3) = \frac{2^4-1}{2-1} = 15 = 1+2+4+8$ ✓ and
$\sigma(3^2) = \frac{3^3-1}{2} = 13 = 1+3+9$ ✓

**Why this matters later.** $\sigma$ is multiplicative, so $\sigma(n)$ for
$n = p_1^{e_1}\cdots p_k^{e_k}$ is the product of these GP sums. That result belongs to the
multiplicative-functions concept; the GP sum is the piece it is built from.

---

### Theorem 15 (Three-term characterization — GP-1)

> Nonzero numbers $x, y, z$ (in that order) form a GP $\iff y^2 = xz$.

**Proof.** By definition $x, y, z$ is a GP iff the consecutive ratios agree:

$$\frac{y}{x} = \frac{z}{y}$$

Since $x \neq 0$ and $y \neq 0$, multiplying both sides by $xy$ is a reversible operation
and gives $y^2 = xz$. Conversely, from $y^2 = xz$ divide both sides by $xy \neq 0$ to
recover $\frac{y}{x} = \frac{z}{y}$. $\blacksquare$

**Caution — why "nonzero" is required.** The condition $y^2 = xz$ alone is *not*
sufficient if zeros are allowed: $x = 1$, $y = 0$, $z = 5$ satisfies $0 = 0$ but is no GP.
Any implementation must check for zero terms separately. This is exactly the caveat
handled in `isGeometric` in [implementation.cpp](implementation.cpp).

---

### Theorem 16 (Equidistant pairs — GP-2)

> For a GP of $n$ terms, $a_k \cdot a_{n+1-k}$ is independent of $k$.

**Proof.** By Theorem 10,

$$a_k \cdot a_{n+1-k} = \bigl[a\,r^{\,k-1}\bigr]\bigl[a\,r^{\,n-k}\bigr] = a^2 r^{\,(k-1)+(n-k)} = a^2 r^{\,n-1}$$

The $k$ cancels in the exponent, so the product is $a^2 r^{\,n-1} = a_1 a_n$ for every $k$.
$\blacksquare$

**Remark.** Compare with Theorem 3: identical structure, with sums replaced by products
and $d$ by $r$. Theorem 17 explains why this parallel is exact rather than coincidental.

---

### Theorem 17 (AP $\leftrightarrow$ GP bridge — GP-4)

> Let $(a_n)$ be a sequence of **positive** reals. Then $(a_n)$ is a GP with ratio $r$ if
> and only if $(\log a_n)$ is an AP with difference $\log r$.

**Proof.** Positivity makes every logarithm defined.

$(\Rightarrow)$ If $a_{n+1} = r\,a_n$, take logs of both sides:
$\log a_{n+1} = \log r + \log a_n$, so $\log a_{n+1} - \log a_n = \log r$, a constant.
Hence $(\log a_n)$ is an AP with difference $\log r$.

$(\Leftarrow)$ If $\log a_{n+1} - \log a_n = c$ for all $n$, exponentiate:
$\frac{a_{n+1}}{a_n} = e^{c}$, a constant. Hence $(a_n)$ is a GP with $r = e^{c}$.
$\blacksquare$

**Why it matters.** Every AP theorem transfers to a GP theorem and vice versa: Theorem 3
becomes Theorem 16, "sum" becomes "product", "arithmetic mean" becomes "geometric mean".
The logarithm is the dictionary between the two.

---

### Theorem 18 (AM–GM for two terms)

> For reals $x, y \geq 0$: $\ \dfrac{x + y}{2} \geq \sqrt{xy}$, with equality iff $x = y$.

**What it says.** The arithmetic mean (the AP middle term) is never smaller than the
geometric mean (the GP middle term).

**Proof.** Since $x, y \geq 0$, the real square roots $\sqrt{x}, \sqrt{y}$ exist. A square
of a real number is nonnegative:

$$
\begin{aligned}
\bigl(\sqrt{x} - \sqrt{y}\bigr)^2 &\geq 0\\
x - 2\sqrt{x}\sqrt{y} + y &\geq 0\\
x + y &\geq 2\sqrt{xy}\\
\frac{x + y}{2} &\geq \sqrt{xy}
\end{aligned}
$$

*Equality.* $\bigl(\sqrt{x} - \sqrt{y}\bigr)^2 = 0$ iff $\sqrt{x} = \sqrt{y}$ iff $x = y$
(squaring is injective on nonnegatives). $\blacksquare$

**Key step.** The entire proof is "a square is nonnegative" — everything else is
rearrangement. That is the standard shape of an inequality proof: exhibit the difference
as an obviously nonnegative quantity.

---

### Theorem 19 (Convergence of an infinite GP — GP-8)

> The series $a + ar + ar^2 + \cdots$ converges iff $|r| < 1$ (for $a \neq 0$), and then
> its sum is $\dfrac{a}{1 - r}$.

**Depends on:** limits of sequences (real analysis).

**Proof.** By Theorem 11, for $r \neq 1$ the partial sums are
$S_n = a \cdot \frac{1 - r^{\,n}}{1 - r}$. So the series converges iff $r^{\,n}$
converges, and the limit is determined by $\lim r^{\,n}$.

*Claim: if $|r| < 1$ then $r^{\,n} \to 0$.* If $r = 0$ this is immediate. Otherwise write
$\frac{1}{|r|} = 1 + h$ with $h > 0$ (possible since $|r| < 1$). By Bernoulli's inequality
($(1+h)^n \geq 1 + nh$ for $h \geq -1$, itself a one-line induction),

$$\frac{1}{|r|^n} = (1 + h)^n \geq 1 + nh \quad \Longrightarrow \quad \bigl|r^{\,n}\bigr| = |r|^n \leq \frac{1}{1 + nh} \to 0$$

so $r^{\,n} \to 0$ by squeezing. Hence
$S_n \to a \cdot \frac{1 - 0}{1 - r} = \frac{a}{1 - r}$.

*Divergence otherwise.* If $|r| > 1$ then $|r|^n \to \infty$, so $S_n$ is unbounded. If
$r = 1$ then $S_n = na \to \pm\infty$. If $r = -1$ then $S_n$ alternates between $a$ and
$0$, so it has no limit. In all these cases the series diverges. $\blacksquare$

---

### Theorem 20 (Correctness of the divide-and-conquer geometric sum)

> Let $G(n) = \sum_{i=0}^{n-1} r^i$ (so $G(0) = 0$). Then for every $k \geq 0$ and
> $n \geq 1$:
> **(a)** $G(2k) = G(k)\bigl(1 + r^{\,k}\bigr)$ **(b)** $G(n) = G(n-1) + r^{\,n-1}$.

**What it says.** These two identities compute a geometric sum modulo anything, using only
$+$ and $\times$ — no division by $r - 1$, so no modular inverse is required. This is the
algorithm in [README.md §6.2](README.md#62-powers-and-geometric-sums-modulo-m--olog-n) and
`geometricSumMod` in [implementation.cpp](implementation.cpp).

**Proof of (a).** Split the $2k$ terms into the first $k$ and the last $k$:

$$G(2k) = \sum_{i=0}^{k-1} r^i \; + \; \sum_{i=k}^{2k-1} r^i$$

In the second sum substitute $i = k + j$, with $j$ running $0, \dots, k-1$:

$$\sum_{i=k}^{2k-1} r^i = \sum_{j=0}^{k-1} r^{\,k+j} = r^{\,k}\sum_{j=0}^{k-1} r^{\,j} = r^{\,k}G(k)$$

Therefore $G(2k) = G(k) + r^{\,k}G(k) = G(k)\bigl(1 + r^{\,k}\bigr)$. $\blacksquare$

**Proof of (b).** Immediate from the definition: $G(n)$ is $G(n-1)$ plus its last term
$r^{\,n-1}$. $\blacksquare$

**Why this terminates and how fast.** Each application of (a) halves $n$; each application
of (b) decreases $n$ by one and makes it even, so (a) applies next. Two steps therefore at
least halve $n$, giving $O(\log n)$ recursive calls. Each call computes one power
$r^{\,k}$ in $O(\log n)$ multiplications by binary exponentiation, so the total is
$O(\log^2 n)$ multiplications. Since every operation is $+$ or $\times$, the recurrence is
valid modulo any $m$, including moduli where $r - 1$ is not invertible. $\blacksquare$

---

### Theorem 21 (Residues of an AP — AP-7)

> Fix $m \geq 1$ and an AP $a_n = a + (n-1)d$. The set of residues
> $\{a_n \bmod m\}$ is:
> **(a)** all of $\{0, 1, \dots, m-1\}$ if $\gcd(d, m) = 1$;
> **(b)** exactly the residues congruent to $a$ modulo $g$, where $g = \gcd(d, m)$,
> otherwise — that is, only $m/g$ of the $m$ classes are ever hit.

**Depends on:** Bézout's identity and modular arithmetic. Stated here because it is the
bridge from progressions to congruences; a full treatment belongs to those concepts.

**Proof of (b) — the restriction.** Let $g = \gcd(d, m)$, so $g \mid d$. Every term
satisfies

$$a_n = a + (n-1)d \equiv a \pmod g$$

since $g \mid (n-1)d$. So no term can lie in a residue class that is not $\equiv a
\pmod g$. Modulo $m$, the classes satisfying $x \equiv a \pmod g$ number exactly $m/g$.
$\blacksquare$

**Proof of (a) — full coverage when $g = 1$.** Let $t$ be any target residue. By Bézout,
$\gcd(d, m) = 1$ gives integers $u, v$ with $du + mv = 1$, hence $du \equiv 1 \pmod m$;
$u$ is a modular inverse of $d$. Choose

$$n - 1 \equiv (t - a)u \pmod m$$

which is solvable by picking any representative in $\{0, \dots, m-1\}$. For that $n$,

$$a_n = a + (n-1)d \equiv a + (t - a)ud \equiv a + (t - a) \cdot 1 = t \pmod m$$

So every residue $t$ is attained. $\blacksquare$

**Consequence.** An AP $a, a+d, a+2d, \dots$ can contain primes in abundance only when
$\gcd(a, d) = 1$ — otherwise every term shares the factor $\gcd(a, d)$. That necessary
condition is exactly the hypothesis of **Dirichlet's theorem on primes in arithmetic
progressions**, which states it is also sufficient (proof far beyond this concept).

---

## Summary of dependencies

| Theorem | Depends on |
|---|---|
| 1–9 (AP) | induction, integer arithmetic only |
| 10–13, 15–16 (GP) | induction, integer arithmetic only |
| 14 ($\sigma(p^k)$) | unique factorization + Theorem 11 |
| 17 (AP $\leftrightarrow$ GP) | properties of logarithms |
| 18 (AM–GM) | order properties of $\mathbb{R}$ |
| 19 (infinite GP) | limits, Bernoulli's inequality |
| 20 (divide-and-conquer sum) | Theorem 11's setting; no division needed |
| 21 (AP mod $m$) | Bézout's identity, modular arithmetic |
