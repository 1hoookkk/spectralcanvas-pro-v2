# Digital Audio DSP (Zölzer) — Chunk 9/30

> Source: `Udo-Zolzer-digital-audio-signal-processing.9780470997857.40435.pdf` · Pages ~1-331 · Extractor: pdfminer.six
> Target ≈ 2800 words, Overlap = 200 words · Actual = 803

> Note: DSP textbook content for SpectralCanvasPro RT-audio knowledge base

---
49
a) Unweighted PSDs
(cid:111)
B
d
/
)
f
(
E
E
S
(cid:111)
B
d
/
)
f
(
E
E
S
40
20
0
(cid:358)20
(cid:358)40
0
0
(cid:358)10
(cid:358)20
(cid:358)30
(cid:358)40
0
He8
Wa3
Wa9
2
4
6
8
10
f / kHz (cid:111)
12
14
16
18
20
b) Weighted PSDs
He8
Wa3
Wa9
2
4
6
8
f / kHz (cid:111)
10
12
14
16
Figure 2.33 Power density spectrum of three ﬁlter approximations (Wa3 third-order ﬁlter, Wa9 ninth-
order ﬁlter, He8 eighth-order ﬁlter [Wan92, Hel07]): (a) unweighted PSDs, (b) inverse ISO 389-7
weighted PSDs.
truncation curves for 2’s complement number representation. The absolute error shown in
Fig. 2.37 is given by e = xQ − x.
Table 2.2 Rounding and truncation of 2s complement numbers.
Type
2’s c. (r)
2’s c. (t)
Quantization
xQ = Q(cid:6)Q
xQ = Q(cid:6)Q
Error limits
−1x + 0.5(cid:7) −Q/2 ≤ xQ − x ≤ Q/2
−1x(cid:7)
−Q ≤ xQ − x ≤ 0
Digital audio signals are coded in the 2’s complement number representation. For 2’s
complement representation, the range of values from −Xmax to +Xmax is normalized to
the range −1 to +1 and is represented by the weighted ﬁnite sum xQ = −b0 + b1 · 0.5 +
b2 · 0.25 + b3 · 0.125 + · · · + bw−1 · 2−(w−1). The variables b0 to bw−1 are called bits and
can take the values 1 or 0. The bit b0 is called the MSB (most signiﬁcant bit) and bw−1
is called the LSB (least signiﬁcant bit). For positive numbers, b0 is equal to 0 and for

50
x 10(cid:358)4
Quantization
(cid:111)
)
n
(
x
(cid:111)
)
n
(
x
Q
+Q
(cid:358)Q
100
200
300
400
0
x 10(cid:358)4
500
n (cid:111)
600
700
800
900
1000
1
0
(cid:358)1
1
0
(cid:358)1
0
100
200
300
400
(cid:358)80
(cid:358)100
(cid:358)120
(cid:358)140
(cid:111)
B
d
n
i
|
)
f
/
f
(
S
S
Q
Q
X
X
|
0
0.05
0.1
0.15
0.2
600
700
800
900
1000
0.3
0.35
0.4
0.45
500
n (cid:111)
0.25
f/f
S (cid:111)
Figure 2.34 Psychoacoustic noise shaping: signal x(n), quantized signal xQ(n) and power density
spectrum of quantized signal.
,(cid:16)(cid:7)
2(cid:22)
(cid:5)(cid:23) 3
23
(cid:23)I
(cid:5)(cid:22)
(cid:14)(cid:23)
(cid:5)(cid:23)
(cid:14)(cid:23)
(cid:23)
(cid:22)
3
(cid:5)(cid:23)I
(cid:14)(cid:23)
(cid:5)23
(cid:14)(cid:23)
(cid:5)2(cid:22)
(cid:14)(cid:23)
(cid:6)(cid:16)(cid:3)(cid:10)(cid:2)-(cid:14)2'(cid:18)(cid:14)$(cid:11)(cid:12)(cid:28)(cid:4)(cid:2)(cid:12)(cid:2)(cid:10)(cid:7)
(cid:14)(cid:23) (cid:5)(cid:22)
(cid:5)(cid:23)
(cid:14)(cid:23)
(cid:5)2
(cid:14)(cid:23)
(cid:5)23
(cid:14)(cid:23)
(cid:5)2(cid:22)
(cid:14)(cid:23)
(cid:5)2(cid:23)
(cid:14)(cid:23)
!(cid:10)(cid:18)(cid:16)(cid:3)(cid:10)(cid:2)-(cid:14)2'(cid:18)(cid:14)$(cid:11)(cid:12)(cid:28)(cid:4)(cid:2)(cid:12)(cid:2)(cid:10)(cid:7)
(cid:5)(cid:23) 2(cid:22)
(cid:14)(cid:23) 23
(cid:14)(cid:23) (cid:23)I
(cid:14)(cid:23) (cid:23)
(cid:22)(cid:14)(cid:23)
(cid:14)(cid:23) 3
(cid:6)(cid:16)(cid:3)(cid:10)(cid:2)-(cid:14)(cid:27)(cid:10)(cid:7)(cid:2)(cid:3)(cid:2)(cid:8)
2(cid:22)
(cid:14)(cid:23)
(cid:14)(cid:23) 23
(cid:14)(cid:23) (cid:23)I
(cid:14)(cid:23) (cid:23)
(cid:22)(cid:14)(cid:23)
(cid:14)(cid:23) 3
!(cid:10)(cid:18)(cid:16)(cid:3)(cid:10)(cid:2)-(cid:14)(cid:27)(cid:10)(cid:7)(cid:2)(cid:3)(cid:2)(cid:8)
Figure 2.35 Fixed-point formats.
negative numbers b0 equals 1. For a 3-bit quantization (see Fig. 2.38), a quantized value can
be represented by xQ = −b0 + b1 · 0.5 + b2 · 0.25. The smallest quantization step size is
0.25. For a positive number 0.75 it follows that 0.75 = −0 + 1 · 0.5 + 1 · 0.25. The binary
coding for 0.75 is 011.

2.4 Number Representation
