# Modular Arithmetic — Problems

Ten problems, hardest last. Every **check value** here was computed, not guessed — if your
code prints something else, the bug is yours. Answers at the bottom.

---

## By hand

**1. The sign of `%`.** Predict what C++ prints for `-17 % 5`, `17 % -5` and `-17 % -5`.
Then say which of the three you could safely use as an index into an array of size $5$, and
write the one-line fix that makes all of them safe.

**2. Cancellation.** Show that $2\cdot3 \equiv 2\cdot0 \pmod 6$ while $3 \not\equiv 0
\pmod 6$ — so "divide both sides by $2$" is false here. Then state the modulus you *are*
allowed to cancel a factor of $4$ from, working mod $12$.

**3. Last digit.** Find the last digit of $7^{2024}$ without computing the number. Name the
length of the cycle and say why the cycle exists at all.

**4. The exponent is not reducible.** Give explicit $a$, $k$, $m$ with

$$a^{\,k} \bmod m \;\neq\; a^{\,k \bmod m} \bmod m$$

Then state the rule that *is* true, and the condition it needs. Keep your example small
enough to check on paper.

---

## With code

**5. Watch it wrap.** Take $a = b = 3037000500$ and a modulus $M = 10^9+7$. Print
`a * b % M` computed in `long long`, and again through `__int128`. Report both numbers.
Which one is right, and what does the sign of the wrong one tell you?

**6. Too big to parse.** Reduce the $100$-digit number

```text
1234567890234567890234567890234567890234567890234567890234567890234567890234567890234567890234567890
```

modulo $97$, reading it as a string. You may not use a big-integer type.

**7. A sum that overflows twice.** Compute $\bigl(1 + 2 + \cdots + 10^{18}\bigr) \bmod
(10^9+7)$. Note that $n(n+1)$ overflows `long long` *before* the division by two, and that
$n(n+1)/2$ overflows again — so neither the naive formula nor a loop will do.

**8. Verify the rules.** For every $m \in [1,20]$ and every $a,b \in [-50,50]$, check that
`norm(a+b, m)`, `norm(a-b, m)` and `norm(a*b, m)` agree with doing each operation on the
already-reduced values. Report how many of the $20 \times 101 \times 101$ triples fail.

---

## Harder

**9. Why $998244353$.** Factor $998244352$ completely. What property of that factorization
makes this modulus the one used for NTT-based problems, and why is $10^9+7$ useless for
that purpose despite also being prime?

**10. The modulus you get for free.** Unsigned overflow in C++ *is* arithmetic modulo
$2^{64}$, so a rolling hash can skip the `%` entirely. Explain why competitive programmers
still avoid it, and what property $2^{64}$ lacks that $10^9+7$ has. Then say what changes
if you use two different prime moduli at once.

---

## Judge problems

| where | problem | what it drills |
|---|---|---|
| LC 1922 · Med | [Count Good Numbers](https://leetcode.com/problems/count-good-numbers/) | $5^{\lceil n/2\rceil}4^{\lfloor n/2\rfloor} \bmod 10^9+7$ — reduce as you go, never build the value |
| LC 372 · Med | [Super Pow](https://leetcode.com/problems/super-pow/) | the exponent arrives as a digit array and $1337 = 7\cdot191$ is **composite** — exactly problem 4's trap |
| LC 1015 · Med | [Smallest Integer Divisible by K](https://leetcode.com/problems/smallest-integer-divisible-by-k/) | build $1, 11, 111, \dots$ **as residues** — the true numbers overflow immediately |
| LC 50 · Med | [Pow(x, n)](https://leetcode.com/problems/powx-n/) | the same squaring loop with no modulus, plus the negative-exponent case |

Modular arithmetic is rarely the *whole* problem — it is the substrate underneath most
counting problems. The tables in [fast power](../fast-power/),
[modular inverse](../modular-inverse/) and [Euler's totient](../euler-phi/) are where it
actually gets drilled.

---

## Answers

**1.** `-17 % 5 == -2`, `17 % -5 == 2`, `-17 % -5 == -2`. C++ truncates toward zero, so the
result takes the sign of the **dividend** — only the middle one is non-negative, and none of
them is reliable as an index. The fix is `((x % m) + m) % m`, or the branchier
`x %= m; if (x < 0) x += m;`.

**2.** $2\cdot3 = 6 \equiv 0$ and $2\cdot0 = 0 \equiv 0$, so the two sides agree; but
$3 \not\equiv 0 \pmod 6$, so the common factor cannot be dropped. The valid rule is

$$ac \equiv bc \pmod m \implies a \equiv b \ \Bigl(\mathrm{mod}\ \tfrac{m}{\gcd(c,m)}\Bigr)$$

Cancelling $4$ modulo $12$: $\gcd(4,12) = 4$, so what survives is a congruence modulo
$\frac{12}{4} = \mathbf{3}$, not modulo $12$.

**3.** Powers of $7$ mod $10$ run $7, 9, 3, 1, 7, 9, 3, 1, \dots$ — a cycle of length
$\mathbf{4}$. Since $2024 \equiv 0 \pmod 4$, the last digit is $\mathbf{1}$. The cycle
exists because there are only $10$ possible residues, so the sequence must repeat; it
returns to $1$ rather than merely repeating because $\gcd(7,10)=1$.

**4.** Take $a=2$, $k=5$, $m=3$: $2^5 = 32 \equiv \mathbf{2} \pmod 3$, while
$2^{5 \bmod 3} = 2^2 = 4 \equiv \mathbf{1} \pmod 3$. Different. The true rule reduces the
exponent modulo $\varphi(m)$, not $m$:

$$\gcd(a,m) = 1 \implies a^{\,k} \equiv a^{\,k \bmod \varphi(m)} \pmod m$$

Here $\varphi(3) = 2$ and $2^{5 \bmod 2} = 2^1 = 2$ ✓. When $\gcd(a,m) \neq 1$ even that
fails, and you need the safe form $a^{(k \bmod \varphi(m)) + \varphi(m)}$ — see
[Euler's totient](../euler-phi/).

**5.** The true product is $3037000500^2 = 9223372037000250000$, which is about
$1.45\times10^{11}$ **past** the `long long` ceiling of $9223372036854775807$. So:

| `long long` | $\mathbf{-145697812}$ |
|---|---|
| `__int128` | $\mathbf{436646196}$ |

The second is correct. The **negative sign** is the tell: a residue mod $10^9+7$ can never
be negative, so any minus sign here is proof the product wrapped. That said, do not rely on
spotting it — the wrap is just as often positive and looks perfectly ordinary.

**6.** $\mathbf{93}$. Horner's rule, reducing after every digit:
`r = (r * 10 + (c - '0')) % 97`. The value of `r` never exceeds $10\cdot96 + 9 = 969$, so
nothing overflows and the length of the input is irrelevant.

**7.** $\mathbf{1225}$. The true sum is $500000000000000000500000000000000000$, a 36-digit
number. Reduce first, then multiply: take $n \bmod M$ and $(n+1) \bmod M$, multiply them
with `mulMod`, and halve by multiplying by the inverse of $2$ — or, simpler, note that
exactly one of $n$ and $n+1$ is even and halve **that one before** reducing.

**8.** $\mathbf{0}$ failures, out of $204020$ triples. That is the whole content of the
compatibility rule: reducing early can never change the answer for $+$, $-$ or $\times$.
Adding division to that loop breaks it immediately.

**9.** $998244352 = 2^{23} \times 7 \times 17 = 2^{23} \times 119$. The point is the
$\mathbf{2^{23}}$: the multiplicative group mod $p$ has order $p-1$, so it contains an
element of order $2^{23}$ — a primitive $2^{23}$-rd root of unity. NTT needs exactly that,
which caps transform lengths at $2^{23} \approx 8.4$ million. For $10^9+7$,
$10^9+6 = 2 \times 500000003$ with $500000003$ prime, so the largest power of two dividing
it is just $2^1$. Only length-2 transforms exist. Prime is not enough; the *shape* of
$p-1$ is what matters. See [primitive roots](../primitive-roots/) for orders.

**10.** $2^{64}$ is **not prime** — in fact it is the extreme opposite, a single prime power
— so most residues have no inverse and, far worse, the modulus has many small factors to
exploit. Anti-hash tests built with Thue–Morse strings force collisions on any
$2^{64}$-based polynomial hash in a few thousand characters, and they are standard on
Codeforces. A prime modulus has no such structure to attack.

Using two independent prime moduli multiplies the collision probability rather than adding
to it: two 31-bit primes behave like one 62-bit modulus, so a random collision needs about
$2^{31}$ pairs instead of $2^{15}$. It also defeats the fixed anti-hash tests, since they
are constructed against one specific modulus. Randomizing the **base** at run time is the
other half of the defence.

---

[README.md](README.md) · [implementation.cpp](implementation.cpp)
