# Audio Plugins C++ (Pirkle) — Chunk 25/73

> Source: `Designing Audio Effect Plug-Ins in C++_ With Digital Audio Signal Processing Theory - PDF Room.pdf` · Pages ~1-560 · Extractor: pdfminer.six
> Target ≈ 2800 words, Overlap = 200 words · Actual = 2660

> Note: DSP textbook content for SpectralCanvasPro RT-audio knowledge base

---
158  Chapter 5
 Factor out a0
 where
 H (z) 5 a0
1 1 a1z21 1 a2z22
1 1 b1z21 1 b2z22
(5.87)
  a1 5
 a2 5
a1
a0
a2
a0
  Step 4: Plot the poles and zeros of the transfer function
 The bi-quad will produce a conjugate pair of zeros and conjugate pair of poles from the
numerator and denominator respectively. Calculating these locations is the same as in the
pure second-order feed forward and feed back topologies. All you need to do is plot them in
the same unit circle. The transfer function becomes (by simple substitution from previous
sections):
  H(z) 5 a0
1 2 2Rz cos(u)z21 1 Rz
1 2 2Rp cos(f)z21 1 Rp
2z22
2z22
(5.88)
  Figure 5.62  shows a pair of poles and a pair of zeros plotted together. Each has its own
radius, R z  and R p , and angle, u and w. The same kind of pole/zero tug of war goes on with the
bi-quad, only now there are more competing entities.
 Estimating the frequency response is complicated by the additional poles and zeros, but the
rules are still the same:
•
•
   Locate each evaluation frequency on the outer rim of the unit circle.
   Draw a line from the point on the circle to  each  zero and measure the length of these
 vectors. Do it for each evaluation frequency.
Im
RP
R P
φ
z
θ
Rz
– φ
Re
 Figure 5.62:    Second-order poles and zeros.

Basic DSP Theory  159
•
•
•
•
   Draw a line from the point on the circle to  each  pole and measure the length of these
 vectors. Do it for each evaluation frequency.
   Multiply all the zero magnitudes together.
   Multiply all the  inverse  pole magnitudes together.
   Divide the zero magnitude by the pole magnitude for the ﬁ nal result at that frequency.
 Using the following coefﬁ cients, a 0  5 1.0, a 1  5 0.73, a 2  5 1.00, b 1  5 20.78, b 2  5 0.88,
we can directly ﬁ nd the pole and zero locations from  Equation 5.88  (note that because a 0
is 1.0, you don’t have to calculate the a terms). The pairs of poles and zeros are plotted in
 Figure 5.63 .
 Zeros are calculated as follows:
 and
   a2 5 R2
z 5 1.00
 Rz 5 "1.00 5 1.00
 a1 5 22R cos(u) 5 0.73
(5.89)
 2(1.00) cos(u) 5 0.73
 cos(u) 5 0.365
 u 5 arccos(0.365)
 u 5 68.68
 Poles are calculated as follows:
 b2 5 R2
p 5 0.88
 Rp 5 "0.88 5 0.94
Im
φ
θ
Re
 Figure 5.63:    Pole/zero plot for the example bi-quad ﬁ lter.

160  Chapter 5
 and
  b1 5 22R cos(f) 5 20.78
 2 (0.94) cos(f) 5 0.78
  cos(f) 5
0.78
2(0.94)
 f 5 arccos(0.414)
 f 5 65.58
(5.90)
 The poles and zeros are in close proximity to each other. The zero is directly on the unit
circle (R z  5 1.0), so we expect a notch to occur there. The pole is near the unit circle but not
touching it, so we expect a resonance there.
 We are not going to go through the full response estimation or direct evaluation since it’s just
repetitive algebra at this point. But, we can use RackAFX’s bi-quad module to set up the ﬁ lter
and evaluate it ( Figure 5.64 ). The frequency response clearly shows the resonant peaking due
to the pole, then the sudden notch due to the nearby zero.
 In  Figure 5.64 , it’s easy to locate the places where the pole or zero dominates. In the low
frequencies, the pole dominates and at high frequencies the zero dominates. This is an
example of a direct  z -plane design where we place a pole and zero pair directly in the  z -plane,
then calculate the coefﬁ cients. In the next chapter, we will examine some basic Inﬁ nite
Impulse Response (IIR)  design techniques and the direct  z -plane method will be the ﬁ rst.
  Final Review Notes
 In this chapter you learned the basics of DSP theory; speciﬁ cally, you learned the sequence:
1.
2.
3.
   Take the  z  transform of the difference equation.
   Fashion the difference equation into a transfer function.
   Factor out a 0  as the scalar gain coefﬁ cient.
+24.0 dB
+12.0 dB
0.0 dB
–12.0 dB
–24.0 dB
–36.0 dB
–48.0 dB
Effect  of the  pole
2 kHz  4 kHz
6 kHz
8 kHz  10 kHz  12 kHz  14 kHz  16 kHz  18 kHz  20 kHz
Effect  of the  zero
 Figure 5.64:    The plot from RackAFX shows the resonant peak and notch formed
by the pole and zero.

Basic DSP Theory  161
4.
5.
6.
   Estimate the frequency response.
   Direct evaluation of frequency response.
    z  transform of impulse response as a ﬁ nal check.
 For geometric estimation, the frequency response and phase responses of a ﬁ lter can be found
with  Equations 5.91  and  5.92 .
  0 H (ejv) 0
v 5 a0
N
q
i51
N
q
i51
Ui
Vi
 where
 where
  N 5 the filter order
 Ui 5 the geometric length from the point (v) on the unit circle to the ith zero
 Vi 5 the geometric length from the point (v) on the unit circle to the ith pole
N
  Arg(H (ejv))|v 5 a
i51
N
ui 2 a
i51
fi
  N 5 the filter order
 ui 5 the angle between the ith zero and the vector Ui
 fi 5 the angle between the ith pole and the vector Vi
(5.91)
(5.92)
 For direct evaluation, you simply plug in various values for frequency and crank through the
algebra. We applied this same routine to feed-forward, feed-back, and combination types of
algorithms, and then used RackAFX to check our results. We also classiﬁ ed the ﬁ lters into
IIR and Finite Impulse Response (FIR)  types.
  IIR Filters
•
•
•
•
•
   Any ﬁ lter with a feed-back path is IIR in nature, even if it has feed-forward branches
as well.
   The feed-back paths in IIR ﬁ lters produce poles in the  z -plane and the poles cause gain to
occur in the magnitude response.
   An IIR ﬁ lter can blow up when its output steadily increases toward inﬁ nity, which occurs
when the poles are located outside the unit circle.
   If the IIR ﬁ lter also has feed-forward branches it will produce zeros as well as poles.
   IIR ﬁ lters can have fast transient responses but may ring.
  FIR Filters
•
•
   The FIR ﬁ lter only has feed-forward paths.
   It only produces zeros in the  z -plane.

162  Chapter 5
•
•
•
   The FIR ﬁ lter is unconditionally stable since its output can only go to zero in the
worst case.
   FIR ﬁ lters will have slower transient responses because of the time smearing they do on
the impulse response.
   The more delay elements in the FIR, the poorer the transient response becomes.
     Bibliography
 Ifeachor, E. C. and Jervis, B. W. 1993.  Digital Signal Processing: A Practical Approach , Chapter 3. Menlo Park,
CA: Addison-Wesley.
 Kwakernaak, H. and Sivan, R. 1991.  Modern Signals and Systems , Chapter 3. Englewood Cliffs, NJ: Prentice-Hall.
 Moore, R. 1990.  Elements of Computer Music , Chapter 2. Englewood Cliffs, NJ: Prentice-Hall.
 Oppenheim, A. V. and Schafer, R. W. 1999.  Discrete-Time Signal Processing  (2nd ed.), Chapter 3. Englewood
Cliffs, NJ: Prentice-Hall.
 Orfanidis, S. 1996.  Introduction to Signal Processing , Chapters 10–11. Englewood Cliffs, NJ: Prentice-Hall.
 Steiglitz, K. 1996.  A DSP Primer with Applications to Digital Audio and Computer Music , Chapters 4–5. Menlo
Park, CA: Addison-Wesley.
Audio Filter Designs: IIR Filters
   CHAP TE R 6
    It’s time to put the theory into practice and make some audio ﬁ lters and equalizers (EQs).
You know that the coefﬁ cients of a ﬁ lter determine its frequency response and other
characteristics. But how do you ﬁ nd the coefﬁ cients? There are two fundamental ways to ﬁ nd
the coefﬁ cients of the inﬁ nite impulse response (IIR) ﬁ lter:
(cid:129)
(cid:129)
   Direct  z -plane design
   Analog ﬁ lter to digital ﬁ lter conversion
 This chapter uses the following ﬁ lter naming conventions:
(cid:129)
(cid:129)
(cid:129)
(cid:129)
   LPF: Low-pass ﬁ lter
   HPF: High-pass ﬁ lter
   BPF: Band-pass ﬁ lter
   BSF: Band-stop ﬁ lter
  6.1    Direct z-Plane Design
 In this ﬁ rst category of design techniques, you manipulate the poles and zeros directly in
the  z -plane to create the response you want. You take advantage of the simple equations that
relate the coefﬁ cients to the pole/zero locations. Consider the bi-quad.  Equation 6.1  shows the
numerator.  Equation 6.2  shows the denominator.
H(z) 5 a0(1 1 a1z21 1 a2z22)
5 a0(1 2 2Rcos (u)z21 1 R2z22)
(6.1)
then
a1 5 22Rcos (u)
a2 5 R2
163

164  Chapter 6
H(z)  5 a0 c
1
1 1 b1z21 1 b2z22
d
5 a0 c
1
(1 2 2Rcos(u)z21 1 R2z22)
d
(6.2)
then
b1 5 22Rcos(u)
b2 5 R2
 For the numerator or denominator, the a 1  or b 1  coefﬁ cients are in direct control over the
angles of the zeros or poles. The distance  R  to the zeros or poles is determined by both a 1 , a 2
or b 1 , b 2 . For ﬁ rst-order ﬁ lters, the coefﬁ cients only control the location of the pole and zero
on the real axis. There are no conjugate pairs. However, careful placement of the pole and
zero can still result in useful audio ﬁ lters.
   6.2    Single Pole Filters
 A block diagram of a single pole ﬁ lter is shown in  Figure 6.1 .
 The difference equation is as follows:
  y(n) 5 a0x(n) 2 b1y(n 2 1)
(6.3)
  6.2.1  First-Order LPF and HPF
 Specify:
(cid:129)
    f  c , the corner frequency
x(n)
a0
y(n)
-1
z
–b1
 Figure 6.1:    The ﬁ rst-order feed-back ﬁ lter and difference equation.

Audio Filter Designs: IIR Filters  165
High frequencies are far
enough that they get
attenuation
Im
Low frequencies are
close to the pole and get
gain
Re
 Figure 6.2:    The ﬁ rst-order LPF has a single pole and zero on the real axis.
 The design equations are as follows:
             LPF
uc 5 2pfc/fs
g 5 2 2 cos(uc)
b1 5 "g2 2 1 2 g
a0 5 1 1 b1
             HPF
uc 5 2pfc/fs
g 5 2 1 cos(uc)
b1 5 g 2 "g2 2 1
a0 5 1 2 b1
 (6.4)
 These simple ﬁ rst-order ﬁ lters work by moving the pole back and forth across the real axis
while holding a ﬁ xed zero at Nyquist. When the pole is on the right side of the unit circle,
low frequencies are gained up due to their close proximity to the pole. High frequencies
are close to the zero and get attenuated ( Figure 6.2 ). This produces the low-pass response
in  Figure 6.3 .
 When the pole/zero pair are reversed, the opposite effect happens and the high frequencies are
now boosted with low frequencies attenuated. There is also a zero at 0 Hz.   This produces the
high-pass response. These are simple but very useful designs and are found in many effects
including delay and reverb algorithms.
    6.3    Resonators
 A resonator is a band-pass ﬁ lter that can be made to have a very narrow peak. The simple
version uses a second-order feed-back topology.
  6.3.1  Simple Resonator
 A block diagram of a simple resonator is shown in  Figure 6.4 .

166  Chapter 6
+12.0 dB
0.0 dB
-12.0  dB
-24.0  dB
-36.0  dB
-48.0  dB
-60.0  dB
+12.0 dB
0.0 dB
-12.0  dB
-24.0  dB
-36.0  dB
-48.0  dB
-60.0  dB
2kHz
4kHz
6kHz  8kHz  10kHz  12kHz  14kHz  16kHz  18kHz  20kHz
10Hz
100 Hz
1kHz
10 kHz
 Figure 6.3:    Linear and log frequency responses of the one-pole low-pass ﬁ lter with  f  c   = 1 kHz.
x(n)
a0
y(n)
–1
z
-1
z
–b1
–b2
 Figure 6.4:    Second-order feed-back ﬁ lter block diagram.
 The difference equation is as follows:
  y(n) 5 a0x(n) 2 b1y(n 2 1) 2 b2y(n 2 2)
(6.5)
 Specify:
(cid:129)
(cid:129)
    f  c , center frequency
    BW , 3 dB bandwidth; or  Q , quality factor

 The design equations are as follows:
Audio Filter Designs: IIR Filters  167
uc 5 2pfc/fs
W 5 fc/Q
b2 5 ea22p
b
BW
fs
b1 5
24b2
1 1 b2
 cos(uc)
a0 5 (1 2 b2)Å1 2
b2
1
4b2
(6.6)
 The resonator works by simple manipulation of the conjugate poles formed with the second-
order feed-back network. The b 2  term controls the distance out to the pole which makes the
resonant peak sharper (when the pole is close to the unit circle) or wider (when the pole is
farther away from the unit circle). The b 1  and b 2  terms control the angle of the pole, which
controls the center frequency, shown in  Figure 6.5 . The a 0  term is used to scale the ﬁ lter so its
peak output is always normalized to unity gain or 0 dB.
 The drawback to this design is that it is only symmetrical in frequency response at one
frequency, p/2, when the low-frequency and high-frequency magnitude vectors are
symmetrical. At all other frequencies, the response is shifted asymmetrically. When the pole
is in the ﬁ rst quadrant, it is nearer to the low frequencies than high frequencies and so the
Im
b2 controls the  radius
of the  pole
b1 and b2  control the
angle of
the pole
Re
R
R
 Figure 6.5:    The location of the resonator’s conjugate poles are
determined by the coefﬁ cients.

168  Chapter 6
Im
+12.0 dB
0.0  dB
–12.0  dB
Re
–24.0  dB
–36.0  dB
–48.0  dB
–60.0  dB
10Hz
100 Hz
1kHz
10 kHz
 Figure 6.6:    The asymmetrical response shape when the pole is in the ﬁ rst quadrant; notice
the difference in gain at DC versus Nyquist. This ﬁ lter has  f  s   = 44.1 kHz,
 f  c  = p/4 = 5.5125 kHz, and  Q  = 10.
Im
+12.0 dB
0.0 dB
–12.0 dB
Re
–24.0 dB
–36.0 dB
–48.0 dB
–60.0 dB
10Hz
100 Hz
1kHz
10kHz
 Figure 6.7:    The asymmetrical response shape when the pole is in the second quadrant; notice the
difference in gain at DC versus Nyquist. This ﬁ lter has  f  s  = 44.1 kHz,
 f  c  = 3p/4 =16.5375 kHz, and  Q  = 10.
low end is boosted ( Figure 6.6 ). The opposite happens ( Figure 6.7 ) when the pole moves to
the second quadrant and the high frequencies get the boost. The solution to the asymmetry
problem is a really useful ﬁ lter, especially for making ﬁ lter banks of extremely narrow band-
pass ﬁ lters.
   6.3.2  Smith-Angell Improved Resonator
 A block diagram of a Smith-Angell improved resonator is shown in  Figure 6.8 .
 The difference equation is as follows:
 y(n) 5 a0x(n) 1 a2x(n 2 2) 2 b1y(n 2 1) 2 b2y(n 2 2)
(6.7)
