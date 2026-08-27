# Proofs — Extended Euclidean Algorithm

Three results. Each one: the claim, what it means, the proof, and why it matters in code.

**Borrowed from earlier concepts:**

- **Linearity** ([divisibility](../divisibility/proofs.md)) — $d \mid a$ and $d \mid b$
  imply $d \mid (ax+by)$.
- **Bézout** ([gcd](../gcd-and-euclidean-algorithm/proofs.md)) — such $x, y$ exist, and
  $ax+by$ ranges over exactly the multiples of $\gcd(a,b)$.
- **Euclid's lemma** (same file) — $\gcd(a,b)=1$ and $a \mid bc$ imply $a \mid c$.

---

## 1. Why the recursion is correct

**Claim.** `extgcd(a, b)` returns $g = \gcd(a,b)$ together with $x, y$ satisfying
$ax + by = g$.

**In words.** The plain Euclidean algorithm throws the coefficients away. This version
carries them back up the recursion, adjusting them at each return.

**Proof by induction on the recursion depth.**

**Base case $b = 0$.** The code returns $g = a$ with $(x,y) = (1,0)$, and indeed

$$a\cdot 1 + 0\cdot 0 = a = \gcd(a, 0) \quad\checkmark$$

**Inductive step.** Assume the recursive call on $(b,\ a \bmod b)$ is correct, so it hands
back $g$ and $x_1, y_1$ with

$$b\,x_1 + (a \bmod b)\,y_1 = g$$

Substitute $a \bmod b = a - \bigl\lfloor \frac ab \bigr\rfloor b$:

$$b\,x_1 + \Bigl(a - \Bigl\lfloor\tfrac ab\Bigr\rfloor b\Bigr)y_1 = g$$

Now collect the $a$ terms and the $b$ terms separately:

$$a\,y_1 \;+\; b\Bigl(x_1 - \Bigl\lfloor\tfrac ab\Bigr\rfloor y_1\Bigr) \;=\; g$$

Comparing with $ax + by = g$ gives

$$x = y_1, \qquad y = x_1 - \Bigl\lfloor\frac{a}{b}\Bigr\rfloor\,y_1$$

which is exactly what the code assigns. The returned $g$ is correct because the underlying
recursion is the ordinary Euclidean algorithm. $\blacksquare$

**In code.** If you forget the two lines, redo this substitution instead of guessing.

**On overflow.** The coefficients stay bounded (roughly $|x| \le \frac{b}{2g}$ and
$|y| \le \frac{a}{2g}$), so if $a$ and $b$ fit in the type, so do $x$ and $y$ — nothing
overflows *inside* `extgcd`. The risky multiplication is whatever you do with the result
afterwards.

---

## 2. All solutions of $ax + by = c$

**Claim.** Let $g = \gcd(a,b)$.

1. A solution exists $\iff g \mid c$.
2. If $(x_0, y_0)$ solves $ax+by=g$, then $\bigl(\frac cg x_0,\ \frac cg y_0\bigr)$ solves
   $ax+by=c$.
3. **Every** solution has the form

$$x = x_0 + k\cdot\frac{b}{g}, \qquad y = y_0 - k\cdot\frac{a}{g}, \qquad k \in \mathbb{Z}$$

**Proof of 1.** ($\Rightarrow$) $g$ divides both $a$ and $b$, so by linearity it divides
$ax + by = c$. ($\Leftarrow$) If $g \mid c$, scale a Bézout pair by $c/g$.

**Proof of 2.** Multiply $ax_0 + by_0 = g$ through by $\frac cg$.

**Proof of 3 — the family is complete.** This is the part worth reading, because it is what
tells you the step size.

Suppose $(x,y)$ and $(x',y')$ both solve $ax + by = c$. Subtracting the two equations:

$$a\,(x - x') = -\,b\,(y - y')$$

Divide by $g$, writing $a = g\,a'$ and $b = g\,b'$ — where $\gcd(a',b') = 1$, because
dividing out the gcd always leaves coprime numbers:

$$a'\,(x-x') = -\,b'\,(y-y')$$

So $b'$ divides $a'(x-x')$. Since $\gcd(a',b') = 1$, **Euclid's lemma** forces $b'$ into
the other factor:

$$b' \mid (x - x')$$

Write $x - x' = k\,b' = k\frac bg$. Substituting back gives $y - y' = -k\frac ag$. So any
two solutions differ by exactly one step of the stated family, and nothing finer exists.
$\blacksquare$

**In code.** Three separate bugs are prevented here:

- the `if (c % g != 0)` early exit;
- scaling by `c / g` — solving $= g$ is *not* solving $= c$;
- stepping by `b / g`, not `b`. With $g > 1$, stepping by $b$ silently skips $g-1$ out of
  every $g$ solutions, which breaks "find the smallest non-negative $x$".

---

## 3. When two congruences can be merged (CRT)

**Claim.** The system

$$x \equiv a_1 \pmod{m_1}, \qquad x \equiv a_2 \pmod{m_2}$$

has a solution $\iff\ g = \gcd(m_1,m_2)$ divides $(a_2 - a_1)$. The solution is then unique
modulo $\mathrm{lcm}(m_1, m_2)$.

**In words.** The two congruences must agree on the information they share. If they
disagree there, no number can satisfy both.

**Proof.** Every solution of the first congruence looks like

$$x = a_1 + m_1\,t$$

for some integer $t$. Substituting into the second:

$$a_1 + m_1 t \equiv a_2 \pmod{m_2} \qquad\Longleftrightarrow\qquad m_1\,t \equiv a_2 - a_1 \pmod{m_2}$$

That is the equation $m_1 t + m_2 s = a_2 - a_1$, so by §2 it is solvable exactly when
$g = \gcd(m_1,m_2)$ divides $a_2 - a_1$ — which is the stated condition.

When it holds, §2 also tells us $t$ is determined modulo $\frac{m_2}{g}$. Therefore
$x = a_1 + m_1 t$ is determined modulo

$$m_1 \cdot \frac{m_2}{g} = \mathrm{lcm}(m_1, m_2) \qquad \blacksquare$$

**A concrete failure.** $x \equiv 1 \pmod 6$ and $x \equiv 2 \pmod 8$. Here $g = 2$ and
$a_2 - a_1 = 1$, which $2$ does not divide. And indeed the first congruence forces $x$
odd while the second forces $x$ even. **No solution** — skipping this check is a real bug,
not a theoretical one.

**In code.**

- the `if ((a2 - a1) % g != 0) return NO_SOLUTION;` guard;
- the merged modulus is $\mathrm{lcm}$, not the product — using the product is wrong
  whenever the moduli share a factor;
- fold many congruences by merging pairwise. Watch for overflow: $\mathrm{lcm}$ grows fast,
  and ten moduli near $10^9$ will exceed 64 bits.
