# Proofs — Primitive Roots and Discrete Logarithm

Five results. Each one: the claim, what it means, the proof, and why it matters in code.

**Borrowed from earlier concepts:**

- **Euler's theorem** ([totient](../euler-totient-function/proofs.md)) —
  $\gcd(a,m)=1 \Rightarrow a^{\varphi(m)} \equiv 1$.
- **The totient identity** (same file) — $\sum_{d \mid n}\varphi(d) = n$. This is what
  makes §3 work.
- **Euclid's lemma** ([gcd](../gcd-and-euclidean-algorithm/proofs.md)).
- **Division algorithm** ([divisibility](../divisibility/proofs.md)).

---

## 1. The order exists, and divides $\varphi(m)$

**Claim.** Let $\gcd(a,m)=1$ and let $d = \mathrm{ord}_m(a)$ be the smallest $k \ge 1$ with
$a^k \equiv 1$. Then

$$a^{\,k} \equiv 1 \pmod m \quad\Longleftrightarrow\quad d \mid k$$

and in particular $d \mid \varphi(m)$.

**In words.** The exponents that send $a$ back to $1$ are *exactly* the multiples of the
first one. Nothing in between ever works.

**Proof.** First, $d$ exists: Euler's theorem gives $a^{\varphi(m)} \equiv 1$, so the set of
valid exponents is non-empty, and a non-empty set of positive integers has a least element.

($\Leftarrow$) If $k = d\,t$ then $a^k = (a^d)^t \equiv 1^t = 1$.

($\Rightarrow$) Suppose $a^k \equiv 1$. By the **division algorithm**, write

$$k = q\,d + r, \qquad 0 \le r < d$$

Then

$$a^{\,r} = a^{\,k - qd} = a^{\,k}\cdot\bigl(a^{\,d}\bigr)^{-q} \equiv 1 \cdot 1^{-q} = 1 \pmod m$$

(the inverse is legitimate: $a^d \equiv 1$ is invertible). So $a^r \equiv 1$ with
$0 \le r < d$. If $r > 0$ this contradicts $d$ being the **smallest** such exponent.
Hence $r = 0$, i.e. $d \mid k$. $\blacksquare$

**Then $d \mid \varphi(m)$** is the special case $k = \varphi(m)$, using Euler.

**In code.** This is what makes computing the order cheap. Instead of trying
$k = 1, 2, 3, \dots$, start from $\varphi(m)$ — which is guaranteed to work — and divide
out prime factors as long as the result still works:

```cpp
ll ord = phi(m);
for (ll q : distinctPrimeFactors(ord))
    while (ord % q == 0 && powerMod(a, ord / q, m) == 1) ord /= q;
```

Only divisors of $\varphi(m)$ can possibly be the order, so nothing else is worth testing.

---

## 2. The order of a power

**Claim.** If $\mathrm{ord}_m(a) = d$, then for any $k \ge 1$

$$\mathrm{ord}_m\!\left(a^{\,k}\right) \;=\; \frac{d}{\gcd(d,\,k)}$$

**In words.** Raising to the $k$-th power shortens the cycle by whatever $k$ shares with
its length.

**Proof.** Let $e = \gcd(d,k)$. Using §1 twice:

$$\bigl(a^{\,k}\bigr)^{t} \equiv 1 \quad\Longleftrightarrow\quad a^{\,kt} \equiv 1 \quad\Longleftrightarrow\quad d \mid k\,t$$

Divide that divisibility by $e$:

$$d \mid k\,t \quad\Longleftrightarrow\quad \frac de \ \Big|\ \frac ke\,t$$

Now $\gcd\!\left(\frac de, \frac ke\right) = 1$ (dividing out the gcd leaves coprime
numbers), so by **Euclid's lemma** $\frac de$ must divide $t$ itself:

$$\frac de \ \Big|\ \frac ke\,t \quad\Longleftrightarrow\quad \frac de \ \Big|\ t$$

So the exponents $t$ that work are exactly the multiples of $\frac de$, and the smallest
positive one is $\frac de = \frac{d}{\gcd(d,k)}$. $\blacksquare$

**Corollary — how many primitive roots there are.** Suppose $g$ is a primitive root mod
$m$, so $\mathrm{ord}(g) = \varphi(m)$. Every coprime residue is $g^k$ for some
$0 \le k < \varphi(m)$, and by the claim

$$\mathrm{ord}\!\left(g^{\,k}\right) = \varphi(m) \quad\Longleftrightarrow\quad \gcd(k, \varphi(m)) = 1$$

The number of such $k$ is $\varphi(\varphi(m))$ by definition. So **once one primitive root
exists, there are exactly $\varphi(\varphi(m))$ of them.** $\blacksquare$

**In code.** This is why searching $g = 2, 3, 4, \dots$ terminates quickly: primitive roots
are not rare.

---

## 3. Every prime has a primitive root

**Claim.** For every prime $p$ there exists $g$ with $\mathrm{ord}_p(g) = p-1$.

**In words.** This is the theorem that makes discrete logarithms mod $p$ well posed. It is
also the most interesting proof here — it counts elements of each possible order and shows
the counts are forced.

**One lemma, cited.** *A polynomial of degree $d$ with coefficients mod a prime $p$ has at
most $d$ roots mod $p$.* (True because $\mathbb{Z}/p$ is a field, so the factor theorem
applies and each root splits off a linear factor. It **fails** for composite moduli —
$x^2 \equiv 1 \pmod 8$ has four solutions, which is exactly why primes are special here.)

**Proof.** For each $d \mid p-1$, let

$$\psi(d) = \#\{\,a \in \{1,\dots,p-1\} \ :\ \mathrm{ord}_p(a) = d\,\}$$

**Step 1 — the orders account for everything.** Every $a \in \{1,\dots,p-1\}$ is coprime to
$p$, so it has an order, and by §1 that order divides $p-1$. Sorting the $p-1$ elements by
their order:

$$\sum_{d \,\mid\, p-1} \psi(d) \;=\; p-1$$

**Step 2 — $\psi(d) \le \varphi(d)$ for every $d$.** If $\psi(d) = 0$ this is clear. So
suppose some $a$ has order exactly $d$. Consider

$$a^0,\ a^1,\ \dots,\ a^{\,d-1}$$

These are **distinct** (if $a^i \equiv a^j$ with $i<j<d$ then $a^{\,j-i} \equiv 1$ with
$0 < j-i < d$, contradicting minimality of $d$), and each satisfies
$\bigl(a^i\bigr)^d = (a^d)^i \equiv 1$. So they are $d$ distinct roots of the polynomial

$$x^{\,d} - 1 \equiv 0 \pmod p$$

By the lemma that polynomial has **at most** $d$ roots — so these are *all* of them.
Therefore any element of order $d$ (it certainly satisfies $x^d \equiv 1$) must be one of
the $a^i$. And by §2, $\mathrm{ord}(a^i) = d$ exactly when $\gcd(i,d) = 1$, which happens
for $\varphi(d)$ values of $i$. Hence $\psi(d) = \varphi(d)$ in this case, and in general

$$\psi(d) \le \varphi(d)$$

**Step 3 — the totient identity forces equality.** From the
[totient concept](../euler-totient-function/proofs.md):

$$\sum_{d \,\mid\, p-1} \varphi(d) \;=\; p-1$$

Combining with Step 1, the two sums are equal:

$$\sum_{d \,\mid\, p-1} \psi(d) \;=\; \sum_{d \,\mid\, p-1} \varphi(d)$$

But Step 2 says $\psi(d) \le \varphi(d)$ **term by term**. If any single term were strictly
smaller, the left sum would be strictly smaller than the right. So no term can be smaller:

$$\psi(d) = \varphi(d) \qquad\text{for every } d \mid p-1$$

**Step 4 — read off the answer.** Take $d = p-1$:

$$\psi(p-1) = \varphi(p-1) \ \ge\ 1$$

so at least one element has order exactly $p-1$ — a primitive root. $\blacksquare$

**The shape of the argument.** Two sums are equal, and one is termwise $\le$ the other;
therefore they agree termwise. That squeeze is the whole trick, and it needs the totient
identity to supply the second sum.

**Why only special moduli have primitive roots.** The lemma about root counts fails for
composite $m$, and with it the argument. The full classification
($m = 1, 2, 4, p^k, 2p^k$) is **cited, not proved here**.

---

## 4. The fast primitive-root test

**Claim.** For a prime $p$ and $\gcd(g,p) = 1$:

$$\mathrm{ord}_p(g) = p-1 \quad\Longleftrightarrow\quad g^{\frac{p-1}{q}} \not\equiv 1 \pmod p \ \text{ for every prime } q \mid (p-1)$$

**In words.** To confirm the order is as large as possible, you only need to rule out the
*maximal proper divisors* of $p-1$ — and there is one of those per prime factor.

**Proof.** ($\Rightarrow$) If the order is $p-1$, then no smaller positive exponent gives
$1$. Each $\frac{p-1}{q}$ is smaller than $p-1$ (as $q \ge 2$), so none of them can.

($\Leftarrow$) Prove the contrapositive: suppose $d = \mathrm{ord}_p(g) < p-1$. By §1,
$d \mid p-1$, so $\frac{p-1}{d}$ is an integer greater than $1$, and therefore has some
prime factor $q$. Then $q \mid \frac{p-1}{d}$, so writing $\frac{p-1}{d} = q\,t$ gives

$$p - 1 = d\,q\,t \qquad\Longrightarrow\qquad \frac{p-1}{q} = d\,t$$

So $d \mid \frac{p-1}{q}$, and by §1 that means

$$g^{\frac{p-1}{q}} \equiv 1 \pmod p$$

i.e. the test fails for this $q$. $\blacksquare$

**In code.** $p-1$ has at most about $15$ distinct prime factors below $2^{64}$, so this is
a handful of exponentiations instead of $O(p)$ work. The cost is dominated by factoring
$p-1$, not by the tests.

---

## 5. Baby-step giant-step is correct

**Claim.** Let $n = \lceil\sqrt m\,\rceil$. Every $x$ with $1 \le x \le m$ can be written

$$x = i\,n - j, \qquad 1 \le i \le n, \quad 0 \le j < n$$

and therefore $g^x \equiv b$ has a solution in range iff

$$\bigl(g^{\,n}\bigr)^{i} \equiv b\,g^{\,j} \pmod m \quad\text{for some such } i, j$$

**In words.** Split the unknown exponent into a "how many big strides" part and a "how far
back" part. Each side of the rearranged equation then involves only one of them, so you can
tabulate one side and look up the other — meet in the middle.

**Proof of the representation.** Given $x$ in $[1, m]$, choose

$$i = \left\lceil \frac{x}{n} \right\rceil, \qquad j = i\,n - x$$

Then $i \ge 1$, and $i \le \lceil m/n \rceil \le n$ because $n^2 \ge m$. For $j$: by
definition of the ceiling, $(i-1)n < x \le i\,n$, so

$$0 \le i\,n - x < n$$

which is exactly $0 \le j < n$. ✓

**Proof of the equivalence.** Since $\gcd(g,m)=1$, $g$ is invertible mod $m$, so
multiplying both sides of a congruence by $g^{\,j}$ is reversible:

$$g^{\,in-j} \equiv b \quad\Longleftrightarrow\quad g^{\,in} \equiv b\,g^{\,j} \quad\Longleftrightarrow\quad \bigl(g^{\,n}\bigr)^i \equiv b\,g^{\,j}$$

$\blacksquare$

**Cost.** The baby steps compute $n$ values $b\,g^{\,j}$; the giant steps compute at most
$n$ values $(g^n)^i$. With a hash map for lookups, that is $O(\sqrt m)$ time and
$O(\sqrt m)$ memory.

**In code — getting the *smallest* $x$.** Scan $i$ ascending, and when storing baby steps
keep the **largest** $j$ for each value (later writes overwrite earlier ones). Since
$x = i\,n - j$, a larger $j$ means a smaller $x$ for that $i$ — so the first hit found is
genuinely the minimum. Getting this backwards returns *a* solution rather than *the
smallest*, which many problems reject.

**Why the reverse direction is not this easy.** Computing $g^x$ takes $O(\log x)$ by
repeated squaring; recovering $x$ takes $O(\sqrt m)$ here, and no polynomial-time method is
known for general $m$. That asymmetry is deliberate in cryptography.
