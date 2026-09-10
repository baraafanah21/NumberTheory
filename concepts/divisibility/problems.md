# Divisibility — Problems

Ten problems, hardest last. Every **check value** here was computed, not guessed — if your
code prints something else, the bug is yours. Answers at the bottom.

---

## By hand

**1. Two divisor functions.** For $n = 360$: write the factorization, then give $\tau(n)$
(how many divisors) and $\sigma(n)$ (their sum). Neither needs a loop.

**2. The lockers.** $1000$ lockers, all closed. Student $k$ (for $k=1\dots1000$) toggles
every locker whose number is a multiple of $k$. How many end up **open**, and which ones?
The answer is a one-line consequence of "divisors pair up".

**3. Divisible by 6 but not by 4.** How many integers in $[1,10^6]$? Counting multiples is
$O(1)$ — do not loop, and do not subtract the wrong set.

**4. Exactly three divisors.** How many $n \le 10^6$ have $\tau(n) = 3$ exactly? Say what
such an $n$ must look like before you count.

---

## With code

**5. The reusable skeleton.** Build $\tau(m)$ for **all** $m \le 10^6$ with

```cpp
for (int d = 1; d <= n; ++d)
    for (int m = d; m <= n; m += d) tau[m] += 1;
```

Report (a) the largest $\tau(m)$ and the smallest $m$ achieving it, and (b) the total
$\sum_{m\le10^6}\tau(m)$. Then compute (b) a second way, in a **single** loop, and confirm
the two agree.

**6. Odd divisor count.** Prove $\tau(n)$ is odd exactly when $n$ is a perfect square, then
count the $n \le 10^7$ with odd $\tau(n)$ — in $O(1)$, without a sieve.

**7. Two silent failures.** (a) The loop `for (ll d = 1; d * d <= n; ++d)` is wrong for
large $n$: at roughly which $d$ does `d * d` overflow signed 64-bit, and what does the loop
do then? Rewrite it. (b) In C++, `-17 % 5` is $-2$, but the remainder from the division
algorithm is $3$. Write `properMod(a, b)` that always lands in $[0,|b|)$.

**8. The trap, concretely.** Find $d, a, b$ all below $20$ with $d \mid ab$ but
$d \nmid a$ and $d \nmid b$. What extra hypothesis on $d$ makes the implication true — and
which later theorem in this repo depends on it?

---

## Harder

**9. Why the double loop is $\Theta(n\log n)$.** Prove
$\ \sum_{m\le n}\tau(m) = \sum_{d\le n}\lfloor n/d\rfloor$, then use
$\sum_{d\le n} 1/d = \Theta(\log n)$ to bound the skeleton in problem 5. Check the identity
numerically at $n = 10^6$.

**10. Digits only.** A number arrives as a $10^5$-digit string — far too big for any
integer type. Decide divisibility by $3$, by $11$, and by $8$, each in one pass. Which of
the three does **not** need to look at every digit?

---

## Judge problems

| where | problem | what it drills |
|---|---|---|
| CF 630J · 1100 | [Divisibility](https://codeforces.com/problemset/problem/630/J) | count $m \le 10^{18}$ divisible by all of $2..10$ — one lcm, then $\lfloor n/d\rfloor$ |
| CF 26A · 900 | [Almost Prime](https://codeforces.com/problemset/problem/26/A) | count $n \le 3000$ with exactly two distinct prime divisors |
| CF 762A · 1400 | [k-th divisor](https://codeforces.com/problemset/problem/762/A) | the $O(\sqrt n)$ pair loop, and the $d = n/d$ edge case |
| CF 1512G · 1700 | [Short Task](https://codeforces.com/problemset/problem/1512/G) | the problem-5 skeleton with `sigma[m] += d` |
| LC 1952 · Easy | [Three Divisors](https://leetcode.com/problems/three-divisors/) | problem 4 as a judge task |
| LC 1492 · Med | [The kth Factor of n](https://leetcode.com/problems/the-kth-factor-of-n/) | divisors in $O(\sqrt n)$, sorted, without double-counting $\sqrt n$ |
| LC 2427 · Easy | [Number of Common Factors](https://leetcode.com/problems/number-of-common-factors/) | common divisors are the divisors of the gcd |
| LC 1201 · Med | [Ugly Number III](https://leetcode.com/problems/ugly-number-iii/) | $\lfloor n/d\rfloor$ counting plus inclusion–exclusion, under a binary search |

---

## Answers

**1.** $360 = 2^3\cdot3^2\cdot5$, so $\tau = 4\cdot3\cdot2 = 24$ and
$\sigma = \frac{2^4-1}{1}\cdot\frac{3^3-1}{2}\cdot\frac{5^2-1}{4} = 15\cdot13\cdot6 = 1170$.

**2.** Locker $m$ is toggled once per divisor of $m$, so it ends open iff $\tau(m)$ is odd
iff $m$ is a perfect square: $\lfloor\sqrt{1000}\rfloor = \mathbf{31}$ lockers, namely
$1,4,9,\dots,961$.

**3.** $\lfloor 10^6/6\rfloor - \lfloor 10^6/12\rfloor = 166666 - 83333 = \mathbf{83333}$.
The excluded set is the multiples of $\mathrm{lcm}(6,4) = 12$, **not** of $24$.

**4.** $\tau(n) = 3$ forces $n = p^2$ for a prime $p$ ($3$ is prime, so the exponent pattern
must be a single $e+1 = 3$). So count primes $p \le 1000$: $\mathbf{168}$.

**5.** Max $\tau(m) = \mathbf{240}$, first at $m = \mathbf{720720}$ (also 831600, 942480,
982800, 997920). Total $\sum\tau = \mathbf{13970034}$, which equals
$\sum_{d\le10^6}\lfloor 10^6/d\rfloor$ — the single loop of problem 9.

**6.** Divisors pair as $d \leftrightarrow n/d$, contributing $2$ each, unless $d = n/d$,
i.e. $n = d^2$. So $\tau$ is odd exactly for squares, and the count is
$\lfloor\sqrt{10^7}\rfloor = \mathbf{3162}$.

**7.** (a) `d * d` overflows once $d > \sqrt{2^{63}-1} \approx 3037000499$; the product
wraps negative, the condition stays true, and the loop runs past $n$ — or forever. Use
`d <= n / d`. (b) `((a % b) + b) % b`, with `b` replaced by `|b|` for negative moduli.

**8.** $6 \mid 4\times9$ while $6\nmid4$ and $6\nmid9$ — because $6 = 2\cdot3$ splits across
the factors. The implication needs $d$ **prime**: that is Euclid's lemma, proved in
[gcd](../gcd/), and it is exactly what makes factorization unique.

**9.** Count the pairs $(d,m)$ with $d \mid m$ and $m \le n$ in two orders: by $m$ it is
$\sum_m \tau(m)$; by $d$ it is $\sum_d \#\{\text{multiples of } d \le n\} =
\sum_d\lfloor n/d\rfloor$. The inner loop of the skeleton runs $\lfloor n/d\rfloor$ times,
so the total work is that same sum, which is $n\sum_d 1/d + O(n) = \Theta(n\log n)$.
At $n=10^6$ both sides are $13970034$ ✓.

**10.** By $3$: digit sum divisible by $3$. By $11$: alternating digit sum divisible by
$11$. By $8$: the **last three digits** only — $1000 = 8\cdot125$, so everything above them
is already a multiple of $8$. That is the one that ignores the other $10^5-3$ digits.

---

[README.md](README.md) · [proofs.md](proofs.md) · [implementation.cpp](implementation.cpp)
