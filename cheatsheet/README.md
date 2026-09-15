# Cheatsheet

Use this as a last-minute reference, not as a substitute for the proofs and
preconditions in the concept folders.

## Core identities

| task                              | identity or rule                                     |
| --------------------------------- | ---------------------------------------------------- |
| arithmetic progression            | $n(a_1+a_n)/2$                                       |
| count multiples of $d$ in $[1,n]$ | $\lfloor n/d\rfloor$                                 |
| gcd and lcm                       | $\operatorname{lcm}(a,b)=a/\gcd(a,b)\times b$        |
| linear combination                | $ax+by=\gcd(a,b)$ has a solution                     |
| Euler totient                     | $\varphi(n)=n\prod_{p\mid n}(1-1/p)$                 |
| divisor count                     | $\tau(n)=\prod(e_i+1)$ for $n=\prod p_i^{e_i}$       |
| sum of divisors                   | $\sigma(n)=\prod(1+p+\cdots+p^{e_i})$                |
| modular inverse                   | $a^{-1}\pmod m$ exists iff $\gcd(a,m)=1$             |
| Euler's theorem                   | $a^{\varphi(m)}\equiv1\pmod m$ only if $\gcd(a,m)=1$ |
| Fermat's little theorem           | $a^{p-1}\equiv1\pmod p$ for prime $p$ and $p\nmid a$ |
| CRT, coprime moduli               | one solution modulo $m_1m_2$                         |

## Algorithm choices

| constraints or request           | first tool                                 |
| -------------------------------- | ------------------------------------------ |
| one gcd or lcm                   | Euclidean algorithm                        |
| coefficients for $ax+by$         | extended Euclid                            |
| one inverse modulo $m$           | extended Euclid; Fermat only for prime $m$ |
| $a^b\bmod m$ with huge $b$       | binary exponentiation                      |
| all primes up to $n$             | sieve of Eratosthenes                      |
| factor many values up to a limit | SPF sieve                                  |
| divisors of one $n$              | trial division to $\sqrt n$                |
| primality of one 64-bit value    | deterministic Miller-Rabin bases           |
| factors of one hard 64-bit value | Pollard rho plus Miller-Rabin              |
| several remainder conditions     | CRT after checking compatibility           |
| solve $g^x\equiv b$              | baby-step giant-step, with group checks    |

## Implementation guardrails

- Use `long long` for values that exceed `int`; use `__int128` for a product before
  reducing modulo a 64-bit modulus.
- Normalize a remainder with `(x % mod + mod) % mod` before using it as an index or
  returning it as a canonical answer.
- In a divisor loop, test `d * d <= n` without overflow, for example with
  `d <= n / d` when the type can be tight.
- After trial division, record the leftover `n > 1`; it is a prime factor.
- Treat modulus `1` as the identity edge case: every integer is congruent to `0`.
- Never cancel or divide modulo `m` without proving the divisor is invertible.
- Check coprimality before reducing an exponent with Euler's theorem.
- Test `0`, `1`, perfect squares, a large prime factor, modulus `1`, and a non-coprime
  pair before trusting an implementation.

## Complexity at a glance

| algorithm             | complexity                                              |
| --------------------- | ------------------------------------------------------- |
| Euclid                | $O(\log\min(a,b))$                                      |
| trial division        | $O(\sqrt n)$                                            |
| binary exponentiation | $O(\log b)$ multiplications                             |
| sieve                 | $O(n\log\log n)$ time, $O(n)$ memory                    |
| baby-step giant-step  | $O(\sqrt m)$ time and memory                            |
| Miller-Rabin          | $O(k\log^3 n)$ with $k$ bases, implementation-dependent |

---

[repository root](../README.md) · [connections](../connections/README.md) ·
[mistakes](../mistakes/README.md) · [templates](../templates/README.md)
