# Digital Audio DSP (Zölzer) — Chunk 12/30

> Source: `Udo-Zolzer-digital-audio-signal-processing.9780470997857.40435.pdf` · Pages ~1-331 · Extractor: pdfminer.six
> Target ≈ 2800 words, Overlap = 200 words · Actual = 7853

> Note: DSP textbook content for SpectralCanvasPro RT-audio knowledge base

---
Chapter 5
Equalizers
Spectral sound equalization is one of the most important methods for processing audio
signals. Equalizers are found in various forms in the transmission of audio signals from a
sound studio to the listener. The more complex ﬁlter functions are used in sound studios.
But in almost every consumer product like car radios, hiﬁ ampliﬁers, simple ﬁlter func-
tions are used for sound equalization. We ﬁrst discuss basic ﬁlter types followed by the
design and implementation of recursive audio ﬁlters. In Sections 5.3 and 5.4 linear phase
nonrecursive ﬁlter structures and their implementation are introduced.
5.1 Basics
For ﬁltering of audio signals the following ﬁlter types are used:
• Low-pass and high-pass ﬁlters with cutoff frequency fc (3 dB cutoff frequency)
are shown with their magnitude response in Fig. 5.1. They have a pass-band in the
lower and higher frequency range, respectively.
• Band-pass and band-stop ﬁlters (magnitude responses in Fig. 5.1) have a center
frequency fc and a lower and upper fl und fu cutoff frequency. They have a pass-
and stop-band in the middle of the frequency range. For the bandwidth of a band-pass
or a band-stop ﬁlter we have
fb = fu − fl .
Band-pass ﬁlters with a constant relative bandwidth fb/fc are very important for
audio applications [Cre03]. The bandwidth is proportional to the center frequency,
which is given by fc =
fl · fu (see Fig. 5.2).
• Octave ﬁlters are band-pass ﬁlters with special cutoff frequencies given by
(5.1)
√
fu = 2 · fl,
(cid:31)
fc =
fl · fu =
√
2 · fl .
(5.2)
(5.3)
A spectral decomposition of the audio frequency range with octave ﬁlters is shown
in Fig. 5.3. At the lower and upper cutoff frequency an attenuation of −3 dB occurs.
Digital Audio Signal Processing Second Edition Udo Zölzer
© 2008 John Wiley & Sons, Ltd
116
(cid:111)
S
|
)
f
/
f
(
H
|
(cid:111)
|
)
f
/
f
(
H
|
S
1
0.8
0.707
0.6
0.4
0.2
0
0
1
0.8
0.707
0.6
0.4
0.2
0
0
Low(cid:358)pass f
c
/f
S
=0.1
(cid:111)
0.1
0.2
0.3
0.4
0.5
f/f
S (cid:111)
/f
Band(cid:358)pass f
l
S
/f
=0.1 and f
u
S
=0.2
S
|
)
f
/
f
(
H
|
(cid:111)
|
)
f
/
f
(
S
H
|
0.1
0.2
0.3
0.4
0.5
f/f
S (cid:111)
Equalizers
High(cid:358)pass f
c
/f
S
=0.1
0.1
0.2
0.3
0.4
0.5
f/f
S (cid:111)
/f
Band(cid:358)stop f
l
S
/f
=0.1 and f
u
S
=0.2
0.1
0.2
0.3
0.4
0.5
f/f
S (cid:111)
1
0.8
0.707
0.6
0.4
0.2
0
0
1
0.8
0.707
0.6
0.4
0.2
0
0
Figure 5.1 Linear magnitude responses of low-pass, high-pass, band-pass, and band-stop ﬁlters.
The upper octave band is represented as a high-pass. A parallel connection of octave
ﬁlters can be used for a spectral analysis of the audio signal in octave frequency
bands. This decomposition is used for the signal power distribution across the octave
= 2 · fci−1 . The weight-
bands. For the center frequencies of octave bands we get fci
ing of octave bands with gain factors Ai and summation of the weighted octave
bands represents an octave equalizer for sound processing (see Fig. 5.4). For this
application the lower and upper cutoff frequencies need an attenuation of −6 dB,
such that a sinusoid at the crossover frequency has gain of 0 dB. The attenuation
of −6 dB is achieved through a series connection of two octave ﬁlters with −3 dB
attenuation.
• One-third octave ﬁlters are band-pass ﬁlters (see Fig. 5.3) with cutoff frequencies
given by
√
fu = 3
√
fc = 6
2 · fl,
2 · fl.
(5.4)
(5.5)
The attenuation at the lower and upper cutoff frequency is −3 dB. One-third octave
ﬁlters split an octave into three frequency bands (see Fig. 5.3).
5.1 Basics
117
a) Band(cid:358)pass filters with constant relative bandwidth versus logarithmic frequency
5
0
(cid:358)3
(cid:358)5
(cid:358)10
(cid:358)15
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
(cid:358)20
20
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
5
0
(cid:358)3
(cid:358)5
(cid:358)10
(cid:358)15
(cid:358)20
200
f/Hz (cid:111)
2000
20000
b)  Band(cid:358)pass filters with constant relative bandwidth versus linear frequency
0.2
0.4
0.6
0.8
1
f/Hz (cid:111)
1.2
1.4
1.6
1.8
2
4
x 10
Figure 5.2 Logarithmic magnitude responses of band-pass ﬁlters with constant relative bandwidth.
Octave band(cid:358)pass filters
One(cid:358)third octave band(cid:358)pass filters
S
(cid:111)
|
)
f
/
f
(
H
|
1
0.8
0.707
0.6
0.4
0.2
0
0
(cid:111)
|
)
f
/
f
(
S
H
|
0.1
0.2
0.3
0.4
0.5
f/f
S (cid:111)
1
0.8
0.707
0.6
0.4
0.2
0
0
0.1
0.2
0.3
0.4
0.5
f/f
S (cid:111)
Figure 5.3 Linear magnitude responses of octave ﬁlters and decomposition of an octave band by
three one-third octave ﬁlters.
• Shelving ﬁlters and peak ﬁlters are special weighting ﬁlters, which are based on
low-pass/high-pass/band-pass ﬁlters and a direct path (see Section 5.2.2). They have
no stop-band compared to low-pass/high-pass/band-pass ﬁlters. They are used in
Equalizers
f/Hz
118
|H(f)|/dB
+12
+6
0
-6
-12
x(n)
y(n)
A1
A2
AN
BP1
BP2
BPN
Figure 5.4 Parallel connection of band-pass ﬁlters (BP) for octave/one-third octave equalizers with
gain factors (Ai for octave or one-third octave band).
a series connection of shelving and peak ﬁlters as shown in Fig. 5.5. The lower
frequency range is equalized by low-pass shelving ﬁlters and the higher frequencies
are modiﬁed by high-pass shelving ﬁlters. Both ﬁlter types allow the adjustment of
cutoff frequency and gain factor. For the mid-frequency range a series connection
of peak ﬁlters with variable center frequency, bandwidth, and gain factor are used.
These shelving and peak ﬁlters can also be applied for octave and one-third octave
equalizers in a series connection.
|H(f)|/dB
+12
+6
0
-6
-12
f c
LF
Shelving
Filter
x(n)
f b1
f c1
Peak
Filter
f bi
f ci
Peak
Filter
f/Hz
f c
HF
Shelving
Filter
y(n)
Cutoff frequency f
Gain G in dB
c
Center frequency f
Bandwidth f
b1
Gain G in dB
c1
Center frequency f
Bandwidth f
bi
Gain G in dB
ci
Cutoff frequency f
Gain G in dB
c
Figure 5.5 Series connection of shelving and peak ﬁlters (low-frequency LF, high-frequency HF).
• Weighting ﬁlters are used for signal level and noise measurement applications. The
signal from a device under test is ﬁrst passed through the weighting ﬁlter and then a
root mean square or peak value measurement is performed. The two most often used
ﬁlters are the A-weighting ﬁlter and the CCIR-468 weighting ﬁlter (see Fig. 5.6).
Both weighting ﬁlters take the increased sensitivity of the human perception in the
5.2 Recursive Audio Filters
119
1–6 kHz frequency range into account. The 0 dB of the magnitude response of both
ﬁlters is crossed at 1 kHz. The CCIR-468 weighting ﬁlter has a gain of 12 dB at
6 kHz. A variant of the CCIR-468 ﬁlter is the ITU-ARM 2 kHz weighting ﬁlter,
which is a 5.6 dB down tilted version of the CCIR-468 ﬁlters and passes the 0 dB at
2 kHz.
10
0
(cid:358)10
(cid:358)20
(cid:358)30
(cid:358)40
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
(cid:358)50
20
A(cid:358)weighting
CCIR 486 weighting
200
2000
20000
f/Hz (cid:111)
Figure 5.6 Magnitude responses of weighting ﬁlters for root mean square and peak value
measurements.
5.2 Recursive Audio Filters
5.2.1 Design
A certain ﬁlter response can be approximated by two kinds of transfer function. On the one
hand, the combination of poles and zeros leads to a very low-order transfer function H (z) in
fractional form, which solves the given approximation problem. The digital implementation
of this transfer function needs recursive procedures owing to its poles. On the other hand,
the approximation problem can be solved by placing only zeros in the z-plane. This transfer
function H (z) has, besides its zeros, a corresponding number of poles at the origin of
the z-plane. The order of this transfer function, for the same approximation conditions, is
substantially higher than for transfer functions consisting of poles and zeros. In view of
an economical implementation of a ﬁlter algorithm in terms of computational complexity,
recursive ﬁlters achieve shorter computing time owing to their lower order. For a sampling
rate of 48 kHz, the algorithm has 20.83 µs processing time available. With the DSPs
presently available it is easy to implement recursive digital ﬁlters for audio applications
within this sampling period using only one DSP. To design the typical audio equalizers
we will start with ﬁlter designs in the S-domain. These ﬁlters will then be mapped to the
Z-domain by the bilinear transformation.
Low-pass/High-pass Filters. In order to limit the audio spectrum, low-pass and high-
pass ﬁlters with Butterworth response are used in analog mixers. They offer a monotonic
pass-band and a monotonically decreasing stop-band attenuation per octave (n · 6 dB/oct.)
that is determined by the ﬁlter order. Low-pass ﬁlters of the second and fourth order
are commonly used. The normalized and denormalized second-order low-pass transfer
120
functions are given by
Equalizers
HLP(s) =
1
Q∞ s + 1
s2 + 1
and HLP(s) =
ω2
c
s2 + ωc
Q∞ s + ω2
c
,
(5.6)
where ωc is the cutoff frequency and Q∞ is the pole quality factor. The Q-factor Q∞ of a
Butterworth approximation is equal to 1/
2. The denormalization of a transfer function is
obtained by replacing the Laplace variable s by s/ωg in the normalized transfer function.
√
The corresponding second-order high-pass transfer functions
HHP(s) =
s2
Q∞ s + 1
s2 + 1
and HHP(s) =
s2
s2 + ωc
Q∞ s + ω2
c
(5.7)
are obtained by a low-pass to high-pass transformation. Figure 5.7 shows the pole-zero
locations in the s-plane. The amplitude frequency response of a high-pass ﬁlter with a 3 dB
cutoff frequency of 50 Hz and a low-pass ﬁlter with a 3 dB cutoff frequency of 5000 Hz
are shown in Fig. 5.8. Second- and fourth-order ﬁlters are shown.
a)
j(cid:12)
b)
j(cid:12)
(cid:5)
(2)
(cid:5)
Figure 5.7 Pole-zero location for (a) second-order low-pass and (b) second-order high-pass.
5
0
(cid:358)3
(cid:358)5
(cid:358)10
(cid:358)15
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
2nd HP
2nd LP
4th HP
4th LP
(cid:358)20
20
200
f/Hz (cid:111)
2000
20000
Figure 5.8 Frequency response of
(second/fourth order), low-pass fc = 5000 Hz (second/fourth order).
low-pass and high-pass ﬁlters – high-pass fc = 50 Hz
Table 5.1 summarizes the transfer functions of low-pass and high-pass ﬁlters with
Butterworth response.
5.2 Recursive Audio Filters
121
Table 5.1 Transfer functions of low-pass and high-pass ﬁlters.
Low-pass H (s) =
H (s) =
High-pass H (s) =
H (s) =
1
√
2s + 1
s2 +
1
(s2 + 1.848s + 1)(s2 + 0.765s + 1)
s2
√
s2 +
2s + 1
s4
(s2 + 1.848s + 1)(s2 + 0.765s + 1)
second order
fourth order
second order
fourth order
Band-pass and band-stop ﬁlters. The normalized and denormalized band-pass transfer
functions of second order are
HBP(s) =
1
Q∞ s
Q∞ s + 1
s2 + 1
and HBP(s) =
and the band-stop transfer functions are given by
HBS(s) =
s2 − 1
Q∞ s + 1
s2 + 1
and HBS(s) =
The relative bandwidth can be expressed by the Q-factor
Q∞ = fc
fb
,
ωc
Q∞ s
Q∞ s + ω2
c
s2 + ωc
s2 − ω2
c
Q∞ s + ω2
c
s2 + ωc
,
.
(5.8)
(5.9)
(5.10)
which is the ratio of center frequency fc and the 3 dB bandwidth given by fb. The magni-
tude responses of band-pass ﬁlters with constant relative bandwidth are shown in Fig. 5.2.
Such ﬁlters are also called constant-Q ﬁlters. The geometric symmetric behavior of the
frequency response regarding the center frequency fc is clearly noticeable (symmetry re-
garding the center frequency using a logarithmic frequency axis).
Shelving Filters. Besides the purely band-limiting ﬁlters like low-pass and high-pass ﬁl-
ters, shelving ﬁlters are used to perform weighting of certain frequencies. A simple ap-
proach for a ﬁrst-order low-pass shelving ﬁlter is given by
H (s) = 1 + HLP(s) = 1 + H0
s + 1
.
(5.11)
It consists of a ﬁrst-order low-pass ﬁlter with dc ampliﬁcation of H0 connected in parallel
with an all-pass system of transfer function equal to 1. Equation (5.11) can be written as
H (s) = s + (1 + H0)
s + 1
= s + V0
s + 1
,
(5.12)
122
Equalizers
where V0 determines the ampliﬁcation at ω = 0. By changing the parameter V0, any desired
boost (V0 > 1) and cut (V0 < 1) level can be adjusted. Figure 5.9 shows the frequency
responses for fc = 100 Hz. For V0 < 1, the cutoff frequency is dependent on V0 and is
moved toward lower frequencies.
20
10
0
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
(cid:358)10
(cid:358)20
20
 > 1
V
0
 < 1
V
0
200
f/Hz (cid:111)
2000
20000
Figure 5.9 Frequency response of transfer function (5.12) with varying V0 and cutoff frequency
fc = 100 Hz.
In order to obtain a symmetrical frequency response with respect to the zero decibel line
without changing the cutoff frequency, it is necessary to invert the transfer function (5.12)
in the case of cut (V0 < 1). This has the effect of swapping poles with zeros and leads to
the transfer function
H (s) = s + 1
s + V0
(5.13)
for the cut case. Figure 5.10 shows the corresponding frequency responses for varying V0.
20
10
0
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
(cid:358)10
(cid:358)20
20
 < 1
V
0
 > 1
V
0
200
f/Hz (cid:111)
2000
20000
Figure 5.10 Frequency responses of transfer function (5.13) with varying V0 and cutoff frequency
fc = 100 Hz.
Finally, Figure 5.11 shows the locations of poles and zeros for both the boost and the
cut case. By moving zeros and poles on the negative σ -axis, boost and cut can be adjusted.
5.2 Recursive Audio Filters
123
,(cid:11)(cid:11)(cid:18)(cid:7)
](cid:14)
$(cid:13)(cid:7)
](cid:14)
(cid:5)(cid:22)
(cid:13)
(cid:5)L
3
(cid:14)L
3
(cid:5)(cid:22)
(cid:13)
(cid:5)L
3
(cid:14)L
3
Figure 5.11 Pole-zero locations of a ﬁrst-order low-frequency shelving ﬁlter.
The equivalent shelving ﬁlter for high frequencies can be obtained by
H (s) = 1 + HHP(s) = 1 + H0s
s + 1
,
(5.14)
which is a parallel connection of a ﬁrst-order high-pass with gain H0 and a system with
transfer function equal to 1. In the boost case the transfer function can written with V0 =
H0 + 1 as
H (s) = sV0 + 1
s + 1
,
V0 > 1,
(5.15)
and for cut we get
H (s) = s + 1
sV0 + 1
The parameter V0 determines the value of the transfer function H (s) at ω = ∞ for high-
frequency shelving ﬁlters.
V0 > 1.
(5.16)
,
In order to increase the slope of the ﬁlter response in the transition band, a general
second-order transfer function
H (s) = a2s2 + a1s + a0
2s + 1
s2 +
√
(5.17)
is considered, in which complex zeros are added to the complex poles. The calculation of
poles leads to
s∞ 1/2 =
1
2 (−1 ± j ).
(5.18)
If the complex zeros
!
s◦ 1/2 =
(−1 ± j )
V0
2
are moved on a straight line with the help of the parameter V0 (see Fig. 5.12), the transfer
function
H (s) = s2 +
s2 +
of a second-order low-frequency shelving ﬁlter is obtained. The parameter V0 determines
the boost for low frequencies. The cut case can be achieved by inversion of (5.20).
2V0s + V0
√
2s + 1
(5.20)
(5.19)
√

124
](cid:14)
,(cid:11)(cid:11)(cid:18)(cid:7)
L3
$(cid:13)(cid:7)
L3
Equalizers
](cid:14)
(cid:5)(cid:22)
(cid:13)
(cid:5)(cid:22)
(cid:13)
Figure 5.12 Pole-zero locations of a second-order low-frequency shelving ﬁlter.
A low-pass to high-pass transformation of (5.20) provides the transfer function
H (s) = V0s2 +
s2 +
√
√
2V0s + 1
2s + 1
of a second-order high-frequency shelving ﬁlter. The zeros
s◦ 1/2 =
"
1
2V0
(−1 ± j )
(5.21)
(5.22)
are moved on a straight line toward the origin with increasing V0 (see Fig. 5.13). The cut
case is obtained by inverting the transfer function (5.21). Figure 5.14 shows the amplitude
frequency response of a second-order low-frequency shelving ﬁlter with cutoff frequency
100 Hz and a second-order high-frequency shelving ﬁlter with cutoff frequency 5000 Hz
(parameter V0).
,(cid:11)(cid:11)(cid:18)(cid:7)
](cid:14)
$(cid:13)(cid:7)
](cid:14)
L
3
(cid:5)(cid:22)
(cid:13)
L
3
(cid:5)(cid:22)
(cid:13)
Figure 5.13 Pole-zero locations of second-order high-frequency shelving ﬁlter.
Peak Filter. Another equalizer used for boosting or cutting any desired frequency is the
peak ﬁlter. A peak ﬁlter can be obtained by a parallel connection of a direct path and a
5.2 Recursive Audio Filters
125
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
20
15
10
5
0
(cid:358)5
(cid:358)10
(cid:358)15
(cid:358)20
20
200
f/Hz (cid:111)
2000
20000
Figure 5.14 Frequency responses of second-order low-/high-frequency shelving ﬁlters – low-
frequency shelving ﬁlter fc = 100 Hz (parameter V0), high-frequency shelving ﬁlter fc = 5000 Hz
(parameter V0).
band-pass according to
H (s) = 1 + HBP(s).
With the help of a second-order band-pass transfer function
HBP(s) = (H0/Q∞)s
Q∞ s + 1
s2 + 1
,
the transfer function
H (s) = 1 + HBP(s) =
s2 + 1+H0
s2 + 1
Q∞ s + 1
Q∞ s + 1
=
s2 + V0
s2 + 1
Q∞ s + 1
Q∞ s + 1
(5.23)
(5.24)
(5.25)
of a peak ﬁlter can be derived. It can be shown that the maximum of the amplitude fre-
quency response at the center frequency is determined by the parameter V0. The relative
bandwidth is ﬁxed by the Q-factor. The geometrical symmetry of the frequency response
relative to the center frequency remains constant for the transfer function of a peak ﬁl-
ter (5.25). The poles and zeros lie on the unit circle. By adjusting the parameter V0, the
complex zeros are moved with respect to the complex poles. Figure 5.15 shows this for the
boost and cut cases. With increasing Q-factor, the complex poles move toward the j ω-axis
on the unit circle.
Figure 5.16 shows the amplitude frequency response of a peak ﬁlter by changing the
parameter V0 at a center frequency of 500 Hz and a Q-factor of 1.25. Figure 5.17 shows
126
Equalizers
,(cid:11)(cid:11)(cid:18)(cid:7)
](cid:14)
$(cid:13)(cid:7)
](cid:14)
L
3
(cid:5)(cid:22)
(cid:13)
L
3
(cid:5)(cid:22)
(cid:13)
Figure 5.15 Pole-zero locations of a second-order peak ﬁlter.
the variation of the Q-factor Q∞ at a center frequency of 500 Hz, a boost/cut of ±16 dB
and Q-factor of 1.25. Finally, the variation of the center frequency with boost and cut of
±16 dB and a Q-factor 1.25 is shown in Fig. 5.18.
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
20
15
10
5
0
(cid:358)5
(cid:358)10
(cid:358)15
(cid:358)20
20
200
f/Hz (cid:111)
2000
20000
Figure 5.16 Frequency response of a peak ﬁlter – fc = 500 Hz, Q∞ = 1.25, cut parameter V0.
Mapping to Z-domain. In order to implement a digital ﬁlter, the ﬁlter designed in the
S-domain with transfer function H (s) is converted to the Z-domain with the help of a
suitable transformation to obtain the transfer function H (z). The impulse-invariant trans-
formation is not suitable as it leads to overlapping effects if the transfer function H (s) is not
band-limited to half the sampling rate. An independent mapping of poles and zeros from
the S-domain into poles and zeros in the Z-domain is possible with help of the bilinear
5.2 Recursive Audio Filters
127
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
20
15
10
5
0
(cid:358)5
(cid:358)10
(cid:358)15
(cid:358)20
20
200
f/Hz (cid:111)
2000
20000
Figure 5.17 Frequency responses of peak ﬁlters – fc = 500 Hz, boost/cut ±16 dB, Q∞ = 0.707,
1.25, 2.5, 3, 5.
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
20
15
10
5
0
(cid:358)5
(cid:358)10
(cid:358)15
(cid:358)20
20
200
f/Hz (cid:111)
2000
20000
Figure 5.18 Frequency responses of peak ﬁlters – boost/cut ±16 dB, Q∞ = 1.25, fc = 50, 200,
1000, 4000 Hz.
128
transformation given by
s = 2
T
z − 1
z + 1
.
Tables 5.2–5.5 contain the coefﬁcients of the second-order transfer function
H (z) = a0 + a1z−1 + a2z−2
1 + b1z−1 + b2z−2 ,
Equalizers
(5.26)
(5.27)
which are determined by the bilinear transformation and the auxiliary variable K =
tan(ωcT /2) for all audio ﬁlter types discussed. Further ﬁlter designs of peak and shelving
ﬁlters are discussed in [Moo83, Whi86, Sha92, Bri94, Orf96a, Dat97, Cla00]. A method
for reducing the warping effect of the bilinear transform is proposed in [Orf96b]. Strategies
for time-variant switching of audio ﬁlters can be found in [Rab88, Mou90, Zöl93, Din95,
Väl98].
Table 5.2 Low-pass/high-pass/band-pass ﬁlter design.
a0
a1
a2
b1
Low-pass (second order)
√
K 2
2K + K 2
1 +
2K 2
√
2K + K 2
1 +
√
K 2
2K + K2
1 +
2(K 2 − 1)
√
1 +
2K + K2
a0
a1
a2
b1
High-pass (second order)
√
1
2K + K 2
1 +
√
−2
2K + K 2
1 +
√
1
2K + K2
1 +
2(K 2 − 1)
√
1 +
2K + K2
b2
1 −
1 +
b2
1 −
1 +
√
√
2K + K2
2K + K2
√
√
2K + K2
2K + K2
Band-pass (second order)
a0
1
Q K
Q K + K 2
1 + 1
a1
0
a2
b1
−
1
Q K
Q K + K 2
1 + 1
2(K 2 − 1)
1 + 1
Q K + K 2
b2
1 − 1
1 + 1
Q K + K 2
Q K + K 2
5.2.2 Parametric Filter Structures
Parametric ﬁlter structures allow direct access to the parameters of the transfer function,
like center/cutoff frequency, bandwidth and gain, via control of associated coefﬁcients. To
modify one of these parameters, it is therefore not necessary to compute a complete set
of coefﬁcients for a second-order transfer function, but instead only one coefﬁcient in the
ﬁlter structure is calculated.
Independent control of gain, cutoff/center frequency and bandwidth for shelving and
peak ﬁlters is achieved by a feed forward (FF) structure for boost and a feed backward
5.2 Recursive Audio Filters
129
Table 5.3 Peak ﬁlter design.
a1
a0
1 + V0
1 + 1
Q∞ K + K 2
Q∞ K + K 2
2(K 2 − 1)
Q∞ K + K 2
1 + 1
Peak (boost V0 = 10G/20)
b1
a2
1 − V0
1 + 1
Q∞ K + K 2
Q∞ K + K 2
2(K 2 − 1)
Q∞ K + K 2
1 + 1
b2
1 − 1
1 + 1
Q∞ K + K 2
Q∞ K + K 2
Peak (cut V0 = 10
−G/20)
a0
1 + 1
1 + V0
Q∞ K + K 2
Q∞ K + K 2
a1
2(K 2 − 1)
Q∞ K + K 2
1 + V0
a2
1 − 1
1 + V0
Q∞ K + K 2
Q∞ K + K 2
b1
2(K 2 − 1)
Q∞ K + K 2
1 + V0
b2
1 − V0
1 + V0
Q∞ K + K 2
Q∞ K + K 2
Table 5.4 Low-frequency shelving ﬁlter design.
Low-frequency shelving (boost V0 = 10G/20)
a0
1 +
(cid:31)
2V0K + V0K2
√
2K + K2
1 +
a1
2(V0K2 − 1)
2K + K2
1 +
√
a2
1 −
(cid:31)
2V0K + V0K2
√
2K + K2
1 +
b1
2(K2 − 1)
√
1 +
2K + K2
b2
1 −
1 +
√
√
2K + K2
2K + K2
Low-frequency shelving (cut V0 = 10
−G/20)
a0
√
a1
a2
√
1 +
(cid:31)
2K + K2
2V0K + V0K2
1 +
2(K2 − 1)
(cid:31)
2V0K + V0K2
1 +
1 −
(cid:31)
2K + K2
2V0K + V0K2
1 +
b1
2(V0K2 − 1)
(cid:31)
2V0K + V0K2
1 +
b2
1 −
1 +
(cid:31)
(cid:31)
2V0K + V0K2
2V0K + V0K2
(cid:31)
a0
V0 +
1 +
2V0K + K2
√
2K + K2
Table 5.5 High-frequency shelving ﬁlter design.
High-frequency shelving (boost V0 = 10G/20)
a1
√
2(K2 − V0)
2K + K2
1 +
(cid:31)
a2
V0 −
1 +
2V0K + K2
√
2K + K2
b1
2(K2 − 1)
√
1 +
2K + K2
b2
1 −
1 +
√
√
2K + K2
2K + K2
High-frequency shelving (cut V0 = 10
−G/20)
a0
√
(cid:31)
1 +
V0 +
2K + K2
2V0K + K2
a1
a2
2(K2 − 1)
(cid:31)
V0 +
2V0K + K2
√
(cid:31)
1 −
V0 +
2K + K2
2V0K + K2
b1
2(K2/V0 − 1)
(cid:31)
2/V0K + K2/V0
1 +
b2
1 −
1 +
(cid:31)
(cid:31)
2/V0K + K2/V0
2/V0K + K2/V0
(FB) structure for cut as shown in Fig. 5.19. The corresponding transfer functions are:
GFW (z) = 1 + H0H (z),
GFB(z) =
1
1 + H0H (z)
.
(5.28)
(5.29)
130
Equalizers
x(n)
-
y(n)
H(z)
H0
Cut
Boost
Figure 5.19 Filter structure for implementing boost and cut ﬁlters.
The boost/cut factor is V0 = 1 + H0. For digital ﬁlter implementations, it is necessary
for the FB case that the inner transfer function be of the form H (z) = z−1H1(z) to ensure
causality. A parametric ﬁlter structure proposed by Harris [Har93] is based on the FF/FB
technique, but the frequency response shows slight deviations near z = 1 and z = −1 from
that desired. This is due to the z−1 in the FF/FB branch. Delay-free loops inside ﬁlter
computations can be solved by the methods presented in [Här98, Fon01, Fon03]. Higher-
order parametric ﬁlter designs have been introduced in [Kei04, Orf05, Hol06a, Hol06b,
Hol06c, Hol06d]. It is possible to implement typical audio ﬁlters with only an FF structure.
The complete decoupling of the control parameters is possible for the boost case, but there
remains a coupling between bandwidth and gain factor for the cut case. In the following,
two approaches for parametric audio ﬁlter structures based on an all-pass decomposition of
the transfer function will be discussed.
Regalia Filter [Reg87]. The denormalized transfer function of a ﬁrst-order shelving ﬁlter
is given by
H (s) = s + V0ωc
s + ωc
with
H (0) = V0,
H (∞) = 1.
A decomposition of (5.30) leads to
H (s) = s
s + ωc
+ V0
ωc
s + ωc
.
(5.30)
(5.31)
The low-pass and high-pass transfer functions in (5.31) can be expressed by an all-pass
decomposition of the form
s
s + ωc
V0ωc
s + ωc
(cid:5)
(cid:4)
1 + s − ωc
s + ωc
(cid:4)
1 − s − ωc
s + ωc
= 1
2
= V0
2
,
(cid:5)
.
(5.32)
(5.33)
(5.34)
With the all-pass transfer function
AB(s) = s − ωc
s + ωc
5.2 Recursive Audio Filters
for boost, (5.30) can be rewritten as
H (s) = 1
2
[1 + AB(s)] + 1
2 V0[1 − AB (s)].
The bilinear transformation
s = 2
T
z − 1
z + 1
H (z) = 1
2
[1 + AB(z)] + 1
2 V0[1 − AB(z)]
leads to
with
and the frequency parameter
AB(z) = − z−1 + aB
1 + aBz−1
aB = tan(ωcT /2) − 1
tan(ωcT /2) + 1
.
131
(5.35)
(5.36)
(5.37)
(5.38)
A ﬁlter structure for direct implementation of (5.36) is presented in Fig. 5.20a. Other
possible structures can be seen in Fig. 5.20b,c. For the cut case V0 < 1, the cutoff frequency
of the ﬁlter moves toward lower frequencies [Reg87].
A
x(n)
B
x(n)
C
x(n)
1/2
A(z)
y(n)
A(z)
A(z)
-
V0 /2
1/2
-
V0 /2
(1-V0 )/2
(1+V0 )/2
y(n)
y(n)
Figure 5.20 Filter structures by Regalia.
In order to retain the cutoff frequency for the cut case [Zöl95], the denormalized transfer
function of a ﬁrst-order shelving ﬁlter (cut)
H (s) = s + ωc
s + ωc/V0
,
(5.39)
132
with the boundary conditions
H (0) = V0,
H (∞) = 1,
can be decomposed as
H (s) =
s
s + ωc/V0
+
ωc
s + ωc/V0
.
With the all-pass decompositions
s
s + ωc/V0
ωc
s + ωc/V0
(cid:5)
(cid:4)
1 + s − ωc/V0
s + ωc/V0
(cid:4)
1 − s − ωc/V0
s + ωc/V0
= 1
2
= V0
2
,
(cid:5)
,
and the all-pass transfer function
AC(s) = s − ωc/V0
s + ωc/V0
for cut, (5.39) can be rewritten as
H (s) = 1
2
[1 + AC(s)] + V0
2
[1 − AC(s)].
The bilinear transformation leads to
H (z) = 1
2
[1 + AC(z)] + V0
2
[1 − AC(z)]
with
and the frequency parameter
AC(z) = − z−1 + aC
1 + aCz−1
aC = tan(ωcT /2) − V0
tan(ωcT /2) + V0
.
Equalizers
(5.40)
(5.41)
(5.42)
(5.43)
(5.44)
(5.45)
(5.46)
(5.47)
Due to (5.45) and (5.36), boost and cut can be implemented with the same ﬁlter structure
(see Fig. 5.20). However, it has to be noted that the frequency parameter aC as in (5.47) for
cut depends on the cutoff frequency and gain.
A second-order peak ﬁlter is obtained by a low-pass to band-pass transformation ac-
cording to
−1 → −z
z
−1 z−1 + d
1 + dz−1 .
For an all-pass as given in (5.37) and (5.46), the second-order all-pass is given by
ABC(z) = z−2 + d(1 + aBC)z−1 + aBC
1 + d(1 + aBC)z−1 + aBCz−2
(5.48)
(5.49)
5.2 Recursive Audio Filters
with parameters (cut as in [Zöl95])
d = − cos((cid:4)c),
V0 = H (ej (cid:4)c),
aB = 1 − tan(ωbT /2)
1 + tan(ωbT /2)
aC = V0 − tan(ωbT /2)
V0 + tan(ωbT /2)
,
.
133
(5.50)
(5.51)
(5.52)
(5.53)
The center frequency fc is ﬁxed by the parameter d, the bandwidth fb by the parameters
aB and aC, and gain by the parameter V0.
Simpliﬁed All-pass Decomposition [Zöl95]. The transfer function of a ﬁrst-order low-
frequency shelving ﬁlter can be decomposed as
H (s) = s + V0ωc
s + ωc
= 1 + H0
(5.54)
= 1 + H0
2
with
ωc
s + ωc
(cid:4)
1 − s − ωc
s + ωc
(cid:5)
(5.55)
(5.56)
(5.57)
(5.58)
V0 = H (s = 0),
H0 = V0 − 1,
V0 = 10G/20
(G in dB).
The transfer function (5.55) is composed of a direct branch and a low-pass ﬁlter. The
ﬁrst-order low-pass ﬁlter is again implemented by an all-pass decomposition. Applying
the bilinear transformation to (5.55) leads to
with
H (z) = 1 + H0
2
[1 − A(z)]
A(z) = − z−1 + aB
1 + aBz−1 .
For cut, the following decomposition can be derived:
H (s) = s + ωc
s + ωc/V0
ωc/V0
= 1 + (V0 − 1)
(cid:12) (cid:13)(cid:14) (cid:15)
s + ωc/V0
H0
(cid:4)
1 − s − ωc/V0
s + ωc/V0
= 1 + H0
2
(cid:5)
.
(5.59)
(5.60)
(5.61)
(5.62)
(5.63)
134
Equalizers
The bilinear transformation applied to (5.63) again gives (5.59). The ﬁlter structure is
identical for boost and cut. The frequency parameter aB for boost and aC for cut can be
calculated as
aB = tan(ωcT /2) − 1
tan(ωcT /2) + 1
aC = tan(ωcT /2) − V0
tan(ωcT /2) + V0
,
.
(5.64)
(5.65)
The transfer function of a ﬁrst-order low-frequency shelving ﬁlter can be calculated as
1 + (1 + aBC) H0
2
H (z) =
+ (aBC + (1 + aBC) H0
1 + aBCz−1
2 )z−1
.
(5.66)
With A1(z) = −A(z) the signal ﬂow chart in Fig. 5.21 shows a ﬁrst-order low-pass
ﬁlter and a ﬁrst-order low-frequency shelving ﬁlter.
first-order LF shelving filter
x(n)
A1(z)
y(n)
H0 /2
first-order low-pass filter
x(n)
A1(z)
1 /2
y(n)
Figure 5.21 Low-frequency shelving ﬁlter and ﬁrst-order low-pass ﬁlter.
The decomposition of a denormalized transfer function of a ﬁrst-order high-frequency
shelving ﬁlter can be given in the form
H (s) = sV0 + ωc
s + ωc
= 1 + H0
s
s + ωc
(cid:4)
1 + s − ωc
s + ωc
where
= 1 + H0
2
V0 = H (s = ∞),
H0 = V0 − 1.
(cid:5)
(5.67)
(5.68)
(5.69)
(5.70)
The transfer function results by adding a high-pass ﬁlter to a constant. Applying the bilinear
transformation to (5.68) gives
H (z) = 1 + H0
2
[1 + A(z)]
(5.71)
5.2 Recursive Audio Filters
with
A(z) = − z−1 + aB
1 + aBz−1 .
For cut, the decomposition can be given by
H (s) = s + ωc
s/V0 + ωc
s
= 1 + (V0 − 1)
(cid:12) (cid:13)(cid:14) (cid:15)
s + V0ωc
H0
(cid:4)
1 + s − V0ωc
s + V0ωc
= 1 + H0
2
(cid:5)
,
135
(5.72)
(5.73)
(5.74)
(5.75)
which in turn results in (5.71) after a bilinear transformation. The boost and cut parameters
can be calculated as
aB = tan(ωcT /2) − 1
tan(ωcT /2) + 1
aC = V0tan(ωcT /2) − 1
V0tan(ωcT /2) + 1
,
.
(5.76)
(5.77)
The transfer function of a ﬁrst-order high-frequency shelving ﬁlter can then be written as
1 + (1 − aBC) H0
2
H (z) =
+ (aBC + (aBC − 1) H0
1 + aBCz−1
2 )z−1
.
(5.78)
With A1(z) = −A(z) the signal ﬂow chart in Fig. 5.22 shows a ﬁrst-order high-pass ﬁlter
and a high-frequency shelving ﬁlter.
first-order HF shelving filter
A1(z)
-
H0 /2
y(n)
first-order high-pass filter
A1(z)
-
1 /2
y(n)
x(n)
x(n)
Figure 5.22 First-order high-frequency shelving and high-pass ﬁlters.
The implementation of a second-order peak ﬁlter can be carried out with a low-pass to
band-pass transformation of a ﬁrst-order shelving ﬁlter. But the addition of a second-order
band-pass ﬁlter to a constant branch also results in a peak ﬁlter. With the help of an all-pass
136
Equalizers
implementation of a band-pass ﬁlter as given by
and
H (z) = 1
2
[1 − A2(z)]
A2(z) =
−aB + (d − daB)z−1 + z−2
1 + (d − daB)z−1 − aBz−2 ,
a second-order peak ﬁlter can be expressed as
H (z) = 1 + H0
2
[1 − A2(z)].
The bandwidth parameters aB and aC for boost and cut are given
aB = tan(ωbT /2) − 1
tan(ωbT /2) + 1
aC = tan(ωbT /2) − V0
tan(ωbT /2) + V0
,
.
The center frequency parameter d and the coefﬁcient H0 are given by
d = −cos((cid:4)c),
V0 = H (ej (cid:4)c ),
H0 = V0 − 1.
The transfer function of a second-order peak ﬁlter results in
(5.79)
(5.80)
(5.81)
(5.82)
(5.83)
(5.84)
(5.85)
(5.86)
H (z) =
1 + (1 + aBC) H0
2
+ d(1 − aBC)z−1 + (−aBC − (1 + aBC) H0
1 + d(1 − aBC)z−1 − aBCz−2
The signal ﬂow charts for a second-order peak ﬁlter and a second-order band-pass ﬁlter are
shown in Fig. 5.23.
2 )z−2
(5.87)
.
second-order peak filter
x(n)
A2(z)
-
H0 /2
y(n)
second-order band-pass filter
x(n)
A2(z)
-
1 /2
y(n)
Figure 5.23 Second-order peak ﬁlter and band-pass ﬁlter.
The frequency responses for high-frequency shelving, low-frequency shelving and peak
ﬁlters are shown in Figs 5.24, 5.25 and 5.26.
5.2 Recursive Audio Filters
137
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
20
15
10
5
0
(cid:358)5
(cid:358)10
(cid:358)15
(cid:358)20
20
200
f/Hz (cid:111)
2000
20000
Figure 5.24 Low-frequency ﬁrst-order shelving ﬁlter (G = ±18 dB, fc = 20, 50, 100, 1000 Hz).
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
20
15
10
5
0
(cid:358)5
(cid:358)10
(cid:358)15
(cid:358)20
20
200
f/Hz (cid:111)
2000
20000
Figure 5.25 First-order high-frequency shelving ﬁlter (G = ±18 dB, fc = 1, 3, 5, 10, 16 kHz).
138
Equalizers
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
20
15
10
5
0
(cid:358)5
(cid:358)10
(cid:358)15
(cid:358)20
20
200
f/Hz (cid:111)
2000
20000
Figure 5.26 Second-order peak ﬁlter (G = ±18 dB, fc = 50, 100, 1000, 3000, 10000 Hz, fb =
100 Hz).
5.2.3 Quantization Effects
The limited word-length for digital recursive ﬁlters leads to two different types of quanti-
zation error. The quantization of the coefﬁcients of a digital ﬁlter results in linear distortion
which can be observed as a deviation from the ideal frequency response. The quantization
of the signal inside a ﬁlter structure is responsible for the maximum dynamic range and
determines the noise behavior of the ﬁlter. Owing to rounding operations in a ﬁlter struc-
ture, roundoff noise is produced. Another effect of the signal quantization is limit cycles.
These can be classiﬁed as overﬂow limit cycles, small-scale limit cycles and limit cycles
correlated with the input signal. Limit cycles are very disturbing owing to their small-band
(sinusoidal) nature. The overﬂow limit cycles can be avoided by suitable scaling of the
input signal. The effects of other errors mentioned above can be reduced by increasing the
word-lengths of the coefﬁcient and the state variables of the ﬁlter structure.
The noise behavior and coefﬁcient sensitivity of a ﬁlter structure depend on the topol-
ogy and the cutoff frequency (position of the poles in the Z-domain) of the ﬁlter. Since
common audio ﬁlters operate between 20 Hz and 20 kHz at a sampling rate of 48 kHz,
the ﬁlter structures are subjected to specially strict criteria with respect to error behavior.
The frequency range for equalizers is between 20 Hz and 4–6 kHz because the human
voice and many musical instruments have their formants in that frequency region. For
given coefﬁcient and signal word-lengths (as in a digital signal processor), a ﬁlter structure
with low roundoff noise for audio application can lead to a suitable solution. For this, the
following second-order ﬁlter structures are compared.
5.2 Recursive Audio Filters
139
The basis of the following considerations is the relationship between the coefﬁcient
sensitivity and roundoff noise. This was ﬁrst stated by Fettweis [Fet72]. By increasing the
pole density in a certain region of the z-plane, the coefﬁcient sensitivity and the roundoff
noise of the ﬁlter structure are reduced. Owing to these improvements, the coefﬁcient
word-length as well as signal word-length can be reduced. Work in designing digital ﬁlters
with minimum word-length for coefﬁcients and state variables was ﬁrst carried out by
Avenhaus [Ave71].
Typical audio ﬁlters like high-/low-pass, peak/shelving ﬁlters can be described by the
second-order transfer function
H (z) = a0 + a1z−1 + a2z−2
1 + b1z−1 + b2z−2 .
(5.88)
The recursive part of the difference equation which can be derived from the transfer func-
tion (5.88) is considered more closely, since it plays a major role in affecting the error
behavior. Owing to the quantization of the coefﬁcients in the denominator in (5.88), the
distribution of poles in the z-plane is restricted (see Fig. 5.27 for 6-bit quantization of
coefﬁcients). The pole distribution in the second quadrant of the z-plane is the mirror image
of the ﬁrst quadrant. Figure 5.28 shows a block diagram of the recursive part. Another
equivalent representation of the denominator is given by
H (z) =
N(z)
1 − 2r cos ϕz−1 + r 2z−2 .
(5.89)
Here r is the radius and ϕ the corresponding phase of the complex poles. By quantizing
these parameters, the pole distribution is altered, in contrast to the case where b1 and b2 are
quantized as in (5.88).
z-plane
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
→
}
z
{
m
I
.
............................
............................
............................
............................
............................
............................
............................
............................
............................
............................
............................
............................
............................
............................
............................
............................
............................
............................
............................
...........................
...........................
...........................
...........................
...........................
...........................
...........................
...........................
...........................
...........................
..........................
..........................
..........................
..........................
..........................
..........................
..........................
..........................
.........................
.........................
.........................
.........................
.........................
.........................
........................
........................
........................
........................
........................
........................
.......................
.......................
.......................
.......................
.......................
.......................
......................
......................
......................
......................
.....................
.....................
.....................
.....................
....................
....................
....................
....................
...................
...................
...................
...................
..................
..................
..................
.................
.................
.................
................
................
................
................
...............
...............
..............
..............
..............
..............
.............
.............
............
............
............
............
...........
...........
..........
..........
.........
.........
.........
.........
........
........
.......
.......
......
......
.....
.....
.....
.....
....
....
...
...
..
..
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
..................
.......................
.......................
..................
.....
............................
.....
............................
......................
......................
...................
...................
............
............
.........
.........
...........................
..........................
...........................
..........................
....
.....................
............................
.....................
................
....................
....................
....
................
............................
........................
........................
..............
.........................
..............
.........................
...
............................
...
............................
........
........
...........................
...........................
..
............................
............................
.................
..
.......................
.......................
.................
...........
...........
..........................
..........................
.
............................
............................
.
........................
...............
............................
...............
.......
.............
.......
............................
.........................
........................
...........................
......................
.............
..................
......................
...........................
..................
.........................
............................
............................
............................
............................
.....................
.....................
...................
...................
............................
............................
............................
..........................
..........
..........
....................
....................
..........................
......
......
...........................
...........................
................
.......................
.......................
................
............
............
.........................
.........................
.....
........................
..............
.....
..............
...........................
........................
...........................
.........
.........
..........................
..........................
-1
-1
.
1
-0.5
0
Re{z} →
0.5
Figure 5.27 Direct-form structure – pole distribution (6-bit quantization).
140
Equalizers
’7(cid:10):
"7(cid:10):
  (cid:5)(cid:22)
  (cid:5)(cid:22)
(cid:5).
(cid:22)
(cid:5).
(cid:23)
Figure 5.28 Direct-form structure – block diagram of recursive part.
The state variable structure [Mul76, Bom85] is based on the approach by Gold and
Rader [Gol67], which is given by
H (z) =
N(z)
1 − 2Re{z∞}z−1 + (Re{z∞}2 + Im{z∞}2)z−2 .
(5.90)
The possible pole locations are shown in Fig. 5.29 for 6-bit quantization (a block diagram
of the recursive part is shown in Fig. 5.30). Owing to the quantization of real and imaginary
parts, a uniform grid of different pole locations results. In contrast to direct quantization of
the coefﬁcients b1 and b2 in the denominator, the quantization of the real and imaginary
parts leads to an increase in the pole density at z = 1. The possible pole locations in the
second quadrant in the z-plane are the mirror images of the ones in the ﬁrst quadrant.
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
.
.
-0.2
-0.4
-0.6
-0.8
........
........
............
............
..............
..............
................
................
..................
..................
...................
...................
....................
....................
-1
-1
......................
......................
.......................
.......................
........................
........................
.........................
.........................
.........................
.........................
..........................
..........................
...........................
...........................
............................
............................
............................
............................
.............................
.............................
.............................
.............................
..............................
..............................
..............................
..............................
...............................
...............................
...............................
...............................
...............................
...............................
...............................
...............................
................................
................................
................................
................................
................................
................................
................................
................................
................................
................................
................................
................................
................................
................................
-0.5
.................................
.................................
................................
................................
................................
................................
................................
................................
................................
................................
................................
................................
................................
................................
................................
................................
...............................
...............................
...............................
...............................
...............................
...............................
...............................
...............................
.........................
.........................
.........................
.........................
........................
........................
.......................
.......................
......................
......................
....................
....................
...................
...................
..................
..................
................
................
..............
..............
............
............
..............................
..............................
..............................
..............................
.............................
.............................
.............................
.............................
............................
............................
............................
............................
...........................
...........................
..........................
..........................
0.5
.
........
........
.
.
.
1
0
Re{z} →
Figure 5.29 Gold and Rader – pole distribution (6-bit quantization).
In [Kin72] a ﬁlter structure is suggested which has a pole distribution as shown in
Fig. 5.31 (for a block diagram of the recursive part, see Fig. 5.32).
5.2 Recursive Audio Filters
141
’7(cid:10):
  (cid:5)(cid:22)
(cid:8)(cid:14)(cid:20)(cid:11)(cid:18)(cid:14)(cid:15)
(cid:8)(cid:14)(cid:18)(cid:16)(cid:10)(cid:14)(cid:15)
(cid:5)(cid:8)(cid:14)(cid:18)(cid:16)(cid:10)(cid:14)(cid:15)
  (cid:5)(cid:22)
(cid:8)(cid:14)(cid:20)(cid:11)(cid:18)(cid:14)(cid:15)
"7(cid:10):
Figure 5.30 Gold and Rader – block diagram of recursive part.
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
..
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
..
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
...........
............
..........
..............
...............
.........
................
.......
..................
......
...................
.....
....................
.....................
...
.......................
..
........................
.........................
.
..........................
.
............................
.
.............................
.
..............................
.
.
...............................
.
.................................
.
..................................
.
.
.
...................................
.
.
....................................
.
.
.....................................
.
.
.
.......................................
.
.
.
........................................
.
.
.
.
.........................................
.
.
.
.
..........................................
.
.
.
.
...........................................
.
.
.
.
............................................
.
.
.
.
.
..............................................
.
.
.
.
.
...............................................
.
.
.
.
.
................................................
.
.
.
.
.
.
.................................................
.
.
.
.
.
.
..................................................
.
.
.
.
.
.
.
...................................................
.
.
.
.
.
.
.
.
....................................................
.
.
.
.
.
.
.
.....................................................
.
.
.
.
.
.
.
......................................................
.
.
.
.
.
.......................................................
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
........................................................
.
.
.
.
.
.
.
.
.
.........................................................
.
.
.
.
.
.
.
..........................................................
.
.
.
.
.
...........................................................
.
............................................................
.............................................................
..............................................................
..............................................................
.............................................................
............................................................
...........................................................
..........................................................
.........................................................
........................................................
.......................................................
......................................................
.....................................................
....................................................
...................................................
..................................................
.................................................
................................................
...............................................
..............................................
.
.
.
............................................
.
...........................................
.
.
..........................................
.
.........................................
.
.
........................................
.
.......................................
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
.....................................
.
.
....................................
.
.
.
...................................
.
.
.
..................................
.
.
.................................
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
...............................
.
.
.
..............................
.
.
.............................
.
.
............................
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
..........................
.
.
.........................
.
.
........................
.
.
.
.
.......................
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
.....................
.
.
.
....................
.
.
...................
.
.
..................
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
................
.
.
...............
.
.
..............
.
.
.
.
.
.
.
............
.
.
...........
.
..........
.
.
.........
.
.
.
.
.
.......
.
......
.
.....
.
.
.
.
...
.
..
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
1
-0.5
0
Re{z} →
0.5
Figure 5.31 Kingsbury – pole distribution (6-bit quantization).
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
Figure 5.32 Kingsbury – block diagram of recursive part.
The corresponding transfer function,
H (z) =
1 − (2 − k1k2 − k2
N(z)
1)z−1 + (1 − k1k2)z−2
,
(5.91)
142
Equalizers
shows that in this case the coefﬁcients b1 and b2 can be obtained by a linear combination
of the quantized coefﬁcients k1 and k2. The distance d of the pole from the point z = 1
determines the coefﬁcients
1 − 2r cos ϕ + r 2,
k1 = d =
k2 = 1 − r 2
k1
,
(5.92)
(5.93)
as illustrated in Fig. 5.33.
(cid:27)(cid:12)^ ‘
-
(cid:8)
(cid:15)
(cid:19)(cid:2)^ ‘
Figure 5.33 Geometric interpretation.
The ﬁlter structures under consideration showed that by a suitable linear combination
of quantized coefﬁcients, any desired pole distribution can be obtained. An increase of the
pole density at z = 1 can be achieved by inﬂuencing the linear relationship between the
coefﬁcient k1 and the distance d from z = 1 [Zöl89, Zöl90]. The nonlinear relationship of
the new coefﬁcients gives the following structure with the transfer function
H (z) =
1 − (2 − z1z2 − z3
N(z)
1)z−1 + (1 − z1z2)z−2
and coefﬁcients
1 + b1 + b2,
(cid:31)
z1 = 3
z2 = 1 − b2
z1
,
with
(cid:31)
z1 = 3
d 2.
(5.94)
(5.95)
(5.96)
(5.97)
The pole distribution of this structure is shown in Fig. 5.34. The block diagram of the
recursive part is illustrated in Fig. 5.35. The increase in the pole density at z = 1, in contrast
to previous pole distributions is observed. The pole distributions of the Kingsbury and
Zölzer structures show a decrease in the pole density for higher frequencies. For the pole
density, a symmetry with respect to the imaginary axis as in the case of the direct-form
structure and the Gold and Rader structure is not possible. But changing the sign in the
recursive part of the difference equation results in a mirror image of the pole density. The
mirror image can be achieved through a change of sign in the denominator polynomial. The
denominator polynomial
D(z) = 1
!(cid:14)(cid:15)(cid:12)(cid:13)
± (2 − z1z2 − z3
1)z
−1 + (1 − z1z2)z
−2
(5.98)
shows that the real part depends on the coefﬁcient of z−1.
