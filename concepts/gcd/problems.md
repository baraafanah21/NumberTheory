# GCD and the Euclidean Algorithm — Problems

Ten problems, hardest last. Every **check value** here was computed, not guessed — if your
code prints something else, the bug is yours. Answers at the bottom.

---

## By hand

**1. Run the algorithm.** Compute $\gcd(1071, 462)$ by writing out every division step
$a = qb + r$. How many steps does it take?

**2. lcm without overflow.** Compute $\mathrm{lcm}(48,180)$. Then take
$a = b = 10^{18}$: the true lcm is $10^{18}$ and fits comfortably in 64 bits, yet one of
the two orderings `a * b / g` and `(a / g) * b` still fails. Which, and why is the other
one exact?

**3. Two jugs.** With a $6$-litre and a $10$-litre jug (fill, empty, pour between), can you
measure exactly $8$ litres? Exactly $9$? State the rule you used, not just the answers.

**4. Same remainder.** For which integers $d > 1$ do $27$, $39$ and $51$ all leave the
**same** remainder mod $d$? Turn the condition on remainders into one gcd.

---

## With code

**5. Coprime pairs.** Count the ordered pairs $(a,b)$ with $1 \le a,b \le 10$ and
$\gcd(a,b)=1$. Brute force is fine here — keep the number, you will meet it again in
[Euler's totient](../euler-totient-function/).

**6. Exponent gcd.** Compute $\gcd(2^{100}-1,\ 2^{60}-1)$ — the numbers have 31 and 19
digits, so do it *without* big integers. Guess the pattern from small cases first.

**7. Fibonacci gcd.** Verify $\gcd(F_{12}, F_{18}) = F_{\gcd(12,18)}$ numerically
($F_1 = F_2 = 1$). Then check the identity for five more pairs.

**8. Worst case.** Which inputs make Euclid take the most steps for their size? Find the
smallest pair $(a,b)$ with $a > b$ needing exactly $10$ division steps, and say what the
sequence of such pairs is.

---

## Harder

**9. Folding an array.** Explain why `g = 0; for (x : v) g = gcd(g, x);` needs no
special case for the first element, and what it returns for an all-zero array. Then: what
does $\gcd$ of the whole array being $1$ tell you about *pairs* in the array — and what
does it not?

**10. The product identity.** Prove $\gcd(a,b)\cdot\mathrm{lcm}(a,b) = ab$ for positive
$a,b$. Two routes: exponent-wise via $\min + \max = x + y$ (needs
[unique factorization](../unique-factorization/)), or directly from the definitions
(needs only Bézout). Do the second.

---

## Judge problems

| where | problem | what it drills |
|---|---|---|
| CF 664A · 800 | [Complicated GCD](https://codeforces.com/problemset/problem/664/A) | $\gcd$ of every integer in $[a,b]$ — consecutive integers are coprime |
| CF 1325A · 800 | [EhAb AnD gCd](https://codeforces.com/problemset/problem/1325/A) | construct $a,b$ with $\gcd + \mathrm{lcm}$ given |
| CF 876B · 1300 | [Divisiblity of Differences](https://codeforces.com/problemset/problem/876/B) | problem 4's pattern: equal differences mod $m$ means bucket by remainder |
| CF 1458A · 1600 | [Row GCD](https://codeforces.com/problemset/problem/1458/A) | $\gcd(a_1+b_j,\dots,a_n+b_j) = \gcd(a_1+b_j,\ a_2-a_1,\ \dots)$ — gcd of differences again |
| CF 2034A · 800 | [King Keykhosrow's Mystery](https://codeforces.com/problemset/problem/2034/A) | smallest $m$ with $m \bmod a = m \bmod b$ |
| LC 365 · Med | [Water and Jug Problem](https://leetcode.com/problems/water-and-jug-problem/) | problem 3 as a judge task — pure Bézout |
| LC 1071 · Easy | [Greatest Common Divisor of Strings](https://leetcode.com/problems/greatest-common-divisor-of-strings/) | Euclid transplanted to strings; the answer's length is $\gcd$ of the lengths |
| LC 914 · Easy | [X of a Kind in a Deck of Cards](https://leetcode.com/problems/x-of-a-kind-in-a-deck-of-cards/) | fold $\gcd$ over counts, then test $\ge 2$ |
| LC 1447 · Med | [Simplified Fractions](https://leetcode.com/problems/simplified-fractions/) | reduce before comparing — problem 5 in disguise |

---

## Answers

**1.** Three steps: $1071 = 2\cdot462 + 147$, $462 = 3\cdot147 + 21$, $147 = 7\cdot21 + 0$.
So $\gcd = \mathbf{21}$.

**2.** $\gcd(48,180) = 12$, so $\mathrm{lcm} = \frac{48}{12}\cdot180 = \mathbf{720}$. With
$a=b=10^{18}$: $g = 10^{18}$, and `a * b` is $10^{36}$ — overflow — while
`(a / g) * b = 1 * 10^18` is exact. Dividing first is always safe because $g \mid a$, so
the division leaves no remainder.

**3.** Reachable amounts are exactly the multiples of $\gcd(6,10) = 2$. So $8$: **yes**;
$9$: **no**.

**4.** Equal remainders means $d$ divides every difference, so $d \mid \gcd(39-27,\
51-27) = \gcd(12,24) = 12$. The answers are the divisors of $12$ above $1$:
$\mathbf{2, 3, 4, 6, 12}$.

**5.** $\mathbf{63}$ of the $100$ ordered pairs. (It is $2\sum_{k\le10}\varphi(k) - 1$ —
which is why the count reappears in the totient folder.)

**6.** $\gcd(2^m-1,\ 2^n-1) = 2^{\gcd(m,n)}-1$, so the answer is
$2^{20}-1 = \mathbf{1048575}$.

**7.** $F_{12} = 144$, $F_{18} = 2584$, $\gcd = \mathbf{8} = F_6$ ✓, and
$\gcd(12,18) = 6$.

**8.** Consecutive Fibonacci numbers — they are the worst case, since every quotient is
$1$, the slowest possible descent. Ten steps first happens at
$(a,b) = \mathbf{(144, 89)}$. This is Lamé's theorem: the step count is $O(\log\min(a,b))$,
under $90$ for any 64-bit input.

**9.** $\gcd(0,x) = |x|$, so starting at $0$ makes the first iteration return $|v_0|$ —
the identity element does the special-casing for you. An all-zero array returns $0$
(the convention $\gcd(0,0)=0$). A whole-array gcd of $1$ says nothing about pairs:
$\gcd(6,10,15) = 1$ while $\gcd(6,10)=2$, $\gcd(6,15)=3$, $\gcd(10,15)=5$ — no pair is
coprime. **Pairwise** coprimality is a strictly stronger condition, and it is the one
[CRT](../chinese-remainder-theorem/) needs.

**10.** Let $g = \gcd(a,b)$ and $\ell = ab/g$. First, $\ell$ **is** a common multiple:
$\ell = (a/g)\,b = a\,(b/g)$, and both $a/g$ and $b/g$ are integers. Now let $m$ be *any*
common multiple. Bézout gives $g = au+bv$; multiply through by $\frac{m}{ab}$:

$$\frac m\ell \;=\; \frac{mg}{ab} \;=\; \frac{m(au+bv)}{ab} \;=\; \frac mb\,u + \frac ma\,v$$

Both $m/a$ and $m/b$ are integers, so $m/\ell$ is one too — that is, $\ell \mid m$. So
$\ell$ divides **every** common multiple and is therefore the least, giving
$g\,\ell = ab$. Note where Bézout was needed: without it, $\ell$ is only *a* common
multiple, with no reason to be the smallest.

---

[README.md](README.md) · [proofs.md](proofs.md) · [implementation.cpp](implementation.cpp)
