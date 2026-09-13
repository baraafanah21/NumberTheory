# Pattern: $n$ is up to $10^{18}$

> *"The bound is astronomically larger than any loop could reach, but the state is tiny."*

When $n$ is enormous and the thing being repeated is small and fixed, the answer is a
$\log n$ algorithm. This pattern is how you find it.

---

## Recognition clues

* a bound of $10^{18}$, or an exponent arriving as a **string of digits**
* **"after $n$ operations"**, "on day $n$", "the $n$-th term", with $n$ huge
* a **linear recurrence** $f(n) = c_1f(n-1) + \cdots + c_kf(n-k)$ and $n$ past $10^7$
* **"how many paths of length exactly $n$"** in a small graph
* applying the same transformation $n$ times — a shuffle, a matrix, a function
* a **tower** of exponents $a^{b^{c}}$
* $n$ doublings of anything
* Fibonacci, tribonacci, or any fixed-depth recurrence at absurd $n$

---

## Key observation

**Squaring doubles the exponent for one multiplication.** Walk the bits of $n$:

$$a^{13} = a^{8}\cdot a^{4}\cdot a^{1}, \qquad 13 = 1101_2$$

$\lfloor\log_2 n\rfloor$ squarings plus $\mathrm{popcount}(n)-1$ multiplications — at most
$126$ operations for any 64-bit exponent.

The part worth internalising: **nothing in that argument used numbers**. Binary
exponentiation works for any associative operation with an identity, so the same loop gives
matrix powers, permutation powers, function iteration, and cheapest-path-with-exactly-$n$-edges
under $\max$-plus.

When the *exponent itself* is too large to hold, shrink it — but only with the right
modulus:

$$\gcd(a,m)=1 \implies a^{\,k} \equiv a^{\,k \bmod \varphi(m)} \pmod m$$

$$\text{for any } a, \text{ once } k \ge \log_2 m: \quad a^{\,k} \equiv a^{\,(k \bmod \varphi(m)) + \varphi(m)} \pmod m$$

---

## Relevant concepts

[fast-power](../concepts/fast-power/) ·
[modular-arithmetic](../concepts/modular-arithmetic/) ·
[euler-phi](../concepts/euler-phi/) · [primitive-roots](../concepts/primitive-roots/)

---

## Typical approach

1. Identify the **state**. If it is a single number, plain `powMod`. If it is $k$ numbers
   updated linearly, a $k\times k$ matrix.
2. Write the transition as a matrix and confirm it is **constant** — matrix power needs the
   same transition every step.
3. Raise it with the same bit loop, reducing **every entry** at every multiply.
4. Exponent given as digits? Horner on the exponent:
   $a^{10k+d} = (a^{k})^{10}\cdot a^{d}$.
5. Exponent needs reducing and the modulus is composite? Use the safe $+\varphi(m)$ form,
   and recurse for a tower — $\varphi$ reaches $1$ in $O(\log m)$ levels.
6. Sanity-check against a brute-force loop for small $n$ **before** trusting it at
   $10^{18}$. Matrix code is easy to get subtly wrong.

---

## Complexity

| $a^n \bmod m$ | $O(\log n)$ |
|---|---|
| $k\times k$ matrix power | $O(k^3\log n)$ |
| exponent as $d$ decimal digits | $O(d\log 10)$ multiplications |
| tower of exponents mod $m$ | $O(\log m)$ levels of $\varphi$ |
| discrete log, the hard direction | $O(\sqrt m)$ |

A $2\times2$ matrix at $n = 10^{18}$ is about $60$ matrix multiplies, so roughly $500$
scalar multiplications. Free.

---

## Common traps

| | |
|---|---|
| reducing the exponent mod $m$ | it is mod $\varphi(m)$ |
| the $\varphi$ reduction when $\gcd(a,m) \neq 1$ | use the $+\varphi(m)$ safe form |
| `base * base % m` above $m \sim 3\times10^9$ | needs `__int128` |
| not reducing inside the matrix multiply | two products near $10^9$ overflow when summed |
| `r = 1` when $m$ may be $1$ | write `1 % m` |
| a negative exponent | that is an inverse, not a power |
| `std::pow` for integer powers | returns `double`, wrong past $2^{53}$ |
| `exp >>= 1` on a signed negative value | infinite loop |
| a transition that is not actually constant | matrix power does not apply |
| $0^0$ | this loop returns $1$; check what the problem wants |

---

## Example problems here

| where | problem | the shape |
|---|---|---|
| CF 913A · 800 | [Modular Exponentiation](https://codeforces.com/problemset/problem/913/A) | $n \le 10^8$ makes the naive loop the trap |
| LC 50 · Med | [Pow(x, n)](https://leetcode.com/problems/powx-n/) | the loop itself, plus negative exponents |
| LC 1922 · Med | [Count Good Numbers](https://leetcode.com/problems/count-good-numbers/) | $n \le 10^{15}$, two powers multiplied |
| LC 2550 · Med | [Count Collisions of Monkeys](https://leetcode.com/problems/count-collisions-of-monkeys-on-a-polygon/) | $2^n - 2$, with the subtraction going negative |
| LC 372 · Med | [Super Pow](https://leetcode.com/problems/super-pow/) | exponent as a digit array, composite modulus $1337 = 7\cdot191$ |
| CF 906D · 2700 | [Power Tower](https://codeforces.com/problemset/problem/906/D) | a genuine tower; recurse with the safe form since $\gcd$ is never guaranteed |
| CF 1106F · 2400 | [Lunar New Year and a Recursive Sequence](https://codeforces.com/problemset/problem/1106/F) | discrete log, then matrix-power the exponents mod $p-1$, then exponentiate back |

The matrix half of this pattern is under-represented in the repository. Any "linear
recurrence at $n = 10^{18}$" problem fills that gap, and
[fast-power](../concepts/fast-power/) ships a tested $2\times2$ implementation to start
from.

---

[patterns index](README.md) · [concepts](../concepts/) · [problems](../problems/README.md)
