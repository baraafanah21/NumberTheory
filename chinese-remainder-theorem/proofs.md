# Proofs — Chinese Remainder Theorem

Five results. Each one: the claim, what it means, the proof, and why it matters in code.

**Borrowed from earlier concepts:**

- **Bézout** ([gcd](../gcd-and-euclidean-algorithm/proofs.md)) — $ax+by$ ranges over
  exactly the multiples of $\gcd(a,b)$.
- **Linear congruences** ([modular inverse](../modular-multiplicative-inverse/proofs.md)) —
  $ax \equiv b \pmod m$ is solvable iff $\gcd(a,m) \mid b$, and $x$ is then determined
  modulo $\frac{m}{\gcd(a,m)}$.
- **Modular inverse** (same file) — exists iff $\gcd(a,m)=1$.

---

## 1. Merging two congruences

**Claim.** The system

$$x \equiv a_1 \pmod{m_1}, \qquad x \equiv a_2 \pmod{m_2}$$

has a solution **if and only if** $g = \gcd(m_1,m_2)$ divides $a_2 - a_1$. When it does,
the solution is unique modulo $\mathrm{lcm}(m_1,m_2)$.

**In words.** The two congruences overlap on the information they share — namely modulo
$g$. If they disagree there, nothing can satisfy both. If they agree, everything glues.

**Proof.** Every solution of the first congruence has the form

$$x = a_1 + m_1\,t, \qquad t \in \mathbb{Z}$$

Substituting into the second:

$$a_1 + m_1 t \equiv a_2 \pmod{m_2} \qquad\Longleftrightarrow\qquad m_1\,t \equiv a_2 - a_1 \pmod{m_2}$$

This is a linear congruence in $t$. By the borrowed result it is solvable exactly when

$$\gcd(m_1, m_2) \ \big|\ (a_2 - a_1)$$

which is the stated condition. And when solvable, $t$ is determined modulo
$\frac{m_2}{g}$.

**Uniqueness of $x$.** Since $t$ is pinned down modulo $\frac{m_2}{g}$, and
$x = a_1 + m_1 t$, the value $x$ is pinned down modulo

$$m_1 \cdot \frac{m_2}{g} \;=\; \frac{m_1 m_2}{\gcd(m_1,m_2)} \;=\; \mathrm{lcm}(m_1,m_2)$$

using the gcd–lcm identity. $\blacksquare$

**A concrete failure.** $x \equiv 1 \pmod 6$ and $x \equiv 2 \pmod 8$. Here $g = 2$ and
$a_2 - a_1 = 1$, which $2$ does not divide. Directly: the first forces $x$ odd, the second
forces $x$ even. **No solution.**

**In code.**

- the `if (diff % g != 0) return NO_SOLUTION;` guard is this condition;
- the merged modulus is $\mathrm{lcm}$, **not** the product — those differ whenever the
  moduli share a factor;
- solving for $t$ is where the modular inverse of $\frac{m_1}{g}$ modulo $\frac{m_2}{g}$
  appears; it exists because dividing out the gcd leaves coprime numbers.

---

## 2. The coprime case, and the CRT bijection

**Claim.** If $\gcd(m_1,m_2) = 1$ then the system is **always** solvable, uniquely modulo
$m_1m_2$. Equivalently, the map

$$\Phi:\ \mathbb{Z}/m_1m_2 \ \longrightarrow\ \mathbb{Z}/m_1 \times \mathbb{Z}/m_2,
\qquad \Phi(x) = (x \bmod m_1,\ x \bmod m_2)$$

is a **bijection**.

**In words.** With coprime moduli, a residue mod $m_1m_2$ and a *pair* of residues carry
exactly the same information. Nothing is lost or gained by switching between them.

**Proof.** Solvability is §1 with $g = 1$: the condition $1 \mid (a_2-a_1)$ is automatic.
Uniqueness is modulo $\mathrm{lcm}(m_1,m_2) = m_1m_2$, again because $g=1$.

For the bijection: solvability says $\Phi$ is **surjective** (every pair $(a_1,a_2)$ is hit),
and uniqueness says $\Phi$ is **injective** (two $x$ giving the same pair are congruent mod
$m_1m_2$). A map that is both is a bijection. Alternatively, both sides have exactly
$m_1m_2$ elements, so surjectivity alone forces bijectivity. $\blacksquare$

**Where you have already used this.** The proof that Euler's totient is multiplicative
([totient](../euler-totient-function/proofs.md) §2) is exactly this bijection, restricted to
the residues coprime to the modulus.

---

## 3. Folding many congruences

**Claim.** A system of $k$ congruences can be solved by merging them **two at a time**: the
result of merging the first two can be treated as a single congruence and merged with the
third, and so on.

**In words.** Pairwise merging is not a heuristic — the merged congruence has exactly the
same solution set as the two it replaced, so nothing is lost at any step.

**Proof (induction on $k$).** The key observation is that §1 does not merely produce *a*
solution: it shows

$$\{x : x \equiv a_1 \ (m_1) \text{ and } x \equiv a_2 \ (m_2)\}
\;=\; \{x : x \equiv r \ (\mathrm{lcm}(m_1,m_2))\}$$

for the merged residue $r$ — the solution set of the pair **is** the solution set of a
single congruence. (Left-to-right is uniqueness; right-to-left is because $r$ satisfies both
and the modulus is a multiple of each $m_i$.)

Now induct. For $k = 1$ there is nothing to do. For $k > 1$, merging the first two replaces
them by one congruence with the identical solution set, leaving a system of $k-1$
congruences; apply the induction hypothesis. If any merge fails, the pair is contradictory,
so the whole system is unsolvable. $\blacksquare$

**In code.** Start the fold from the trivial congruence $x \equiv 0 \pmod 1$ — every $x$
satisfies it, so it is the identity element and removes the empty-list special case.

**Overflow warning.** The modulus grows to $\mathrm{lcm}$ of everything. Ten moduli near
$10^9$ overflow 64 bits long before the tenth merge. Check the magnitude, or use
`__int128`.

---

## 4. The direct formula for pairwise coprime moduli

**Claim.** Let $m_1, \dots, m_k$ be **pairwise** coprime, $M = \prod_i m_i$, and
$M_i = M/m_i$. Then each $M_i$ is invertible modulo $m_i$, and with
$y_i = M_i^{-1} \bmod m_i$,

$$x \;=\; \sum_{i=1}^{k} a_i\,M_i\,y_i \pmod M$$

satisfies every congruence $x \equiv a_i \pmod{m_i}$.

**In words.** Build the answer out of $k$ "indicator" numbers: $M_iy_i$ is $1$ modulo
$m_i$ and $0$ modulo all the others. Then just scale each by $a_i$ and add.

**Proof.**

**Step 1 — $y_i$ exists.** $M_i$ is the product of all $m_j$ with $j \neq i$. Each such
$m_j$ is coprime to $m_i$ by **pairwise** coprimality, so their product is too:
$\gcd(M_i, m_i) = 1$. Hence $M_i$ is invertible mod $m_i$.

**Step 2 — check the $j$-th congruence.** Reduce the sum modulo $m_j$:

$$x \;=\; \sum_{i} a_i M_i y_i \;\equiv\; a_j M_j y_j \pmod{m_j}$$

because for every $i \neq j$ the modulus $m_j$ appears as a factor of $M_i = M/m_i$, so
$m_j \mid M_i$ and that whole term is $\equiv 0$.

**Step 3 — the surviving term is $a_j$.** By definition $M_j y_j \equiv 1 \pmod{m_j}$, so

$$x \equiv a_j \cdot 1 = a_j \pmod{m_j}$$

This holds for every $j$, so $x$ solves the system. $\blacksquare$

**Why *pairwise* is essential.** Step 1 breaks otherwise. With $m = (6,10,15)$ we have
$M_1 = 150$ and $\gcd(150, 6) = 6 \neq 1$, so no $y_1$ exists — even though
$\gcd(6,10,15) = 1$.

**In code.** Prefer the pairwise merge of §3: it handles non-coprime moduli, and it never
forms the full product $M$, which can overflow when the merged $\mathrm{lcm}$ would not.

---

## 5. Why you can split a computation across moduli

**Claim.** Let $m_1,\dots,m_k$ be pairwise coprime with $M = \prod m_i$. If a value $v$ is
computed from integers using only $+$, $-$ and $\times$, then computing the same expression
**independently modulo each $m_i$** and reconstructing by CRT gives $v \bmod M$ — and
therefore $v$ exactly, provided $0 \le v < M$.

**In words.** This is what makes CRT a *tool* rather than a puzzle: it converts one
computation in huge numbers into several computations in machine-sized ones.

**Proof.** By §2 (extended to $k$ moduli via §3), the map

$$\Phi(x) = (x \bmod m_1,\ \dots,\ x \bmod m_k)$$

is a bijection $\mathbb{Z}/M \to \prod_i \mathbb{Z}/m_i$. Reduction mod $m_i$ respects
addition and multiplication:

$$(a + b) \bmod m_i = \bigl((a \bmod m_i) + (b \bmod m_i)\bigr) \bmod m_i$$

and likewise for $\times$ — so $\Phi$ carries each operation to the coordinate-wise one.
By induction over the expression tree, the tuple obtained by evaluating the whole
expression coordinate-wise equals $\Phi(v)$. Since $\Phi$ is injective, CRT reconstruction
recovers $v \bmod M$ uniquely. If additionally $0 \le v < M$, that residue **is** $v$.
$\blacksquare$

**In code.** Pick enough primes that their product exceeds the largest possible answer, run
the computation modulo each one, then reconstruct. Exact big-number arithmetic without a
big-number library — and each modular run stays in 64-bit registers.

**The condition that bites.** You must bound $v$ *in advance* to choose enough moduli. If
$v \ge M$ the reconstruction returns $v \bmod M$, which is silently wrong rather than
detectably wrong.
