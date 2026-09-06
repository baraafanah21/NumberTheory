# Proofs — Primitive Roots and Discrete Logarithm

**Borrowed:** **Euler's theorem** and the identity $\sum_{d \mid n}\varphi(d) = n$
([totient](../euler-totient-function/proofs.md)); **Euclid's lemma**
([gcd](../gcd-and-euclidean-algorithm/proofs.md)); the **division algorithm**
([divisibility](../divisibility/proofs.md)).

---

## 1. The order exists and divides $\varphi(m)$

**Claim.** With $d = \mathrm{ord}_m(a)$:
$\ a^{\,k} \equiv 1 \iff d \mid k$, and in particular $d \mid \varphi(m)$.

**Proof.** $d$ exists because Euler gives $a^{\varphi(m)} \equiv 1$, so the set of valid
exponents is non-empty and has a least element.

($\Leftarrow$) $k = dt$ gives $a^k = (a^d)^t \equiv 1$.

($\Rightarrow$) Divide: $k = qd + r$ with $0 \le r < d$. Then

$$a^{\,r} = a^{\,k}\cdot\bigl(a^{\,d}\bigr)^{-q} \equiv 1$$

If $r>0$ this contradicts $d$ being **smallest**. So $r=0$. $\blacksquare$

Taking $k = \varphi(m)$ gives $d \mid \varphi(m)$.

**Justifies** never searching for the order. Start at $\varphi(m)$ — guaranteed to work —
and strip prime factors while it still works:

```cpp
ll ord = phi(m);
for (ll q : distinctPrimeFactors(ord))
    while (ord % q == 0 && powMod(a, ord / q, m) == 1) ord /= q;
```

---

## 2. The order of a power

**Claim.** If $\mathrm{ord}_m(a) = d$ then
$\ \mathrm{ord}_m(a^{\,k}) = \dfrac{d}{\gcd(d,k)}$.

**Proof.** Let $e = \gcd(d,k)$. By §1,

$$\bigl(a^{\,k}\bigr)^t \equiv 1 \iff d \mid kt \iff \frac de \ \Big|\ \frac ke\,t$$

and $\gcd\!\left(\frac de,\frac ke\right) = 1$, so **Euclid's lemma** forces
$\frac de \mid t$. The smallest such $t$ is $\frac de$. $\blacksquare$

**Corollary — how many primitive roots.** If $g$ is one, every coprime residue is $g^k$,
and $\mathrm{ord}(g^k) = \varphi(m)$ iff $\gcd(k,\varphi(m))=1$. So there are exactly
$\varphi(\varphi(m))$ of them — which is why searching $g=2,3,\dots$ terminates fast.

---

## 3. Every prime has a primitive root

**Claim.** For every prime $p$ some $g$ has $\mathrm{ord}_p(g) = p-1$.

**One lemma, cited.** *A degree-$d$ polynomial mod a prime $p$ has at most $d$ roots.* (True
because $\mathbb{Z}/p$ is a field. It **fails** for composite moduli — $x^2 \equiv 1 \pmod 8$
has four solutions, which is exactly why primes are special here.)

**Proof.** For each $d \mid p-1$ let $\psi(d)$ count the elements of order exactly $d$.

**Step 1.** Every element of $\{1,\dots,p-1\}$ has an order, dividing $p-1$ by §1, so

$$\sum_{d \mid p-1} \psi(d) = p-1$$

**Step 2: $\psi(d) \le \varphi(d)$.** If $\psi(d) = 0$ this is clear. Otherwise take $a$ of
order $d$. The powers $a^0,\dots,a^{d-1}$ are distinct (else $a^{j-i} \equiv 1$ with
$0 < j-i < d$) and all satisfy $x^d \equiv 1$. By the lemma that polynomial has at most $d$
roots — so these are **all** of them. Any element of order $d$ is therefore some $a^i$,
and by §2 that has order $d$ exactly when $\gcd(i,d)=1$ — which happens $\varphi(d)$ times.

**Step 3.** From the totient identity, $\sum_{d \mid p-1}\varphi(d) = p-1$ as well. So the
two sums are equal while Step 2 gives $\psi(d) \le \varphi(d)$ **termwise**. If any single
term were strictly smaller the left sum would be too. Hence

$$\psi(d) = \varphi(d) \qquad\text{for every } d \mid p-1$$

**Step 4.** Take $d = p-1$: $\ \psi(p-1) = \varphi(p-1) \ge 1$. $\blacksquare$

**The shape of the argument:** two equal sums, one termwise $\le$ the other, therefore equal
termwise. That squeeze needs the totient identity to supply the second sum.

**Why only special moduli.** The root-counting lemma fails for composite $m$, and with it
the argument. The full classification ($1,2,4,p^k,2p^k$) is **cited, not proved**.

---

## 4. The fast primitive-root test

**Claim.** For prime $p$: $\ \mathrm{ord}_p(g) = p-1 \iff g^{\frac{p-1}{q}} \not\equiv 1$
for every prime $q \mid (p-1)$.

**Proof.** ($\Rightarrow$) Each $\frac{p-1}{q} < p-1$, so no such exponent can give $1$.

($\Leftarrow$) Contrapositive. If $d = \mathrm{ord}_p(g) < p-1$ then $d \mid p-1$ by §1, so
$\frac{p-1}{d} > 1$ and has a prime factor $q$. Writing $\frac{p-1}{d} = qt$ gives
$\frac{p-1}{q} = dt$, so $d \mid \frac{p-1}{q}$ and by §1

$$g^{\frac{p-1}{q}} \equiv 1$$

i.e. the test fails for that $q$. $\blacksquare$

**Justifies** a handful of exponentiations instead of $O(p)$ work — $p-1$ has at most about
$15$ distinct prime factors below $2^{64}$. The cost is dominated by factoring $p-1$.

---

## 5. Baby-step giant-step is correct

**Claim.** With $n = \lceil\sqrt m\,\rceil$, every $x \in [1,m]$ is $x = in - j$ for some
$1 \le i \le n$, $0 \le j < n$; hence $g^x \equiv b$ iff
$\bigl(g^n\bigr)^i \equiv b\,g^{\,j}$ for such $i,j$.

**The representation.** Take $i = \lceil x/n \rceil$ and $j = in - x$. Then $i \ge 1$, and
$i \le \lceil m/n\rceil \le n$ since $n^2 \ge m$. By definition of the ceiling
$(i-1)n < x \le in$, so $0 \le in-x < n$. ✓

**The equivalence.** $\gcd(g,m)=1$ makes $g$ invertible, so multiplying both sides by
$g^{\,j}$ is reversible:

$$g^{\,in-j} \equiv b \iff g^{\,in} \equiv b\,g^{\,j} \iff \bigl(g^{\,n}\bigr)^i \equiv b\,g^{\,j}$$

$\blacksquare$

**Why it is fast.** Each side involves only one variable, so tabulate one and look up the
other — $O(\sqrt m)$ time and memory.

**Getting the *smallest* $x$.** Scan $i$ ascending, and when storing baby steps keep the
**largest** $j$ per key (later writes overwrite). Since $x = in - j$, a larger $j$ means a
smaller $x$ for that $i$ — so the first hit is the minimum. Getting this backwards returns
*a* solution rather than *the smallest*, which many problems reject.
