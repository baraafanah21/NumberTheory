# Progressions — Problems

Ten problems, hardest last. Every **check value** here was computed, not guessed — if your
code prints something else, the bug is yours. Answers at the bottom.

---

## By hand

**1. Count then sum.** For $7,\ 11,\ 15,\ \dots,\ 2023$: how many terms, and what is their
sum? Do the count *before* the sum — the sum formula needs $n$.

**2. Multiples in a range.** How many multiples of $7$ are in $[1,10^6]$, and what is their
sum? $O(1)$, no loop.

**3. The halving bug.** Someone writes the AP sum as `(n / 2) * (2*a + (n-1)*d)`. For
$a=1,\ d=1,\ n=3$ it returns $4$; the true sum is $6$. Which of the two factors is even
here, and why is it *always* one of them?

**4. Odd numbers.** Show $1+3+5+\cdots+(2k-1) = k^2$ from the AP sum formula. Then: for
which $n$ and $k$ can $n$ be written as a sum of $k$ **distinct positive odd** integers?
Two conditions — one on size, one on parity.

---

## With code

**5. Geometric sum under any modulus.** Implement $G(n) = 1 + r + \cdots + r^{\,n-1}
\bmod m$ using only $+$ and $\times$:

$$G(2k) = G(k)\bigl(1+r^{\,k}\bigr), \qquad G(n) = G(n-1) + r^{\,n-1}\ (n \text{ odd})$$

Check $G(10)$ with $r=3,\ m=2^{20}$ → $29524$. Then compute $G(10^{18})$ with
$r=3,\ m=2^{20}$. Note $\gcd(r-1,m) = 2$, so $(r-1)^{-1}$ does **not** exist and the
closed form is unusable.

**6. Multiples of 3 or 5.** Sum every integer in $[1,10^9]$ divisible by $3$ or by $5$.
Three AP sums and inclusion–exclusion. The answer is past $10^{17}$, so watch the
intermediate products too.

**7. Where it overflows.** In signed 64-bit, find the largest $n$ for which `n * (n + 1)`
does not overflow, and the largest $n$ for which the *result* $n(n+1)/2$ still fits. The
gap between the two answers is the entire reason to divide first.

**8. A divisor without factoring.** Using $\ (a-b)\mid(a^n-b^n)$, name two divisors of
$2^{91}-1$ greater than $1$ — without computing the number. (Which is the smallest prime
factor?)

---

## Harder

**9. Sum of triangular numbers.** Find a closed form for
$\ \sum_{i=1}^{n} \frac{i(i+1)}{2}$ and check it at $n=10$. It is not an AP — but it is a
difference of two things you already know.

**10. Why $O(\log^2 n)$.** Prove $G(2k) = G(k)(1+r^{k})$, then explain why `geoSumMod`
costs $O(\log^2 n)$ rather than $O(\log n)$ — and how to get it down to $O(\log n)$.

---

## Judge problems

| where | problem | what it drills |
|---|---|---|
| CF 1327A · 1100 | [Sum of Odd Integers](https://codeforces.com/problemset/problem/1327/A) | problem 4, verbatim: $k$ distinct odds need $n \ge k^2$ and $n \equiv k \pmod 2$ |
| CF 1537A · 800 | [Arithmetic Array](https://codeforces.com/problemset/problem/1537/A) | mean of an array as a sum condition |
| CF 1194A · 800 | [Remove a Progression](https://codeforces.com/problemset/problem/1194/A) | the survivors of the removal process are an AP with $d=2$ — noticing that *is* the problem |
| LC 413 · Med | [Arithmetic Slices](https://leetcode.com/problems/arithmetic-slices/) | counting APs inside an array; the run of length $L$ contributes $\binom{L-1}{2}$ |
| LC 829 · Hard | [Consecutive Numbers Sum](https://leetcode.com/problems/consecutive-numbers-sum/) | write $n$ as an AP with $d=1$ — solve $n = \frac{k(2a+k-1)}{2}$ for $k$ |
| LC 1922 · Med | [Count Good Numbers](https://leetcode.com/problems/count-good-numbers/) | $5^a4^b \bmod p$ — the fast-exponentiation half of this folder |

---

## Answers

**1.** $\frac{2023-7}{4}+1 = 505$ terms; sum $= \frac{505}{2}(7+2023) = 512575$. Dropping
the $+1$ gives $504$ terms and a wrong sum — steps are not terms.

**2.** $k = \lfloor 10^6/7\rfloor = 142857$ multiples, summing to
$7\cdot\frac{k(k+1)}{2} = 71428928571$. Well past $2^{31}$.

**3.** $n=3$ is odd, so $n-1$ is even and the bracket $2a+(n-1)d$ is even — here $4$.
`n / 2` truncates $1.5$ to $1$. In general exactly one of $n$ and $2a+(n-1)d$ is even
(if $n$ is odd, $(n-1)d$ is even and so is the whole bracket), so halve *that* one.

**4.** $a=1,\ d=2$ gives $S_k = \frac k2(2 + 2(k-1)) = k^2$. Since $k$ distinct positive
odds are at least $1,3,\dots,2k-1$, we need $n \ge k^2$; and a sum of $k$ odd numbers has
the parity of $k$, so $n \equiv k \pmod 2$. Both together are also sufficient — add $2$ to
the largest term repeatedly.

**5.** $G(10) = 29524$. $G(10^{18}) \bmod 2^{20} = \mathbf{524288} = 2^{19}$.

**6.** $233333334166666668$.

**7.** `n * (n + 1)` survives up to $n = 3037000499$; the halved result fits up to
$n = 4294967295 = 2^{32}-1$. Dividing the even factor first buys about $1.41\times$ the
range — and costs nothing.

**8.** $91 = 7\times13$, and $d \mid n \Rightarrow (2^d-1)\mid(2^n-1)$, so $2^7-1 = 127$
and $2^{13}-1 = 8191$ both divide it. $127$ is in fact the smallest prime factor.

**9.** $\sum_{i\le n}\frac{i(i+1)}{2} = \frac{n(n+1)(n+2)}{6}$; at $n=10$ that is $220$ ✓.
Fastest route: $\frac{i(i+1)}{2} = \binom{i+1}{2}$, and $\sum_i \binom{i+1}{2} =
\binom{n+2}{3}$ by the hockey-stick identity.

**10.** Split $1 + r + \cdots + r^{2k-1}$ into the first $k$ terms and the last $k$; the
last $k$ are the first $k$ each multiplied by $r^{k}$, so the total is $G(k)(1+r^{k})$.
The recursion halves $n$ each step ($O(\log n)$ levels) but every level calls `powMod` for
$r^{k}$, itself $O(\log n)$ — hence $O(\log^2 n)$. Carrying $r^{k}$ down the recursion
instead of recomputing it makes the whole thing $O(\log n)$.

---

[README.md](README.md) · [proofs.md](proofs.md) · [implementation.cpp](implementation.cpp)
