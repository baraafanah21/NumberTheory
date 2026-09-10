# Extended Euclidean Algorithm — Problems

Ten problems, hardest last. Every **check value** here was computed, not guessed — if your
code prints something else, the bug is yours. Answers at the bottom.

---

## By hand

**1. Run it.** Compute $\gcd(240,46)$ and Bézout coefficients $x,y$ with
$240x + 46y = \gcd$. Verify by substituting. Is your $x$ positive?

**2. Scale to the target.** Solve $6x + 10y = 8$: give one solution, then the full family,
then the solution with the **smallest non-negative $x$**.

**3. Not everything is solvable.** Show $6x + 10y = 9$ has no integer solution, in one
line.

**4. All solutions.** Solve $21x + 14y = 35$ completely. What are the two step sizes, and
why is neither of them $21$ or $14$?

---

## With code

**5. Bigger coefficients.** Run `extgcd(1234, 4321)`. Report $g$, $x$, $y$, and then the
smallest **positive** $x$ solving $1234x + 4321y = 1$. Check that $1234x \equiv 1
\pmod{4321}$.

**6. The stepping bug.** For $6x+10y=8$ you have $x_0 = 8$. Step by $b = 10$ instead of
$b/g = 5$ and list the $x$ values you can reach. Which correct solution is invisible to
you, and what does that do to "smallest non-negative $x$"?

**7. An inverse, the hard way.** Use `extgcd` to compute $17^{-1} \bmod 3120$ — the modulus
is composite, so Fermat is unavailable. Both normalizations in `modInverse` matter here;
say which line each one fixes.

**8. Coin amounts.** With only $6$- and $11$-unit coins (non-negative counts), what is the
largest amount you **cannot** make? Verify your answer and the six amounts above it by
brute force. Why does the same question have no finite answer for $6$ and $10$?

---

## Harder

**9. Derive, don't memorize.** The recursive call returns $x_1,y_1$ with
$b\,x_1 + (a \bmod b)\,y_1 = g$. Derive the two update lines from it, using
$a \bmod b = a - \lfloor a/b\rfloor b$. Then predict what breaks if you swap them.

**10. Why nothing overflows.** Prove the coefficients stay small — for $a > b > 0$ the
returned $x,y$ satisfy $|x| \le \frac{b}{2g}$ and $|y| \le \frac{a}{2g}$. Conclude that
`extgcd` never overflows on inputs that themselves fit, even though `solveDiophantine`
easily can.

---

## Judge problems

Extended Euclid rarely headlines a problem — it usually hides inside a modular inverse or a
CRT merge. These are the ones where it *is* the problem:

| where | problem | what it drills |
|---|---|---|
| CF 7C · 1800 | [Line](https://codeforces.com/problemset/problem/7/C) | $Ax+By+C=0$ in integers: solvability is $\gcd(A,B) \mid C$, then scale — problems 2 and 3 at $2\cdot10^9$ scale |
| LC 365 · Med | [Water and Jug Problem](https://leetcode.com/problems/water-and-jug-problem/) | Bézout as a reachability test |
| LC 1250 · Hard | [Check If It Is a Good Array](https://leetcode.com/problems/check-if-it-is-a-good-array/) | Bézout for $n$ numbers — the reachable set is the multiples of the whole array's gcd |
| LC 2543 · Hard | [Check if Point Is Reachable](https://leetcode.com/problems/check-if-point-is-reachable/) | run the moves backwards; the invariant is the gcd, and it must be a power of two |

---

## Answers

**1.** $g = 2$, $x = -9$, $y = 47$: $\ 240(-9) + 46(47) = -2160 + 2162 = 2$ ✓. Yes, $x$
came back **negative** — normal, and the reason `modInverse` normalizes.

**2.** $g = \gcd(6,10) = 2 \mid 8$, so it is solvable. `extgcd` gives $6(2)+10(-1) = 2$;
scaling by $c/g = 4$: $(x_0,y_0) = (8,-4)$. All solutions:

$$x = 8 + 5k, \qquad y = -4 - 3k$$

Smallest non-negative $x$ is $\mathbf{3}$ (at $k=-1$), with $y = -1$: $18 - 10 = 8$ ✓.

**3.** $\gcd(6,10) = 2$ divides both $6x$ and $10y$, so it divides the left side — but
$2 \nmid 9$.

**4.** $g = 7 \mid 35$. From $21(1) + 14(-1) = 7$, scaling by $5$ gives $(5,-5)$. The steps
are $b/g = 2$ and $a/g = 3$:

$$x = 5 + 2k, \qquad y = -5 - 3k$$

Stepping by $14$ and $21$ would visit only every 7th solution.

**5.** $g = 1$, $x = -1082$, $y = 309$ (check: $1234(-1082) + 4321(309) = 1$). The smallest
positive $x$ is $-1082 + 4321 = \mathbf{3239}$, and $1234\cdot3239 \equiv 1 \pmod{4321}$ ✓.

**6.** Stepping by $10$ from $8$ reaches $\dots, 18, 8, -2, \dots$ — it skips $3$ and $13$
entirely, because with $g = 2$ the true spacing is $5$. "Smallest non-negative $x$" then
answers $8$ instead of $3$: half the solutions are invisible, and the bug is silent.

**7.** $17^{-1} \equiv \mathbf{2753} \pmod{3120}$ ($17\cdot2753 = 46801 = 15\cdot3120+1$).
The input normalization `((a % m) + m) % m` handles a negative or oversized `a`; the output
normalization `((x % m) + m) % m` handles `extgcd` returning a negative coefficient — here
it returns $-367$, and $-367 + 3120 = 2753$.

**8.** $\mathbf{49}$ — the Frobenius number $ab - a - b = 66 - 17$. Every amount from $50$
up is representable. For $6$ and $10$ there is no answer: only multiples of
$\gcd(6,10) = 2$ are reachable at all, so infinitely many amounts (every odd one) fail.
Coprimality is what makes the question finite.

**9.** Substituting $a \bmod b = a - \lfloor a/b\rfloor b$ into
$b\,x_1 + (a \bmod b)\,y_1 = g$ gives

$$b\,x_1 + \Bigl(a - \Bigl\lfloor\tfrac ab\Bigr\rfloor b\Bigr) y_1
= a\,y_1 + b\Bigl(x_1 - \Bigl\lfloor\tfrac ab\Bigr\rfloor y_1\Bigr) = g$$

so $x = y_1$ and $y = x_1 - \lfloor a/b\rfloor y_1$ — the quotient attaches to $y$, never
to $x$. Swap them and the identity $ax+by=g$ simply stops holding; the returned $g$ is
still right, which is what makes the mistake survive casual testing.

**10.** Induct on the recursion. Base case $b = 0$ gives $(1,0)$, within the bound. For the
step, the recursive call on $(b,\ a \bmod b)$ returns $x_1,y_1$ bounded by
$\frac{a \bmod b}{2g}$ and $\frac{b}{2g}$; the updates $x = y_1$ and
$y = x_1 - \lfloor a/b\rfloor y_1$ then keep $|x| \le \frac{b}{2g}$ and
$|y| \le \frac{a}{2g}$, since $\lfloor a/b\rfloor (a \bmod b) < a$. So the coefficients
never exceed the inputs — but `solveDiophantine` multiplies them by $c/g$, and *that*
product is where overflow lives.

---

[README.md](README.md) · [proofs.md](proofs.md) · [implementation.cpp](implementation.cpp)
