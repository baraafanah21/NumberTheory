# Proofs — Modular Multiplicative Inverse

Five results. Each one: the claim, what it means, the proof, and why it matters in code.

**Borrowed from earlier concepts:**

- **Bézout** ([gcd](../gcd-and-euclidean-algorithm/proofs.md)) — the values $ax + my$ are
  exactly the multiples of $\gcd(a,m)$.
- **Euclid's lemma** (same file) — if $p$ is prime and $p \mid xy$ then $p \mid x$ or
  $p \mid y$.

---

## 1. The inverse exists exactly when $\gcd(a,m) = 1$

**Claim.**

$$a\,x \equiv 1 \pmod m \ \text{ has a solution} \quad\Longleftrightarrow\quad \gcd(a,m) = 1$$

**In words.** You can divide by $a$ under a modulus precisely when $a$ shares no factor
with the modulus.

**Proof.** Unpack the congruence. $a x \equiv 1 \pmod m$ means $m$ divides $ax - 1$, so
$ax - 1 = mk$ for some integer $k$, which rearranges to

$$a\,x + m\,(-k) = 1$$

So the question becomes: *can $1$ be written as a combination of $a$ and $m$?*

By **Bézout**, the numbers of the form $ax + my$ are exactly the multiples of
$\gcd(a,m)$. So $1$ is among them precisely when

$$\gcd(a,m) \mid 1 \quad\Longleftrightarrow\quad \gcd(a,m) = 1 \qquad \blacksquare$$

**In code.** This is the `if (g != 1) return -1;` line. It also explains the two facts you
rely on daily:

- **Prime modulus $p$:** every $a \in \{1, \dots, p-1\}$ has an inverse, since nothing
  smaller than a prime shares a factor with it. That is why competitive problems use
  $10^9+7$.
- **Composite modulus:** most values have no inverse. Mod $6$, the multiples of $2$ run
  $2, 4, 0, 2, 4, 0, \dots$ — they never reach $1$.

---

## 2. The inverse is unique

**Claim.** If $ax \equiv 1$ and $ay \equiv 1 \pmod m$, then $x \equiv y \pmod m$.

**In words.** "The" inverse is well defined — there is only one answer in $[0, m)$.

**Proof.** Start from $x$ and slide the known relation in:

$$x \equiv x \cdot 1 \equiv x\,(a\,y) \equiv (x\,a)\,y \equiv 1 \cdot y \equiv y \pmod m$$

using associativity and both hypotheses. $\blacksquare$

**In code.** This is why `modInverse` may return whichever representative it likes, as long
as you normalize into $[0, m)$ — everyone else computing "the" inverse gets the same value.

---

## 3. Fermat's little theorem, and why $a^{-1} = a^{\,p-2}$

**Claim.** Let $p$ be prime and $p \nmid a$. Then

$$a^{\,p-1} \equiv 1 \pmod p \qquad\Longrightarrow\qquad a^{-1} \equiv a^{\,p-2} \pmod p$$

**In words.** Multiplying every nonzero residue by $a$ just **shuffles** them. Comparing
the product before and after the shuffle forces $a^{\,p-1}$ to be $1$.

**Proof.** Consider the list

$$a\cdot 1,\quad a\cdot 2,\quad \dots,\quad a\cdot(p-1) \pmod p$$

**Step 1 — none of them is $0$.** If $p \mid ai$ then, since $p$ is prime, Euclid's lemma
gives $p \mid a$ or $p \mid i$. Both are excluded ($p \nmid a$ by hypothesis, and
$1 \le i \le p-1$).

**Step 2 — they are all different.** Suppose $ai \equiv aj$. Then $p \mid a(i-j)$, and by
Euclid's lemma again $p \mid (i - j)$. But $|i - j| < p$, so $i = j$.

**Step 3 — so they are a rearrangement.** We have $p-1$ distinct nonzero residues drawn
from a set of exactly $p-1$ nonzero residues. So the list is a permutation of
$\{1, 2, \dots, p-1\}$.

**Step 4 — multiply everything.** The product of the list equals the product of
$\{1,\dots,p-1\}$, because they are the same numbers in a different order:

$$(a\cdot 1)(a\cdot 2)\cdots\bigl(a\cdot(p-1)\bigr) \;\equiv\; 1\cdot 2\cdots (p-1) \pmod p$$

The left side is $a^{\,p-1}\,(p-1)!$, so

$$a^{\,p-1}\,(p-1)! \;\equiv\; (p-1)! \pmod p$$

**Step 5 — cancel $(p-1)!$.** Each of $1, \dots, p-1$ is coprime to $p$, so $(p-1)!$ is
too, and by §1 it has an inverse. Multiplying both sides by it:

$$a^{\,p-1} \equiv 1 \pmod p \qquad \blacksquare$$

**Getting the inverse.** Split off one factor of $a$:

$$a \cdot a^{\,p-2} = a^{\,p-1} \equiv 1 \quad\Longrightarrow\quad a^{-1} \equiv a^{\,p-2}$$

**In code.** `powerMod(a, p - 2, p)` — one line, no extra function.

**Why the primality hypothesis is not optional.** Steps 1 and 2 both use Euclid's lemma,
which needs $p$ prime. On a composite modulus the list is *not* a permutation (it can
contain zeros and repeats), the whole argument collapses, and `powerMod(a, m-2, m)`
returns a meaningless number — with no error. Mod $12$ it gives $5^{-1} = 1$, and
$5 \times 1 = 5 \neq 1$.

---

## 4. The $O(n)$ inverse table

**Claim.** For a prime $p$ and $2 \le i < p$:

$$i^{-1} \;\equiv\; -\Bigl\lfloor \frac{p}{i} \Bigr\rfloor \cdot \bigl(p \bmod i\bigr)^{-1} \pmod p$$

**In words.** Divide $p$ by $i$ and look at the remainder. It is smaller than $i$, so its
inverse is already known — and it determines $i^{-1}$ directly.

**Proof.** Write $p = q\,i + r$ with $q = \lfloor p/i\rfloor$ and $r = p \bmod i$. Reducing
modulo $p$, the left side vanishes:

$$q\,i + r \equiv 0 \pmod p$$

Both $i$ and $r$ are nonzero mod $p$ (we have $1 \le i < p$, and $r \neq 0$ because
$i \nmid p$ for a prime $p$ with $1 < i < p$), so both are invertible by §1. Multiply the
whole congruence by $i^{-1}r^{-1}$:

$$q\,r^{-1} + i^{-1} \equiv 0 \pmod p \qquad\Longrightarrow\qquad i^{-1} \equiv -\,q\,r^{-1} \pmod p$$

Since $r < i$, the value $r^{-1}$ was computed on an earlier iteration. $\blacksquare$

**In code.** Keep it non-negative by writing $-q$ as $p - q$:

```cpp
inv[1] = 1;
for (int i = 2; i <= n; ++i)
    inv[i] = (p - p / i) * inv[p % i] % p;
```

$O(n)$ total. This is the standard setup for factorial tables used by $\binom{n}{k} \bmod p$.
It requires $p$ prime and $n < p$.

---

## 5. Solving $ax \equiv b \pmod m$ in general

**Claim.** Let $g = \gcd(a, m)$. Then

$$a\,x \equiv b \pmod m \ \text{ is solvable} \iff g \mid b$$

and when solvable there are **exactly $g$** solutions modulo $m$, spaced $\frac mg$ apart.

**In words.** If $a$ and $m$ share a factor you cannot invert $a$ — but the congruence may
still be solvable, and then it has several answers rather than one.

**Proof.** The congruence says $m \mid (ax - b)$, i.e. $ax - b = my$ for some $y$, i.e.

$$a\,x + m\,(-y) = b$$

By Bézout this is solvable exactly when $g \mid b$.

For the count: from the extended Euclid concept, all solutions of that equation have the
form $x = x_0 + k\cdot\frac{m}{g}$ for integer $k$. Reducing modulo $m$, the values
$k = 0, 1, \dots, g-1$ give distinct residues, and $k = g$ shifts by
$g \cdot \frac mg = m$, returning to $x_0$. So there are exactly $g$ distinct solutions
mod $m$. $\blacksquare$

**How to actually solve it.** Divide everything by $g$:

$$\frac ag\,x \equiv \frac bg \pmod{\frac mg}$$

Now $\gcd\!\left(\frac ag, \frac mg\right) = 1$, so $\frac ag$ *is* invertible and one
solution drops out. The other $g-1$ follow by adding $\frac mg$ repeatedly.

**In code.** Return a list, not a single value. Reporting one answer when there are $g$ is
the classic half-right solution.
