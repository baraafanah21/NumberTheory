# GCD and the Euclidean Algorithm

**Use it for:** reducing fractions, lcm/cycle problems, coprimality conditions,
reachability with fixed steps, lattice points, and any "same remainder" condition.

**Prereq:** [divisibility](../divisibility/). **Used by:** extended Euclid → modular
inverse → CRT; Euclid's lemma → unique factorization.

---

## 1. The algorithm

$$\gcd(a, 0) = \lvert a \rvert, \qquad \gcd(a,b) = \gcd(b,\ a \bmod b)$$

```cpp
ll gcd(ll a, ll b) { while (b) { ll r = a % b; a = b; b = r; } return a < 0 ? -a : a; }
```

Three lines. $O(\log \min(a,b))$ — at most **90 iterations** for any 64-bit input, so it is
effectively free. Order does not matter: if $a < b$ the first step just swaps them.

`std::gcd` / `std::lcm` exist in `<numeric>` (C++17), but see pitfall 4 — `std::lcm` will
not save you from overflow.

---

## 2. Facts you actually use

| # | Fact | Where it shows up |
|---|---|---|
| **G1** | $\gcd(a,b) = \gcd(b,\ a \bmod b)$, $\gcd(a,0) = \lvert a \rvert$ | the algorithm |
| **G2** | $\mathrm{lcm}(a,b) = \dfrac{\lvert ab \rvert}{\gcd(a,b)}$ | cycles, periods, coincidences |
| **G3** | $\gcd$ is associative → fold an array left to right | `g = gcd(g, x)` |
| **G4** | $\gcd(ca, cb) = \lvert c \rvert\gcd(a,b)$ | pulling factors out |
| **G5** | $\gcd(a/g,\ b/g) = 1$ where $g = \gcd(a,b)$ | reducing fractions |
| **G6** | **Bézout:** $\{ax + by\}$ = exactly the multiples of $\gcd(a,b)$ | reachability, solvability |
| **G7** | **Euclid's lemma:** $p$ prime, $p \mid ab$ $\Rightarrow$ $p\mid a$ or $p \mid b$ | fixes divisibility's D7 trap |
| **G8** | $\gcd(a^m - 1,\ a^n - 1) = a^{\gcd(m,n)} - 1$ | collapses huge-number gcds |
| **G9** | Worst case = consecutive Fibonacci ($\gcd(F_{n+2}, F_{n+1})$ costs $n$ steps) | the input to stress-test with |

**Conventions to fix in your head:** $\gcd(0,0) = 0$; $\gcd$ is always returned
non-negative; $\gcd(a,b) = 1$ does **not** mean either is prime ($\gcd(8,9) = 1$).

---

## 3. Algorithms

| Task | Time | Space |
|---|---|---|
| $\gcd(a,b)$ | $O(\log \min(a,b))$ | $O(1)$ |
| $\mathrm{lcm}(a,b)$ | $O(\log \min(a,b))$ | $O(1)$ |
| gcd of $n$ values | $O(n \log M)$, early-exit at 1 | $O(1)$ |
| range gcd queries | sparse table: $O(n\log n\log M)$ build, $O(\log M)$ query | $O(n \log n)$ |
| binary gcd (no division) | $O(\log \max)$ iterations | $O(1)$ |

**lcm, written correctly:**

```cpp
ll lcm(ll a, ll b) {
    if (!a || !b) return 0;                 // else you divide by a zero gcd
    ll r = (a / gcd(a, b)) * b;             // divide FIRST -- a*b overflows
    return r < 0 ? -r : r;
}
```

**Array gcd:**

```cpp
ll g = 0;                                   // 0 is the identity: gcd(0,x) = |x|
for (ll x : v) { g = gcd(g, x); if (g == 1) break; }
```

---

## 4. Patterns

**gcd of differences — the highest-value trick here.** *"Find every $d>1$ such that all
$a_i$ leave the same remainder mod $d$."* Same remainder means $d \mid (a_i - a_j)$, so

$$d \ \Big|\ g = \gcd(a_2 - a_1,\ a_3 - a_1,\ \dots)$$

Answer: the divisors of $g$. Converts a congruence condition into one gcd. Also solves
"add the same $k$ to every element to make them all divisible by something".

**Reachability.** With steps $\pm a$ and $\pm b$, you reach **exactly** the multiples of
$\gcd(a,b)$ (G6). So "can I measure $c$ litres with $a$- and $b$-litre jugs?" is just
$\gcd(a,b) \mid c$. Same for frog jumps, coin combinations with negatives allowed.

**Lattice points on a segment.** Integer points strictly between $(x_1,y_1)$ and
$(x_2,y_2)$: $\gcd(\lvert\Delta x\rvert, \lvert\Delta y\rvert) - 1$. Including endpoints:
$\gcd + 1$.

**Cycles and coincidences.** Periods $p$ and $q$ align every $\mathrm{lcm}(p,q)$; in
$[1,N]$ that is $\lfloor N/\mathrm{lcm}(p,q)\rfloor$ times.

**Prefix gcds take $O(\log M)$ distinct values.** Extending a prefix either keeps the gcd
or at least halves it. So for a fixed left endpoint there are only $O(\log M)$ distinct
subarray gcds — that turns "count subarrays with gcd = $k$" from $O(n^2)$ into
$O(n\log M)$:

```cpp
// all distinct (gcd, count) pairs for subarrays ending at i
vector<pair<ll,int>> cur, nxt;
for (int i = 0; i < n; ++i) {
    nxt.clear(); nxt.push_back({v[i], 1});
    for (auto [g, c] : cur) {
        ll ng = gcd(g, v[i]);
        if (ng == nxt.back().first) nxt.back().second += c;
        else nxt.push_back({ng, c});
    }
    cur = nxt;                       // cur.size() is O(log M)
}
```

**Range gcd queries.** gcd is associative *and* idempotent, so a sparse table works with
overlapping intervals — no need for a segment tree unless there are updates.

**Huge powers.** Anything shaped $a^m - 1$: use G8 to drop to $\gcd(m,n)$.
$\gcd(2^{10^6}-1,\ 2^{10^6-1}-1) = 1$ falls out instantly.

**Reduce before comparing.** Store fractions as $(p/g,\ q/g)$ with the sign in the
numerator, or equal fractions hash differently.

---

## 5. Pitfalls

| # | Mistake | Fix |
|---|---|---|
| 1 | `a * b / g` overflows | `(a / g) * b` |
| 2 | `lcm(0, x)` divides by a zero gcd | guard zeros first |
| 3 | Negative inputs give a negative gcd | take `abs` on entry |
| 4 | Trusting `std::lcm` | UB if the **result** overflows; you still owe the range check |
| 5 | `std::gcd(int, unsigned)` | surprising common type; keep types matched |
| 6 | Recursing `gcd(b, a % b)` before the `b == 0` check | division by zero |
| 7 | Forgetting $\gcd(0,0) = 0$ | an all-zero array returns 0, not 1 |
| 8 | Assuming set-gcd 1 means pairwise coprime | $\gcd(6,10,15)=1$, yet no pair is coprime |
| 9 | `abs(LLONG_MIN)` | UB; widen the type if inputs can hit the minimum |

Pitfall 8 matters: CRT needs **pairwise** coprimality, which is strictly stronger.

---

## 6. Code

[implementation.cpp](implementation.cpp):

| Function | Purpose |
|---|---|
| `gcdIterative`, `gcdRecursive` | the algorithm, both forms |
| `binaryGcd` | division-free (shifts and subtraction only) |
| `lcm` | overflow-safe |
| `gcdOfArray`, `coprime` | folding and testing |
| `gcdOfDifferences` | the "same remainder" pattern |
| `latticePointsOnSegment` | $\gcd + 1$ |
| `gcdOfPowersMinusOne` | G8 |
| `euclidSteps` | step counter, for the Fibonacci worst case |

---

## 7. Why it works

[proofs.md](proofs.md) — short: why the step is valid, why it terminates in $O(\log)$, why
Bézout gives reachability, why Euclid's lemma holds, and why the lcm formula is exact.
