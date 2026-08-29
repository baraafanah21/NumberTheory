# Proofs — Unique Factorization

Five results. The first two are the theorem itself; the rest are what it unlocks.

**Borrowed from [gcd](../gcd-and-euclidean-algorithm/proofs.md):** **Euclid's lemma** — if
$p$ is prime and $p \mid ab$, then $p \mid a$ or $p \mid b$. Everything here depends on it,
and nothing here is used to prove it (that would be circular — Euclid's lemma comes from
Bézout).

---

## 1. Existence: every $n > 1$ is a product of primes

**Claim.** Every integer $n > 1$ can be written as a product of one or more primes.

**In words.** Keep splitting until nothing splits any further. The splitting must stop,
because the pieces get strictly smaller and cannot go below $2$.

**Proof (strong induction on $n$).** Suppose the claim holds for every integer strictly
between $1$ and $n$.

**Case 1: $n$ is prime.** Then $n$ is a product of one prime — itself. ✓

**Case 2: $n$ is composite.** By definition there is a factorization $n = a\,b$ with
$1 < a < n$ and $1 < b < n$. Both $a$ and $b$ lie strictly between $1$ and $n$, so the
induction hypothesis applies to each: both are products of primes. Concatenating those two
lists writes $n$ as a product of primes. ✓

By strong induction the claim holds for all $n > 1$. $\blacksquare$

**Why strong induction and not ordinary induction.** The factors $a$ and $b$ are *some*
smaller numbers, not $n-1$. You need the hypothesis for **all** smaller values, which is
exactly what strong induction gives.

---

## 2. Uniqueness

**Claim.** If

$$p_1 p_2 \cdots p_r \;=\; q_1 q_2 \cdots q_s$$

with all $p_i, q_j$ prime, then $r = s$ and the two lists are the same up to reordering.

**In words.** This is the half that actually needs Euclid's lemma. Existence just needed
"numbers get smaller"; uniqueness needs to know that a prime cannot be *spread across*
several factors.

**First, a corollary of Euclid's lemma we need.**

> If a prime $p$ divides a product $a_1a_2\cdots a_k$, then $p \mid a_i$ for some $i$.

*Proof:* induction on $k$. For $k=1$ it is trivial. For $k > 1$, group the product as
$a_1 \cdot (a_2\cdots a_k)$ and apply Euclid's lemma: either $p \mid a_1$ (done), or
$p \mid a_2\cdots a_k$, and the induction hypothesis finishes it. $\blacksquare$

**Proof of uniqueness (induction on $r$).**

**Base $r = 1$.** Then $p_1 = q_1\cdots q_s$. Since $p_1$ is prime its only divisors are
$1$ and itself, so no $q_j$ can be a proper factor; hence $s = 1$ and $p_1 = q_1$. ✓

**Inductive step.** Consider $p_1$. It divides the left side, hence the right side:

$$p_1 \ \big|\ q_1q_2\cdots q_s$$

By the corollary, $p_1 \mid q_j$ for some $j$. But $q_j$ is **prime**, so its only divisors
are $1$ and $q_j$ — and $p_1 \neq 1$. Therefore

$$p_1 = q_j$$

Reorder the right-hand list so that $j = 1$, and cancel the common factor $p_1 = q_1$ from
both sides (legal — it is nonzero):

$$p_2 \cdots p_r \;=\; q_2 \cdots q_s$$

This is the same statement with $r-1$ primes on the left, so by the induction hypothesis
the remaining lists match up to order. Adding back the pair $p_1 = q_1$ gives the full
result. $\blacksquare$

**The one step that carries all the weight.** "$p_1$ divides the product, therefore
$p_1$ *equals* one of the $q_j$." Both halves are needed: Euclid's lemma to land on a
single $q_j$, and primality of $q_j$ to upgrade "divides" to "equals".

**Where it fails without Euclid's lemma.** In the even-numbers system from the README,
$60 = 2\times30 = 6\times10$ with all four factors "prime" there. The step above breaks
immediately: $6$ divides $2 \times 30$ but equals neither.

**Why $1$ must not be called prime.** If it were, $6 = 2\cdot3 = 1\cdot2\cdot3 = 1\cdot1\cdot2\cdot3$
would all be distinct factorizations, and uniqueness would be false by convention alone.

---

## 3. Divisors are exactly the exponent-wise smaller numbers

**Claim.** Let $n = p_1^{e_1}\cdots p_k^{e_k}$ be canonical. Then for $d > 0$:

$$d \mid n \quad\Longleftrightarrow\quad d = p_1^{f_1}\cdots p_k^{f_k} \ \text{ with } \ 0 \le f_i \le e_i \ \text{ for every } i$$

and distinct exponent vectors give distinct divisors.

**In words.** A divisor cannot use a prime that $n$ does not have, and cannot use one more
copies than $n$ has.

**Proof.** ($\Leftarrow$) If $f_i \le e_i$ for all $i$, then

$$n = d \cdot \prod_i p_i^{\,e_i - f_i}$$

and every exponent $e_i - f_i$ is $\ge 0$, so the second factor is a positive integer.
Hence $d \mid n$.

($\Rightarrow$) Suppose $d \mid n$, say $n = d\,c$. Factor $d$ and $c$ into primes
(§1) and concatenate: this gives a prime factorization of $n$. By **uniqueness** (§2) it
must be *the* factorization of $n$. So every prime appearing in $d$ is one of the $p_i$,
and the number of copies of $p_i$ in $d$ plus the number in $c$ equals $e_i$. Since the
count in $c$ is $\ge 0$, the count in $d$ is at most $e_i$.

Distinctness: two different exponent vectors give two different canonical forms, hence
different numbers — again by uniqueness. $\blacksquare$

**In code.** This is what makes the formulas from earlier concepts valid:

$$\tau(n) = \prod_i (e_i+1) \qquad\text{(choose each } f_i \text{ independently from } e_i+1 \text{ options)}$$

$$\sigma(n) = \prod_i \left(1 + p_i + \cdots + p_i^{e_i}\right) = \prod_i \frac{p_i^{\,e_i+1}-1}{p_i-1}$$

(expanding that product produces each divisor exactly once — which is precisely the
distinctness clause). It is also why the `spf` factorization loop in the
[sieve](../sieve-of-eratosthenes/) returns *the* factorization.

---

## 4. gcd and lcm by exponents

**Claim.** Writing $a = \prod_p p^{\alpha_p}$ and $b = \prod_p p^{\beta_p}$ (exponents
$\ge 0$, finitely many nonzero):

$$\gcd(a,b) = \prod_p p^{\min(\alpha_p,\beta_p)}, \qquad \mathrm{lcm}(a,b) = \prod_p p^{\max(\alpha_p,\beta_p)}$$

**Proof.** By §3, $d$ is a common divisor of $a$ and $b$ exactly when its exponent vector
$(\delta_p)$ satisfies

$$\delta_p \le \alpha_p \ \text{ and } \ \delta_p \le \beta_p \quad\text{for every } p
\qquad\Longleftrightarrow\qquad \delta_p \le \min(\alpha_p,\beta_p)$$

Such a $d$ is largest when every $\delta_p$ is as large as allowed, i.e.
$\delta_p = \min(\alpha_p,\beta_p)$. The lcm argument is identical with the inequalities
reversed. $\blacksquare$

**An instant corollary.** Since $\min(x,y) + \max(x,y) = x + y$ for every pair,

$$\gcd(a,b)\cdot\mathrm{lcm}(a,b) = \prod_p p^{\min + \max} = \prod_p p^{\alpha_p+\beta_p} = a\,b$$

which re-derives the identity from the gcd concept in one line — though at the cost of the
whole factorization theorem, whereas the original proof needed only Euclid's lemma.

**Do not use this to compute.** Factoring is exponentially slower than Euclid. This
characterization is for *reasoning*, not for code.

---

## 5. $\sqrt{2}$ is irrational

**Claim.** There are no integers $a, b$ with $\left(\frac ab\right)^2 = 2$.

**In words.** A short, complete application — and a good demonstration that uniqueness has
real consequences rather than being bookkeeping.

**Proof.** Suppose $a^2 = 2\,b^2$ with $a, b$ positive integers. Count the copies of the
prime $2$ on each side, using the fact (from §2) that the exponent of a prime in a number is
well defined.

Let $\alpha$ be the exponent of $2$ in $a$, and $\beta$ the exponent of $2$ in $b$. Squaring
doubles every exponent, so:

$$\text{exponent of } 2 \text{ in } a^2 = 2\alpha, \qquad \text{exponent of } 2 \text{ in } 2b^2 = 1 + 2\beta$$

Since $a^2$ and $2b^2$ are the same number, uniqueness forces their exponents to match:

$$2\alpha = 1 + 2\beta$$

The left side is even and the right side is odd — impossible. $\blacksquare$

**Why this needs unique factorization.** The argument is only valid because "the exponent
of $2$ in $n$" is a well-defined quantity. Without uniqueness a number could have two
factorizations with different numbers of $2$s, and the comparison would be meaningless.

**Generalizing for free.** The same parity argument shows $\sqrt{p}$ is irrational for
every prime $p$, and more broadly that $\sqrt n$ is rational only when $n$ is a perfect
square — because $\sqrt n$ rational forces every exponent in $n$ to be even.
