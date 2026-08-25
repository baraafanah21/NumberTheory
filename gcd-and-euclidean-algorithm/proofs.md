# Why it works — GCD and the Euclidean Algorithm

Only the proofs that change what you write. Prerequisites D1 (linearity) and D2
(division algorithm) come from [divisibility](../divisibility/proofs.md).

---

## 1. Why the Euclidean step is valid (G1)

> $(a, b)$ and $(a - qb,\ b)$ have **the same set of common divisors**. Hence
> $\gcd(a,b) = \gcd(b,\ a \bmod b)$.

If $d \mid a$ and $d \mid b$, then $d \mid (a - qb)$ by linearity. Conversely if
$d \mid (a-qb)$ and $d \mid b$, then $d \mid \bigl((a-qb) + qb\bigr) = a$.

So the step loses **no** information — that is why it can be iterated. Taking
$q = \lfloor a/b\rfloor$ makes $a - qb = a \bmod b$.

**Justifies:** `while (b) { r = a % b; a = b; b = r; }`.

---

## 2. Why it terminates, and in $O(\log)$

**Terminates:** remainders satisfy $0 \leq r < \lvert b \rvert$, so the second argument
strictly decreases and is bounded below by $0$. It must hit $0$.

**$O(\log)$:** two steps at least halve the value. Let $r_k$ be the remainder sequence.

- If $r_{k+1} \leq r_k/2$, then $r_{k+2} < r_{k+1} \leq r_k/2$.
- If $r_{k+1} > r_k/2$, then the quotient is exactly $1$, so
  $r_{k+2} = r_k - r_{k+1} < r_k/2$.

Either way $r_{k+2} < r_k/2$, giving at most $2\log_2\min(a,b)$ steps.

**Worst case is Fibonacci.** If $n$ steps are needed then $a \geq F_{n+2}$, $b \geq F_{n+1}$
(induction: $a = qb + r \geq b + r \geq F_{n+1} + F_n$). So consecutive Fibonacci numbers
are the smallest inputs requiring $n$ steps — the sharp bound is $\approx 1.44\log_2$.

**Practical takeaway:** under 90 iterations for any 64-bit input. Stress-test with
Fibonacci pairs; never worry about the running time.

---

## 3. Bézout, and why reachability works (G6)

> There exist $x, y$ with $ax + by = \gcd(a,b)$, and
> $$\{ax + by : x, y \in \mathbb{Z}\} = \{\text{multiples of } \gcd(a,b)\}$$

Let $g$ be the **smallest positive** value of $ax + by$ (it exists: $a^2 + b^2 > 0$ is one
such value). Divide $a$ by $g$: $a = qg + r$ with $0 \leq r < g$. Then

$$r = a - qg = a - q(ax_0 + by_0) = a(1 - qx_0) + b(-qy_0)$$

is *also* a combination. If $r > 0$ it would be a positive combination smaller than $g$ —
impossible. So $r = 0$, i.e. $g \mid a$; likewise $g \mid b$. And any common divisor $c$
divides $ax_0 + by_0 = g$ by linearity, so $c \leq g$. Hence $g = \gcd(a,b)$.

For the set: every combination is a multiple of $g$ (linearity), and every multiple $kg$ is
the combination $a(kx_0) + b(ky_0)$.

**Justifies:**
- steps of $\pm a, \pm b$ reach exactly the multiples of $\gcd(a,b)$;
- $ax + by = c$ is solvable **iff** $\gcd(a,b) \mid c$;
- every common divisor *divides* the gcd — the property later proofs need.

*This proof is non-constructive: it does not produce $x, y$. That is what the extended
Euclidean algorithm is for.*

---

## 4. Euclid's lemma (G7)

> $\gcd(a,b) = 1$ and $a \mid bc$ $\Rightarrow$ $a \mid c$. In particular for prime $p$:
> $p \mid bc \Rightarrow p \mid b$ or $p \mid c$.

By Bézout, $ax + by = 1$. Multiply by $c$:

$$acx + bcy = c$$

$a$ divides $acx$ trivially, and $a \mid bc$ gives $a \mid bcy$. By linearity $a$ divides
the sum, which is $c$.

For the prime version: if $p \nmid b$ then $\gcd(p,b)$ must be $1$ (the only divisors of
$p$ are $1$ and $p$), so the first part applies.

**Justifies:** this is the fix for divisibility's D7 trap. It is also exactly what makes
prime factorization **unique** — which is why factorization-based algorithms are allowed to
assume "the" factorization exists.

---

## 5. Why the lcm formula is exact (G2)

> $\gcd(a,b)\cdot\mathrm{lcm}(a,b) = \lvert ab \rvert$

Let $g = \gcd(a,b)$, $a = ga'$, $b = gb'$ with $\gcd(a',b') = 1$. Set $L = \lvert ab\rvert/g$.

- $L$ is a common multiple: $L = \lvert a \rvert \cdot \frac{\lvert b\rvert}{g}$ and
  $g \mid b$, so $a \mid L$; symmetrically $b \mid L$.
- $L$ divides every common multiple: if $M = as = bt$ then $a's = b't$, so $b' \mid a's$;
  since $\gcd(a',b')=1$, Euclid's lemma gives $b' \mid s$, and substituting shows
  $\frac{ab}{g} \mid M$.

**Justifies:** `(a / g) * b`. The early division is *exact* because $g \mid a$ — that is
what makes reordering safe rather than a hack.

---

## 6. $\gcd(a^m - 1,\ a^n - 1) = a^{\gcd(m,n)} - 1$ (G8)

Assume $m \geq n$. The key identity:

$$a^m - 1 = a^{m-n}\bigl(a^n - 1\bigr) + \bigl(a^{m-n} - 1\bigr)$$

(expand: $a^m - a^{m-n} + a^{m-n} - 1$ ✓). So $a^{m-n}-1$ is
$(a^m-1) - q(a^n-1)$ with $q = a^{m-n}$, and by §1:

$$\gcd(a^m-1,\ a^n-1) = \gcd(a^{m-n}-1,\ a^n-1)$$

The exponents just went $(m,n) \to (m-n,\ n)$ — a *subtractive* Euclidean step. Induct
until one exponent is $0$, where $\gcd(a^m-1, 0) = a^m - 1$ and $\gcd(m,0) = m$.

**Justifies:** `ipow(a, gcd(m, n)) - 1`. A gcd of 300000-digit numbers becomes a gcd of two
small exponents.

---

## 7. Binary GCD identities

For $u, v > 0$:

| Case | Identity | Why |
|---|---|---|
| both even | $\gcd(u,v) = 2\gcd(u/2, v/2)$ | G4 with $c = 2$ |
| $v$ odd | $\gcd(u,v) = \gcd(u/2, v)$ | $g \mid v$ makes $g$ odd, so $\gcd(g,2)=1$ and Euclid's lemma pushes $g$ into $u/2$ |
| both odd | $\gcd(u,v) = \gcd(u-v, v)$ | §1 with $q = 1$ |

Odd $-$ odd is even, so the third case feeds the second and every iteration removes a bit
— $O(\log\max)$ iterations.

**Justifies:** the division-free variant. On modern CPUs with fast hardware division, plain
Euclid is usually just as fast for 64-bit; binary GCD wins for big integers.

---

## 8. Facts used without proof

Short enough to check yourself, and nothing downstream depends on the argument:

- **Associativity** $\gcd(a,b,c) = \gcd(\gcd(a,b),c)$: both sides divide all three and are
  divisible by every common divisor (§3), so they divide each other.
- **$\gcd(ca,cb) = \lvert c\rvert\gcd(a,b)$**: scale the whole set of combinations in §3 by $c$.
- **$\gcd(a/g, b/g) = 1$**: apply the previous line with $c = g$ to $\gcd(a,b) = g$.
- **Prime-factorization form** ($\min$ of exponents): true, but requires factoring, so it is
  for proofs only — never an algorithm.
