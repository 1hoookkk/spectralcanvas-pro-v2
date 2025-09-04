# Digital Audio DSP (Zölzer) — Chunk 4/30

> Source: `Udo-Zolzer-digital-audio-signal-processing.9780470997857.40435.pdf` · Pages ~1-331 · Extractor: pdfminer.six
> Target ≈ 2800 words, Overlap = 200 words · Actual = 6051

> Note: DSP textbook content for SpectralCanvasPro RT-audio knowledge base

---
Chapter 2
Quantization
Basic operations for AD conversion of a continuous-time signal x(t) are the sampling
and quantization of x(n) yielding the quantized sequence xQ(n) (see Fig. 2.1). Before
discussing AD/DA conversion techniques and the choice of the sampling frequency fS =
1/TS in Chapter 3 we will introduce the quantization of the samples x(n) with ﬁnite
number of bits. The digitization of a sampled signal with continuous amplitude is called
quantization. The effects of quantization starting with the classical quantization model are
discussed in Section 2.1. In Section 2.2 dither techniques are presented which, for low-level
signals, linearize the process of quantization. In Section 2.3 spectral shaping of quantization
errors is described. Section 2.4 deals with number representation for digital audio signals
and their effects on algorithms.
Sampling
fS
x(t)
Analog
Low-pass
x(n)
x (n)Q
Quantizer
Figure 2.1 AD conversion and quantization.
2.1 Signal Quantization
2.1.1 Classical Quantization Model
Quantization is described by Widrow’s quantization theorem [Wid61]. This says that a
quantizer can be modeled (see Fig. 2.2) as the addition of a uniform distributed random
signal e(n) to the original signal x(n) (see Fig. 2.2, [Wid61]). This additive model,
xQ(n) = x(n) + e(n),
is based on the difference between quantized output and input according to the error signal
e(n) = xQ(n) − x(n).
(2.2)
(2.1)
Digital Audio Signal Processing Second Edition Udo Zölzer
© 2008 John Wiley & Sons, Ltd
22
Quantization
x(n)
Q
x (n)Q
x(n)
Q
x (n)Q
x(n)
x (n)=x(n)+e(n)
Q
e(n)
_
e(n)
Figure 2.2 Quantization.
This linear model of the output xQ(n) is only then valid when the input amplitude has a
wide dynamic range and the quantization error e(n) is not correlated with the signal x(n).
Owing to the statistical independence of consecutive quantization errors the autocorrelation
of the error signal is given by rEE(m) = σ 2
· δ(m), yielding a power density spectrum
E
SEE(ej (cid:4)) = σ 2
E.
The nonlinear process of quantization is described by a nonlinear characteristic curve as
shown in Fig. 2.3a, where Q denotes the quantization step. The difference between output
and input of the quantizer provides the quantization error e(n) = xQ(n) − x(n), which is
shown in Fig. 2.3b. The uniform probability density function (PDF) of the quantization
error is given (see Fig. 2.3b) by
pE(e) = 1
Q
(cid:1)
rect
(cid:2)
.
e
Q
(2.3)
’
)
(cid:23))
)
(cid:5))
(cid:5)(cid:23))
(cid:2)
’
’
(cid:9):
.:
)
(cid:23)
)
(cid:23)
(cid:28)(cid:14)(cid:14)(cid:14)7(cid:2):
(cid:30)
(cid:22)
)
(cid:2)
)
(cid:23)
)
(cid:23)
Figure 2.3 (a) Nonlinear characteristic curve of a quantizer. (b) Quantization error e and its
probability density function (PDF) pE(e).
The mth moment of a random variable E with a PDF pE(e) is deﬁned as the expected
value of Em:
E{Em} =
(cid:3) ∞
−∞
empE(e) de.
(2.4)
2.1 Signal Quantization
23
For a uniform distributed random process, as in (2.3), the ﬁrst two moments are given by
The signal-to-noise ratio
mE = E{E} = 0 mean value
σ 2
E
= E{E2} = Q2
12
variance.
SNR = 10 log10
(cid:2)
(cid:1)
σ 2
X
σ 2
E
dB
(2.5)
(2.6)
(2.7)
is deﬁned as the ratio of signal power σ 2
X to error power σ 2
E.
For a quantizer with input range ±xmax and word-length w, the quantization step size
can be expressed as
By deﬁning a peak factor,
Q = 2xmax/2w.
PF = xmax
σX
= 2w−1Q
σX
,
the variances of the input and the quantization error can be written as
σ 2
X
σ 2
E
,
= x2
max
P 2
F
= Q2
12
= 1
12
x2
max
22w
22 = 1
3
x2
max2
−2w.
The signal-to-noise ratio is then given by
(cid:1)
SNR = 10 log10
max/P 2
x2
F
max2−2w
1
3 x2
= 6.02w − 10 log10(P 2
F /3) dB.
(cid:2)
= 10 log10
(cid:2)
(cid:1)
22w 3
P 2
F
A sinusoidal signal (PDF as in Fig. 2.4) with PF =
√
2 gives
For a signal with uniform PDF (see Fig. 2.4) and PF =
3 we can write
SNR = 6.02w + 1.76 dB.
√
SNR = 6.02w dB,
and for a Gaussian distributed signal (probability of overload <10−5 leads to PF = 4.61,
see Fig. 2.5), it follows that
SNR = 6.02w − 8.5 dB.
It is obvious that the signal-to-noise ratio depends on the PDF of the input. For digital
audio signals that exhibit nearly Gaussian distribution, the maximum signal-to-noise ratio
for given word-length w is 8.5 dB lower than the rule-of-thumb formula (2.14) for the
signal-to-noise ratio.
(2.8)
(2.9)
(2.10)
(2.11)
(2.12)
(2.13)
(2.14)
(2.15)
24
(cid:111)
)
x
(
X
p
2.5
2
1.5
1
0.5
0
(cid:86)X=xmax/(cid:151)2
-1
-0.5
0
x/xmax (cid:111)
0.5
1
(cid:111)
)
x
(
X
p
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
(cid:86)X=xmax/(cid:151)3
-1
-0.5
0
x/xmax (cid:111)
0.5
1
Figure 2.4 Probability density function (sinusoidal signal and signal with uniform PDF).
(cid:111)
)
x
(
X
p
0.5
0.45
0.4
0.35
0.3
0.25
0.2
0.15
0.1
0.05
0
(cid:86)X=xmax/4.61
-1
-0.5
0
x/xmax (cid:111)
0.5
1
Figure 2.5 Probability density function (signal with Gaussian PDF).
2.1.2 Quantization Theorem
The statement of the quantization theorem for amplitude sampling (digitizing the ampli-
tude) of signals was given by Widrow [Wid61]. The analogy for digitizing the time axis is
the sampling theorem due to Shannon [Sha48]. Figure 2.6 shows the amplitude quantization
and the time quantization. First of all, the PDF of the output signal of a quantizer is
determined in terms of the PDF of the input signal. Both probability density functions
are shown in Fig. 2.7. The respective characteristic functions (Fourier transform of a PDF)
of the input and output signals form the basis for Widrow’s quantization theorem.
2.1 Signal Quantization
25
Discrete
amplitude
32767
32766
+1
x(t)
x(n)
n
o
i
t
a
z
i
t
n
a
u
q
e
d
u
t
i
l
p
m
A
1
0
-1
-32767
-32768
-1
x(8)
0
1
2
3
4
5
6
7
8
9 10 11
Time quantization
Figure 2.6 Amplitude and time quantization.
t
n
Discrete
time
x
x
+1
x(t)
x(n)
)
x
(
X
p
Q
x(8)
t
)
x
(
Q
X
p
-1
Figure 2.7 Probability density function of signal x(n) and quantized signal xQ(n).
First-order Statistics of the Quantizer Output
Quantization of a continuous-amplitude signal x with PDF pX(x) leads to a discrete-
amplitude signal y with PDF pY (y) (see Fig. 2.8). The continuous PDF of the input is
sampled by integrating over all quantization intervals (zone sampling). This leads to a
discrete PDF of the output.
In the quantization intervals, the discrete PDF of the output is determined by the
probability
W [kQ] = W
(cid:4)
− Q
2
+ kQ ≤ x <
(cid:5)
(cid:3)
+ kQ
=
Q
2
Q/2+kQ
−Q/2+kQ
pX(x) dx.
(2.16)
26
Quantization
’
0(cid:28)(cid:14)(cid:14)(cid:14)7’:
)
"
(cid:28)(cid:14)(cid:14)(cid:14)7":
*
0(cid:28)(cid:14)(cid:14)(cid:14)7’:
*(cid:28)(cid:14)(cid:14)(cid:14)7":
(cid:5)5)
(cid:5)<)
(cid:5)2)
(cid:5)(cid:23))
(cid:5))
3
)
(cid:23))
2)
<)
5)
’
"
3
(cid:22)
(cid:23)
Figure 2.8 Zone sampling of the PDF.
For the intervals k = 0, 1, 2, it follows that
(cid:3)
Q/2
pY (y) = δ(0)
−Q/2
(cid:3)
= δ(y − Q)
pX(x) dx,
Q/2+Q
−Q/2+Q
(cid:3)
Q/2+2Q
pX(x) dx,
= δ(y − 2Q)
pX(x) dx,
−Q/2+2Q
− Q
2
− Q
2
− Q
2
≤ y <
Q
2
,
+ Q ≤ y <
Q
2
+ Q,
+ 2Q ≤ y <
Q
2
+ 2Q.
The summation over all intervals gives the PDF of the output
pY (y) =
=
∞(cid:6)
k=−∞
∞(cid:6)
k=−∞
δ(y − kQ)W (kQ)
δ(y − kQ)W (y),
where
Using
(cid:3)
Q/2+kQ
−Q/2+kQ
(cid:1)
W (kQ) =
pX(x) dx,
W (y) =
(cid:3) ∞
−∞
(cid:1)
= rect
rect
(cid:2)
y
Q
(cid:2)
pX(x) dx
y − x
Q
∗ pX(y).
δQ(y) =
∞(cid:6)
k=−∞
δ(y − kQ),
the PDF of the output is given by
(cid:4)
(cid:1)
pY (y) = δQ(y)
rect
(cid:5)
(cid:5) pX(y)
.
(cid:2)
y
Q
(2.17)
(2.18)
(2.19)
(2.20)
(2.21)
(2.22)
(2.23)
2.1 Signal Quantization
27
The PDF of the output can hence be determined by convolution of a rect function [Lip92]
with the PDF of the input. This is followed by an amplitude sampling with resolution Q as
described in (2.23) (see Fig. 2.9).
(cid:22)
)
(cid:28)(cid:14)(cid:14)(cid:14)7":
0
)
(cid:23)
")
(cid:23)
*(cid:28)(cid:14)(cid:14)(cid:14)7":
Figure 2.9 Determining PDF of the output.
Using F T {f1(t) · f2(t)} = 1
2π F1(j ω) ∗ F2(j ω), the characteristic function (Fourier
transform of pY (y)) can be written, with uo = 2π/Q, as
PY (ju) = 1
2π
uo
∞(cid:6)
k=−∞
(cid:4)
sin
(cid:7)
u Q
2
u Q
2
(cid:8)
(cid:5)
· PX(ju)
(cid:5)
Q
δ(u − kuo) ∗
(cid:7)
u Q
2
u Q
2
sin
(cid:4)
(cid:8)
δ(u − kuo) ∗
· PX(ju)
∞(cid:6)
=
k=−∞
PY (ju) =
∞(cid:6)
k=−∞
PX(ju − jkuo)
(cid:9)
sin
(u − kuo) Q
2
(u − kuo) Q
2
(cid:10)
.
(2.24)
(2.25)
(2.26)
Equation (2.26) describes the sampling of the continuous PDF of the input. If the
quantization frequency uo = 2π/Q is twice the highest frequency of the characteristic
function PX(ju) then periodically recurring spectra do not overlap. Hence, a reconstruction
of the PDF of the input pX(x) from the quantized PDF of the output pY (y) is possible (see
Fig. 2.10). This is known as Widrow’s quantization theorem. Contrary to the ﬁrst sampling
theorem (Shannon’s sampling theorem, ideal amplitude sampling in the time domain)
F A(j ω) = 1
k=−∞ F (j ω − j kωo), it can be observed that there is an additional
T
(cid:11)∞
(cid:10)
multiplication of the periodically characteristic function with
If the base-band of the characteristic function (k = 0)
PY (ju) = PX(ju)
sin
(cid:8)
(cid:15)
(cid:7)
u Q
2
u Q
(cid:13)(cid:14)
2
PE (ju)
(cid:12)
(cid:9)
sin
(u−kuo) Q
2
(u−kuo) Q
2
(see (2.26)).
(2.27)
is considered, it is observed that it is a product of two characteristic functions. The
multiplication of characteristic functions leads to the convolution of PDFs from which
the addition of two statistically independent signals can be concluded. The characteristic
function of the quantization error is thus
PE(ju) =
(cid:8)
,
sin
(cid:7)
u Q
2
u Q
2
(2.28)
28
Quantization
0(cid:28)(cid:14)(cid:14)(cid:14)7’:
*(cid:28)(cid:14)(cid:14)(cid:14)7":
)
’
"
0(cid:1)(cid:14)(cid:14)(cid:14)7(cid:13):
(cid:1)(cid:14)(cid:14)(cid:14)7*
(cid:13)
:
(cid:13)
0(cid:1)(cid:14)(cid:14)(cid:14)7
:(cid:13)
(cid:18)(cid:16)(cid:10)7 )
(cid:23) :
(cid:13)
:
7
(cid:13)
)
(cid:23)
(cid:13)(cid:11)
(cid:13)(cid:11)
(cid:13)
and the PDF
(see Fig. 2.11).
(cid:28)(cid:14)(cid:14)(cid:14)7(cid:2):
(cid:30)
(cid:22)
)
)
(cid:23)
)
(cid:23)
(cid:2)
Figure 2.10 Spectral representation.
pE(e) = 1
Q
(cid:1)
rect
(cid:2)
e
Q
(2.29)
(cid:22)
(cid:1)(cid:14)(cid:14)(cid:14)7
(cid:30) (cid:13)
:
(cid:23)(cid:2)
)
(cid:23)(cid:2)
)
(cid:13)
Figure 2.11 PDF and characteristic function of quantization error.
The modeling of the quantization process as an addition of a statistically independent
noise signal to the input signal leads to a continuous PDF of the output (see Fig. 2.12,
convolution of PDFs and sampling in the interval Q gives the discrete PDF of the output).
The PDF of the discrete-valued output comprises Dirac pulses at distance Q with values
equal to the continuous PDF (see (2.23)). Only if the quantization theorem is valid can the
continuous PDF be reconstructed from the discrete PDF.
pX+E(y)= p
pX
E*
pY(y)
x(n)
Q
y(n)
Q
y
x(n)
x(n)+e(n)
e(n)
Figure 2.12 PDF of model.
In many cases, it is not necessary to reconstruct the PDF of the input. It is sufﬁcient to
calculate the moments of the input from the output. The mth moment can be expressed in
2.1 Signal Quantization
terms of the PDF or the characteristic function:
(cid:3) ∞
E{Y m} =
−∞
ympY (y) dy
(cid:16)
(cid:16)
= (−j )m d mPY (ju)
(cid:16)
(cid:16)
dum
u=0
29
(2.30)
(2.31)
.
If the quantization theorem is satisﬁed then the periodic terms in (2.26) do not overlap and
the mth derivative of PY (ju) is solely determined by the base-band1 so that, with (2.26),
E{Y m} = (−j )m d m
dum PX(ju)
sin
With (2.32), the ﬁrst two moments can be determined as
mY = E{Y } =E {X},
σ 2
Y
= E{Y 2} = E{X2}
(cid:12) (cid:13)(cid:14) (cid:15)
σ 2
X
.
+ Q2
12(cid:12)(cid:13)(cid:14)(cid:15)
σ 2
E
Second-order Statistics of Quantizer Output
(cid:8)
(cid:7)
u Q
2
u Q
2
(cid:16)
(cid:16)
(cid:16)
(cid:16)
.
u=0
(2.32)
(2.33)
(2.34)
In order to describe the properties of the output in the frequency domain, two output values
Y1 (at time n1) and Y2 (at time n2) are considered [Lip92]. For the joint density function,
(cid:4)
(cid:1)
(cid:2)
(cid:5)
pY1Y2 (y1, y2) = δQQ(y1, y2)
rect
(cid:5) pX1X2(y1, y2)
,
(2.35)
y1
Q
,
y2
Q
with
and
δQQ(y1, y2) = δQ(y1) · δQ(y2)
(cid:1)
(cid:1)
(cid:2)
(cid:2)
(cid:1)
rect
y1
Q
,
y2
Q
= rect
· rect
y1
Q
y2
Q
(cid:2)
.
(2.36)
(2.37)
For the two-dimensional Fourier transform, it follows that
PY1Y2(ju1, ju2) =
∞(cid:6)
∞(cid:6)
k=−∞
(cid:4)
sin
∗
l=−∞
(cid:7)
Q
u1
2
Q
2
∞(cid:6)
u1
∞(cid:6)
=
δ(u1 − kuo)δ(u2 − luo)
(cid:8)
(cid:8)
(cid:7)
(cid:5)
sin
·
Q
2
u2
Q
2
u2
· PX1X2(ju1, ju2)
(2.38)
k=−∞
(cid:9)
sin
·
l=−∞
(u1 − kuo) Q
2
(u1 − kuo) Q
2
PX1X2(ju1
(cid:10)
− j luo)
(cid:10)
− jkuo, ju2
(cid:9)
(u2 − luo) Q
2
(u2 − luo) Q
2
sin
·
.
(2.39)
1This is also valid owing to the weaker condition of Sripad and Snyder [Sri77] discussed in the next section.
30
Quantization
Similar to the one-dimensional quantization theorem, a two-dimensional theorem
[Wid61] can be formulated: the joint density function of the input can be reconstructed
from the joint density function of the output, if PX1X2(ju1, ju2) = 0 for u1 ≥ uo/2 and
u2 ≥ uo/2. Here again, the moments of the joint density function can be calculated as
follows:
E{Y m
1 Y n
2
} =(− j )m+n ∂ m+n
1 ∂un
∂um
2
PX1X2(ju1, ju2)
sin
(cid:8)
Q
2
(cid:7)
u1
Q
2
u1
sin
Q
2
(cid:8)
(cid:16)
(cid:16)
(cid:16)
(cid:16)
(cid:7)
u2
Q
2
u2
.
(2.40)
u1=0,u2=0
From this, the autocorrelation function with m = n2 − n1 can be written as
rYY (m) = E{Y1Y2}(m) =



E{X2} + Q2
12
E{X1X2}(m),
, m = 0,
elsewhere
(2.41)
(for m = 0 we obtain (2.34)).
2.1.3 Statistics of Quantization Error
First-order Statistics of Quantization Error
The PDF of the quantization error depends on the PDF of the input and is dealt with in
(cid:10)
the following. The quantization error e = xQ − x is restricted to the interval
. It
(cid:10)
2 , Q
depends linearly on the input (see Fig. 2.13). If the input value lies in the interval
2
then the error is e = 0 − x. For the PDF we obtain pE(e) = pX(e). If the input value lies
then the quantization error is e = Q(cid:6)Q−1x + 0.5(cid:7) − x
in the interval
(cid:10)
and is again restricted to
. The PDF of the quantization error is consequently
pE(e) = pX(e + Q) and is added to the ﬁrst term. For the sum over all intervals we can
write
(cid:10)
+ Q, Q
+ Q
(cid:9)
2
2 , Q
− Q
2 , Q
− Q
(cid:9)
2
− Q
− Q
2
(cid:9)
(cid:9)
2
pX(e − kQ), − Q
2
≤ e <
Q
2
,
elsewhere.
(2.42)


∞(cid:6)
pE(e) =

k=−∞
0,
(cid:28)(cid:14)(cid:14)(cid:14)7’:
0
(cid:5))1(cid:23)
)1(cid:23)
’
)1(cid:23)
(cid:5))1(cid:23)
(cid:2)
’
(cid:5))1(cid:23)
)1(cid:23)
Figure 2.13 Probability density function and quantization error.
Because of the restricted values of the variable of the PDF, we can write
(cid:1)
pE(e) = rect
(cid:1)
= rect
e
Q
e
Q
(cid:2) ∞(cid:6)
pX(e − kQ)
k=−∞
(cid:2)
[pX(e) ∗ δQ(e)].
(2.43)
(2.44)
2.1 Signal Quantization
31
The PDF of the quantization error is determined by the PDF of the input and can be
computed by shifting and windowing a zone. All individual zones are summed up for
calculating the PDF of the quantization error [Lip92]. A simple graphical interpretation
of this overlapping is shown in Fig. 2.14. The overlapping leads to a uniform distribution
of the quantization error if the input PDF pX(x) is spread over a sufﬁcient number of
quantization intervals.
(cid:28)(cid:14)(cid:14)(cid:14)7’:0
(cid:5))1(cid:23)
)1(cid:23)
’
(cid:30)(cid:28)(cid:14)(cid:14)(cid:14)7(cid:2):
(cid:2)
(cid:5))1(cid:23)
)1(cid:23)
(cid:2)
(cid:5))1(cid:23)
)1(cid:23)
Figure 2.14 Probability density function of the quantization error.
For the Fourier transform of the PDF from (2.44) it follows that
(cid:7)
u Q
2
u Q
2
(cid:8)
PE(ju) = 1
2π
PX(ju)
2π
Q
k=−∞
∞(cid:6)
sin
Q
(cid:4)
∗
(cid:8)
δ(u − kuo)
sin
(cid:7)
u Q
2
u Q
2
∞(cid:6)
=
=
(cid:4) ∞(cid:6)
∗
k=−∞
(cid:4)
PX(jkuo)
k=−∞
(cid:8)
PX(jkuo)δ(u − kuo)
(cid:7)
u Q
2
u Q
2
∗ δ(u − kuo)
sin
(cid:5)
(cid:5)
PE(ju) =
∞(cid:6)
k=−∞
PX(jkuo)
(cid:9)
sin
(u − kuo) Q
2
(u − kuo) Q
2
(cid:10)
.
(cid:5)
(2.45)
(2.46)
(2.47)
(2.48)
If the quantization theorem is satisﬁed, i.e. if PX(ju) = 0 for u > uo/2, then there is only
one non-zero term (k = 0 in (2.48)). The characteristic function of the quantization error is
reduced, with PX(0) = 1, to
PE(ju) =
.
(2.49)
sin
(cid:8)
(cid:7)
u Q
2
u Q
2
32
Hence, the PDF of the quantization error is
pE(e) = 1
Q
(cid:1)
rect
(cid:2)
.
e
Q
Quantization
(2.50)
Sripad and Snyder [Sri77] have modiﬁed the sufﬁcient condition of Widrow (band-limited
characteristic function of input) for a quantization error of uniform PDF by the weaker
condition
PX(j kuo) = PX
j
= 0 for all k (cid:8)= 0.
(2.51)
(cid:1)
(cid:2)
2πk
Q
The uniform distribution of the input PDF,
pX(x) = 1
Q
(cid:1)
rect
(cid:2)
,
x
Q
with characteristic function
PX(ju) =
(cid:8)
,
sin
(cid:7)
u Q
2
u Q
2
(2.52)
(2.53)
does not satisfy Widrow’s condition for a band-limited characteristic function, but instead
the weaker condition,
(cid:1)
PX
j
(cid:2)
2πk
Q
= sin(πk)
πk
= 0,
for all k (cid:8)= 0,
(2.54)
is fulﬁlled. From this follows the uniform PDF (2.49) of the quantization error. The weaker
condition from Sripad and Snyder extends the class of input signals for which a uniform
PDF of the quantization error can be assumed.
In order to show the deviation from the uniform PDF of the quantization error as a
function of the PDF of the input, (2.48) can be written as
PE (ju) = PX(0)
(cid:9)
sin
u Q
2
u Q
2
(cid:10)
+
∞(cid:6)
k=−∞,k(cid:8)=0
(cid:1)
∞(cid:6)
PX
j
(cid:1)
j
(cid:2)
PX
2πk
Q
2πk
Q
(cid:9)
sin
k=−∞,k(cid:8)=0
(cid:10)
u Q
2
u Q
2
=
(cid:10)
sin
(cid:9)
u Q
2
u Q
2
+
(cid:2)
(cid:9)
sin
(cid:10)
(u − kuo) Q
2
(u − kuo) Q
2
∗ δ(u − ku0).
(2.55)
The inverse Fourier transform yields
pE(e) = 1
Q


=

(cid:2)(cid:4)
(cid:1)
rect
e
Q
1 +
∞(cid:6)
k=−∞,k(cid:8)=0
(cid:2)
(cid:1)
PX
j
2πk
Q
(cid:4)
1 +
∞(cid:6)
k(cid:8)=0
1
Q
0,
(cid:1)
PX
j
(cid:1)
exp
j
(cid:2)
(cid:1)
exp
j
2πk
Q
2πke
Q
(cid:2)(cid:5)
(cid:2)(cid:5)
, − Q
2
2πke
Q
≤ e <
Q
2
,
(2.56)
(2.57)
elsewhere.
Equation (2.56) shows the effect of the input PDF on the deviation from a uniform PDF.
2.1 Signal Quantization
33
Second-order Statistics of Quantization Error
To describe the spectral properties of the error signal, two values E1 (at time n1) and E2
(at time n2) are considered [Lip92]. The joint PDF is given by
(cid:1)
pE1E2(e1, e2) = rect
e1
Q
,
e2
Q
(cid:2)
[pX1X2(e1, e2) ∗ δQQ(e1, e2)].
(2.58)
Here δQQ(e1, e2) = δQ(e1) · δQ(e2) and rect(e1/Q, e2/Q) = rect(e1/Q) · rect(e2/Q). For
the Fourier transform of the joint PDF, a similar procedure to that shown by (2.45)–(2.48)
leads to
PE1E2(ju1, ju2) =
∞(cid:6)
∞(cid:6)
k1=−∞
(cid:9)
sin
·
k2=−∞
(u1 − k1uo) Q
2
(u1 − k1uo) Q
2
PX1X2 (jk1uo, jk2uo)
(cid:9)
(u2 − k2uo) Q
2
(u2 − k2uo) Q
2
sin
(cid:10)
(cid:10)
.
If the quantization theorem and/or the Sripad–Snyder condition
PX1X2(jk1uo, jk2uo) = 0
for all k1, k2 (cid:8)= 0
are satisﬁed then
u1
Thus, for the joint PDF of the quantization error,
PE1E2(ju1, ju2) =
(cid:9)
sin
(cid:10)
sin
(cid:10)
.
Q
2
(cid:9)
u2
Q
2
u2
Q
2
u1
Q
2
(cid:1)
(cid:2)
pE1E2 (e1, e2) = 1
Q
rect
· 1
e1
Q
Q
= pE1 (e1) · pE2(e2).
rect
(cid:1)
(cid:2)
, − Q
2
e2
Q
≤ e1, e2 <
Q
2
Due to the statistical independence of quantization errors (2.63),
E{Em
1 En
2
} =E {Em
1
} · E{En
2
}.
For the moments of the joint PDF,
E{Em
1 En
2
} = (−j )m+n ∂ m+n
1 ∂un
∂um
2
PE1E2 (u1, u2)
(cid:16)
(cid:16)
(cid:16)
(cid:16)
.
u1=0,u2=0
From this, it follows for the autocorrelation function with m = n2 − n1 that
rEE(m) = E{E1E2} =
m = 0,
elsewhere
=
(cid:21)
E{E2},
E{E1E2},


Q2
12
0,
= Q2
12(cid:12)(cid:13)(cid:14)(cid:15)
σ 2
E
δ(m).

, m = 0,
elsewhere
(2.59)
(2.60)
(2.61)
(2.62)
(2.63)
(2.64)
(2.65)
(2.66)
(2.67)
(2.68)
34
The power density spectrum of the quantization error is then given by
SEE(ej (cid:4)) =
+∞(cid:6)
m=−∞
rEE(m) e
−j (cid:4)m = Q2
12
,
Quantization
(2.69)
which is equal to the variance σ 2
E
= Q2/12 of the quantization error (see Fig. 2.15).
r
EE
(m)
(cid:5)2
E
.(cid:6)(m)
EE
j(cid:3)
S (e )
Q2
12
-5 -4 -3 -2 -1
0
1
2
3
4
5
m
-(cid:4)
+(cid:4)
2(cid:4)
(cid:3)
Figure 2.15 Autocorrelation rEE(m) and power density spectrum SEE(ej (cid:4)) of quantization
error e(n).
Correlation of Signal and Quantization Error
To describe the correlation of the signal and the quantization error [Sri77], the second
moment of the output with (2.26) is derived as follows:
(cid:16)
(cid:16)
(cid:16)
(cid:16)
(2.70)
E{Y 2} =(− j )2 d 2PY (ju)
du2
(cid:4)
∞(cid:6)
(cid:2)
sin(πk)
πk
+ Q ˙PX
(cid:2)
(cid:1)
− 2πk
Q
sin(πk) − πk cos(πk)
π 2k2
¨PX
u=0
(cid:1)
− 2πk
Q
(cid:2)
= (−j )2
k=−∞
(cid:1)
PX
+ Q2
4
= E{X2} + Q
π
(cid:5)
− 2πk
Q
∞(cid:6)
(2 − π 2k2) sin(πk) − 2πk cos(πk)
π 3k3
(cid:1)
− 2πk
Q
(−1)k
k
+ E{E2}.
˙PX
(cid:2)
k=−∞,k(cid:8)=0
(2.71)
(2.72)
With the quantization error e(n) = y(n) − x(n),
E{Y 2} =E {X2} +2 E{X · E} +E {E2},
(2.73)
where the term E{X · E}, with (2.72), is written as
E{X · E} = Q
2π
∞(cid:6)
k=−∞,k(cid:8)=0
(−1)k
k
˙PX
(cid:1)
− 2πk
Q
(cid:2)
.
With the assumption of a Gaussian PDF of the input we obtain
pX(x) = 1√
2πσ
(cid:1)
exp
(cid:2)
−x2
2σ 2
(2.74)
(2.75)
2.1 Signal Quantization
with the characteristic function
(cid:1)
PX(ju) = exp
(cid:2)
.
−u2σ 2
2
35
(2.76)
Using (2.57), the PDF of the quantization error is then given by
pE(e) =
(cid:4)



1
Q
0,
1 + 2
∞(cid:6)
k=1
(cid:1)
cos
(cid:2)
(cid:1)
− 2π 2k2σ 2
Q2
exp
(cid:2)(cid:5)
,
2πke
Q
− Q
2
≤ e <
Q
2
,
elsewhere.
(2.77)
Figure 2.16a shows the PDF (2.77) of the quantization error for different variances of the
input.
2
1.8
1.6
1.4
1.2
1
0.8
0.6
0.4
0.2
→
)
e
(
E
p
0.2
a)
0.3
0.4
0.8
0.09
0.08
0.07
0.06
0.05
0.04
0.03
0.02
0.01
→
)
2
E
(
E
b)
Q 1
0
-0.5 -0.4 -0.3 -0.2 -0.1
0
0.1 0.2 0.3 0.4 0.5
0
0
0.1 0.2 0.3 0.4 0.5 0.6 0.7 0.8 0.9
1
e/Q →
σ →
Figure 2.16 (a) PDF of quantization error for different standard deviations of a Gaussian PDF input.
(b) Variance of quantization error for different standard deviations of a Gaussian PDF input.
For the mean value and the variance of a quantization error, it follows using (2.77) that
E{E} =0 and
E{E2} =
(cid:3) ∞
−∞
e2pE(e) de = Q2
12
(cid:4)
1 + 12
π 2
∞(cid:6)
k=1
(−1)k
k2
(cid:1)
− 2π 2k2σ 2
Q2
exp
(cid:2)(cid:5)
.
(2.78)
Figure 2.16b shows the variance of the quantization error (2.78) for different variances of
the input.
For a Gaussian PDF input as given by (2.75) and (2.76), the correlation (see (2.74))
between input and quantization error is expressed as
E{X · E} =2 σ 2
(−1)k exp
(cid:1)
− 2π 2k2σ 2
Q2
(cid:2)
.
∞(cid:6)
k=1
(2.79)
The correlation is negligible for large values of σ/Q.
36
2.2 Dither
2.2.1 Basics
Quantization
The requantization (renewed quantization of already quantized signals) to limited word-
lengths occurs repeatedly during storage, format conversion and signal processing algo-
rithms. Here, small signal levels lead to error signals which depend on the input. Owing
to quantization, nonlinear distortion occurs for low-level signals. The conditions for the
classical quantization model are not satisﬁed anymore. To reduce these effects for signals
of small amplitude, a linearization of the nonlinear characteristic curve of the quantizer is
performed. This is done by adding a random sequence d(n) to the quantized signal x(n)
(see Fig. 2.17) before the actual quantization process. The speciﬁcation of the word-length
is shown in Fig. 2.18. This random signal is called dither. The statistical independence of
the error signal from the input is not achieved, but the conditional moments of the error
signal can be affected [Lip92, Ger89, Wan92, Wan00].
-7(cid:10):
7(cid:18)(cid:14).(cid:16)(cid:7):
’7(cid:10):
7;9(cid:8)(cid:14).(cid:16)(cid:7):
)
"7(cid:10):
7;(cid:14).(cid:16)(cid:7):
Figure 2.17 Addition of a random sequence before a quantizer.
;(cid:14).(cid:16)(cid:7)
(cid:8)(cid:14).(cid:16)(cid:7)
(cid:12)
(cid:18)(cid:14)
H(cid:2)(cid:8)(cid:11)(cid:14).(cid:16)(cid:7)(cid:18)
(cid:26)(cid:16)(cid:7)(cid:21)(cid:2)(cid:8)
(cid:6)(cid:16)(cid:3)(cid:10)
Figure 2.18 Speciﬁcation of the word-length.
The sequence d(n), with amplitude range (−Q/2 ≤ d(n) ≤ Q/2), is generated with
the help of a random number generator and is added to the input. For a dither value with
Q = 2−(w−1):
dk = k2
−r Q, −2s−1 ≤ k ≤ 2s−1 − 1.
(2.80)
The index k of the random number dk characterizes the value from the set of N = 2s
possible numbers with the probability
P (dk) =
(cid:21)
2−s, −2s−1 ≤ k ≤ 2s−1 − 1,
0,
elsewhere.
(2.81)
(cid:11)
k dkP (dk), the variance σ 2
D
=
(cid:11)
k
[dk − d]2P (dk) and the
With the mean value d =
quadratic mean d 2 =
k d 2
(cid:11)
k P (dk), we can rewrite the variance as σ 2
d
For a static input amplitude V and the dither value dk the rounding operation [Lip86] is
= d 2 − d
.
2
expressed as
(cid:22)
g(V + dk) = Q
(cid:23)
+ 0.5
.
V + dk
Q
(2.82)
2.2 Dither
For the mean of the output g(V ) as a function of the input V , we can write
g(V ) =
(cid:6)
k
g(V + dk)P (dk).
The quadratic mean of the output g2(V ) for input V is given by
g2(V ) =
(cid:6)
k
g2(V + dk)P (dk).
For the variance d 2
R(V ) for input V ,
d 2
R(V ) =
(cid:6)
{g(V + dk) − g(V )}2P (dk) = g2(V ) − {g(V )}2.
k
37
(2.83)
(2.84)
(2.85)
The above equations have the input V as a parameter. Figures 2.19 and 2.20 illustrate the
mean output g(V ) and the standard deviation dR(V ) within a quantization step size, given
by (2.83), (2.84) and (2.85). The examples of rounding and truncation demonstrate the
linearization of the characteristic curve of the quantizer. The coarse step size is replaced
by a ﬁner one. The quadratic deviation from the mean output d 2
R(V ) is termed noise
modulation. For a uniform PDF dither, this noise modulation depends on the amplitude
(see Figs. 2.19 and 2.20). It is maximum in the middle of the quantization step size
and approaches zero toward the end. The linearization and the suppression of the noise
modulation can be achieved by a triangular PDF dither with bipolar characteristic [Van89]
and rounding operation (see Fig. 2.20). Triangular PDF dither is obtained by adding two
statistically independent dither signals with uniform PDF (convolution of PDFs). A dither
signal with higher-order PDF is not necessary for audio signals [Lip92, Wan00].
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
Unipolar RECT Dither
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
Unipolar TRI Dither
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
Figure 2.19 Truncation – linearizing and suppression of noise modulation (s = 4, m = 0).
The total noise power for this quantization technique consists of the dither power and
the power of the quantization error [Lip86]. The following noise powers are obtained by
integration with respect to V .

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
