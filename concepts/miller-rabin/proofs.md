# Proofs — Miller–Rabin

**Borrowed:** **Fermat's little theorem**
([modular inverse](../modular-multiplicative-inverse/proofs.md) §3) and **Euclid's lemma**
([gcd](../gcd-and-euclidean-algorithm/proofs.md)).

---

## 1. The Fermat test, and why it fails

**Claim.** If $a^{\,n-1} \not\equiv 1 \pmod n$ for some $a$ with $\gcd(a,n)=1$, then $n$ is
composite.

**Proof.** Contrapositive of Fermat: were $n$ prime, every such $a$ would satisfy
$a^{\,n-1}\equiv1$. $\blacksquare$

**Why this is not a primality test.** The converse is false, and not just occasionally.
A **Carmichael number** is a composite $n$ with $a^{\,n-1}\equiv1$ for *every* $a$ coprime
to $n$. The smallest is

$$561 = 3\times11\times17$$

*Check:* for $a$ coprime to $561$, Fermat gives $a^2\equiv1 \pmod 3$, $a^{10}\equiv1
\pmod{11}$, $a^{16}\equiv1 \pmod{17}$. Since $560$ is divisible by $2$, $10$ and $16$, we
get $a^{560}\equiv1$ modulo each of $3,11,17$, hence modulo their product by **CRT**.
$\blacksquare$

There are infinitely many Carmichael numbers, so no amount of extra bases rescues the
Fermat test. Miller–Rabin needs a genuinely stronger condition.

---

## 2. Square roots of 1 modulo a prime

**Claim.** If $p$ is prime and $x^2 \equiv 1 \pmod p$, then $x \equiv \pm1 \pmod p$.

**Proof.** $p \mid (x^2-1) = (x-1)(x+1)$. By **Euclid's lemma**, $p \mid (x-1)$ or
$p \mid (x+1)$ — that is, $x \equiv 1$ or $x \equiv -1$. $\blacksquare$

**This is the whole idea of the test.** It fails for composites: modulo $8$ the equation
$x^2\equiv1$ has **four** solutions $\{1,3,5,7\}$, and modulo $561$ it has eight. Those
extra square roots of $1$ are what Miller–Rabin detects — and unlike the Fermat condition,
a Carmichael number cannot hide them.

---

## 3. A prime always passes

**Claim.** Let $n$ be an odd prime and write $n-1 = d\cdot2^{\,s}$ with $d$ odd. Then for
every $a$ with $n \nmid a$:

$$a^{\,d} \equiv 1 \quad\text{or}\quad a^{\,2^{r}d} \equiv -1 \ \text{ for some } 0 \le r < s$$

**In words.** Square $a^{\,d}$ repeatedly and you reach $a^{\,n-1}$, which Fermat says is
$1$. Walking back from that $1$, the step before it must have been $\pm1$ — and if it was
$1$ you keep walking back. Either you reach $-1$ somewhere, or the very first entry was
already $1$.

**Proof.** Consider the chain

$$a^{\,d},\quad a^{\,2d},\quad a^{\,4d},\quad \dots,\quad a^{\,2^{s}d} = a^{\,n-1}$$

Each term is the square of the previous. By **Fermat** the last one is $1$.

If every term equals $1$, then in particular the first does, giving $a^{\,d}\equiv1$ — the
first alternative.

Otherwise let $r$ be the **largest** index with $a^{\,2^{r}d} \not\equiv 1$. Since the last
term is $1$ we have $r < s$, and by maximality the next term is $1$:

$$\bigl(a^{\,2^{r}d}\bigr)^2 = a^{\,2^{r+1}d} \equiv 1$$

So $a^{\,2^{r}d}$ is a square root of $1$; by §2 it is $\pm1$; and it is not $1$ by choice
of $r$. Hence $a^{\,2^{r}d} \equiv -1$ — the second alternative. $\blacksquare$

**Justifies the code exactly.** Compute $x = a^{\,d}$; accept immediately if $x$ is $1$ or
$n-1$; otherwise square up to $s-1$ times, accepting if $n-1$ ever appears. Falling out of
the loop means neither alternative held, so **$n$ is composite** — with certainty, not
probability.

**Note the asymmetry.** This direction is one-sided: a "composite" answer is a *proof*
(the base $a$ is a witness), while a "probably prime" answer is only evidence until §4
bounds it.

---

## 4. A composite fails for most bases

**Claim.** If $n > 2$ is an odd composite, then at least $\frac34$ of the bases
$a \in \{1,\dots,n-1\}$ are witnesses — the test correctly reports "composite".

**Cited, not proved here.** The argument counts the *non*-witnesses as a subgroup of
$(\mathbb{Z}/n)^\times$ and shows it is a proper subgroup of index $\ge 4$ when $n$ is
composite, splitting into cases on whether $n$ is a prime power or has two distinct prime
factors. It is a page of group theory beyond the scope of this repo (Rabin, 1980).

**What it gives you.** With $k$ random bases the chance of wrongly calling a composite
prime is at most $4^{-k}$ — so $k=20$ gives $10^{-12}$.

**But you can do better than probability.** Because failure is so common, exhaustive
computation has *verified* fixed base sets against all composites below a bound. Writing
$\psi_k$ for the smallest composite passing the first $k$ primes as bases:

| bases | deterministic for |
|---|---|
| $\{2,3,5,7\}$ | $n < 3.2\times10^9$ |
| $\{2,3,5,7,11,13\}$ | $n < 3.5\times10^{12}$ |
| first 11 primes, $\{2,\dots,31\}$ | $n < 3.8\times10^{18}$ — **does not cover all 64-bit** |
| first 12 primes, $\{2,\dots,37\}$ | $n < 3.18\times10^{23}$ |

So the twelve-base test is **deterministic** for every 64-bit input: not "prime with high
probability", but prime. These bounds are computational results (Jaeschke; Sorenson &
Webster), cited rather than derived.

**The trap in the table.** Eleven primes stop at $3.8\times10^{18}$, just below
$2^{64} \approx 1.8\times10^{19}$. Dropping one base to save time makes the test wrong on
part of the `long long` range.

---

## 5. Why `__int128` is not optional

**Claim.** Computing `a * b % n` in 64-bit arithmetic is wrong whenever
$a\,b \ge 2^{63}$.

**Proof.** Signed overflow in C++ is undefined behaviour, and in practice wraps modulo
$2^{64}$ — so the value reduced is $ab \bmod 2^{64}$, not $ab$. Since Miller–Rabin runs at
$n$ up to $9\times10^{18}$, products reach $8\times10^{37} \approx 2^{126}$. $\blacksquare$

**Justifies** `(__int128)a * b % n`, which holds the full product before reducing. This is
the single most common way a hand-written Miller–Rabin silently returns nonsense: it works
on every small test and fails on the large inputs it exists to handle.
