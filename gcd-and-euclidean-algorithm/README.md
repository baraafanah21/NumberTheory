# GCD and the Euclidean Algorithm

**Position in the hierarchy:** the first genuinely number-theoretic concept. It turns
divisibility from a static relation into an *algorithm*.

**Depends on:** divisibility (not yet written in this repo — the four facts used are listed
below), and one result from
[arithmetic-and-geometric-progressions](../arithmetic-and-geometric-progressions/) (GP-5,
used in Property G11).

**Concepts that depend on it:** extended Euclidean algorithm → modular inverse → CRT;
Euclid's lemma → unique factorization; lcm; Farey sequences; continued fractions.

---

## 0. Prerequisites assumed

These belong to the **divisibility** concept. They are stated, not proved, here.

| | Fact | Used for |
|---|---|---|
| **D1** | **Linearity.** If $d \mid a$ and $d \mid b$, then $d \mid (ax + by)$ for all integers $x, y$. | The engine of the whole concept (Theorem 2) |
| **D2** | **Size bound.** If $d \mid a$ and $a \neq 0$, then $\lvert d \rvert \leq \lvert a \rvert$. | gcd exists (Theorem 1) |
| **D3** | **Division algorithm.** For integers $a$ and $b \neq 0$ there exist *unique* $q, r$ with $a = qb + r$ and $0 \leq r < \lvert b \rvert$. | Termination (Theorem 3) |
| **D4** | **Transitivity.** If $c \mid b$ and $b \mid a$ then $c \mid a$. | Assorted |

Everything else in this file is proved in [proofs.md](proofs.md).

---

## 1. Definition

**Common divisor.** An integer $d$ is a common divisor of $a$ and $b$ if $d \mid a$ and
$d \mid b$.

**Greatest common divisor.** For integers $a, b$ not both zero, $\gcd(a, b)$ is the largest
integer that divides both:

$$\gcd(a, b) = \max\{\, d \in \mathbb{Z} : d \mid a \text{ and } d \mid b \,\}$$

**Convention.** $\gcd(0, 0) = 0$. Every integer divides $0$, so the set above is unbounded
and the maximum does not exist; $0$ is chosen because it is the right answer under the
*second* definition below, and it makes $\gcd$ associative with no special cases.

**The better definition.** $g \geq 0$ is *the* gcd of $a$ and $b$ when

$$g \mid a, \quad g \mid b, \quad \text{and every common divisor } c \text{ satisfies } c \mid g$$

Read that last clause carefully: not "$c \leq g$" but "$c$ **divides** $g$". This is a
stronger statement, it is the one every proof actually uses, it makes $\gcd(0,0) = 0$ fall
out naturally, and it is the version that survives into rings where "largest" is
meaningless. That the two definitions agree is **Theorem 6**, and it is not obvious — it
is a consequence of Bézout's identity.

**Coprime.** $a$ and $b$ are *coprime* (or *relatively prime*) when $\gcd(a, b) = 1$:
they share no prime factor. Note this does **not** require either to be prime — $8$ and
$9$ are coprime.

**Least common multiple.** For $a, b$ not both zero, $\mathrm{lcm}(a,b)$ is the smallest
positive integer that both divide. By convention $\mathrm{lcm}(a, 0) = 0$.

---

## 2. Intuition

**The gcd is the largest tile that fits both lengths exactly.**

Take two sticks of length $a$ and $b$. A ruler of length $d$ "measures" a stick if a whole
number of copies lays out exactly. The gcd is the longest ruler that measures both.

**Euclid's idea, in one sentence:** *whatever measures both sticks also measures their
difference.*

If $d$ measures $a$ and $d$ measures $b$, lay the shorter stick against the longer and
look at the leftover piece $a - b$. Since $d$ tiles both, it tiles the leftover too. And
the reverse holds: anything measuring $b$ and $a - b$ measures $a = b + (a-b)$. So

$$\{\text{common divisors of } a, b\} = \{\text{common divisors of } b,\ a - b\}$$

The two pairs have *the same set of common divisors* — not merely the same maximum. That
is why the trick can be applied over and over without losing information, and it is why
[proofs.md](proofs.md) proves set equality rather than just equality of maxima.

**From subtraction to remainder.** Repeatedly subtracting $b$ from $a$ until you drop
below $b$ is exactly computing $a \bmod b$. So one modulo operation replaces a whole run of
subtractions:

$$\gcd(a, b) = \gcd(b,\ a \bmod b)$$

That single substitution is the difference between an algorithm that takes $O(a/b)$ steps
and one that takes $O(\log \min(a,b))$ steps.

**The geometric picture.** Tile an $a \times b$ rectangle with the largest squares you can:
repeatedly cut off $b \times b$ squares, and you are left with a $b \times (a \bmod b)$
rectangle. Recurse. The final square — the one that tiles the leftover exactly — has side
$\gcd(a, b)$.

```text
a = 12, b = 8

+-----------+-----+          +-----+-----+-----+
|           |     |          | 4x4 | 4x4 |     |
|   8 x 8   | 4x8 |   -->    +-----+-----+ ... |
|           |     |          | 4x4 | 4x4 |     |
+-----------+-----+          +-----+-----+-----+

leftover 8 x 4  ->  tiled exactly by 4 x 4 squares  ->  gcd(12, 8) = 4
```

**Why the algorithm is fast.** Every two steps, the numbers at least halve (Theorem 4).
Halving from $10^{18}$ reaches $1$ in about $60$ steps, so the Euclidean algorithm on
64-bit inputs finishes in well under a hundred divisions — regardless of how large the
inputs are.

---

## 3. Mathematical Formulation

### Core recurrence

$$
\begin{aligned}
\gcd(a, 0) &= \lvert a \rvert\\
\gcd(a, b) &= \gcd\bigl(b,\ a \bmod b\bigr) \qquad (b \neq 0)
\end{aligned}
$$

### Bézout's identity

For all integers $a, b$ there exist integers $x, y$ with

$$ax + by = \gcd(a, b)$$

and $\gcd(a,b)$ is the **smallest positive integer** expressible as $ax + by$. More
completely, the set of all such combinations is exactly the set of multiples of the gcd:

$$\{\, ax + by : x, y \in \mathbb{Z} \,\} = \gcd(a,b)\,\mathbb{Z}$$

This concept proves that $x, y$ **exist**. *Computing* them is the extended Euclidean
algorithm — the next concept.

### gcd and lcm together

$$\gcd(a,b) \cdot \mathrm{lcm}(a,b) = \lvert ab \rvert$$

### Via prime factorization

If $a = \prod_p p^{\alpha_p}$ and $b = \prod_p p^{\beta_p}$ (exponents $\geq 0$), then

$$\gcd(a,b) = \prod_p p^{\min(\alpha_p, \beta_p)}, \qquad \mathrm{lcm}(a,b) = \prod_p p^{\max(\alpha_p, \beta_p)}$$

This is the *conceptually* clearest formula and the *computationally* worst one — it
requires factoring, which is far harder than running Euclid. Use it for proofs, never for
code. Since $\min(\alpha,\beta) + \max(\alpha,\beta) = \alpha + \beta$, it also gives an
instant second proof of the gcd–lcm identity above.

---

## 4. Properties

### Basic

**G1 (Symmetry and sign).** $\gcd(a,b) = \gcd(b,a) = \gcd(\lvert a \rvert, \lvert b \rvert)$.
*Why:* $d \mid a \iff d \mid -a$, so the set of common divisors never sees the signs. The
gcd is always taken $\geq 0$.

**G2 (Base cases).** $\gcd(a, 0) = \lvert a \rvert$ and $\gcd(a, 1) = 1$.
*Why:* every integer divides $0$, so the common divisors of $a$ and $0$ are just the
divisors of $a$, the largest being $\lvert a \rvert$.

**G3 (The Euclidean step).** For $b \neq 0$,

$$\gcd(a, b) = \gcd(b,\ a \bmod b) = \gcd(a - b,\ b)$$

*Why:* by D1, any common divisor of $a$ and $b$ divides $a - qb$; and any common divisor
of $b$ and $a - qb$ divides $(a - qb) + qb = a$. The two pairs have identical common-divisor
sets. **This is the entire algorithm.**

### Structural

**G4 (Distributivity).** $\gcd(ca, cb) = \lvert c \rvert \gcd(a, b)$.
*Why it matters:* it lets you pull common factors out before recursing, and it is the
identity that makes binary GCD's "strip the shared factor of 2" step legal.

**G5 (Reduction to coprimality).** If $g = \gcd(a,b) \neq 0$ then
$\gcd\!\left(\frac{a}{g}, \frac{b}{g}\right) = 1$.
*Why it matters:* this is what "reduce a fraction to lowest terms" means, and it is the
standard way to *normalize* a pair before doing anything else.

**G6 (Bézout / universal property).** $\gcd(a,b)$ is the least positive value of
$ax + by$, and **every** common divisor of $a$ and $b$ divides $\gcd(a,b)$.
*Why it matters:* the second half is the bridge between the two definitions in §1 and is
the hypothesis of nearly every later proof. It is *strictly stronger* than "the gcd is the
biggest common divisor".

**G7 (Euclid's lemma).** If $\gcd(a, b) = 1$ and $a \mid bc$, then $a \mid c$.
In particular, for a prime $p$: $\ p \mid bc \implies p \mid b$ or $p \mid c$.
*Why it matters:* **this is the single most important consequence of Bézout.** It is what
makes prime factorization *unique* — without it, "prime" and "irreducible" come apart and
the fundamental theorem of arithmetic fails.

**G8 (gcd–lcm duality).** $\gcd(a,b)\cdot\mathrm{lcm}(a,b) = \lvert ab \rvert$, so
$\mathrm{lcm}(a,b) = \dfrac{\lvert ab \rvert}{\gcd(a,b)}$.
*Why it matters:* there is no separate "Euclidean algorithm for lcm" — you compute the gcd
and divide. Divide **first** to avoid overflow (§8).

**G9 (Associativity).** $\gcd(a, b, c) = \gcd\bigl(\gcd(a,b), c\bigr)$, and likewise for
lcm. *Why it matters:* the gcd of an array is a left fold, computable in one pass.

**G10 (Factorization form).** $\gcd$ takes $\min$ of prime exponents, $\mathrm{lcm}$ takes
$\max$. *Depends on:* unique factorization.

### Two results worth knowing

**G11 (Exponent gcd).** For an integer $a \geq 2$ and $m, n \geq 1$:

$$\gcd\bigl(a^m - 1,\ a^n - 1\bigr) = a^{\gcd(m,n)} - 1$$

*Why:* the Euclidean algorithm running on $a^m - 1, a^n - 1$ mirrors, step for step, the
*subtractive* Euclidean algorithm running on the exponents $m, n$. The link is GP-5 from
the progressions concept. Check: $\gcd(2^6 - 1,\ 2^4 - 1) = \gcd(63, 15) = 3 = 2^{\gcd(6,4)} - 1$. ✓
*This also explains Mersenne numbers:* $\gcd(2^m-1, 2^n-1) = 1$ exactly when
$\gcd(m,n) = 1$.

**G12 (Worst case is Fibonacci — Lamé's theorem).** If the Euclidean algorithm on
$a > b \geq 1$ takes $n$ division steps, then $a \geq F_{n+2}$ and $b \geq F_{n+1}$. The
worst case is exactly a pair of consecutive Fibonacci numbers, and it gives the
$O(\log \min(a,b))$ bound with the sharpest possible constant.
*Why it matters:* it tells you the algorithm's true worst input, which is the input to test
against.

---

## 5. Examples

**Euclid on $\gcd(48, 18)$:**

$$
\begin{aligned}
48 &= 2 \cdot 18 + 12\\
18 &= 1 \cdot 12 + 6\\
12 &= 2 \cdot 6 + 0 \quad \Longrightarrow \quad \gcd(48,18) = 6
\end{aligned}
$$

**Euclid on $\gcd(1071, 462)$ — watch the numbers collapse:**

$$
\begin{aligned}
1071 &= 2 \cdot 462 + 147\\
462 &= 3 \cdot 147 + 21\\
147 &= 7 \cdot 21 + 0 \quad \Longrightarrow \quad \gcd(1071,462) = 21
\end{aligned}
$$

Three steps for four-digit inputs.

**Why the remainder version matters.** $\gcd(1000000, 3)$ by *subtraction* takes about
$333{,}333$ steps; by *remainder*, two.

**Bézout, by unwinding the first example.** From $18 = 1\cdot 12 + 6$ we get
$6 = 18 - 12$, and from $48 = 2 \cdot 18 + 12$ we get $12 = 48 - 2\cdot 18$. Substituting:

$$6 = 18 - (48 - 2 \cdot 18) = 3 \cdot 18 - 1 \cdot 48$$

so $x = -1$, $y = 3$. (Doing this mechanically is the extended Euclidean algorithm.)

**Coprime but neither prime.** $\gcd(8, 9) = 1$, since $8 = 2^3$ and $9 = 3^2$ share no
prime.

**lcm via G8.** $\mathrm{lcm}(12, 18) = \frac{12 \cdot 18}{\gcd(12,18)} = \frac{216}{6} = 36$.

**G4 in action.** $\gcd(48, 18) = \gcd(6 \cdot 8,\ 6 \cdot 3) = 6\gcd(8,3) = 6 \cdot 1 = 6$. ✓

**G12 in action.** $\gcd(F_{11}, F_{10}) = \gcd(89, 55)$ takes $9$ steps, and a brute-force
search confirms no pair below $100$ takes more:

$$89 \to 55 \to 34 \to 21 \to 13 \to 8 \to 5 \to 3 \to 2 \to 1 \to 0$$

Every quotient is $1$ except the last (which is $2$), so almost no step removes more than a
single copy — the remainder version degenerates into the subtractive one, which is exactly
what makes this the worst case. Theorem 5 predicts it precisely: $9$ steps requires
$a \geq F_{11} = 89$ and $b \geq F_{10} = 55$, and this pair meets both bounds with
equality.

---

## 6. Algorithm

### 6.1 The Euclidean algorithm — $O(\log \min(a,b))$

**Idea.** Replace $(a, b)$ by $(b,\ a \bmod b)$ until the second entry is $0$; the first
entry is then the gcd.

**Steps.**

1. If $b = 0$, return $\lvert a \rvert$.
2. Otherwise compute $r = a \bmod b$.
3. Recurse on $(b, r)$.

**Why it works.** Two separate things must hold, and they are proved separately:

- *Correctness* — G3 says each step preserves the entire set of common divisors, so it
  preserves the gcd. When we stop, G2 gives the answer directly. (Theorem 2)
- *Termination* — by D3 the remainders satisfy $0 \leq r < \lvert b \rvert$, so the second
  argument strictly decreases and is bounded below by $0$. A strictly decreasing sequence
  of non-negative integers must terminate. (Theorem 3)

Beginners often prove only one of these. Neither alone is enough: an algorithm can be
correct-if-it-halts and never halt.

**Complexity.** After any two steps the value has at least halved (Theorem 4), so the
number of divisions is at most $2\log_2 \min(a,b) = O(\log \min(a,b))$. The sharp bound
(Theorem 5, Lamé) is about $\log_\varphi$, roughly $1.44 \log_2$.

- **Time:** $O(\log \min(a,b))$ division operations.
- **Space:** $O(1)$ iterative, $O(\log \min(a,b))$ recursive (though the recursion is a
  tail call and compilers usually flatten it).

### 6.2 Binary GCD (Stein's algorithm) — division-free

Useful where division is expensive relative to shifts, and instructive because it is built
entirely from G1 and G4. It rests on three identities, for $u, v > 0$:

$$
\begin{aligned}
\gcd(2u, 2v) &= 2\gcd(u,v) && \text{both even: extract the shared factor } 2\\
\gcd(2u, v) &= \gcd(u, v) && v \text{ odd: } 2 \text{ is not common, so drop it}\\
\gcd(u, v) &= \gcd\bigl(\lvert u - v \rvert,\ \min(u,v)\bigr) && \text{both odd}
\end{aligned}
$$

The third case produces an even number ($\text{odd} - \text{odd}$), so the second case
fires next and the value shrinks. Every iteration removes at least one bit.

- **Time:** $O(\log^2(\max(a,b)))$ bit operations, $O(\log \max)$ loop iterations.
- **Space:** $O(1)$.

In practice on modern CPUs with fast hardware division, plain Euclid is usually as fast or
faster for 64-bit integers; binary GCD wins for big integers and on hardware without a
divide instruction.

### 6.3 LCM — $O(\log \min(a,b))$

Compute $\gcd$, then apply G8 — but **divide before multiplying**:

$$\mathrm{lcm}(a,b) = \frac{a}{\gcd(a,b)} \cdot b$$

not $\frac{ab}{\gcd(a,b)}$. Mathematically identical; computationally not. The product
$ab$ overflows for inputs far smaller than those for which the answer itself overflows,
and $\gcd(a,b) \mid a$ guarantees the early division is exact. Handle $a = 0$ or $b = 0$
separately — the gcd is then $0$ and you would divide by zero.

### 6.4 gcd of an array — $O(n \log M)$

Fold left with G9: `g = 0; for x in v: g = gcd(g, x)`. Starting from $0$ is the natural
identity, since $\gcd(0, x) = \lvert x \rvert$.

An early exit helps a lot in practice: once $g = 1$ it can never change, so break.

**Complexity summary**

| Operation | Time | Space |
|---|---|---|
| Euclidean gcd | $O(\log \min(a,b))$ | $O(1)$ iterative |
| binary gcd | $O(\log \max(a,b))$ iterations | $O(1)$ |
| lcm | $O(\log \min(a,b))$ | $O(1)$ |
| gcd of $n$ values | $O(n \log M)$ | $O(1)$ |
| gcd via factorization | as hard as factoring | — |

---

## 7. Problem-Solving

### How to recognize it

- **Wording:** "largest number dividing all of them", "reduce to lowest terms", "can he
  reach exactly", "how often do they coincide", "split into equal groups with none left
  over".
- **Coprimality as a *condition*:** "count pairs with $\gcd = 1$", "for how many $k$ is
  $\gcd(k, n) = 1$" (that is Euler's $\varphi$ — a later concept).
- **Two periodic processes** meeting again → lcm.
- **A quantity invariant under subtracting one input from another** → the answer depends
  only on the gcd.

### Common patterns

**Reduce a fraction.** Divide both parts by $\gcd$ (G5). Normalize the sign into the
numerator so that equal fractions have equal representations — essential when using
fractions as map keys.

**gcd of differences.** *"Find every $d > 1$ such that all $a_i$ leave the same remainder
mod $d$."* Same remainder means $d \mid (a_i - a_j)$, so $d$ divides
$g = \gcd(a_2 - a_1,\ a_3 - a_1,\ \dots)$. The answer is the set of divisors of $g$. This
converts a condition about congruences into a single gcd — one of the highest-value tricks
here.

**Reachability / step problems.** With steps of size $a$ and $b$ in either direction, the
reachable positions are **exactly** the multiples of $\gcd(a,b)$ (Theorem 10, from Bézout).
So "can I measure exactly $c$ litres with $a$- and $b$-litre jugs?" is precisely
"$\gcd(a,b) \mid c$".

**Lattice points on a segment.** The number of integer points strictly between
$(x_1,y_1)$ and $(x_2,y_2)$ is $\gcd(\lvert \Delta x \rvert, \lvert \Delta y \rvert) - 1$;
counting both endpoints gives $\gcd + 1$. *Why:* the primitive direction vector is
$(\Delta x, \Delta y)/g$, and you take exactly $g$ steps along it.

**Cycles and coincidences.** Two events with periods $p$ and $q$ coincide every
$\mathrm{lcm}(p,q)$. In $[1, N]$ that is $\lfloor N / \mathrm{lcm}(p,q) \rfloor$ times —
which combines with the AP-6 counting from the progressions concept.

**Prefix gcds take few distinct values.** As you extend a prefix, the running gcd either
stays the same or at least halves, so a fixed left endpoint yields $O(\log M)$ distinct
gcd values. That turns "count subarrays whose gcd is $k$" from $O(n^2)$ into
$O(n \log M)$.

**Range gcd queries.** gcd is associative and idempotent, so a sparse table answers range
gcd in $O(\log M)$ per query with $O(n \log n \log M)$ preprocessing — overlapping
intervals are harmless.

### Typical tricks

- **Normalize first:** take absolute values, and reduce by the gcd, before comparing or
  hashing pairs.
- **Start the fold at $0$,** not at `v[0]` — it removes an edge case.
- **Break when the running gcd hits $1$.**
- **Use G11** when the problem involves $a^m - 1$ shapes: it collapses a huge-number gcd to
  a tiny exponent gcd.
- **When you see "same remainder", subtract** to eliminate the unknown remainder.

### Important observations

- $\gcd(a,b) = 1$ does **not** mean either is prime.
- The gcd of a set can be computed incrementally and never increases as you add elements.
- $\gcd(a, b) \mid \mathrm{lcm}(a,b)$ always, but pairwise coprimality is strictly stronger
  than $\gcd$ of the whole set being $1$: $\gcd(6, 10, 15) = 1$ although no two of them are
  coprime.

---

## 8. Common Mistakes

**Conceptual**

1. **Assuming $\gcd(a,b) = 1$ implies pairwise coprimality in a larger set.**
   $\gcd(6,10,15) = 1$ but every pair shares a factor. Many problems need the *pairwise*
   condition (CRT, for instance) and the two are not the same.
2. **Believing "greatest" means only "largest".** The load-bearing property is that every
   common divisor *divides* the gcd (G6). Proofs that use only "$\leq$" usually cannot be
   completed.
3. **Thinking Euclid needs $a > b$.** It does not: if $a < b$ then $a \bmod b = a$ and the
   first step simply swaps them. Adding a swap is harmless but unnecessary.
4. **Forgetting $\gcd(0,0)$.** Not "undefined and therefore ignorable" — pick the
   convention $0$ and make the code agree, or an all-zero array will surprise you.
5. **Using the prime-factorization formula as an algorithm.** Correct, and exponentially
   slower than Euclid.
6. **Confusing $\mathrm{lcm}$ with the product.** They coincide only when the inputs are
   coprime.

**Implementation**

7. **lcm overflow:** `a * b / g` overflows when `a*b` exceeds the type even if the answer
   fits. Write `a / g * b` — the division is exact because $g \mid a$.
8. **Dividing by a zero gcd.** `lcm(0, 0)` computes `0/0`. Guard it.
9. **Negative inputs.** C++ `%` takes the sign of the dividend, so `gcd(-48, 18)` can
   return a negative value if you never take absolute values. Normalize on entry.
10. **`std::abs` on `LLONG_MIN`.** Its negation is not representable; the behaviour is
    undefined. If inputs can reach the type's minimum, widen the type or handle it
    explicitly.
11. **`std::gcd` / `std::lcm` (C++17, `<numeric>`) with mixed signedness.** Passing an
    `int` and an `unsigned` gives a surprising common type. They also have UB when the
    absolute value of an argument is not representable in the result type, and `std::lcm`
    has UB if the result overflows — it does *not* protect you from mistake 7.
12. **Recursing without a base case for $b = 0$**, or writing `gcd(b, a % b)` before the
    zero check — an immediate division by zero.
13. **Folding an array starting from `v[0]` and skipping index 0**, then mishandling an
    empty array. Start from `0` and fold everything.
14. **Assuming the recursion depth is a risk.** It is $O(\log \min(a,b))$ — at most about
    $90$ frames for 64-bit inputs. Stack overflow is not a real concern here; the
    iterative form is preferable for other reasons (clarity, no call overhead).

---

## Files

- [README.md](README.md) — this file: the concept.
- [proofs.md](proofs.md) — rigorous proofs, including Bézout and Euclid's lemma.
- [implementation.cpp](implementation.cpp) — C++17 implementation with complexities.
