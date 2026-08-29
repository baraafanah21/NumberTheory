# Proofs — Sieve of Eratosthenes

Five results. Each one: the claim, what it means, the proof, and why it matters in code.

**Borrowed from [divisibility](../divisibility/proofs.md):** a composite $n$ always has a
prime factor $\le \sqrt n$, because if $n = ab$ with $1 < a \le b$ then $a^2 \le ab = n$.

---

## 1. Why the sieve is correct

**Claim.** After the sieve finishes, a number $m \ge 2$ is left unmarked **if and only if**
it is prime.

**In words.** Crossing out multiples removes exactly the composites — no primes are lost,
and no composites survive.

**Proof.**

**No prime gets marked.** A number is only ever marked as $m = k\,p$ with $k \ge 2$ and
$p \ge 2$. Such an $m$ has a divisor $p$ with $1 < p < m$, so $m$ is composite. Primes are
therefore never marked.

**Every composite gets marked.** Let $m$ be composite and let $p$ be its smallest prime
factor. Then $p \le \sqrt m \le \sqrt n$, so the outer loop does reach $p$. And $p$ is
still unmarked when the loop reaches it — by the first half, primes never get marked. So
the inner loop runs for $p$, and it marks every multiple of $p$ from $p^2$ upward.

It remains to check $m$ is one of those, i.e. that $m \ge p^2$. Write $m = p\,k$. Every
prime factor of $k$ is also a prime factor of $m$, hence $\ge p$ by minimality of $p$. Since
$m$ is composite, $k > 1$, so $k$ has at least one prime factor and therefore $k \ge p$.
Thus

$$m = p\,k \ \ge\ p\cdot p = p^2$$

so $m$ lies in the marked range. $\blacksquare$

**In code.** This proves both optimisations at once:

- **starting at $p^2$** loses nothing, because the argument above shows the smallest
  multiple of $p$ that *needs* $p$ to mark it is $p^2$ itself;
- **stopping the outer loop at $\sqrt n$** loses nothing, because every composite
  $m \le n$ is caught by its smallest prime factor, which is $\le \sqrt m \le \sqrt n$.

---

## 2. Why multiples below $p^2$ are already gone

**Claim.** If $k < p$ then $k\,p$ was marked before the pass for $p$ began.

**In words.** This is the reason the inner loop may start at $p^2$ rather than $2p$ — half
the intuition people are missing when they write the slower version.

**Proof.** Let $k < p$ with $k \ge 2$, and let $q$ be the smallest prime factor of $k$.
Then

$$q \le k < p$$

so $q$ is a prime strictly smaller than $p$, meaning its pass ran earlier. And $q$ divides
$k$, hence divides $kp$. So $kp$ is a multiple of $q$ and was marked during $q$'s pass —
provided $kp \ge q^2$, which holds since $kp > q \cdot q$ (both $k \ge q$ and $p > q$).
$\blacksquare$

---

## 3. Why the linear sieve is linear

**Claim.** In the loop

```cpp
for (int i = 2; i <= n; ++i) {
    if (spf[i] == 0) { spf[i] = i; primes.push_back(i); }
    for (int p : primes) {
        if (p > spf[i] || (ll)i * p > n) break;
        spf[i * p] = p;
    }
}
```

every composite $m \le n$ is written exactly **once**, namely as $m = i \cdot p$ with
$p = \mathrm{spf}(m)$ and $i = m/p$.

**In words.** The ordinary sieve marks $12$ three times (via $2$, via $3$, and via $2$
again). Here each composite is produced by exactly one $(i, p)$ pair, so the total work is
the number of composites — linear.

**Proof of "at least once".** Let $m$ be composite, $p = \mathrm{spf}(m)$ and $i = m/p$.

Every prime factor of $i$ is a prime factor of $m$, so it is $\ge p$ by minimality. Hence

$$\mathrm{spf}(i) \ge p$$

Now consider the outer iteration for this $i$. The inner loop walks the primes in
**increasing** order, and breaks only when the current prime exceeds $\mathrm{spf}(i)$.
Since $p \le \mathrm{spf}(i)$, the loop has not broken by the time it reaches $p$. And
$i\,p = m \le n$, so the size guard passes too. Therefore $\mathrm{spf}[m]$ is set. ✓

**Proof of "at most once".** Suppose the loop writes $m = i'\,p'$ at some point. The break
condition guarantees $p' \le \mathrm{spf}(i')$, so every prime factor of $i'$ is $\ge p'$.
Since the prime factors of $m$ are $p'$ together with those of $i'$, all of them are
$\ge p'$ — and $p'$ is itself one of them. Therefore

$$p' = \mathrm{spf}(m), \qquad i' = m/p'$$

So the pair $(i', p')$ is forced: there is only one way the loop can produce $m$.
$\blacksquare$

**Total work.** Each composite is written once and each prime is found once, so the loop
body executes $O(n)$ times.

**In code.** The `p > spf[i]` break *is* the algorithm. Remove it and you get the ordinary
sieve with extra bookkeeping. (The commonly seen `if (i % p == 0) break;` after the
assignment is the same condition written differently.)

---

## 4. Where $\log\log n$ comes from

**Claim.** The plain sieve performs $\Theta(n \log\log n)$ marking operations.

**Proof.** The pass for prime $p$ marks the multiples of $p$ up to $n$, of which there are
about $n/p$. Summing over the primes:

$$\text{total marks} \;=\; \sum_{p \le n,\ p \text{ prime}} \frac{n}{p} \;=\; n \sum_{p \le n} \frac{1}{p}$$

So everything reduces to the sum of reciprocals of primes. **Mertens' second theorem**
(cited, not proved here — it needs analytic number theory) states

$$\sum_{p \le n} \frac{1}{p} \;=\; \ln\ln n + M + o(1)$$

for a constant $M \approx 0.2615$. Hence the total is $\Theta(n\log\log n)$. $\blacksquare$

**Contrast this with the divisibility sieve.** There the inner loop ran for **every** $d$,
giving $\sum_{d\le n} n/d = n H_n = \Theta(n\log n)$. Here it runs only for **primes**,
and primes are sparse enough that the sum collapses from $\log n$ to $\log\log n$.

**In practice.** $\log\log n < 4$ for every $n$ below $10^{18}$, so the sieve is
effectively linear. A sieve to $10^7$ runs in well under a second.

---

## 5. Factorizing with `spf` costs $O(\log m)$

**Claim.** Repeatedly dividing $m$ by $\mathrm{spf}[m]$ terminates in at most $\log_2 m$
divisions and yields the full prime factorization.

**Proof.** Each division replaces $m$ by $m/p$ where $p \ge 2$, so the value at least
halves every step. Starting from $m$, after $k$ steps the value is at most $m/2^k$, which
reaches $1$ by $k = \log_2 m$.

Correctness: at each step $\mathrm{spf}[m]$ is by definition a prime dividing $m$, and
after dividing it out the table entry for the new value gives the next smallest prime
factor. Since we always remove the smallest, the primes come out in increasing order and
none is missed. $\blacksquare$

That this is *the* factorization, and not merely *a* factorization, is
[unique factorization](../unique-factorization/proofs.md) §2.

**In code.** For $m \le 10^7$ this is about $23$ operations worst case, versus $\sqrt m
\approx 3000$ for trial division. If you are going to factorize many numbers in a range,
building the `spf` table first is almost always the right move.

**What it unlocks.** With the factorization $m = p_1^{e_1}\cdots p_k^{e_k}$ in hand:

$$\tau(m) = \prod (e_i + 1), \qquad \sigma(m) = \prod \frac{p_i^{\,e_i+1}-1}{p_i - 1}$$

(both from the [divisibility](../divisibility/) concept), each in $O(\log m)$.
