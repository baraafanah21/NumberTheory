# Unique Factorization — Problems

Ten problems, hardest last. Every **check value** here was computed, not guessed — if your
code prints something else, the bug is yours. Answers at the bottom.

---

## By hand

**1. One factorization, three functions.** Factor $5040$, then read off $\tau$, $\sigma$
and $\varphi$ from the exponents alone — no loops over divisors.

**2. Legendre's formula.** What is the exponent of $3$ in $100!$? And how many trailing
zeros does $100!$ have? Do not compute $100!$; count how many factors each prime
contributes:

$$v_p(n!) = \sum_{i\ge1}\Bigl\lfloor \frac{n}{p^{\,i}}\Bigr\rfloor$$

**3. Perfect powers.** Is $2^{30}$ a perfect fifth power? State the exponent rule, then
give the base.

**4. Exponent-wise gcd and lcm.** With $a = 2^3\cdot3^5\cdot7$ and $b = 2^5\cdot3^2\cdot5$,
write $\gcd$ and $\mathrm{lcm}$ from the exponents, and verify
$\gcd\cdot\mathrm{lcm} = ab$. Then explain in one line why the identity is *obvious* in
this language.

---

## With code

**5. The line people forget.** Delete `if (n > 1) f.push_back({n, 1});` from `factorize`
and run it on $26$. What comes back? Characterize **exactly** which $n$ the deletion
breaks, and which it does not.

**6. Smallest with 100 divisors.** Find the smallest $n$ with exactly $100$ divisors.
Brute-forcing $\tau$ upward works; better, reason about which exponent patterns multiply
to $100$ and assign the big exponents to the small primes.

**7. Counting by lcm.** How many **ordered** pairs $(a,b)$ satisfy
$\mathrm{lcm}(a,b) = 72$? Get it by brute force, then find the formula in terms of the
exponents of $72$ and check the two agree.

**8. Squarefree.** Count the squarefree integers in $[1,100]$ (no repeated prime factor).
Then say how you would count them up to $10^{12}$, where enumerating is hopeless.

---

## Harder

**9. Uniqueness really is needed.** In the "even numbers only" system from the README,
find a second number with two genuinely different factorizations into that system's
primes, and identify which step of the usual uniqueness proof fails for it.

**10. $\sqrt2$, and a divisor count.** Prove $\sqrt2$ is irrational by comparing the parity
of the exponent of $2$ on each side of $a^2 = 2b^2$ — a proof that only makes sense once
factorization is unique. Then, in one line: how many divisors does $10^{18}$ have?

---

## Judge problems

| where | problem | what it drills |
|---|---|---|
| CF 1114C · 1700 | [Trailing Loves (or L'oeufs?)](https://codeforces.com/problemset/problem/1114/C) | problem 2 in an arbitrary base — factor $b$, apply Legendre per prime, take the min |
| CF 1165D · 1600 | [Almost All Divisors](https://codeforces.com/problemset/problem/1165/D) | reconstruct $n$ from its divisor list — pairing $d \leftrightarrow n/d$ |
| CF 1228C · 1700 | [Primes and Multiplication](https://codeforces.com/problemset/problem/1228/C) | exponent counting over a whole range, mod $10^9+7$ |
| CF 1225D · 1800 | [Power Products](https://codeforces.com/problemset/problem/1225/D) | reduce every exponent mod $k$ — pairs match iff the residue vectors are complementary |
| LC 172 · Med | [Factorial Trailing Zeroes](https://leetcode.com/problems/factorial-trailing-zeroes/) | problem 2 as a judge task |
| LC 2521 · Med | [Distinct Prime Factors of Product of Array](https://leetcode.com/problems/distinct-prime-factors-of-product-of-array/) | the union of prime supports — no products needed |
| LC 1735 · Hard | [Count Ways to Make Array With Product](https://leetcode.com/problems/count-ways-to-make-array-with-product/) | distribute each prime's exponent independently: stars and bars per prime |
| LC 2572 · Med | [Count the Number of Square-Free Subsets](https://leetcode.com/problems/count-the-number-of-square-free-subsets/) | problem 8 plus a bitmask over the primes below 30 |

---

## Answers

**1.** $5040 = 2^4\cdot3^2\cdot5\cdot7$. So $\tau = 5\cdot3\cdot2\cdot2 = \mathbf{60}$;
$\sigma = 31\cdot13\cdot6\cdot8 = \mathbf{19344}$; and
$\varphi = 5040\cdot\frac12\cdot\frac23\cdot\frac45\cdot\frac67 = \mathbf{1152}$.

**2.** $v_3(100!) = 33+11+3+1 = \mathbf{48}$. Trailing zeros are governed by the scarcer
of $2$ and $5$, which is $5$: $v_5(100!) = 20+4 = \mathbf{24}$.

**3.** $n$ is a perfect $k$-th power iff every exponent is divisible by $k$. Here
$30 = 5\cdot6$, so yes: $2^{30} = (2^6)^5 = \mathbf{64^5}$.

**4.** $\gcd = 2^3\cdot3^2 = \mathbf{72}$, $\mathrm{lcm} = 2^5\cdot3^5\cdot5\cdot7 =
\mathbf{272160}$, and $72\cdot272160 = 13608\cdot1440$ ✓. Obvious because
$\min(\alpha,\beta) + \max(\alpha,\beta) = \alpha+\beta$ holds prime by prime.

**5.** Without the line, `factorize(26)` returns just $\{(2,1)\}$ — the factor $13$ is
gone, and the product no longer reconstructs $26$. The loop only reaches $p \le \sqrt n$,
so it breaks exactly for $n$ with a prime factor **greater than $\sqrt n$** (which after
the loop is whatever is left in `n`). There is at most one such factor, which is why one
trailing `if` suffices.

**6.** $\mathbf{45360} = 2^4\cdot3^4\cdot5\cdot7$, with
$\tau = 5\cdot5\cdot2\cdot2 = 100$ ✓. The exponent pattern $(4,4,1,1)$ beats alternatives
like $(24,3)$ or $(9,4,1)$ because large exponents belong on the smallest primes.

**7.** $\mathbf{35}$. For $n = \prod p_i^{e_i}$, an ordered pair with $\mathrm{lcm} = n$
picks exponents $\alpha_i,\beta_i \le e_i$ with $\max = e_i$, which can be done in
$2e_i+1$ ways. For $72 = 2^3\cdot3^2$: $7\cdot5 = 35$.

**8.** $\mathbf{61}$ squarefree integers up to $100$. To reach $10^{12}$, count by
inclusion–exclusion over squares: $\sum_{d\le10^6}\mu(d)\lfloor n/d^2\rfloor$ — the
squarefree numbers are what is left after removing multiples of $4, 9, 25, \dots$

**9.** $36 = 2\times18 = 6\times6$: in the even-only system $2$, $6$ and $18$ are all
"prime" ($18 = 2\times9$ does not count, as $9$ is odd). The step that fails is
**Euclid's lemma** — $6 \mid 2\times18$, yet $6$ divides neither factor within the system.
Existence of a factorization is fine; only uniqueness collapses.

**10.** Suppose $\sqrt2 = a/b$ in lowest terms, so $a^2 = 2b^2$. The exponent of $2$ in a
square is even (double whatever it is in the number), so the left side has an even
exponent of $2$ and the right side an odd one — impossible, since unique factorization
makes "the exponent of $2$" a single well-defined number. And
$10^{18} = 2^{18}5^{18}$ has $19\cdot19 = \mathbf{361}$ divisors.

---

[README.md](README.md) · [proofs.md](proofs.md) · [implementation.cpp](implementation.cpp)
