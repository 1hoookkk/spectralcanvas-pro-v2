# Digital Audio DSP (Zölzer) — Chunk 5/30

> Source: `Udo-Zolzer-digital-audio-signal-processing.9780470997857.40435.pdf` · Pages ~1-331 · Extractor: pdfminer.six
> Target ≈ 2800 words, Overlap = 200 words · Actual = 325

> Note: DSP textbook content for SpectralCanvasPro RT-audio knowledge base

---
38
(cid:111)
)
V
(
R
d
d
n
a
)
V
(
¯g
1
0.9
0.8
0.7
0.6
0.5
0.4
0.3
0.2
0.1
0
Bipolar RECT Dither
¯g(V)
....................................................................................................
dR(V)
0
0.2
0.4
0.6
V/Q (cid:111)
0.8
1
(cid:111)
)
V
(
R
d
d
n
a
)
V
(
¯g
1
0.9
0.8
0.7
0.6
0.5
0.4
0.3
0.2
0.1
0
Quantization
Bipolar TRI Dither
¯g(V)
dR(V)
....................................................................................................
0
0.2
0.4
0.6
V/Q (cid:111)
0.8
1
Figure 2.20 Rounding – linearizing and suppression of noise modulation (s = 4, m = 1).
1. Mean dither power d 2:
d 2 = 1
Q
= 1
Q
(cid:3)
Q
0
(cid:3)
Q
d 2
R(V ) dV
(cid:6)
0
k
{g(V + dk) − g(V )}2P (dk) dV .
(This is equal to the deviation from mean output in accordance with (2.83).)
2. Mean of total noise power d 2
tot:
(cid:3)
d 2
tot
= 1
Q
(cid:6)
Q
0
k
{g(V + dk) − V }2P (dk) dV .
(This is equal to the deviation from an ideal straight line.)
In order to derive a relationship between d 2
tot and d 2, the quantization error given by
Q(V + dk) = g(V + dk) − (V + dk)
is used to rewrite (2.88) as
(cid:3)
(cid:6)
d 2
tot
=
k
(cid:6)
=
P (dk)
P (dk)
1
Q
1
Q
Q
0
(cid:3)
Q
0
d 2
k P (dk)
1
Q
k
+
(cid:6)
k
(Q2(V + dk) + 2dkQ(V + dk) + d 2
(cid:6)
k
dkP (dk)
Q2(V + dk) dV + 2
(cid:3)
Q
dV .
0
k ) dV
(cid:3)
Q
1
Q
0
Q(V + dk) dV
(2.91)
(2.86)
(2.87)
(2.88)
(2.89)
(2.90)

2.2 Dither
