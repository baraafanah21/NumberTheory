# Proofs — Divisibility

Every theorem here corresponds to a property stated in [README.md](README.md).

**Notation.** All letters denote integers. $a \mid b$ means $\exists k \in \mathbb{Z}$ with
$b = ak$. For $n \geq 1$, $D(n)$ is the set of *positive* divisors of $n$,
$\tau(n) = \lvert D(n) \rvert$, and $\sigma(n) = \sum_{d \in D(n)} d$.

**Dependencies.** Theorems 1–11 use nothing but integer arithmetic and the well-ordering
principle — this is the base of the repository. Theorem 12 uses unique factorization and
GP-7; Theorem 13 is self-contained; Theorem 14 uses GP-5 from
[arithmetic-and-geometric-progressions](../arithmetic-and-geometric-progressions/proofs.md).

**Where these are used.** Theorems 3, 4, 5 and 2 are exactly the facts **D1, D2, D3, D4**
imported by [gcd-and-euclidean-algorithm](../gcd-and-euclidean-algorithm/proofs.md).

**A note on proof style.** Almost every proof below has the same shape: *unpack the
definition into a witness $k$, do algebra, repack into a witness*. Divisibility is defined
by multiplication, so its proofs never divide anything. Watch for that pattern — it is the
main technique of the whole subject.

---

## Part I — The relation

### Theorem 1 (Basic facts)

> For every integer $a$: **(a)** $a \mid a$; **(b)** $1 \mid a$; **(c)** $a \mid 0$;
> **(d)** $a \mid 1$ if and only if $a = \pm 1$; **(e)** $0 \mid b$ if and only if $b = 0$.

**Proof.** Each is an instance of "produce the witness $k$".

(a) $a = a \cdot 1$, so $k = 1$ works.

(b) $a = 1 \cdot a$, so $k = a$ works.

(c) $0 = a \cdot 0$, so $k = 0$ works. Note this holds for $a = 0$ too, giving $0 \mid 0$.

(d) $(\Leftarrow)$ $1 = 1\cdot 1$ and $1 = (-1)(-1)$. $(\Rightarrow)$ If $a \mid 1$ then
$1 = ak$ for some integer $k$. Neither $a$ nor $k$ is $0$, so
$\lvert a \rvert \geq 1$ and $\lvert k \rvert \geq 1$; if $\lvert a \rvert \geq 2$ then
$\lvert ak \rvert \geq 2 > 1$, a contradiction. Hence $\lvert a \rvert = 1$.

(e) $(\Leftarrow)$ by (c). $(\Rightarrow)$ If $0 \mid b$ then $b = 0 \cdot k = 0$.
$\blacksquare$

**Why (c) and (e) matter.** They are not edge cases to be waved away — they are what make
$\gcd(0,0)$ need a convention, and they are why the divisor $0$ must be excluded from the
division algorithm.

---

### Theorem 2 (Transitivity — **D4**)

> If $c \mid b$ and $b \mid a$, then $c \mid a$.

**Proof.** By hypothesis there are integers $m, n$ with

$$b = cm, \qquad a = bn$$

Substituting the first into the second:

$$a = (cm)n = c(mn)$$

and $mn$ is an integer, so $c \mid a$. $\blacksquare$

**Key step.** Composing the two witnesses into one, $k = mn$. That is the whole proof.

---

### Theorem 3 (Linearity — **D1**)

> If $d \mid a$ and $d \mid b$, then $d \mid (ax + by)$ for **all** integers $x, y$.

**What it says.** Divisibility survives arbitrary integer combinations. Special cases
worth naming: $d \mid (a + b)$, $d \mid (a - b)$, and $d \mid ac$ for every $c$.

**Why it is the workhorse.** Nearly every argument in elementary number theory takes the
form "$d$ divides these two things, therefore it divides this combination of them". The
Euclidean algorithm is one application; the divisibility tests of Theorem 14 are another.

**Proof.** By hypothesis $a = dm$ and $b = dn$ for integers $m, n$. Then

$$ax + by = (dm)x + (dn)y = d(mx + ny)$$

and $mx + ny$ is an integer, so $d \mid (ax + by)$. $\blacksquare$

**Corollary 3.1 (many terms).** If $d \mid a_i$ for $i = 1, \dots, k$, then
$d \mid \sum_{i=1}^{k} a_i x_i$ for any integers $x_i$. *Proof:* induction on $k$, applying
Theorem 3 at each step. Used in Theorem 14, where a number is a sum of many digit terms.

**Corollary 3.2 (subtracting a known multiple).** If $d \mid b$, then
$d \mid a \iff d \mid (a - qb)$ for every integer $q$. *Proof:* both directions are
Theorem 3, once with coefficients $(1, -q)$ and once with $(1, q)$. *This is precisely the
Euclidean step*, and it is why the GCD concept's Theorem 2 needs nothing beyond D1.

---

### Theorem 4 (Size bound — **D2**)

> If $d \mid a$ and $a \neq 0$, then $\lvert d \rvert \leq \lvert a \rvert$.

**The hypothesis is essential.** Without $a \neq 0$ the statement is false in the worst
possible way: every integer divides $0$, so a divisor of $0$ can be arbitrarily large.

**Proof.** Write $a = dk$. If $k = 0$ then $a = 0$, contradicting the hypothesis; so
$k \neq 0$ and hence $\lvert k \rvert \geq 1$ (a nonzero integer has absolute value at
least $1$). Therefore

$$\lvert a \rvert = \lvert d \rvert \cdot \lvert k \rvert \geq \lvert d \rvert \cdot 1 = \lvert d \rvert$$

$\blacksquare$

**Key step.** "A nonzero integer has absolute value $\geq 1$" — the discreteness of
$\mathbb{Z}$. Nothing analogous holds in $\mathbb{Q}$, which is exactly why divisibility is
interesting over $\mathbb{Z}$ and trivial over a field.

**Corollary 4.1 (finiteness).** For $a \neq 0$, the set of divisors of $a$ is finite,
contained in $[-\lvert a \rvert, \lvert a \rvert]$. This is what lets "the greatest common
divisor" exist.

---

### Theorem 5 (Antisymmetry — **D6**)

> If $a \mid b$ and $b \mid a$, then $a = \pm b$. Consequently, restricted to
> $\mathbb{Z}_{>0}$, the relation $\mid$ is a partial order.

**Proof.** *Case $a = 0$.* Then $a \mid b$ gives $b = 0$ by Theorem 1(e), so $a = b$.
Symmetrically if $b = 0$.

*Case both nonzero.* Theorem 4 applies in both directions:

$$\lvert a \rvert \leq \lvert b \rvert \quad\text{and}\quad \lvert b \rvert \leq \lvert a \rvert$$

so $\lvert a \rvert = \lvert b \rvert$, i.e. $a = \pm b$. $\blacksquare$

**Partial order.** On $\mathbb{Z}_{>0}$: reflexive (Theorem 1a), transitive (Theorem 2),
and antisymmetric (just proved, with the $\pm$ collapsing to $=$ for positives). It is
*partial*, not total: $4 \nmid 6$ and $6 \nmid 4$, so the two are incomparable.

**How this gets used.** "Each divides the other, hence they are equal" is a standard way to
finish a proof about non-negative quantities — it appears three times in the GCD concept
(Theorems 8, 12 and 15 there).

---

### Theorem 6 (Scaling and cancellation — **D7**)

> **(a)** If $a \mid b$ then $ac \mid bc$ for every $c$.
> **(b)** If $ac \mid bc$ and $c \neq 0$, then $a \mid b$.

**Proof of (a).** From $b = ak$ we get $bc = (ac)k$, the same witness $k$. $\blacksquare$

**Proof of (b).** From $ac \mid bc$ there is $k$ with $bc = (ac)k = (ak)c$. Since
$c \neq 0$ we may cancel it in the integers:

$$bc = (ak)c \quad\Longrightarrow\quad b = ak$$

so $a \mid b$. $\blacksquare$

**Why $c \neq 0$ is not optional.** With $c = 0$, the hypothesis $ac \mid bc$ reads
$0 \mid 0$, which is *always* true (Theorem 1c). So dropping the condition would let you
"derive" $a \mid b$ for every pair — for instance $5 \mid 7$. The cancellation
$bc = (ak)c \Rightarrow b = ak$ is exactly the step that fails.

---

## Part II — The division algorithm

### Theorem 7 (Division algorithm — **D3**)

> For all integers $a$ and $b \neq 0$ there exist **unique** integers $q, r$ with
> $$a = qb + r, \qquad 0 \leq r < \lvert b \rvert$$

**What it says.** Every division leaves a remainder, that remainder is genuinely smaller
than the divisor, and both $q$ and $r$ are pinned down with no freedom. Existence powers
the Euclidean algorithm; **uniqueness** is what makes "$a \bmod b$" a well-defined function
rather than a choice.

**Proof of existence (well-ordering).** Consider the set of non-negative values that $a$
can be reduced to by subtracting multiples of $b$:

$$S = \{\, a - qb \ :\ q \in \mathbb{Z},\ a - qb \geq 0 \,\}$$

*$S$ is nonempty.* Take $q = -\lvert a \rvert \operatorname{sgn}(b)$, so that
$qb = -\lvert a \rvert \lvert b \rvert$. Then

$$a - qb = a + \lvert a \rvert\lvert b \rvert \geq a + \lvert a \rvert \geq 0$$

using $\lvert b \rvert \geq 1$ (as $b \neq 0$) and $\lvert a \rvert \geq -a$.

*$S$ has a least element.* $S$ is a nonempty set of non-negative integers, so by the
well-ordering principle it has a minimum. Call it $r = a - qb \geq 0$ for the corresponding
$q$. This already gives $a = qb + r$ with $r \geq 0$.

*The remainder is small: $r < \lvert b \rvert$.* Suppose instead $r \geq \lvert b \rvert$.
Since $\operatorname{sgn}(b)\,b = \lvert b \rvert$,

$$r - \lvert b \rvert = a - qb - \operatorname{sgn}(b)\,b = a - \bigl(q + \operatorname{sgn}(b)\bigr)b$$

so $r - \lvert b \rvert$ is again of the form $a - q'b$, and it is $\geq 0$ by assumption —
hence it lies in $S$. But $\lvert b \rvert > 0$ makes $r - \lvert b \rvert < r$,
contradicting the minimality of $r$. Therefore $r < \lvert b \rvert$. $\blacksquare$

**Key step.** Choosing $S$ to be the set of *non-negative* reductions, then letting
minimality force $r$ below $\lvert b \rvert$: if the remainder were still as large as the
divisor, you could subtract once more, contradicting that you had already gone as low as
possible.

**Proof of uniqueness.** Suppose

$$a = q_1b + r_1 = q_2b + r_2, \qquad 0 \leq r_1, r_2 < \lvert b \rvert$$

Rearranging, $(q_1 - q_2)b = r_2 - r_1$. Take absolute values:

$$\lvert q_1 - q_2 \rvert \cdot \lvert b \rvert = \lvert r_2 - r_1 \rvert$$

Both remainders lie in $[0, \lvert b \rvert)$, so their difference satisfies
$\lvert r_2 - r_1 \rvert < \lvert b \rvert$. Combining,

$$\lvert q_1 - q_2 \rvert \cdot \lvert b \rvert < \lvert b \rvert
\quad\Longrightarrow\quad \lvert q_1 - q_2 \rvert < 1$$

after dividing by $\lvert b \rvert > 0$. But $\lvert q_1 - q_2 \rvert$ is a non-negative
integer, and the only such integer below $1$ is $0$. So $q_1 = q_2$, and then
$r_1 = a - q_1b = a - q_2b = r_2$. $\blacksquare$

**Key step.** "Two remainders in a window of width $\lvert b \rvert$ differ by less than
$\lvert b \rvert$." Discreteness finishes it: a non-negative integer $< 1$ must be $0$.

---

### Theorem 8 (Divisibility is a zero remainder)

> For $b \neq 0$: $\quad b \mid a \iff$ the remainder of $a$ divided by $b$ is $0$.

**Proof.** $(\Leftarrow)$ If $r = 0$ then $a = qb$, so $b \mid a$ with witness $q$.

$(\Rightarrow)$ If $b \mid a$, write $a = bk$. Then

$$a = kb + 0, \qquad 0 \leq 0 < \lvert b \rvert$$

is a valid decomposition in the sense of Theorem 7. By the **uniqueness** half of
Theorem 7, it is *the* decomposition, so the remainder is $0$. $\blacksquare$

**Why uniqueness is doing the work.** Without it, exhibiting one decomposition with
remainder $0$ would not rule out another with a nonzero remainder. This is the theorem that
licenses testing `b % a == 0` in code, so it is worth knowing it rests on uniqueness.

---

## Part III — The divisors of a number

### Theorem 9 (Divisor pairing — **D8**)

> Let $n \geq 1$. The map $\varphi(d) = n/d$ is a well-defined bijection
> $D(n) \to D(n)$, is its own inverse, and satisfies
> $$\min\bigl(d,\ n/d\bigr) \leq \sqrt{n} \quad \text{for every } d \in D(n)$$

**Why it matters.** This is the theorem that makes divisor enumeration cost $O(\sqrt n)$
instead of $O(n)$ — every algorithm in §6 of the README rests on it.

**Proof that $\varphi$ is well defined.** Let $d \in D(n)$, so $d \geq 1$ and $n = dk$ for
some integer $k$. Then $n/d = k$ is a positive integer (positive because $n, d > 0$), and
$k \mid n$ since $n = k \cdot d$ exhibits the witness $d$. So $\varphi(d) = k \in D(n)$.

**Proof that $\varphi$ is an involution, hence a bijection.**

$$\varphi\bigl(\varphi(d)\bigr) = \frac{n}{\ n/d\ } = d$$

A map equal to its own inverse is a bijection.

**Proof of the $\sqrt n$ bound.** Suppose, for contradiction, that both $d > \sqrt n$ and
$n/d > \sqrt n$. Multiplying two inequalities between positive numbers:

$$n = d \cdot \frac{n}{d} > \sqrt{n}\cdot\sqrt{n} = n$$

which is absurd. Hence at least one of the pair is $\leq \sqrt n$. $\blacksquare$

**Corollary 9.1 (enumeration is complete).** Every divisor of $n$ is either some
$d \leq \lfloor\sqrt n\rfloor$ with $d \mid n$, or is $n/d$ for such a $d$. So the loop
"for $d = 1$ to $\lfloor\sqrt n\rfloor$, if $d \mid n$ emit $d$ and $n/d$" emits every
divisor at least once — and exactly once, provided the fixed point $d = n/d$ is emitted a
single time.

---

### Theorem 10 (Odd divisor count — **D9**)

> For $n \geq 1$: $\ \tau(n)$ is odd $\iff n$ is a perfect square.

**Intuition.** Divisors come in couples. A couple contributes $2$ to the count, so the
count can only be odd if some divisor is its own partner — and a divisor is its own partner
exactly when it is $\sqrt n$.

**Proof.** By Theorem 9, $\varphi(d) = n/d$ is an involution on $D(n)$. An involution
partitions its domain into orbits $\{d, \varphi(d)\}$ of size $2$, together with fixed
points (orbits of size $1$). Therefore

$$\tau(n) \equiv \#\{\text{fixed points}\} \pmod 2$$

since every size-$2$ orbit contributes an even amount.

Now identify the fixed points. For $d \in D(n)$,

$$\varphi(d) = d \iff \frac{n}{d} = d \iff n = d^2$$

*If $n$ is not a perfect square*, no such $d$ exists, so there are $0$ fixed points and
$\tau(n)$ is even.

*If $n = m^2$ with $m \geq 1$*, then $d = m$ is a fixed point, and it is the only one: $d$
is positive and $d^2 = n$ determines $d = \sqrt n = m$ uniquely among positives. So there
is exactly $1$ fixed point and $\tau(n)$ is odd. $\blacksquare$

**Key step.** Counting modulo $2$ by orbits, rather than counting divisors directly. This
"pair things up and look at what is left over" argument is worth internalizing — it solves
the locker problem and many like it in one line.

---

### Theorem 11 (Trial division up to $\sqrt n$ suffices)

> Let $n > 1$. If $n$ is composite, then $n$ has a divisor $d$ with $2 \leq d \leq \sqrt n$.
> Equivalently: if no integer in $[2, \sqrt n]$ divides $n$, then $n$ is prime.

**Proof.** Let $n$ be composite, so $n = ab$ with $1 < a$ and $1 < b$ (both factors
non-trivial). Relabel so that $a \leq b$. Then

$$a^2 \leq ab = n \quad\Longrightarrow\quad a \leq \sqrt n$$

and $a \geq 2$ since $a > 1$ and $a$ is an integer. So $a$ is the required divisor.

The second statement is the contrapositive: if no $d \in [2, \sqrt n]$ divides $n$, then
$n$ cannot be composite, and being $> 1$ it is prime. $\blacksquare$

**Key step.** Ordering the two factors. Without $a \leq b$ there is no reason for either to
be small; with it, the smaller one is forced below $\sqrt n$.

---

### Theorem 12 ($\tau$ and $\sigma$ from the factorization)

> Let $n = p_1^{e_1}p_2^{e_2}\cdots p_k^{e_k}$ with distinct primes $p_i$ and $e_i \geq 1$.
> Then
> $$\tau(n) = \prod_{i=1}^{k}(e_i + 1), \qquad
> \sigma(n) = \prod_{i=1}^{k}\frac{p_i^{\,e_i+1} - 1}{p_i - 1}$$

**Depends on:** unique factorization, and GP-7 from the progressions concept.

**Step 1 — describe the divisors.** By unique factorization, the positive divisors of $n$
are exactly the numbers

$$d = p_1^{f_1}p_2^{f_2}\cdots p_k^{f_k}, \qquad 0 \leq f_i \leq e_i$$

and distinct exponent vectors $(f_1, \dots, f_k)$ give distinct divisors. So $D(n)$ is in
bijection with the set of such vectors.

**Step 2 — count them.** Each $f_i$ is chosen independently from $\{0, 1, \dots, e_i\}$, a
set of size $e_i + 1$. By the multiplication principle,

$$\tau(n) = \prod_{i=1}^{k}(e_i + 1)$$

**Step 3 — sum them.** Summing $d$ over all exponent vectors, and using distributivity to
factor the sum:

$$\sigma(n) = \sum_{f_1=0}^{e_1}\cdots\sum_{f_k=0}^{e_k} \prod_{i=1}^{k} p_i^{f_i}
= \prod_{i=1}^{k}\left(\sum_{f=0}^{e_i} p_i^{\,f}\right)$$

Expanding the right-hand product term by term reproduces exactly one summand for each
exponent vector, which is the identity used. Each inner sum is a geometric series with
ratio $p_i \neq 1$, so by GP-7:

$$\sum_{f=0}^{e_i} p_i^{\,f} = \frac{p_i^{\,e_i+1}-1}{p_i - 1}$$

$\blacksquare$

**Check.** $36 = 2^2 3^2$: $\ \tau = 3 \cdot 3 = 9$ ✓ and
$\sigma = \frac{2^3-1}{1}\cdot\frac{3^3-1}{2} = 7 \cdot 13 = 91$ ✓ (matching the explicit
list $1+2+3+4+6+9+12+18+36 = 91$).

---

## Part IV — Counting and complexity

### Theorem 13 (Harmonic bound — the sieve is $\Theta(n\log n)$)

> Let $H_n = \sum_{d=1}^{n} \frac{1}{d}$. Then for $n \geq 1$:
> $$\frac{\lfloor \log_2 n\rfloor}{2} \ \leq\ H_n \ \leq\ \lfloor \log_2 n \rfloor + 1$$
> Consequently the divisor sieve of README §6.4 performs
> $$\sum_{d=1}^{n}\left\lfloor \frac{n}{d}\right\rfloor = \Theta(n\log n)$$
> operations.

**Why this needs proving.** The sieve looks like a nested double loop over $n$ values, so
"obviously $O(n^2)$" — and that is wrong. The inner loop runs $n/d$ times, not $n$, and the
sum of those is far smaller than it looks.

**Proof of the bounds on $H_n$ (dyadic blocking).** Group the terms of $H_n$ into blocks by
powers of two: block $j$ consists of the indices $d$ with $2^j \leq d < 2^{j+1}$. Block $j$
has $2^j$ terms (when it fits entirely below $n$).

*Upper bound.* Within block $j$ every term satisfies $\frac{1}{d} \leq \frac{1}{2^j}$, so

$$\sum_{d = 2^j}^{2^{j+1}-1}\frac{1}{d} \ \leq\ 2^j \cdot \frac{1}{2^j} = 1$$

The indices $1, \dots, n$ are covered by blocks $j = 0, 1, \dots, \lfloor \log_2 n\rfloor$,
that is $\lfloor \log_2 n \rfloor + 1$ blocks, each contributing at most $1$. Hence
$H_n \leq \lfloor\log_2 n\rfloor + 1$.

*Lower bound.* Within block $j$ every term satisfies
$\frac{1}{d} > \frac{1}{2^{j+1}}$, so

$$\sum_{d=2^j}^{2^{j+1}-1}\frac{1}{d} \ \geq\ 2^j \cdot \frac{1}{2^{j+1}} = \frac{1}{2}$$

Blocks $j = 0, \dots, \lfloor\log_2 n\rfloor - 1$ lie entirely within $1, \dots, n$, and
there are $\lfloor\log_2 n\rfloor$ of them, each contributing at least $\frac12$. Hence
$H_n \geq \frac{\lfloor\log_2 n\rfloor}{2}$. $\blacksquare$

**Key step.** Blocks double in length while their terms halve in size, so each block
contributes a constant. The number of blocks is logarithmic — that is the entire reason
$H_n$ grows like $\log n$.

**Proof of the sieve bound.** For the upper bound, drop the floor:

$$\sum_{d=1}^{n}\left\lfloor\frac{n}{d}\right\rfloor \ \leq\ \sum_{d=1}^{n}\frac{n}{d} = nH_n = O(n\log n)$$

For the lower bound, use $\lfloor x \rfloor > x - 1$:

$$\sum_{d=1}^{n}\left\lfloor\frac{n}{d}\right\rfloor \ >\ \sum_{d=1}^{n}\left(\frac{n}{d} - 1\right) = nH_n - n = \Omega(n\log n)$$

since $nH_n$ grows strictly faster than $n$. $\blacksquare$

**Corollary 13.1.** The average number of divisors of an integer in $[1, n]$ is
$\frac{1}{n}\sum_{m\leq n}\tau(m) = \frac{1}{n}\sum_{d\leq n}\lfloor n/d\rfloor \approx \ln n$.
Divisors are *rare* on average, even though individual numbers can have many.

---

## Part V — Divisibility tests

### Theorem 14 (Decimal divisibility tests — **D10**)

> Write $n = \sum_{k=0}^{m} d_k 10^k$ in decimal, with digits $d_k$. Let
> $S(n) = \sum_k d_k$ (digit sum) and $A(n) = \sum_k (-1)^k d_k$ (alternating digit sum,
> starting with $+$ at the units place). Then:
> **(a)** $9 \mid n \iff 9 \mid S(n)$, and $3 \mid n \iff 3 \mid S(n)$;
> **(b)** $11 \mid n \iff 11 \mid A(n)$;
> **(c)** $2^j \mid n \iff 2^j$ divides the number formed by the last $j$ digits, and the
> same for $5^j$.

**Depends on:** GP-5, $(a - b) \mid (a^t - b^t)$, from the progressions concept. Notably
**no modular arithmetic is required** — these are consequences of divisibility alone.

**Proof of (a).** First, for every $k \geq 0$,

$$9 = 10 - 1 \ \mid\ 10^k - 1^k = 10^k - 1$$

directly by GP-5 with $a = 10$, $b = 1$. (Concretely, $10^k - 1 = \underbrace{99\cdots9}_{k}$.)

Now subtract the digit sum from $n$ term by term:

$$n - S(n) = \sum_{k=0}^{m} d_k 10^k - \sum_{k=0}^{m} d_k = \sum_{k=0}^{m} d_k\bigl(10^k - 1\bigr)$$

Every summand is a multiple of $9$, so by Corollary 3.1 (linearity over many terms),

$$9 \mid \bigl(n - S(n)\bigr)$$

Finally, both directions follow from linearity (Theorem 3):

- If $9 \mid n$, then since $9 \mid (n - S(n))$ we get $9 \mid \bigl(n - (n - S(n))\bigr) = S(n)$.
- If $9 \mid S(n)$, then $9 \mid \bigl(S(n) + (n - S(n))\bigr) = n$.

For $3$: since $3 \mid 9$ and $9 \mid (10^k - 1)$, transitivity (Theorem 2) gives
$3 \mid (10^k - 1)$, and the identical argument runs through. $\blacksquare$

**Proof of (b).** Apply GP-5 with $a = 10$ and $b = -1$:

$$10 - (-1) = 11 \ \mid\ 10^k - (-1)^k$$

Then

$$n - A(n) = \sum_{k=0}^{m} d_k 10^k - \sum_{k=0}^{m}(-1)^k d_k = \sum_{k=0}^{m} d_k\bigl(10^k - (-1)^k\bigr)$$

Each summand is a multiple of $11$, so $11 \mid (n - A(n))$ by Corollary 3.1, and the same
two-line linearity argument as in (a) gives $11 \mid n \iff 11 \mid A(n)$. $\blacksquare$

**Proof of (c).** Split $n$ at the $j$-th digit: write $n = L + 10^j H$, where $L$ is the
number formed by the last $j$ digits ($0 \leq L < 10^j$) and $H$ is the rest. Since
$10^j = 2^j 5^j$, we have $2^j \mid 10^j$, hence $2^j \mid 10^j H$. Then by linearity:

- If $2^j \mid n$, then $2^j \mid (n - 10^jH) = L$.
- If $2^j \mid L$, then $2^j \mid (L + 10^jH) = n$.

The argument for $5^j$ is identical. $\blacksquare$

**Key step, in all three parts.** The same two moves: show $d$ divides each
$\bigl(10^k - c_k\bigr)$ for the chosen weights $c_k$, then use linearity to transfer
divisibility between $n$ and $\sum d_k c_k$. Choosing $c_k = 1$ gives the digit sum;
$c_k = (-1)^k$ gives the alternating sum. Any $c$ with $d \mid (10 - c)$ would work — that
is the general recipe, and it explains why $7$ has no clean test in base $10$.

---

### Theorem 15 (What divisibility does **not** give — D11)

> $d \mid ab$ does **not** imply $d \mid a$ or $d \mid b$.

**Counterexample.** $d = 6$, $a = 4$, $b = 9$:

$$6 \mid 36 = 4 \cdot 9, \qquad\text{but}\qquad 6 \nmid 4 \ \text{ and } \ 6 \nmid 9$$

**Why it fails.** $6 = 2 \cdot 3$, and the product $4 \cdot 9 = 2^2 \cdot 3^2$ supplies the
$2$ from one factor and the $3$ from the other. The divisor is *split across* the two
factors, so neither alone contains it. A divisor that cannot be split this way — a
**prime** — does satisfy the implication.

**The correct statement.** If $p$ is prime and $p \mid ab$, then $p \mid a$ or $p \mid b$.
That is **Euclid's lemma**, and it is *not* provable from divisibility alone: it requires
Bézout's identity. See
[gcd-and-euclidean-algorithm/proofs.md](../gcd-and-euclidean-algorithm/proofs.md),
Theorem 10.

**Why the boundary matters.** Euclid's lemma is precisely what makes prime factorization
*unique*. The gap illustrated here is the reason unique factorization is a theorem
requiring real work, rather than an obvious property of the integers. $\blacksquare$

---

## Summary of dependencies

| Theorem | Proves | Depends on |
|---|---|---|
| 1 (basic facts) | $a\mid a$, $1\mid a$, $a\mid 0$, $0\mid b \Rightarrow b=0$ | definition only |
| 2 (transitivity) | **D4** | definition only |
| 3 (**linearity**) | **D1** | definition only |
| 4 (size bound) | **D2** | discreteness of $\mathbb{Z}$ |
| 5 (antisymmetry) | **D6** | Theorems 1, 4 |
| 6 (scale / cancel) | **D7** | definition, cancellation in $\mathbb{Z}$ |
| 7 (**division algorithm**) | **D3** | well-ordering |
| 8 (zero remainder) | $b \mid a \iff r = 0$ | Theorem 7 (uniqueness) |
| 9 (divisor pairing) | **D8** | definition, ordering of $\mathbb{R}$ |
| 10 ($\tau$ parity) | **D9** | Theorem 9, parity counting |
| 11 (trial division) | $\sqrt n$ bound | ordering the factors |
| 12 ($\tau$, $\sigma$ formulas) | closed forms | unique factorization, GP-7 |
| 13 (harmonic bound) | sieve is $\Theta(n\log n)$ | dyadic blocking |
| 14 (divisibility tests) | **D10** | GP-5, Corollary 3.1 |
| 15 (non-implication) | D11 | counterexample |
