# Proofs — Chinese Remainder Theorem

**Borrowed:** **Bézout** ([gcd](../gcd-and-euclidean-algorithm/proofs.md)); **linear
congruences** and **modular inverse**
([modular inverse](../modular-multiplicative-inverse/proofs.md)) — $ax \equiv b \pmod m$ is
solvable iff $\gcd(a,m) \mid b$, and $x$ is then determined modulo
$\frac{m}{\gcd(a,m)}$.

---

## 1. Merging two congruences

**Claim.** $x \equiv a_1 \ (m_1)$ and $x \equiv a_2 \ (m_2)$ have a common solution
$\iff g = \gcd(m_1,m_2)$ divides $a_2-a_1$; the solution is then unique modulo
$\mathrm{lcm}(m_1,m_2)$.

**Proof.** Every solution of the first is $x = a_1 + m_1 t$. Substituting into the second:

$$m_1\,t \equiv a_2 - a_1 \pmod{m_2}$$

a linear congruence in $t$, solvable exactly when $g \mid (a_2-a_1)$ — the stated
condition. When solvable, $t$ is determined modulo $\frac{m_2}{g}$, so
$x = a_1 + m_1t$ is determined modulo

$$m_1\cdot\frac{m_2}{g} = \frac{m_1m_2}{\gcd(m_1,m_2)} = \mathrm{lcm}(m_1,m_2) \qquad\blacksquare$$

**Concrete failure.** $x \equiv 1 \ (6)$ and $x \equiv 2 \ (8)$: $g = 2$ does not divide
$a_2-a_1 = 1$. Directly, the first forces $x$ odd and the second forces $x$ even.

**Justifies** the `diff % g != 0` guard, and that the merged modulus is $\mathrm{lcm}$, not
the product. Solving for $t$ is where the inverse of $\frac{m_1}{g}$ mod $\frac{m_2}{g}$
appears — it exists because dividing out the gcd leaves coprime numbers.

---

## 2. The coprime case is a bijection

**Claim.** If $\gcd(m_1,m_2)=1$ the system is always solvable, uniquely mod $m_1m_2$.
Equivalently

$$\Phi(x) = (x \bmod m_1,\ x \bmod m_2)$$

is a **bijection** $\mathbb{Z}/m_1m_2 \to \mathbb{Z}/m_1 \times \mathbb{Z}/m_2$.

**Proof.** §1 with $g=1$: the condition $1 \mid (a_2-a_1)$ is automatic, and the modulus is
$\mathrm{lcm} = m_1m_2$. Solvability makes $\Phi$ surjective; uniqueness makes it
injective. $\blacksquare$

**Already used.** The proof that Euler's totient is multiplicative
([totient](../euler-totient-function/proofs.md) §2) *is* this bijection, restricted to the
residues coprime to the modulus.

---

## 3. Folding many congruences

**Claim.** Merging two at a time is valid.

**Proof.** §1 shows more than "a solution exists" — it shows the solution set of the pair
**equals** the solution set of one congruence:

$$\{x : x \equiv a_1 (m_1),\ x \equiv a_2 (m_2)\} = \{x : x \equiv r \ (\mathrm{lcm}(m_1,m_2))\}$$

(left-to-right is uniqueness, right-to-left because $r$ satisfies both). So replacing the
pair loses nothing, and induction on $k$ finishes it. If any merge fails, that pair is
contradictory and so is the whole system. $\blacksquare$

**In code**, start the fold from $x \equiv 0 \pmod 1$ — the identity, which removes the
empty-list case.

**Overflow.** The modulus grows to the $\mathrm{lcm}$ of everything; ten moduli near
$10^9$ exceed 64 bits.

---

## 4. The direct formula

**Claim.** For **pairwise** coprime $m_i$, with $M = \prod m_i$, $M_i = M/m_i$ and
$y_i = M_i^{-1} \bmod m_i$, the value $x = \sum_i a_iM_iy_i$ solves the system.

**Proof.** *$y_i$ exists:* $M_i$ is the product of the other moduli, each coprime to $m_i$
by pairwise coprimality, so $\gcd(M_i, m_i) = 1$.

*It works:* reduce modulo $m_j$. For $i \neq j$ the modulus $m_j$ is a factor of
$M_i = M/m_i$, so that term vanishes, leaving

$$x \equiv a_jM_jy_j \equiv a_j\cdot 1 = a_j \pmod{m_j} \qquad\blacksquare$$

**Why *pairwise* is essential.** The first step fails otherwise: with $(6,10,15)$ we get
$M_1 = 150$ and $\gcd(150,6) = 6 \neq 1$, so no $y_1$ exists — even though
$\gcd(6,10,15)=1$.

---

## 5. Splitting a computation is valid

**Claim.** If $v$ is built from integers using only $+,-,\times$, computing the expression
independently modulo each pairwise-coprime $m_i$ and reconstructing gives $v \bmod M$ —
hence $v$ exactly when $0 \le v < M$.

**Proof.** By §2 and §3, $\Phi(x) = (x \bmod m_1,\dots,x \bmod m_k)$ is a bijection onto
the product. Reduction respects the operations:

$$(a+b) \bmod m_i = \bigl((a \bmod m_i)+(b \bmod m_i)\bigr) \bmod m_i$$

and likewise for $\times$, so $\Phi$ carries each operation to the coordinate-wise one. By
induction over the expression, evaluating coordinate-wise yields $\Phi(v)$; injectivity
means reconstruction recovers $v \bmod M$. $\blacksquare$

**Justifies** exact big-number arithmetic in machine words: choose primes whose product
exceeds the answer, compute modulo each, reconstruct.

**The condition that bites.** You must bound $v$ *in advance*. If $v \ge M$ the
reconstruction returns $v \bmod M$ — silently wrong rather than detectably wrong.
