# Digital Audio DSP (Zölzer) — Chunk 21/30

> Source: `Udo-Zolzer-digital-audio-signal-processing.9780470997857.40435.pdf` · Pages ~1-331 · Extractor: pdfminer.six
> Target ≈ 2800 words, Overlap = 200 words · Actual = 5034

> Note: DSP textbook content for SpectralCanvasPro RT-audio knowledge base

---
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

210
Room Simulation
In general, for N delay systems we can write
y(n) =
qj (n + mj ) =
N(cid:6)
i=1
N(cid:6)
i=1
ciqi(n) + dx(n),
aijqi(n) + bj x(n),
1 ≤ j ≤ N.
The Z-transform leads to
with
Y (z) = cT Q(z) + d · X(z),
D(z) · Q(z) = A · Q(z) + b · X(z)
→ Q(z) = [D(z) − A]−1b · X(z),
Q(z) =


 ,



Q1(z)
...
QN (z)


 ,



b1
...
bN






c1
...
cN
c =
b =
and the diagonal delay matrix
D(z) = diag[z
−m1 · · ·z
−mN ].
With (6.68) the Z-transform of the output is given by
Y (z) = cT [D(z) − A]−1b · X(z) + d · X(z)
and the transfer function by
H (z) = cT [D(z) − A]−1b + d.
(6.65)
(6.66)
(6.67)
(6.68)
(6.69)
(6.70)
(6.71)
(6.72)
T
The system is stable if the feedback matrix A can be expressed as a product of a
unitary matrix U (U−1 = U
) and a diagonal matrix with gii < 1 (derivation in [Sta82]).
Figure 6.25 shows a general feedback system with input vector X(z), output vector Y(z),
a diagonal matrix D(z) consisting of purely delay systems z−mi , and a feedback matrix A.
This feedback matrix consists of an orthogonal matrix U multiplied by the matrix G which
results in a weighting of the feedback matrix A.
If an orthogonal matrix U is chosen and the weighting matrix is equal to the unit matrix
G = I, the system in Fig. 6.25 implements a white-noise random signal with Gaussian
distribution when a pulse excitation is applied to the input. The time density of this signal
slowly increases with time. If the diagonal elements of the weighting matrix G are less
than one, a random signal with exponential amplitude decay results. With the help of
the weighting matrix G, the reverberation time can be adjusted. Such a feedback system
performs the convolution of an audio input signal with an impulse response of exponential
decay.
The effect of the orthogonal matrix U on the subjective sound perception of subsequent
reverberation is of particular interest. A relationship between the distribution of the eigen-
values of the matrix U on the unit circle and the poles of the system transfer function cannot
6.3 Subsequent Reverberation
211
A=UG
X(z)
D(z)
Y(z)
Figure 6.25 Feedback system.
be described analytically, owing to the high order of the feedback system. In [Her94], it is
shown experimentally that the distribution of eigenvalues within the right-hand or left-hand
complex plane produces a uniform distribution of poles of the system transfer function.
Such a feedback matrix leads to an acoustically improved reverberation. The echo density
rapidly increases to the maximum value of one sample per sampling period for a uniform
distribution of eigenvalues. Besides the feedback matrix, additional digital ﬁltering is nec-
essary for spectrally shaping the subsequent reverberation and for implementing frequency-
dependent decay times (see [Jot91]). The following example illustrates the increase of the
echo density.
Example: First, a system with only one feedback path per comb ﬁlter is considered. The
feedback matrix is then given by
A = g√
2
I.
(6.73)
Figure 6.26 shows the impulse response and the amplitude frequency response.
Impulse Response
Frequency Response
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
→
B
d
n
i
)
S
f
/
f
(
H
30
25
20
15
10
5
0
-5
-10
-15
-20
200
400
600
800
1000
n →
0 0.05 0.1 0.15 0.2 0.25 0.3 0.35 0.4 0.45 0.5
f/fS
→
Figure 6.26 Impulse response and frequency response of a 4-delay system with a unit matrix as
unitary feedback matrix (g = 0.83).
212
Room Simulation
With the feedback matrix
A = g√
2




0
−1
1
0




1
0
0
1 −1
1
0
0 −1
0 −1
0
(6.74)
from [Sta82], the impulse response and the corresponding frequency response shown in
Fig. 6.27 are obtained. In contrast to Fig. 6.26, an increase in the echo density of the impulse
response is observed.
Impulse Response
Frequency Response
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
→
B
d
n
i
)
S
f
/
f
(
H
30
25
20
15
10
5
0
-5
-10
-15
-20
200
400
600
800
1000
n →
0 0.05 0.1 0.15 0.2 0.25 0.3 0.35 0.4 0.45 0.5
f/fS
→
Figure 6.27 Impulse response and frequency response of a 4-delay system with unitary feedback
matrix (g = 0.63).
6.3.3 Feedback All-pass Systems
In addition to the general feedback systems, simple delay systems with feedback have been
used for room simulators (see Fig. 6.28). Theses simulators are based on a delay line,
where single delays are fed back with L feedback coefﬁcients to the input. The sum of
the input signal and feedback signal is low-pass ﬁltered or spectrally weighted by a low-
frequency shelving ﬁlter and is then put to the delay line again. The ﬁrst N reﬂections are
extracted out of the delay line according to the reﬂection pattern of the simulated room.
They are weighted and added to the output signal. The mixing between the direct signal
and the room signal is adjusted by the factor gMIX. The inner system can be described by a
rational transfer function H (z) = Y (z)/X(z). In order to avoid a low frequency density the
feedback delay lengths can be made time-variant [Gri89, Gri91].
Increasing the echo density can be achieved by replacing the delays z−Mi by frequency
dependent all-pass systems A(z−Mi ). This extension was ﬁrst proposed by Gardner in
[Gar92, Gar98]. In addition to the replacement of z−Mi → A(z−Mi ), the all-pass systems
can be extended by embedded all-pass systems [Gar92]. Figure 6.29 shows an all-pass
system (Fig. 6.29a) where the delay z−M is replaced by a further all-pass and a unit delay
z−1 (Fig. 6.29b). The integration of a unit delay avoids delay-free loops. In Fig. 6.29c the
inner all-pass is replaced by a cascade of two all-pass systems and a further delay z−M3.
6.4 Approximation of Room Impulse Responses
213
y(n)
1-gMIX
gMIX
g1
g2
g3
gN
x(n)
TP
z-M1
z-M2
z-M3
z-MN
gFB1
gFB2
gFB3
Figure 6.28 Room simulation with delay line and forward and backward coefﬁcients.
The resulting system is again an all-pass system [Gar92, Gar98]. A further modiﬁcation of
the general all-pass system is shown in Fig. 6.29d [Dat97, Vää97, Dah00]. Here, a delay
z−M followed by a low-pass and a weighting coefﬁcient is used. The resulting system
is called an absorbent all-pass system. With these embedded all-pass systems the room
simulator shown in Fig. 6.28 is extended to a feedback all-pass system which is shown in
Fig. 6.30 [Gar92,Gar98]. The feedback is performed by a low-pass ﬁlter and a feedback
coefﬁcient g, which adjusts the decay behavior. The extension to a stereo room simulator
is described in [Dat97, Dah00] and is depicted in Fig. 6.31 [Dah00]. The cascaded all-pass
systems Ai(z) in the left and right channel can be a combination of embedded and absorbent
all-pass systems. Both output signals of the all-pass chains are fed back to the input and
added. In front of both all-pass chains a coupling of both channels with a weighted sum
and difference is performed. The setup and parameters of such a system are discussed in
[Dah00]. A precise adjustment of reverberation time and control of echo density can be
achieved by the feedback coefﬁcients of the all-passes. The frequency density is controlled
by the scaling of the delay lengths of the inner all-pass systems.
6.4 Approximation of Room Impulse Responses
In contrast to the systems for simulation of room impulse responses discussed up to this
point, a method is now presented that measures and approximates the room impulse re-
sponse in one step [Zöl90b, Sch92, Sch93] (see Fig. 6.32). Moreover, it leads to a paramet-
ric representation of the room impulse response. Since the decay times of room impulse
responses decrease for high frequencies, use is made of multirate signal processing.
The analog system that is to be measured and approximated is excited with a binary
pseudo-random sequence x(n) via a DA converter. The resulting room signal gives a digital
sequence y(n) after AD conversion. The discrete-time sequence y(n) and the pseudo-
random sequence x(n) are each decomposed by an analysis ﬁlter bank into subband signals
y1, . . . , yP and x1, . . . , xP respectively. The sampling rate is reduced in accordance with
the bandwidth of the signals. The subband signals y1, . . . , yP are approximated by adjust-
ing the subband systems H1(z) = A1(z)/B1(z), . . . , HP (z) = AP (z)/BP (z). The outputs
214
a)
A(z)
g
z-M
-g
b)
c)
d)
Room Simulation
g1
x(n)
A(z)
z-1
y(n)
-g1
g1
x(n)
x(n)
A (z)1
A (z)2
z-M3
y(n)
-g1
g1
z-M
-g1
a
y(n)
H (z)
TP
Figure 6.29 Embedded and absorbing all-pass system [Gar92, Gar98, Dat97, Vää97, Dah00].
1-gMIX
gMIX
y(n)
g1
g2
g3
gN
x(n)
A (z)1
A (z)2
A (z)3
A (z)N
g
TP
Figure 6.30 Room simulator with embedded all-pass systems [Gar92,Gar98].
ˆy1, . . . , ˆyP of these subband systems give an approximation of the measured subband
signals. With this procedure, the impulse response is given in parametric form (subband
parameters) and can be directly simulated in the digital domain.
6.4 Approximation of Room Impulse Responses
gL1
gL2
gL3
gLN
x (n)
L
1/ 2
A (z)
L1
A (z)
L2
A (z)
L3
A (z)
LN
x (n)R
1/ 2
_
A (z)
R1
A (z)
R2
A (z)
R3
A (z)
RN
gR1
gR2
gR3
gRN
215
y (n)
L
1-gMIX
gMIX
gMIX
1-gMIX
y (n)R
Figure 6.31 Stereo room simulator with absorbent all-pass systems [Dah00].
&(cid:10)(cid:9)(cid:4)(cid:11)(cid:3)
(cid:6)"(cid:18)(cid:7)(cid:2)(cid:12)
(cid:26)&$
(cid:21)7(cid:7):
&(cid:26)$
"7(cid:10):
(cid:21)7(cid:10):
&@,
’7(cid:10):
&@,
(cid:22)"
(cid:24)"
(cid:22)’
’(cid:1)
&(cid:10)(cid:9)(cid:4)"(cid:18)(cid:16)(cid:18)
@(cid:16)(cid:4)(cid:7)(cid:2)(cid:8)(cid:14),(cid:9)(cid:10)(cid:17)
(cid:22)"g
g
(cid:1)"
(cid:22)&(cid:14)(cid:14)7 :
(cid:22),(cid:14)(cid:14)7 :
&(cid:14)(cid:14)7 :(cid:1)
(cid:1),(cid:14)(cid:14)7 :
(cid:6)(cid:13)..(cid:9)(cid:10)-
(cid:6)"(cid:18)(cid:7)(cid:2)(cid:12)(cid:18)
g"7(cid:10):
(cid:6)@,
(cid:6)"(cid:10)(cid:7)(cid:21)(cid:2)(cid:18)(cid:16)(cid:18)
@(cid:16)(cid:4)(cid:7)(cid:2)(cid:8)(cid:14),(cid:9)(cid:10)(cid:17)
Figure 6.32 System measuring and approximating room impulse responses.
216
Room Simulation
By suitably adjusting the analysis ﬁlter bank [Sch94], the subband impulse responses
are obtained directly from the cross-correlation function
hi ≈ rxi yi .
(6.75)
The subband impulse responses are approximated by a nonrecursive ﬁlter and a recursive
comb ﬁlter. The cascade of both ﬁlters leads to the transfer function
Hi(z) = b0 + · · · + bMi z−Mi
1 − giz−Ni
=
∞(cid:6)
ni =0
hi (ni)z
−ni ,
(6.76)
which is set equal to the impulse response in subband i. Multiplying both sides of (6.76)
by the denominator 1 − giz−Ni gives
b0 + · · · + bMi z
−Mi =
(cid:2)
hi(ni )z
−ni
(1 − giz
−Ni ).
(6.77)
(cid:1) ∞(cid:6)
ni =0
Truncating the impulse response of each subband to K samples and comparing the co-
efﬁcients of powers of z on both sides of the equation, the following set of equations is
obtained:












=
























b0
b1
...
bM
0
...
0
h0
h1
...
hM
hM+1
...
hK
0
h0
...
0
0
...
hM−1 hM−2
hM−1
hM
...
...
hK−2
hK−1
· · ·
· · ·
· · ·
· · ·
· · ·
0
0
...
hM−N
hM−N+1
...
hK−N
















 .
(6.78)





1
0
...
−g
The coefﬁcients b0, . . . , bM and g in the above equation are determined in two steps.
First, the coefﬁcient g of the comb ﬁlter is calculated from the exponentially decaying
envelope of the measured subband impulse response. The vector [1, 0, . . . , g]T is then
used to determine the coefﬁcients [b0, b1, . . . , bM]T .
For the calculation of the coefﬁcient g, we start with the impulse response of the comb
ﬁlter H (z) = 1/(1 − gz−N ) given by
We further make use of the integrated impulse response
h(l = Nn) = gl.
he(k) =
∞(cid:6)
n=k
h(n)2
(6.79)
(6.80)
deﬁned in [Schr65]. This describes the rest energy of the impulse response at time k. By
taking the logarithm of he(k), a straight line over time index k is obtained. From the slope
of the straight line we use
ln g = N · ln he(n1) − ln he(n2)
n1 − n2
,
n1 < n2,
(6.81)
6.5 Java Applet – Fast Convolution
217
to determine the coefﬁcient g [Sch94]. For M = N, the coefﬁcients in (6.78) of the numer-
ator polynomial are obtained directly from the impulse response
bn = hn,
bM = hM − gh0.
n = 0, 1, . . . , M − 1,
(6.82)
Hence, the numerator polynomial of (6.76) is a direct reproduction of the ﬁrst M samples
of the impulse response (see Fig. 6.33). The denominator polynomial approximates the
further exponentially decaying impulse response. This method is applied to each subband.
The implementation complexity can be reduced by a factor of 10 compared with the direct
implementation of the broad-band impulse response [Sch94]. However, owing to the group
delay caused by the ﬁlter bank, this method is not so suitable for real-time applications.
h(n)
h0
b0
b1
hM
b0g
bM
b0g2
bMg
n
Figure 6.33 Determining model parameters from the measured impulse response.
6.5 Java Applet – Fast Convolution
The applet shown in Fig. 6.34 demonstrates audio effects resulting from a fast convolution
algorithm. It is designed for a ﬁrst insight into the perceptual effects of convolving an
impulse response with an audio signal.
The applet generates an impulse response by modulating the amplitude of a random
signal. The graphical interface presents the curve of the amplitude modulation, which can
be manipulated with three control points. Two control points are used for the initial behavior
of the amplitude modulation. The third control point is used for the exponential decay of
the impulse response. You can choose between two predeﬁned audio ﬁles from our web
server (audio1.wav or audio2.wav) or your own local wav ﬁle to be processed [Gui05].
6.6 Exercises
1. Room Impulse Responses
1. How can we measure a room impulse response?
2. What kind of test signal is necessary?
3. How does the length of the impulse response affect the length of the test signal?
218
Room Simulation
Figure 6.34 Java applet – fast convolution.
2. First Reﬂections
For a given sound (voice sound) calculate the delay time of a single ﬁrst reﬂection. Write a
Matlab program for the following computations.
1. How do we choose this delay time? What coefﬁcient should be used for it?
2. Write an algorithm which performs the convolution of the input mono signal with
two impulse responses which simulate a reﬂection to the left output yL(n) and a
second reﬂection to the right output yR(n). Check the results by listening to the
output sound.
3. Improve your algorithm to simulate two reﬂections which can be positioned at any
angle inside the stereo mix.
3. Comb and All-pass Filters
1. Comb Filters: Based on the Schroeder algorithm, draw a signal ﬂow graph for a
comb ﬁlter consisting of a single delay line of M samples with a feedback loop
containing an attenuation factor g.
(a) Derive the transfer function of the comb ﬁlter.
(b) Now the attenuation factor g is in the feed-forward path and in the feedback
loop no attenuation is applied. Why can we consider the impulse response of
this model to be similar to the previous one?
(c) In both cases how should we choose the gain factor? What will happen if we
do otherwise?
(d) Calculate the reverberation time of the comb ﬁlter for fS = 44.1 kHz, M = 8
and g speciﬁed previously.
(e) Write down what you know about the ﬁlter coefﬁcients, plot the pole/zero
locations and the frequency response of the ﬁlter
References
219
2. All-pass Filters: Realize an all-pass structure as suggested by Schroeder.
(a) Why can we expect a better result with an all-pass ﬁlter than with comb ﬁlter?
Write a Matlab function for a comb and all-pass ﬁlter with M = 8, 16.
(b) Derive the transfer function and show the pole/zero locations, the impulse re-
sponse, the magnitude and phase responses.
(c) Perform the ﬁltering of an audio signal with the two ﬁlters and estimate the
delay length M which leads to a perception of a room impression.
4. Feedback Delay Networks
Write a Matlab program which realizes a feedback delay network.
1. What is the reason for a unitary feedback matrix?
2. What is the advantage of using a unitary circulant feedback matrix?
3. How do you control the reverberation time?
References
[All79]
J. B. Allen, D. A. Berkeley: Image Method for Efﬁcient Simulating Small Room
Acoustics, J. Acoust. Soc. Am., Vol. 65, No. 4, pp. 943–950, 1979.
[And90] Y. Ando: Concert Hall Acoustics, Springer-Verlag, Berlin, 1990.
[Bro01]
S. Browne: Hybrid Reverberation Algorithm Using Truncated Impulse
Response Convolution and Recursive Filtering, MSc Thesis, University of
Miami, Coral Gables, FL, June 2001.
[Bar71] M. Barron: The Subjective Effects of First Reﬂections in Concert Halls – The
Need for Lateral Reﬂections, J. Sound and Vibration, Vol. 15, pp. 475–494,
1971.
[Bar81] M. Barron, A. H. Marschall: Spatial Impression Due to Early Lateral
Reﬂections in Concert Halls: The Derivation of a Physical Measure, J. Sound
and Vibration, Vol. 77, pp. 211–232, 1981.
[Bel99]
F. A. Beltran, J. R. Beltran, N. Holzem, A. Gogu: Matlab Implementation
of Reverberation Algorithms, Proc. Workshop on Digital Audio Effects
(DAFx-99), pp. 91–96, Trondheim, 1999.
[Bla74]
J. Blauert: Räumliches Hören, S. Hirzel Verlag, Stuttgart, 1974.
[Bla85]
[Ble01]
J. Blauert: Räumliches Hören, Nachschrift-Neue Ergebnisse und Trends seit
1972, S. Hirzel Verlag, Stuttgart, 1985.
B. Blesser: An Interdisciplinary Synthesis of Reverberation Viewpoints,
J. Audio Eng. Soc., Vol. 49, pp. 867–903, October 2001.
220
[Cre78]
L. Cremer, H. A. Müller: Die wissenschaftlichen Grundlagen der Raumakustik,
Vols. 1 and 2, S. Hirzel Verlag, Stuttgart, 1976/78.
Room Simulation
[Cre03]
L. Cremer, M. Möser: Technische Akustik, Springer-Verlag, Berlin, 2003.
[Dah00]
[Dat97]
[Ege96]
[Far00]
[Fre00]
L. Dahl, J.-M. Jot: A Reverberator Based on Absorbent All-Pass Filters, in
Proc. of the COST G-6 Conference on Digital Audio Effects (DAFX-00),
Verona, December 2000.
J. Dattorro: Effect Design – Part 1: Reverberator and Other Filters, J. Audio
Eng. Soc., Vol. 45, pp. 660–684, September 1997.
G. P. M. Egelmeers, P. C. W. Sommen: A New Method for Efﬁcient Convolution
in Frequency Domain by Nonuniform Partitioning for Adaptive Filtering, IEEE
Trans. Signal Processing, Vol. 44, pp. 3123–3192, December 1996.
A. Farina: Simultaneous Measurement of Impulse Response and Distortion with
a Swept-Sine Technique, Proc. 108th AES Convention, Paris, February 2000.
J. Frenette: Reducing Artiﬁcial Reverberation Algorithm Requirements Using
Time-Varying Feddback Delay Networks, MSc thesis, University of Miami,
Coral Gables, FL, December 2000.
[Gar95] W. G. Gardner: Efﬁcient Convolution Without Input-Output Delay, J. Audio
Eng. Soc., Vol. 43, pp. 127–136, 1995.
[Gar98] W. G. Gardner: Reverberation Algorithms, in M. Kahrs and K. Brandenburg
(Ed.), Applications of Digital Signal Processing to Audio and Acoustics,
Kluwer Academic Publishers, Boston, pp. 85–131, 1998.
[Ger71] M. A. Gerzon: Synthetic Stereo Reverberation, Studio Sound, No. 13, pp. 632–
635, 1971 and No. 14, pp. 24–28, 1972.
[Ger76] M. A. Gerzon: Unitary (Energy-Preserving) Multichannel Networks with
Feedback, Electronics Letters, Vol. 12, No. 11, pp. 278–279, 1976.
[Ger92] M. A. Gerzon: The Design of Distance Panpots, Proc. 92nd AES Convention,
Preprint No. 3308, Vienna, 1992.
[Gri89]
[Gri91]
D. Griesinger: Practical Processors and Programs for Digital Reverberation,
Proc. AES 7th Int. Conf., pp. 187–195, Toronto, 1989.
D. Griesinger: Improving Room Acoustics through Time-Variant Synthetic
Reverberation, in Proc. 90th Conv. Audio Eng. Soc., Preprint 3014, February
1991.
[Gui05] M. Guillemard, C. Ruwwe, U. Zölzer: J-DAFx – Digital Audio Effects in Java,
Proc. 8th Int. Conference on Digital Audio Effects (DAFx-05), pp. 161–166,
Madrid, 2005.
[Her94]
T. Hertz: Implementierung und Untersuchung von Rückkopplungssystemen zur
digitalen Raumsimulation, Diplomarbeit, TU Hamburg-Harburg, 1994.
References
[Iid95]
K. Iida, K. Mizushima, Y. Takagi, T. Suguta: A New Method of Generating
Artiﬁcial Reverberant Sound, Proc. 99th AES Convention, Preprint No. 4109,
October 1995.
221
[Joh00] M. Joho, G. S. Moschytz: Connecting Partitioned Frequency-Domain Filters
in Parallel or in Cascade, IEEE Trans. CAS-II: Analog and Digital Signal
Processing, Vol. 47, No. 8, pp. 685–698, August 2000.
[Jot91]
[Jot92]
J. M. Jot, A. Chaigne: Digital Delay Networks for Designing Artiﬁcial
Reverberators, Proc. 94th AES Convention, Preprint No. 3030, Berlin, 1991.
J. M. Jot: An Analysis/Synthesis Approach to Real-Time Artiﬁcial Reverbera-
tion, Proc. ICASSP-92, pp. 221–224, San Francisco, 1992.
[Ken95a] G. S. Kendall: A 3-D Sound Primer: Directional Hearing and Stereo
Reproduction, Computer Music J., Vol. 19, No. 4, pp. 23–46, Winter 1995.
[Ken95b] G. S. Kendall. The Decorrelation of Audio Signals and Its Impact on Spatial
Imagery, Computer Music J., Vol. 19, No. 4, pp. 71–87, Winter 1995.
[Kut91]
H. Kuttruff: Room Acoustics, 3rd edn, Elsevier Applied Sciences, London,
1991.
[Lee03a] W.-C. Lee, C.-M. Liu, C.-H. Yang, J.-I. Guo, Perceptual Convolution for
Reverberation, Proc. 115th Convention 2003, Preprint No. 5992, October 2003
[Lee03b] W.-C. Lee, C.-M. Liu, C.-H. Yang, J.-I. Guo, Fast Perceptual Convolution
for Reverberation, Proc. of the 6th Int. Conference on Digital Audio Effects
(DAFX-03), London, September 2003.
[Lok01]
T. Lokki, J. Hiipakka: A Time-Variant Reverberation Algorithm for Reverber-
ation Enhancement Systems, Proc. of the COST G-6 Conference on Digital
Audio Effects (DAFX-01), Limerick, December 2001.
[Moo78]
J. A. Moorer: About This Reverberation Business, Computer Music J., Vol. 3,
No. 2, pp. 13–28, 1978.
[Mac76]
F. J. MacWilliams, N. J. A. Sloane: Pseudo-Random Sequences and Arrays,
IEEE Proc., Vol. 64, pp. 1715–1729, 1976.
[Mül01]
S. Müller, P. Massarani: Transfer-Function Measurement with Sweeps, J. Audio
Eng. Soc., Vol. 49, pp. 443–471, 2001.
[Pii98]
E. Piirilä, T. Lokki, V. Välimäki, Digital Signal Processing Techniques for
Non-exponentially Decaying Reverberation, Proc. 1st COST-G6 Workshop on
Digital Audio Effects (DAFX98), Barcelona, 1998.
[Soo90]
J. S. Soo, K. K. Pang: Multidelay Block Frequency Domain Adaptive Filter,
IEEE Trans. ASSP, Vol. 38, No. 2, pp. 373–376, February 1990.
222
[Rei95]
[Roc95]
[Roc96]
Room Simulation
A. J. Reijen, J. J. Sonke, D. de Vries: New Developments in Electro-
Acoustic Reverberation Technology, Proc. 98th AES Convention 1995, Preprint
No. 3978, February 1995.
D. Rocchesso: The Ball within the Box: A Sound-Processing Metaphor,
Computer Music J., Vol. 19, No. 4, pp. 47–57, Winter 1995.
D. Rocchesso: Strutture ed Algoritmi per l’Elaborazione del Suono basati
su Reti di Linee di Ritardo Interconnesse, PhD thesis, University of Padua,
February 1996.
[Roc97a] D. Rocchesso: Maximally-Diffusive Yet Efﬁcient Feedback Delay Networks
for Artiﬁcial Reverberation, IEEE Signal Processing Letters, Vol. 4, No. 9,
pp. 252–255, September 1997.
[RS97b] D. Rocchesso, J. O. Smith: Circulant and Elliptic Feedback Delay Net- works
for Artiﬁcial Reverberation, IEEE Trans. Speech and Audio Processing, Vol. 5,
No. 1, pp. 51–63, January 1997.
[Roc02]
D. Rocchesso: Spatial Effects, in U. Zölzer (Ed.), DAFX – Digital Audio
Effects, John Wiley & Sons, Ltd, Chichester, pp. 137–200, 2002.
[Sch92] M. Schönle, U. Zölzer, N. Fliege: Modeling of Room Impulse Responses
by Multirate Systems, Proc. 93rd AES Convention, Preprint No. 3447, San
Francisco, 1992.
[Sch93] M. Schönle, N. J. Fliege, U. Zölzer: Parametric Approximation of Room
Impulse Responses by Multirate Systems, Proc. ICASSP-93, Vol. 1, pp. 153–
156, 1993.
[Sch94] M. Schönle: Wavelet-Analyse und parametrische Approximation von Raumim-
pulsantworten, Dissertation, TU Hamburg-Harburg, 1994.
[Schr61] M. R. Schroeder, B. F. Logan: Colorless Artiﬁcial Reverberation, J. Audio Eng.
Soc., Vol. 9, pp. 192–197, 1961.
[Schr62] M. R. Schroeder: Natural Sounding Artiﬁcial Reverberation, J. Audio Eng.
Soc., Vol. 10, pp. 219–223, 1962.
[Schr65] M. R. Schroeder: New Method of Measuring Reverberation Time, J. Acoust.
Soc. Am., Vol. 37, pp. 409–412, 1965.
[Schr70] M. R. Schroeder: Digital Simulation of Sound Transmission in Reverberant
Spaces, J. Acoust. Soc. Am., Vol. 47, No. 2, pp. 424–431, 1970.
[Schr87] M. R. Schroeder: Statistical Parameters of the Frequency Response Curves of
Large Rooms, J. Audio Eng. Soc., Vol. 35, pp. 299–305, 1987.
[Smi85]
J. O. Smith: A New Approach to Digital Reverberation Using Closed
Waveguide Networks, Proc. International Computer Music Conference, pp. 47–
53, Vancouver, 1985.
References
[Sta02]
[Sta82]
[Vää97]
223
G. B. Stan, J. J. Embrechts, D. Archambeau: Comparison of Different Impulse
Response Measurement Techniques, J. Audio Eng. Soc., Vol. 50, pp. 249–262,
2002.
J. Stautner, M. Puckette: Designing Multi-channel Reverberators, Computer
Music J., Vol. 6, No. 1, pp. 56–65, 1982.
R. Väänänen, V. Välimäki, J. Huopaniemi: Efﬁcient and Parametric
Reverberator for Room Acoustics Modeling, Proc. Int. Computer Music Conf.
(ICMC’97), pp. 200–203, Thessaloniki, September 1997.
[Zöl90]
U. Zölzer, N. J. Fliege, M. Schönle, M. Schusdziarra: Multirate Digital
Reverberation System, Proc. 89th AES
Convention, Preprint No. 2968, Los Angeles, 1990.
