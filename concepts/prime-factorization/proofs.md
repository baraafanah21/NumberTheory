# Proofs — Unique Factorization

**Borrowed** from [gcd](../gcd-and-euclidean-algorithm/proofs.md): **Euclid's lemma** —
$p$ prime and $p \mid ab$ $\Rightarrow$ $p \mid a$ or $p \mid b$. Everything rests on it,
and nothing here is used to prove it (that would be circular — it comes from Bézout).

---

## 1. Existence

**Claim.** Every $n>1$ is a product of primes.

**Proof (strong induction).** If $n$ is prime it is a product of one. Otherwise
$n = ab$ with $1 < a,b < n$; both lie strictly between $1$ and $n$, so by the induction
hypothesis both are products of primes, and concatenating the lists gives one for $n$.
$\blacksquare$

**Strong** induction, because the factors are *some* smaller numbers, not $n-1$.

---

## 2. Uniqueness

**Claim.** If $p_1\cdots p_r = q_1\cdots q_s$ with all factors prime, the two lists agree
up to order.

**First, a corollary of Euclid's lemma.** *If $p$ divides $a_1a_2\cdots a_k$ then
$p \mid a_i$ for some $i$.* Induction on $k$: group as $a_1\cdot(a_2\cdots a_k)$ and apply
the lemma. $\blacksquare$

**Proof (induction on $r$).** For $r=1$: $p_1 = q_1\cdots q_s$ is prime, so no $q_j$ can be
a proper factor, forcing $s=1$.

For $r>1$: $p_1$ divides the left side, hence the right, so by the corollary
$p_1 \mid q_j$ for some $j$. But $q_j$ is **prime**, so its only divisors are $1$ and
itself — and $p_1 \neq 1$. Therefore

$$p_1 = q_j$$

Reorder so $j=1$, cancel the common factor, and apply the induction hypothesis to
$p_2\cdots p_r = q_2\cdots q_s$. $\blacksquare$

**The step carrying all the weight:** "$p_1$ divides the product, therefore $p_1$
**equals** some $q_j$." Euclid's lemma lands you on one factor; primality upgrades
"divides" to "equals". In the even-numbers system this breaks at once — $6$ divides
$2\times30$ but equals neither.

**Why $1$ is not prime.** Otherwise $6 = 2\cdot3 = 1\cdot2\cdot3 = 1\cdot1\cdot2\cdot3$
would be distinct factorizations, and uniqueness would fail by convention alone.

---

## 3. Divisors are the exponent-wise smaller numbers

**Claim.** For $n = \prod p_i^{e_i}$ canonical and $d>0$:

$$d \mid n \iff d = \prod p_i^{f_i} \ \text{ with } \ 0 \le f_i \le e_i$$

and distinct exponent vectors give distinct divisors.

**Proof.** ($\Leftarrow$) $n = d\cdot\prod p_i^{\,e_i-f_i}$ and every exponent is $\ge 0$.

($\Rightarrow$) If $n = dc$, factor $d$ and $c$ and concatenate: that is a prime
factorization of $n$, so by **uniqueness** it *is* the factorization of $n$. Hence every
prime in $d$ is some $p_i$, and its count in $d$ plus its count in $c$ equals $e_i$ — so
the count in $d$ is at most $e_i$. Distinctness is uniqueness again. $\blacksquare$

**Justifies** $\tau(n) = \prod(e_i+1)$ (choose each $f_i$ independently) and
$\sigma(n) = \prod\bigl(1+p_i+\cdots+p_i^{e_i}\bigr)$ (expanding the product produces each
divisor exactly once — that is the distinctness clause). Also why the `spf` loop in the
sieve returns *the* factorization.

---

## 4. gcd and lcm by exponents

**Claim.** $\gcd(a,b) = \prod_p p^{\min(\alpha_p,\beta_p)}$ and
$\mathrm{lcm}(a,b) = \prod_p p^{\max(\alpha_p,\beta_p)}$.

**Proof.** By §3, $d$ is a common divisor iff $\delta_p \le \alpha_p$ and
$\delta_p \le \beta_p$ for every $p$ — that is, $\delta_p \le \min(\alpha_p,\beta_p)$. It
is largest when every $\delta_p$ is as large as allowed. Reverse the inequalities for lcm.
$\blacksquare$

**Corollary in one line.** $\min(x,y)+\max(x,y) = x+y$, so
$\gcd(a,b)\cdot\mathrm{lcm}(a,b) = ab$ — though at the cost of the whole factorization
theorem, whereas the original proof needed only Euclid's lemma.

---

## 5. $\sqrt2$ is irrational

**Claim.** No integers $a,b$ satisfy $a^2 = 2b^2$.

**Proof.** Count the copies of the prime $2$ on each side — legitimate because §2 makes
"the exponent of $2$" well defined. Squaring doubles every exponent, so with $\alpha$ the
exponent of $2$ in $a$ and $\beta$ that in $b$:

$$\underbrace{2\alpha}_{\text{in } a^2} \;=\; \underbrace{1 + 2\beta}_{\text{in } 2b^2}$$

Even equals odd — impossible. $\blacksquare$

**This needs uniqueness.** Without it a number could have two factorizations with different
numbers of $2$s and the comparison would be meaningless.

**Free generalization.** The same parity argument shows $\sqrt n$ is rational only when $n$
is a perfect square.
