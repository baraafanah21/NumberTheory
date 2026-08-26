# GCD and the Euclidean Algorithm

The **greatest common divisor** $\gcd(a,b)$ is the largest number dividing both $a$ and
$b$. The Euclidean algorithm finds it in a handful of steps, no matter how large the
inputs.

**You need this for:** reducing fractions, lcm and cycle problems, coprimality conditions,
and "can I reach exactly $c$ using steps of $a$ and $b$".

**Before this:** [divisibility](../divisibility/). **After this:**
[extended Euclid](../extended-euclidean-algorithm/).

---

## The idea

**Whatever divides both numbers also divides their difference.**

Picture two sticks of length $a$ and $b$. A ruler that measures both exactly also measures
the leftover piece when you lay the short stick against the long one. So

$$\gcd(a,\ b) \;=\; \gcd(a - b,\ b)$$

Repeatedly subtracting $b$ from $a$ until you drop below $b$ is just computing $a \bmod b$,
so one modulo replaces a whole run of subtractions:

$$\boxed{\ \gcd(a,\ b) \;=\; \gcd(b,\ a \bmod b\,), \qquad \gcd(a,\ 0) = |a|\ }$$

**Worked example.**

$$
\begin{aligned}
\gcd(48, 18):\quad 48 &= 2\times 18 + 12\\
18 &= 1\times 12 + \phantom{0}6\\
12 &= 2\times \phantom{0}6 + \phantom{0}0 \qquad\Longrightarrow\qquad \gcd = 6
\end{aligned}
$$

---

## The algorithm

```cpp
ll gcd(ll a, ll b) {
    a = a < 0 ? -a : a;              // signs never matter; keep the result positive
    b = b < 0 ? -b : b;
    while (b) { ll r = a % b; a = b; b = r; }
    return a;
}
```

**Speed:** $O(\log \min(a,b))$ — **under 90 iterations for any 64-bit input**, so it is
effectively free. You never need to worry about its cost.

Order does not matter: if $a < b$ then $a \bmod b = a$, and the first step simply swaps
them.

---

## The facts you use

### Least common multiple

$$\mathrm{lcm}(a,b) \;=\; \frac{|a\,b|}{\gcd(a,b)}$$

There is no separate algorithm for lcm — compute the gcd and divide.

> **Write it as `(a / g) * b`, not `a * b / g`.** Both are mathematically equal, but
> `a * b` overflows for inputs much smaller than those whose lcm overflows. Dividing first
> is safe because $g$ always divides $a$ exactly.

### Bézout's identity

There exist integers $x, y$ with

$$a\,x + b\,y = \gcd(a,b)$$

and more usefully, the set of all values $ax + by$ is **exactly the multiples of
$\gcd(a,b)$**.

This is why: *with steps of size $a$ and $b$ (forwards or backwards), the positions you can
reach are precisely the multiples of $\gcd(a,b)$.* So "can I measure exactly $c$ litres
with $a$- and $b$-litre jugs?" is just asking whether $\gcd(a,b) \mid c$.

### Euclid's lemma

$$p \text{ prime and } p \mid a\,b \qquad\Longrightarrow\qquad p \mid a \ \text{ or } \ p \mid b$$

This repairs the trap from the divisibility concept ($6 \mid 4\times 9$ but $6$ divides
neither). It is also what makes prime factorization **unique**.

### Small facts

| Fact | Use |
|---|---|
| $\gcd$ is associative | fold an array: `g = gcd(g, x)` |
| $\gcd(0, x) = \|x\|$ | start the fold at $0$ — no special case for the first element |
| $\gcd(a/g,\ b/g) = 1$ | this is "reduce a fraction to lowest terms" |
| $\gcd(0,0) = 0$ | by convention |

---

## Two patterns worth knowing

### Reducing a fraction

Divide both parts by $\gcd$. Put the sign in the numerator so that equal fractions get
equal representations — necessary if you ever use them as map keys.

### The "same remainder" trick

*"Find every $d > 1$ such that all the $a_i$ leave the same remainder mod $d$."*

Same remainder means $d$ divides every difference $a_i - a_j$. So $d$ must divide

$$g = \gcd\bigl(a_2 - a_1,\ a_3 - a_1,\ \dots\bigr)$$

and the answers are exactly the divisors of $g$. This converts a statement about
remainders into a single gcd — one of the most useful moves in the topic.

---

## Complexity

| Task | Time |
|---|---|
| $\gcd(a,b)$ | $O(\log\min(a,b))$ |
| $\mathrm{lcm}(a,b)$ | $O(\log\min(a,b))$ |
| gcd of $n$ values | $O(n\log M)$ |

---

## Common mistakes

| Mistake | Fix |
|---|---|
| `a * b / g` overflows | `(a / g) * b` |
| `lcm(0, x)` divides by a zero gcd | guard zeros first |
| Negative inputs give a negative result | take absolute values on entry |
| Forgetting $\gcd(0,0) = 0$ | an all-zero array returns $0$, not $1$ |
| Thinking $\gcd(a,b)=1$ means one is prime | $\gcd(8,9)=1$, neither is prime |
| Thinking set-gcd $1$ means *pairwise* coprime | $\gcd(6,10,15)=1$, yet **no pair** is coprime |

That last one bites later: CRT needs the **pairwise** condition, which is strictly
stronger.

`std::gcd` and `std::lcm` exist in `<numeric>`, but `std::lcm` is undefined behaviour if
the *result* overflows — it does not save you from the first mistake.

---

## Files

- [proofs.md](proofs.md) — why the step is valid, why it is fast, Bézout, Euclid's lemma.
- [implementation.cpp](implementation.cpp) — four functions.
