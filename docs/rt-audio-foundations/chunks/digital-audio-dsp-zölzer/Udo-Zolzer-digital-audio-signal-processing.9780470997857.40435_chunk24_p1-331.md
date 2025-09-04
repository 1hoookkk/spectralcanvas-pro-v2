# Digital Audio DSP (Zölzer) — Chunk 24/30

> Source: `Udo-Zolzer-digital-audio-signal-processing.9780470997857.40435.pdf` · Pages ~1-331 · Extractor: pdfminer.six
> Target ≈ 2800 words, Overlap = 200 words · Actual = 4469

> Note: DSP textbook content for SpectralCanvasPro RT-audio knowledge base

---
Chapter 8
Sampling Rate Conversion
Several different sampling rates are established for digital audio applications. For broad-
casting, professional and consumer audio, sampling rates of 32, 48 and 44.1 kHz are used.
Moreover, other sampling rates are derived from different frame rates for ﬁlm and video. In
connecting systems with different uncoupled sampling rates, there is a need for sampling
rate conversion. In this chapter, synchronous sampling rate conversion with rational factor
L/M for coupled clock rates and asynchronous sampling rate conversion will be discussed
where the different sampling rates are not synchronized with each other.
8.1 Basics
Sampling rate conversion consists out of upsampling and downsampling and anti-imaging
and anti-aliasing ﬁltering [Cro83, Vai93, Fli00, Opp99]. The discrete-time Fourier trans-
form of the sampled signal x(n) with sampling frequency fS = 1/T (ωS = 2πfS) is given
by
X(ej (cid:4)) = 1
T
∞(cid:6)
k=−∞
(cid:1)
Xa
j ω + j k
2π
T(cid:12)(cid:13)(cid:14)(cid:15)
ωS
(cid:2)
, (cid:4) = ωT ,
(8.1)
with the Fourier transform Xa(j ω) of the continuous-time signal x(t). For ideal sampling
the condition
X(ej (cid:4)) = 1
T
Xa(j ω),
|(cid:4)| ≤ π,
holds.
8.1.1 Upsampling and Anti-imaging Filtering
For upsampling the signal
x(n) ◦—• X(ej (cid:4))
Digital Audio Signal Processing Second Edition Udo Zölzer
© 2008 John Wiley & Sons, Ltd
(8.2)
(8.3)
242
Sampling Rate Conversion
by a factor L between consecutive samples L − 1 zero samples will be included (see
Fig. 8.1). This leads to the upsampled signal
w(m) =
(cid:24)
$
x
m
L
0,
(cid:25)
, m = 0, ±L, ±2L, . . . ,
otherwise,
(8.4)
with sampling frequency f (cid:14)
S
Fourier transform
= 1/T (cid:14) = L · fS = L/T ((cid:4)(cid:14) = (cid:4)/L) and the corresponding
W (ej (cid:4)(cid:14)
) =
∞(cid:6)
m=−∞
w(m) e
−j m(cid:4)(cid:14) =
∞(cid:6)
m=−∞
x(m) e
−j mL(cid:4)(cid:14) = X(ej L(cid:4)(cid:14)
).
(8.5)
The suppression of the image spectra is achieved by anti-imaging ﬁltering of w(m) with
h(m), such that the output signal is given by
y(m) = w(m) ∗ h(m),
) = H (ej (cid:4)(cid:14)
Y (ej (cid:4)(cid:14)
) · X(ej (cid:4)(cid:14)L).
(8.6)
(8.7)
To adjust the signal power in the base-band the Fourier transform of the impulse-response
H (ej (cid:4)(cid:14)
) =
(cid:21)
L,
0,
|(cid:4)(cid:14)| ≤ π/L,
otherwise,
(8.8)
needs a gain factor L in the pass-band, such that the output signal y(m) has the Fourier
transform given by
Y (ej (cid:4)(cid:14)
) = LX(ej (cid:4)(cid:14)L)
∞(cid:6)
1
T
(cid:12)
k=−∞
= L
(cid:1)
Xa
j ω + j Lk
(cid:13)(cid:14)
with (8.1) and (8.5)
(cid:1)
∞(cid:6)
j ω + j Lk
= L
1
LT (cid:14)
Xa
(cid:1)
k=−∞
∞(cid:6)
Xa
= 1
T (cid:14)
(cid:12)
spectrum of signal with f (cid:14)
S
j ω + j k
(cid:13)(cid:14)
k=−∞
2π
T (cid:14)
(cid:15)
=LfS
.
2π
T
(cid:2)
(cid:15)
2π
LT (cid:14)
(cid:2)
(cid:2)
(8.9)
(8.10)
(8.11)
(8.12)
The output signal represents the sampling of the input x(t) with sampling frequency f (cid:14)
S
LfS.
=
8.1.2 Downsampling and Anti-aliasing Filtering
For downsampling a signal x(n) by M the signal has to be band-limited to π/M in order to
avoid aliasing after the downsampling operation (see Fig. 8.2). Band-limiting is achieved
8.1 Basics
243
x(n)
fS=1/T
(cid:3) (cid:12)= T
w(m)
L
h(m)
y(m)
f ’S=1/T’=LfS
(cid:3) (cid:12)
(cid:12)
'= T'= T/L= /L
(cid:3)
f ’S
x(n)
X(ej
)(cid:3)
0
1
2
3
n
w(m)
3
1/T
L
1/T
(cid:4)
2(cid:4)
4(cid:4)
6(cid:4)
(cid:3)
W(ej
’)=(cid:3) X(ejL ’)(cid:3)
’)(cid:3)
|H
(ej
|
0
1
2
3
4
5
6
7
8
9
m
(cid:4)/3
(cid:4)/L
2 /3(cid:4)
2 /L(cid:4)
(cid:4)
4 /3(cid:4)
4 /L(cid:4)
(cid:25)(cid:4)
6 /L(cid:4)
(cid:3)’ L=3
(cid:3) (cid:3)’= /L
y(m)
Y(ej
')(cid:3)
L/T=1/T’
0
1
2
3
4
5
6
7
8
9
m
(cid:4)/3
(cid:4)
2(cid:4)
(cid:3)’
Figure 8.1 Upsampling by L and anti-imaging ﬁltering in time and frequency domain.
by ﬁltering with H (ej (cid:4)) according to
w(m) = x(m) ∗ h(m),
W (ej (cid:4)) = X(ej (cid:4)) · H (ej (cid:4)),
|(cid:4)| ≤ π/M,
1,
0, otherwise.
H (ej (cid:4)) =
(cid:21)
(8.13)
(8.14)
(8.15)
Downsampling of w(m) is performed by taking every Mth sample, which leads to the
output signal
with the Fourier transform
y(n) = w(Mn)
Y (ej (cid:4)(cid:14)
) = 1
M
M−1(cid:6)
l=0
W (ej ((cid:4)(cid:14)−2πl)/M)).
For the base-band spectrum (|(cid:4)(cid:14)| ≤ π and l = 0) we get
(8.16)
(8.17)
Y (ej (cid:4)(cid:14)
) = 1
M
H (ej (cid:4)(cid:14)/M ) · X(ej (cid:4)(cid:14)/M ) = 1
M
X(ej (cid:4)(cid:14)/M ),
|(cid:4)
(cid:14)| ≤ π,
(8.18)
244
Sampling Rate Conversion
and for the Fourier transform of the output signal we can derive
Y (ej (cid:4)(cid:14)
) = 1
M
X(ej (cid:4)(cid:14)/M ) = 1
M
∞(cid:6)
k=−∞
1
T
(cid:12)
(cid:1)
Xa
j ω + j k
(cid:13)(cid:14)
with (8.1)
(cid:1)
2π
MT
(cid:2)
(cid:15)
∞(cid:6)
= 1
T (cid:14)
(cid:12)
k=−∞
(cid:13)(cid:14)
Xa
j ω + j k
2π
T (cid:14)
(cid:2)
,
(cid:15)
(8.19)
(8.20)
which represents a sampled signal y(n) with f (cid:14)
S
= fS/M.
spectrum of signal with f
(cid:14)
S
=fS /M
x(m)
h(m)
fS=1/T
(cid:3) (cid:12)= T
w(m)
fS
M
y(n)
f ’S=1/T’=fS/M
(cid:12)
(cid:3) (cid:12)
’= T’= TM=
(cid:3)(cid:26)
x(m)
X(ej
(cid:3)
)
1
1/T
(ej
)(cid:3)
|
|H
0
1
2
3
4
5
6
7
8
9
m
(cid:4)/3
2 /3(cid:4)
(cid:4)
4 /3(cid:4)
(cid:25)(cid:4)
(cid:3)
w(m)
W(ej
(cid:3)
)
1/T
0
1
2
3
4
5
6
7
8
9
m
(cid:4)/3
2 /3(cid:4)
(cid:4)
4 /3(cid:4)
(cid:25)(cid:4)
(cid:3)
y(n)
3
1/MT=1/T’
Y(ej
(cid:3)
’)
0
1
2
3
n
(cid:4)
M /3(cid:4)
2(cid:4)
M2 /3(cid:4)
M(cid:4)
4(cid:4)
M4 /3(cid:4)
6(cid:4)
M(cid:25)(cid:4)
(cid:3)’
M=3
(cid:3) (cid:3)’= M
Figure 8.2 Anti-aliasing ﬁltering and downsampling by M in time and frequency domain.
8.2 Synchronous Conversion
Sampling rate conversion for coupled sampling rates by a rational factor L/M can be
performed by the system shown in Fig. 8.3. After upsampling by a factor L, anti-imaging
ﬁltering at LfS is carried out, followed by downsampling by factor M. Since after up-
sampling and ﬁltering only every Mth sample is used, it is possible to develop efﬁcient
8.2 Synchronous Conversion
245
algorithms that reduce complexity. In this respect two methods are in use: one is based on
a time-domain interpretation [Cro83] and the other [Hsi87] uses Z-domain fundamentals.
Owing to its computational efﬁciency, only the method in the Z-domain will be considered.
x(n)
fS
L
w(k)
LfS
h(k)
v(k)
LfS
M
y(m)
fSO
=(L/M)fS
Figure 8.3 Sampling rate conversion by factor L/M.
Starting with the ﬁnite impulse response h(n) of length N and its Z-transform
H (z) =
N−1(cid:6)
n=0
h(n)z
−n,
(8.21)
the polyphase representation [Cro83, Vai93, Fli00] with M components can be expressed as
with
or
with
H (z) =
M−1(cid:6)
k=0
−kEk(zM )
z
ek(n) = h(nM + k),
k = 0, 1, . . . , M − 1,
H (z) =
M−1(cid:6)
k=0
−(M−1−k)Rk(zM )
z
rk(n) = h(nM − k),
k = 0, 1, . . . , M − 1.
(8.22)
(8.23)
(8.24)
(8.25)
The polyphase decomposition as given in (8.22) and (8.24) is referred to as type 1 and 2, re-
spectively. The type 1 polyphase decomposition corresponds to a commutator model in the
anti-clockwise direction whereas the type 2 is in the clockwise direction. The relationship
between R(z) and E(z) is described by
Rk(z) = EM−1−k(z).
(8.26)
With the help of the identities [Vai93] shown in Fig. 8.4 and the decomposition (Euclid’s
theorem)
−1 = z
−pLzqM,
z
(8.27)
it is possible to move the inner delay elements of Fig. 8.5. Equation (8.27) is valid if M
and L are prime numbers. In a cascade of upsampling and downsampling, the order of
functional blocks can be exchanged (see Fig. 8.5b).
The use of polyphase decomposition can be demonstrated with the help of an example
for L = 2 andM = 3. This implies a sampling rate conversion from 48 kHz to 32 kHz.
Figures 8.6 and 8.7 show two different solutions for polyphase decomposition of sam-
pling rate conversion by 2/3. Further decompositions of the upsampling decomposition of
Fig. 8.7 are demonstrated in Fig. 8.8. First, interpolation is implemented with a polyphase
246
Sampling Rate Conversion
H(z )M
M
M
H(z)
a)
x(n)
fS
b)
L
H(z )L
H(z)
L
Figure 8.4 Identities for sampling rate conversion.
x(n)
fS
L
z-pL+qM
M
LfS
y(m)
fSO
=(L/M)fS
z-p
L
M
z+q
y(m)
fSO
=(L/M)fS
LfS
LfS
fS/M
L
M
x(n)
fS
x(n)
fS
M
L
y(m)
fSO
=(L/M)fS
y(m)
fSO
=(L/M)fS
Figure 8.5 Decomposition in accordance with Euclid’s theorem.
decomposition and the delay z−1 is decomposed to z−1 = z−2z3. Then, the downsampler
of factor 3 is moved through the adder into the two paths (Fig. 8.8b) and the delays are
moved according to the identities of Fig. 8.4. In Fig. 8.8c, the upsampler is exchanged with
the downsampler, and in a ﬁnal step (Fig. 8.8d) another polyphase decomposition of E0(z)
and E1(z) is carried out. The actual ﬁlter operations E0k(z) and E1k(z) with k = 0, 1, 2 are
performed at 1
3 of the input sampling rate.
8.3 Asynchronous Conversion
Plesiochronous systems consist of partial systems with different and uncoupled sampling
rates. Sampling rate conversion between such systems can be achieved through a DA
conversion with the sampling rate of the ﬁrst system followed by an AD conversion with the
sampling rate of the second system. A digital approximation of this approach is made with
a multirate system [Lag81, Lag82a, Lag82b, Lag82c, Lag83, Ram82, Ram84]. Figure 8.9a
shows a system for increasing the sampling rate by a factor L followed by an anti-imaging
8.3 Asynchronous Conversion
247
x(n)
x(n)
2
2
H(z)
3
y(m)
y(m)
z-1
z-1
3
3
3
E (z)
0
E (z)
1
E (z)
2
Figure 8.6 Polyphase decomposition for downsampling L/M = 2/3.
x(n)
2
H(z)
3
y(m)
x(n)
E (z)
0
E (z)
1
2
2
3
y(m)
z-1
Figure 8.7 Polyphase decomposition for upsampling L/M = 2/3.
ﬁlter H (z) and a resampling of the interpolated signal y(k). The samples y(k) are held
= 1/fSO .
for a clock period (see Fig. 8.9c) and then sampled with output clock period TSO
The interpolation sampling rate must be increased sufﬁciently that the difference of two
consecutive samples y(k) is smaller than the quantization step Q. The sample-and-hold
function applied to y(k) suppresses the spectral images at multiples of LfS (see Fig. 8.9b).
The signal obtained is a band-limited continuous-time signal which can be sampled with
output sampling rate fSO .
For the calculation of the necessary oversampling rate, the problem is considered in
the frequency domain. The sinc function of a sample-and-hold system (see Fig. 8.9b) at
frequency ˜f = (L − 1
2 )fS is given by
(cid:24)
(cid:25)
sin
E( ˜f ) =
π ˜f
LfS
π ˜f
LfS
=
(cid:24)
sin
π(L− 1
LfS
2 )fS
(cid:25)
π(L− 1
LfS
2 )fS
=
(cid:7)
sin
π − π
2L
π − π
2L
(cid:8)
.
(8.28)
248
a)
x(n)
b)
x(n)
c)
x(n)
d)
x(n)
E (z)
0
E (z)
1
E (z)
0
E (z)
1
E (z)
0
E (z)
1
2
2
2
2
3
3
3
z-3
z2
3
3
2
2
3
3
3
3
3
3
E (z)
00
E (z)
01
E (z)
02
E (z)
10
E (z)
11
E (z)
12
z1
z1
z-1
z1
z-1
z-1
z-1
Sampling Rate Conversion
3
y(m)
y(m)
y(m)
z-1
z-1
2
2
y(m)
z-1
Figure 8.8 Sampling rate conversion by factor 2/3.
With sin(α − β) = sin(α) cos(β) − cos(α) sin(β) we derive
(cid:8) ≈ 1
(cid:8)
(cid:7)
E( ˜f ) =
π
sin
2L
(cid:7)
1 − 1
2L
π
(cid:8) ≈ π/2L
(cid:7)
1 − 1
π
2L
2L − 1
≈ 1
2L
.
(8.29)
8.3 Asynchronous Conversion
a)
x(n)
fS
L
H(z)
y(k)
LfS
Rect
y(t)
fSO
249
y(m)
b)
Y(f)
fS
 π
sin( f/Lf )
S
~
f
LfS
f
c)
y(k)
y(t)
k
k
+1
k+2
k
k+3
Q
Figure 8.9 Approximation of DA/AD conversions.
This value of (8.29) should be lower than Q
2 and allows the computation of the interpolation
factor L. For a given word-length w and quantization step Q, the necessary interpolation
rate L is calculated by
,
Q
2
2−(w−1)
2
≥ 1
2L
≥ 1
2L
(cid:17)→ L ≥ 2w−1.
For a linear interpolation between upsampled samples y(k), we can derive
E( ˜f ) =
=
(cid:24)
(cid:25)
π ˜f
LfS
(cid:25)
2
sin2
(cid:24)
π ˜f
LfS
(cid:24)
sin2
(cid:24)
π(L− 1
LfS
2 )fS
(cid:25)
2
π(L− 1
LfS
2 )fS
(cid:25)
≈ 1
(2L)2 .
(8.30)
(8.31)
(8.32)
(8.33)
(8.34)
(8.35)
250
Sampling Rate Conversion
With this it is possible to reduce the necessary interpolation rate to
L1 ≥ 2w/2−1.
(8.36)
Figure 8.10 demonstrates this with a two-stage block diagram. First, interpolation up to a
sampling rate L1fS is performed by conventional ﬁltering. In a second stage upsampling by
factor L2 is done by linear interpolation. The two-stage approach must satisfy the sampling
rate LfS
= (L1L2)fS.
The choice of the interpolation algorithm in the second stage enables the reduction of
the ﬁrst oversampling factor. More details are discussed in Section 8.2.2.
y(k)
y(t)
x(n)
L1
H (z)1
L2
H (z)2
Rect
y(m)
fSO
Figure 8.10 Linear interpolation before virtual sample-and-hold function.
8.3.1 Single-stage Methods
Direct conversion methods implement the block diagram [Lag83, Smi84, Par90, Par91a,
Par91b, Ada92, Ada93] shown in Fig. 8.9a. The calculation of a discrete sample on an
output grid of sampling rate fSO from samples x(n) at sampling rate fSI can be written as
DFT[x(n − α)] = X(ej (cid:4)) e
−j α(cid:4) = X(ej (cid:4))Hα(ej (cid:4)),
where 0 ≤ α < 1. With the transfer function
and the properties
Hα(ej (cid:4)) = e
−j α(cid:4)
(cid:21)
H (ej (cid:4)) =
1, 0 ≤ |(cid:4)| ≤ (cid:4)c,
0, (cid:4)c < |(cid:4)| < π,
the impulse response is given by
hα(n) = h(n − α) = (cid:4)c
π
sin[(cid:4)c(n − α)]
(cid:4)c(n − α)
.
From (8.37) we can express the delayed signal
∞(cid:6)
x(n − α) =
x(m)h(n − α − m)
m=−∞
∞(cid:6)
m=−∞
=
x(m)
(cid:4)c
π
sin[(cid:4)c(n − α − m)]
(cid:4)c(n − α − m)
(8.37)
(8.38)
(8.39)
(8.40)
(8.41)
(8.42)
as the convolution between x(n) and h(n − α). Figure 8.11 illustrates this convolution in
the time domain for a ﬁxed α. Figure 8.12 shows the coefﬁcients h(n − αi) for discrete
8.3 Asynchronous Conversion
251
’7(cid:10)(cid:5)(cid:14)(cid:14):(cid:28)
’7(cid:10):
’7(cid:10)(cid:5)(cid:22):
’7(cid:10)9(cid:22):
’7(cid:7):
’7(cid:10)(cid:5)(cid:23):
(cid:18)(cid:16)(cid:10)(cid:20)(cid:131)(cid:14)(cid:14)(cid:14)(cid:14)(cid:14)7(cid:10)(cid:5)(cid:14)(cid:14):(cid:132)(cid:28)
(cid:20)(cid:29)(cid:20)
(cid:28)
(cid:10)(cid:5)(cid:23)
(cid:10)(cid:5)(cid:22)
(cid:10)(cid:5)(cid:28)
(cid:10)
(cid:10)9(cid:22)
(cid:10)
Figure 8.11 Convolution sum (8.42) in the time domain.
’7(cid:10)(cid:5)(cid:22):
’7(cid:10)(cid:5)(cid:23):
’7(cid:10):
’7(cid:10)9(cid:22):
’7(cid:7):
(cid:18)(cid:16)(cid:10)(cid:20)(cid:131)(cid:14)(cid:14)(cid:14)(cid:14)(cid:14)7(cid:10)(cid:5)(cid:14)(cid:14)(cid:14)(cid:14):(cid:132)(cid:28)
(cid:20)(cid:29)(cid:20)
(cid:16)
(cid:10)(cid:5)(cid:23)
(cid:10)(cid:5)(cid:22)
(cid:10)
(cid:10)9(cid:22)
(cid:10)
(cid:16)(cid:14)M(cid:14)2(cid:14)(cid:14)(cid:14)(cid:14)(cid:14)(cid:23)(cid:14)(cid:14)(cid:14)(cid:14)(cid:14)(cid:22)(cid:14)(cid:14)(cid:14)(cid:14)(cid:14)3
Figure 8.12 Convolution sum (8.42) for different αi .
αi (i = 0, . . . , 3) which are obtained from the intersection of the sinc function with the
discrete samples x(n).
In order to limit the convolution sum, the impulse response is windowed, which gives
hW (n − αi ) = w(n)
(cid:4)c
π
sin[(cid:4)c(n − αi )]
(cid:4)c(n − αi)
,
n = 0, . . . , 2M.
(8.43)
From this, the sample estimate
ˆx(n − αi ) =
M(cid:6)
m=−M
x(m)hW (n − αi − m)
(8.44)
results. A graphical interpretation of the time-variant impulse response which depends on
αi is shown in Fig. 8.13. The discrete segmentation between two input samples into N
intervals leads to N partial impulse responses of length 2M + 1.
If the output sampling rate is smaller than the input sampling rate (fSO < fSI ), band-
limiting (anti-aliasing) to the output sampling rate has to be done. This can be achieved
with factor β = fSO /fSI and leads, with the scaling theorem of the Fourier transform, to
h(n − α) = β(cid:4)c
π
sin[β(cid:4)c(n − α)]
β(cid:4)c(n − α)
.
(8.45)
252
Sampling Rate Conversion
This time-scaling of the impulse response has the consequence that the number of coefﬁ-
cients of the time-variant partial impulse responses is increased. The number of required
states also increases. Figure 8.14 shows the time-scaled impulse response and elucidates
the increase in the number M of the coefﬁcients.
(cid:68)=0
(cid:68)=0.25
(cid:68)=0.5
(cid:68)=0.75
-M
0
M
Figure 8.13 Sinc function and different impulse responses.
(cid:68)=0
(cid:68)=0.25
(cid:68)=0.5
(cid:68)=0.75
-M
0
M
Figure 8.14 Time-scaled impulse response.
8.3.2 Multistage Methods
The basis of a multistage conversion method [Lag81, Lag82, Kat85, Kat86] is shown
in Fig. 8.15a and will be described in the frequency domain as shown in Fig. 8.15b–d.
Increasing the sampling rate up to LfS before the sample-and-hold function is done in four
stages. In the ﬁrst two stages, the sampling rate is increased by a factor of 2 followed by
an anti-imaging ﬁlter (see Fig. 8.15b,c), which leads to a four times oversampled spectrum
8.3 Asynchronous Conversion
253
(Fig. 8.15d). In the third stage, the signal is upsampled by a factor of 32 and the image
spectra are suppressed (see Fig. 8.15d,e). In the fourth stage (Fig. 8.15e) the signal is
upsampled to a sampling rate of LfS by a factor of 256 and a linear interpolator. The
sinc2 function of the linear interpolator suppresses the images at multiples of 128fS up to
the spectrum at LfS. The virtual sample-and-hold function is shown in Fig. 8.15f, where
resampling at the output sampling rate is performed. A direct conversion of this kind of
cascaded interpolation structure requires anti-imaging ﬁltering after every upsampling with
the corresponding sampling rate. Although the necessary ﬁlter order decreases owing to
a decrease in requirements for ﬁlter design, an implementation of the ﬁlters in the third
and fourth stages is not possible directly. Following a suggestion by Lagadec [Lag82c],
the measurement of the ratio of input to output rate is used to control the polyphase
ﬁlters in the third and fourth stages (see Fig. 8.16a, CON = control) to reduce complexity.
Figures 8.16b–d illustrate an interpretation in the time domain. Figure 8.16b shows the
interpolation of three samples between two input samples x(n) with the help of the ﬁrst and
second interpolation stage. The abscissa represents the intervals of the input sampling rate
and the sampling rate is increased by factor of 4. In Fig. 8.16c the four times oversampled
signal is shown. The abscissa shows the four times oversampled output grid. It is assumed
that output sample y(m = 0) and input sample x(n = 0) are identical. The output sample
y(m = 1) is now determined in such a form that with the interpolator in the third stage only
two polyphase ﬁlters just before and after the output sample need to be calculated. Hence,
only two out of a total of 31 possible polyphase ﬁlters are calculated in the third stage.
Figure 8.16d shows these two polyphase output samples. Between these two samples, the
output sample y(m = 1) is obtained with a linear interpolation on a grid of 255 values.
Instead of the third and fourth stages, special interpolation methods can be used to cal-
culate the output y(m) directly from the four times oversampled input signal (see Fig. 8.17)
[Sti91, Cuc91, Liu92]. The upsampling factor L3 = 2w−3 for the last stage is calculated
according to L = 2w−1 = L1L2L3 = 22L3. Section 8.4 is devoted to different interpolation
methods which allow a real-time calculation of ﬁlter coefﬁcients. This can be interpreted
as time-variant ﬁlters in which the ﬁlter coefﬁcients are derived from the ratio of sampling
rates. The calculation of one ﬁlter coefﬁcient set for the output sample at the output rate
is done by measuring the ratio of input to output sampling rate as described in the next
section.
8.3.3 Control of Interpolation Filters
The measurement of the ratio of input and output sampling rate is used for controlling the
interpolation ﬁlters [Lag82a]. By increasing the sampling rate by a factor of L the input
sampling period is divided into L = 2w−1 = 215 parts for a signal word-length of w = 16
bits. The time instant of the output sample is calculated on this grid with the help of the
measured ratio of sampling periods TSO /TSI as follows.
A counter is clocked with LfSI and reset by every new input sampling clock. A saw-
tooth curve of the counter output versus time is obtained as shown in Fig. 8.18. The
counter runs from 0 to L − 1 during one input sampling period. The output sampling period
TSO starts at time ti−2, which corresponds to counter output zi−2, and stops at time ti−1,
with counter output zi−1. The difference between both counter measurements allows the
calculation of the output sampling period TSO with a resolution of LfSI .
x(n)
2
H (z)1
2
H (z)2
32
H (z)3
256
H (z)4
Sampling Rate Conversion
y(k)
y(t)
Rect
y(m)
fSO
254
a)
b)
c)
Y(f)
H (f)1
Y(f)
H (f)2
fS
2fS
3fS
4fS
fS
2fS
3fS
4fS
d)
Y(f)
H (f)3
fS
2fS
3fS
4fS
e)
Y(f)
H (f)=4
si (f)2
4fS
32fS
64fS
128fS
f)
Y(f)
si( f/Lf )
S
LfS
LfS
LfS
f
f
f
f
f
fS
2fS
3fS
4fS
LfS
Figure 8.15 Multistage conversion – frequency-domain interpretation.
The new counter measurement is added to the difference of previous counter measure-
ments. As a result, the new counter measurement is obtained as
ti = (ti−1 + TSO ) ⊕ TSI .
The modulo operation can be carried out with an accumulator of word-length w − 1 = 15.
The resulting time ti determines the time instant of the output sample at the output sampling
rate and therefore the choice of the polyphase ﬁlter in a single-stage conversion or the time
instant for a multistage conversion.
(8.46)

8.3 Asynchronous Conversion
