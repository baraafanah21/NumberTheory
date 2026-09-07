# Chinese Remainder Theorem — Problems

Ten problems, hardest last. Every **check value** here was computed, not guessed — if your
code prints something else, the bug is yours. Answers at the bottom.

---

## By hand

**1. The classic.** Solve

$$x \equiv 2 \pmod 3, \qquad x \equiv 3 \pmod 5, \qquad x \equiv 2 \pmod 7$$

Give the smallest non-negative $x$ **and** the modulus of the answer.

**2. A contradiction.** Show $x \equiv 1 \pmod 6$ and $x \equiv 2 \pmod 8$ has no solution,
using a parity argument — then confirm it matches the general criterion
$\gcd(m_1,m_2) \mid (a_2-a_1)$.

**3. Not the product.** Solve $x \equiv 3 \pmod{10}$, $x \equiv 8 \pmod{15}$. The moduli
share a factor, yet the system is fine. What is the combined modulus, and what would you
have got by multiplying?

**4. One off from everything.** Find the smallest positive $x$ leaving remainder $1$ mod
$2$, $2$ mod $3$, $3$ mod $4$, …, $6$ mod $7$. There is a one-line shortcut — spot it
before merging six congruences.

---

## With code

**5. Basket of eggs.** A basket has $n$ eggs. Taken out $2$, $3$, $4$, $5$ or $6$ at a
time, one is left over; taken out $7$ at a time, none is. Find the smallest such $n$ with
`crtAll`, and the period at which solutions repeat.

**6. Detecting an inconsistent system.** Run `crtAll` on

$$x \equiv 1 \pmod 6, \qquad x \equiv 3 \pmod{10}, \qquad x \equiv 8 \pmod{15}$$

Every *pair* of moduli here shares a factor. Which pair kills it, and does the answer
depend on the order you merge them in?

**7. Reconstruct a big number.** Take $x = 123456789012345678$ and its residues modulo the
three primes $10^9+7$, $10^9+9$ and $998244353$. Recover $x$ from the three residues. Then:
what is the largest $x$ this triple can represent — and would **two** of the three have
been enough for this particular $x$?

**8. Where it overflows.** Take ten moduli near $10^9$ and compute the lcm as a big
integer: how many digits? Then explain what `crtMerge` does when the true lcm exceeds
$2^{63}$, and why using `mulMod` inside is not enough to save you.

---

## Harder

**9. Nothing is lost.** Prove that the merged congruence produced by `crtMerge` has
**exactly** the same solution set as the two it replaces. That is what licenses folding a
system pairwise instead of solving it all at once.

**10. Two formulas, one theorem.** Derive the direct formula
$x \equiv \sum_i a_i M_i y_i \pmod M$ (with $M_i = M/m_i$, $y_i = M_i^{-1} \bmod m_i$),
prove it needs **pairwise** coprimality, and give a concrete system where it silently
returns a wrong answer but `crtMerge` reports the truth.

---

## Judge problems

| where | problem | what it drills |
|---|---|---|
| CF 687B · 1800 | [Remainders Game](https://codeforces.com/problemset/problem/687/B) | when do $x \bmod c_i$ determine $x \bmod k$ — the answer is per prime power of $k$ |
| CF 919E · 2100 | [Congruence Equation](https://codeforces.com/problemset/problem/919/E) | $n\,a^n \equiv b \pmod p$: $a^n$ has period $p-1$ and $n$ lives mod $p$, so combine the two moduli |
| CF 1500B · 2200 | [Two chandeliers](https://codeforces.com/problemset/problem/1500/B) | count coincidences of two cycles up to day $t$, then binary search $t$ |
| LC 878 · Hard | [Nth Magical Number](https://leetcode.com/problems/nth-magical-number/) | two periodic conditions merged through $\mathrm{lcm}$, then counted |
| LC 1201 · Med | [Ugly Number III](https://leetcode.com/problems/ugly-number-iii/) | three periods, inclusion–exclusion, binary search |

LeetCode has no problem that needs a genuine CRT merge; the two above are the closest —
periodic conditions combined through $\mathrm{lcm}$, which is CRT with the residues all
zero.

---

## Answers

**1.** $x \equiv \mathbf{23} \pmod{\mathbf{105}}$. The moduli are pairwise coprime, so the
answer is unique mod $3\cdot5\cdot7$.

**2.** $x \equiv 1 \pmod 6$ forces $x$ odd; $x \equiv 2 \pmod 8$ forces $x$ even. By the
criterion: $\gcd(6,8) = 2$ and $a_2 - a_1 = 1$, and $2 \nmid 1$ ✓ — the same fact, stated
arithmetically.

**3.** $\gcd(10,15) = 5$ divides $8-3 = 5$, so it merges: $x \equiv \mathbf{23}
\pmod{\mathbf{30}}$. The modulus is $\mathrm{lcm}(10,15) = 30$, **not** $150$. Using $150$
would report $\frac{150}{30} = 5$ times too few solutions in any range.

**4.** Every congruence says $x \equiv -1$ modulo its own modulus, so
$x \equiv -1 \pmod{\mathrm{lcm}(2,\dots,7)}$, and $\mathrm{lcm} = 420$. Hence
$x = \mathbf{419}$.

**5.** $n \equiv 1 \pmod{\mathrm{lcm}(2,3,4,5,6)} = 1 \pmod{60}$ and $n \equiv 0 \pmod 7$,
giving $n = \mathbf{301}$, with solutions repeating every $\mathbf{420}$.

**6.** The first two merge fine ($\gcd(6,10) = 2 \mid 2$) to $x \equiv 13 \pmod{30}$, and
that then fails against $x \equiv 8 \pmod{15}$. The root cause is the pair
$(6,15)$: $\gcd = 3$, difference $8-1 = 7$, and $3 \nmid 7$. Order does **not** matter — an
inconsistent system fails whichever way you fold it, because each merge preserves the
solution set exactly (problem 9).

**7.** The residues are $148148162$, $901234595$, $751949330$, and merging them recovers
$x$ exactly. The product of all three is about $9.98\times10^{26}$, so any
$x < 9.98\times10^{26}$ is uniquely determined. Two of them would also have sufficed
*here*: $(10^9+7)(10^9+9) = 1000000016000000063 > 1.23\times10^{17} = x$ — but only just,
and that modulus is already past $2^{59}$, so every multiplication in the merge needs
`mulMod`. The rule is $\prod m_i > x$, checked against the true bound on $x$, not hoped.

**8.** The lcm of ten distinct primes near $10^9$ has $\mathbf{89}$ digits, against the
$19$ digits of $2^{63}$. `crtMerge` computes `lcm = c1.mod / g * c2.mod`, which silently
wraps; `mulMod` only protects the *products of residues*, not the modulus itself. Once the
modulus is wrong, every later merge is garbage. Detect it before it happens: check
`c1.mod / g > LLONG_MAX / c2.mod`.

**9.** Let $S$ be the set of $x$ satisfying both $x \equiv a_1 \pmod{m_1}$ and
$x \equiv a_2 \pmod{m_2}$. Any such $x$ is $\equiv$ any other modulo both $m_1$ and $m_2$,
hence modulo $\mathrm{lcm}(m_1,m_2)$ — so $S$ is contained in a single residue class mod
the lcm. Conversely, the merged class satisfies both original congruences by construction
(it was built as $a_1 + m_1t$ with $t$ chosen to fix the second). So $S$ is *exactly* the
merged class: no solutions gained, none lost, and folding is safe.

**10.** Modulo $m_j$, every term with $i \ne j$ contains the factor $M_i$, which is
divisible by $m_j$ — so it vanishes, leaving $a_jM_jy_j \equiv a_j\cdot1 = a_j$. The
construction needs $y_i = M_i^{-1} \bmod m_i$ to exist, i.e. $\gcd(M_i, m_i) = 1$, which is
precisely **pairwise** coprimality. A setwise-coprime counterexample: $x \equiv 1 \pmod 6$,
$x \equiv 3 \pmod{10}$, $x \equiv 8 \pmod{15}$ has $\gcd(6,10,15) = 1$, so the formula
happily forms $M = 900$ and returns a number — but the system is inconsistent (problem 6),
and `crtMerge` says so.

---

[README.md](README.md) · [proofs.md](proofs.md) · [implementation.cpp](implementation.cpp)
