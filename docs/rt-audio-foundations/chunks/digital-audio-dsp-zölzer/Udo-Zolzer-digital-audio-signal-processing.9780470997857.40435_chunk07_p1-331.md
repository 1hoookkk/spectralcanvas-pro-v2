# Digital Audio DSP (Zölzer) — Chunk 7/30

> Source: `Udo-Zolzer-digital-audio-signal-processing.9780470997857.40435.pdf` · Pages ~1-331 · Extractor: pdfminer.six
> Target ≈ 2800 words, Overlap = 200 words · Actual = 3257

> Note: DSP textbook content for SpectralCanvasPro RT-audio knowledge base

---
The integrals in (2.91) are independent of dk. Moreover,
value of the quantization error
39
(cid:11)
k P (dk) = 1. With the mean
and the quadratic mean error
it is possible to rewrite (2.91) as
e = 1
Q
(cid:3)
Q
0
Q(V ) dV
e2 = 1
Q
(cid:3)
Q
0
Q2(V ) dV ,
d 2
tot
= e2 + 2de + d 2.
With σ 2
E
= e2 − e2 and σ 2
D
= d 2 − d
2
, (2.94) can be written as
d 2
tot
= σ 2
E
+ (d + e)2 + σ 2
D.
(2.92)
(2.93)
(2.94)
(2.95)
E) and the dither addition (d, d 2, σ 2
Equations (2.94) and (2.95) describe the total noise power as a function of the quantization
(e, e2, σ 2
D). It can be seen that for zero-mean quantiza-
tion, the middle term in (2.95) results in d + e = 0. The acoustically perceptible part of the
e and σ 2
total error power is represented by σ 2
d .
2.2.2 Implementation
The random sequence d(n) is generated with the help of a random number generator
with uniform PDF. For generating a triangular PDF random sequence, two independent
uniform PDF random sequences d1(n) and d2(n) can be added. In order to generate a
triangular high-pass dither, the dither value d1(n) is added to −d1(n − 1). Thus, only one
random number generator is required. In conclusion, the following dither sequences can be
implemented:
dRECT(n) = d1(n),
dTRI(n) = d1(n) + d2(n),
dHP(n) = d1(n) − d1(n − 1).
(2.96)
(2.97)
(2.98)
The power density spectra of triangular PDF dither and triangular PDF HP dither are shown
in Fig. 2.21. Figure 2.22 shows histograms of a uniform PDF dither and a triangular PDF
high-pass dither together with their respective power density spectra. The amplitude range
of a uniform PDF dither lies between ±Q/2, whereas it lies between ±Q for triangular
PDF dither. The total noise power for triangular PDF dither is doubled.
2.2.3 Examples
The effect of the input amplitude of the quantizer is shown in Fig. 2.23 for a 16-bit
quantizer (Q = 2−15). A quantized sinusoidal signal with amplitude 2−15 (1-bit amplitude)
40
Quantization
Power Density Spectrum
HP
TRI
4
3.5
3
2.5
2
1.5
1
0.5
→
)
Ω
j
e
(
1
D
1
D
S
/
)
Ω
j
e
(
D
D
S
0
0
0.05
0.1
0.15
0.2
0.3
0.35
0.4
0.45
0.5
0.25
f/fS
→
Figure 2.21 Normalized power density spectrum for triangular PDF dither (TRI) with d1(n) + d2(n)
and triangular PDF high-pass dither (HP) with d1(n) − d1(n − 1).
and frequency f/fS = 64/1024 is shown in Fig. 2.23a,b for rounding and truncation.
Figure 2.23c,d shows their corresponding spectra. For truncation, Fig. 2.23c shows the
spectral line of the signal and the spectral distribution of the quantization error with the
harmonics of the input signal. For rounding (Fig. 2.23d with special signal frequency
f/fS = 64/1024), the quantization error is concentrated in uneven harmonics.
In the following, only the rounding operation is used. By adding a uniform PDF random
signal to the actual signal before quantization, the quantized signal shown in Fig. 2.24a
results. The corresponding power density spectrum is illustrated in Fig. 2.24c. In the time
domain, it is observed that the 1-bit amplitudes approach zero so that the regular pattern of
the quantized signal is affected. The resulting power density spectrum in Fig. 2.24c shows
that the harmonics do not occur anymore and the noise power is uniformly distributed over
the frequencies. For triangular PDF dither, the quantized signal is shown in Fig. 2.24b.
Owing to triangular PDF, amplitudes ±2Q occur besides the signal values ±Q and zero.
Figure 2.24d shows the increase of the total noise power.
In order to illustrate the noise modulation for uniform PDF dither, the amplitude of
the input is reduced to A = 2−18 and the frequency is chosen as f/fS = 14/1024. This
means that input amplitude to the quantizer is 0.25 bit. For a quantizer without additive
dither, the quantized output signal is zero. For RECT dither, the quantized signal is shown
in Fig. 2.25a. The input signal with amplitude 0.25Q is also shown. The power density
spectrum of the quantized signal is shown in Fig. 2.25c. The spectral line of the signal and
the uniform distribution of the quantization error can be seen. But in the time domain,
a correlation between positive and negative amplitudes of the input and the quantized
positive and negative values of the output can be observed. In hearing tests this noise
2.2 Dither
41
a) Histogram RECT
b) Histogram HP
50
45
40
35
30
25
20
15
10
5
50
45
40
35
30
25
20
15
10
5
0
-2
-1.5
-1
-0.5
0
x/Q
c) PDS
0.5
1
1.5
2
0
-2
-1.5
-1
-0.5
0.5
1
1.5
2
0
x/Q
d) PDS
→
B
d
n
i
)
Ω
j
e
(
D
D
S
0
-10
-20
-30
-40
-50
-60
-70
-80
-90
-100
0 0.05 0.1 0.15 0.2 0.25 0.3 0.35 0.4 0.45 0.5
f/fS
→
→
B
d
n
i
)
Ω
j
e
(
D
D
S
0
-10
-20
-30
-40
-50
-60
-70
-80
-90
-100
0 0.05 0.1 0.15 0.2 0.25 0.3 0.35 0.4 0.45 0.5
f/fS
→
Figure 2.22 (a,b) Histogram and (c,d) power density spectrum of uniform PDF dither (RECT) with
d1(n) and triangular PDF high-pass dither (HP) with d1(n) − d1(n − 1).
modulation occurs if the amplitude of the input is decreased continuously and falls below
the amplitude of the quantization step. This process occurs for all fade-out processes that
occur in speech and music signals. For positive low-amplitude signals, two output states,
zero and Q, occur, and for negative low-amplitude signals, the output states zero and −Q,
occur. This is observed as a disturbing rattle which is overlapped to the actual signal. If the
input level is further reduced the quantized output approaches zero.
In order to reduce this noise modulation at low levels, a triangular PDF dither is
used. Figure 2.25b shows the quantized signal and Fig. 2.25d shows the power density
spectrum. It can be observed that the quantized signal has an irregular pattern. Hence a
direct association of positive half-waves with the positive output values as well as vice
versa is not possible. The power density spectrum shows that spectral line of the signal
along with an increase in noise power owing to triangular PDF dither. In acoustic hearing
tests, the use of triangular PDF dither results in a constant noise ﬂoor even if the input level
is reduced to zero.
Quantization
b) Rounding
42
→
)
n
(
Q
x
x10-5
6
4
2
0
-2
-4
a) Truncation
x10-5
6
4
2
0
-2
-4
→
)
n
(
Q
x
-6
0
10
20
30
40
60
70
80
90 100
50
n →
-6
0
10
20
30
40
60
70
80
90 100
50
n →
c) f/fs 64/1024
d) f/fs 64/1024
→
B
d
n
i
)
Ω
j
e
(
X
X
S
-80
-90
-100
-110
-120
-130
-140
0 0.05 0.1 0.15 0.2 0.25 0.3 0.35 0.4 0.45 0.5
f/fS
→
→
B
d
n
i
)
Ω
j
e
(
X
X
S
-80
-90
-100
-110
-120
-130
-140
0 0.05 0.1 0.15 0.2 0.25 0.3 0.35 0.4 0.45 0.5
f/fS
→
Figure 2.23 One-bit amplitude – quantizer with truncation (a,c) and rounding (b,d).
2.3 Spectrum Shaping of Quantization – Noise Shaping
Using the linear model of a quantizer in Fig. 2.26 and the relations
e(n) = y(n) − x(n),
y(n) = [x(n)]Q
= x(n) + e(n),
(2.99)
(2.100)
(2.101)
the quantization error e(n) may be isolated and fed back through a transfer function H (z)
as shown in Fig. 2.27. This leads to the spectral shaping of the quantization error as given
by
y(n) = [x(n) − e(n) ∗ h(n)]Q
= x(n) + e(n) − e(n) ∗ h(n),
e1(n) = y(n) − x(n)
= e(n) ∗ (δ(n) − h(n)),
(2.102)
(2.103)
(2.104)
(2.105)
2.3 Spectrum Shaping of Quantization – Noise Shaping
43
a) RECT Dither
x10-5
6
4
2
0
-2
-4
→
)
n
(
Q
x
b) TRI Dither
x10-5
6
4
2
0
-2
-4
→
)
n
(
Q
x
-6
0
10
20
30
40
60
70
80
90 100
50
n →
-6
0
10
20
30
40
60
70
80
90 100
50
n →
c) f/fs 64/1024
d) f/fs 64/1024
→
B
d
n
i
)
Ω
j
e
(
X
X
S
-80
-90
-100
-110
-120
-130
-140
0 0.05 0.1 0.15 0.2 0.25 0.3 0.35 0.4 0.45 0.5
f/fS
→
→
B
d
n
i
)
Ω
j
e
(
X
X
S
-80
-90
-100
-110
-120
-130
-140
0 0.05 0.1 0.15 0.2 0.25 0.3 0.35 0.4 0.45 0.5
f/fS
→
Figure 2.24 One-bit amplitude – rounding with RECT dither (a,c) and TRI dither (b,d).
and the corresponding Z-transforms
Y (z) = X(z) + E(z)(1 − H (z))
E1(z) = E(z)(1 − H (z)).
(2.106)
(2.107)
A simple spectrum shaping of the quantization error e(n) is achieved by feeding back with
H (z) = z−1 as shown in Fig. 2.28, and leads to
and the Z-transforms
y(n) = [x(n) − e(n − 1)]Q
= x(n) − e(n − 1) + e(n),
e1(n) = y(n) − x(n)
= e(n) − e(n − 1),
Y (z) = X(z) + E(z)(1 − z
E1(z) = E(z)(1 − z
−1).
−1),
(2.108)
(2.109)
(2.110)
(2.111)
(2.112)
(2.113)
44
→
)
n
(
Q
x
x10-5
6
4
2
0
-2
-4
-6
0
a) RECT Dither
20
40
60
80 100 120 140 160 180 200
n →
c) f/fs 14/1024
→
)
n
(
Q
x
x10-5
6
4
2
0
-2
-4
-6
0
Quantization
b) TRI Dither
20
40
60
80 100 120 140 160 180 200
n →
d) f/fs 14/1024
→
B
d
n
i
)
Ω
j
e
(
X
X
S
-80
-90
-100
-110
-120
-130
-140
0 0.05 0.1 0.15 0.2 0.25 0.3 0.35 0.4 0.45 0.5
f/fS
→
→
B
d
n
i
)
Ω
j
e
(
X
X
S
-80
-90
-100
-110
-120
-130
-140
0 0.05 0.1 0.15 0.2 0.25 0.3 0.35 0.4 0.45 0.5
f/fS
→
Figure 2.25 0.25-bit amplitude – rounding with RECT dither (a,c) and TRI dither (b,d).
’7(cid:10):
)
"7(cid:10):
(cid:2)7(cid:10):
Figure 2.26 Linear model of quantizer.
Equation (2.113) shows a high-pass weighting of the original error signal e(n). By choosing
H (z) = z−1(−2 + z−1), second-order high-pass weighting given by
E2(z) = E(z)(1 − 2z
−1 + z
−2)
(2.114)
can be achieved. The power density spectrum of the error signal for the two cases is given by
SE1E1 (ej (cid:4)) = |1 − e
SE2E2 (ej (cid:4)) = |1 − 2 e
−j (cid:4)|2SEE(ej (cid:4)),
−j (cid:4) + e
−j 2(cid:4)|2SEE(ej (cid:4)).
(2.115)
(2.116)
2.3 Spectrum Shaping of Quantization – Noise Shaping
45
Figure 2.29 shows the weighting of power density spectrum by this noise shaping
technique.
’7(cid:10):
)
"7(cid:10):
(cid:2)7(cid:10):
=7 :
Figure 2.27 Spectrum shaping of quantization error.
’7(cid:10):
)
"7(cid:10):
(cid:2)7(cid:10):
  (cid:5)(cid:22)
Figure 2.28 High-pass spectrum shaping of quantization error.
Power Density Spectrum
→
)
Ω
j
e
(
E
E
S
/
)
Ω
j
e
(
2
,
1
E
2
,
1
E
S
16
14
12
10
8
6
4
2
.
0
. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
0
0.05
0.1
0.15
0.2
0.3
0.35
0.4
0.45
0.5
0.25
f/fS
→
Figure 2.29 Spectrum shaping (SEE(ej (cid:4)) · · ·, SE1E1 (ej (cid:4)) —, SE2E2 (ej (cid:4)) - - - ).
By adding a dither signal d(n) (see Fig. 2.30), the output and the error are given by
y(n) = [x(n) + d(n) − e(n − 1)]Q
= x(n) + d(n) − e(n − 1) + e(n)
(2.117)
(2.118)
46
and
e1(n) = y(n) − x(n)
= d(n) + e(n) − e(n − 1).
For the Z-transforms we write
Y (z) = X(z) + E(z)(1 − z
E1(z) = E(z)(1 − z
−1) + D(z).
−1) + D(z),
Quantization
(2.119)
(2.120)
(2.121)
(2.122)
The modiﬁed error signal e1(n) consists of the dither and the high-pass shaped quantization
error.
-7(cid:10):
’7(cid:10):
)
"7(cid:10):
(cid:2)7(cid:10):
  (cid:5)(cid:22)
Figure 2.30 Dither and spectrum shaping.
By moving the addition (Fig. 2.31) of the dither directly before the quantizer, a high-
pass spectrum shaping is obtained for both the error signal and the dither. Here the
following relationships hold:
y(n) = [x(n) + d(n) − e0(n − 1)]Q
= x(n) + d(n) − e0(n − 1) + e(n),
e0(n) = y(n) − (x(n) − e0(n − 1))
= d(n) + e(n),
y(n) = x(n) + d(n) − d(n − 1) + e(n) − e(n − 1),
e1(n) = d(n) − d(n − 1) + e(n) − e(n − 1),
with the Z-transforms given by
Y (z) = X(z) + E(z)(1 − z
E1(z) = E(z)(1 − z
−1) + D(z)(1 − z
−1).
−1) + D(z)(1 − z
−1),
(2.123)
(2.124)
(2.125)
(2.126)
(2.127)
(2.128)
(2.129)
(2.130)
Apart from the discussed feedback structures which are easy to implement on a digital
signal processor and which lead to high-pass noise shaping, psychoacoustic-based noise
shaping methods have been proposed in the literature [Ger89, Wan92, Hel07]. These
methods use special approximations of the hearing threshold (threshold in quiet, absolute
threshold) for the feedback structure 1 − H (z). Figure 2.32a shows several hearing
threshold models as a function of frequency [ISO389, Ter79, Wan92]. It can be seen
2.4 Number Representation
47
that the sensitivity of human hearing is high for frequencies between 2 and 6 kHz and
sharply decreases for high and low frequencies. Figure 2.32b also shows the inverse ISO
389-7 threshold curve which represents an approximation of the ﬁltering operation in our
perception. The feedback ﬁlter of the noise shaper should affect the quantization error
with the inverse ISO 389 weighting curve. Hence, the noise power in the frequency range
with high sensitivity should be reduced and shifted toward lower and higher frequencies.
Figure 2.33a shows the unweighted power density spectra of the quantization error for three
special ﬁlters H (z) [Wan92, Hel07]. Figure 2.33b depicts the same three power density
spectra, weighted by the inverse ISO 389 threshold of Fig. 2.32b. These weighted power
density spectra show that the perceived noise power is reduced by all three noise shapers
versus the frequency axis. Figure 2.34 shows a sinusoid with amplitude Q = 2−15, which
is quantized to w = 16 bits with psychoacoustic noise shaping. The quantized signal xQ(n)
consists of different amplitudes reﬂecting the low-level signal. The power density spectrum
of the quantized signal reﬂects the psychoacoustic weighting of the noise shaper with a
ﬁxed ﬁlter. A time-variant psychoacoustic noise shaping is described in [DeK03, Hel07],
where the instantaneous masking threshold is used for adaptation of a time-variant ﬁlter.
-7(cid:10):
’7(cid:10):
)
"7(cid:10):
(cid:2)(cid:14)(cid:14)7(cid:10):
(cid:11)
  (cid:5)(cid:22)
Figure 2.31 Modiﬁed dither and spectrum shaping.
2.4 Number Representation
The different applications in digital signal processing and transmission of audio signals
leads to the question of the type of number representation for digital audio signals. In
this section, basic properties of ﬁxed-point and ﬂoating-point number representation in the
context of digital audio signal processing are presented.
2.4.1 Fixed-point Number Representation
In general, an arbitrary real number x can be approximated by a ﬁnite summation
xQ =
w−1(cid:6)
i=0
bi2i,
(2.131)
where the possible values for bi are 0 and 1.
The ﬁxed-point number representation with a ﬁnite number w of binary places leads to
four different interpretations of the number range (see Table 2.1 and Fig. 2.35).
48
Quantization
a) Hearing Thresholds in Quiet
ISO 389(cid:358)7, diffuse field
Terhardt
inverse F(cid:358)weighting
2
10
3
10
f / Hz (cid:111)
4
10
b) Inverse ISO 389(cid:358)7 Threshold
(cid:111)
B
d
/
l
e
v
e
L
e
r
u
s
s
e
r
P
d
n
u
o
S
(cid:111)
B
d
/
|
)
f
(
H
|
80
60
40
20
0
(cid:358)20
10
0
(cid:358)10
(cid:358)20
(cid:358)30
(cid:358)40
0
2
4
6
8
f / kHz (cid:111)
10
12
14
16
Figure 2.32 (a) Hearing thresholds in quiet. (b) Inverse ISO 389-7 threshold curve.
Table 2.1 Bit location and range of values.
Type
Bit location
Range of values
Signed 2’s c.
Unsigned 2’s c.
Signed int.
Unsigned int.
(cid:11)
w−1
i=1 b−i 2
−i
w
i=1 b−i 2
xQ = −b0 +
(cid:11)
xQ =
xQ = −bw−12w−1 +
w−1
i=0 bi 2i
xQ =
(cid:11)
(cid:11)
−i
−(w−1)
−1 ≤ xQ ≤ 1 − 2
−w
0 ≤ xQ ≤ 1 − 2
w−2
i=0 bi 2i −2w−1 ≤ xQ ≤ 2w−1 − 1
0 ≤ xQ ≤ 2w − 1
The signed fractional representation (2’s complement) is the usual format for digital
audio signals and for algorithms in ﬁxed-point arithmetic. For address and modulo
operation, the unsigned integer is used. Owing to ﬁnite word-length w, overﬂow occurs
as shown in Fig. 2.36. These curves have to be taken into consideration while carrying out
operations, especially additions in 2’s complement arithmetic.
Quantization is carried out with techniques as shown in Table 2.2 for rounding and
truncation. The quantization step size is characterized by Q = 2−(w−1) and the symbol (cid:6)x(cid:7)
denotes the biggest integer smaller than or equal to x. Figure 2.37 shows the rounding and

2.4 Number Representation
