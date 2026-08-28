# Proofs — Euler's Totient Function

Five results. Each one: the claim, what it means, the proof, and why it matters in code.

**Borrowed from earlier concepts:**

- **Euclid's lemma, general form** ([gcd](../gcd-and-euclidean-algorithm/proofs.md)) — if
  $\gcd(a,b) = 1$ and $a \mid bc$ then $a \mid c$.
- **CRT** ([extended Euclid](../extended-euclidean-algorithm/proofs.md)) — for coprime
  $m, n$, a number mod $mn$ is determined by its pair of residues mod $m$ and mod $n$.
- **Unique factorization** — cited, as elsewhere in this repo.

---

## 1. $\varphi(p^k) = p^k - p^{\,k-1}$

**Claim.** For a prime $p$ and $k \ge 1$:

$$\varphi(p^k) = p^k - p^{\,k-1} = p^k\left(1 - \frac1p\right)$$

**In words.** The only way to share a factor with $p^k$ is to be a multiple of $p$. So
count those and subtract.

**Proof.** Let $1 \le a \le p^k$. Since the only prime dividing $p^k$ is $p$,

$$\gcd(a, p^k) > 1 \quad\Longleftrightarrow\quad p \mid a$$

The multiples of $p$ in $[1, p^k]$ are $p, 2p, \dots, p^{k-1}\cdot p$, so there are exactly
$p^{k-1}$ of them. Everything else is coprime to $p^k$:

$$\varphi(p^k) = p^k - p^{\,k-1} \qquad \blacksquare$$

**Special case $k=1$:** $\varphi(p) = p - 1$ — every number below a prime is coprime to it.

---

## 2. $\varphi$ is multiplicative

**Claim.** If $\gcd(m,n) = 1$ then

$$\varphi(m\,n) = \varphi(m)\,\varphi(n)$$

**In words.** Being coprime to $mn$ means being coprime to $m$ *and* coprime to $n$ — two
independent conditions. CRT says the choices really are independent, so the counts
multiply.

**Proof.** By **CRT**, since $\gcd(m,n)=1$, the map

$$x \ \longmapsto\ (x \bmod m,\ \ x \bmod n)$$

is a bijection from $\{0, 1, \dots, mn-1\}$ onto all pairs $(u, v)$ with $0 \le u < m$ and
$0 \le v < n$. (CRT says every such pair comes from exactly one $x$.)

Now the key observation:

$$\gcd(x, mn) = 1 \quad\Longleftrightarrow\quad \gcd(x,m) = 1 \ \text{ and } \ \gcd(x,n) = 1$$

($\Rightarrow$) any common factor of $x$ and $m$ is a common factor of $x$ and $mn$.
($\Leftarrow$) a prime dividing both $x$ and $mn$ must divide $m$ or $n$ (Euclid's lemma),
contradicting one of the two hypotheses.

Furthermore $\gcd(x, m) = \gcd(x \bmod m,\ m)$, so the condition depends only on the
residue pair. Therefore the bijection **restricts** to a bijection

$$\{x : \gcd(x,mn)=1\} \ \longleftrightarrow\ \{u : \gcd(u,m)=1\} \times \{v : \gcd(v,n)=1\}$$

Counting both sides gives $\varphi(mn) = \varphi(m)\varphi(n)$. $\blacksquare$

**Why coprimality is required.** Without it the CRT bijection does not exist. Concretely
$\varphi(2)\varphi(2) = 1 \cdot 1 = 1$, but $\varphi(4) = 2$.

---

## 3. The product formula

**Claim.**

$$\varphi(n) = n \prod_{p \,\mid\, n} \left(1 - \frac1p\right)$$

over the **distinct** primes dividing $n$.

**Proof.** Write $n = p_1^{e_1}p_2^{e_2}\cdots p_k^{e_k}$. The prime powers
$p_i^{e_i}$ are pairwise coprime, so applying §2 repeatedly:

$$\varphi(n) = \varphi(p_1^{e_1})\,\varphi(p_2^{e_2})\cdots\varphi(p_k^{e_k})$$

Now substitute §1 for each factor:

$$\varphi(n) = \prod_{i=1}^{k} p_i^{e_i}\left(1 - \frac{1}{p_i}\right)
= \left(\prod_{i=1}^{k} p_i^{e_i}\right)\prod_{i=1}^{k}\left(1 - \frac{1}{p_i}\right)
= n\prod_{i=1}^{k}\left(1 - \frac{1}{p_i}\right)$$

$\blacksquare$

**Why the product uses each prime once.** The exponent $e_i$ is fully absorbed into the
leading $n$ — only the *distinct* primes appear in the correction factors. Writing
$\left(1-\frac12\right)$ twice for $n = 4$ is a common error.

**In code.** This is why the loop strips each prime out completely before applying its
factor:

```cpp
while (n % p == 0) n /= p;     // consume the whole power
result -= result / p;          // but scale by (1 - 1/p) only ONCE
```

and why `result -= result / p` is used rather than a multiplication: $1/p$ is $0$ in
integer arithmetic, whereas `result / p` is exact here because $p$ divides `result` at
that moment.

---

## 4. Euler's theorem

**Claim.** If $\gcd(a, m) = 1$ then

$$a^{\,\varphi(m)} \equiv 1 \pmod m$$

**In words.** Multiplying every coprime residue by $a$ just **shuffles** them. Comparing
the product before and after forces $a^{\varphi(m)}$ to be $1$. It is the same argument
that proves Fermat's little theorem, run on the coprime residues instead of all nonzero
ones.

**Proof.** Let $S = \{r_1, r_2, \dots, r_{\varphi(m)}\}$ be the residues mod $m$ that are
coprime to $m$ — there are $\varphi(m)$ of them by definition.

**Step 1 — multiplying by $a$ keeps you inside $S$.** If $\gcd(r, m) = 1$ and
$\gcd(a,m)=1$, then $ar$ shares no prime with $m$ either, so $\gcd(ar, m) = 1$ and
$ar \bmod m \in S$.

**Step 2 — the map is injective.** Suppose $a r_i \equiv a r_j \pmod m$. Then
$m \mid a(r_i - r_j)$. Since $\gcd(a,m) = 1$, **Euclid's lemma** gives

$$m \mid (r_i - r_j)$$

and as both lie in $[0, m)$, this forces $r_i = r_j$.

**Step 3 — so it is a permutation.** An injective map from the finite set $S$ into itself
is a bijection. So $\{a r_1, \dots, a r_{\varphi(m)}\}$ is $S$ again, reordered.

**Step 4 — multiply everything.** Since the two lists contain the same residues,

$$\prod_{i=1}^{\varphi(m)} (a\,r_i) \;\equiv\; \prod_{i=1}^{\varphi(m)} r_i \pmod m$$

Pulling out the $a$'s from the left side:

$$a^{\,\varphi(m)} \prod_i r_i \;\equiv\; \prod_i r_i \pmod m$$

**Step 5 — cancel the product.** Each $r_i$ is coprime to $m$, so $\prod_i r_i$ is too,
and therefore has an inverse mod $m$. Multiplying both sides by it:

$$a^{\,\varphi(m)} \equiv 1 \pmod m \qquad \blacksquare$$

**Where the hypothesis is used.** Twice — Step 1 and Step 2 both need $\gcd(a,m)=1$.
Without it the theorem is simply false: $2^{\varphi(4)} = 2^2 = 4 \equiv 0 \pmod 4$, not $1$.

**In code — two consequences.**

$$a^{-1} \equiv a^{\,\varphi(m)-1} \pmod m$$

which works for **any** modulus, generalizing the Fermat trick from the inverse concept
(there $m = p$ and $\varphi(p) = p-1$). And

$$a^{\,k} \equiv a^{\,k \bmod \varphi(m)} \pmod m$$

because writing $k = q\,\varphi(m) + r$ gives
$a^k = \bigl(a^{\varphi(m)}\bigr)^q a^r \equiv 1^q a^r = a^r$. This is how you handle an
exponent with a hundred thousand digits.

**The trap, stated precisely.** That last reduction needs $\gcd(a,m) = 1$. For general $a$
the correct statement — valid for all $a$ once $k \ge \log_2 m$ — is

$$a^{\,k} \equiv a^{\,(k \bmod \varphi(m)) + \varphi(m)} \pmod m$$

(proved by splitting $m$ into prime powers; **cited here, not proved**). Use this form when
you cannot guarantee coprimality. Counterexample for the naive version: $a=2$, $m=4$,
$k=4$ gives $2^4 \equiv 0$, while $2^{4 \bmod 2} = 2^0 = 1$.

---

## 5. $\sum_{d \mid n} \varphi(d) = n$

**Claim.** Summing $\varphi$ over all divisors of $n$ returns $n$.

**In words.** Sort the numbers $1 \dots n$ by what their gcd with $n$ is. Each bucket turns
out to be counted by a totient, and the buckets must add back to $n$.

**Proof.** Partition $\{1, 2, \dots, n\}$ according to $\gcd(k, n)$. That gcd is always a
divisor of $n$, so the buckets are indexed by the divisors $d \mid n$:

$$\{1,\dots,n\} \;=\; \bigsqcup_{d \,\mid\, n} B_d, \qquad B_d = \{\,k : \gcd(k,n) = d\,\}$$

Now count $B_d$. Every $k$ with $\gcd(k,n) = d$ is a multiple of $d$, say $k = d\,j$ with
$1 \le j \le n/d$. And

$$\gcd(d\,j,\ n) = d \quad\Longleftrightarrow\quad \gcd\!\left(j,\ \frac nd\right) = 1$$

(dividing both entries of the gcd by $d$). So the elements of $B_d$ correspond exactly to
the $j \in [1, n/d]$ coprime to $n/d$, giving

$$|B_d| = \varphi\!\left(\frac nd\right)$$

Summing the sizes of a partition:

$$n = \sum_{d \,\mid\, n} \varphi\!\left(\frac nd\right)$$

Finally, as $d$ runs over the divisors of $n$, so does $n/d$ (the divisor-pairing from the
[divisibility](../divisibility/) concept). Relabelling gives

$$n = \sum_{d \,\mid\, n} \varphi(d) \qquad \blacksquare$$

**Check.** $n = 12$: divisors $1,2,3,4,6,12$ give
$1+1+2+2+2+4 = 12$ ✓

**In code.** This is a cheap and very effective self-test for a $\varphi$ table — the
implementation uses it as one of its checks.
