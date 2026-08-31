# GCD and the Euclidean Algorithm

$\gcd(a,b)$ is the largest number dividing both. Euclid finds it in a handful of steps
however large the inputs.

**Use:** reducing fractions, lcm and cycles, coprimality, reachability. **Needs:**
[divisibility](../divisibility/) **Next:**
[unique factorization](../unique-factorization/) ·
[extended Euclid](../extended-euclidean-algorithm/)

---

## The idea

**Whatever divides both numbers also divides their difference.** Repeatedly subtracting
$b$ from $a$ until you drop below $b$ *is* computing $a \bmod b$, so:

$$\boxed{\ \gcd(a,b) = \gcd(b,\ a \bmod b), \qquad \gcd(a,0) = |a|\ }$$

$$
\begin{aligned}
48 &= 2\times18 + 12\\
18 &= 1\times12 + \phantom{0}6\\
12 &= 2\times\phantom{0}6 + \phantom{0}0 \qquad\Longrightarrow\qquad \gcd(48,18) = 6
\end{aligned}
$$

```cpp
ll gcd(ll a, ll b) {
    a = a < 0 ? -a : a;                      // signs never matter
    b = b < 0 ? -b : b;
    while (b) { ll r = a % b; a = b; b = r; }
    return a;
}
```

$O(\log\min(a,b))$ — **under 90 iterations for any 64-bit input**, so its cost never
matters. Order is irrelevant: if $a<b$ the first step swaps them.

---

## Key facts

**lcm.** $\ \mathrm{lcm}(a,b) = \dfrac{|ab|}{\gcd(a,b)}$ — no separate algorithm.

> Write `(a / g) * b`, not `a * b / g`. The product overflows for inputs whose lcm fits;
> dividing first is exact because $g \mid a$.

**Bézout.** There are integers $x,y$ with $ax+by = \gcd(a,b)$, and the values $ax+by$ are
**exactly the multiples of $\gcd(a,b)$**. So with steps of size $a$ and $b$ you reach
precisely the multiples of $\gcd(a,b)$ — "can I measure $c$ litres with $a$- and
$b$-litre jugs?" is just $\gcd(a,b) \mid c$.

**Euclid's lemma.** $p$ prime and $p \mid ab$ $\Rightarrow$ $p \mid a$ or $p \mid b$. This
repairs divisibility's trap ($6 \mid 4{\times}9$ but $6$ divides neither) and is what makes
factorization unique.

**Small facts.** $\gcd$ is associative, so fold an array with `g = gcd(g, x)` starting from
$0$ (since $\gcd(0,x)=|x|$, no first-element special case). $\gcd(a/g,\ b/g) = 1$ — that is
"reduce to lowest terms". $\gcd(0,0)=0$ by convention.

---

## Two patterns

**Same remainder → subtract.** *"Which $d>1$ make all $a_i$ leave the same remainder?"*
Same remainder means $d$ divides every difference, so $d$ divides

$$g = \gcd(a_2-a_1,\ a_3-a_1,\ \dots)$$

and the answers are the divisors of $g$. Turns a statement about remainders into one gcd.

**Reduce before comparing.** Store fractions as $(p/g,\ q/g)$ with the sign in the
numerator, or equal fractions hash differently.

---

## Complexity

| $\gcd$, $\mathrm{lcm}$ | $O(\log\min(a,b))$ |
|---|---|
| gcd of $n$ values | $O(n\log M)$ |

---

## Pitfalls

| | |
|---|---|
| `a * b / g` overflows | `(a / g) * b` |
| `lcm(0, x)` divides by a zero gcd | guard zeros |
| negative inputs give a negative result | `abs` on entry |
| forgetting $\gcd(0,0)=0$ | an all-zero array returns $0$ |
| $\gcd(a,b)=1$ means one is prime | no: $\gcd(8,9)=1$ |
| set-gcd $1$ means *pairwise* coprime | no: $\gcd(6,10,15)=1$, yet no pair is |

The last one bites later — CRT needs the pairwise condition. `std::lcm` is UB if the
*result* overflows, so it does not save you from the first row.

---

[proofs.md](proofs.md) · [implementation.cpp](implementation.cpp)
