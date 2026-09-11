# Extended Euclidean Algorithm

[Bézout](../gcd/) says integers $x,y$ exist with
$ax+by = \gcd(a,b)$, but never produces them. This does.

**Use:** solving $ax+by=c$; and it is the engine under
[modular inverse](../modular-inverse/) and
[CRT](../chinese-remainder-theorem/). **Needs:**
[gcd](../gcd/)

---

## The algorithm

```cpp
// returns g = gcd(a,b), sets x,y with a*x + b*y == g
ll extgcd(ll a, ll b, ll &x, ll &y) {
    if (b == 0) { x = 1; y = 0; return a; }    // a*1 + 0*0 = a
    ll x1, y1;
    ll g = extgcd(b, a % b, x1, y1);
    x = y1;
    y = x1 - (a / b) * y1;
    return g;
}
```

Ordinary Euclid with the coefficients carried back up. **Derive the two update lines
rather than memorizing them** — the recursive call gives
$b\,x_1 + (a \bmod b)\,y_1 = g$, and substituting $a \bmod b = a - \lfloor a/b\rfloor b$
regroups to

$$a\,y_1 + b\Bigl(x_1 - \Bigl\lfloor\tfrac ab\Bigr\rfloor y_1\Bigr) = g$$

so $x = y_1$ and $y = x_1 - \lfloor a/b\rfloor y_1$. Ten seconds, and you never mix up
which coefficient gets the quotient.

$O(\log\min(a,b))$. The coefficients stay small, so nothing overflows inside.

**Example.** $\gcd(240,46) = 2$ with $x=-9$, $y=47$: $\ 240(-9) + 46(47) = 2$ ✓ — note
$x$ came back **negative**, which is normal and matters when you use it.

---

## Solving $ax + by = c$

$$\text{solvable} \iff \gcd(a,b) \mid c$$

Run `extgcd` for $ax+by=g$, then **scale both coefficients by $c/g$**. All solutions:

$$x = x_0 + k\cdot\frac{b}{g}, \qquad y = y_0 - k\cdot\frac{a}{g}$$

> The steps are $\frac bg$ and $\frac ag$, **not** $b$ and $a$. With $g>1$, stepping by $b$
> skips $g-1$ of every $g$ solutions — which silently breaks "smallest non-negative $x$".

**Example.** $6x+10y=8$: here $g=2 \mid 8$, so $x=8,\ y=-4$; stepping by $5$ and $3$ gives
$x=3,\ y=-1$ too. But $6x+10y=9$ has **no** solution, since $2 \nmid 9$.

Same statement as "steps of $a$ and $b$ reach exactly the multiples of $\gcd(a,b)$" —
except now you learn *how many* of each step.

---

## How to recognize it in a problem

Look for:

* **$ax + by = c$ in integers** — stated directly, or as a line $Ax+By+C=0$ asked for a
  lattice point
* **"can you make exactly $c$"** using unlimited $\pm a$ and $\pm b$ steps → solvable iff
  $\gcd(a,b) \mid c$
* buying/selling two items at fixed prices to hit an exact total
* **a linear congruence** $ax \equiv b \pmod m$ — the same equation with $y$ hidden
* **"smallest non-negative $x$"** satisfying a congruence → solve, then shift by $b/g$
* a [modular inverse](../modular-inverse/) on a modulus that is **not prime**
* merging two congruences → [CRT](../chinese-remainder-theorem/), which is this underneath
* counting how many solutions lie in a range

**The tell.** Two unknowns, one linear equation, integers only. That under-determined shape
is exactly what Bézout resolves, and the answer is always a one-parameter family rather
than a single point.

**Anti-pattern.** If you only need to know *whether* it is solvable, plain
[gcd](../gcd/) answers that — you do not need the coefficients. Reach for `extgcd` when you
need the actual $x$ and $y$.

---

## Complexity

| `extgcd`, solving $ax+by=c$ | $O(\log\min(a,b))$ |
|---|---|

---

## Pitfalls

| | |
|---|---|
| using $x$ as returned — it can be negative | normalize when the context needs it |
| forgetting to scale by $c/g$ | you solved $=g$, not $=c$ |
| stepping by $b$ instead of $b/g$ | you skip solutions |
| assuming $ax+by=c$ is always solvable | needs $\gcd(a,b) \mid c$ |

---

[proofs.md](proofs.md) · [implementation.cpp](implementation.cpp) · [problems.md](problems.md)
