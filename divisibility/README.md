# Divisibility

**Position in the hierarchy:** the root. Everything in number theory is built on this
relation.

**Depends on:** nothing but integer arithmetic. Two *examples* borrow results from
[arithmetic-and-geometric-progressions](../arithmetic-and-geometric-progressions/) (GP-5,
for the divisibility tests, and AP-6, for counting multiples), but no core result here
needs them.

**Concepts that depend on it:** everything. Immediately,
[gcd-and-euclidean-algorithm](../gcd-and-euclidean-algorithm/) uses D1–D4 below as its
stated prerequisites.

---

## 1. Definition

Let $a, b \in \mathbb{Z}$. We say **$a$ divides $b$**, written $a \mid b$, if there exists
an integer $k$ with

$$b = ak$$

We then call $a$ a **divisor** (or **factor**) of $b$, and $b$ a **multiple** of $a$. The
negation is written $a \nmid b$.

**The single most important observation about this definition:** it is stated with
*multiplication*, not division. Nothing is divided, no fraction appears, and the question
"is $b/a$ an integer?" never has to be asked. That is what makes divisibility usable in
proofs — you get to produce a witness $k$ and multiply, rather than reason about quotients.

**Zero, handled once and for all.** These trip everyone up, and all three follow directly
from the definition:

| Statement | True? | Witness / reason |
|---|---|---|
| $a \mid 0$ for every $a$ | **true** | $0 = a \cdot 0$, take $k = 0$ |
| $0 \mid 0$ | **true** | $0 = 0 \cdot k$ for any $k$ |
| $0 \mid b$ for $b \neq 0$ | **false** | $0 \cdot k = 0 \neq b$ always |

So $0$ is divisible by everything, and divides only itself. Note that $0 \mid 0$ being
*true* does **not** make $0/0$ meaningful — another reminder that $\mid$ is not division.

**Vocabulary.** The **trivial divisors** of $n$ are $\pm 1$ and $\pm n$. A **proper
divisor** of $n$ is a positive divisor other than $n$ itself. For $n \geq 1$ we write

$$\tau(n) = \#\{\, d \geq 1 : d \mid n \,\}, \qquad \sigma(n) = \sum_{d \geq 1,\ d \mid n} d$$

for the number and the sum of the positive divisors. ($\tau$ is also written $d(n)$.)

---

## 2. Intuition

**Tiling.** $a \mid b$ means a stick of length $a$ tiles a stick of length $b$ exactly —
a whole number of copies, nothing left over, nothing overhanging.

```text
a = 3, b = 12:   [---][---][---][---]          exactly 4 tiles   ->  3 | 12
a = 5, b = 12:   [-----][-----][--]            2 tiles + leftover 2  ->  5 | 12 is false
```

**The number line.** The multiples of $a$ are evenly spaced marks. $a \mid b$ asks whether
$b$ lands exactly on a mark. This makes the zero cases obvious: with $a = 0$ every "step"
stays at the origin, so the only reachable point is $0$ — which is precisely
"$0 \mid b$ iff $b = 0$".

**What the remainder is for.** When the tiling *fails*, the leftover piece measures the
failure. That leftover is the remainder, and the statement that it always exists, is
unique, and is smaller than $a$ is the **division algorithm** — the most useful single
fact in this file. Divisibility is then the special case "the leftover is zero":

$$a \mid b \iff (b \bmod a) = 0$$

**Divisibility is an ordering, but a partial one.** On the positive integers, $\mid$
behaves like $\leq$: reflexive ($a \mid a$), transitive, and antisymmetric. But unlike
$\leq$ it does **not** compare everything — $4 \nmid 6$ and $6 \nmid 4$. The right mental
picture is not a line but a *lattice*:

```text
                    12
                  /    \
                 4      6
                  \    / \
                   2      3
                    \    /
                      1
```

An arrow upward means "divides". This picture is why "greatest common divisor" is really
"greatest *lower bound*", and why the GCD concept insists the load-bearing property is
"every common divisor **divides** the gcd" rather than "is $\leq$ it".

**Why divisors come in pairs.** If $d$ divides $n$, then so does $n/d$, and the two
multiply back to $n$. So divisors always come as couples $\{d,\ n/d\}$ straddling
$\sqrt{n}$: one at most $\sqrt{n}$, the other at least. To find every divisor of $n$ you
therefore only need to walk up to $\sqrt{n}$ — the basis of nearly every divisor algorithm.

---

## 3. Mathematical Formulation

### The relation

$$a \mid b \quad \Longleftrightarrow \quad \exists\, k \in \mathbb{Z} : b = ak$$

### The division algorithm

For integers $a$ and $b \neq 0$ there exist **unique** integers $q$ (quotient) and $r$
(remainder) with

$$a = qb + r, \qquad 0 \leq r < \lvert b \rvert$$

For $b > 0$ the quotient is $q = \lfloor a/b \rfloor$. Note the constraint is
$0 \leq r$ — the mathematical remainder is **never negative**, which is *not* what C++'s
`%` operator gives you (§8).

### Divisor counting and summing

For $n \geq 1$ with prime factorization $n = p_1^{e_1}p_2^{e_2}\cdots p_k^{e_k}$:

$$\tau(n) = \prod_{i=1}^{k}(e_i + 1), \qquad
\sigma(n) = \prod_{i=1}^{k} \frac{p_i^{\,e_i+1} - 1}{p_i - 1}$$

*Depends on:* unique factorization. Each factor of $\sigma$ is a geometric series sum —
this is exactly GP-7 from the progressions concept, one prime at a time.

### Counting multiples

The number of multiples of $d$ in $[1, n]$ is $\left\lfloor \dfrac{n}{d} \right\rfloor$.
(Proved as AP-6 in the progressions concept; used constantly here.)

---

## 4. Properties

The first four are exactly what the GCD concept imports as **D1–D4**.

**D1 (Linearity).** If $d \mid a$ and $d \mid b$, then for *all* integers $x, y$:

$$d \mid (ax + by)$$

*Why:* write $a = dm$, $b = dn$; then $ax + by = d(mx + ny)$, and $mx + ny$ is an integer.
**This is the workhorse of the entire subject.** Special cases: $d \mid (a+b)$,
$d \mid (a-b)$, $d \mid ab$.

**D2 (Size bound).** If $d \mid a$ and $a \neq 0$, then $\lvert d \rvert \leq \lvert a \rvert$.
*Why:* $a = dk$ with $k \neq 0$, so $\lvert k \rvert \geq 1$ and
$\lvert a \rvert = \lvert d \rvert\lvert k \rvert \geq \lvert d \rvert$. The hypothesis
$a \neq 0$ is essential — every integer divides $0$, however large.

**D3 (Division algorithm).** For $b \neq 0$, the $q$ and $r$ above exist and are **unique**.
*Why it matters:* existence gives the Euclidean algorithm its step; *uniqueness* is what
makes "the remainder" a well-defined object you can compute with.

**D4 (Transitivity).** If $c \mid b$ and $b \mid a$, then $c \mid a$.
*Why:* $b = cm$ and $a = bn$ give $a = c(mn)$.

**D5 (Reflexivity and units).** $a \mid a$; $1 \mid a$; $a \mid 0$; and $a \mid 1$ only for
$a = \pm 1$.

**D6 (Antisymmetry).** If $a \mid b$ and $b \mid a$ then $a = \pm b$. Restricted to
positive integers, $a \mid b$ and $b \mid a$ force $a = b$ — so $\mid$ is a genuine partial
order on $\mathbb{Z}_{>0}$, which is the lattice picture in §2.

**D7 (Scaling and cancellation).** $a \mid b \implies ac \mid bc$ for every $c$. Conversely,
if $ac \mid bc$ and $c \neq 0$, then $a \mid b$. *The $c \neq 0$ is not optional:*
$0 \mid 0$ holds for every pair, so cancelling $c = 0$ would "prove" anything.

**D8 (Divisor pairing).** For $n \geq 1$, the map $d \mapsto n/d$ is a bijection from the
positive divisors of $n$ to themselves, and it is its own inverse. Every divisor $d$
satisfies $d \leq \sqrt{n}$ or $n/d \leq \sqrt{n}$.
*Why it matters:* this is why divisor enumeration costs $O(\sqrt{n})$ and not $O(n)$.

**D9 (Odd divisor count).** $\tau(n)$ is odd **iff** $n$ is a perfect square.
*Why:* pairing partitions the divisors into two-element sets $\{d, n/d\}$, except when
$d = n/d$ — which happens exactly once, when $n = d^2$. A parity argument, no computation.

**D10 (Divisibility tests).** For a decimal integer $n$:

| Divisor | Test |
|---|---|
| $2^k$, $5^k$ | look at the last $k$ digits only |
| $3$, $9$ | the digit sum has the same divisibility as $n$ |
| $11$ | the **alternating** digit sum has the same divisibility as $n$ |

*Why:* $9 \mid (10^k - 1)$ and $11 \mid (10^k - (-1)^k)$, both instances of GP-5
($(a-b) \mid (a^n - b^n)$), combined with linearity D1. No modular arithmetic is needed —
these are pure divisibility facts, proved in [proofs.md](proofs.md) Theorem 9.

**D11 (What is *not* true).** $d \mid ab$ does **not** imply $d \mid a$ or $d \mid b$.
Counterexample: $6 \mid 4 \cdot 9 = 36$, yet $6 \nmid 4$ and $6 \nmid 9$. The implication
holds precisely when $d$ is prime — that is Euclid's lemma, and it needs Bézout, not
divisibility alone. See [gcd-and-euclidean-algorithm/proofs.md](../gcd-and-euclidean-algorithm/proofs.md)
Theorem 10.

---

## 5. Examples

**The relation.** $3 \mid 12$ (take $k = 4$). $5 \nmid 12$. $7 \mid -21$ (take $k = -3$) —
divisibility is indifferent to sign.

**Division algorithm, positive.** $a = 17$, $b = 5$: $\ 17 = 3 \cdot 5 + 2$, so $q = 3$,
$r = 2$.

**Division algorithm, negative — the case that surprises people.** $a = -17$, $b = 5$:

$$-17 = (-4)\cdot 5 + 3, \qquad q = -4,\ r = 3$$

**not** $q = -3, r = -2$. The remainder must satisfy $0 \leq r < 5$, so it is $3$. C++'s
`-17 % 5` returns $-2$ instead — see §8.

**Linearity D1 in action.** $7 \mid 21$ and $7 \mid 35$, therefore $7$ divides every
$21x + 35y$: for instance $21 \cdot 3 - 35 \cdot 1 = 28$, and indeed $7 \mid 28$. ✓

**Divisors of 36 and the pairing D8.**

$$1 \cdot 36, \quad 2 \cdot 18, \quad 3 \cdot 12, \quad 4 \cdot 9, \quad 6 \cdot 6$$

giving $\{1,2,3,4,6,9,12,18,36\}$, so $\tau(36) = 9$ and $\sigma(36) = 91$. Note $6$ pairs
with itself — $36$ is a perfect square, and $\tau(36) = 9$ is odd, exactly as D9 predicts.
Enumerating required checking only $1$ through $6 = \sqrt{36}$.

**Contrast, a non-square.** $\tau(12) = 6$ (even): $\{1,12\},\{2,6\},\{3,4\}$, three clean
pairs, no fixed point.

**Formula check.** $36 = 2^2 \cdot 3^2$, so $\tau(36) = (2+1)(2+1) = 9$ ✓ and

$$\sigma(36) = \frac{2^3 - 1}{1}\cdot\frac{3^3-1}{2} = 7 \cdot 13 = 91 \ ✓$$

**D10 in action.** Is $9 \mid 12345$? Digit sum $1+2+3+4+5 = 15$, and $9 \nmid 15$, so no.
Is $3 \mid 12345$? $3 \mid 15$, so yes. Is $11 \mid 918082$? Alternating sum
$2 - 8 + 0 - 8 + 1 - 9 = -22$, and $11 \mid -22$, so yes.

**D11 in action.** $6 \mid 36 = 4 \cdot 9$, but $6 \nmid 4$ and $6 \nmid 9$. Divisibility
of a product tells you nothing about the factors unless the divisor is prime.

---

## 6. Algorithm

### 6.1 The division algorithm in code — $O(1)$

C++ integer division **truncates toward zero**, so `-17 / 5 == -3` and `-17 % 5 == -2`.
The mathematical convention of D3 requires $0 \leq r < \lvert b \rvert$. To get it:

$$q = \left\lfloor \frac{a}{b} \right\rfloor, \qquad r = a - qb$$

with a genuine *floor*. Implement floor division by correcting the truncated quotient
downward when the operands have opposite signs and the division was inexact. Equivalently,
normalize the remainder with `((a % b) + b) % b`.

Which convention you want depends on the problem — but you must know which one you have.

### 6.2 Enumerating divisors — $O(\sqrt{n})$ time

**Idea.** By D8 divisors come in pairs $\{d,\ n/d\}$ straddling $\sqrt{n}$. Walk $d$ from
$1$ to $\lfloor\sqrt{n}\rfloor$; whenever $d \mid n$, record both $d$ and $n/d$.

**Steps.**

1. For $d = 1, 2, \dots$ while $d \leq n/d$:
2. If $d \mid n$, emit $d$; and if $d \neq n/d$, emit $n/d$ as well.

**Why it works.** D8 guarantees every divisor is either $\leq \sqrt n$ (so it is found
directly as some $d$) or $\geq \sqrt n$ (so its partner is found and it is emitted as
$n/d$). The guard $d \neq n/d$ prevents emitting $\sqrt n$ twice when $n$ is a perfect
square — the classic off-by-one here.

**The loop bound.** Write `d <= n / d`, **not** `d * d <= n`. The latter overflows for
$n$ near the top of the type long before the loop would end.

- **Time:** $O(\sqrt n)$. **Space:** $O(\tau(n))$ for the output, $O(1)$ if you only count.

Counting $\tau(n)$ and summing $\sigma(n)$ use the identical loop, adding $2$ (or $1$ at a
fixed point) and $d + n/d$ respectively.

### 6.3 Trial division for primality — $O(\sqrt{n})$

$n > 1$ is prime iff no $d$ with $2 \leq d \leq \sqrt n$ divides it. **Why $\sqrt n$
suffices:** if $n = ab$ with $1 < a \leq b$, then $a^2 \leq ab = n$, so $a \leq \sqrt n$.
A composite therefore *always* betrays itself below $\sqrt n$. (Proof: Theorem 14.)

This belongs properly to the primes concept; it is included because the $\sqrt n$ bound is
a divisibility fact, not a primality fact.

### 6.4 Divisor-count sieve — $O(n \log n)$

To get $\tau(m)$ for **all** $m \leq n$ at once, invert the loop: instead of asking each $m$
for its divisors, let each $d$ visit its own multiples.

```text
for d = 1 .. n:
    for m = d, 2d, 3d, ... <= n:
        tau[m] += 1
```

**Why it works.** The inner loop increments $\tau[m]$ exactly once per divisor $d$ of $m$,
so every divisor is counted exactly once. The same skeleton computes $\sigma$ (add $d$
instead of $1$), the smallest prime factor, and much else — **this "iterate over multiples"
pattern is one of the most reusable ideas in the subject.**

**Complexity.** The total work is

$$\sum_{d=1}^{n} \left\lfloor \frac{n}{d} \right\rfloor \leq n\sum_{d=1}^{n}\frac{1}{d} = nH_n = \Theta(n\log n)$$

The harmonic sum is what turns an apparently quadratic double loop into $n \log n$ —
proved in Theorem 11.

**Complexity summary**

| Operation | Time | Space |
|---|---|---|
| test $a \mid b$ | $O(1)$ | $O(1)$ |
| division algorithm (floored) | $O(1)$ | $O(1)$ |
| all divisors of $n$ | $O(\sqrt n)$ | $O(\tau(n))$ |
| $\tau(n)$ or $\sigma(n)$, single $n$ | $O(\sqrt n)$ | $O(1)$ |
| primality by trial division | $O(\sqrt n)$ | $O(1)$ |
| $\tau(m)$ for all $m \leq n$ | $O(n\log n)$ | $O(n)$ |
| digit-based divisibility test | $O(\log n)$ | $O(1)$ |

---

## 7. Problem-Solving

### How to recognize it

- **Wording:** "divisible by", "leaves no remainder", "split into equal groups", "how many
  divisors", "is a multiple of".
- **A perfect-square question in disguise:** anything about an *odd* number of divisors, or
  about toggling switches — that is D9.
- **"For every pair $(i, j)$ with $i \mid j$"**: the answer is almost never a double loop;
  it is the multiples pattern of §6.4.
- **Constraints of size $10^{12}$ with a single query** point at $O(\sqrt n)$;
  **constraints of size $10^6$ with many queries** point at a sieve.

### Common patterns

**Enumerate divisors in $O(\sqrt n)$.** The default tool. Remember the output is *not*
sorted if you emit pairs as you find them — sort at the end if order matters.

**Count multiples instead of iterating them.** The number of multiples of $d$ in $[1,n]$ is
$\lfloor n/d \rfloor$ — an $O(1)$ answer to a question that looks like it needs a loop.
Combine with inclusion–exclusion for "divisible by $a$ or $b$".

**Swap the order of summation.** Sums like $\sum_{m \leq n}\ \sum_{d \mid m} f(d)$ become
$\sum_{d \leq n} f(d)\lfloor n/d \rfloor$ by counting each pair $(d, m)$ from the other
side. This single move converts many $O(n\sqrt n)$ solutions into $O(n \log n)$ or
$O(n)$ — it is the same idea as §6.4, used algebraically.

**The locker / light-switch problem.** $n$ switches, pass $k$ toggles every $k$-th switch;
which end up on? Switch $m$ is toggled once per divisor, so it ends on iff $\tau(m)$ is odd
iff $m$ is a perfect square (D9). Recognizing this turns a simulation into a one-liner.

**Divisibility tests on huge numbers.** When the input is a 10⁵-digit *string*, you cannot
parse it into an integer. Use D10: digit sum for $3$ and $9$, alternating sum for $11$, last
digits for $2^k$ and $5^k$.

**"Same remainder" reduces to differences.** If $d \mid (a - b)$ then $a$ and $b$ leave the
same remainder mod $d$ — the bridge to congruences, and the reason the gcd-of-differences
trick in the GCD concept works.

### Typical tricks

- **Loop with `d <= n / d`**, never `d * d <= n`.
- **Handle the perfect-square fixed point once**, or you double-count.
- **Normalize negative remainders** with `((a % b) + b) % b` before using them as indices.
- **Never test divisibility in floating point.** `n / d * d == n` with `double` fails
  silently past $2^{53}$; use the integer `%`.
- **Prefer $\tau$ parity over computing $\sqrt n$** when asking "is this a perfect square"
  inside a divisor loop you are already running.

### Important observations

- $\tau(n)$ is small: below $10^{18}$ no integer has more than $103680$ divisors, and
  typical values are tiny. Storing all divisors of one number is cheap; storing them for
  every number up to $n$ is not.
- Divisibility says nothing about size in one direction: $5 \mid 10^{18}$, and $5$ is
  small. But D2 does bound divisors of nonzero numbers.
- $a \mid b$ and $b \mid a$ together pin $a$ to $\pm b$ (D6) — a standard way to finish a
  proof by showing two quantities divide each other.

---

## 8. Common Mistakes

**Conceptual**

1. **Believing $0 \mid 0$ is false, or that $a \mid 0$ needs a special case.** Both are
   true, straight from the definition. What *is* false is $0 \mid b$ for $b \neq 0$.
2. **Reading $a \mid b$ backwards.** It means "$a$ divides $b$", so $a$ is the *small* one.
   $3 \mid 12$, not $12 \mid 3$. The notation looks like a fraction bar and invites exactly
   the wrong reading.
3. **Assuming $d \mid ab \implies d \mid a$ or $d \mid b$.** False in general — $6 \mid 36$
   with $6 \nmid 4$, $6 \nmid 9$. It needs $d$ prime (Euclid's lemma).
4. **Dropping the $a \neq 0$ in D2**, then "concluding" that a divisor of $0$ is bounded.
5. **Cancelling a possibly-zero factor in D7.** $ac \mid bc \implies a \mid b$ requires
   $c \neq 0$.
6. **Confusing "number of divisors" with "number of prime factors".** $\tau(12) = 6$, but
   $12$ has two distinct primes.
7. **Expecting the mathematical remainder from `%`.** D3 demands $0 \leq r < \lvert b \rvert$;
   C++ does not deliver that for negative operands.

**Implementation**

8. **`d * d <= n` overflowing.** For $n$ near $9 \cdot 10^{18}$, `d * d` wraps and the loop
   misbehaves. Write `d <= n / d`.
9. **Double-emitting $\sqrt n$** for perfect squares. Guard with `if (d != n / d)`.
10. **`-17 % 5 == -2` in C++.** The sign follows the *dividend*. Using that directly as an
    array index is an out-of-bounds read. Normalize first.
11. **`n % 0`** — undefined behaviour, and typically a crash. The division algorithm
    requires $b \neq 0$; guard it.
12. **Using `std::sqrt` for the loop bound.** `(int)sqrt(n)` can be off by one from
    floating-point rounding near perfect squares. Either use the `d <= n / d` form, or
    correct the result with a `while` afterwards.
13. **Forgetting divisors come out unsorted** from the pairing loop, then relying on order.
14. **Sieving with `int` when $\sigma$ can overflow.** $\sigma(m)$ can exceed $m$
    substantially; size the accumulator type for the sum, not for the input.
15. **Testing divisibility on a number too large to parse.** If the input is a digit string,
    parse it into an `int`/`long long` only when it fits; otherwise use D10.

---

## Files

- [README.md](README.md) — this file: the concept.
- [proofs.md](proofs.md) — rigorous proofs, including the division algorithm and D1–D4.
- [implementation.cpp](implementation.cpp) — C++17 implementation with complexities.
