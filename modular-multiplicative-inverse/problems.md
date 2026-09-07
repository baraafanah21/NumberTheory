# Modular Multiplicative Inverse — Problems

Ten problems, hardest last. Every **check value** here was computed, not guessed — if your
code prints something else, the bug is yours. Answers at the bottom.

---

## By hand

**1. Two inverses.** Find $3^{-1} \bmod 11$ and $10^{-1} \bmod 17$. Small enough to hunt
by hand — but say which method you would use at scale.

**2. Who has one.** Which of $1,\dots,11$ have an inverse mod $12$? Count them, then name
the function of $12$ that predicts the count.

**3. A fraction.** Compute $\frac73 \bmod 11$, and verify the result really behaves like
"seven divided by three" — multiply it back by $3$.

**4. Fermat, misapplied.** Someone computes $5^{-1} \bmod 12$ as $5^{\,12-2} \bmod 12$.
What do they get, what is the truth, and why does the code not crash or warn?

---

## With code

**5. Binomials mod a prime.** Precompute factorials and inverse factorials mod $10^9+7$ and
report $\binom{1000}{500}$ and $\binom{10^6}{500000}$. Use **one** exponentiation for the
whole inverse-factorial table, not one per entry.

**6. Not coprime.** Solve $14x \equiv 30 \pmod{100}$ — all solutions in $[0,100)$. How
many should there be before you start, and how far apart?

**7. The linear table.** Build $1^{-1},\dots,16^{-1}$ mod $17$ with

```cpp
inv[1] = 1;
for (int i = 2; i <= n; ++i) inv[i] = (p - p / i) * inv[p % i] % p;
```

Check every entry against $i\cdot\mathrm{inv}[i] \equiv 1$. Then time it against calling
`modInverse` in a loop for $n = 10^6$.

**8. A constant you will recognize.** Compute $2^{-1} \bmod (10^9+7)$. You will see this
number in other people's code — what is it "really"?

---

## Harder

**9. Exactly $g$ solutions.** Prove that $ax \equiv b \pmod m$ is solvable iff
$g = \gcd(a,m)$ divides $b$, and that it then has **exactly $g$** solutions mod $m$, spaced
$m/g$ apart. Where does the standard "multiply by $a^{-1}$" argument break down when
$g > 1$?

**10. Why the table recurrence works.** Derive
$\ \mathrm{inv}[i] = -\lfloor p/i\rfloor\cdot\mathrm{inv}[p \bmod i] \bmod p$
from $p = \lfloor p/i\rfloor\, i + (p \bmod i)$. Then explain why the recursion always
terminates, and why `p - p / i` appears in the code instead of a minus sign.

---

## Judge problems

| where | problem | what it drills |
|---|---|---|
| CF 1514C · 1600 | [Product 1 Modulo N](https://codeforces.com/problemset/problem/1514/C) | pair each unit with its inverse; the leftovers are the self-inverse ones — problem 2 taken seriously |
| CF 495B · 1600 | [Modular Equations](https://codeforces.com/problemset/problem/495/B) | solve $n \bmod x = k$ for $x$ — a congruence read the other way round |
| CF 300C · 1800 | [Beautiful Numbers](https://codeforces.com/problemset/problem/300/C) | $\binom nk \bmod p$ over many $k$ — problem 5 with a time limit |
| LC 2400 · Med | [Number of Ways to Reach a Position After Exactly k Steps](https://leetcode.com/problems/number-of-ways-to-reach-a-position-after-exactly-k-steps/) | one binomial mod $10^9+7$ |
| LC 1922 · Med | [Count Good Numbers](https://leetcode.com/problems/count-good-numbers/) | fast exponentiation mod $p$, the engine behind Fermat inverses |
| LC 1735 · Hard | [Count Ways to Make Array With Product](https://leetcode.com/problems/count-ways-to-make-array-with-product/) | stars-and-bars binomials mod $p$ — needs the whole factorial + inverse-factorial setup |

---

## Answers

**1.** $3^{-1} \equiv \mathbf 4 \pmod{11}$ ($3\cdot4 = 12$); $10^{-1} \equiv \mathbf{12}
\pmod{17}$ ($10\cdot12 = 120 = 7\cdot17+1$). At scale: Fermat if the modulus is prime,
extended Euclid otherwise.

**2.** $\mathbf{1, 5, 7, 11}$ — exactly the residues coprime to $12$, and there are
$\varphi(12) = 4$ of them. Mod a composite, most residues have no inverse at all.

**3.** $3^{-1} = 4$, so $\frac73 \equiv 7\cdot4 = 28 \equiv \mathbf 6 \pmod{11}$. Check:
$3\cdot6 = 18 \equiv 7$ ✓.

**4.** They get $5^{10} \equiv \mathbf 1 \pmod{12}$; the true inverse is $\mathbf 5$
($5\cdot5 = 25 \equiv 1$). Nothing warns, because Fermat's theorem is simply *false* for a
composite modulus — the exponentiation succeeds and returns a number that happens to be
wrong. This is the single most expensive mistake in the folder.

**5.** $\binom{1000}{500} \equiv \mathbf{159835829}$ and $\binom{10^6}{500000} \equiv
\mathbf{996692777} \pmod{10^9+7}$. Build `fact[]` forward, invert `fact[n]` **once** with
Fermat, then walk `invfact[]` backwards with `invfact[i-1] = invfact[i] * i`.

**6.** $g = \gcd(14,100) = 2$ divides $30$, so there are exactly $2$ solutions, $50$ apart:
$x \in \{\mathbf{45},\ \mathbf{95}\}$. Dividing through by $g$ gives $7x \equiv 15
\pmod{50}$, whose unique solution is $x \equiv 15\cdot7^{-1} \equiv 15\cdot43 \equiv 45$.
Reporting only $45$ is the classic half-right answer.

**7.** Mod $17$: $1, 9, 6, 13, 7, 3, 5, 15, 2, 12, 14, 10, 4, 11, 8, 16$. The table is
$O(n)$ against $O(n\log p)$ for the loop of `modInverse` calls — at $n = 10^6$ that is
roughly a $30\times$ difference in multiplications.

**8.** $2^{-1} \equiv \mathbf{500000004} \pmod{10^9+7}$, which is $\frac{p+1}{2}$ — the
modular version of "one half". Seeing it in a codebase almost always means someone divided
by $2$.

**9.** ($\Rightarrow$) If $ax \equiv b$ then $m \mid ax - b$, and $g$ divides both $a$ and
$m$, hence $g \mid b$. ($\Leftarrow$) Write $a = ga'$, $b = gb'$, $m = gm'$ with
$\gcd(a',m') = 1$; the congruence becomes $a'x \equiv b' \pmod{m'}$, which has the unique
solution $x_0 \equiv b'(a')^{-1} \pmod{m'}$. Lifting back to modulus $m$, the solutions are
$x_0, x_0+m', \dots, x_0+(g-1)m'$ — exactly $g$ of them, spaced $m' = m/g$. The
"multiply by $a^{-1}$" argument breaks at the first step: when $g>1$, $a^{-1} \bmod m$ does
not exist, so there is nothing to multiply by.

**10.** Reduce $p = \lfloor p/i\rfloor\,i + (p \bmod i)$ mod $p$:
$\ \lfloor p/i\rfloor\,i + (p \bmod i) \equiv 0$. Multiply by
$i^{-1}(p \bmod i)^{-1}$:

$$\lfloor p/i\rfloor\,(p \bmod i)^{-1} + i^{-1} \equiv 0
\quad\Longrightarrow\quad i^{-1} \equiv -\Bigl\lfloor \frac pi\Bigr\rfloor\,(p \bmod i)^{-1}$$

It terminates because $p \bmod i < i$, so every lookup is at a strictly smaller index
already filled in. `p - p / i` is just $-\lfloor p/i\rfloor$ made non-negative, so the
product never goes negative — the same normalization habit as everywhere else in this
folder.

---

[README.md](README.md) · [proofs.md](proofs.md) · [implementation.cpp](implementation.cpp)
