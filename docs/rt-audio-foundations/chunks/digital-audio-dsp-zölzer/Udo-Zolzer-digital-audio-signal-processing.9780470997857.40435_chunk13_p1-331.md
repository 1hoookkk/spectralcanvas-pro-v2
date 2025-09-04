# Digital Audio DSP (Zölzer) — Chunk 13/30

> Source: `Udo-Zolzer-digital-audio-signal-processing.9780470997857.40435.pdf` · Pages ~1-331 · Extractor: pdfminer.six
> Target ≈ 2800 words, Overlap = 200 words · Actual = 4978

> Note: DSP textbook content for SpectralCanvasPro RT-audio knowledge base

---
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

5.2 Recursive Audio Filters
