# Proofs — GCD and the Euclidean Algorithm

**Borrowed** from [divisibility](../divisibility/proofs.md): **linearity**
($d\mid a$, $d\mid b$ $\Rightarrow$ $d \mid ax+by$) and the **division algorithm**.

---

## 1. The Euclidean step is valid

**Claim.** $(a,b)$ and $(a-qb,\ b)$ have **the same set of common divisors**; hence
$\gcd(a,b) = \gcd(b,\ a \bmod b)$.

**Proof.** If $d \mid a$ and $d \mid b$ then $d \mid (a-qb)$ by linearity. Conversely if
$d \mid (a-qb)$ and $d \mid b$ then $d \mid \bigl((a-qb)+qb\bigr) = a$. $\blacksquare$

The step loses **no** information — not merely "the maximum is preserved" — which is why it
can be iterated. Taking $q = \lfloor a/b\rfloor$ makes $a-qb = a \bmod b$.

---

## 2. It terminates, in $O(\log)$

**Terminates.** Remainders satisfy $0 \le r < |b|$, so the second argument strictly
decreases and is bounded below. $\blacksquare$

**Speed: two steps at least halve the value.** Let $r_k$ be the remainder sequence.

- If $r_{k+1} \le \frac{r_k}{2}$, then $r_{k+2} < r_{k+1} \le \frac{r_k}{2}$.
- If $r_{k+1} > \frac{r_k}{2}$, the quotient is exactly $1$, so
  $r_{k+2} = r_k - r_{k+1} < \frac{r_k}{2}$.

Either way $r_{k+2} < r_k/2$, giving at most $2\log_2\min(a,b)$ steps. $\blacksquare$

**The idea:** a small drop now forces a large drop next — the algorithm cannot be lazy
twice running. The worst inputs are consecutive Fibonacci numbers (every quotient is $1$);
use those to stress-test.

---

## 3. Bézout

**Claim.** There are $x,y$ with $ax+by = \gcd(a,b)$, and

$$\{\,ax+by\,\} = \{\text{multiples of } \gcd(a,b)\}$$

**Proof.** Let $g$ be the **smallest positive** value of $ax+by$ (one exists:
$a^2+b^2 > 0$). Divide $a$ by $g$: $a = qg + r$ with $0 \le r < g$. Then

$$r = a - qg = a - q(ax_0+by_0) = a(1-qx_0) + b(-qy_0)$$

is *also* of the form $ax+by$. If $r>0$ it would be a positive such value below $g$ —
impossible. So $r=0$, i.e. $g \mid a$; likewise $g \mid b$. Any common divisor $c$ divides
$ax_0+by_0 = g$ by linearity, so $c \le g$. Hence $g = \gcd(a,b)$.

For the set: every $ax+by$ is a multiple of $g$ by linearity, and every $kg$ equals
$a(kx_0)+b(ky_0)$. $\blacksquare$

**The key move:** the remainder of $a$ divided by $g$ is *again* of the form $ax+by$, and
minimality forces it to zero.

**Justifies** reachability (steps of $\pm a,\pm b$ reach exactly the multiples of the gcd),
solvability of $ax+by=c$ iff $\gcd(a,b) \mid c$, and the fact every later proof needs:
every common divisor **divides** the gcd.

*Non-constructive — it does not produce $x,y$. That is
[extended Euclid](../extended-euclidean-algorithm/).*

---

## 4. Euclid's lemma

**Claim.** $\gcd(a,b)=1$ and $a \mid bc$ $\Rightarrow$ $a \mid c$. For prime $p$:
$p \mid bc$ $\Rightarrow$ $p\mid b$ or $p\mid c$.

**Proof.** Bézout gives $ax+by=1$. Multiply by $c$:

$$acx + bcy = c$$

Now $a \mid acx$ trivially, and $a \mid bc$ gives $a \mid bcy$; by linearity $a$ divides
their sum, which is $c$. $\blacksquare$

**Prime version.** If $p \nmid b$ then $\gcd(p,b)$ is $1$ or $p$, and it cannot be $p$ —
so it is $1$ and the first part applies. $\blacksquare$

**Why it matters.** Divisibility showed $6 \mid 4{\times}9$ with $6$ dividing neither,
because $6 = 2{\times}3$ **splits** across the factors. A prime cannot split. This is also
exactly what makes prime factorization unique.

---

## 5. The lcm formula is exact

**Claim.** $\gcd(a,b)\cdot\mathrm{lcm}(a,b) = |ab|$.

**Proof.** Let $g=\gcd(a,b)$, $a=ga'$, $b=gb'$ with $\gcd(a',b')=1$, and $L = |ab|/g$.

*$L$ is a common multiple:* $L = |a|\cdot\frac{|b|}{g}$ and $g \mid b$, so $a \mid L$;
symmetrically $b \mid L$.

*$L$ divides every common multiple:* if $M = as = bt$ then cancelling $g$ gives
$a's = b't$, so $b' \mid a's$; since $\gcd(a',b')=1$, **Euclid's lemma** gives $b' \mid s$,
and substituting shows $\frac{ab}{g} \mid M$. $\blacksquare$

**Justifies `(a / g) * b`** — dividing first is not a hack but exact, since $g \mid a$.
