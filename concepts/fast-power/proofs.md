# Proofs — Fast Power (Binary Exponentiation)

**Borrowed** from [modular arithmetic](../modular-arithmetic/proofs.md): congruence
survives $\times$, hence $a \equiv b \Rightarrow a^k \equiv b^k$ (§3), so every
intermediate may be reduced; the unique representative in $[0,m)$ (§2); and the overflow
threshold that forces `mulMod` (§4). From
[modular inverse](../modular-inverse/proofs.md): **Fermat**, $a^{\,p-1} \equiv 1$ for a
prime $p \nmid a$ (§3), used only in §6.

The algorithm is four lines and the proof is one invariant. What the proof is *worth* is
§3: it never mentions numbers, which is why the same loop computes matrix powers, function
iterates and multiplication itself.

---

## 1. The loop is correct

**Claim.** For $n \ge 0$, `powMod(a, n, m)` returns $a^{\,n} \bmod m$.

**Proof.** Track the quantity

$$I \;=\; r \cdot \mathtt{base}^{\,\mathtt{exp}}$$

*Initially* $r = 1$, $\mathtt{base} = a$, $\mathtt{exp} = n$, so $I = a^{\,n}$.

*Each iteration leaves $I$ unchanged.* Write $e = \mathtt{exp} > 0$ and $c = \mathtt{base}$.

- $e$ **even**, $e = 2k$. The new state is $(r,\ c^2,\ k)$, and
  $r\,(c^2)^k = r\,c^{2k} = r\,c^{\,e}$.
- $e$ **odd**, $e = 2k+1$. The new state is $(r c,\ c^2,\ k)$, and
  $(rc)\,(c^2)^k = r\,c^{\,2k+1} = r\,c^{\,e}$.

*It terminates.* $e \ge 1$ gives $\lfloor e/2 \rfloor < e$, so `exp` strictly decreases and
is bounded below by $0$.

*At exit* $\mathtt{exp} = 0$, so $I = r \cdot \mathtt{base}^{\,0} = r$. Since $I$ never
changed, $r = a^{\,n}$. $\blacksquare$

Under a modulus every equality above is a congruence instead, valid because congruence
survives multiplication (borrowed). Nothing else changes.

**The one place order matters.** In the odd case $r$ is multiplied by the **old** `base`,
before the squaring. The code does exactly that:

```cpp
if (exp & 1) r = mulMod(r, base, m);   // old base
base = mulMod(base, base, m);          // then square
```

Swapping the two lines multiplies in $c^2$ where the invariant needs $c$, and the result is
wrong for every $n$ with more than one set bit — a transposition that still compiles, still
terminates, and still returns a plausible number.

---

## 2. The exact cost

**Claim.** For $n \ge 1$ with $b = \lfloor\log_2 n\rfloor + 1$ bits, the loop runs exactly
$b$ times and performs exactly $b + \mathrm{popcount}(n)$ calls to `mulMod`.

**Proof.** `exp` is right-shifted once per iteration and the loop ends when it reaches $0$,
so the iteration count is the number of bits, $b$. Each iteration squares once —
unconditionally — giving $b$ squarings; and multiplies into $r$ exactly when the current
bit is set, giving $\mathrm{popcount}(n)$ further calls. $\blacksquare$

**Reconciling that with the README's count.** Two of those calls are doing no work. The
squaring in the final iteration produces a `base` that is never read again, and the first
multiply into $r$ has $r = 1$. Discounting both,

$$\lfloor\log_2 n\rfloor \ \text{squarings} \;+\; \bigl(\mathrm{popcount}(n)-1\bigr) \ \text{multiplications}$$

which is the README's figure. The code keeps the two redundant calls because branching to
avoid them costs more than they do.

**For a 64-bit exponent** the worst case is $n = 2^{64}-1$: $b = 64$ and
$\mathrm{popcount} = 64$, so $128$ calls, of which $126$ do work. That is the whole budget
for an exponent of $10^{18}$ — the reason the concept exists.

---

## 3. Nothing above used numbers

**Claim.** The proof of §1 holds verbatim in any **monoid**: a set with an associative
binary operation and an identity element.

**Proof.** Inspect what §1 actually invoked.

1. $c^{\,2k} = (c^2)^{k}$ — regrouping a product, which is **associativity** and nothing
   else.
2. $r = 1$ starts the invariant at $a^{\,n}$ — an **identity** element.
3. $\mathtt{base}^{\,0} = 1$ at exit — the same identity.

Commutativity was never used. Inverses were never used. That the elements are integers was
never used. $\blacksquare$

**Which is why the table in the README is not a list of tricks.** Each row is a monoid:

| the monoid | identity | $a^n$ means |
|---|---|---|
| integers mod $m$ under $\times$ | $1$ | modular power |
| $k \times k$ matrices under $\times$ | $I_k$ | linear recurrences, walk counts |
| permutations under composition | the identity permutation | applying a shuffle $n$ times |
| functions under composition | $\mathrm{id}$ | the $n$-th iterate |
| integers mod $m$ under $+$ | $0$ | **multiplication** by $n$ |

**Justifies `matPow`** ([implementation.cpp:95](implementation.cpp#L95)) as the identical
loop — matrix multiplication is associative, and its non-commutativity is harmless because
every factor in sight is a power of the same $A$, and powers of one element always commute.

**Justifies `mulModByDoubling`** ([implementation.cpp:47](implementation.cpp#L47)) as the
last row: replace $\times$ by $+$, squaring by doubling, and the identity $1$ by $0$ — note
the function starts `r = 0`, not `r = 1`, which is that substitution and not a typo. It
computes $a \cdot b \bmod m$ with no `__int128`, using only additions that stay below $2m$.

---

## 4. The three guards before the loop

**$m$ may be $1$.** Then every integer is congruent to $0$, and by the borrowed uniqueness
the only representative in $[0,1)$ is $0$. So $a^{\,0}$ must be reported as $0$, not $1$:

```cpp
ll r = 1 % m;                       // not  ll r = 1;
```

The literal $1$ is the identity of the *monoid*; `1 % m` is its representative in the
output range. They differ in exactly one case, and that case is a favourite of problem
setters ([implementation.cpp:21](implementation.cpp#L21)).

**The base may be negative or oversized.** `norm` is valid here precisely because
congruence survives multiplication: swapping `base` for its residue changes no later class,
so $(-3)^3 \bmod 7$ may be computed as $4^3 \bmod 7 = 64 \bmod 7 = 1$ — and indeed
$-27 \equiv 1 \pmod 7$.

**The exponent may not be negative.** Two independent reasons, either sufficient:

- $a^{-k}$ is not a power but an **inverse**, $(a^{-1})^{k}$, which requires
  $\gcd(a,m)=1$ and is a different concept.
- Mechanically, `exp >>= 1` on a negative signed value is an arithmetic shift: the sign bit
  is copied, $-1$ shifts to $-1$, and `exp > 0` is never reached from below. The loop does
  not return a wrong answer, it does not return.

Hence the guard at [implementation.cpp:20](implementation.cpp#L20).

---

## 5. Horner in the exponent

**Claim.** For a decimal string denoting $N = \sum_i d_i 10^{\,L-1-i}$, the recurrence

$$r_0 = 1, \qquad r_{i+1} = \bigl(r_i\bigr)^{10} \cdot a^{\,d_i}$$

ends at $r_L \equiv a^{\,N} \pmod m$.

**Proof.** Let $N_i$ be the value of the first $i$ digits, so $N_{i+1} = 10N_i + d_i$ and
$N_L = N$. Induct: $r_0 = 1 = a^{\,N_0}$ with $N_0 = 0$, and if $r_i \equiv a^{\,N_i}$ then

$$r_{i+1} \equiv \bigl(a^{\,N_i}\bigr)^{10} \cdot a^{\,d_i} = a^{\,10N_i + d_i} = a^{\,N_{i+1}}$$

using only the exponent law $(a^{x})^{y} = a^{xy}$ and $a^x a^y = a^{x+y}$. $\blacksquare$

**The contrast with the same trick on a value.**
[Modular arithmetic §6](../modular-arithmetic/proofs.md) reduces a huge decimal *value* mod
$m$ by reducing after every digit. Here the digits build an **exponent**, and the exponent
may not be reduced mod $m$ at all — so the point is different: Horner means $N$ is never
held in any integer type, because it is consumed ten-fold at a time. Each step costs a
`powMod(·, 10, ·)` plus one small power, so an $L$-digit exponent costs
$O(L\log 10)$ multiplications ([implementation.cpp:64](implementation.cpp#L64)).

*If you do want to shrink the exponent rather than stream it, that is
$\varphi(m)$ and a coprimality condition — see [Euler's totient](../euler-phi/proofs.md) §4,
not this page.*

---

## 6. The Fermat inverse, and where it goes silently wrong

**Borrowed claim.** For a prime $p$ and $p \nmid a$: $a^{\,p-1} \equiv 1 \pmod p$, hence
$a \cdot a^{\,p-2} \equiv 1$ and $a^{-1} \equiv a^{\,p-2} \pmod p$. Proved in
[modular inverse](../modular-inverse/proofs.md) §3; not reproved here.

**Justifies `modInversePrime`** ([implementation.cpp:77](implementation.cpp#L77)) as one
`powMod` call, $O(\log p)$.

**The precondition is not decorative.** On a composite modulus $a^{\,m-2}$ is simply some
other residue, returned without complaint. At $m = 9$, $a = 2$:

$$2^{\,7} = 128 \equiv 2 \pmod 9, \qquad \text{yet} \qquad 2 \cdot 2 = 4 \not\equiv 1 \pmod 9$$

The true inverse is $5$, since $2 \cdot 5 = 10 \equiv 1$. The call returns $2$, the program
continues, and the wrong answer surfaces somewhere else entirely. For a composite modulus
use the extended-Euclid inverse, which also *reports* when none exists.
