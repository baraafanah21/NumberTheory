# Divisors — Problems

Ten problems, hardest last. Every **check value** here was computed, not guessed — if your
code prints something else, the bug is yours. Answers at the bottom.

---

## By hand

**1. Both formulas.** Factor $360$, then compute $\tau(360)$ and $\sigma(360)$ from the
exponents. Do not list the divisors.

**2. The locker problem.** A hundred lockers start closed. Student $k$ toggles every
$k$-th locker, for $k = 1..100$. How many end up open, and which ones? Answer it with one
sentence about $\tau$, not by simulating.

**3. Exactly twelve.** Find the smallest positive integer with exactly $12$ divisors.
Explain how you searched the *shapes* $12 = \prod(e_i+1)$ rather than the numbers.

**4. Multiplicativity needs coprimality.** Compute $\tau(2)\,\tau(2)$ and $\tau(4)$, and
$\tau(2)\,\tau(3)$ and $\tau(6)$. Which pair agrees, which does not, and what exactly is
the hypothesis being violated?

---

## With code

**5. Sum over a range.** Compute $\sum_{m=1}^{10^6} \tau(m)$. Then do it a second way in
$O(\sqrt n)$ with no sieve at all, using $\sum_{m\le n}\tau(m) = \sum_{d\le n}\lfloor
n/d\rfloor$, and confirm the two agree.

**6. Exactly four divisors.** Count the $n \le 10^6$ with $\tau(n) = 4$. Then say which two
factorization shapes produce that, and check your count of $\tau(n)=2$ against the number
of primes below $10^6$.

**7. Perfect numbers.** Find every $n < 10^4$ with $\sigma(n) = 2n$. Report them, and say
what happens to an `int` accumulator if you try the same search to $10^7$.

**8. The record holder.** Find the $n \le 10^9$ with the most divisors. Brute force is
hopeless — search over exponent patterns $2^{e_1}3^{e_2}5^{e_3}\cdots$ with
$e_1 \ge e_2 \ge \cdots$ instead, and say why that restriction loses nothing.

---

## Harder

**9. Why the exponents descend.** In problem 8 you assumed the optimal $n$ uses the
smallest primes with non-increasing exponents. Prove both halves: that swapping a larger
prime's exponent onto a smaller prime never increases $n$ while keeping $\tau$ fixed, and
that skipping a prime is never optimal.

**10. Counting by lcm.** How many **ordered** pairs $(a,b)$ satisfy
$\mathrm{lcm}(a,b) = n$? Derive a formula from the exponents of $n$, check it against a
brute-force count for $n = 36$, and say why the answer is a product over primes at all.

---

## Judge problems

| where | problem | what it drills |
|---|---|---|
| CF 762A · 1400 | [k-th divisor](https://codeforces.com/problemset/problem/762/A) | the $O(\sqrt n)$ pair loop, sorted, with the $d = n/d$ edge case |
| CF 1512G · 1700 | [Short Task](https://codeforces.com/problemset/problem/1512/G) | the $O(n\log n)$ skeleton with `sigma[m] += d`, then smallest-preimage bookkeeping |
| CF 26A · 900 | [Almost Prime](https://codeforces.com/problemset/problem/26/A) | counting distinct prime divisors over a range with the same skeleton |
| CF 1165D · 1600 | [Almost All Divisors](https://codeforces.com/problemset/problem/1165/D) | reconstruct $n$ from its divisor list — pure $d \leftrightarrow n/d$ pairing |
| LC 1952 · Easy | [Three Divisors](https://leetcode.com/problems/three-divisors/) | $\tau(n)=3$ means $n=p^2$, which is problem 3's reasoning in miniature |
| LC 1492 · Med | [The kth Factor of n](https://leetcode.com/problems/the-kth-factor-of-n/) | divisors in $O(\sqrt n)$ without double-counting the square root |
| LC 2427 · Easy | [Number of Common Factors](https://leetcode.com/problems/number-of-common-factors/) | common divisors are exactly the divisors of the gcd |
| LC 1390 · Med | [Four Divisors](https://leetcode.com/problems/four-divisors/) | problem 6 as a judge task, and it wants $\sigma$ of those $n$ too |
| LC 507 · Easy | [Perfect Number](https://leetcode.com/problems/perfect-number/) | problem 7, one value at a time |

---

## Answers

**1.** $360 = 2^3\cdot3^2\cdot5$. So

$$\tau(360) = 4\cdot3\cdot2 = \mathbf{24}, \qquad
\sigma(360) = (1{+}2{+}4{+}8)(1{+}3{+}9)(1{+}5) = 15\cdot13\cdot6 = \mathbf{1170}$$

**2.** Locker $n$ is toggled once per divisor of $n$, so it ends open exactly when
$\tau(n)$ is **odd**, which happens exactly when $n$ is a perfect square. Below $100$ there
are $\mathbf{10}$: $1, 4, 9, 16, 25, 36, 49, 64, 81, 100$.

**3.** $\mathbf{60}$. The shapes of $12$ as a product of $(e_i+1)$ are $12$, $6\cdot2$,
$4\cdot3$, $3\cdot2\cdot2$, $2\cdot2\cdot3$. Assign the **largest exponent to the smallest
prime** and compare:

| shape | smallest $n$ |
|---|---|
| $2^{11}$ | $2048$ |
| $2^5\cdot3$ | $96$ |
| $2^3\cdot3^2$ | $72$ |
| $2^2\cdot3\cdot5$ | $\mathbf{60}$ |

Five candidates instead of sixty numbers. This is how every "exactly $k$ divisors" problem
should be attacked.

**4.** $\tau(2)\tau(2) = 4$ but $\tau(4) = 3$ — **disagree**, because $\gcd(2,2) = 2 \neq
1$. $\tau(2)\tau(3) = 4$ and $\tau(6) = 4$ — **agree**, since $\gcd(2,3)=1$. The hypothesis
is coprimality, and it is not decoration: with a shared prime the exponents add rather than
staying independent, so the counts multiply wrongly.

**5.** $\sum_{m\le10^6}\tau(m) = \mathbf{13970034}$. The second route is the better one to
remember: $\tau(m)$ counts pairs $(d, m)$ with $d \mid m$, so summing over $m$ counts every
such pair once, and grouping by $d$ gives $\sum_d \lfloor n/d\rfloor$. Using the fact that
$\lfloor n/d\rfloor$ takes only $O(\sqrt n)$ distinct values, that sum evaluates in
$O(\sqrt n)$ with **no array at all** — the divisor-block trick, and it scales to
$n = 10^{12}$ where the sieve cannot go.

**6.** $\mathbf{209892}$ numbers below $10^6$ have exactly four divisors. The two shapes are
$p^3$ and $p\,q$ with $p \neq q$, since $4 = 4$ or $4 = 2\cdot2$. As a cross-check,
$\tau(n) = 2$ gives $\mathbf{78498}$, which is exactly $\pi(10^6)$, the prime count — a free
correctness test on your table.

**7.** $\mathbf{6, 28, 496, 8128}$. All four are even and of Euclid's form
$2^{k-1}(2^k-1)$ with $2^k-1$ prime; no odd perfect number is known to exist. Pushing to
$10^7$ with an `int` accumulator **overflows** — $\sigma$ of a highly composite number near
$10^7$ passes $3\times10^7$ comfortably, and the running sieve sums far more than that
before any single entry is finished. Use `long long`.

**8.** $\mathbf{735134400}$, with $\tau = \mathbf{1344}$. Its factorization is

$$735134400 = 2^6\cdot3^3\cdot5^2\cdot7\cdot11\cdot13\cdot17$$

Searching exponent patterns means at most a few thousand candidates rather than $10^9$
numbers. For scale, the record below $10^6$ is $720720$ with $\tau = 240$, and below
$10^{18}$ it is $897612484786617600$ with $\tau = 103680$.

**9.** *Exponents descend.* Suppose $p < q$ carry exponents $e_p < e_q$. Swapping them
keeps $\tau = \prod(e_i+1)$ identical, and changes $n$ by the factor

$$\frac{p^{e_q}q^{e_p}}{p^{e_p}q^{e_q}} = \Bigl(\frac pq\Bigr)^{e_q - e_p} < 1$$

since $p<q$ and $e_q > e_p$. So the swapped number is strictly smaller with the same divisor
count, and the original was not optimal. $\blacksquare$

*No gaps.* If $n$ uses prime $q$ but skips a smaller prime $p$, replace every factor of $q$
with $p$. Again $\tau$ is unchanged and $n$ shrinks by $(p/q)^{e} < 1$. $\blacksquare$

Together these cut the search space to non-increasing exponent patterns over the first
$\sim15$ primes, because $2\cdot3\cdots47$ already passes $10^{18}$.

**10.** $\mathbf{\prod_i (2e_i + 1)}$. Work one prime at a time. If $n$ has $p^{e}$, then
$a$ contributes $p^{\alpha}$ and $b$ contributes $p^{\beta}$ with
$\max(\alpha,\beta) = e$ — that is the definition of lcm, exponent-wise. The pairs with
$\max = e$ are: $\alpha = e$ with $\beta \in 0..e$, or $\beta = e$ with $\alpha \in 0..e$,
minus the doubly-counted $(e,e)$, giving $2(e+1) - 1 = 2e+1$.

The answer is a **product** because the primes are independent: by
[unique factorization](../prime-factorization/) choosing the exponents of $a$ and $b$ at
each prime is a free, separate choice.

For $n = 36 = 2^2\cdot3^2$ this gives $5\cdot5 = \mathbf{25}$, which matches a brute-force
count over all $(a,b)$ with $a,b \le 36$ ✓.

---

[README.md](README.md) · [implementation.cpp](implementation.cpp)
