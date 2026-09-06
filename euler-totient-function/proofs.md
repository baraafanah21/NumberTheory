# Proofs — Euler's Totient Function

**Borrowed:** **Euclid's lemma** ([gcd](../gcd-and-euclidean-algorithm/proofs.md)); **CRT**
([chinese remainder theorem](../chinese-remainder-theorem/proofs.md)); **unique
factorization** ([unique factorization](../unique-factorization/proofs.md)).

---

## 1. $\varphi(p^k) = p^k - p^{\,k-1}$

**Claim.** As stated, for prime $p$ and $k \ge 1$.

**Proof.** The only prime dividing $p^k$ is $p$, so for $1 \le a \le p^k$,

$$\gcd(a,p^k) > 1 \iff p \mid a$$

The multiples of $p$ in $[1,p^k]$ are $p, 2p, \dots, p^{k-1}\cdot p$ — exactly $p^{k-1}$ of
them. $\blacksquare$

At $k=1$: $\varphi(p) = p-1$.

---

## 2. $\varphi$ is multiplicative

**Claim.** $\gcd(m,n)=1 \Rightarrow \varphi(mn) = \varphi(m)\varphi(n)$.

**In words.** Being coprime to $mn$ means being coprime to $m$ *and* to $n$ — two
independent conditions. CRT says the choices really are independent, so the counts
multiply.

**Proof.** By **CRT**, $x \mapsto (x \bmod m,\ x \bmod n)$ is a bijection from
$\{0,\dots,mn-1\}$ onto all pairs. And

$$\gcd(x,mn)=1 \iff \gcd(x,m)=1 \ \text{ and } \ \gcd(x,n)=1$$

($\Rightarrow$) a common factor of $x$ and $m$ is one of $x$ and $mn$. ($\Leftarrow$) a
prime dividing $x$ and $mn$ divides $m$ or $n$ by **Euclid's lemma**, contradicting one
hypothesis.

Since $\gcd(x,m) = \gcd(x \bmod m,\ m)$, the condition depends only on the residue pair, so
the bijection **restricts** to one between the coprime residues on each side. Counting both
sides gives the claim. $\blacksquare$

**Why coprimality is required.** Without it there is no CRT bijection —
$\varphi(2)\varphi(2)=1$ while $\varphi(4)=2$.

---

## 3. The product formula

**Claim.** $\ \varphi(n) = n\prod_{p \mid n}\bigl(1-\frac1p\bigr)$ over **distinct** primes.

**Proof.** Write $n = \prod_i p_i^{e_i}$. The prime powers are pairwise coprime, so §2
applies repeatedly, then §1 to each factor:

$$\varphi(n) = \prod_i \varphi(p_i^{e_i}) = \prod_i p_i^{e_i}\Bigl(1-\frac1{p_i}\Bigr)
= n\prod_i\Bigl(1-\frac1{p_i}\Bigr) \qquad\blacksquare$$

**Why each prime appears once.** The exponent $e_i$ is absorbed into the leading $n$; only
the *distinct* primes give correction factors. Writing $\bigl(1-\frac12\bigr)$ twice for
$n=4$ is the classic error.

**Justifies** the loop shape: strip the whole power (`while (n % p == 0) n /= p;`) but
apply the factor **once**, and use `r -= r / p` rather than a multiplication, since $1/p$
truncates to $0$ while `r / p` is exact at that moment.

---

## 4. Euler's theorem

**Claim.** $\gcd(a,m)=1 \Rightarrow a^{\,\varphi(m)} \equiv 1 \pmod m$.

**In words.** Multiplying every coprime residue by $a$ **shuffles** them; comparing the
product before and after forces $a^{\varphi(m)}$ to be $1$. Same argument as Fermat, run on
the coprime residues instead of all nonzero ones.

**Proof.** Let $S = \{r_1,\dots,r_{\varphi(m)}\}$ be the residues coprime to $m$.

*Multiplying by $a$ stays in $S$:* $\gcd(r,m)=1$ and $\gcd(a,m)=1$ give $\gcd(ar,m)=1$.

*It is injective:* $ar_i \equiv ar_j$ gives $m \mid a(r_i-r_j)$, and since $\gcd(a,m)=1$,
**Euclid's lemma** gives $m \mid (r_i-r_j)$; both lie in $[0,m)$, so $r_i = r_j$.

An injective map from a finite set to itself is a **bijection**, so multiplying all
elements of each list gives the same value:

$$a^{\,\varphi(m)}\prod_i r_i \;\equiv\; \prod_i r_i \pmod m$$

Each $r_i$ is coprime to $m$, so their product is invertible — cancel it. $\blacksquare$

**Where the hypothesis is used:** twice, in both bullet points. Without it the theorem is
false — $2^{\varphi(4)} = 4 \equiv 0 \pmod 4$.

**Two consequences.** $a^{-1} \equiv a^{\,\varphi(m)-1}$ for **any** modulus (generalizing
the Fermat trick, where $m=p$ and $\varphi(p)=p-1$); and writing
$k = q\varphi(m)+r$ gives

$$a^{\,k} = \bigl(a^{\varphi(m)}\bigr)^q a^{\,r} \equiv a^{\,k \bmod \varphi(m)}$$

which is how you handle an exponent with a hundred thousand digits.

**The trap, stated precisely.** That reduction needs $\gcd(a,m)=1$. For general $a$ the
correct form — valid for all $a$ once $k \ge \log_2 m$ — is

$$a^{\,k} \equiv a^{\,(k \bmod \varphi(m)) + \varphi(m)} \pmod m$$

(**cited, not proved** — it needs a prime-power split). Counterexample for the naive
version: $a=2$, $m=4$, $k=4$ gives $2^4 \equiv 0$ while $2^{4 \bmod 2} = 1$.

---

## 5. $\sum_{d \mid n} \varphi(d) = n$

**Claim.** As stated.

**In words.** Sort $1\dots n$ by their gcd with $n$; each bucket turns out to be counted by
a totient, and the buckets must add back to $n$.

**Proof.** Partition $\{1,\dots,n\}$ by $\gcd(k,n)$, which is always a divisor of $n$:

$$\{1,\dots,n\} = \bigsqcup_{d \mid n} B_d, \qquad B_d = \{k : \gcd(k,n) = d\}$$

Every $k \in B_d$ is a multiple of $d$, say $k = dj$ with $1 \le j \le n/d$, and

$$\gcd(dj,\ n) = d \iff \gcd\Bigl(j,\ \frac nd\Bigr) = 1$$

so $|B_d| = \varphi(n/d)$. Summing the parts:

$$n = \sum_{d \mid n}\varphi\Bigl(\frac nd\Bigr) = \sum_{d \mid n}\varphi(d)$$

since $n/d$ runs over the divisors as $d$ does. $\blacksquare$

**Check.** $n=12$: $1+1+2+2+2+4 = 12$ ✓

**Used** as a cheap self-test for a $\varphi$ table — and it is the identity that makes the
primitive-root theorem work.
