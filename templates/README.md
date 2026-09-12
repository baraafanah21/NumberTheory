# Templates

Contest-ready code, stripped of the explanations. Each file is the algorithm and nothing
else — no `main`, no demo, no self-check. When you want to know *why* a line is there, the
matching concept folder explains it.

Every function here was lifted from a `concepts/*/implementation.cpp` in this repository
and left unchanged apart from dropping comments. Each one is tested against brute force
before being committed.

---

## What to grab

| file | gives you | needs | concept |
|---|---|---|---|
| [gcd.cpp](gcd.cpp) | `gcd`, `lcm` | — | [gcd](../concepts/gcd/) |
| [extgcd.cpp](extgcd.cpp) | `extgcd`, `diophantine` | — | [extended-euclid](../concepts/extended-euclid/) |
| [modpow.cpp](modpow.cpp) | `norm`, `mulMod`, `powMod` | — | [fast-power](../concepts/fast-power/) |
| [modinv.cpp](modinv.cpp) | `modInverse`, `inverseTable`, `solveLinearCongruence` | modpow, extgcd | [modular-inverse](../concepts/modular-inverse/) |
| [ncr-mod-p.cpp](ncr-mod-p.cpp) | `Binomial` — `choose`, `perm` | modpow | [modular-inverse](../concepts/modular-inverse/) |
| [sieve.cpp](sieve.cpp) | `sieve` | — | [sieve](../concepts/sieve/) |
| [spf-sieve.cpp](spf-sieve.cpp) | `spfSieve`, `factorizeSpf` | — | [sieve](../concepts/sieve/) |
| [segmented-sieve.cpp](segmented-sieve.cpp) | `primesInRange` | sieve | [sieve](../concepts/sieve/) |
| [divisor-sieve.cpp](divisor-sieve.cpp) | `tauUpTo`, `sigmaUpTo` | — | [divisors](../concepts/divisors/) |
| [factorize.cpp](factorize.cpp) | `factorize`, `countDivisors`, `sumDivisors`, `allDivisors` | — | [prime-factorization](../concepts/prime-factorization/) |
| [phi.cpp](phi.cpp) | `phi`, `phiSieve` | — | [euler-phi](../concepts/euler-phi/) |
| [crt.cpp](crt.cpp) | `crtMerge`, `crtAll` | modpow, extgcd | [CRT](../concepts/chinese-remainder-theorem/) |
| [miller-rabin.cpp](miller-rabin.cpp) | `isPrime`, deterministic to $2^{64}$ | modpow | [miller-rabin](../concepts/miller-rabin/) |
| [pollard-rho.cpp](pollard-rho.cpp) | `pollardRho`, `factorizeBig` | modpow, gcd, miller-rabin | [pollard-rho](../concepts/pollard-rho/) |
| [bsgs.cpp](bsgs.cpp) | `discreteLog` | modpow | [primitive-roots](../concepts/primitive-roots/) |

**Paste the dependencies first.** `modpow.cpp` is the one most things need, because
`mulMod` and `norm` live there.

---

## Which tool, at a glance

| situation | file |
|---|---|
| primes below $10^7$, many queries | `sieve.cpp` |
| factorize many numbers below $10^7$ | `spf-sieve.cpp` |
| primes in $[l,r]$ with $r \le 10^{12}$ | `segmented-sieve.cpp` |
| factorize one $n \le 10^{12}$ | `factorize.cpp` |
| is one $n \le 9\times10^{18}$ prime | `miller-rabin.cpp` |
| factorize one $n \le 9\times10^{18}$ | `pollard-rho.cpp` |
| $\tau$ or $\sigma$ over a whole range | `divisor-sieve.cpp` |
| divide under a modulus | `modinv.cpp` |
| $\binom nk \bmod p$, many queries | `ncr-mod-p.cpp` |
| several simultaneous congruences | `crt.cpp` |
| solve $g^x \equiv b$ | `bsgs.cpp` |

---

## Rules these follow

* **`long long` everywhere.** `int` overflows at $2\times10^9$, which most of these pass.
* **`__int128` for `mulMod`.** `a * b % m` wraps silently once $m > \sim3\times10^9$.
* **`1 % m`, never `1`**, for any accumulator — the modulus can be $1$.
* **`d <= n / d`, never `d * d <= n`** — the product overflows near $3\times10^9$.
* **`(a / g) * b` for lcm**, never `a * b / g`.
* **Normalize before returning** anything that could be negative.

---

## What they deliberately do not do

No namespaces, no templates, no operator overloading, no `ModInt` class. Every function
takes the modulus as an argument, because in a contest you will want to change it, mix two
of them, or print an intermediate value. Abstraction costs more time than it saves at this
size.

They also do not validate input. `pollardRho` on a prime never returns, `factorizeSpf`
past the table bound reads out of range. The concept READMEs list the preconditions; these
files assume you met them.

---

[repository root](../README.md) · [concepts](../concepts/) · [problems](../problems/README.md)
