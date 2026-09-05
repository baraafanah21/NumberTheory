# Sieve of Eratosthenes

All primes up to $n$, in barely more than $O(n)$. Testing each number separately costs
$O(n\sqrt n)$ — hopeless at $n = 10^7$.

**Use:** many primes, many factorizations, divisor functions over a range. **Needs:**
[divisibility](../divisibility/) **Next:**
[Euler's totient](../euler-totient-function/)

---

## The idea

Do not ask *"is this prime?"* — **cross out everything that cannot be**. Take the smallest
unmarked number (it is prime, since nothing smaller divides it), cross out its multiples,
repeat.

```text
2  3  4  5  6  7  8  9 10 11 12 13 14 15      start
2  3  X  5  X  7  X  9  X 11  X 13  X 15      multiples of 2
2  3  X  5  X  7  X  X  X 11  X 13  X  X      multiples of 3
```

```cpp
std::vector<char> sieve(int n) {
    std::vector<char> isPrime(n + 1, 1);
    isPrime[0] = isPrime[1] = 0;
    for (int p = 2; (ll)p * p <= n; ++p)         // stop at sqrt(n)
        if (isPrime[p])
            for (ll m = (ll)p * p; m <= n; m += p)   // start at p*p
                isPrime[m] = 0;
    return isPrime;
}
```

**Both optimisations are the same fact** — *a composite is always caught by its smallest
prime factor*:

- **start at $p^2$**: any multiple $kp$ with $k<p$ has a smaller prime factor and is
  already gone;
- **stop at $\sqrt n$**: a composite $m \le n$ has a prime factor $\le \sqrt m \le \sqrt n$.

$O(n\log\log n)$ — and $\log\log n < 4$ for any $n$ you will sieve, so treat it as linear.

---

## The upgrade: smallest prime factor

Store `spf[m]` instead of a bool. Same cost, far more useful: `spf[m] == m` tests
primality, **and** any $m \le n$ factorizes in $O(\log m)$:

```cpp
while (m > 1) { int p = spf[m]; while (m % p == 0) m /= p; /* p is a prime factor */ }
```

Each step at least halves $m$ — about 23 operations at $m = 10^7$, versus $\sim 3000$ for
trial division. Once you can factorize instantly, $\tau$, $\sigma$, $\varphi$ all follow.

**The linear sieve** builds it in exactly $O(n)$ by crossing out each composite **once**:

```cpp
for (int i = 2; i <= n; ++i) {
    if (spf[i] == 0) { spf[i] = i; primes.push_back(i); }
    for (int p : primes) {
        if (p > spf[i] || (ll)i * p > n) break;      // the crucial break
        spf[i * p] = p;
    }
}
```

The `p > spf[i]` break forces every composite to be built only as
(smallest prime factor) × (the rest) — see [proofs.md](proofs.md) §3.

> In practice the plain sieve is often *faster* despite the worse bound: it is
> cache-friendly and branch-free. Use the linear sieve when you want the prime list and
> `spf` together.

---

## Too big to fit: segmented sieve

For primes in $[L,R]$ with $R$ up to $10^{12}$, note any composite there has a prime factor
$\le\sqrt R$. So sieve the small primes up to $\sqrt R$, make a boolean window of size
$R-L+1$, and cross out multiples inside it starting from
$\max\bigl(p^2,\ \lceil L/p\rceil\,p\bigr)$. Memory is the **window**, not the range.

---

## Complexity

| sieve up to $n$ | $O(n\log\log n)$, $O(n)$ space |
|---|---|
| linear sieve | $O(n)$ |
| factorize one $m \le n$ with `spf` | $O(\log m)$ |
| primes in $[L,R]$ | $O((R-L)\log\log R + \sqrt R)$ |

One primality test of a big number → trial division. Many queries below $10^7$ → sieve
once. Need factorizations → `spf`. Range near $10^{12}$ → segmented.

There are about $\frac{n}{\ln n}$ primes below $n$ — roughly $10^6$ below $1.5\times10^7$,
so the list is cheap to store.

---

## Pitfalls

| | |
|---|---|
| forgetting $0$ and $1$ are not prime | set both `false` |
| `p * p` overflowing `int` | cast, or loop `p <= n / p` |
| starting the inner loop at $2p$ | correct but slower |
| running the outer loop to $n$ | wasted passes |
| `vector<bool>` being slow | it is a bit-array; `vector<char>` for speed |
| sieving $10^{12}$ directly | use the segmented version |
| calling `sieve()` per test case | sieve **once**, globally |
| trial-dividing when you have `spf` | factorize in $O(\log m)$ |

---

[proofs.md](proofs.md) · [implementation.cpp](implementation.cpp)
