# Sieve of Eratosthenes

Finding **all** primes up to $n$, in barely more than $O(n)$ time.

Testing each number separately with trial division costs $O(\sqrt n)$ each, so
$O(n\sqrt n)$ overall — hopeless for $n = 10^7$. The sieve does the whole range at once.

**You need this for:** any problem needing many primes, factorizations, or divisor
functions over a range.

**Before this:** [divisibility](../divisibility/).

---

## The idea

Do not ask *"is this number prime?"* — instead, **cross out everything that cannot be**.

Start with all numbers $2 \dots n$ unmarked. Take the smallest unmarked number: it is
prime, because nothing smaller divides it. Cross out all of its multiples. Repeat.

```text
2  3  4  5  6  7  8  9 10 11 12 13 14 15 16      start
2  3  X  5  X  7  X  9  X 11  X 13  X 15  X      cross out multiples of 2
2  3  X  5  X  7  X  X  X 11  X 13  X  X  X      cross out multiples of 3
2  3  X  5  X  7  X  X  X 11  X 13  X  X  X      5: nothing new below 25
```

Left standing: $2, 3, 5, 7, 11, 13$.

---

## The algorithm

```cpp
std::vector<bool> sieve(int n) {
    std::vector<bool> isPrime(n + 1, true);
    isPrime[0] = isPrime[1] = false;
    for (int p = 2; (ll)p * p <= n; ++p)         // stop at sqrt(n)
        if (isPrime[p])
            for (int m = p * p; m <= n; m += p)  // start at p*p
                isPrime[m] = false;
    return isPrime;
}
```

**Two optimisations, both worth understanding rather than memorizing:**

- **Start the inner loop at $p^2$, not $2p$.** Any multiple $kp$ with $k < p$ has a prime
  factor smaller than $p$, so it was already crossed out on an earlier pass.
- **Stop the outer loop at $\sqrt n$.** If $p > \sqrt n$ then $p^2 > n$, so there is
  nothing left in range to cross out.

**Cost:** $O(n \log\log n)$ time, $O(n)$ space. The $\log\log n$ factor is under $4$ for
any $n$ you will ever sieve, so treat it as "basically linear".

---

## The upgrade: smallest prime factor

Instead of storing *"is prime"*, store **the smallest prime factor** of each number. Same
cost, far more useful:

```cpp
std::vector<int> spf(n + 1, 0);
for (int i = 2; i <= n; ++i)
    if (spf[i] == 0)                       // i is prime
        for (int m = i; m <= n; m += i)
            if (spf[m] == 0) spf[m] = i;   // only if not already set
```

Now `spf[m] == m` tests primality, **and** you can factorize any $m \le n$ in
$O(\log m)$ by repeatedly dividing it out:

```cpp
while (m > 1) { int p = spf[m]; while (m % p == 0) m /= p; /* p is a prime factor */ }
```

Each step at least halves $m$, so there are at most $\log_2 m$ of them.

Once you can factorize instantly, $\tau(n)$, $\sigma(n)$ and $\varphi(n)$ all follow from
their formulas.

---

## The linear sieve

A refinement that runs in **exactly** $O(n)$ by ensuring every composite is crossed out
**once**, not once per prime factor:

```cpp
std::vector<int> primes, spf(n + 1, 0);
for (int i = 2; i <= n; ++i) {
    if (spf[i] == 0) { spf[i] = i; primes.push_back(i); }
    for (int p : primes) {
        if (p > spf[i] || (ll)i * p > n) break;   // the crucial break
        spf[i * p] = p;
    }
}
```

The `p > spf[i]` break is the whole trick: it forces each composite to be built **only**
as (its smallest prime factor) × (the rest). See [proofs.md](proofs.md) §3.

> **Is it worth it?** In practice the plain sieve is often *faster* despite the worse
> bound, because it is cache-friendly and does no branching. Use the linear sieve when you
> need the prime list and `spf` together, or when you are also computing a multiplicative
> function during the sweep.

---

## When the range is too big: segmented sieve

To find primes in $[L, R]$ with $R$ up to $10^{12}$ — far too large for an array — note
that any composite in that range has a prime factor $\le \sqrt R$. So:

1. Sieve the small primes up to $\sqrt R$ (about $10^6$ entries).
2. Make a boolean array of size $R - L + 1$ representing $[L, R]$.
3. For each small prime $p$, cross out its multiples inside the window, starting from
   $\max(p^2,\ \lceil L/p\rceil \cdot p)$.

Memory is $O(\sqrt R + (R-L))$ — the window, not the whole range.

---

## Complexity

| Task | Time | Space |
|---|---|---|
| sieve up to $n$ | $O(n\log\log n)$ | $O(n)$ |
| linear sieve | $O(n)$ | $O(n)$ |
| factorize one $m \le n$ using `spf` | $O(\log m)$ | — |
| primes in $[L,R]$, segmented | $O\bigl((R-L)\log\log R + \sqrt R\bigr)$ | $O(\sqrt R + R - L)$ |
| trial division, one number | $O(\sqrt n)$ | $O(1)$ |

**Choosing:** one primality test of a big number → trial division. Many queries below
$10^7$ → sieve once. Need factorizations → `spf` sieve. Range near $10^{12}$ → segmented.

**How many primes are there?** About $\frac{n}{\ln n}$ — roughly $10^6$ below $1.5\times10^7$,
so a prime list is cheap to store.

---

## Common mistakes

| Mistake | Fix |
|---|---|
| Forgetting $0$ and $1$ are not prime | set both to `false` |
| `p * p` overflowing `int` when $n$ is near $2^{31}$ | cast to `long long`, or loop `p <= n / p` |
| Starting the inner loop at $2p$ | correct but slower; $p^2$ is the point |
| Running the outer loop to $n$ instead of $\sqrt n$ | wasted passes that mark nothing |
| Using `vector<bool>` and being surprised it is slow | it is a bit-array; fine for memory, use `vector<char>` for speed |
| Sieving $10^{12}$ directly | impossible; use the segmented version |
| Calling `sieve()` inside a loop over test cases | sieve **once**, globally |
| Trial-dividing after you already have `spf` | factorize in $O(\log m)$ instead |

---

## Files

- [proofs.md](proofs.md) — why starting at $p^2$ is safe, why stopping at $\sqrt n$ is
  safe, where $\log\log n$ comes from, and why the linear sieve is linear.
- [implementation.cpp](implementation.cpp) — four functions with self-checks.
