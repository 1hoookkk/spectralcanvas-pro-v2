# Digital Audio DSP (Zölzer) — Chunk 23/30

> Source: `Udo-Zolzer-digital-audio-signal-processing.9780470997857.40435.pdf` · Pages ~1-331 · Extractor: pdfminer.six
> Target ≈ 2800 words, Overlap = 200 words · Actual = 9153

> Note: DSP textbook content for SpectralCanvasPro RT-audio knowledge base

---
Chapter 7
Dynamic Range Control
The dynamic range of a signal is deﬁned as the logarithmic ratio of maximum to minimum
signal amplitude and is given in decibels. The dynamic range of an audio signal lies
between 40 and 120 dB. The combination of level measurement and adaptive signal level
adjustment is called dynamic range control. Dynamic range control of audio signals is
used in many applications to match the dynamic behavior of the audio signal to different
requirements. While recording, dynamic range control protects the AD converter from
overload or is employed in the signal path to optimally use the full amplitude range of
a recording system. For suppressing low-level noise, so-called noise gates are used so that
the audio signal is passed through only from a certain level onwards. While reproducing
music and speech in a car, shopping center, restaurant or disco the dynamics have to match
the special noise characteristics of the environment. Therefore the signal level is measured
from the audio signal and a control signal is derived which then changes the signal level to
control the loudness of the audio signal. This loudness control is adaptive to the input level.
7.1 Basics
Figure 7.1 shows a block diagram of a system for dynamic range control. After measuring
the input level XdB(n), the output level YdB(n) is affected by multiplying the delayed input
signal x(n) by a factor g(n) according to
y(n) = g(n) · x(n − D).
(7.1)
The delay of the signal x(n) compared with the control signal g(n) allows predictive control
of the output signal level. This multiplicative weighting is carried out with corresponding
attack and release time. Multiplication leads, in terms of a logarithmic level representation
of the corresponding signals, to the addition of the weighting level GdB(n) to the input
level XdB(n), giving the output level
YdB(n) = XdB(n) + GdB(n).
(7.2)
Digital Audio Signal Processing Second Edition Udo Zölzer
© 2008 John Wiley & Sons, Ltd
226
Dynamic Range Control
x(n)
X (n)
dB
x(n-D)
z -D
x
PEAK
x
RMS
(n)
(n)
Level
Measurement
Static
Curve
f(n)
Attack/
Release
Time
y(n)
Y (n)=G
dB
g(n)
G (n)
dB
Figure 7.1 System for dynamic range control.
(n)+X (n)
dB
dB
7.2 Static Curve
The relationship between input level and weighting level is deﬁned by a static level curve
GdB(n) = f (XdB(n)). An example of such a static curve is given in Fig. 7.2. Here, the
output level and the weighting level are given as functions of the input level.
X/dB
X/dB
-90 -80 -70 -60 -50 -40 -30 -20 -10 0
CT
(cid:24)
C
LT
(cid:24)
L
-CS(LT-CT)
ET
(cid:24)
E
NT
ES(ET-NT)
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
Y/dB
-90 -80 -70 -60 -50 -40 -30 -20 -10 0
NT
ET
CT
LT
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
G/dB
Figure 7.2 Static curve with the parameters (LT = limiter threshold, CT = compressor threshold,
ET = expander threshold and NT = noise gate threshold).
With the help of a limiter, the output level is limited when the input level exceeds
the limiter threshold LT. All input levels above this threshold lead to a constant output
level. The compressor maps a change of input level to a certain smaller change of output
level. In contrast to a limiter, the compressor increases the loudness of the audio signal.
The expander increases changes in the input level to larger changes in the output level.
With this, an increase in the dynamics for low levels is achieved. The noise gate is used
to suppress low-level signals, for noise reduction and also for sound effects like truncating
the decay of room reverberation. Every threshold used in particular parts of the static curve
is deﬁned as the lower limit for limiter and compressor and upper limit for expander and
noise gate.
In the logarithmic representation of the static curve the compression factor R (ratio) is
deﬁned as the ratio of the input level change (cid:8)PI to the output level change (cid:8)PO:
R = (cid:8)PI
(cid:8)PO
.
(7.3)
7.2 Static Curve
227
With the help of Fig. 7.3 the straight line equation YdB(n) = CT + R−1(XdB(n) − CT) and
the compression factor
R = XdB(n) − CT
YdB(n) − CT
= tan βC
(7.4)
are obtained, where the angle β is deﬁned as shown in Fig. 7.2. The relationship between
the ratio R and the slope S can also be derived from Fig. 7.3 and is expressed as
S = 1 − 1
R
R = 1
1 − S
,
.
X/dB
X/dB
-90 -80 -70 -60 -50 -40 -30 -20 -10 0
CT=30 dB
CR=3/1
CS=1-1/3=2/3
S
1
1/R
1
0
-10
-20
-30
-40
-50
-60
Y/dB
-90 -80 -70 -60 -50 -40 -30 -20 -10 0
CT
0
-10
-20
-30
-40
-50
-60
G/dB
Figure 7.3 Compressor curve (compressor ratio CR/slope CS).
Typical compression factors are
R = ∞,
R > 1,
0 < R < 1,
R = 0,
limiter,
compressor (CR: compressor ratio),
expander (ER: expander ratio),
noise gate.
The transition from logarithmic to linear representation leads, from (7.4), to
R =
log10
log10
ˆx(n)
cT
ˆy(n)
cT
,
(7.5)
(7.6)
(7.7)
(7.8)
where ˆx(n) and ˆy(n) are the linear levels and cT denotes the linear compressor threshold.
Rewriting (7.8) gives the linear output level
= 101/R log10( ˆx(n)/cT ) =
ˆy(n)
cT
ˆy(n) = c1−1/R
T
(cid:2)
1/R
(cid:1)
ˆx(n)
cT
· ˆx1/R(n)
(7.9)
as a function of input level. The control factor g(n) can be calculated by the quotient
g(n) =
ˆy(n)
ˆx(n)
=
(cid:1)
ˆx(n)
cT
(cid:2)
1/R−1
.
(7.10)
228
Dynamic Range Control
With the help of tables and interpolation methods, it is possible to determine the control
factor without taking logarithms and antilogarithms. The implementation described as fol-
lows, however, makes use of the logarithm of the input level and calculates the control
level GdB(n) with the help of the straight line equation. The antilogarithm leads to the
value f (n) which gives the control factor g(n) with corresponding attack and release time
(see Fig. 7.1).
7.3 Dynamic Behavior
Besides the static curve of dynamic range control, the dynamic behavior in terms of attack
and release times plays a signiﬁcant role in sound quality. The rapidity of dynamic range
control depends also on the measurement of PEAK and RMS values [McN84, Sti86].
7.3.1 Level Measurement
Level measurements [McN84] can be made with the systems shown in Figs 7.4 and 7.5.
For PEAK measurement, the absolute value of the input is compared with the peak value
xPEAK(n). If the absolute value is greater than the peak value, the difference is weighted
with the coefﬁcient AT (attack time) and added to (1 − AT) · xPEAK(n − 1). For this attack
case |x(n)| > xPEAK(n − 1) we get the difference equation (see Fig. 7.4)
xPEAK(n) = (1 − AT) · xPEAK(n − 1) + AT · |x(n)|
with the transfer function
H (z) =
AT
1 − (1 − AT)z−1 .
(7.11)
(7.12)
If the absolute value of the input is smaller than the peak value |x(n)| ≤ xPEAK(n − 1) (the
release case), the new peak value is given by
xPEAK(n) = (1 − RT) · xPEAK(n − 1)
(7.13)
with the release time coefﬁcient RT. The difference signal of the input will be muted by
the nonlinearity such that the difference equation for the peak value is given according to
(7.13). For the release case the transfer function
H (z) =
1
1 − (1 − RT)z−1
(7.14)
is valid. For the attack case the transfer function (7.12) with coefﬁcient AT is used and for
the release case the transfer function (7.14) with the coefﬁcient RT. The coefﬁcients (see
Section 7.3.3) are given by
(cid:1)
AT = 1 − exp
(cid:1)
RT = 1 − exp
(cid:2)
,
(cid:2)
,
−2.2TS
ta/1000
−2.2TS
tr /1000
(7.15)
(7.16)
7.3 Dynamic Behavior
229
where the attack time ta and the release time tr are given in milliseconds (TS sampling
interval). With this switching between ﬁlter structures one achieves fast attack responses
for increasing input signal amplitudes and slow decay responses for decreasing input signal
amplitudes.
x(n)
|x(n)|
_
AT
_
x
PEAK
(n)
0 for
RT
|x(n)|>x
for |x(n)|<x
PEAK
(n)
PEAK
(n)
z-1
x2
RMS(n)
Figure 7.4 PEAK measurement.
TAV
x(n)
x2(n)
_
z-1
Figure 7.5 RMS measurement (TAV = averaging coefﬁcient).
The computation of the RMS value
xRMS(n) =
(cid:28)
(cid:29)
(cid:29)
(cid:30) 1
N
N−1(cid:6)
i=0
x2(n − i)
(7.17)
over N input samples can be achieved by a recursive formulation. The RMS measurement
shown in Fig. 7.5 uses the square of the input and performs averaging with a ﬁrst-order
low-pass ﬁlter. The averaging coefﬁcient
(cid:1)
TAV = 1 − exp
(cid:2)
−2.2TA
tM /1000
(7.18)
is determined according to the time constant calculation discussed in Section 7.3.3, where
tM is the averaging time in milliseconds. The difference equation is given by
RMS(n) = (1 − TAV) · x2
x2
RMS(n − 1) + TAV · x2(n)
with the transfer function
H (z) =
TAV
1 − (1 − TAV)z−1 .
(7.19)
(7.20)
230
Dynamic Range Control
7.3.2 Gain Factor Smoothing
Attack and release times can be implemented by the system shown in Fig. 7.6 [McN84].
The attack coefﬁcient AT or release coefﬁcient RT is obtained by comparing the input
control factor and the previous one. A small hysteresis curve determines whether the control
factor is in the attack or release state and hence gives the coefﬁcient AT or RT. The system
also serves to smooth the control signal. The difference equation is given by
g(n) = (1 − k) · g(n − 1) + k · f (n),
with k = AT or k = RT, and the corresponding transfer function leads to
H (z) =
k
1 − (1 − k)z−1 .
%7(cid:10):
  (cid:5)(cid:22)
&+
(cid:19)+
(7.21)
(7.22)
(cid:3)7(cid:10):
  (cid:5)(cid:22)
Figure 7.6 Implementing attack and release time or gain factor smoothing.
7.3.3 Time Constants
If the step response of a continuous-time system is
g(t) = 1 − e
−t /τ ,
τ = time constant,
(7.23)
then sampling (step-invariant transform) the step response gives the discrete-time step
response
g(nTS) = ε(nTS) − e
−nTS /τ = 1 − zn
∞,
z∞ = e
−TS /τ .
(7.24)
The Z-transform leads to
G(z) = z
z − 1
−
1
1 − z∞z−1
=
1 − z∞
(z − 1)(1 − z∞z−1)
.
With the deﬁnition of attack time ta = t90 − t10, we derive
0.1 = 1 − e
0.9 = 1 − e
−t10/τ ← t10 = 0.1τ,
−t90/τ ← t90 = 0.9τ.
(7.25)
(7.26)
(7.27)
7.4 Implementation
231
The relationship between attack time ta and the time constant τ of the step response is
obtained as follows:
0.9/0.1 = e(t90−t10)/τ
ln(0.9/0.1) = (t90 − t10)/τ
ta = t90 − t10 = 2.2τ.
Hence, the pole is calculated as
z∞ = e
−2.2TS/ta .
(7.28)
(7.29)
A system for implementing the given step response is obtained by the relationship between
the Z-transform of the impulse response and the Z-transform of the step response:
H (z) = z − 1
z
(7.30)
G(z).
The transfer function can now be written as
H (z) = (1 − z∞)z−1
1 − z∞z−1
(7.31)
with the pole z∞ = e−2.2TS/ta adjusting the attack, release or averaging time. For the co-
efﬁcients of the corresponding time constant ﬁlters the attack case is given by (7.15), the
release case by (7.16) and the averaging case by (7.18). Figure 7.7 shows an example where
the dotted lines mark the t10 and t90 time.
7.4 Implementation
The programming of a system for dynamic range control is described in the following
sections.
7.4.1 Limiter
The block diagram of a limiter is presented in Fig. 7.8. The signal xPEAK(n) is determined
from the input with variable attack and release time. The logarithm to the base 2 of this peak
signal is taken and compared with the limiter threshold. If the signal is above the threshold,
the difference is multiplied by the negative slope of the limiter LS. Then the antilogarithm
of the result is taken. The control factor f (n) obtained is then smoothed with a ﬁrst-order
low-pass ﬁlter (SMOOTH). If the signal xPEAK(n) lies below the limiter threshold, the
signal f (n) is set to f (n) = 1. The delayed input x(n − D1) is multiplied by the smoothed
control factor g(n) to give the output y(n).
7.4.2 Compressor, Expander, Noise Gate
The block diagram of a compressor/expander/noise gate is shown in Fig. 7.9. The basic
structure is similar to the limiter. In contrast to the limiter, the logarithm of the signal
232
Dynamic Range Control
 = 80.00 ms
 = 10.00 ms, t
t
r
a
1
0.8
0.6
0.4
0.2
0
0
1
0.5
0
(cid:358)0.5
(cid:358)1
0
x(n)
x
PEAK
(n)
20
40
60
80
100
t in ms (cid:111)
120
140
160
180
 = 200.00 ms
 = 0.20 ms, t
t
r
a
x(n)
x
PEAK
(n)
200
400
600
800
1000
t in ms (cid:111)
1200
1400
1600
1800
Figure 7.7 Attack and release behavior for time-constant ﬁlters.
’7(cid:10):
(cid:26)(cid:30)(cid:15)(cid:14)(cid:22)
’7(cid:10)(cid:5)(cid:26)(cid:14)(cid:14):(cid:22)
"7(cid:10):
(cid:15)+
(cid:15)(cid:6)
’(cid:14)(cid:14)(cid:14)(cid:14)(cid:14)(cid:14)(cid:14)7(cid:10):
(cid:1)(cid:30)&N
(cid:1)(cid:30)&N
&+1(cid:19)+
(cid:4)(cid:11)(cid:3)
(cid:23)
Figure 7.8 Limiter.
(cid:3)7(cid:10):
%7(cid:10):
#
(cid:23)
(cid:6)/((+=
xRMS(n) is taken and multiplied by 0.5. The value obtained is compared with three thresh-
olds in order to determine the operating range of the static curve. If one of the three
thresholds is crossed, the resulting difference is multiplied by the corresponding slope
(CS, ES, NS) and the antilogarithm of the result is taken. A ﬁrst-order low-pass ﬁlter
subsequently provides the attack and release time.
7.4 Implementation
’7(cid:10):
’7(cid:10)(cid:5)(cid:26)(cid:14)(cid:14):(cid:23)
(cid:26)(cid:30)(cid:15)(cid:14)(cid:23)
233
"7(cid:10):
’(cid:14)(cid:14)(cid:14)(cid:14)(cid:14)7(cid:10):
(cid:19)/(cid:6)
$+
$(cid:6)
(cid:19)/(cid:6)
3(cid:29)5
(cid:4)(cid:11)(cid:3)
(cid:23)
(cid:3)7(cid:10):
%7(cid:10):
#
(cid:23)
&+
(cid:19)+
(cid:30)+
(cid:30)(cid:6)
(cid:19)(cid:9)(cid:10)(cid:3)(cid:2)(cid:14)(cid:26)(cid:2)(cid:7)(cid:2)(cid:20)(cid:7)(cid:11)(cid:8)
(cid:24)+
(cid:24)(cid:6)
(cid:30)(cid:6)7(cid:30)+(cid:5)(cid:24)+:
(cid:130)3(cid:130)
Figure 7.9 Compressor/expander/noise gate.
’7(cid:10):
’7(cid:10)(cid:5)(cid:26):
(cid:26)(cid:30)(cid:15)&*
"7(cid:10):
(cid:3)7(cid:10):
’(cid:14)(cid:14)(cid:14)(cid:14)(cid:14)(cid:14)7(cid:10):
(cid:1)(cid:30)&N
(cid:1)(cid:30)&N
(cid:4)(cid:11)(cid:3)
(cid:23)
(cid:19)/(cid:6)
’(cid:14)(cid:14)(cid:14)(cid:14)(cid:14)7(cid:10):
(cid:19)/(cid:6)
3(cid:29)5
(cid:15)+
(cid:5)(cid:15)(cid:6) (cid:5)$(cid:6)7(cid:15)+(cid:5)$+:
$+
(cid:5)$(cid:6)
%7(cid:10):
#
(cid:23)
(cid:14)(cid:14)(cid:14)&+1(cid:19)+
(cid:6)/((+=
(cid:30)+
(cid:30)(cid:6)
(cid:19)(cid:9)(cid:10)(cid:3)(cid:2)(cid:14)(cid:26)(cid:2)(cid:7)(cid:2)(cid:20)(cid:7)(cid:11)(cid:8)
(cid:24)+
(cid:24)(cid:6)
(cid:30)(cid:6)7(cid:30)+(cid:5)(cid:24)+:
(cid:130)3(cid:130)
Figure 7.10 Limiter/compressor/expander/noise gate.
7.4.3 Combination System
A combination of a limiter that uses PEAK measurement, and a compressor/expander/noise
gate that is based on RMS measurement, is presented in Fig. 7.10. The PEAK and RMS
234
Dynamic Range Control
values are measured simultaneously. If the linear threshold of the limiter is crossed, the
logarithm of the peak signal xPEAK(n) is taken and the upper path of the limiter is used
to calculate the characteristic curve. If the limiter threshold is not crossed, the logarithm
of the RMS value is taken and one of the three lower paths is used. The additive terms in
the limiter and noise gate paths result from the static curve. After going through the range
detector, the antilogarithm is taken. The sequence f (n) is smoothed with a SMOOTH ﬁlter
in the limiter case, or weighted with corresponding attack and release times of the relevant
operating range (compressor, expander or noise gate). By limiting the maximum level, the
dynamic range is reduced. As a consequence, the overall static curve can be shifted up by
a gain factor. Figure 7.11 demonstrates this with a gain factor equal to 10 dB. This static
parameter value is directly included in the control factor g(n).
X/dB
X/dB
-90 -80 -70 -60 -50 -40 -30 -20 -10 0
LT
Gain
CT
ET
NT
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
Y/dB
-90 -80 -70 -60 -50 -40 -30 -20 -10 0
NT
ET
CT
LT
+10
0
-10
-20
-30
-40
-50
-60
-70
-80
G/dB
Figure 7.11 Shifting the static curve by a gain factor.
As an example, Fig. 7.12 illustrates the input x(n), the output y(n) and the control factor
g(n) of a compressor/expander system. It is observed that signals with high amplitude are
compressed and those with low amplitude are expanded. An additional gain of 12 dB
shows the maximum value of 4 for the control factor g(n). The compressor/expander
system operates in the linear region of the static curve if the control factor is equal to
4. If the control factor is between 1 and 4, the system operates as a compressor. For
control factors lower than 1, the system works as an expander (3500 < n <4500 and
6800 < n <7900). The compressor is responsible for increasing the loudness of the signal,
whereas the expander increases the dynamic range for signals of small amplitude.
7.5 Realization Aspects
7.5.1 Sampling Rate Reduction
In order to reduce the computational complexity, downsampling can be carried out after
calculating the PEAK/RMS value (see Fig. 7.13). As the signals xPEAK(n) and xRMS(n) are
already band-limited, they can be directly downsampled by taking every second or fourth
value of the sequence. This downsampled signal is then processed by taking its logarithm,
calculating the static curve, taking the antilogarithm and ﬁltering with corresponding attack
7.5 Realization Aspects
235
Input  x(n)
1000
2000
3000
4000
5000
n
Output y(n)
6000
7000
8000
9000
10000
1000
2000
3000
4000
5000
n
6000
7000
8000
9000
10000
Gain Factor g(n)
)
n
(
x
)
n
(
y
)
n
(
g
1
0.5
0
-0.5
-1
1
0.5
0
-0.5
-1
4
3
2
1
0
1000
2000
3000
4000
5000
6000
7000
8000
9000
10000
n
Figure 7.12 Signals x(n), y(n) and g(n) for dynamic range control.
and release time with reduced sampling rate. The following upsampling by a factor of
4 is achieved by repeating the output value four times. This procedure is equivalent to
upsampling by a factor of 4 followed by a sample-and-hold transfer function.
The nesting and spreading of partial program modules over four sampling periods is
shown in Fig. 7.14. The modules PEAK/RMS (i.e. PEAK/RMS calculation) and MULT
(delay of input and multiplication with g(n)) are performed every input sampling period.
The number of processor cycles for PEAK/RMS and MULT are denoted by Z1 and Z3
respectively. The modules LD(X), CURVE, 2x and SMO have a maximum of Z2 processor
236
’7(cid:10):
’(cid:14)(cid:14)(cid:14)(cid:14)(cid:14)(cid:14)(cid:14)7(cid:10):
(cid:1)(cid:30)&N
(cid:1)(cid:30)&N
<
(cid:4)(cid:11)(cid:3)
(cid:23)
3(cid:29)5
(cid:19)/(cid:6)
’(cid:14)(cid:14)(cid:14)(cid:14)(cid:14)(cid:14)7(cid:10):
(cid:19)/(cid:6)
Dynamic Range Control
"7(cid:10):
(cid:3)7(cid:10):
’7(cid:10)(cid:5)(cid:26):
(cid:26)(cid:30)(cid:15)&*
(cid:15)+
(cid:5)(cid:15)(cid:6)
(cid:5)$(cid:6)7(cid:15)+(cid:5)$+:
$+
(cid:5)$(cid:6)
#
(cid:23)
&+1(cid:19)+
(cid:6)/((+=
<
(cid:30)+
(cid:30)(cid:6)
(cid:19)(cid:9)(cid:10)(cid:3)(cid:2)(cid:14)(cid:26)(cid:2)(cid:7)(cid:2)(cid:20)(cid:7)(cid:11)(cid:8)
(cid:24)+
(cid:24)(cid:6) (cid:30)(cid:6)7(cid:30)+(cid:5)(cid:24)+:
(cid:130)3(cid:130)
Figure 7.13 Dynamic system with sampling rate reduction.
cycles and are processed consecutively in the given order. This procedure is repeated every
four sampling periods. The total number of processor cycles per sampling period for the
complete dynamics algorithm results from the sum of all three modules.
3
(cid:22)
(cid:23)
2
<
5
+
(cid:19)/(cid:6)
(cid:1)(cid:30)&N
(cid:19)/(cid:6)
(cid:1)(cid:30)&N
(cid:19)/(cid:6)
(cid:1)(cid:30)&N
(cid:19)/(cid:6)
(cid:1)(cid:30)&N
(cid:19)/(cid:6)
(cid:1)(cid:30)&N
(cid:15)(cid:26)70:
$!(cid:19)L(cid:30)
’
(cid:23)
(cid:6)/(
(cid:15)(cid:26)70:
/!(cid:15)+
/!(cid:15)+ /!(cid:15)+ /!(cid:15)+
/!(cid:15)+
H(cid:22)
H(cid:23)
H2
$"(cid:20)(cid:4)(cid:2)(cid:18)
Figure 7.14 Nesting technique.
7.5.2 Curve Approximation
Besides taking logarithms and antilogarithms, other simple operations like comparisons and
addition/multiplication occur in calculating the static curve. The logarithm of the
7.6 Java Applet – Dynamic Range Control
PEAK/RMS value is taken as follows:
x = M · 2E,
ld(x) = ld(M) + E.
237
(7.32)
(7.33)
First, the mantissa is normalized and the exponent is determined. The function ld(M) is
then calculated by a series expansion. The exponent is simply added to the result.
The logarithmic weighting factor G and the antilogarithm 2G are given by
G = − E − M,
−M .
−E · 2
2G = 2
(7.34)
(7.35)
Here, E is a natural number and M is a fractional number. The antilogarithm 2G is calcu-
lated by expanding the function 2−M in a series and multiplying by 2−E. A reduction of
computational complexity can be achieved by directly using log and antilog tables.
7.5.3 Stereo Processing
For stereo processing, a common control factor g(n) is needed. If different control factors
are used for both channels, limiting or compressing one of the two stereo signals causes a
displacement of the stereo balance. Figure 7.15 shows a stereo dynamic system in which the
sum of the two signals is used to calculate a common control factor g(n). The following
processing steps of measuring the PEAK/RMS value, downsampling, taking logarithm,
calculating static curve, taking antilog attack and release time and upsampling with a
sample-and-hold function remain the same. The delay (DEL) in the direct path must be
the same for both channels.
’(cid:14)(cid:14)(cid:14)7(cid:10):(cid:19)
(cid:26)(cid:30)(cid:15)(cid:14)
"(cid:14)(cid:14)(cid:14)7(cid:10):(cid:19)
(cid:1)(cid:30)&N
(cid:19)/(cid:6)
<
(cid:4)(cid:11)(cid:3)
(cid:23)
(cid:27)
$
+
&
+
(cid:6)
(cid:30)
L
(cid:19)
!
$
#
(cid:23)
&+
(cid:19)+
<
(cid:3)7(cid:10):
’(cid:14)(cid:14)(cid:14)7(cid:10):
(cid:15)
(cid:26)(cid:30)(cid:15)(cid:14)
"(cid:14)(cid:14)(cid:14)7(cid:10):
(cid:15)
Figure 7.15 Stereo dynamic system.
7.6 Java Applet – Dynamic Range Control
The applet shown in Fig. 7.16 demonstrates dynamic range control. It is designed for a
ﬁrst insight into the perceptual effects of dynamic range control of an audio signal. You
238
Dynamic Range Control
can adjust the characteristic curve with two control points. You can choose between two
predeﬁned audio ﬁles from our web server (audio1.wav or audio2.wav) or your own local
wav ﬁle to be processed [Gui05].
Figure 7.16 Java applet – dynamic range control.
7.7 Exercises
1. Low-pass Filtering for Envelope Detection
Generally, envelope computation is performed by low-pass ﬁltering the input signal’s ab-
solute value or its square.
1. Sketch the block diagram of a recursive ﬁrst-order low-pass H (z) = λ/[(1 − (1 −
λ)z−1)].
2. Sketch its step response. What characteristic measure of the envelope detector can
be derived from the step response and how?
3. Typically, the low-pass ﬁlter is modiﬁed to use a non-constant ﬁlter coefﬁcient λ.
How does λ depend on the signal? Sketch the response to a rect signal of the low-
pass ﬁlter thus modiﬁed.
2. Discrete-time Specialties of Envelope Detection
Taking absolute value or squaring are non-linear operations. Therefore, care must be taken
when using them in discrete-time systems as they introduce harmonics the frequency of
which may violate the Nyquist bound. This can lead to unexpected results, as a simple
example illustrates. Consider the input signal x(n) = sin( π
2 n + ϕ), ϕ ∈ [0, 2π].
References
239
1. Sketch x(n), |x(n)| and x2(n) for different values of ϕ.
2. Determine the value of the envelope after perfect low-pass ﬁltering, i.e. averaging,
|x(n)|. Note: As the input signal is periodical, it is sufﬁcient to consider one pe-
riod, e.g.
¯x = 1
4
3(cid:6)
n=0
|x(n)|.
3. Similarly, determine the value of the envelope after averaging x2(n).
3. Dynamic Range Processors
Sketch the characteristic curves mapping input level to output level and input level to gain
for and describe brieﬂy the application of:
1. limiter;
2. compressor;
3. expander;
4. noise gate.
References
[Gui05] M. Guillemard, C. Ruwwe, U. Zölzer: J-DAFx – Digital Audio Effects in Java,
Proc. 8th Int. Conference on Digital Audio Effects (DAFx-05), pp. 161–166,
Madrid, 2005.
[McN84] G. W. McNally: Dynamic Range Control of Digital Audio Signals, J. Audio
Eng. Soc., Vol. 32, pp. 316–327, 1984.
[Sti86]
E. Stikvoort: Digital Dynamic Range Compressor for Audio, J. Audio Eng.
Soc., Vol. 34, pp. 3–9, 1986.

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
