# Euler's Totient Function — Problems

Ten problems, hardest last. Every **check value** here was computed, not guessed — if your
code prints something else, the bug is yours. Answers at the bottom.

---

## By hand

**1. Five values.** Give $\varphi(1)$, $\varphi(6)$, $\varphi(7)$, $\varphi(9)$ and
$\varphi(12)$, listing the coprime residues for the last one.

**2. A round number.** Compute $\varphi(10^6)$ from the product formula. Only the
**distinct** primes appear — a repeated prime counted twice is the standard slip.

**3. The divisor identity.** Verify $\sum_{d \mid n}\varphi(d) = n$ for $n = 36$ by listing
all nine divisors and their totients.

**4. An inverse mod a composite.** Compute $7^{-1} \bmod 60$ two ways: by Euler
($a^{\varphi(m)-1}$) and by extended Euclid. Which is cheaper here, and why is Euler's
route almost never the one you want?

---

## With code

**5. Huge exponent, prime modulus.** Compute $2^{10^6} \bmod (10^9+7)$. Then say which
theorem lets you reduce the exponent, and by what.

**6. Huge exponent, hostile case.** Compute $12^{\,10^{27}} \bmod 100$, with the exponent
given as a **string**. Note $\gcd(12,100) = 4 \neq 1$. Report all three of: the true value,
what the naive reduction $k \bmod \varphi(m)$ gives, and what the safe form
$(k \bmod \varphi(m)) + \varphi(m)$ gives.

**7. All totients at once.** Sieve $\varphi(1..10^6)$ and report $\sum_{k\le10^6}\varphi(k)$.
Then use it: how many pairs $(a,b)$ with $1 \le a < b \le 10^6$ are coprime? (Compare your
answer for $n = 10$ against the $63$ from the [gcd](../gcd-and-euclidean-algorithm/)
problems.)

**8. Inverting $\varphi$.** Find every $n$ with $\varphi(n) = 8$. Bound your search first —
$\varphi(n) \ge \sqrt{n/2}$, so you need only test $n$ up to a computable limit.

---

## Harder

**9. Primes, characterized.** Prove $\varphi(n) = n-1$ if and only if $n$ is prime. Then
give the analogous characterization of $n = p^k$.

**10. Why the identity is true.** Prove $\sum_{d\mid n}\varphi(d) = n$ by sorting
$1,2,\dots,n$ into buckets by $\gcd(k,n)$. Show the bucket for $\gcd = d$ has exactly
$\varphi(n/d)$ members, then finish. This is the proof that makes the primitive-root
theorem work in the [next folder](../primitive-roots-and-discrete-log/).

---

## Judge problems

| where | problem | what it drills |
|---|---|---|
| CF 776E · 2100 | [The Holmes Children](https://codeforces.com/problemset/problem/776/E) | built directly on $\sum_{d\mid n}\varphi(d) = n$ — problem 10, as a judge task |
| CF 906D · 2700 | [Power Tower](https://codeforces.com/problemset/problem/906/D) | a tower of exponents mod $m$: recurse with the safe form of problem 6, since $\gcd$ is never guaranteed |
| LC 372 · Med | [Super Pow](https://leetcode.com/problems/super-pow/) | exponent arrives as a digit array and the modulus $1337 = 7\cdot191$ is composite — exactly where the naive $\varphi$ reduction is wrong |
| LC 1447 · Med | [Simplified Fractions](https://leetcode.com/problems/simplified-fractions/) | the coprime-pair count of problem 7, small enough to brute force |

---

## Answers

**1.** $\varphi(1) = 1$, $\varphi(6) = 2$, $\varphi(7) = 6$, $\varphi(9) = 6$,
$\varphi(12) = 4$ — the last from $\{1,5,7,11\}$.

**2.** $10^6 = 2^6\cdot5^6$, so
$\varphi = 10^6\bigl(1-\frac12\bigr)\bigl(1-\frac15\bigr) = \mathbf{400000}$. The
exponents never enter the product; only the two distinct primes do.

**3.** Divisors $1,2,3,4,6,9,12,18,36$ with totients $1,1,2,2,2,6,4,6,12$, summing to
$\mathbf{36}$ ✓.

**4.** $7^{-1} \equiv \mathbf{43} \pmod{60}$ ($7\cdot43 = 301 = 5\cdot60+1$). Euler needs
$\varphi(60) = 16$, which needs the factorization of $60$; extended Euclid needs nothing
but the two numbers and runs in $O(\log m)$. For a one-off inverse, extgcd wins — Euler's
form is for when you already have $\varphi(m)$, or want the *formula* rather than the
number.

**5.** $2^{10^6} \equiv \mathbf{235042059} \pmod{10^9+7}$. Fermat (the prime case of
Euler) gives $a^{p-1}\equiv1$, so the exponent may be reduced mod $p-1 = 10^9+6$ — here it
is already smaller, so the win is only that you *know* it is safe.

**6.** True value $\mathbf{76}$. Naive $k \bmod \varphi(100) = k \bmod 40 = 0$ gives
$12^0 = \mathbf 1$ — wrong. The safe form $(k \bmod 40) + 40 = 40$ gives
$12^{40} \equiv \mathbf{76}$ ✓. The extra $+\varphi(m)$ costs nothing and is valid for
**all** $a$ once $k \ge \log_2 m$; use it unless you have checked $\gcd(a,m) = 1$.

**7.** $\sum_{k\le10^6}\varphi(k) = \mathbf{303963552392}$. Coprime pairs with $a<b\le n$
are $\sum_{k=2}^{n}\varphi(k) = \mathbf{303963552391}$ — the same sum minus the $k=1$ term,
since for each $b$ the valid $a<b$ number exactly $\varphi(b)$. At $n=10$ this gives $31$
unordered pairs with $a<b$, matching $63 = 2\cdot31+1$ ordered pairs including $(1,1)$.

**8.** $\varphi(n) = 8$ for $n \in \{\mathbf{15, 16, 20, 24, 30}\}$. Nothing above $30$
works: $\varphi(n) \ge \sqrt{n/2}$ bounds the search at $n \le 128$, and a sieve settles
it.

**9.** If $n$ is prime, all of $1,\dots,n-1$ are coprime to it, so $\varphi(n) = n-1$.
Conversely, if $n$ is composite it has a divisor $d$ with $1 < d < n$, and $d$ is not
coprime to $n$ — so at least two of $1,\dots,n$ (namely $d$ and $n$) are excluded, giving
$\varphi(n) \le n-2$. And $\varphi(n) = n - n/p$ exactly when $n$ is a power of the single
prime $p$.

**10.** Sort $1,\dots,n$ by $g = \gcd(k,n)$, which is always a divisor of $n$. The bucket
for a fixed $d \mid n$ holds the $k$ with $\gcd(k,n) = d$; writing $k = d\,k'$, that says
$k' \le n/d$ and $\gcd(k', n/d) = 1$ — so the bucket has $\varphi(n/d)$ members. Every $k$
lands in exactly one bucket, hence

$$n = \sum_{d\mid n}\varphi(n/d) = \sum_{d\mid n}\varphi(d)$$

the last step because $d \mapsto n/d$ permutes the divisors.

---

[README.md](README.md) · [proofs.md](proofs.md) · [implementation.cpp](implementation.cpp)
