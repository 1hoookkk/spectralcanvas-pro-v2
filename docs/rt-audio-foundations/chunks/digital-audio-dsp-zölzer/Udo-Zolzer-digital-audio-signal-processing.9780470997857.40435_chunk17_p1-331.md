# Digital Audio DSP (Zölzer) — Chunk 17/30

> Source: `Udo-Zolzer-digital-audio-signal-processing.9780470997857.40435.pdf` · Pages ~1-331 · Extractor: pdfminer.six
> Target ≈ 2800 words, Overlap = 200 words · Actual = 13178

> Note: DSP textbook content for SpectralCanvasPro RT-audio knowledge base

---
151
SNR/dB
100
Zölzer
Gold and Rader
Kingsbury
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
Figure 5.47 SNR vs. cutoff frequency – quantization after accumulator (fc > 2 kHz).
of the error signals in addition to the complex poles. This zero near the poles reduces
the amplifying effect of the pole. If it is now possible to introduce another zero into the
noise transfer function then the effect of the poles is compensated for to a larger extent.
The procedure of feeding back the quantization error as shown in Chapter 2 produces an
additional zero in the noise transfer function [Tra77, Cha78, Abu79, Bar82, Zöl89]. The
feedback of the quantization error is ﬁrst demonstrated with the help of the direct-form
structure as shown in Fig. 5.48. This generates a zero at z = 1 in the noise transfer function
given by
G1.O(z) =
1 − z−1
1 + b1z−1 + b2z−2 .
(5.113)
(5.114)
The resulting variance σ 2 of the quantization error at the output of the ﬁlter is presented
in Fig. 5.48. In order to produce two zeros at z = 1, the quantization error is fed back
over two delays weighted 2 and −1 (see Fig. 5.48b). The noise transfer function is, hence,
given by
G2.O(z) = 1 − 2z−1 + z−2
1 + b1z−1 + b2z−2 .
The signal-to-noise ratio of the direct-form is plotted versus the cutoff frequency in
Fig. 5.49. Even a single zero signiﬁcantly improves the signal-to-noise ratio in the direct
form. The coefﬁcients b1 and b2 approach −2 and 1 respectively with the decrease of
the cutoff frequency. With this, the error is ﬁltered with a second-order high-pass. The
introduction of the additional zeros in the noise transfer function only affects the noise
signal of the ﬁlter. The input signal is only affected by the transfer function H (z). If the
feedback coefﬁcients are chosen equal to the coefﬁcients b1 and b2 in the denominator
polynomial, complex zeros are produced that are identical to the complex poles. The noise
transfer function G(z) is then reduced to unity. The choice of complex zeros directly at the
location of the complex poles corresponds to double-precision arithmetic.
In [Abu79] an improvement of noise behavior for the direct form in any desired location
of the z-plane is achieved by placing additional simple-to-implement complex zeros near
152
Equalizers
y(n)
x(n)
_
_
x(n)
z-1
Q
-b1
-b2
a)
z-1
z-1
z-1
z-1
Q
z-1
z-1
-1
2
-b1
-b2
b)
y(n)
σ 2
DF 1
= σ 2
E
σ 2
DF 2
= σ 2
E
2
(1 − b2)(1 + b2 − b1)
6 + 2b1 − 2b2
(1 − b2)(1 + b2 − b1)
Figure 5.48 Direct form with noise shaping.
Direct form
(douple zero at z=1)
Direct form
(single zero at z=1)
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
90
80
70
60
50
Figure 5.49 SNR – Noise shaping in direct-form ﬁlter structures.
the poles. For implementing ﬁlter algorithms with digital signal processors, these kinds of
suboptimal zero are easily realized. Since the Gold and Rader, Kingsbury, and Zölzer ﬁlter
5.2 Recursive Audio Filters
153
structures already have zeros in their respective noise transfer functions, it is sufﬁcient to
use a simple feedback for the quantization error. By virtue of this extension, the block
diagrams in Figs 5.50, 5.51 and 5.52 are obtained.
  (cid:5)(cid:22)
  (cid:5)(cid:22)
(cid:8)(cid:14)(cid:18)(cid:16)(cid:10) (cid:15)
’7(cid:10):
)
  (cid:5)(cid:22)
)
  (cid:5)(cid:22)
"7(cid:10):
(cid:8)(cid:14)(cid:20)(cid:11)(cid:18) (cid:15)
(cid:8)(cid:14)(cid:20)(cid:11)(cid:18) (cid:15)
(cid:5)(cid:8)(cid:14)(cid:18)(cid:16)(cid:10)
(cid:15)
σ 2
ye
= σ 2
E
2 + b1
1 − b2
Figure 5.50 Gold and Rader ﬁlter with noise shaping.
  (cid:5)(cid:22)
)
  (cid:5)(cid:22)
  (cid:5)(cid:22)
)
  (cid:5)(cid:22)
"7(cid:10):
(cid:17)
(cid:22)
  (cid:5)(cid:22)
)
(cid:17)
(cid:23)
’7(cid:10):
(cid:17)
(cid:22)
σ 2
ye
= σ 2
E
(1 + k2
1)((1 + b2)(6 − 2b2) + 2b2
1
+ 8b1) + 2k2
(1 − b2)(1 + b2 − b1)(1 + b2 − b1)
1(1 + b1 + b2)
Figure 5.51 Kingsbury ﬁlter with noise shaping.
The effect of noise shaping on signal-to-noise ratio is shown in Figs 5.53 and 5.54. The
almost ideal noise behavior of all ﬁlter structures for 16-bit quantization and very small
cutoff frequencies can be observed. The effect of this noise shaping for increasing cutoff
frequencies is shown in Fig. 5.54. The compensating effect of the two zeros at z = 1 is
reduced.
154
’7(cid:10):
  (cid:5)(cid:22)
)
  (cid:5)(cid:22)
(cid:22)
Equalizers
  (cid:5)(cid:22)
  (cid:5)(cid:22)
)
  (cid:5)(cid:22)
"7(cid:10):
(cid:22)
(cid:22)
)
(cid:23)
σ 2
ye
= σ 2
E
SNR/dB
100
(1 + z2
1)((1 + b2)(6 − 2b2) + 2b2
1
+ 8b1) + 2z4
(1 − b2)(1 + b2 − b1)(1 + b2 − b1)
1(1 + b1 + b2)
Figure 5.52 Zölzer ﬁlter with noise shaping.
- Direct form (double zero at z=1)
- Gold and Rader
- Kingsbury
- Zölzer
90
80
70
60
50
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
Figure 5.53 SNR – noise shaping (20–200 Hz).
Scaling
In a ﬁxed-point implementation of a digital ﬁlter, a transfer function from the input of the
ﬁlter to a junction within the ﬁlter has to be determined, as well as the transfer function
from the input to the output. By scaling the input signal, it has to be guaranteed that the
signals remain within the number range at each junction and at the output.
In order to calculate scaling coefﬁcients, different criteria can be used. The Lp norm is
deﬁned as
Lp = (cid:11)H (cid:11)p =
(cid:4)
(cid:3)
π
−π
1
2π
|H (ej (cid:4))|p d(cid:4)
(cid:5)
1/p
,
and an expression for the L∞ norm follows for p = ∞:
L∞ = (cid:11)H (ej (cid:4))(cid:11)∞ = max
0≤(cid:4)≤π
|H (ej (cid:4))|.
(5.115)
(5.116)

5.2 Recursive Audio Filters

155
SNR/dB
100
90
80
70
60
50
200
Gold and Rader
Kingsbury
Zölzer
Direct form
(double zero at z=1)
Figure 5.54 SNR – noise shaping (200–12000 Hz).
f/Hz
12k
The L∞ norm represents the maximum of the amplitude frequency response. In general,
the modulus of the output is
with
|y(n)| ≤ (cid:11)H (cid:11)p(cid:11)X(cid:11)q
1
p
+ 1
q
= 1, p, q ≥ 1.
(5.117)
(5.118)
For the L1, L2 and L∞ norms the explanations in Table 5.10 can be used.
Table 5.10 Commonly used scaling.
q
p
1 ∞ Given max. value of input spectrum
scaling w.r.t. the L1 norm of H (ej (cid:4))
Given L1 norm of input spectrum X(ej (cid:4))
scaling w.r.t. the L∞ norm of H (ej (cid:4))
∞ 1
2
2
Given L2 norm of input spectrum X(ej (cid:4))
scaling w.r.t. the L2 norm of H (ej (cid:4))
With
the L∞ norm is given by
|yi(n)| ≤ (cid:11)Hi(ej (cid:4))(cid:11)∞(cid:11)X(ej (cid:4))(cid:11)1,
L∞ = (cid:11)hi (cid:11)∞ =
∞
max
k=0
|hi (k)|.
(5.119)
(5.120)
156
Equalizers
For a sinusoidal input signal of amplitude 1 we get (cid:11)X(ej (cid:4))(cid:11)1 = 1. For |yi(n)| ≤ 1 to be
valid, the scaling factor must be chosen as
Si =
1
(cid:11)Hi(ej (cid:4))(cid:11)∞
.
(5.121)
The scaling of the input signal is carried out with the maximum of the amplitude frequency
response with the goal that for |x(n)| ≤ 1, |yi(n)| ≤ 1. As a scaling coefﬁcient for the input
signal the highest scaling factor Si is chosen. To determine the maximum of the transfer
function
(cid:11)H (ej (cid:4))(cid:11)∞ = max
0≤(cid:4)≤π
|H (ej (cid:4))|
of a second-order system
H (z) = a0 + a1z−1 + a2z−1
1 + b1z−1 + b2z−1
= a0z2 + a1z + a2
z2 + b1z + b2
,
the maximum value can be calculated as
|H (ej (cid:4))|2 =
α0(cid:14) (cid:15)(cid:12) (cid:13)
a0a2
b2
α1
(cid:13)
(cid:15)(cid:12)
(cid:14)
a1(a0 + a2)
cos2((cid:4)) +
2b2
cos2((cid:4)) + b1(1 + b2)
(cid:15)
(cid:12)
2b2
(cid:13)(cid:14)
β1
α2
(cid:13)
(cid:15)(cid:12)
(cid:14)
(a0 − a2)2 + a2
1
4b2
cos((cid:4)) +
cos((cid:4)) +
(1 − b2)2 + b2
1
4b2
(cid:13)(cid:14)
β2
(cid:12)
(cid:15)
(5.122)
= S2.
(5.123)
With x = cos((cid:4)) it follows that
(S2 − α0)x2 + (β1S2 − α1)x + (β2S2 − α2) = 0.
(5.124)
The solution of (5.124) leads to x = cos((cid:4)max / min) which must be real (−1 ≤ x ≤ 1) for
the maximum/minimum to occur at a real frequency. For a single solution (repeated roots)
of the above quadratic equation, the discriminant must be D = (p/2)2 − q = 0 (x2 + px +
q = 0). It follows that
D = (β1S2 − α1)2
4(S2 − α0)2
− β2S2 − α2
S2 − α0
= 0
(5.125)
and
S4(β2
1
− 4β2) + S2(4α2 + 4α0β2 − 2α1β1) + (α2
1
(5.126)
The solution of (5.126) gives two solutions for S2. The solution with the larger value is
chosen. If the discriminant D is not greater than zero, the maximum lies at x = 1 (z = 1)
or x = −1 (z = −1) as given by
− 4α0α2) = 0.
or
S2 = α0 + α1 + α2
1 + β1 + β2
S2 = α0 − α1 + α2
1 − β1 + β2
.
(5.127)
(5.128)
5.3 Nonrecursive Audio Filters
157
Limit Cycles
Limit cycles are periodic processes in a ﬁlter which can be measured as sinusoidal signals.
They arise owing to the quantization of state variables. The different types of limit cycle
and the methods necessary to prevent them are brieﬂy listed below:
• overﬂow limit cycles
→ saturation curve
→ scaling
• limit cycles for vanishing input
→ noise shaping
→ dithering
• limit cycles correlated with the input signal
→ noise shaping
→ dithering.
5.3 Nonrecursive Audio Filters
To implement linear phase audio ﬁlters, nonrecursive ﬁlters are used. The basis of an
efﬁcient implementation is the fast convolution
y(n) = x(n) ∗ h(n) ◦—• Y (k) = X(k) · H (k),
(5.129)
where the convolution in the time domain is performed by transforming the signal and
the impulse response into the frequency domain, multiplying the corresponding Fourier
transforms and inverse Fourier transform of the product into the time domain signal (see
Fig. 5.55). The transform is carried out by a discrete Fourier transform of length N, such
that N = N1 + N2 − 1 is valid and time-domain aliasing is avoided. First we discuss the
basics. We then introduce the convolution of long sequences followed by a ﬁlter design for
linear phase ﬁlters.
N
N2
h(n)
N1
x(n)
0
0
N-FFT
H(k)
N-FFT
X(k)
y(n)=x(n)*h(n)
y(n)=IFFT[X(k).H(k)]
Figure 5.55 Fast convolution of signal x(n) of length N1 and impulse response h(n) of length N2
delivers the convolution result y(n) = x(n) ∗ h(n) of length N1 + N2 − 1.
158
Equalizers
5.3.1 Basics of Fast Convolution
IDFT Implementation with DFT Algorithm. The discrete Fourier transformation (DFT)
is described by
N−1(cid:6)
X(k) =
n=0
−j 2π/N ,
WN = e
x(n)W nk
N
= DFTk[x(n)],
and the inverse discrete Fourier transformation (IDFT) by
x(n) = 1
N
N−1(cid:6)
k=0
X(k)W
−nk
N .
Suppressing the scaling factor 1/N, we write
(cid:14)
x
(n) =
N−1(cid:6)
k=0
X(k)W
−nk
N
= IDFTn[X(k)],
so that the following symmetrical transformation algorithms hold:
(cid:14)
X
(k) = 1√
N
x(n) = 1√
N
N−1(cid:6)
n=0
N−1(cid:6)
k=0
x(n)W nk
N ,
(cid:14)
X
(k)W
−nk
N .
(5.130)
(5.131)
(5.132)
(5.133)
(5.134)
(5.135)
The IDFT differs from the DFT only by the sign in the exponential term.
An alternative approach for calculating the IDFT with the help of a DFT is described
as follows [Cad87, Duh88]. We will make use of the relationships
Conjugating (5.133) gives
x(n) = a(n) + j · b(n),
∗
(n) = b(n) + j · a(n).
j · x
(cid:14)∗
x
(n) =
N−1(cid:6)
k=0
∗
X
(k)W nk
N .
The multiplication of (5.138) by j leads to
(cid:14)∗
j · x
(n) =
N−1(cid:6)
k=0
∗
j · X
(k)W nk
N .
Conjugating and multiplying (5.139) by j results in
(cid:14)
x
(n) = j ·
∗
(j · X
(k)W nk
N
(cid:5)∗
.
(cid:4)
N−1(cid:6)
k=0
(5.136)
(5.137)
(5.138)
(5.139)
(5.140)
An interpretation of (5.137) and (5.140) suggests the following way of performing the IDFT
with the DFT algorithm:
5.3 Nonrecursive Audio Filters
159
1. Exchange the real with the imaginary part of the spectral sequence
Y (k) = YI (k) + j YR(k).
2. Transform with DFT algorithm
DFT[Y (k)] = yI (n) + jyR(n).
3. Exchange the real with the imaginary part of the time sequence
y(n) = yR(n) + jyI (n).
For implementation on a digital signal processor, the use of DFT saves memory for IDFT.
Discrete Fourier Transformation of Two Real Sequences. In many applications, stereo
signals that consist of a left and right channel are processed. With the help of the DFT, both
channels can be transformed simultaneously into the frequency domain [Sor87, Ell82].
For a real sequence x(n) we obtain
X(k) = X
= X
∗
∗
k = 0, 1, . . . , N − 1
(−k),
(N − k).
(5.141)
(5.142)
For a discrete Fourier transformation of two real sequences x(n) and y(n), a complex
sequence is ﬁrst formed according to
The Fourier transformation gives
z(n) = x(n) + jy(n).
DFT[z(n)] = DFT[x(n) + jy(n)]
= ZR(k) + j ZI (k)
= Z(k),
where
Z(k) = ZR(k) + j ZI (k)
= XR(k) + j XI (k) + j [YR(k) + j YI (k)]
= XR(k) − YI (k) + j [XI (k) + YR(k)].
Since x(n) and y(n) are real sequences, it follows from (5.142) that
Z(N − k) = ZR(N − k) + j ZI (N − k) = Z
∗
(k)
= XR(k) − j XI (k) + j [YR(k) − j YI (k)]
= XR(k) + YI (k) − j [XI (k) − YR(k)].
Considering the real part of Z(k), adding (5.148) and (5.151) gives
2XR(k) = ZR(k) + ZR(N − k)
→ XR(k) = 1
2
[ZR(k) + ZR(N − k)],
(5.143)
(5.144)
(5.145)
(5.146)
(5.147)
(5.148)
(5.149)
(5.150)
(5.151)
(5.152)
(5.153)
160
Equalizers
and subtraction of (5.151) from (5.148) results in
2YI (k) = ZR(N − k) − ZR(k)
→ YI (k) = 1
2
[ZR(N − k) − ZR(k)].
Considering the imaginary part of Z(k), adding (5.148) and (5.151) gives
2YR(k) = ZI (k) + ZI (N − k)
→ YR(k) = 1
2
[ZI (k) + ZI (N − k)],
and subtraction of (5.151) from (5.148) results in
2XI (k) = ZI (k) − ZI (N − k)
→ XI (k) = 1
2
[ZI (k) − ZI (N − k)].
Hence, the spectral functions are given by
X(k) = DFT[x(n)] = XR(k) + j XI (k)
[ZR(k) + ZR(N − k)]
= 1
2
+ j
1
2
= 1
2
+ j
[ZI (k) − ZI (N − k)],
Y (k) = DFT[y(n)] = YR(k) + j YR(k)
k = 0, 1, . . . ,
N
2
,
[ZI (k) + ZI (N − k)]
1
2
[ZR(N − k) − ZR(k)],
k = 0, 1, . . . ,
N
2
,
and
XR(k) + j XI (k) = XR(N − k) − j XI (N − k)
YR(k) + j YI (k) = YR(N − k) − j YI (N − k),
(5.154)
(5.155)
(5.156)
(5.157)
(5.158)
(5.159)
(5.160)
(5.161)
(5.162)
(5.163)
(5.164)
k = N
2
+ 1, . . . , N − 1. (5.165)
Fast Convolution if Spectral Functions are Known. The spectral functions X(k), Y (k)
and H (k) are known. With the help of (5.148), the spectral sequence can be formed by
Z(k) = ZR(k) + j ZI (k)
= XR(k) − YI (k) + j [XI (k) + YR(k)],
k = 0, 1, . . . , N − 1.
(5.166)
(5.167)
Filtering is done by multiplication in the frequency domain:
(cid:14)
Z
(k) = [ZR(k) + j ZI (k)][HR(k) + j HI (k)]
= ZR(k)HR(k) − ZI (k)HI (k) + j [ZR(k)HI (k) + ZI (k)HR(k)].
(5.168)
The inverse transformation gives
(cid:14)
z
(n) = [x(n) + jy(n)] ∗ h(n) = x(n) ∗ h(n) + jy(n) ∗ h(n)
(5.169)
(cid:14)
= IDFT[Z
(cid:14)
= z
(k)]
R(n) + jz(cid:14)
I (n),
(5.170)
5.3 Nonrecursive Audio Filters
so that the ﬁltered output sequence is given by
(cid:14)
(cid:14)
x
y
(n) = z
(n) = z
(cid:14)
R(n),
(cid:14)
I (n).
161
(5.171)
(5.172)
The ﬁltering of a stereo signal can hence be done by transformation into the frequency
domain, multiplication of the spectral functions and inverse transformation of left and right
channels.
5.3.2 Fast Convolution of Long Sequences
The fast convolution of two real input sequences xl(n) and xl+1(n) of length N1 with the
impulse response h(n) of length N2 leads to the output sequences
yl(n) = xl(n) ∗ h(n),
yl+1(n) = xl+1(n) ∗ h(n),
(5.173)
(5.174)
of length N1 + N2 − 1. The implementation of a nonrecursive ﬁlter with fast convolution
becomes more efﬁcient than the direct implementation of an FIR ﬁlter for ﬁlter lengths
N > 30. Therefore the following procedure will be performed:
• Formation of a complex sequence
z(n) = xl(n) + jxl+1(n).
(5.175)
• Fourier transformation of the impulse response h(n) that is padded with zeros to a
length N ≥ N1 + N2 − 1,
H (k) = DFT[h(n)]
(FFT-length N).
(5.176)
• Fourier transformation of the sequence z(n) that is padded with zeros to a length
N ≥ N1 + N2 − 1,
Z(k) = DFT[z(n)]
(FFT-length N).
(5.177)
• Formation of a complex output sequence
e(n) = IDFT[Z(k)H (k)]
= z(n) ∗ h(n)
= xl(n) ∗ h(n) + jxl+1(n) ∗ h(n).
• Formation of a real output sequence
yl(n) = Re{e(n)},
yl+1(n) = Im{e(n)}.
(5.178)
(5.179)
(5.180)
(5.181)
(5.182)
162
Equalizers
x (n)
l
h(n)
x
(n)
l+1
x
(n)
l+2
y (n)=x
l
l(n)*h(n)
y
(n)=x
l+1
l+1(n)*h(n)
y
(n)=x
l+2
l+2(n)*h(n)
y(n)
Figure 5.56 Fast convolution with partitioning of the input signal x(n) into blocks of length L.
For the convolution of an inﬁnite-length input sequence (see Fig. 5.56) with an impulse
response h(n), the input sequence is partitioned into sequences xm(n) of length L:
(cid:21)
xm(n) =
x(n),
0,
(m − 1)L ≤ n ≤ mL − 1,
otherwise.
(5.183)
The input sequence is given by superposition of ﬁnite-length sequences according to
x(n) =
∞(cid:6)
m=1
xm(n).
(5.184)
The convolution of the input sequence with the impulse response h(n) of length M gives
M−1(cid:6)
k=0
M−1(cid:6)
y(n) =
=
=
h(k)x(n − k)
∞(cid:6)
m=1
xm(n − k)
(cid:5)
h(k)xm(n − k)
.
h(k)
k=0
∞(cid:6)
(cid:4)
M−1(cid:6)
m=1
k=0
(5.185)
(5.186)
(5.187)
The term in brackets corresponds to the convolution of a ﬁnite-length sequence xm(n)
of length L with the impulse response of length M. The output signal can be given as
superposition of convolution products of length L + M − 1. With these partial convolution
products
h(k)xm(n − k),
(m − 1)L ≤ n ≤ mL + M − 2,


M−1(cid:6)
ym(n) =

k=0
0,
the output signal can be written as
(5.188)
(5.189)
otherwise,
y(n) =
∞(cid:6)
m=1
ym(n).
5.3 Nonrecursive Audio Filters
163
h (n)
1
h (n)
2
h (n)P
Figure 5.57 Partitioning of the impulse response h(n).
If the length M of the impulse response is very long, it can be similarly partitioned into
P parts each of length M/P (see Fig. 5.57). With
(cid:1)
hp
n − (p − 1)
(cid:2)
$
=
h(n),
0,
M
P
M
(p − 1)
P
otherwise,
≤ n ≤ p
M
P
− 1,
it follows that
h(n) =
P(cid:6)
p=1
(cid:1)
hp
n − (p − 1)
(cid:2)
.
M
P
With Mp = pM/P and (5.189) the following partitioning can be done:
(5.190)
(5.191)
(5.192)
y(n) =
∞(cid:6)
(cid:4)
M−1(cid:6)
m=1
k=0
(cid:12)
(cid:5)
h(k)xm(n − k)
(cid:15)
(cid:13)(cid:14)
ym(n)
=
∞(cid:6)
(cid:4)
M1−1(cid:6)
m=1
k=0
h(k)xm(n − k) +
M2−1(cid:6)
k=M1
h(k)xm(n − k) + · · · +
M−1(cid:6)
(cid:5)
h(k)xm(n − k)
.
k=MP −1
(5.193)
This can be rewritten as
y(n) =
(cid:4)
∞(cid:6)
M1−1(cid:6)
m=1
k=0
(cid:12)
h1(k)xm(n − k)
(cid:15)
(cid:13)(cid:14)
ym1
M1−1(cid:6)
+
k=0
(cid:12)
h2(k)xm(n − M1 − k)
(cid:15)
(cid:13)(cid:14)
ym2
M1−1(cid:6)
k=0
(cid:12)
+
M1−1(cid:6)
k=0
(cid:12)
h3(k)xm(n − 2M1 − k)
(cid:15)
(cid:13)(cid:14)
ym3
+ · · · +
(cid:5)
hP (k)xm(n − (P − 1)M1 − k)
(cid:15)
(cid:13)(cid:14)
ymP
=
∞(cid:6)
m=1
[ym1(n) + ym2(n − M1) + · · · + ymP (n − (P − 1)M1)
(cid:12)
(cid:15)
(cid:13)(cid:14)
ym(n)
].
(5.194)
164
Equalizers
An example of partitioning the impulse response into P = 4 parts is graphically shown in
Fig. 5.58. This leads to
y(n) =
(cid:4)
∞(cid:6)
M1−1(cid:6)
m=1
k=0
(cid:12)
h1(k)xm(n − k)
(cid:15)
(cid:13)(cid:14)
ym1
+
M1−1(cid:6)
k=0
(cid:12)
h2(k)xm(n − M1 − k)
(cid:15)
(cid:13)(cid:14)
ym2
M1−1(cid:6)
+
k=0
(cid:12)
h3(k)xm(n − 2M1 − k)
(cid:15)
(cid:13)(cid:14)
ym3
+
M1−1(cid:6)
k=0
(cid:12)
h4(k)xm(n − 3M1 − k)
(cid:15)
(cid:13)(cid:14)
ym4
(cid:5)
=
∞(cid:6)
m=1
]. (5.195)
[ym1(n) + ym2(n − M1) + ym3(n − 2M1) + ym4(n − 3M1)
(cid:12)
(cid:15)
(cid:13)(cid:14)
ym(n)
’(cid:22)
’(cid:23)
’2
’<
’5
’>
’(cid:22)c(cid:21)(cid:22)
’(cid:22)c(cid:21)(cid:23)
’(cid:23)c(cid:21)(cid:22)
’(cid:22)c(cid:21)2
’(cid:23)c(cid:21)(cid:23)
’2c(cid:21)(cid:22)
’(cid:22)c(cid:21)<
’(cid:23)c(cid:21)2
’2c(cid:21)(cid:23)
’<c(cid:21)(cid:22)
’(cid:23)c(cid:21)<
’2c(cid:21)2
’<c(cid:21)(cid:23)
’5c(cid:21)(cid:22)
’2c(cid:21)<
’<c(cid:21)2
’5c(cid:21)(cid:23)
’>c(cid:21)(cid:22)
’<c(cid:21)<
’5c(cid:21)2
’5c(cid:21)<
"(cid:22)
"(cid:23)
"2
"<
"5
">
Figure 5.58 Scheme for a fast convolution with P = 4.
The procedure of a fast convolution by partitioning the input sequence x(n) as well as the
impulse response h(n) is given in the following for the example in Fig. 5.58.
5.3 Nonrecursive Audio Filters
1. Decomposition of the impulse response h(n) of length 4M:
h1(n) = h(n)
0 ≤ n ≤ M − 1,
h2(n − M) = h(n) M ≤ n ≤ 2M − 1,
2M ≤ n ≤ 3M − 1,
h3(n − 2M) = h(n)
3M ≤ n ≤ 4M − 1.
h4(n − 3M) = h(n)
2. Zero-padding of partial impulse responses up to a length 2M:
(cid:21)
h1(n) =
h2(n) =
h3(n) =
h4(n) =
h1(n),
0,
(cid:21)
(cid:21)
(cid:21)
h2(n),
0,
h3(n),
0,
h4(n),
0,
0 ≤ n ≤ M − 1,
M ≤ n ≤ 2M − 1,
0 ≤ n ≤ M − 1,
M ≤ n ≤ 2M − 1,
0 ≤ n ≤ M − 1,
M ≤ n ≤ 2M − 1,
0 ≤ n ≤ M − 1,
M ≤ n ≤ 2M − 1.
165
(5.196)
(5.197)
(5.198)
(5.199)
(5.200)
(5.201)
(5.202)
(5.203)
3. Calculating and storing
Hi(k) = DFT[hi (n)],
i = 1, . . . , 4 (FFT-length 2M).
(5.204)
4. Decomposition of the input sequence x(n) into partial sequences xl(n) of length M:
(l − 1)M ≤ n ≤ lM − 1,
l = 1, . . . , ∞.
xl(n) = x(n),
(5.205)
5. Nesting partial sequences:
zm(n) = xl(n) + jxl+1(n), m = 1, . . . , ∞.
(5.206)
6. Zero-padding of complex sequence zm(n) up to a length 2M:
(cid:21)
zm(n) =
zm(n),
0,
(l − 1)M ≤ n ≤ lM − 1,
lM ≤ n ≤ (l + 1)M − 1.
7. Fourier transformation of the complex sequences zm(n):
Zm(k) = DFT[zm(n)] = ZmR(k) + j ZmI (k)
(FFT-length 2M).
8. Multiplication in the frequency domain:
[ZR(k) + jZI (k)][HR(k) + jHI (k)] = ZR(k)HR(k) − ZI (k)HI (k)
+ j [ZR(k)HI (k) + ZI (k)HR(k)],
Em1(k) = Zm(k)H1(k)
Em2(k) = Zm(k)H2(k)
Em3(k) = Zm(k)H3(k)
Em4(k) = Zm(k)H4(k)
k = 0, 1, . . . , 2M − 1,
k = 0, 1, . . . , 2M − 1,
k = 0, 1, . . . , 2M − 1,
k = 0, 1, . . . , 2M − 1.
(5.207)
(5.208)
(5.209)
(5.210)
(5.211)
(5.212)
(5.213)
166
Equalizers
9. Inverse transformation:
em1(n) = IDFT[Zm(k)H1(k)] n = 0, 1, . . . , 2M − 1,
em2(n) = IDFT[Zm(k)H2(k)] n = 0, 1, . . . , 2M − 1,
em3(n) = IDFT[Zm(k)H3(k)] n = 0, 1, . . . , 2M − 1,
em4(n) = IDFT[Zm(k)H4(k)] n = 0, 1, . . . , 2M − 1.
10. Determination of partial convolutions:
Re{em1(n)} = xl ∗ h1,
Im{em1(n)} = xl+1 ∗ h1,
Re{em2(n)} = xl ∗ h2,
Im{em2(n)} = xl+1 ∗ h2,
Re{em3(n)} = xl ∗ h3,
Im{em3(n)} = xl+1 ∗ h3,
Re{em4(n)} = xl ∗ h4,
Im{em4(n)} = xl+1 ∗ h4.
(5.214)
(5.215)
(5.216)
(5.217)
(5.218)
(5.219)
(5.220)
(5.221)
(5.222)
(5.223)
(5.224)
(5.225)
11. Overlap-add of partial sequences, increment l = l + 2 and m = m + 1, and back to
step 5.
Based on the partitioning of the input signal and the impulse response and the following
Fourier transform, the result of each single convolution is only available after a delay of
one block of samples. Different methods to reduce computational complexity or overcome
the block delay have been proposed [Soo90, Gar95, Ege96, Mül99, Mül01, Garc02]. These
methods make use of a hybrid approach where the ﬁrst part of the impulse response is used
for time-domain convolution and the other parts are used for fast convolution in the fre-
quency domain. Figure 5.59a,b demonstrates a simple derivation of the hybrid convolution
scheme, which can be described by the decomposition of the transfer function as
H (z) =
M−1(cid:6)
i=0
−iN Hi(z),
z
(5.226)
where the impulse response has length M · N and M is the number of smaller partitions
of length N. Figure 5.59c,d shows two different signal ﬂow graphs for the decomposition
given by (5.226) of the entire transfer function. In particular, Fig. 5.59d highlights (with
gray background) that in each branch i = 1, . . . , M − 1 a delay of i · N occurs and each
ﬁlter Hi(z) has the same length and makes use of the same state variables. This means
that they can be computed in parallel in the frequency domain with 2N-FFTs/IFFTs and
the outputs have to be delayed according to (i − 1) · N, as shown in Fig. 5.59e. A further
simpliﬁcation shown in Fig. 5.59f leads to one input 2N-FFT and block delays z−1 for
the frequency vectors. Then, parallel multiplications with Hi(k) of length 2N and the
summation of all intermediate products are performed before one output 2N-IFFT for the
overlap-add operation in the time domain is used. The ﬁrst part of the impulse response
5.3 Nonrecursive Audio Filters
167
represented by H0(z) is performed by direct convolution in the time domain. The frequency
and time domain parts are then overlapped and added. An alternative realization for fast
convolution is based on the overlap and save operation.
a)
x(n)
c)
x(n)
-N
z
-N
z
-N
z
e)
x(n)
h(n)
b)
N
N
N
N
H(z)
y(n)
d)
y(n)
x(n)
n
y(n)
H0(z)
H1(z)
H2(z)
-N
z
-2N
z
-(M-1)N
z
HM-1(z)
f)
y(n)
x(n)
H0(z)
y(n)
FFT
IFFT
-N
z
-(M-2)N
z
-1
z
-1
z
)
k
(
1
H
)
k
(
2
H
)
k
(
1
-
M
H
H0(z)
H1(z)
H2(z)
HM-1(z)
H0(z)
T
F
F
T
F
F
T
F
F
)
k
(
1
H
)
k
(
2
H
)
k
(
1
-
M
H
T
F
F
I
T
F
F
I
T
F
F
I
Figure 5.59 Hybrid fast convolution.
5.3.3 Filter Design by Frequency Sampling
Audio ﬁlter design for nonrecursive ﬁlter realizations by fast convolution can be carried out
by the frequency sampling method. For linear phase systems we obtain
H (ej (cid:4)) = A(ej (cid:4)) e
−j (NF −1/2)(cid:4),
(5.227)
where A(ej (cid:4)) is a real-valued amplitude response and NF is the length of the impulse
response. The magnitude |H (ej (cid:4))| is calculated by sampling in the frequency domain at
equidistant places
f
fS
= k
NF
,
k = 0, 1, . . . , NF − 1,
according to
|H (ej (cid:4))| =A(e j 2πk/NF ),
k = 0, 1, . . . ,
NF
2
− 1.
(5.228)
(5.229)
168
Equalizers
Hence, a ﬁlter can be designed by fulﬁlling conditions in the frequency domain. The linear
phase is determined as
−j NF
−1
2 (cid:4) = e
e
(cid:1)
−j 2π(NF −1/2)k/NF
NF − 1
2
2π
k
NF
= cos
(cid:2)
(cid:1)
− j sin
2π
NF − 1
2
k
NF
(cid:2)
,
(5.230)
(5.231)
k = 0, 1, . . . ,
NF
2
− 1.
Owing to the real transfer function H (z) for an even ﬁlter length, we have to fulﬁll
(cid:2)
(cid:1)
k = NF
2
H
= 0 ∧ H (k) = H
∗
(NF − k),
k = 0, 1, . . . ,
NF
2
− 1.
(5.232)
This has to be taken into consideration while designing ﬁlters of even length NF . The
impulse response h(n) is obtained through an NF -point IDFT of the spectral sequence
H (k). This impulse response is extended with zero-padding to the length N and then
transformed by an N-point DFT resulting in the spectral sequence H (k) of the ﬁlter.
Example: For NF = 8, |H (k)| = 1 (k = 0, 1, 2, . . . , 7) and |H (4)| = 0, the group delay
is tG = 3.5. Figure 5.60 shows the amplitude, real part and imaginary part of the transfer
function and the impulse response h(n).
5.4 Multi-complementary Filter Bank
The subband processing of audio signals is mainly used in source coding applications for
efﬁcient transmission and storing. The basis for the subband decomposition is critically
sampled ﬁlter banks [Vai93, Fli00]. These ﬁlter banks allow a perfect reconstruction of
the input provided there is no processing within the subbands. They consist of an analysis
ﬁlter bank for decomposing the signal in critically sampled subbands and a synthesis ﬁlter
bank for reconstructing the broad-band output. The aliasing in the subbands is eliminated
by the synthesis ﬁlter bank. Nonlinear methods are used for coding the subband signals.
The reconstruction error of the ﬁlter bank is negligible compared with the errors due to the
coding/decoding process. Using a critically sampled ﬁlter bank as a multi-band equalizer,
multi-band dynamic range control or multi-band room simulation, the processing in the
subbands leads to aliasing at the output. In order to avoid aliasing, a multi-complementary
ﬁlter bank [Fli92, Zöl92, Fli00] is presented which enables an aliasing-free processing in
the subbands and leads to a perfect reconstruction of the output. It allows a decomposition
into octave frequency bands which are matched to the human ear.
5.4.1 Principles
Figure 5.61 shows an octave-band ﬁlter bank with critical sampling. It performs a suc-
cessive low-pass/high-pass decomposition into half-bands followed by downsampling by a
factor 2. The decomposition leads to the subbands Y1 to YN (see Fig. 5.62). The transition
5.4 Multi-complementary Filter Bank
169
Magnitude |H(k)|
Re{H(k)}
1.2
1
0.8
0.6
0.4
0.2
0
1
0.5
0
(cid:358)0.5
(cid:358)1
(cid:358)0.2
0
2
4
k (cid:111)
6
8
0
2
Impulse response h(n)
6
8
4
k (cid:111)
Im{H(k)}
0.5
0.4
0.3
0.2
0.1
0
1
0.5
0
(cid:358)0.5
(cid:358)1
0
2
4
n (cid:111)
6
8
0
2
4
k (cid:111)
6
8
Figure 5.60 Filter design by frequency sampling (NF even).
frequencies of this decomposition are given by
(cid:4)Ck = π
2
−k+1,
2
k = 1, 2, . . . , N − 1.
(5.233)
In order to avoid aliasing in subbands, a modiﬁed octave-band ﬁlter bank is considered
which is shown in Fig. 5.63 for a two-band decomposition. The cutoff frequency of the
modiﬁed ﬁlter bank is moved from π
2 to a lower frequency. This means that in downsam-
pling the low-pass branch, no aliasing occurs in the transition band (e.g. cutoff frequency
π
3 ). The broader high-pass branch cannot be downsampled. A continuation of the two-band
decomposition described leads to the modiﬁed octave-band ﬁlter bank shown in Fig. 5.64.
The frequency bands are depicted in Fig. 5.65 showing that besides the cutoff frequencies
(cid:4)Ck = π
3
−k+1,
2
k = 1, 2, . . . , N − 1,
(5.234)
the bandwidth of the subbands is reduced by a factor 2. The high-pass subband Y1 is an
exception.
The special low-pass/high-pass decomposition is carried out by a two-band comple-
mentary ﬁlter bank as shown in Fig. 5.66. The frequency responses of a decimation ﬁlter
HD(z), interpolation ﬁlter HI (z) and kernel ﬁlter HK (z) are shown in Fig. 5.67.
170
Equalizers
’7(cid:10):
(cid:15)(cid:1)
(cid:23)
(cid:15)(cid:1)
(cid:23)
(cid:15)(cid:1)
(cid:23)
=(cid:1)
(cid:23)
(cid:6)(cid:1)
(cid:23)
=(cid:1)
=(cid:1)
(cid:23)
(cid:6)(cid:1)
(cid:23)
=(cid:1)
=(cid:1)
(cid:23)
(cid:6)(cid:1)
(cid:23)
=(cid:1)
(cid:6)(cid:1)
*
(cid:22)
*
(cid:23)
*
(cid:24)(cid:5)(cid:22)
*
(cid:24)
"7(cid:10):
(cid:15)(cid:1)
(cid:23)
(cid:15)(cid:1)
(cid:23)
(cid:15)(cid:1)
(cid:23)
Figure 5.61 Octave-band QMF ﬁlter bank (SP = signal processing, LP = low-pass, HP = high-pass).
=7(cid:2) ]
:(cid:20)
*
(cid:24)
*
2
*
(cid:23)
*
(cid:22)
(cid:20)
$(cid:24)(cid:5)(cid:22)
(cid:2)(cid:16)(cid:17)
(cid:2)(cid:16)(cid:18)
(cid:2)(cid:16)(cid:19)
(cid:20)
(cid:2)
Figure 5.62 Octave-frequency bands.
=7(cid:2) ]
:(cid:20)
/$@
)/@
(cid:2)(cid:16)(cid:21)
(cid:2)(cid:16)(cid:19)
(cid:19)(cid:2)(cid:16)(cid:21)
(cid:20)
(cid:2)
Figure 5.63 Two-band decomposition.
’7(cid:10):
(cid:15)(cid:1)
(cid:23)
(cid:15)(cid:1)
(cid:23)
(cid:15)(cid:1)
(cid:23)
=(cid:1)
=(cid:1)
(cid:6)(cid:1)
*
(cid:22)
(cid:6)(cid:1)
*
(cid:23)
=(cid:1)
(cid:6)(cid:1)
*
(cid:24)(cid:5)(cid:22)
(cid:6)(cid:1)
*
(cid:24)
"7(cid:10):
(cid:15)(cid:1)
(cid:23)
(cid:15)(cid:1)
(cid:23)
(cid:15)(cid:1)
(cid:23)
Figure 5.64 Modiﬁed octave-band ﬁlter bank.
The low-pass ﬁltering of a signal x1(n) is done with the help of a decimation ﬁlter
HD(z), the downsampler of factor 2 and the kernel ﬁlter HK (z) and leads to y2(2n). The
Z-transform of y2(2n) is given by
Y2(z) = 1
2
[HD(z
1
2 )X1(z
1
2 )HK (z) + HD(−z
1
2 )X1(−z
1
2 )HK (z)].
(5.235)
5.4 Multi-complementary Filter Bank
171
=7(cid:2) ]
:(cid:20)
*
(cid:24)
*
2
*
(cid:23)
*
(cid:22)
(cid:20)
$(cid:24)(cid:5)(cid:22)
(cid:2)(cid:16)(cid:22)(cid:19)
(cid:2)(cid:16)(cid:23)
(cid:2)(cid:16)(cid:21)
(cid:2)(cid:16)(cid:19)
(cid:2)
(cid:20)
Figure 5.65 Modiﬁed octave decomposition.
(cid:14)7(cid:10):
’
(cid:14)
(cid:22)
= (cid:26)
(cid:23)
= N
(cid:26)
(cid:14)
"
(cid:22)(cid:15)
(cid:14)(cid:14)
(cid:14)(cid:14)(cid:14)(cid:14)(cid:14)"
(cid:22)
(cid:14)
(cid:6)(cid:1)
= (cid:27)
(cid:23)
= (cid:27)
(cid:23)
g
’
(cid:14)7(cid:10):
(cid:14)
(cid:22)
(cid:14)(cid:14)(cid:14)7(cid:23)(cid:10)
"
(cid:23)
:
(cid:6)(cid:1)
Figure 5.66 Two-band complementary ﬁlter bank.
=7(cid:2) ]
:(cid:20)
7 :=(cid:27)
=
(cid:26)
7 :
(cid:23)
N=(cid:14)(cid:14) 7  :
(cid:2)(cid:16)(cid:21)
(cid:2)(cid:16)(cid:19)
(cid:19)(cid:2)(cid:16)(cid:21)
(cid:20)
(cid:2)
Figure 5.67 Design of HD(z), HI (z) and HK (z).
The interpolated low-pass signal y1L(n) is generated by upsampling by a factor 2 and
ﬁltering with the interpolation ﬁlter HI (z). TheZ -transform of y1L(n) is given by
Y1L(z) = Y2(z2)HI (z)
2 HD(z)HI (z)HK (z2)
= 1
(cid:13)(cid:14)
(cid:15)
(cid:12)
G1(z)
2 HD(−z)HI (z)HK (z2)
X1(z) + 1
(cid:13)(cid:14)
(cid:15)
(cid:12)
G2(z)
X1(−z).
(5.236)
(5.237)
The high-pass signal y1(n) is obtained by subtracting the interpolated low-pass signal
y1L(n) from the delayed input signal x1(n − D). The Z-transform of the high-pass signal
is given by
Y1(z) = z
= [z
−DX1(z) − Y1L(z)
−D − G1(z)]X1(z) − G2(z)X1(−z).
(5.239)
The low-pass and high-pass signals are processed individually. The output signal ˆx1(n)
is formed by adding the high-pass signal to the upsampled and ﬁltered low-pass signal.
With (5.237) and (5.239) the Z-transform of ˆx1(n) can be written as
(5.238)
ˆX1(z) = Y1L(z) + Y1(z) = z
−DX1(z).
(5.240)
Equation (5.240) shows the perfect reconstruction of the input signal which is delayed by
D sampling units.
172
Equalizers
The extension to N subbands and performing the kernel ﬁlter using complementary
techniques [Ram88, Ram90] leads to the multi-complementary ﬁlter bank as shown in
Fig. 5.68. Delays are integrated in the high-pass (Y1) and band-pass subbands (Y2 to YN−2)
in order to compensate the group delay. The ﬁlter structure consists of N horizontal stages.
The kernel ﬁlter is implemented as a complementary ﬁlter in S vertical stages. The design
of the latter will be discussed later. The vertical delays in the extended kernel ﬁlters (EKF1
to EKFN−1) compensate group delays caused by forming the complementary component.
At the end of each of these vertical stages is the kernel ﬁlter HK . With
zk = z2−(k−1)
and k = 1, . . . , N,
(5.241)
the signals ˆXk(zk) can be written as a function of the signals Xk(zk) as
−D1
ˆX = diag [z
1
−D2
z
2
. . .
−DN
z
N
]X,
(5.242)
with
ˆX = [ ˆX1(z1)
ˆX2(z2)
X = [X1(z1) X2(z2)
ˆXN (zN )]T ,
. . .
. . . XN (zN )]T ,
and with k = N − l the delays are given by
Dk=N = 0,
Dk=N−l = 2DN−l+1 + D,
l = 1, . . . , N − 1.
(5.243)
(5.244)
Perfect reconstruction of the input signal can be achieved if the horizontal delays DH k are
given by
DHk=N
DHk=N−1
DHk=N−l
= 0,
= 0,
= 2DN−l+1,
l = 2, . . . , N − 1.
The implementation of the extended vertical kernel ﬁlters is done by calculating com-
plementary components as shown in Fig. 5.69. After upsampling, interpolating with a
high-pass HP (Fig. 5.69b) and forming the complementary component, the kernel ﬁlter
HK with frequency response as in Fig. 5.69a becomes low-pass with frequency response
as illustrated in Fig. 5.69c. The slope of the ﬁlter characteristic remains constant whereas
the cutoff frequency is doubled. A subsequent upsampling with an interpolation high-pass
(Fig. 5.69d) and complement ﬁltering leads to the frequency response in Fig. 5.69e. With
the help of this technique, the kernel ﬁlter is implemented at a reduced sampling rate. The
cutoff frequency is moved to a desired cutoff frequency by using decimation/interpolation
stages with complement ﬁltering.
Computational Complexity. For an N-band multi-complementary ﬁlter bank with N −
1 decomposition ﬁlters where each is implemented by a kernel ﬁlter with S stages, the
horizontal complexity is given by
HC = HC1 + HC2
(cid:1)
1
2
+ 1
4
+ · · · + 1
2N
(cid:2)
.
(5.245)
5.4 Multi-complementary Filter Bank
173
(cid:30)N@(cid:22)
N=
(cid:30)N@(cid:23)
N=
L(cid:6)(cid:14)(cid:6)
(cid:23)
=(cid:1)
(cid:23)
=(cid:1)
(cid:23)
=(cid:1)
(cid:23)
=(cid:1)
L(cid:6)(cid:14)(cid:23)
(cid:5)
(cid:26)
(cid:14)L(cid:23)
(cid:14)
(cid:14)
(cid:14)
(cid:14)
(cid:14)
(cid:14)
(cid:14)
(cid:14)
(cid:14)
(cid:23)
=(cid:1)
(cid:23)
=(cid:1)
(cid:5)
L(cid:6)(cid:14)(cid:22)
(cid:26)
(cid:14)L(cid:22)
(cid:14)
(cid:14)
(cid:14)
(cid:14)
(cid:14)
(cid:14)
(cid:14)
(cid:14)
(cid:14)
(cid:23)
=(cid:1)
(cid:5)
(cid:26)
(cid:14)L(cid:23)
(cid:14)
(cid:14)
(cid:14)
(cid:14)
(cid:14)
(cid:14)
(cid:14)
(cid:14)
(cid:14)
(cid:23)
=(cid:1)
(cid:5)
(cid:26)
(cid:14)L(cid:22)
(cid:14)
(cid:14)
(cid:14)
(cid:14)
(cid:14)
(cid:14)
(cid:14)
(cid:14)
(cid:14)
(cid:14)
(cid:14)
(cid:14)
(cid:14)
(cid:14)
(cid:14)
(cid:14)
(cid:14)
(cid:14)
(cid:14)
(cid:30)N@(cid:24)(cid:5)(cid:22)
N=
(cid:23)
=(cid:1)
(cid:23)
=(cid:1)
(cid:23)
=(cid:1)
(cid:5)
(cid:26)
(cid:14)L(cid:23)
(cid:14)
(cid:14)
(cid:14)
(cid:14)
(cid:14)
(cid:14)
(cid:14)
(cid:14)
(cid:14)
(cid:23)
=(cid:1)
(cid:5)
(cid:26)
(cid:14)L(cid:22)
(cid:14)
(cid:14)
(cid:14)
(cid:14)
(cid:14)
(cid:14)
(cid:14)
(cid:14)
(cid:14)
(cid:18)
(cid:2)
(cid:3)
(cid:9)
(cid:6)
(cid:7)
(cid:14)
(cid:4)
(cid:9)
(cid:20)
(cid:16)
(cid:7)
(cid:8)
(cid:2)
L
:(cid:22)(cid:22)(cid:14)7 ’
:(cid:23)(cid:23)(cid:14)7 ’
(cid:15)(cid:1)
(cid:23) =,
(cid:15)(cid:1)
(cid:23) =,
’
(cid:24)(cid:5)(cid:22)(cid:14)7
:(cid:24)(cid:5)(cid:22)
:(cid:24)(cid:24)(cid:14)7 ’
(cid:15)(cid:1)
(cid:23) =,
(cid:26)
(cid:14)
(cid:26)
(cid:14)
(cid:26)
(cid:14)
(cid:15)(cid:1)
(cid:23)
(cid:15)(cid:1)
(cid:23)
(cid:15)(cid:1)
(cid:23)
(cid:14)(cid:14)(cid:14)(cid:14)(cid:14)(cid:14)(cid:14)
(cid:26)
=(cid:22)
:(cid:22)(cid:22)(cid:14)7 "
(cid:14)
(cid:6)(cid:1)
g
:(cid:22)(cid:22)(cid:14)7 ’
(cid:15)(cid:1)
(cid:23)
=(cid:6)(cid:14)(cid:22)
(cid:14)(cid:14)(cid:14)(cid:14)(cid:14)(cid:14)(cid:14)
(cid:26)
=(cid:23)
:(cid:23)(cid:23)(cid:14)7 "
(cid:14)
(cid:6)(cid:1)
:(cid:23)(cid:23)(cid:14)7 ’g
=(cid:6)(cid:14)(cid:23)
(cid:14)(cid:14)(cid:14)(cid:14)(cid:14)(cid:14)(cid:14)
"
(cid:24)(cid:5)(cid:22)(cid:14)7
:(cid:24)(cid:5)(cid:22)
(cid:14)
:(cid:24)(cid:24)(cid:14)7 "
(cid:6)(cid:1)
(cid:6)(cid:1)
(cid:15)(cid:1)
(cid:23)
’g
:(cid:24)(cid:5)(cid:22)
(cid:24)(cid:5)(cid:22)(cid:14)7
=(cid:6)(cid:14)(cid:24)(cid:5)(cid:22)
(cid:15)(cid:1)
(cid:23)
g
:(cid:24)(cid:24)(cid:14)7 ’
(cid:14)
=(cid:6)(cid:14)(cid:24)
=(cid:11)(cid:8)(cid:16) (cid:11)(cid:10)(cid:7)(cid:9)(cid:4)(cid:14)(cid:6)(cid:7)(cid:9)(cid:3)(cid:2)(cid:18)
Figure 5.68 Multi-complementary ﬁlter bank.
HC1 denotes the number of operations that are carried out at the input sampling rate. These
operations occur in the horizontal stage HS1 (see Fig. 5.68). HC2 denotes the number
of operations (horizontal stage HS2) that are performed at half of the sampling rate. The
number of operations in the stages from HS2 to HSN are approximately identical but are
calculated at sampling rates that are successively halved.
The complexities VC1 to VCN−1 of the vertical kernel ﬁlters EKF1 to EKFN−1 are
calculated as
VC1 = 1
2
VC2 = 1
4
V1 + V2
V1 + V2
(cid:1)
(cid:1)
1
4
1
8
(cid:2)
+ 1
8
+ 1
16
+ · · · + 1
2S+1
+ · · · + 1
2S+2
,
(cid:2)
= 1
2
VC1,
174
Equalizers
N=
=7(cid:2) ]
:(cid:20)
(cid:23)
=(cid:1)
(cid:23)
=(cid:1)
=7(cid:2) ]
:(cid:20)
(cid:26)
(cid:14)
(cid:14)
(cid:14)
(cid:14)
(cid:14)
(cid:14)
(cid:14)
(cid:14)
(cid:14)
(cid:14)
L(cid:23)
(cid:5)
=7(cid:2) ]
:(cid:20)
(cid:23)
=(cid:1)
(cid:23)
=(cid:1)
=7(cid:2) ]
:(cid:20)
(cid:26)
(cid:14)
(cid:14)
(cid:14)
(cid:14)
(cid:14)
(cid:14)
(cid:14)
(cid:14)
(cid:14)
(cid:14)
L(cid:22)
(cid:5)
=7(cid:2) ]
:(cid:20)
(cid:20)
KK
(cid:19)(cid:2)(cid:16)(cid:21) (cid:2)
(cid:26)=(cid:1)1(cid:27)=(cid:1)
(cid:19)(cid:2)(cid:16)(cid:21)
(cid:2)
(cid:19)(cid:2)(cid:16)(cid:21)
(cid:2)
(cid:20)
K
(cid:20)
K
(cid:9):
.:
(cid:20):
(cid:26)=(cid:1)1(cid:27)=(cid:1)
(cid:19)(cid:2)(cid:16)(cid:21)
(cid:19)(cid:2)(cid:16)(cid:21)
-:
(cid:20)
(cid:2):
(cid:20)
(cid:2)
(cid:2)
Figure 5.69 Multirate complementary ﬁlter.
(cid:1)
VC3 = 1
8
V1 + V2
1
16
+ 1
32
+ · · · + 1
2S+3
(cid:2)
= 1
4
VC1,
VCN−1 = 1
2N−1 V1 + V2
(cid:1)
1
2N
+ · · · +
(cid:2)
1
2S+N−1
= 1
2N−1
VC1,
where V1 depicts the complexity of the ﬁrst stage VS1 and V2 is the complexity of the
second stage VS2 (see Fig. 5.68). It can be seen that the total vertical complexity is given
by
VC = VC1
(cid:1)
1 + 1
2
+ 1
4
+ · · · + 1
2N−1
(cid:2)
.
(5.246)
The upper bound of the total complexity results is the sum of horizontal and vertical
complexities and can be written as
Ctot = HC1 + HC2 + 2VC1.
(5.247)
The total complexity Ctot is independent of the number of frequency bands N and vertical
stages S. This means that for real-time implementation with ﬁnite computation power, any
desired number of subbands with arbitrarily narrow transition bands can be implemented!
5.4 Multi-complementary Filter Bank
175
5.4.2 Example: Eight-band Multi-complementary Filter Bank
In order to implement the frequency decomposition into the eight bands shown in Fig. 5.70,
the multirate ﬁlter structure of Fig. 5.71 is employed. The individual parts of the system
provide means of downsampling (D = decimation), upsampling (I = interpolation), kernel
ﬁltering (K), signal processing (SP), delays (N1 = Delay 1, N2 = Delay 2) and group delay
compensation Mi in the ith band. The frequency decomposition is carried out successively
from the highest to the lowest frequency band. In the two lowest frequency bands, a com-
pensation for group delay is not required. The slope of the ﬁlter response can be adjusted
with the kernel complementary ﬁlter structure shown in Fig. 5.72 which consists of one
stage. The speciﬁcations of an eight-band equalizer are listed in Table 5.11. The stop-band
attenuation of the subband ﬁlters is chosen to be 100 dB.
i
:
%
7
=
i
*?
*2
*(cid:23)
(cid:22)*
%$4
%$2
%$(cid:23)
%$(cid:22)
%(cid:14)(cid:14)1<(cid:6)
%(cid:14)(cid:14)1(cid:23)(cid:6)
%
Figure 5.70 Modiﬁed octave decomposition of the frequency band.
Table 5.11 Transition frequencies fCi and transition bandwidths TB in an eight-band equalizer.
fS [kHz]
fC1 [Hz]
fC2 [Hz]
fC3 [Hz]
fC4 [Hz]
fC5 [Hz]
44.1
TB [Hz]
7350
1280
3675
640
1837.5
320
918.75
160
459.375
80
fC6 [Hz]
≈230
40
fC7 [Hz]
≈115
20
Filter Design
The design of different decimation and interpolation ﬁlters is mainly determined by the
transition bandwidth and the stop-band attenuation for the lower frequency band. As an
example, a design is made for an eight-band equalizer. The kernel complementary ﬁlter
structure for both lower frequency bands is illustrated in Fig. 5.72. The design speciﬁcations
for the kernel low-pass, decimation and interpolation ﬁlters are presented in Fig. 5.73.
Kernel Filter Design. The transition bandwidth of the kernel ﬁlter is known if the transition
bandwidth is given for the lower frequency band. This kernel ﬁlter must be designed for
a sampling rate of f (cid:14)(cid:14)
= 44100/(28). For a given transition bandwidth fT B at a frequency
S
f (cid:14)(cid:14) = f (cid:14)(cid:14)
S /3, the normalized pass-band frequency is
(cid:4)(cid:14)(cid:14)
Pb
2π
= f (cid:14)(cid:14) − fTB/2
f (cid:14)(cid:14)
S
and the normalized stop-band frequency
(cid:4)(cid:14)(cid:14)
Sb
2π
= f (cid:14)(cid:14) + fTB/2
f (cid:14)(cid:14)
S
.
(5.248)
(5.249)
176
Equalizers
’7(cid:10):
"7(cid:10):
<<(cid:29)(cid:22)(cid:14)(cid:17)=
N
(cid:26)
(cid:27)
(cid:23)(cid:23)(cid:29)35(cid:14)(cid:17)=
N
(cid:26)
(cid:27)
(cid:22)(cid:22)(cid:29)3(cid:23)5(cid:14)(cid:17)=
N
(cid:26)
(cid:27)
55(cid:22)(cid:23)(cid:29)5(cid:14)=
N
(cid:26)
(cid:27)
(cid:23)45>(cid:29)(cid:23)5(cid:14)=
N
(cid:26)
(cid:27)
(cid:22)24?(cid:29)(cid:22)(cid:23)(cid:14)=
N
(cid:26)
(cid:27)
>?I(cid:29)3>(cid:14)=
N
(cid:26)
(cid:27)
(cid:26)
(cid:24)(cid:22)
(cid:26)
(cid:24)(cid:22)
(cid:26)
(cid:24)(cid:22)
(cid:26)
(cid:24)(cid:22)
(cid:26)
(cid:24)(cid:22)
(cid:26)
(cid:24)(cid:22)
(cid:26)
(cid:24)(cid:22)
3(cid:5)(cid:23)(cid:23)353(cid:14)=
(cid:24)(cid:23)
4253(cid:5)(cid:23)(cid:23)353(cid:14)=
(cid:6)(cid:1)
/?
(cid:27)
3(cid:5)4253(cid:14)=
(cid:24)(cid:23)
2>45(cid:5)4253(cid:14)=
(cid:6)(cid:1)
/4
(cid:27)
3(cid:5)2>45(cid:14)=
(cid:24)(cid:23)
(cid:22)?24(cid:5)2>45(cid:14)=
(cid:6)(cid:1)
/>
(cid:27)
3(cid:5)(cid:22)?24(cid:14)=
I(cid:22)?(cid:5)(cid:22)?24(cid:14)=
(cid:24)(cid:23)
(cid:6)(cid:1)
/5
(cid:27)
3(cid:5)I(cid:22)?(cid:14)=
(cid:24)(cid:23)
<5I(cid:5)I(cid:22)?(cid:14)=
(cid:6)(cid:1)
/<
(cid:27)
3(cid:5)<52(cid:14)=
(cid:24)(cid:23)
(cid:23)23(cid:5)<5I(cid:14)=
(cid:6)(cid:1)
/2
(cid:27)
3(cid:5)(cid:23)23(cid:14)=
(cid:24)(cid:23)
(cid:27)
(cid:22)(cid:22)5(cid:5)(cid:23)23(cid:14)=
(cid:6)(cid:1)
3(cid:5)(cid:22)(cid:22)5(cid:14)=
(cid:6)(cid:1)
2<<(cid:29)52(cid:14)=
(cid:27)
(cid:27)
(cid:27)
(cid:27)
(cid:27)
(cid:27)
(cid:27)
Figure 5.71 Linear phase eight-band equalizer.
5.4 Multi-complementary Filter Bank
177
’7(cid:10):
(cid:26)(cid:15)(cid:1)
(cid:23)
"7(cid:10):
 (cid:5)(cid:24)(cid:23)
(cid:6)(cid:1)
= 7 :<
= 7 :2
= 7 :5
(cid:27)(cid:15)(cid:1)
(cid:23)
(cid:27)(cid:15)(cid:1)
(cid:23)
(cid:23)
(cid:26)=(cid:1)
= 7 :
3
= 7 :3
= 7 :(cid:22)
 (cid:5)(cid:24) (cid:22)
= 7 :(cid:23)
(cid:23)
(cid:27)=(cid:1)
(cid:6)(cid:1)
Figure 5.72 Kernel complementary ﬁlter structure.
With the help of these parameters the ﬁlter can be designed. Making use of the Parks–
McClellan program, the frequency response shown in Fig. 5.74 is obtained for a transi-
tion bandwidth of fTB = 20 Hz. The necessary ﬁlter length for a stop-band attenuation of
100 dB is 53 taps.
Decimation and Interpolation High-pass Filter. These ﬁlters are designed for a sampling
rate of f (cid:14)
= 44100/(27) and are half-band ﬁlters as illustrated in Fig. 5.73. First a low-pass
S
ﬁlter is designed, followed by a high-pass to low-pass transformation. For a given transition
bandwidth fT B , the normalized pass-band frequency is
= f (cid:14)(cid:14) + fTB/2
f (cid:14)
S
(cid:4)(cid:14)
Pb
2π
(5.250)
and the normalized stop-band frequency is given by
(cid:4)(cid:14)
Sb
2π
= 2f (cid:14)(cid:14) − fTB/2
f (cid:14)
S
.
(5.251)
With these parameters the design of a half-band ﬁlter is carried out. Figure 5.75 shows the
frequency response. The necessary ﬁlter length for a stop-band attenuation of 100 dB is
55 taps.
Decimation and Interpolation Low-pass Filter. These ﬁlters are designed for a sampling
rate of fS = 44100/(26) and are also half-band ﬁlters. For a given transition bandwidth
fTB, the normalized pass-band frequency is
(cid:4)Pb
2π
= 2f (cid:14)(cid:14) + fTB/2
fS
and the normalized stop-band frequency is given by
(cid:4)Sb
2π
= 4f (cid:14)(cid:14) − fTB/2
fS
.
(5.252)
(5.253)
178
N(cid:2)(cid:8)(cid:10)(cid:2)(cid:4)(cid:14)@(cid:16)(cid:4)(cid:7)(cid:2)(cid:8)
i= 7 :i
3
Equalizers
(cid:26)(cid:2)(cid:20)(cid:16)(cid:12)(cid:9)(cid:7)(cid:16)(cid:11)(cid:10)(cid:14)(cid:9)(cid:10)-
(cid:27)(cid:10)(cid:7)(cid:2)(cid:8)(cid:28)(cid:11)(cid:4)(cid:9)(cid:7)(cid:16)(cid:11)(cid:10)
$(cid:11)(cid:12)(cid:28)(cid:4)(cid:2)(cid:12)(cid:2)(cid:10)(cid:7)
(cid:26)(cid:2)(cid:20)(cid:16)(cid:12)(cid:9)(cid:7)(cid:16)(cid:11)(cid:10)(cid:14)(cid:9)(cid:10)-
(cid:27)(cid:10)(cid:7)(cid:2)(cid:8)(cid:28)(cid:11)(cid:4)(cid:9)(cid:7)(cid:16)(cid:11)(cid:10)
$(cid:11)(cid:12)(cid:28)(cid:4)(cid:2)(cid:12)(cid:2)(cid:10)(cid:7)
(cid:19)(cid:2)(cid:16)(cid:21)
(cid:2)
(cid:18)(cid:2)(cid:16)(cid:21)
(cid:19)(cid:2)
(cid:21)(cid:2)
(cid:18)(cid:2)
KK
(cid:20)
(cid:26)=(cid:1)8(cid:27)=(cid:1)
i= 7 :i
(cid:22)
(cid:2)(cid:16)(cid:19)
(cid:19)(cid:2)(cid:16)(cid:21)
(cid:2)
(cid:21)(cid:2)(cid:16)(cid:19)
(cid:19)(cid:2)
K
(cid:20)
(cid:2)(cid:16)(cid:19)
(cid:19)(cid:2)(cid:16)(cid:21)
(cid:2)
(cid:21)(cid:2)(cid:16)(cid:19)
(cid:19)(cid:2)
K(cid:20)
(cid:26)(cid:15)(cid:1)8(cid:27)(cid:15)(cid:1)
(cid:2)(cid:16)(cid:18)
(cid:2)(cid:16)(cid:21)
(cid:2)(cid:16)(cid:19)
(cid:21)(cid:2)(cid:16)(cid:18)
(cid:2)
(cid:20)
i= 7 :i
(cid:23)
i= 7 :i
2
i= 7 :i
5
(cid:2)(cid:16)(cid:18)
(cid:2)(cid:16)(cid:21)
(cid:2)(cid:16)(cid:19)
(cid:21)(cid:2)(cid:16)(cid:18)
(cid:2)
(cid:20)
i= 7 :i
<
(cid:2)(cid:16)(cid:18)
(cid:2)(cid:16)(cid:21)
(cid:2)(cid:16)(cid:19)
(cid:21)(cid:2)(cid:16)(cid:18)
(cid:2)
(cid:20)
Figure 5.73 Decimation and interpolation ﬁlters.
With these parameters the design of a half-band ﬁlter is carried out. Figure 5.76 shows the
frequency response. The necessary ﬁlter length for a stop-band attenuation of 100 dB is
43 taps. These ﬁlter designs are used in every decomposition stage so that the transition
frequencies and bandwidths are obtained as listed in Table 5.11.
Memory Requirements and Latency Time. The memory requirements depend directly
on the transition bandwidth and the stop-band attenuation. Here, the memory operations
for the actual kernel, decimation and interpolation ﬁlters have to be differentiated from the
group delay compensations in the frequency bands. The compensating group delay N1 for
decimation and interpolation high-pass ﬁlters of order ODHP/IHP is calculated with the help
of the kernel ﬁlter order OKF according to
N1 = OKF + ODHP/IHP.
(5.254)
5.4 Multi-complementary Filter Bank
179
Kernel Filter  N=53
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
0
-20
-40
-60
-80
-100
-120
0
10
20
30
40
50
60
70
80
f/Hz (cid:111)
Figure 5.74 Kernel low-pass ﬁlter with a transition bandwidth of 20 Hz.
Interpolation/Decimation Filter 1    N=55
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
0
-20
-40
-60
-80
-100
-120
0
20
40
60
80
100
120
140
160
f/Hz (cid:111)
Figure 5.75 Decimation and interpolation high-pass ﬁlter.
The group delay compensation N2 for the decimation and interpolation low-pass ﬁlters of
order ODLP/ILP is given by
N2 = 2N1 + ODLP/ILP.
(5.255)
180
Equalizers
Interpolation/Decimation Filter 2    N=43
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
0
-20
-40
-60
-80
-100
-120
0
50
100
150
200
250
300
f/Hz (cid:111)
Figure 5.76 Decimation and interpolation low-pass ﬁlter.
The delays M3, . . . , M8 in the individual frequency bands are calculated recursively start-
ing from the two lowest frequency bands:
M3 = 2N2, M4 = 6N2, M5 = 14N2,
M6 = 30N2, M7 = 62N2, M8 = 126N2.
The memory requirements per decomposition stage are listed in Table 5.12. The memory
i Mi = 240N2. The latency time (delay) is given by
for the delays can be computed by
tD = (M8/44100)103 ms (tD = 725 ms).
(cid:11)
Table 5.12 Memory requirements.
Kernel ﬁlter
DHP/IHP
DLP/ILP
N1
N2
OKF
2 · ODHP/IHP
3 · ODLP/ILP
OKF + ODHP/IHP
2 · N1 + ODLP/ILP
5.5 Java Applet – Audio Filters
The applet shown in Fig. 5.77 demonstrates audio ﬁlters. It is designed for a ﬁrst insight into
the perceptual effect of ﬁltering an audio signal. Besides the different ﬁlter types and their
acoustical effect, the applet offers a ﬁrst insight into the logarithmic behavior of loudness
and frequency resolution of our human acoustical perception.
5.6 Exercises
181
The following ﬁlter functions can be selected on the lower right of the graphical user
interface:
• Low-/high-pass ﬁlter (LP/HP) with control parameter
– cutoff frequency fc in hertz (lower horizontal slider)
– all frequencies above (LP) or below (HP) the cutoff frequency are attenuated
according to the shown frequency response.
• Low/high-frequency shelving ﬁlter (LFS/HFS) with control parameters
– cutoff frequency fc in hertz (lower horizontal slider)
– boost/cut in dB (left vertical slider with + for boost or − for cut)
– all frequencies below (LFS) or above (HFS) the cutoff frequency are boosted/
cut according to the selected boost/cut.
• Peak ﬁlter with control parameters
– center frequency fc in hertz (lower horizontal slider)
– boost/cut in dB (left vertical slider with + for boost or − for cut)
– Q-factor Q = fc/fb (right vertical slider), which controls the bandwidth fb of
the boost/cut around the adjusted center frequency fc. Lower Q-factor means
wider bandwidth.
– the peak ﬁlter boosts/cuts the center frequency with a bandwidth adjusted by
the Q-factor.
The center window shows the frequency response (ﬁlter gain versus frequency) of the
selected ﬁlter functions. You can choose between a linear and a logarithmic frequency
axis.
You can choose between two predeﬁned audio ﬁles from our web server (audio1.wav
or audio2.wav) or your own local wav ﬁle to be processed [Gui05].
5.6 Exercises
1. Design of Recursive Audio Filters
1. How can we design a low-frequency shelving ﬁlter? Which parameters deﬁne the
ﬁlter? Explain the control parameters.
2. How can we derive a high-frequency shelving ﬁlter? Which parameters deﬁne the
ﬁlter?
3. What is the difference between ﬁrst- and second-order shelving ﬁlters.
4. How can we design a peak ﬁlter? Which parameters deﬁne the ﬁlter? What is the
ﬁlter order? Explain the control parameters. Explain the Q-factor.
5. How do we derive the digital transfer function?
6. Derive the digital transfer functions for the ﬁrst-order shelving ﬁlters.
182
Equalizers
Figure 5.77 Java applet – audio ﬁlters.
2. Parametric Audio Filters
1. What is the basic idea of parametric ﬁlters?
2. What is the difference between the Regalia and the Zölzer ﬁlter structures? Count
the number of multiplications and additions for both ﬁlter structures.
3. Derive a signal ﬂow graph for ﬁrst- and second-order parametric Zölzer ﬁlters with
a direct-form implementation of the all-pass ﬁlters.
4. Is there a complete decoupling of all control parameters for boost and cut? Which
parameters are decoupled?
3. Shelving Filter: Direct Form
Derive a ﬁrst-order low shelving ﬁlter from a purely band-limiting ﬁrst-order low-pass
ﬁlter. Use a bilinear transform and give the transfer function of the low shelving ﬁlter.
1. Write down what you know about the ﬁlter coefﬁcients and calculate the poles/zeros
as functions of V0 and T . What gain factor do you have if z = ±1?
2. What is the difference between purely band-limiting ﬁlters and the shelving ﬁlter?
3. How can you describe the boost and cut effect related to poles/zeros of the ﬁlter?
4. How do we get a transfer function for the boost case from the cut case?
5. How do we go from a low shelving ﬁlter to a high shelving ﬁlter?
5.6 Exercises
183
4. Shelving Filter: All-pass Form
Implement a ﬁrst-order high shelving ﬁlter for the boost and cut cases with the sampling
rate fS = 44.1 kHz, cutoff frequency fc = 10 kHz and gain G = 12 dB.
1. Deﬁne the all-pass parameters and coefﬁcients for the boost and cut cases.
2. Derive from the all-pass decomposition the complete transfer function of the shelving
ﬁlter.
3. Using Matlab, give the magnitude frequency response for boost and cut. Show the
result for the case where a boost and cut ﬁlter are in a series connection.
4. If the input signal to the system is a unit impulse, give the spectrum of the input and
out signal for the boost and cut cases. What result do you expect in this case when
boost and cut are again cascaded?
5. Quantization of Filter Coefﬁcients
For the quantization of the ﬁlter coefﬁcients different methods have been proposed: direct
form, Gold and Rader, Kingsbury and Zölzer.
1. What is the motivation behind this?
2. Plot a pole distribution using the quantized polar representation of a second-order
IIR ﬁlter
H (z) =
N(z)
1 − 2r cos ϕz−1 + r 2z−2 .
6. Signal Quantization inside the Audio Filter
Now we combine coefﬁcient and signal quantization.
1. Design a digital high-pass ﬁlter (second-order IIR), with a cutoff frequency fc =
50 Hz. (Use the Butterworth, Chebyshev or elliptic design methods implemented in
Matlab.)
2. Quantize the signal only when it leaves the accumulator (i.e. before it is saved in any
state variable).
3. Now quantize the coefﬁcients (direct form), too.
4. Extend your quantization to every arithmetic operation (i.e. after each addition/
multiplication).
184
Equalizers
7. Quantization Effects in Recursive Audio Filters
1. Why is the quantization of signals inside a recursive ﬁlter of special interest?
2. Derive the noise transfer function of the second-order direct-form ﬁlter. Apply a ﬁrst-
and second-order noise shaping to the quantizer inside the direct-form structure and
discuss its inﬂuence. What is the difference between second-order noise shaping and
double-precision arithmetic?
3. Write a Matlab implementation of a second-order ﬁlter structure for quantization and
noise shaping.
8. Fast Convolution
For an input sequence x(n) of length N1 = 500 and the impulse response h(n) of length
N2 = 31, perform the discrete-time convolution.
1. Give the discrete-time convolution sum formula.
2. Using Matlab, deﬁne x(n) as a sum of two sinusoids and derive h(n) with Matlab
function remez(..).
3. Realize the ﬁlter operation with Matlab using:
• the function conv(x,h)
• the sample-by-sample convolution sum method
• the FFT method
• the FFT with overlap-add method.
4. Describe FIR ﬁltering with the fast convolution technique. What conditions do the
input signal and the impulse responses have to fulﬁll if convolution is performed by
equivalent frequency-domain processing?
5. What happens if input signal and impulse response are as long as the FFT transform
length?
6. How can we perform the IFFT by the FFT algorithm?
7. Explain the processing steps
• for a segmentation of the input signal into blocks and fast convolution;
• for a stereo signal by the fast convolution technique;
• for the segmentation of the impulse response.
8. What is the processing delay of the fast convolution technique?
9. Write a Matlab program for fast convolution.
10. How does quantization of the signal inﬂuence the roundoff noise behavior of an FIR
ﬁlter?
References
185
9. FIR Filter Design by Frequency Sampling
1. Why is frequency sampling an important design method for audio equalizers? How
do we sample magnitude and phase response?
2. What is the linear phase frequency response of a system? What is the effect on an
input signal passing through such a system?
3. Explain the derivation of the magnitude and phase response for a linear phase FIR
ﬁlter.
4. What is the condition for a real-valued impulse response of even length N? What is
the group delay?
5. Write a Matlab program for the design of an FIR ﬁlter and verify the example in the
book.
6. If the desired frequency response is an ideal low-pass ﬁlter of length NF = 31 with
cutoff frequency (cid:4)c = π/2, derive the impulse response of this system. What will
the result be for NF = 32 and (cid:4)c = π?
10. Multi-complementary Filter Bank
1. What is an octave-spaced frequency splitting and how can we design a ﬁlter bank for
that task?
2. How can we perform aliasing-free subband processing? How can we achieve narrow
transition bands for a ﬁlter bank? What is the computational complexity of an octave-
spaced ﬁlter bank?
References
[Abu79] A. I. Abu-El-Haija, A. M. Peterson: An Approach to Eliminate Roundoff Errors
in Digital Filters, IEEE Trans. ASSP, Vol. 3, pp. 195–198, April 1979.
[Ave71]
E. Avenhaus: Zum Entwurf digitaler Filter mit minimaler Speicherwortlänge
für Koefﬁzienten und Zustandsgrössen, Ausgewählte Arbeiten über Nachricht-
ensysteme, No. 13, Prof. Dr.-Ing. W. Schüssler, Ed., Erlangen 1971.
[Bar82]
C. W. Barnes: Error Feedback in Normal Realizations of Recursive Digital
Filters, IEEE Trans. Circuits and Systems, pp. 72–75, January 1982.
[Bom85] B. W. Bomar: New Second-Order State-Space Structures for Realizing Low
Roundoff Noise Digital Filters, IEEE Trans. ASSP, Vol. 33, pp. 106–110, Feb.
1985.
[Bri94]
R. Bristow-Johnson: The Equivalence of Various Methods of Computing
Biquad Coefﬁcients for Audio Parametric Equalizers, Proc. 97th AES
Convention, Preprint No. 3906, San Francisco, November 1994.
186
[Cad87]
[Cha78]
[Cla00]
Equalizers
J. A. Cadzow: Foundations of Digital Signal Processing and Data Analysis,
New York: Macmillan, 1987.
T. L. Chang: A Low Roundoff Noise Digital Filter Structure, Proc. Int. Symp.
on Circuits and Systems, pp. 1004–1008, May 1978.
R. J. Clark, E. C. Ifeachor, G. M. Rogers, P. W. J. Van Eetvelt: Techniques
for Generating Digital Equalizer Coefﬁcients, J. Audio Eng. Soc., Vol. 48, pp.
281–298, April 2000.
[Cre03]
L. Cremer, M. Möser: Technische Akustik, Springer-Verlag, Berlin, 2003.
[Dat97]
[Din95]
J. Dattorro: Effect Design – Part 1: Reverberator and Other Filters, J. Audio
Eng. Soc., Vol. 45, pp. 660–684, September 1997.
Yinong Ding, D. Rossum: Filter Morphing of Parametric Equalizers and
Shelving Filters for Audio Signal Processing, J. Audio Eng. Soc., Vol. 43, pp.
821–826, October 1995.
[Duh88]
P. Duhamel, B. Piron, J. Etcheto: On Computing the Inverse DFT, IEEE Trans.
Acoust., Speech, Signal Processing, Vol. 36, pp. 285–286, February 1988.
[Ege96]
G. P. M. Egelmeers, P. C. W. Sommen: A New Method for Efﬁcient Convolution
in Frequency Domain by Nonuniform Partitioning for Adaptive Filtering, IEEE
Trans. Signal Processing, Vol. 44, pp. 3123–3192, December 1996.
[Ell82]
[Fet72]
[Fli92]
D. F. Elliott, K. R. Rao: Fast Transforms: Algorithms, Analyses, Applications,
Academic Press, New York, 1982.
A. Fettweis: On the Connection Between Multiplier Wordlength Limitation and
Roundoff Noise in Digital Filters, IEEE Trans. Circuit Theory, pp. 486–491,
September 1972.
N. J. Fliege, U. Zölzer: Multi-complementary Filter Bank: A New Concept with
Aliasing-Free Subband Signal Processing and Perfect Reconstruction, Proc.
EUSIPCO-92, pp. 207–210, Brussels, August 1992.
[Fli93a]
N.J. Fliege, U. Zölzer: Multi-complementary Filter Bank, Proc. ICASSP-93,
pp. 193–196, Minneapolis, April 1993.
[Fli00]
[Fon01]
[Fon03]
N. Fliege: Multirate Digital Signal Processing, John Wiley & Sons, Ltd,
Chichester, 2000.
F. Fontana, M. Karjalainen: Magnitude-Complementary Filters for Dynamic
Equalization, Proc. of the DAFX-01, Limerick, pp. 97–101, December 2001.
F. Fontana, M. Karjalainen: A Digital Bandpass/Bandstop Complementary
Equalization Filter with Independent Tuning Characteristics, IEEE Signal
Processing Letters, Vol. 10, No. 4, pp. 119–122, April 2003.
[Gar95] W. G. Gardner: Efﬁcient Convolution Without Input-Output Delay, J. Audio
Eng. Soc., Vol. 43, pp. 127–136, 1995.
References
187
[Garc02] G. Garcia: Optimal Filter Partition for Efﬁcient Convolution with Short
Input/Output Delay, Proc. 113th AES Convention, Preprint No. 5660, Los
Angeles, October 2002.
[Gol67]
B. Gold, C. M. Rader: Effects of Parameter Quantization on the Poles of a
Digital Filter, Proc. IEEE, Vol. 55, pp. 688–689, May 1967.
[Gui05] M. Guillemard, C. Ruwwe, U. Zölzer: J-DAFx – Digital Audio Effects in Java,
Proc. 8th Int. Conference on Digital Audio Effects (DAFx-05), pp. 161–166,
Madrid, 2005.
[Har93]
F. J. Harris, E. Brooking: A Versatile Parametric Filter Using an Imbedded
All-Pass Sub-ﬁlter to Independently Adjust Bandwidth, Center Frequency and
Boost or Cut, Proc. 95th AES Convention, Preprint No. 3757, San Francisco,
1993.
[Här98]
A. Härmä: Implementation of Recursive Filters Having Delay Free Loops, Proc.
IEEE Int. Conf. Acoustics, Speech, and Signal Processing (ICASSP’98), Vol. 3,
pp. 1261–1264, Seattle, 1998.
[Hol06a] M. Holters, U. Zölzer: Parametric Recursive Higher-Order Shelving Filters,
Jahrestagung Akustik DAGA, Braunschweig, March 2006.
[Hol06b] M. Holters, U. Zölzer: Parametric Recursive Higher-Order Shelving Filters,
Proc. 120th Conv. Audio Eng. Soc., Preprint No. 6722, Paris, 2006.
[Hol06c] M. Holters, U. Zölzer: Parametric Higher-Order Shelving Filters, Proc.
EUSIPCO-06, Florence, 2006.
[Hol06d] M. Holters, U. Zölzer: Graphic Equalizer Design Using Higher-Order
Recursive Filters, Proc. of the 9th Int. Conference on Digital Audio Effects
(DAFx-06), pp. 37–40, Montreal, September 2006.
[Jur64]
[Kei04]
[Kin72]
E. I. Jury: Theory and Application of the z-Transform Method, John Wiley &
Sons, Inc., New York, 1964.
F. Keiler, U. Zölzer, Parametric Second- and Fourth-Order Shelving Filters for
Audio Applications, Proc. of IEEE 6th International Workshop on Multimedia
Signal Processing, Siena, September–October 2004.
N. G. Kingsbury: Second-Order Recursive Digital Filter Element for Poles
Near the Unit Circle and the Real z-Axis, Electronic Letters, Vol. 8, pp. 155–
156, March 1972.
[Moo83]
J. A. Moorer: The Manifold Joys of Conformal Mapping, J. Audio Eng. Soc.,
Vol. 31, pp. 826–841, 1983.
[Mou90]
J. N. Mourjopoulos, E. D. Kyriakis-Bitzaros, C. E. Goutis: Theory and Real-
Time Implementation of Time-Varying Digital Audio Filters, J. Audio Eng. Soc.,
Vol. 38, pp. 523–536, July/August 1990.
188
[Mul76]
[Mül99]
[Mül01]
Equalizers
C. T. Mullis, R. A. Roberts: Synthesis of Minimum Roundoff Noise Fixed Point
Digital Filters, IEEE Trans. Circuits and Systems, pp. 551–562, September
1976.
C. Müller-Tomfelde: Low Latency Convolution for Real-time Application, In
Proc. of the AES 16th International Conference: Spatial Sound Reproduction,
pp. 454–460, Rovaniemi, Finland, April 1999.
C. Müller-Tomfelde: Time-Varying Filter in Non-uniform Block Convolution,
Proc. of the COST G-6 Conference on Digital Audio Effects (DAFX-01),
Limerick, December 2001.
[Orf96a]
S. J. Orfanidis: Introduction to Signal Processing, Prentice Hall, Englewood
Cliffs, NJ, 1996.
[Orf96b]
S. J. Orfanidis: Digital Parametric Equalizer Design with Prescribed Nyquist-
Frequency Gain, Proc. 101st AES Convention, Preprint No. 4361, Los Angeles,
November 1996.
[Orf05]
S. J. Orfanidis: High-Order Digital Parametric Equalizer Design, J. Audio Eng.
Soc., Vol. 53, pp. 1026–1046, 2005.
[Rab88]
R. Rabenstein: Minimization of Transient Signals in Recursive Time-Varying
Digital Filters, Circuits, Systems, and Signal Processing, Vol. 7, No. 3, pp. 345–
359, 1988.
[Ram88] T. A. Ramstad, T. Saramäki: Efﬁcient Multirate Realization for Narrow
Transition-Band FIR Filters, Proc. IEEE Int. Symp. on Circuits and Syst.,
pp. 2019–2022, Espoo, Finland, June 1988.
[Ram90] T. A. Ramstad, T. Saramäki: Multistage, Multirate FIR Filter Structures for
Narrow Transition-Band Filters, Proc. IEEE Int. Symp. on Circuits and Syst.,
pp. 2017–2021, New Orleans, May 1990.
[Reg87]
P. A. Regalia, S. K. Mitra: Tunable Digital Frequency Response Equalization
Filters, IEEE Transactions on Acoustics, Speech, and Signal Processing,
Vol. 35, No. 1, pp. 118–120, January 1987.
[Sha92]
D. J. Shpak: Analytical Design of Biquadratic Filter Sections for Parametric
Filters, J. Audio Eng. Soc., Vol. 40, pp. 876–885, November 1992.
[Soo90]
[Sor87]
J. S. Soo, K. K. Pang: Multidelay Block Frequency Domain Adaptive Filter,
IEEE Transactions on Acoustics, Speech, and Signal Processing, Vol. 38, No. 2,
pp. 373–376, February 1990.
H. V. Sorensen, D. J. Jones, M. T. Heideman, C. S. Burrus: Real-Valued Fast
Fourier Transform Algorithms, IEEE Trans. ASSP, Vol. 35, No. 6, pp. 849–863,
June 1987.
References
[Sri77]
[Tra77]
[Vai93]
[Väl98]
189
A. B. Sripad, D. L. Snyder: A Necessary and Sufﬁcient Condition for
Quantization Errors to be Uniform and White, IEEE Trans. ASSP, Vol. 25,
pp. 442–448, October 1977.
Tran-Thong, B. Liu: Error Spectrum Shaping in Narrow Band Recursive
Filters, IEEE Trans. ASSP, pp. 200–203, April 1977.
P. P. Vaidyanathan: Multirate Systems and Filter Banks, Prentice Hall,
Englewood Cliffs, NJ, 1993.
V. Välimäki, T. I. Laakso: Suppression of Transients in Time-Varying Recursive
Filters for Audio Signals, Proc. IEEE Int. Conf. Acoustics, Speech, and Signal
Processing (ICASSP’98), Vol. 6, pp. 3569–3572, Seattle, 1998.
[Whi86]
S. A. White: Design of a Digital Biquadratic Peaking or Notch Filter for Digital
Audio Equalization, J. Audio Eng. Soc., Vol. 34, pp. 479–483, 1986.
[Zöl89]
[Zöl90]
[Zöl92]
[Zöl93]
[Zöl94]
[Zöl95]
U. Zölzer: Entwurf digitaler Filter für die Anwendung im Tonstudiobereich,
Wissenschaftliche Beiträge zur Nachrichtentechnik und Signalverarbeitung,
TU Hamburg-Harburg, June 1989.
U. Zölzer: A Low Roundoff Noise Digital Audio Filter, Proc. EUSIPCO-90, pp.
529–532, Barcelona, 1990.
U. Zölzer, N. Fliege: Logarithmic Spaced Analysis Filter Bank for Multiple
Loudspeaker Channels, Proc. 93rd AES Convention, Preprint No. 3453, San
Francisco, 1992.
U. Zölzer, B. Redmer, J. Bucholtz: Strategies for Switching Digital Audio
Filters, Proc. 95th AES Convention, Preprint No. 3714, New York, October
1993.
U. Zölzer: Roundoff Error Analysis of Digital Filters, J. Audio Eng. Soc.,
Vol. 42, pp. 232–244, April 1994.
U. Zölzer, T. Boltze: Parametric Digital Filter Structures, Proc. 99th AES
Convention, Preprint No. 4099, New York, October 1995.
