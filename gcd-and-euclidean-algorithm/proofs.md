# Proofs — GCD and the Euclidean Algorithm

Five results. Each one: the claim, what it means, the proof, and why it matters in code.

**Borrowed from [divisibility](../divisibility/proofs.md):**

- **Linearity** — if $d \mid a$ and $d \mid b$ then $d \mid (ax + by)$.
- **Division algorithm** — for $b \neq 0$ there are unique $q, r$ with $a = qb + r$,
  $0 \le r < |b|$.

Everything below is built from those two.

---

## 1. Why the Euclidean step is valid

**Claim.** For any integer $q$, the pairs $(a,\ b)$ and $(a - qb,\ b)$ have **exactly the
same common divisors**. In particular

$$\gcd(a,\ b) = \gcd(b,\ a \bmod b)$$

**In words.** Subtracting a multiple of $b$ from $a$ does not change *which* numbers divide
both. Not just the largest one — the whole collection. That is why the step can be repeated
forever without losing information.

**Proof.** Two directions, each one application of linearity.

**($\Rightarrow$)** Suppose $d \mid a$ and $d \mid b$. Using linearity with coefficients
$x = 1$, $y = -q$:

$$d \mid \bigl(a\cdot 1 + b\cdot(-q)\bigr) = a - qb$$

and $d \mid b$ was given. So $d$ divides both of the new pair.

**($\Leftarrow$)** Suppose $d \mid (a - qb)$ and $d \mid b$. Using linearity with
coefficients $1$ and $q$ on the *new* pair:

$$d \mid \bigl((a - qb)\cdot 1 + b\cdot q\bigr) = a$$

So $d$ divides both of the original pair.

The two sets of common divisors contain each other, hence are equal — and so are their
largest elements. $\blacksquare$

**Getting to the modulo form.** By the division algorithm, choosing
$q = \lfloor a/b\rfloor$ makes $a - qb = a \bmod b$.

**In code.** This is the line `a = b; b = a % b;`. Note the step is *reversible*
($a - qb \mapsto (a-qb) + qb$), which is precisely why nothing is lost.

---

## 2. Why it terminates, and why it is fast

**Claim.** The algorithm always stops, after at most $2\log_2\min(a,b)$ divisions.

**Termination.** By the division algorithm every remainder satisfies $0 \le r < |b|$. So
the second argument strictly decreases at each step and can never go below $0$. A strictly
decreasing sequence of non-negative integers must reach $0$. $\blacksquare$

**Speed — two steps at least halve the value.** Let $r_k$ be the sequence of remainders.
The claim is $r_{k+2} < \frac{r_k}{2}$. Two cases:

**Case 1: $r_{k+1} \le \frac{r_k}{2}$** (the value already dropped a lot). Then since
remainders keep decreasing,

$$r_{k+2} < r_{k+1} \le \frac{r_k}{2}$$

**Case 2: $r_{k+1} > \frac{r_k}{2}$** (the value barely dropped). Then $r_{k+1}$ is more
than half of $r_k$ but less than $r_k$, so $r_k$ divided by $r_{k+1}$ has quotient exactly
$1$. The modulo becomes a plain subtraction:

$$r_{k+2} = r_k - r_{k+1} < r_k - \frac{r_k}{2} = \frac{r_k}{2}$$

Either way the value halves every two steps, so after about $2\log_2$ steps it reaches
$0$. $\blacksquare$

**The idea in one line.** A *small* drop this step forces a *large* drop next step — the
algorithm can never be lazy twice in a row.

**In code.** Under 90 iterations for any 64-bit input. The slowest inputs are consecutive
Fibonacci numbers (there every quotient is $1$, so no step ever removes more than one
copy) — use those to stress-test.

---

## 3. Bézout's identity

**Claim.** There exist integers $x, y$ with

$$a\,x + b\,y = \gcd(a,b)$$

and moreover the set of *all* values $ax + by$ is exactly the set of multiples of
$\gcd(a,b)$.

**In words.** The gcd is not only a divisor of $a$ and $b$ — it is **reachable** by adding
and subtracting copies of them. Nothing smaller and positive is reachable.

**Proof.** Let $g$ be the **smallest positive** number of the form $ax + by$. (Such numbers
exist: $a\cdot a + b\cdot b = a^2+b^2 > 0$ is one, and any non-empty set of positive
integers has a least element.) Write $g = ax_0 + by_0$.

**Step 1: $g$ divides $a$.** Divide $a$ by $g$: $a = qg + r$ with $0 \le r < g$. Then

$$r = a - qg = a - q(ax_0 + by_0) = a\,(1 - qx_0) + b\,(-qy_0)$$

so $r$ is *also* of the form $ax + by$. If $r > 0$, it would be a positive value of that
form **smaller than $g$** — contradicting that $g$ is the smallest. So $r = 0$, meaning
$g \mid a$. The same argument gives $g \mid b$.

**Step 2: $g$ is the greatest such divisor.** Let $c$ be any common divisor of $a$ and $b$.
By linearity, $c$ divides $ax_0 + by_0 = g$. So $c \le g$. Combined with Step 1,
$g = \gcd(a,b)$.

**Step 3: the set.** Every $ax + by$ is a multiple of $g$ (linearity), and every multiple
$kg$ equals $a(kx_0) + b(ky_0)$. So the set is exactly the multiples of $g$. $\blacksquare$

**The key move.** The remainder of $a$ divided by $g$ is *again* of the form $ax + by$, and
minimality forces it to be zero.

**In code.**

- Steps of $\pm a$ and $\pm b$ reach **exactly** the multiples of $\gcd(a,b)$ — jug
  problems, frog jumps, coin problems.
- $ax + by = c$ has a solution **if and only if** $\gcd(a,b) \mid c$.

> This proof shows $x$ and $y$ **exist** without computing them. Computing them is the
> [extended Euclidean algorithm](../extended-euclidean-algorithm/).

---

## 4. Euclid's lemma

**Claim.** If $\gcd(a,b) = 1$ and $a \mid bc$, then $a \mid c$. In particular, for a prime
$p$:

$$p \mid b\,c \qquad\Longrightarrow\qquad p \mid b \ \text{ or } \ p \mid c$$

**In words.** A number sharing no factor with $b$ can only "get at" the product $bc$
through $c$.

**Proof.** Since $\gcd(a,b) = 1$, Bézout gives integers $x, y$ with

$$a\,x + b\,y = 1$$

Multiply both sides by $c$:

$$a\,c\,x + b\,c\,y = c$$

Now look at the left side. Certainly $a \mid acx$. And we assumed $a \mid bc$, so
$a \mid bcy$. By linearity $a$ divides their sum — which is $c$. $\blacksquare$

**For the prime version.** Suppose $p \nmid b$. The only positive divisors of $p$ are $1$
and $p$, so $\gcd(p, b)$ is $1$ or $p$; it cannot be $p$ (that would give $p \mid b$). So
$\gcd(p,b) = 1$ and the first part applies, yielding $p \mid c$. $\blacksquare$

**Why this matters.** The divisibility concept showed $6 \mid 4\times 9$ while $6$ divides
neither factor — because $6 = 2\times3$ splits across them. A **prime** cannot split, and
this proof is exactly why. It is also the reason prime factorization is unique, which every
factorization-based algorithm silently relies on.

---

## 5. Why the lcm formula is exact

**Claim.**

$$\gcd(a,b)\cdot\mathrm{lcm}(a,b) = |a\,b| \qquad\Longrightarrow\qquad \mathrm{lcm}(a,b) = \frac{|ab|}{\gcd(a,b)}$$

**Proof.** Let $g = \gcd(a,b)$, and write $a = g\,a'$, $b = g\,b'$ where
$\gcd(a', b') = 1$. Put $L = \frac{|ab|}{g}$.

**$L$ is a common multiple.** Since $g \mid b$, the number $\frac{|b|}{g}$ is an integer,
and $L = |a| \cdot \frac{|b|}{g}$, so $a \mid L$. Symmetrically $b \mid L$.

**$L$ divides every common multiple.** Let $M$ be any common multiple, say $M = as = bt$.
Substituting $a = ga'$, $b = gb'$ and cancelling $g$:

$$a'\,s = b'\,t$$

So $b' \mid a's$. Since $\gcd(a',b') = 1$, **Euclid's lemma** gives $b' \mid s$. Write
$s = b'k$; then

$$M = a\,s = a\,b'\,k = \frac{ab}{g}\,k$$

so $L$ divides $M$.

Being a common multiple that divides all common multiples, $L$ is the least one.
$\blacksquare$

**In code.** The formula justifies `(a / g) * b`: dividing before multiplying is not a
hack, it is **exact**, because $g$ divides $a$ by definition. And `a * b / g` overflows on
inputs whose lcm fits comfortably.
