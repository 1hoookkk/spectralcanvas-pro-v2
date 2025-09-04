# Digital Audio DSP (Zölzer) — Chunk 20/30

> Source: `Udo-Zolzer-digital-audio-signal-processing.9780470997857.40435.pdf` · Pages ~1-331 · Extractor: pdfminer.six
> Target ≈ 2800 words, Overlap = 200 words · Actual = 2218

> Note: DSP textbook content for SpectralCanvasPro RT-audio knowledge base

---
202
Room Simulation
Another basis of the Schroeder algorithm is the all-pass ﬁlter shown in Fig. 6.14 with
transfer function
H (z) = z−M − g
1 − gz−M
= z−M
1 − gz−M
−
g
1 − gz−M .
(6.45)
(6.46)
From (6.46) it can be seen that the impulse response can also be expressed as a summation
of cosine oscillations.
’7(cid:10):
(cid:3)
(cid:5)/
(cid:5)(cid:3)
"7(cid:10):
Figure 6.14 All-pass ﬁlter (M = delay length).
The impulse responses and frequency responses of a comb ﬁlter and an all-pass ﬁlter are
presented in Fig. 6.15. Both impulse responses show an exponential decay. A sample in the
impulse response occurs every M sampling periods. The density of samples in the impulse
responses does not increase with time. For the recursive comb ﬁlter, spectral shaping due
to the maxima at the corresponding poles of the transfer function is observed.
Frequency Density
The frequency density describes the number of eigenfrequencies per hertz and is deﬁned
for a comb ﬁlter [Jot91] as
(6.47)
A single comb ﬁlter gives M resonances in the interval [0, 2π], which are separated by a
frequency distance of (cid:8)f = fS/M. In order to increase the frequency density, a parallel
circuit (see Fig. 6.16) of P comb ﬁlters is used which leads to
[1/Hz].
Df = M · TS
H (z) =
P(cid:6)
p=1
z−Mp
1 − gpz−Mp
(cid:4)
=
z−M1
1 − g1z−M1
+
z−M2
1 − g2z−M2
(cid:5)
.
+ · · ·
The choice of the delay systems [Schr62] is suggested as
M1 : MP = 1 : 1.5
and leads to a frequency density
Df =
P(cid:6)
p=1
Mp · TS = P · M · TS.
(6.48)
(6.49)
(6.50)

6.3 Subsequent Reverberation

203
a)
b)
→
)
n
(
h
→
)
S
f
/
f
(
H
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
2.5
2
1.5
1
0.5
0
0
20
40
60
80
100
n →
c)
0 0.05 0.1 0.15 0.2 0.25 0.3 0.35 0.4 0.45 0.5
f/fS
→
→
)
n
(
h
→
)
S
f
/
f
(
H
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
2.5
2
1.5
1
0.5
0
0
20
40
60
80
100
n →
d)
0 0.05 0.1 0.15 0.2 0.25 0.3 0.35 0.4 0.45 0.5
f/fS
→
Figure 6.15 (a) Impulse response of a comb ﬁlter (M = 10, g = −0.6). (b) Impulse response of an
all-pass ﬁlter (M = 10, g = −0.6). (c) Frequency response of a comb ﬁlter. (d) Frequency response
of an all-pass ﬁlter.
In [Schr62] a necessary frequency density of Df = 0.15 eigenfrequencies per hertz is
proposed.
Echo Density
The echo density is the number of reﬂections per second and is deﬁned for a comb ﬁlter
[Jot91] as
Dt = 1
M · TS
[1/s].
For a parallel circuit of comb ﬁlters, the echo density is given by
Dt =
P(cid:6)
p=1
1
Mp · TS
= P
1
M · TS
.
(6.51)
(6.52)
204
Room Simulation
’7(cid:10):
"7(cid:10):
(cid:5)/
(cid:22)
(cid:3)
(cid:22)
(cid:5)/
(cid:23)
(cid:3)
(cid:23)
(cid:5)/
2
(cid:3)
2
Figure 6.16 Parallel circuit of comb ﬁlters.
With (6.50) and (6.52), the number of parallel comb ﬁlters and the mean delay length are
given by
P =
MTS =
(cid:31)
(cid:31)
Df · Dt ,
Df /Dt .
(6.53)
(6.54)
For a frequency density Df = 0.15 and an echo density Dt = 1000 it can be concluded that
the number of parallel comb ﬁlters is P = 12 and the mean delay length is MTS = 12 ms.
Since the frequency density is proportional to the reverberation time, the number of parallel
comb ﬁlters has to be increased accordingly.
A further increase in the echo density is achieved by a cascade circuit of PA all-pass
ﬁlters (see Fig. 6.17) with transfer function
H (z) =
PA%
p=1
z−Mp − gp
1 − gpz−Mp
.
(6.55)
These all-pass sections are connected in series with the parallel circuit of comb ﬁlters. For
a sufﬁcient echo density, 10000 reﬂections per second are necessary [Gri89].
Avoiding Unnatural Resonances
Since the impulse response of a single comb ﬁlter can be described as a sum of M (delay
length) decaying sinusoidal oscillations, the short-time FFT of consecutive parts from this
impulse response gives the frequency response shown in Fig. 6.18 in the time-frequency
domain. Only the maxima are presented. The parallel circuit of comb ﬁlters with the con-
1/Mp
dition (6.49) leads to radii of pole distribution as given by rp = g
(p = 1, 2, . . . , P ).
p
In order to avoid unnatural resonances, the radii of the pole distribution of a parallel circuit

6.3 Subsequent Reverberation

’7(cid:10):
(cid:3)
(cid:22)
(cid:5)/
(cid:22)
(cid:5)(cid:3)
(cid:22)
(cid:3)
(cid:23)
(cid:5)/
(cid:23)
(cid:5)(cid:3)
(cid:23)
205
"7(cid:10):
(cid:3)
2
(cid:5)/
2
(cid:5)(cid:3)
2
Figure 6.17 Cascade circuit of all-pass ﬁlters.
of comb ﬁlters must satisfy the condition
1/Mp
rp = const. = g
p
,
for p = 1, 2, . . . , P .
(6.56)
This leads to the short-time spectra and the pole distribution as shown in Fig. 6.19.
Figure 6.20 shows the impulse response and the echogram (logarithmic presentation of
the amplitude of the impulse response) of a parallel circuit of comb ﬁlters with equal and
unequal pole radii. For unequal pole radius, the different decay times of the eigenfrequen-
cies can be seen.
=7%8(cid:7):
(cid:27)(cid:12)^ ‘
%
(cid:19)(cid:2)^ ‘
(cid:7)
Figure 6.18 Short-time spectra of a comb ﬁlter (M = 8).
Reverberation Time
The reverberation time of a recursive comb ﬁlter can be adjusted with the feedback factor
g which describes the ratio
g = h(n)
h(n − M)
(6.57)
of two different nonzero samples of the impulse response separated by M sampling periods.
The factor g describes the decay constant per M samples. The decay constant per sampling
period can be calculated from the pole radius r = g1/M and is deﬁned as
r = h(n)
h(n − 1)
.
(6.58)

206
Room Simulation
=7%8(cid:7):
(cid:27)(cid:12)^ ‘
%
(cid:19)(cid:2)^ ‘
(cid:7)
→
)
n
(
h
→
B
d
n
i
)
n
(
h
1
0.5
0
-0.5
-1
0
0
-20
-40
-60
-80
-100
0
Figure 6.19 Short-time spectra of a parallel circuit of comb ﬁlters.
Impulse Response (equal pole radius)
Impulse Response (unequal pole radius)
→
)
n
(
h
1
0.5
0
-0.5
-1
0
100
200
300
400
500
n →
100
200
300
400
500
n →
Echogram (equal pole radius)
Echogram (unequal pole radius)
→
B
d
n
i
)
n
(
h
0
-20
-40
-60
-80
-100
0
100
200
300
400
500
n →
100
200
300
400
500
n →
Figure 6.20 Impulse response and echogram.
The relationship between feedback factor g and pole radius r can also be expressed using
(6.57) and (6.58) and is given by
g = h(n)
h(n − M)
= h(n)
h(n − 1)
· h(n − 1)
h(n − 2)
· · · h(n − (M − 1))
h(n − M)
= r · r · r · · ·r = r M .
(6.59)
6.3 Subsequent Reverberation
207
1/Mp
With the constant radius r = g
p
Gp = 20 log10 gp, the attenuation per sampling period is given by
and the logarithmic parameters R = 20 log10 r and
R = Gp
Mp
.
(6.60)
The reverberation time is deﬁned as decay time of the impulse response to −60 dB. With
−60/T60 = R/TS, the reverberation time can be written as
T60 = −60
TS
R
= −60
TSMp
Gp
=
3
|1/gp| Mp · TS.
log10
(6.61)
The control of reverberation time can either be carried out with the feedback factor g or
the delay parameter M. The increase in the reverberation time with factor g is responsible
for a pole radius close to the unit circle and, hence, leads to an ampliﬁcation of maxima of
the frequency response (see (6.43)). This leads to a coloring of the sound impression. The
increase in the delay parameter M, on the other hand, leads to an impulse response whose
nonzero samples are far apart from each other, so that individual echoes can be heard. The
discrepancy between echo density and frequency density for a given reverberation time can
be solved by a sufﬁcient number of parallel comb ﬁlters.
Frequency-dependent Reverberation Time
The eigenfrequencies of rooms have a rapid decay for high frequencies. A frequency-
dependent reverberation time can be implemented with a low-pass ﬁlter
H1(z) =
1
1 − az−1
(6.62)
in the feedback loop of a comb ﬁlter. The modiﬁed comb ﬁlter in Fig. 6.21 has transfer
function
(6.63)
(6.64)
H (z) =
z−M
1 − gH1(z)z−M
with the stability criterion
g
1 − a
< 1.
’7(cid:10):
(cid:5)/
"7(cid:10):
(cid:3)
(cid:9)
(cid:5)(cid:22)
Figure 6.21 Modiﬁed low-pass comb ﬁlter.

208
Room Simulation
The short-time spectra and the pole distribution of a parallel circuit with low-pass comb
ﬁlters are presented in Fig. 6.22. Low eigenfrequencies decay more slowly than higher ones.
The circular pole distribution becomes an elliptical distribution where the low-frequency
poles are moved toward the unit circle.
=7%8(cid:7):
(cid:27)(cid:12)^ ‘
%
(cid:19)(cid:2)^ ‘
(cid:7)
Figure 6.22 Short-time spectra of a parallel circuit of low-pass comb ﬁlters.
Stereo Room Simulation
An extension of the Schroeder algorithm was suggested by Moorer [Moo78]. In addition to
a parallel circuit of comb ﬁlters in series with a cascade of all-pass ﬁlters, a pattern of early
reﬂections is generated. Figure 6.23 shows a room simulation system for a stereo signal.
The generated room signals eL(n) and eR(n) are added to the direct signals xL(n) and
xR(n). The input of the room simulation is the mono signal xM (n) = xL(n) + xR(n) (sum
signal). This mono signal is added to the left and right room signals after going through
a delay line DEL1. The total sum of all reﬂections is fed via another delay line DEL2 to
a parallel circuit of comb ﬁlters which implements subsequent reverberation. In order to
get a high-quality spatial impression, it is necessary to decorrelate the room signals eL(n)
and eR(n) [Bla74, Bla85]. This can be achieved by taking left and right room signals at
different points out of the parallel circuit of comb ﬁlters. These room signals are then fed
to an all-pass section to increase the echo density.
Besides the described system for stereo room simulation in which the mono signal is
processed with a room algorithm, it is also possible to perform complete stereo process-
ing of xL(n) und xR(n), or to process a mono signal xM (n) = xL(n) + xR(n) and a side
(difference) signal xS(n) = xL(n) − xR(n) individually.
6.3.2 General Feedback Systems
Further developments of the comb ﬁlter method by Schroeder tried to improve the acoustic
quality of reverberation and especially the increase in echo density [Ger71, Ger76, Sta82,
Jot91, Jot92, Roc95, Roc96, Roc97a, RS97b]. With respect to [Jot91], the general feedback
system in Fig. 6.24 is considered. For the sake of simplicity only three delay systems are
shown. The feedback of output signals is carried out with the help of a matrix A which
feeds back each of the three outputs to the three inputs.
6.3 Subsequent Reverberation
’
7(cid:10):
(cid:15)
209
"
7(cid:10):
(cid:15)
(cid:26)(cid:30)(cid:15)(cid:22)
(cid:26)(cid:30)(cid:15)(cid:23)
$(/,(cid:22)
$(/,(cid:23)
$(/,2
$(/,<
$(/,(cid:14)(cid:24)
(cid:2)
7(cid:10):
(cid:15)
&(cid:1)(cid:22)(cid:29)(cid:29)/
&(cid:1)(cid:22)(cid:29)(cid:29)/
(cid:2)
7(cid:10):
(cid:19)
7(cid:10):
’
(cid:19)
7(cid:10):
"
(cid:19)
Figure 6.23 Stereo room simulation.
"7(cid:10):
-
(cid:20)
(cid:22)
(cid:20)
(cid:23)
(cid:20)
2
’7(cid:10):
.
(cid:22)
.
(cid:23)
.
2
(cid:31)(cid:14)(cid:14)7(cid:10):
(cid:22)
(cid:5)(cid:12)(cid:22)
(cid:31)(cid:14)(cid:14)7(cid:10):
(cid:23)
(cid:5)(cid:12)(cid:23)
(cid:31)(cid:14)(cid:14)7(cid:10):
2
 (cid:5)(cid:12)2
(cid:9)(cid:22)(cid:22) (cid:9)(cid:22)(cid:23) (cid:9)(cid:22)2
(cid:9)(cid:23)(cid:22) (cid:9)(cid:23)(cid:23) (cid:9)(cid:23)2
(cid:9)2(cid:22) (cid:9)2(cid:23) (cid:9)22
Figure 6.24 General feedback system.
