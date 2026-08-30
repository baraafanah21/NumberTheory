# Extended Euclidean Algorithm

The [gcd concept](../gcd-and-euclidean-algorithm/) proved that integers $x, y$ exist with

$$a\,x + b\,y = \gcd(a,b)$$

but never produced them. This algorithm produces them. Those two numbers — the **Bézout
coefficients** — are what turn "a solution exists" into an actual answer.

**You need this for:** solving $ax + by = c$ in integers, the Chinese Remainder Theorem,
and (next concept) modular inverses.

**Before this:** [gcd](../gcd-and-euclidean-algorithm/). **After this:**
[modular multiplicative inverse](../modular-multiplicative-inverse/).

---

## The algorithm

```cpp
// returns g = gcd(a,b), and sets x, y so that a*x + b*y == g
ll extgcd(ll a, ll b, ll &x, ll &y) {
    if (b == 0) { x = 1; y = 0; return a; }    // a*1 + 0*0 = a
    ll x1, y1;
    ll g = extgcd(b, a % b, x1, y1);
    x = y1;
    y = x1 - (a / b) * y1;
    return g;
}
```

It is the ordinary Euclidean algorithm with the coefficients carried back up the recursion.

**Where those two update lines come from.** The recursive call hands back

$$b\,x_1 + (a \bmod b)\,y_1 = g$$

Replace $a \bmod b$ with $a - \lfloor a/b\rfloor\,b$ and regroup by $a$ and $b$:

$$a\,y_1 \;+\; b\Bigl(x_1 - \Bigl\lfloor\tfrac{a}{b}\Bigr\rfloor y_1\Bigr) \;=\; g$$

Reading off the coefficients gives $x = y_1$ and $y = x_1 - \lfloor a/b\rfloor\,y_1$.
**Re-derive this rather than memorizing it** — it takes ten seconds, and you will not mix
up which coefficient gets the quotient.

$O(\log\min(a,b))$, same speed as ordinary gcd. The coefficients stay small, so nothing
overflows inside the function.

**Example.** $\gcd(240, 46) = 2$, and the algorithm returns $x = -9$, $y = 47$:

$$240\times(-9) + 46\times 47 = -2160 + 2162 = 2 \quad\checkmark$$

Note $x$ came back **negative** — that is normal, and it matters when you use the result.

---

## Use 1: solving $ax + by = c$ in integers

$$\text{solvable} \quad\Longleftrightarrow\quad \gcd(a,b) \mid c$$

If it is solvable, run `extgcd` to get a solution of $ax + by = g$, then **multiply both
coefficients by $c/g$**. All other solutions are

$$x = x_0 + k\cdot\frac{b}{g}, \qquad y = y_0 - k\cdot\frac{a}{g}, \qquad k \in \mathbb{Z}$$

> The steps are $\frac bg$ and $\frac ag$ — **not** $b$ and $a$. When $g > 1$, stepping by
> $b$ skips $g-1$ out of every $g$ solutions, which quietly breaks "find the smallest
> non-negative $x$".

**Example.** $6x + 10y = 8$. Here $g = 2$ and $2 \mid 8$, so it is solvable: $x = 8$,
$y = -4$. Other solutions step by $\frac{10}{2} = 5$ in $x$ and $\frac{6}{2} = 3$ in $y$ —
so $x = 3, y = -1$ also works. But $6x + 10y = 9$ has **no** solution, since $2 \nmid 9$.

This is the same statement as *"with steps of size $a$ and $b$ you can reach exactly the
multiples of $\gcd(a,b)$"* — except now you also learn **how many** of each step to take.

---

## Complexity

| Task | Time |
|---|---|
| `extgcd` | $O(\log\min(a,b))$ |
| solving $ax+by=c$ | $O(\log\min(a,b))$ |

---

## Common mistakes

| Mistake | Fix |
|---|---|
| Using $x$ as returned — it can be negative | normalize when the context needs it |
| Forgetting to scale by $c/g$ | you solved $= g$, not $= c$ |
| Stepping by $b$ instead of $b/g$ | you skip solutions |
| Assuming $ax+by=c$ is always solvable | needs $\gcd(a,b) \mid c$ |

---

## Files

- [proofs.md](proofs.md) — why the recursion is correct, and where the solution family
  comes from.
- [implementation.cpp](implementation.cpp) — two functions.

**What `extgcd` unlocks.** Two concepts are built directly on it:

- [modular multiplicative inverse](../modular-multiplicative-inverse/) — dividing under a
  modulus;
- [Chinese remainder theorem](../chinese-remainder-theorem/) — combining congruences.
