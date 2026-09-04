# Proofs — Modular Multiplicative Inverse

**Borrowed** from [gcd](../gcd-and-euclidean-algorithm/proofs.md): **Bézout** (the values
$ax+my$ are exactly the multiples of $\gcd(a,m)$) and **Euclid's lemma**.

---

## 1. The inverse exists exactly when $\gcd(a,m)=1$

**Claim.** $ax \equiv 1 \pmod m$ is solvable $\iff \gcd(a,m)=1$.

**Proof.** $ax \equiv 1$ means $m \mid (ax-1)$, i.e. $ax - 1 = mk$, i.e.

$$a\,x + m\,(-k) = 1$$

So the question is whether $1$ is of the form $ax+my$. By **Bézout** those values are
exactly the multiples of $\gcd(a,m)$, so $1$ is among them iff $\gcd(a,m) \mid 1$, i.e.
$\gcd(a,m)=1$. $\blacksquare$

**Justifies** `if (g != 1) return -1;`. It also explains the two facts you use daily: mod a
**prime** every nonzero value is invertible (nothing below a prime shares a factor with
it), while mod a composite most are not — $2$ has no inverse mod $6$.

---

## 2. It is unique

**Claim.** If $ax \equiv 1$ and $ay \equiv 1$ then $x \equiv y \pmod m$.

**Proof.** $x \equiv x(ay) \equiv (xa)y \equiv y$. $\blacksquare$

**Justifies** normalizing into $[0,m)$ and calling it *the* inverse — everyone computing it
gets the same value.

---

## 3. Fermat, and why $a^{-1} = a^{\,p-2}$

**Claim.** For prime $p$ with $p \nmid a$: $\ a^{\,p-1} \equiv 1 \pmod p$.

**Proof.** Consider $a\cdot1,\ a\cdot2,\ \dots,\ a\cdot(p-1) \bmod p$.

*None is $0$:* $p \mid ai$ would force $p \mid a$ or $p \mid i$ by **Euclid's lemma**, both
excluded.

*All are distinct:* $ai \equiv aj$ gives $p \mid a(i-j)$, so $p \mid (i-j)$ by Euclid's
lemma, and $|i-j| < p$ forces $i=j$.

So the list is a **permutation** of $\{1,\dots,p-1\}$. Multiplying everything:

$$a^{\,p-1}\,(p-1)! \;\equiv\; (p-1)! \pmod p$$

and $(p-1)!$ is coprime to $p$, hence invertible by §1 — cancel it. $\blacksquare$

Splitting off one factor gives $a\cdot a^{\,p-2} \equiv 1$, so $a^{-1} \equiv a^{\,p-2}$.

**Why primality is not optional.** Both steps use Euclid's lemma. On a composite modulus
the list is not a permutation (it can contain zeros and repeats), the argument collapses,
and `powMod(a, m-2, m)` returns a meaningless number **with no error** — mod $12$ it gives
$5^{-1}=1$, and $5\times1 = 5 \neq 1$.

---

## 4. The $O(n)$ inverse table

**Claim.** For prime $p$ and $2 \le i < p$:
$\ i^{-1} \equiv -\lfloor p/i\rfloor \cdot (p \bmod i)^{-1} \pmod p$.

**Proof.** Write $p = qi + r$ with $q = \lfloor p/i\rfloor$, $r = p \bmod i$. Modulo $p$ the
left side vanishes:

$$q\,i + r \equiv 0 \pmod p$$

Both $i$ and $r$ are nonzero mod $p$, hence invertible by §1. Multiply by $i^{-1}r^{-1}$:

$$q\,r^{-1} + i^{-1} \equiv 0 \qquad\Longrightarrow\qquad i^{-1} \equiv -\,q\,r^{-1}$$

and $r < i$, so $r^{-1}$ was computed earlier. $\blacksquare$

**In code**, write $-q$ as $p-q$ to stay non-negative:
`inv[i] = (p - p / i) * inv[p % i] % p;` — $O(n)$ total, the standard setup for factorial
tables. Requires $p$ prime and $n < p$.

---

## 5. Solving $ax \equiv b \pmod m$ in general

**Claim.** With $g = \gcd(a,m)$: solvable $\iff g \mid b$, and there are then **exactly
$g$** solutions mod $m$, spaced $\frac mg$ apart.

**Proof.** The congruence is $ax + m(-y) = b$, solvable by Bézout iff $g \mid b$. All
solutions have the form $x = x_0 + k\frac mg$; reducing mod $m$, the values
$k = 0,\dots,g-1$ give distinct residues while $k=g$ shifts by $m$ and returns to $x_0$.
$\blacksquare$

**How to solve it.** Divide through by $g$:

$$\frac ag\,x \equiv \frac bg \pmod{\frac mg}$$

Now $\gcd\!\left(\frac ag,\frac mg\right) = 1$, so $\frac ag$ is invertible and one solution
drops out; add $\frac mg$ repeatedly for the rest.

**Justifies** returning a *list*, not a single value.
