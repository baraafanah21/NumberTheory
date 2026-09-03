# Proofs — Extended Euclidean Algorithm

**Borrowed:** **linearity** ([divisibility](../divisibility/proofs.md)), **Bézout** and
**Euclid's lemma** ([gcd](../gcd-and-euclidean-algorithm/proofs.md)).

---

## 1. The recursion is correct

**Claim.** `extgcd(a,b)` returns $g = \gcd(a,b)$ with $ax+by = g$.

**Proof (induction on depth).** Base $b=0$: it returns $g=a$ with $(x,y)=(1,0)$, and
$a\cdot1 + 0\cdot0 = a = \gcd(a,0)$ ✓

Step: the recursive call on $(b,\ a \bmod b)$ gives

$$b\,x_1 + (a \bmod b)\,y_1 = g$$

Substitute $a \bmod b = a - \lfloor \frac ab \rfloor b$ and collect the $a$ and $b$ terms:

$$a\,y_1 \;+\; b\Bigl(x_1 - \Bigl\lfloor\tfrac ab\Bigr\rfloor y_1\Bigr) \;=\; g$$

Comparing with $ax+by=g$ reads off $x = y_1$ and $y = x_1 - \lfloor a/b\rfloor y_1$ —
exactly what the code assigns. The returned $g$ is right because the underlying recursion
is ordinary Euclid. $\blacksquare$

**On overflow.** The coefficients stay bounded (roughly $|x| \le \frac{b}{2g}$ and
$|y| \le \frac{a}{2g}$), so nothing overflows *inside* `extgcd`. The risk is whatever you
do with the result afterwards.

---

## 2. All solutions of $ax+by=c$

**Claim.** With $g = \gcd(a,b)$: solvable $\iff g \mid c$; scaling a Bézout pair by $c/g$
gives one solution; and **every** solution is

$$x = x_0 + k\cdot\frac bg, \qquad y = y_0 - k\cdot\frac ag$$

**Solvability.** ($\Rightarrow$) $g$ divides both $a$ and $b$, so by linearity it divides
$ax+by=c$. ($\Leftarrow$) Scale a Bézout pair by $c/g$. $\blacksquare$

**The family is complete** — this is the part that tells you the step size. If $(x,y)$ and
$(x',y')$ both solve it, subtracting gives

$$a(x-x') = -\,b(y-y')$$

Divide by $g$, writing $a = ga'$ and $b = gb'$ with $\gcd(a',b')=1$:

$$a'(x-x') = -\,b'(y-y')$$

So $b' \mid a'(x-x')$, and since $\gcd(a',b')=1$, **Euclid's lemma** forces
$b' \mid (x-x')$. Write $x-x' = k\,b' = k\frac bg$; substituting back gives
$y-y' = -k\frac ag$. So any two solutions differ by exactly one step of the family, and
nothing finer exists. $\blacksquare$

**Justifies three bugs avoided:** the `c % g != 0` early exit; scaling by `c / g` (solving
$=g$ is not solving $=c$); and stepping by `b / g`, not `b`.

---

**Where the third use went.** Combining congruences is built on §2 but is a topic of its
own — see [chinese-remainder-theorem](../chinese-remainder-theorem/proofs.md).
