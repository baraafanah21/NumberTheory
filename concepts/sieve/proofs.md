# Proofs — Sieve of Eratosthenes

**Borrowed** from [divisibility](../divisibility/proofs.md): a composite $n$ has a prime
factor $\le \sqrt n$.

---

## 1. The sieve is correct

**Claim.** A number $m \ge 2$ is left unmarked $\iff$ it is prime.

**No prime is marked.** Marks happen only as $m = kp$ with $k,p \ge 2$, so $m$ has a
divisor strictly between $1$ and $m$ — it is composite. $\blacksquare$

**Every composite is marked.** Let $p$ be the smallest prime factor of composite $m$. Then
$p \le \sqrt m \le \sqrt n$, so the outer loop reaches $p$, and $p$ is still unmarked (no
prime is ever marked). It remains to check $m \ge p^2$, so that $m$ lies in the marked
range. Write $m = pk$; every prime factor of $k$ is also one of $m$, hence $\ge p$ by
minimality, and $k>1$ since $m$ is composite — so $k \ge p$ and

$$m = pk \ge p^2 \qquad\blacksquare$$

**Justifies both optimisations at once:** starting at $p^2$ loses nothing (the smallest
multiple of $p$ that *needs* $p$ to mark it is $p^2$), and stopping the outer loop at
$\sqrt n$ loses nothing (every composite is caught by its smallest prime factor, which is
$\le\sqrt n$).

---

## 2. Multiples below $p^2$ are already gone

**Claim.** If $2 \le k < p$ then $kp$ was marked before $p$'s pass.

**Proof.** Let $q$ be the smallest prime factor of $k$. Then $q \le k < p$, so $q$'s pass
ran earlier; and $q \mid k \mid kp$. Since $k \ge q$ and $p > q$, we have $kp > q^2$, so
$kp$ lay in $q$'s marked range. $\blacksquare$

---

## 3. The linear sieve is linear

**Claim.** In the loop with the `p > spf[i]` break, every composite $m \le n$ is written
**exactly once**: as $m = i\cdot p$ with $p = \mathrm{spf}(m)$ and $i = m/p$.

**In words.** The ordinary sieve marks $12$ three times. Here each composite has exactly
one producing pair, so the total work is the number of composites — linear.

**At least once.** Let $p = \mathrm{spf}(m)$, $i = m/p$. Every prime factor of $i$ is one of
$m$, hence $\ge p$, so $\mathrm{spf}(i) \ge p$. The inner loop walks primes in **increasing**
order and breaks only past $\mathrm{spf}(i)$ — so it has not broken when it reaches $p$.
And $ip = m \le n$, so the size guard passes.

**At most once.** Suppose the loop writes $m = i'p'$. The break condition guarantees
$p' \le \mathrm{spf}(i')$, so every prime factor of $i'$ is $\ge p'$. The prime factors of
$m$ are $p'$ together with those of $i'$, all $\ge p'$ — and $p'$ is one of them. Hence

$$p' = \mathrm{spf}(m), \qquad i' = m/p'$$

The pair is forced. $\blacksquare$

**Justifies** the break — it *is* the algorithm. Remove it and you get the ordinary sieve
with extra bookkeeping. (`if (i % p == 0) break;` after the assignment is the same
condition written differently.)

---

## 4. Where $\log\log n$ comes from

**Claim.** The plain sieve performs $\Theta(n\log\log n)$ marks.

**Proof.** Prime $p$'s pass marks about $n/p$ numbers, so the total is

$$\sum_{p \le n} \frac np = n\sum_{p \le n}\frac 1p$$

**Mertens' second theorem** (cited — it needs analytic number theory) gives
$\sum_{p\le n}\frac1p = \ln\ln n + M + o(1)$. $\blacksquare$

**Contrast with the divisibility sieve.** There the inner loop ran for **every** $d$,
giving $\sum_{d\le n} n/d = nH_n = \Theta(n\log n)$. Here it runs only for **primes**, and
primes are sparse enough to collapse $\log n$ to $\log\log n$.

**In practice** $\log\log n < 4$ for every $n$ below $10^{18}$ — a sieve to $10^7$ runs in
well under a second.

---

## 5. Factorizing with `spf` costs $O(\log m)$

**Claim.** Repeatedly dividing by $\mathrm{spf}[m]$ terminates in $\le \log_2 m$ divisions
and gives the full factorization.

**Proof.** Each division replaces $m$ by $m/p$ with $p \ge 2$, so the value at least halves
— after $k$ steps it is $\le m/2^k$. Correctness: $\mathrm{spf}[m]$ is by definition a prime
dividing $m$, and removing the smallest each time emits the primes in increasing order with
none missed. $\blacksquare$

That this is *the* factorization, not merely *a* factorization, is
[unique factorization](../unique-factorization/proofs.md) §2.

**Justifies** building `spf` before factorizing many numbers: about $23$ operations at
$m \le 10^7$, versus $\sqrt m \approx 3000$ for trial division. With the factorization in
hand, $\tau(m) = \prod(e_i+1)$ and $\sigma(m) = \prod\frac{p_i^{e_i+1}-1}{p_i-1}$ follow in
$O(\log m)$.
