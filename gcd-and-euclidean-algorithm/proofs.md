# Proofs — GCD and the Euclidean Algorithm

Every theorem here corresponds to a property stated in [README.md](README.md).

**Notation.** $a, b, c, d, q, r$ denote integers. $g = \gcd(a,b)$, always taken
$\geq 0$, with the convention $\gcd(0,0) = 0$. $F_n$ is the Fibonacci sequence
$F_1 = F_2 = 1$, $F_{n+1} = F_n + F_{n-1}$.

**Prerequisites (from the divisibility concept, assumed without proof).**

- **D1 (linearity)** — $d \mid a$ and $d \mid b$ $\implies$ $d \mid (ax + by)$ for all
  $x, y \in \mathbb{Z}$.
- **D2 (size bound)** — $d \mid a$ and $a \neq 0$ $\implies$ $\lvert d \rvert \leq \lvert a \rvert$.
- **D3 (division algorithm)** — for $b \neq 0$ there are unique $q, r$ with $a = qb + r$
  and $0 \leq r < \lvert b \rvert$.
- **D4 (transitivity)** — $c \mid b$ and $b \mid a$ $\implies$ $c \mid a$.

**Other dependencies.** Theorem 13 uses unique factorization; Theorem 14 uses GP-5 from
[arithmetic-and-geometric-progressions](../arithmetic-and-geometric-progressions/proofs.md).
Everything else is self-contained given D1–D4 and the well-ordering principle.

**Logical order.** Theorems 1–5 build and analyse the algorithm using only D1–D3.
Theorem 6 (Bézout) is the pivot: Theorems 7–12 are all its consequences. Reading the
file in order matters more here than in the previous concept.

---

## Part I — The algorithm

### Theorem 1 (The gcd exists)

> For integers $a, b$ not both zero, the set of common divisors is finite and nonempty, so
> it has a greatest element, and that element satisfies $\gcd(a,b) \geq 1$.

**Why this needs saying.** "The largest thing with a property" is only meaningful once you
know such things exist and do not go on forever. For $a = b = 0$ they *do* go on forever,
which is exactly why that case needs a convention rather than a proof.

**Proof.** Let $C = \{\, d \in \mathbb{Z} : d \mid a \text{ and } d \mid b \,\}$.

*Nonempty:* $1 \mid a$ and $1 \mid b$, so $1 \in C$, and hence any greatest element is
$\geq 1$.

*Bounded:* at least one of $a, b$ is nonzero — say $a \neq 0$. Every $d \in C$ divides $a$,
so by **D2**, $\lvert d \rvert \leq \lvert a \rvert$. Thus
$C \subseteq [-\lvert a \rvert, \lvert a \rvert]$, a finite set.

A nonempty finite set of integers has a greatest element. $\blacksquare$

**The excluded case.** If $a = b = 0$ then $C = \mathbb{Z}$: every integer divides $0$.
There is no greatest element, so $\gcd(0,0)$ is fixed *by convention* at $0$. Theorem 6
shows this is the only choice consistent with the divisibility characterization.

---

### Theorem 2 (The Euclidean step — G3)

> For any $q \in \mathbb{Z}$, the pairs $(a, b)$ and $(a - qb,\ b)$ have **exactly the same
> set of common divisors**. Consequently $\gcd(a,b) = \gcd(a - qb,\ b)$, and taking
> $q = \lfloor a/b \rfloor$ gives
> $$\gcd(a,b) = \gcd(b,\ a \bmod b) \qquad (b \neq 0)$$

**What it says, and why the stronger form matters.** It would be enough for the algorithm
to know the two *maxima* agree. But proving the two *sets* agree is no harder and is much
more useful: it means the step throws away no information at all, which is what lets us
iterate it indefinitely and is what Theorem 3 needs.

**Proof.** We show mutual inclusion of the two sets of common divisors.

$(\subseteq)$ Suppose $d \mid a$ and $d \mid b$. By **D1** with the coefficients $x = 1$,
$y = -q$:

$$d \mid \bigl(a \cdot 1 + b \cdot (-q)\bigr) = a - qb$$

and $d \mid b$ by assumption. So $d$ is a common divisor of $(a - qb,\ b)$.

$(\supseteq)$ Suppose $d \mid (a - qb)$ and $d \mid b$. By **D1** with $x = 1$, $y = q$
applied to the pair $(a - qb,\ b)$:

$$d \mid \bigl((a - qb) \cdot 1 + b \cdot q\bigr) = a$$

and $d \mid b$. So $d$ is a common divisor of $(a, b)$.

The two sets are equal, hence so are their greatest elements. $\blacksquare$

**Key step.** Both directions are one application of D1. The symmetry is the point: the
transformation $a \mapsto a - qb$ is *invertible* ($a - qb \mapsto (a-qb) + qb$), and
linearity respects it in both directions.

**Specializing.** By **D3**, writing $a = qb + r$ with $q = \lfloor a/b \rfloor$ gives
$r = a \bmod b = a - qb$. Combined with symmetry ($\gcd(u,v) = \gcd(v,u)$, immediate since
"common divisor" is symmetric), this is the recurrence the algorithm runs on.

---

### Theorem 3 (The Euclidean algorithm is correct and terminates)

> Define $r_0 = \lvert a \rvert$, $r_1 = \lvert b \rvert$, and $r_{k+1} = r_{k-1} \bmod r_k$
> while $r_k \neq 0$. Then the sequence reaches $0$ after finitely many steps, and if $r_n$
> is the last nonzero term, $r_n = \gcd(a,b)$.

**Two separate claims.** Termination and correctness are independent; an algorithm can
compute the right answer *if* it stops and still never stop. Both are proved.

**Proof of termination.** By **D3**, whenever $r_k \neq 0$ we have
$0 \leq r_{k+1} < r_k$. So $r_1 > r_2 > r_3 > \cdots \geq 0$ is a strictly decreasing
sequence of non-negative integers. By the well-ordering principle no such sequence is
infinite: if the process never produced $0$, the set $\{r_1, r_2, \dots\}$ would be a
nonempty set of non-negative integers with no least element, a contradiction. Hence some
$r_{n+1} = 0$. $\blacksquare$

**Proof of correctness.** Apply Theorem 2 at each step:

$$\gcd(r_0, r_1) = \gcd(r_1, r_2) = \gcd(r_2, r_3) = \cdots = \gcd(r_n, r_{n+1}) = \gcd(r_n, 0)$$

Now $\gcd(r_n, 0) = r_n$: every integer divides $0$, so the common divisors of $r_n$ and
$0$ are exactly the divisors of $r_n$, the greatest of which is $\lvert r_n \rvert = r_n$
(the $r_k$ are non-negative by construction). Finally
$\gcd(r_0, r_1) = \gcd(\lvert a \rvert, \lvert b \rvert) = \gcd(a,b)$, since $d \mid a$ iff
$d \mid -a$. $\blacksquare$

**Note on signs.** The absolute values at initialization are what make the result
non-negative. In C++ the `%` operator can return a negative remainder for negative
operands; the loop still terminates and still computes the right value up to sign, which is
why the implementation normalizes with `std::abs` on entry.

---

### Theorem 4 (Two steps at least halve — the $O(\log)$ bound)

> For the remainder sequence of Theorem 3, whenever $r_{k+2}$ is defined,
> $$r_{k+2} < \frac{r_k}{2}$$
> Consequently the algorithm performs at most $2\log_2 \min(\lvert a \rvert, \lvert b \rvert) + O(1)$
> division steps.

**Intuition.** A single step need not shrink much — $\gcd(100, 99)$ goes to
$\gcd(99, 1)$, a huge drop, but $\gcd(100, 51)$ goes to $\gcd(51, 49)$, barely a dent.
The claim is that a *small* drop this step forces a *large* drop next step, so no two
consecutive steps can both be lazy.

**Proof (two cases on how big $r_{k+1}$ is).** Recall $0 \leq r_{k+2} < r_{k+1} < r_k$.

*Case 1: $r_{k+1} \leq \frac{r_k}{2}$.* Then immediately

$$r_{k+2} < r_{k+1} \leq \frac{r_k}{2}$$

*Case 2: $r_{k+1} > \frac{r_k}{2}$.* Since also $r_{k+1} < r_k$, we have
$1 \leq \frac{r_k}{r_{k+1}} < 2$, so the quotient is exactly
$\lfloor r_k / r_{k+1} \rfloor = 1$. Therefore

$$r_{k+2} = r_k \bmod r_{k+1} = r_k - r_{k+1} < r_k - \frac{r_k}{2} = \frac{r_k}{2}$$

In both cases $r_{k+2} < r_k/2$. $\blacksquare$

**Key step.** Case 2 is where the work is: being *close* to $r_k$ forces the quotient to
be $1$, which turns the modulo into a plain subtraction and gives an explicit bound.

**The complexity bound.** Applying the claim repeatedly, $r_{2j} < r_0 / 2^j$. The values
are non-negative integers, so once $r_0/2^j \leq 1$ the sequence has hit $0$. That happens
by $j = \log_2 r_0$, i.e. after at most $2\log_2 r_0$ steps. The same argument from $r_1$
gives the bound in terms of $\min$. Hence $O(\log \min(a,b))$ divisions. $\blacksquare$

---

### Theorem 5 (Lamé — the worst case is Fibonacci)

> If the Euclidean algorithm applied to $a > b \geq 1$ performs exactly $n$ division steps,
> then $a \geq F_{n+2}$ and $b \geq F_{n+1}$. The bound is tight: the pair
> $(F_{n+2}, F_{n+1})$ takes exactly $n$ steps.

**What it says.** Consecutive Fibonacci numbers are the *smallest* inputs requiring $n$
steps — equivalently, they are the worst case for their size. Since
$F_n \sim \varphi^n/\sqrt5$, this sharpens Theorem 4's constant from $2\log_2$ to
$\log_\varphi \approx 1.44\log_2$.

**Proof (strong induction on $n$).**

*Base $n = 1$.* One step means $a \bmod b = 0$, i.e. $b \mid a$ with $a > b \geq 1$. Then
$b \geq 1 = F_2$, and $a$ is a multiple of $b$ strictly larger than $b$, so
$a \geq 2b \geq 2 = F_3$. ✓

*Inductive step.* Let $n \geq 2$ and suppose the claim holds for $n - 1$. Running the
algorithm on $(a,b)$ takes $n$ steps; the first step produces $r = a \bmod b$, and the
remaining computation is the algorithm on $(b, r)$ taking $n - 1$ steps. Since $n \geq 2$,
that remaining computation is nonempty, so $r \geq 1$ and $b > r \geq 1$. By the induction
hypothesis applied to $(b, r)$:

$$b \geq F_{n+1}, \qquad r \geq F_n$$

Now $a = qb + r$ with $q \geq 1$ (because $a > b$), so

$$a = qb + r \geq b + r \geq F_{n+1} + F_n = F_{n+2}$$

which is the claim at $n$. $\blacksquare$

**Key step.** $q \geq 1$ and $a \geq b + r$. The worst case is exactly when every quotient
equals $1$ — no step ever removes more than one copy — and "$a = b + r$ at every stage" is
precisely the Fibonacci recurrence.

**Tightness.** Running the algorithm on $(F_{n+2}, F_{n+1})$ gives
$F_{n+2} = 1 \cdot F_{n+1} + F_n$ at every stage, so each step reduces the index by one and
the total is exactly $n$ steps. $\blacksquare$

---

## Part II — Bézout's identity and its consequences

### Theorem 6 (Bézout's identity, and the universal property — G6)

> Let $a, b$ be integers, not both zero, and let
> $$S = \{\, ax + by : x, y \in \mathbb{Z} \,\} \cap \mathbb{Z}_{>0}$$
> be the set of **positive** integer combinations of $a$ and $b$. Then $S$ has a least
> element $g$, and:
> **(a)** $g = \gcd(a,b)$ — so there exist $x_0, y_0$ with $ax_0 + by_0 = \gcd(a,b)$;
> **(b)** every common divisor $c$ of $a$ and $b$ satisfies $c \mid g$.

**Why this is the pivot of the whole concept.** Part (a) says the gcd is not merely a
divisor — it is *reachable* by addition and subtraction of $a$ and $b$. Part (b) upgrades
"greatest" to "divisible by every other", which is the property every later proof uses.
Note the proof is **non-constructive**: it shows $x_0, y_0$ exist without producing them.
Producing them is the extended Euclidean algorithm.

**Proof that $S$ has a least element.** $S \subseteq \mathbb{Z}_{>0}$, and $S$ is nonempty:
taking $x = a$, $y = b$ gives $a^2 + b^2 > 0$ since $a, b$ are not both zero. By the
well-ordering principle $S$ has a least element $g = ax_0 + by_0 > 0$.

**Proof of (a) — $g$ is a common divisor.** We show $g \mid a$; the argument for $b$ is
identical.

By **D3**, write $a = qg + r$ with $0 \leq r < g$. Then

$$r = a - qg = a - q(ax_0 + by_0) = a(1 - qx_0) + b(-qy_0)$$

so $r$ is itself an integer combination of $a$ and $b$. Now suppose $r > 0$. Then
$r \in S$ and $r < g$, contradicting that $g$ is the *least* element of $S$. Therefore
$r = 0$, i.e. $g \mid a$. Likewise $g \mid b$, so $g$ is a common divisor.

**Proof of (b), and that $g$ is the greatest.** Let $c$ be any common divisor of $a$ and
$b$. By **D1** applied to $g = ax_0 + by_0$,

$$c \mid g$$

which is (b). Since $g > 0$, **D2** gives $\lvert c \rvert \leq g$. So $g$ is a common
divisor that is $\geq$ every common divisor: $g = \gcd(a,b)$, proving (a). $\blacksquare$

**Key step.** The remainder $r$ of $a$ divided by $g$ is *again a combination of $a$ and
$b$*. Minimality of $g$ then forces $r = 0$. This "the least element of a closed set
divides everything in it" argument recurs throughout algebra.

**The $\gcd(0,0)$ convention, justified.** Under the characterization "$g$ divides both and
is divisible by every common divisor", $a = b = 0$ gives $g = 0$: indeed $0 \mid 0$, and
every $c$ (all of $\mathbb{Z}$ are common divisors) satisfies $c \mid 0$. So $0$ is the
unique non-negative answer. $\blacksquare$

---

### Theorem 7 (The combinations are exactly the multiples of the gcd)

> $$\{\, ax + by : x, y \in \mathbb{Z} \,\} = \{\, kg : k \in \mathbb{Z} \,\} = g\mathbb{Z}$$

**What it says.** Adding and subtracting copies of $a$ and $b$ reaches *precisely* the
multiples of $\gcd(a,b)$ — no more, no less. This is the theorem behind every "can I reach
exactly $c$ with steps of $a$ and $b$" problem.

**Proof.** $(\subseteq)$ $g \mid a$ and $g \mid b$, so by **D1** $g \mid (ax + by)$ for all
$x, y$. Every combination is a multiple of $g$.

$(\supseteq)$ By Theorem 6 there are $x_0, y_0$ with $ax_0 + by_0 = g$. Multiplying by any
$k \in \mathbb{Z}$:

$$kg = a(kx_0) + b(ky_0)$$

which is a combination. Every multiple of $g$ is reached.

(If $a = b = 0$ both sides equal $\{0\}$, consistent with $g = 0$.) $\blacksquare$

**Corollary 7.1 (solvability).** The equation $ax + by = c$ has an integer solution **iff**
$\gcd(a,b) \mid c$.

---

### Theorem 8 (Distributivity — G4)

> For any $c \neq 0$: $\ \gcd(ca, cb) = \lvert c \rvert \gcd(a,b)$.

**Proof.** Using Theorem 7 on the pair $(ca, cb)$, its set of combinations is

$$\{\, cax + cby \,\} = c \cdot \{\, ax + by \,\} = c \cdot (g\mathbb{Z}) = (cg)\mathbb{Z} = \bigl(\lvert c \rvert g\bigr)\mathbb{Z}$$

(the last equality because $n\mathbb{Z} = (-n)\mathbb{Z}$). But Theorem 7 applied directly
to $(ca, cb)$ says this same set equals $\gcd(ca,cb)\,\mathbb{Z}$. Two sets
$m\mathbb{Z} = n\mathbb{Z}$ with $m, n \geq 0$ force $m = n$ (each divides the other and
both are non-negative). Hence $\gcd(ca,cb) = \lvert c \rvert g$. $\blacksquare$

**Key step.** Working with the whole *set* of combinations rather than with individual
divisors. Scaling a set of combinations by $c$ is transparent; scaling a "greatest"
directly is not.

---

### Theorem 9 (Reduction to coprimality — G5)

> If $g = \gcd(a,b) \neq 0$, then $\ \gcd\!\left(\dfrac{a}{g}, \dfrac{b}{g}\right) = 1$.

**Proof.** Both $a/g$ and $b/g$ are integers since $g$ divides both. Let
$h = \gcd(a/g,\ b/g)$. Apply Theorem 8 with $c = g > 0$:

$$g = \gcd(a, b) = \gcd\!\left(g \cdot \frac{a}{g},\ g \cdot \frac{b}{g}\right) = g \cdot h$$

Dividing by $g \neq 0$ gives $h = 1$. $\blacksquare$

**Why it matters.** This is exactly "a fraction reduced by its gcd is in lowest terms", and
it is the standard normalization step before applying any result that requires coprime
inputs.

---

### Theorem 10 (Euclid's lemma — G7)

> **(a)** If $\gcd(a,b) = 1$ and $a \mid bc$, then $a \mid c$.
> **(b)** If $p$ is prime and $p \mid bc$, then $p \mid b$ or $p \mid c$.

**What it says.** A number coprime to $b$ can only get at $bc$ through $c$. Part (b) is the
property that makes prime factorization unique — it is *not* the definition of a prime
(that is "no divisors besides $1$ and itself"), and the gap between the two notions is
bridged precisely by Bézout.

**Proof of (a).** By Theorem 6 there are $x, y$ with

$$ax + by = 1$$

Multiply both sides by $c$:

$$acx + bcy = c$$

Now examine the left side. $a \mid acx$ trivially. And $a \mid bc$ by hypothesis, so
$a \mid bcy$. By **D1**, $a$ divides the sum:

$$a \mid (acx + bcy) = c$$

$\blacksquare$

**Key step.** Multiplying the Bézout relation by $c$. This turns "$1$ is a combination of
$a$ and $b$" into "$c$ is a combination of things $a$ already divides".

**Proof of (b).** Suppose $p \nmid b$. The positive divisors of the prime $p$ are $1$ and
$p$, so $\gcd(p, b) \in \{1, p\}$; it cannot be $p$, since $\gcd(p,b) \mid b$ would give
$p \mid b$. Hence $\gcd(p,b) = 1$, and part (a) with $a = p$ yields $p \mid c$.
$\blacksquare$

**Corollary 10.1 (induction to many factors).** If $p$ is prime and
$p \mid a_1a_2\cdots a_k$, then $p \mid a_i$ for some $i$. *Proof:* induction on $k$ using
(b) at each split. This is the step that unique factorization is built on.

---

### Theorem 11 (gcd–lcm duality — G8)

> For nonzero $a, b$: $\ \gcd(a,b) \cdot \mathrm{lcm}(a,b) = \lvert ab \rvert$.

**Proof.** Let $g = \gcd(a,b)$ and set $L = \dfrac{\lvert ab \rvert}{g}$. We show $L$ is
the least positive common multiple, in the strong sense that it divides every common
multiple.

*Step 1 — $L$ is a common multiple.* Since $g \mid b$, the value $\lvert b \rvert / g$ is
an integer, and

$$L = \lvert a \rvert \cdot \frac{\lvert b \rvert}{g}$$

so $a \mid L$. Symmetrically $L = \frac{\lvert a \rvert}{g}\lvert b \rvert$, so $b \mid L$.

*Step 2 — $L$ divides every common multiple.* Write $a = ga'$ and $b = gb'$; by Theorem 9,
$\gcd(a', b') = 1$. Let $M$ be any common multiple, say $M = as = bt$. Then

$$ga's = gb't \quad \Longrightarrow \quad a's = b't$$

So $b' \mid a's$. Since $\gcd(a',b') = 1$, Euclid's lemma (Theorem 10a) gives $b' \mid s$;
write $s = b'k$. Then

$$M = as = ab'k = a \cdot \frac{b}{g} \cdot k = \frac{ab}{g}\,k$$

so $\frac{ab}{g} \mid M$, and up to sign $L \mid M$.

By Step 1, $L$ is a positive common multiple; by Step 2 it divides all of them, so by **D2**
it is the smallest. Hence $\mathrm{lcm}(a,b) = L = \frac{\lvert ab \rvert}{g}$, which
rearranges to the claim. $\blacksquare$

**Key step.** Step 2 is where Euclid's lemma earns its place: coprimality of $a'$ and $b'$
is what forces $b'$ into $s$ rather than being spread across the product.

---

### Theorem 12 (Associativity — G9)

> $\gcd\bigl(\gcd(a,b), c\bigr) = \gcd\bigl(a, \gcd(b,c)\bigr)$, and both equal the greatest
> common divisor of the three numbers.

**Proof.** Let $d = \gcd(\gcd(a,b), c)$. We check $d$ satisfies the defining property of
$\gcd(a,b,c)$: it is a common divisor, and every common divisor divides it.

*Common divisor:* $d \mid \gcd(a,b)$ and $\gcd(a,b) \mid a$, so $d \mid a$ by **D4**;
likewise $d \mid b$. And $d \mid c$ directly.

*Universal:* let $e$ divide $a$, $b$ and $c$. Since $e$ is a common divisor of $a$ and $b$,
Theorem 6(b) gives $e \mid \gcd(a,b)$. So $e$ is a common divisor of $\gcd(a,b)$ and $c$,
and Theorem 6(b) again gives $e \mid d$.

The same argument applies verbatim to $\gcd(a, \gcd(b,c))$. Two non-negative integers each
dividing the other are equal, so the two groupings agree. $\blacksquare$

**Why this licenses the fold.** Associativity is exactly what makes
`g = gcd(g, x)` over a list well defined regardless of order, and it is why the array gcd
is a single left-to-right pass.

---

### Theorem 13 (Factorization form — G10)

> If $a = \prod_p p^{\alpha_p}$ and $b = \prod_p p^{\beta_p}$ over the primes (finitely many
> nonzero exponents), then
> $$\gcd(a,b) = \prod_p p^{\min(\alpha_p,\beta_p)}, \qquad \mathrm{lcm}(a,b) = \prod_p p^{\max(\alpha_p,\beta_p)}$$

**Depends on:** unique factorization (which itself depends on Theorem 10 — so this is a
consequence of Bézout, not an independent route to it. Deriving the gcd this way and then
"proving" Euclid's lemma from it would be circular).

**Proof.** Unique factorization gives: $d \mid a$ (for $d > 0$) iff $d = \prod_p p^{\delta_p}$
with $\delta_p \leq \alpha_p$ for every $p$. So $d$ is a common divisor of $a$ and $b$ iff

$$\delta_p \leq \alpha_p \ \text{ and } \ \delta_p \leq \beta_p \quad\text{for all } p
\qquad\Longleftrightarrow\qquad \delta_p \leq \min(\alpha_p, \beta_p)$$

Such a $d$ is largest when each $\delta_p$ is as large as possible, i.e.
$\delta_p = \min(\alpha_p, \beta_p)$. The lcm argument is identical with the inequalities
reversed. $\blacksquare$

**Corollary 13.1 (a second proof of Theorem 11).** For every $p$,

$$\min(\alpha_p,\beta_p) + \max(\alpha_p,\beta_p) = \alpha_p + \beta_p$$

since the two operations just sort the pair. Multiplying over all $p$ gives
$\gcd(a,b)\cdot\mathrm{lcm}(a,b) = ab$ for positive $a, b$. Shorter than Theorem 11 — but
it costs the entire fundamental theorem of arithmetic, whereas Theorem 11 needs only
Euclid's lemma.

---

### Theorem 14 (Exponent gcd — G11)

> For an integer $a \geq 2$ and integers $m, n \geq 0$:
> $$\gcd\bigl(a^m - 1,\ a^n - 1\bigr) = a^{\gcd(m,n)} - 1$$

**What it says.** A gcd of two astronomically large numbers reduces to a gcd of two tiny
exponents. The reason is that the Euclidean algorithm on $a^m - 1,\ a^n - 1$ *simulates*
the subtractive Euclidean algorithm on $m, n$ — one outer step per exponent subtraction.

**The bridge to the previous concept.** The identity below is GP-5 in disguise: it is the
statement that $a^n - 1$ divides $a^{kn} - 1$, made quantitative.

**Proof (strong induction on $m + n$).** By symmetry assume $m \geq n$.

*Base case $n = 0$.* Then $a^0 - 1 = 0$, and

$$\gcd(a^m - 1,\ 0) = a^m - 1 = a^{\gcd(m,0)} - 1$$

since $\gcd(m, 0) = m$. ✓

*Inductive step, $m \geq n \geq 1$.* The key algebraic identity is

$$a^m - 1 = a^{m-n}\bigl(a^n - 1\bigr) + \bigl(a^{m-n} - 1\bigr)$$

which is verified by expanding the right-hand side:
$a^{m} - a^{m-n} + a^{m-n} - 1 = a^m - 1$. ✓

This exhibits $a^{m-n} - 1$ as $\bigl(a^m - 1\bigr) - q\bigl(a^n - 1\bigr)$ with the
integer $q = a^{m-n}$. So Theorem 2 applies:

$$\gcd\bigl(a^m - 1,\ a^n - 1\bigr) = \gcd\bigl(a^{m-n} - 1,\ a^n - 1\bigr)$$

The pair of exponents has gone from $(m, n)$ to $(m - n,\ n)$, and
$(m-n) + n = m < m + n$, so the induction hypothesis applies:

$$\gcd\bigl(a^{m-n} - 1,\ a^n - 1\bigr) = a^{\gcd(m-n,\,n)} - 1$$

Finally, Theorem 2 *applied to the exponents themselves* gives
$\gcd(m-n,\ n) = \gcd(m,n)$. Substituting completes the step. $\blacksquare$

**Key step.** The same theorem (Theorem 2) is used twice at two different levels: once on
the giant numbers $a^m - 1$, once on the small exponents $m, n$. That parallel *is* the
content of the result.

**Check.** $a = 2$, $m = 6$, $n = 4$: $\gcd(63, 15) = 3$ and $2^{\gcd(6,4)} - 1 = 2^2 - 1 = 3$. ✓

**Corollary 14.1.** $a^m - 1$ and $a^n - 1$ are coprime iff $\gcd(m,n) = 1$. In particular
two Mersenne numbers $2^m - 1$, $2^n - 1$ share a factor exactly when their exponents do.

---

### Theorem 15 (Binary GCD identities)

> For $u, v > 0$:
> **(a)** if $u, v$ are both even, $\gcd(u,v) = 2\gcd\!\left(\frac{u}{2}, \frac{v}{2}\right)$;
> **(b)** if $u$ is even and $v$ is odd, $\gcd(u,v) = \gcd\!\left(\frac{u}{2}, v\right)$;
> **(c)** if $u, v$ are both odd with $u \geq v$, $\gcd(u,v) = \gcd(u - v,\ v)$.

**Proof of (a).** Immediate from Theorem 8 with $c = 2$. $\blacksquare$

**Proof of (b).** Let $g = \gcd(u,v)$.

*$g$ divides $u/2$:* $g \mid v$ and $v$ is odd, so $g$ is odd, hence $\gcd(g, 2) = 1$. Now
$g \mid u = 2 \cdot \frac{u}{2}$, so by Euclid's lemma (Theorem 10a, with $a = g$, $b = 2$,
$c = u/2$) we get $g \mid \frac{u}{2}$. Together with $g \mid v$, $g$ is a common divisor
of $\frac{u}{2}$ and $v$, so by Theorem 6(b), $g \mid \gcd\!\left(\frac{u}{2}, v\right)$.

*Conversely:* $\gcd\!\left(\frac{u}{2}, v\right)$ divides $\frac{u}{2}$, hence divides $u$
by **D4**, and divides $v$; so by Theorem 6(b) it divides $g$.

Each divides the other and both are non-negative, so they are equal. $\blacksquare$

**Proof of (c).** Theorem 2 with $q = 1$. $\blacksquare$

**Why the algorithm terminates.** Case (c) subtracts two odd numbers, producing an even
result, so case (b) fires on the next iteration and strictly reduces a value. Cases (a) and
(b) each remove at least one factor of $2$, i.e. at least one bit. Since the bit-length is
a non-negative integer that strictly decreases at least every other iteration, the process
terminates in $O(\log \max(u,v))$ iterations. $\blacksquare$

---

## Summary of dependencies

| Theorem | Depends on |
|---|---|
| 1 (existence) | D2, finiteness |
| 2 (Euclidean step) | **D1** only |
| 3 (correctness + termination) | Theorem 2, D3, well-ordering |
| 4 (halving bound) | D3, case analysis |
| 5 (Lamé) | Theorem 3, induction |
| 6 (**Bézout**) | D1, D2, D3, well-ordering |
| 7 (combinations $= g\mathbb{Z}$) | Theorem 6, D1 |
| 8 (distributivity) | Theorem 7 |
| 9 (coprime reduction) | Theorem 8 |
| 10 (**Euclid's lemma**) | Theorem 6, D1 |
| 11 (gcd·lcm) | Theorems 9, 10 |
| 12 (associativity) | Theorem 6(b), D4 |
| 13 (factorization form) | unique factorization (itself from Theorem 10) |
| 14 (exponent gcd) | Theorem 2, GP-5, induction |
| 15 (binary gcd) | Theorems 2, 6(b), 8, 10 |
