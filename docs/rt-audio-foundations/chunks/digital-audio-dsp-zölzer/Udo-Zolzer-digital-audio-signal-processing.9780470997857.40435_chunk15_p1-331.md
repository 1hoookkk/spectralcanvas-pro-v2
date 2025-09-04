# Digital Audio DSP (Zölzer) — Chunk 15/30

> Source: `Udo-Zolzer-digital-audio-signal-processing.9780470997857.40435.pdf` · Pages ~1-331 · Extractor: pdfminer.six
> Target ≈ 2800 words, Overlap = 200 words · Actual = 2310

> Note: DSP textbook content for SpectralCanvasPro RT-audio knowledge base

---
143
z-plane
→
}
z
{
m
I
1
0.8
0.6
0.4
0.2
0
-0.2
-0.4
-0.6
-0.8
-1
-1
.
.
.
.
.
.
.
.
.
.
.
.
.
.
.
.
.
.
.
.
.
.
.
.
.
.
.
.
.
.
.
.
.
.
.
.
.
.
.
.
.
.
.
.
.
.
.
.
.
.
.
.
.
.
.
.
.
.
.
.
.
.
.
.
.
.
.
.
.
.
.
.
.
.
.
.
.
.
.
.
.
.
.
.
.
.
.
.
.
.
.
.
.
.
.
.
.
.
.
.
.
.
.
.
.
.
.
.
.
.
.
.
.
.
.
.
.
.
.
.
.
.
.
.
.
.
..............
................
............
..................
..........
....................
........
......................
.......................
......
.........................
...
..........................
............................
.
.............................
..............................
.
.
...............................
.
................................
.
.................................
.
.
.
..................................
.
...................................
.
.
...................................
.
.
....................................
.
.
....................................
.
....................................
.
.
.....................................
.
.....................................
.
.
.....................................
.
.....................................
....................................
.
.
....................................
.
....................................
...................................
..................................
.................................
................................
...............................
.............................
............................
..........................
........................
...................................................................
...................................................................
........................
............................
..........................
...............................
....................................
.............................
................................
.................................
...................................
..................................
....................................
.....................................
.....................................
....................................
.....................................
.....................................
....................................
....................................
....................................
...................................
...................................
..................................
.................................
................................
...............................
.
..............................
.
.............................
.
............................
.
..........................
.........................
.
.
.......................
......................
.
.
.
....................
.
..................
.
.
.
................
.
..............
.
.
.
............
.
.
..........
.
........
.
......
.
.
.
...
.
.
.
.
.
.
.
.
.
.
.
.
.
.
.
.
.
.
.
.
.
.
.
.
.
.
.
.
.
.
.
.
.
.
.
.
.
.
.
.
.
.
.
.
.
.
.
.
.
.
.
.
.
.
.
.
.
.
.
.
.
.
.
.
.
.
.
.
.
.
.
.
.
.
.
.
.
.
.
.
.
.
.
.
.
.
.
.
.
.
.
.
.
.
.
.
.
.
.
.
.
.
.
.
.
.
.
.
.
.
.
.
.
.
-0.5
0
Re{z} →
0.5
.
1
Figure 5.34 Zölzer – pole distribution (6-bit quantization).
’7(cid:10):
  (cid:5)(cid:22)
"7(cid:10):
(cid:22)
(cid:22)
(cid:22)
  (cid:5)(cid:22)
(cid:23)
Figure 5.35 Zölzer – block diagram of recursive part.
Analytical Comparison of Noise Behavior of Different Filter Structures
In this section, recursive ﬁlter structures are analyzed in terms of their noise behavior in
ﬁxed-point arithmetic [Zöl89, Zöl90, Zöl94]. The block diagrams provide the basis for an
analytical calculation of noise power owing to the quantization of state variables. First of
all, the general case is considered in which quantization is performed after multiplication.
For this purpose, the transfer function Gi(z) of every multiplier output to the output of the
ﬁlter structure is determined.
For this error analysis it is assumed that the signal within the ﬁlter structure covers the
whole dynamic range so that the quantization error ei (n) is not correlated with the signal.
Consecutive quantization error samples are not correlated with each other so that a uniform
power density spectrum results [Sri77]. It can also be assumed that different quantization
errors ei(n) are uncorrelated within the ﬁlter structure. Owing to the uniform distribution
of the quantization error, the variance can be given by
σ 2
E
= Q2
12
.
(5.99)

144
Equalizers
The quantization error is added at every point of quantization and is ﬁltered by the cor-
responding transfer function G(z) to the output of the ﬁlter. The variance of the output
quantization noise (due to the noise source e(n)) is given by
#
σ 2
ye
= σ 2
E
1
2πj
G(z)G(z
−1)z
−1 dz.
z=ej(cid:4)
(5.100)
Exact solutions for the ring integral (5.100) can be found in [Jur64] for transfer functions
up to the fourth order. With the L2 norm of a periodic function
(cid:5) 1
2
(cid:4)
(cid:3)
π
(cid:11)G(cid:11)2 =
1
2π
−π
|G(ej (cid:4))|2 d(cid:4)
,
the superposition of the noise variances leads to the total output noise variance
(cid:6)
σ 2
ye
= σ 2
E
(cid:11)Gi (cid:11)2
2.
The signal-to-noise ratio for a full-range sinusoid can be written as
i
SNR = 10 log10
0.5
σ 2
ye
dB.
The ring integral
#
In = 1
2πj
A(z)A(z−1)
B(z)B(z−1)
−1 dz
z
z=ej(cid:4)
is given in [Jur64] for ﬁrst-order systems by
G(z) = a0z + a1
b0z + b1
+ a2
(a2
0
,
I1 =
1)b0 − 2a0a1b1
− b2
1)
b0(b2
0
,
and for second-order systems by
,
I2 =
G(z) = a0z2 + a1z + a2
b0z2 + b1z + b2
A0b0c1 − A1b0b1 + A2(b2
1
b0[(b2
0
+ a2
A0 = a2
1
0
A1 = 2(a0a1 + a1a2),
A2 = 2a0a2,
c1 = b0 + b2.
− b2c1)
2)c1 − (b0b1 − b1b2)b1]
− b2
+ a2
2,
,
(5.101)
(5.102)
(5.103)
(5.104)
(5.105)
(5.106)
(5.107)
(5.108)
(5.109)
(5.110)
(5.111)
(5.112)
In the following, an analysis of the noise behavior for different recursive ﬁlter structures
is presented. The noise transfer functions of individual recursive parts are responsible for
noise shaping.
5.2 Recursive Audio Filters
145
(cid:2)(cid:14)(cid:14)7(cid:10):(cid:14)9(cid:14)(cid:2)(cid:14)(cid:14)7(cid:10):
(cid:23)
(cid:22)
’7(cid:10):
"7(cid:10):
  (cid:5)(cid:22)
  (cid:5)(cid:22)
(cid:5).
(cid:22)
(cid:5).
(cid:23)
Figure 5.36 Direct form with additive error signal.
Table 5.6 Direct form – (a) noise transfer function, (b) quadratic L2 norm and (c) output noise
variance in the case of quantization after every multiplication.
(a) G1(z) = G2(z) =
(b)
(c)
(cid:11)G1(cid:11)2
2
= (cid:11)G2(cid:11)2
2
σ 2
ye
= σ 2
E 2
1 + b2
1 − b2
z2
z2 + b1z + b2
= 1 + b2
1 − b2
1
(1 + b2)2 − b2
1
1
(1 + b2)2 − b2
1
The error transfer function of a second-order direct-form structure (see Fig. 5.36) has
only complex poles (see Table 5.6).
The implementation of poles near the unit circle leads to high ampliﬁcation of the
quantization error. The effect of the pole radius on the noise variance can be observed in
the equation for output noise variance. The coefﬁcient b2 = r 2 approaches 1, which leads
to a huge increase in the output noise variance.
The Gold and Rader ﬁlter structure (Fig. 5.37) has an output noise variance that depends
on the pole radius (see Table 5.7) and is independent of the pole phase. The latter fact is
because of the uniform grid of the pole distribution. An additional zero on the real axis
(z = r cos ϕ) directly beneath the poles reduces the effect of the complex poles.
The Kingsbury ﬁlter (Fig. 5.38 and Table 5.8) and the Zölzer ﬁlter (Fig. 5.39 and
Table 5.9), which is derived from it, show that the noise variance depends on the pole
radius. The noise transfer functions have a zero at z = 1 in addition to the complex poles.
This zero reduces the amplifying effect of the pole near the unit circle at z = 1.
Figure 5.40 shows the signal-to-noise ratio versus the cutoff frequency for the four ﬁlter
structures presented above. The signals are quantized to 16 bits. Here, the poles move with
increasing cutoff frequency on the curve characterized by the Q-factor Q∞ = 0.7071 in the
z-plane. For very small cutoff frequencies, the Zölzer ﬁlter shows an improvement of 3 dB
in terms of signal-to-noise ratio compared with the Kingsbury ﬁlter and an improvement of
146
Equalizers
(cid:2)(cid:14)(cid:14)7(cid:10):(cid:14)9(cid:14)(cid:2)(cid:14)(cid:14)7(cid:10):
(cid:23)
(cid:22)
(cid:2)(cid:14)(cid:14)7(cid:10):(cid:14)9(cid:14)(cid:2)(cid:14)(cid:14)7(cid:10):
<
2
’7(cid:10):
  (cid:5)(cid:22)
(cid:8)(cid:14)(cid:20)(cid:11)(cid:18) (cid:15)
  (cid:5)(cid:22)
(cid:8)(cid:14)(cid:20)(cid:11)(cid:18) (cid:15)
"7(cid:10):
(cid:8)(cid:14)(cid:18)(cid:16)(cid:10) (cid:15)
(cid:5)(cid:8)(cid:14)(cid:18)(cid:16)(cid:10)
(cid:15)
Figure 5.37 Gold and Rader structure with additive error signals.
Table 5.7 Gold and Rader – (a) noise transfer function, (b) quadratic L2 norm and (c) output noise
variance in the case of quantization after every multiplication.
(a)
(b)
G1(z) = G2(z) =
G3(z) = G4(z) =
r sin ϕ
z2 − 2r cos ϕz + r2
z − r cos ϕ
z2 − 2r cos ϕz + r2
(cid:11)G1(cid:11)2
2
= (cid:11)G2(cid:11)2
2
(cid:11)G3(cid:11)2
2
= (cid:11)G4(cid:11)2
2
= 1 + b2
1 − b2
= 1
1 − b2
(r sin ϕ)2
(1 + b2)2 − b2
1
[1 + (r sin ϕ)2](1 + b2)2 − b2
1
(1 + b2)2 − b2
1
(c)
σ 2
ye
= σ 2
E 2
1
1 − b2
(cid:2)(cid:14)(cid:14)7(cid:10):
(cid:22)
(cid:2)(cid:14)(cid:14)7(cid:10):
(cid:23)
(cid:2)(cid:14)(cid:14)7(cid:10):
2
’7(cid:10):
  (cid:5)(cid:22)
"7(cid:10):
(cid:17)
(cid:22)
  (cid:5)(cid:22)
(cid:17)
(cid:22)
(cid:17)
(cid:23)
Figure 5.38 Kingsbury structure with additive error signals.
6 dB compared with the Gold and Rader ﬁlter. Up to 5 kHz, the Zölzer ﬁlter yields better
results (see Fig. 5.41). From 6 kHz onwards, the reduction of pole density in this ﬁlter leads
to a decrease in the signal-to-noise ratio (see Fig. 5.41).
5.2 Recursive Audio Filters
147
Table 5.8 Kingsbury – (a) noise transfer function, (b) quadratic L2 norm and (c) output noise variance
in the case of quantization after every multiplication.
G1(z) =
(a)
G2(z) =
G3(z) =
−k1z
z2 − (2 − k1k2 − k2
1)z + (1 − k1k2)
−k1(z − 1)
z2 − (2 − k1k2 − k2
z − 1
z2 − (2 − k1k2 − k2
1)z + (1 − k1k2)
1)z + (1 − k1k2)
(cid:11)G1(cid:11)2
2
= 1
k1k2
2 − k1k2
2(2 − k1k2) − k2
1
(b)
(cid:11)G2(cid:11)2
2
(cid:11)G3(cid:11)2
2
= k1
k2
= 1
k1k2
2
2(2 − k1k2) − k2
1
2
2(2 − k1k2) − k2
1
(c)
σ 2
ye
= σ 2
E2
5 + 2b1 + 3b2
(1 − b2)(1 + b2 − b1)
Table 5.9 Zölzer – (a) noise transfer function, (b) quadratic L2 norm and (c) output noise variance in
the case of quantization after every multiplication.
G1(z)
=
(a)
G2(z) = G3(z) =
G4(z)
(cid:11)G1(cid:11)2
2
(b)
(cid:11)G2(cid:11)2
2
= (cid:11)G3(cid:11)2
2
(cid:11)G4(cid:11)2
2
−z2
1z
z2 − (2 − z1z2 − z3
1)z + (1 − z1z2)
−z1(z − 1)
z2 − (2 − z1z2 − z3
z − 1
z2 − (2 − z1z2 − z3
=
1)z + (1 − z1z2)
1)z + (1 − z1z2)
=
=
=
z4
1
z1z2
z6
1
z1z2
z3
1
z1z2
2z3
2z3
2 − z1z2
1(2 − z1z2) − z6
1
2
1(2 − z1z2) − z6
1
2
1(2 − z1z2) − z6
1
2z3
(c)
σ 2
ye
= σ 2
E 2
6 + 4(b1 + b2) + (1 + b2)(1 + b1 + b2)1/3
(1 − b2)(1 + b2 − b1)
148
Equalizers
(cid:2)(cid:14)(cid:14)7(cid:10):
(cid:22)
’7(cid:10):
(cid:2)(cid:14)(cid:14)7(cid:10):(cid:14)9(cid:14)(cid:2)(cid:14)(cid:14)7(cid:10):
(cid:23)
2
(cid:2)(cid:14)(cid:14)7(cid:10):
<
  (cid:5)(cid:22)
"7(cid:10):
(cid:22)
(cid:22)
(cid:22)
  (cid:5)(cid:22)
(cid:23)
Figure 5.39 Zölzer structure with additive error signals.
Zölzer
Kingsbury
Gold and Rader
Direct-form
20
40
60
80
100
120
140
160
180
200
f/Hz
SNR/dB
100
80
60
40
20
0
Figure 5.40 SNR vs. cutoff frequency – quantization of products (fc < 200 Hz).
SNR/dB
100
Zölzer
Kingsbury
Gold and Rader
90
80
70
60
50
Direct-form
2k
4k
6k
8k
10k
12k
f/Hz
Figure 5.41 SNR vs. cutoff frequency – quantization of products (fc > 2 kHz).

5.2 Recursive Audio Filters

149
With regard to the implementation of the these ﬁlters with digital signal processors, a
quantization after every multiplication is not necessary. Quantization takes place when the
accumulator has to be stored in memory. This can be seen in Figs 5.42–5.45 by introducing
quantizers where they really occur. The resulting output noise variances are also shown.
The signal-to-noise ratio is plotted versus the cutoff frequency in Figs 5.46 and 5.47. In
the case of direct-form and Gold and Rader ﬁlters, the signal-to-noise ratio increases by
3 dB whereas the output noise variance for the Kingsbury ﬁlter remains unchanged. The
Kingsbury ﬁlter and the Gold and Rader ﬁlters exhibit similar results up to a frequency of
200 kHz (see Fig. 5.46). The Zölzer ﬁlter demonstrates an improvement of 3 dB compared
with these structures. For frequencies of up to 2 kHz (see Fig. 5.47) it is seen that the
increased pole density leads to an improvement of the signal-to-noise ratio as well as a
reduced effect due to coefﬁcient quantization.
’7(cid:10):
"7(cid:10):
)
(cid:5).
(cid:22)
(cid:5).
(cid:23)
  (cid:5)(cid:22)
  (cid:5)(cid:22)
σ 2
ye
= σ 2
E
1 + b2
1 − b2
1
(1 + b2)2 − b2
1
Figure 5.42 Direct-form ﬁlter – quantization after accumulator.
’7(cid:10):
)
  (cid:5)(cid:22)
)
  (cid:5)(cid:22)
"7(cid:10):
(cid:8)(cid:14)(cid:18)(cid:16)(cid:10) (cid:15)
(cid:8)(cid:14)(cid:20)(cid:11)(cid:18) (cid:15)
(cid:8)(cid:14)(cid:20)(cid:11)(cid:18) (cid:15)
(cid:5)(cid:8)(cid:14)(cid:18)(cid:16)(cid:10)
(cid:15)
σ 2
ye
= σ 2
E
1
1 − b2
Figure 5.43 Gold and Rader ﬁlter – quantization after accumulator.
150
’7(cid:10):
)
  (cid:5)(cid:22)
(cid:17)
(cid:22)
)
(cid:17)
(cid:22)
(cid:17)
(cid:23)
Equalizers
)
  (cid:5)(cid:22)
"7(cid:10):
σ 2
ye
= σ 2
E
5 + 2b1 + 3b2
(1 − b2)(1 + b2 − b1)
Figure 5.44 Kingsbury ﬁlter – quantization after accumulator.
’7(cid:10):
)
  (cid:5)(cid:22)
(cid:22)
(cid:22)
(cid:22)
)
(cid:23)
)
  (cid:5)(cid:22)
"7(cid:10):
σ 2
ye
= σ 2
E
2(2 + b1 + b2) + (1 + b2)(1 + b1 + b2)1/3
(1 − b2)(1 + b2 − b1)
Figure 5.45 Zölzer ﬁlter – quantization after accumulator.
Zölzer
Gold and Rader
Kingsbury
Direct-form
20
40
60
80
100
120
140
160
180
200
f/Hz
SNR/dB
100
80
60
40
20
0
Figure 5.46 SNR vs. cutoff frequency – quantization after accumulator (fc < 200 Hz).
Noise Shaping in Recursive Filters
The analysis of the noise transfer function of different structures shows that for three
structures with low roundoff noise a zero at z = 1 occurs in the transfer functions G(z)

5.2 Recursive Audio Filters
