# Digital Audio DSP (Zölzer) — Chunk 22/30

> Source: `Udo-Zolzer-digital-audio-signal-processing.9780470997857.40435.pdf` · Pages ~1-331 · Extractor: pdfminer.six
> Target ≈ 2800 words, Overlap = 200 words · Actual = 9086

> Note: DSP textbook content for SpectralCanvasPro RT-audio knowledge base

---
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
