# Audio Plugins C++ (Pirkle) — Chunk 24/73

> Source: `Designing Audio Effect Plug-Ins in C++_ With Digital Audio Signal Processing Theory - PDF Room.pdf` · Pages ~1-560 · Extractor: pdfminer.six
> Target ≈ 2800 words, Overlap = 200 words · Actual = 2613

> Note: DSP textbook content for SpectralCanvasPro RT-audio knowledge base

---
Basic DSP Theory  149
 Table 5.4:    Challenge answers.
Frequency (v)
Nyquist (p)
½ Nyquist (p/2)
¼ Nyquist (p/4)
zH(v)z
210.2 dB
22.56 dB
123.15 dB
Arg(H)
0.08
285.88
240.38
1.000
0.707
0.500
0.000
–0.500
–0.707
–1.000
0
17
34
51
68
85
102
119
136
153
 Figure 5.50:    Impulse response of the ﬁ lter from RackAFX.
   5.18.2  Challenge
 Finish the rest of the direct evaluation calculations on your own. The answers are in  Table 5.4 .
The exact evaluation once again produces results pretty close to our estimated version. You
can get the complete answers at the RackAFX websites.
  Step 6:  z  transform of impulse response
 The impulse response for this ﬁ lter is shown in  Figure 5.50 . Once again, ﬁ nding the impulse
response by hand is going to be tedious. Instead, I will use RackAFX to do the analysis so we
can compare our estimated and direct evaluation results. The measured responses are shown
in  Figure 5.51 .
    5.19    First-Order Pole-Zero Filter: The Shelving Filter
 The ﬁ rst-order pole-zero ﬁ lter consists of a ﬁ rst-order pole and ﬁ rst-order zero in the same
algorithm. The topology in the block diagram in  Figure 5.52  is a combination feed forward
and feed back since it contains both paths.
 The difference equation is as follows:
  y(n) 5 a0x(n) 1 a1x(n 2 1) 2 b1y(n 2 1)
(5.78)
  Steps 1 to 3: Take the  z  transform of the difference equation to get the transfer function,
then factor out a 0  as the scalar gain coefﬁ cient
Y(z) 5 a0X(z) 1 a1X(z)z21 2 b1Y(z)z21
(5.79)

150  Chapter 5
 Separate variables:
  Y(z) 1 b1Y(z)z21 5 a0X(z) 1 a1X(z)z21
 Y(z)31 1 b1z21 4 5 X(z)3a0 1 a1z21 4
 Form the transfer function:
 Factor out a0:
 where
 H(z) 5
Y(z)
X(z)
5
a0 1 a1z21
1 1 b1z21
 H(z) 5 a0
1 1 a1z21
1 1 b1z21
 a1 5
a1
a0
+24.0  dB
+16.0   dB
+8.0  dB
0.0  dB
–8.0  dB
–16.0   dB
–24.0   dB
+180 .0°
+120 .0°
+60.0°
0.0°
–60.0°
–120.0°
–180.0°
2 kHz  4 kHz  6 kHz  8 kHz  10 kHz  12 kHz  14 kHz  16 kHz  18 kHz  20 kHz
2 kHz  4 kHz  6 kHz  8 kHz  10 kHz  12 kHz  14 kHz  16 kHz  18 kHz  20 kHz
 Figure 5.51:    RackAFX’s frequency and phase responses are taken from the  z -transform
of the impulse response.
x(n)
a0
∑
y(n)
z –1
–1
Z
a1
–b1
 Figure 5.52:    First-order pole-zero ﬁ lter.

  Step 4: Estimate the frequency response
 This transfer function has one pole and one zero and both are ﬁ rst order. Like the other ﬁ rst-
order cases, we can ﬁ nd the pole and zero by inspection of the transfer function:
Basic DSP Theory  151
H(z) 5 a0
        5 a0
1 1 a1z21
1 1 b1z21
a1
z
b1
z
1 1
1 1
(5.80)
 In the numerator, you can see that if  z  5 2a 1  the numerator will go to zero and the transfer
function will go to zero. In the denominator, you can see that if  z  5 2b 1  the denominator
will go to zero and the transfer function will go to inﬁ nity. Therefore we have a zero
at  z  5 2a 1  and a pole at  z  5 2b 1 . For this example, use the following values for the
coefﬁ cients: a 0  5 1.0, a 1  5 20.92, b 1  5 20.71. Then, a 1  5 20.92, and so we now have
a zero at  z  5 2a 1  5 0.92 1  j 0 and a pole at  z  5 2b 1  5 0.71 1  j 0. The pole/zero pair
are plotted in  Figure 5.53 .
 Evaluating the frequency response when you have mixed poles and zeros is the same as
before, but you have to implement both magnitude steps.
•
•
•
   Locate each evaluation frequency on the outer rim of the unit circle.
   Draw a line from the point on the circle to  each  zero and measure the length of these
vectors. Do it for each evaluation frequency.
   Draw a line from the point on the circle to  each  pole and measure the length of these
vectors. Do it for each evaluation frequency.
Im
0.71
Re
0.92
 Figure 5.53:    The pole and zero are both purely real and plotted on the real axis in the  z -plane.

152  Chapter 5
•
•
•
   Multiply all the zero magnitudes together.
   Multiply all the  inverse  pole magnitudes together.
   Divide the zero magnitude by the pole magnitude for the ﬁ nal result at that frequency.
 Mathematically, this last rule looks like  Equation 5.81 :
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
(5.81)
 where
  N 5 the filter order
 Ui 5 the geometric length from the point (v) on the unit circle to the ith zero
 Vi 5 the geometric length from the point (v) on the unit circle to the ith pole
  Equation 5.81  is the ﬁ nal, generalized magnitude response equation for the geometric
interpretation of the pole/zero plots in the  z -plane. For completeness, here’s the equation for
calculating the phase response of a digital ﬁ lter using the geometric method:
N
  Arg(H(e jv))|v 5 a
i51
N
ui 2 a
i51
fi
(5.82)
 where
  N 5 the filter order
 ui 5 the angle between the ith zero and the vector Ui
fi 5 the angle between the ith pole and the vector Vi
  Equations 5.81  and  5.82  together complete the DSP theory for pole/zero interpretation for
estimating the frequency and phase responses of any ﬁ lter. So, let’s do the analysis for this
ﬁ lter—by going through the math, you can see the tug of war going on between the pole and
zero. Follow the analysis sequence in  Figures 5.54  through  5.58 .
  Step 5: Direct evaluation
 You can evaluate the ﬁ lter the same way as before using Euler’s equation to separate
the real and imaginary components from the transfer function. Evaluate at the following
frequencies:
•
•
•
•
   DC: 0
   Nyquist: p
   ½ Nyquist: p/2
   ¼ Nyquist: p/4

Im
0.08
H  (dB)
24
12
0
–12
–24
–36
Re
0.08  *
1
0.29
=  0.27  =  –11.1  dB
0.29
1/4  Nyquist
fs/8
1/2 Nyquist
fs/4
Basic DSP Theory  153
Frequency
Nyquist
fs/2
 Figure 5.54:    The magnitude response at DC is 211.1 dB. Look at the equation and
you can see the zero value bringing down the total while the pole value is trying to push it
back up. In this case, the zero wins and the response is down almost 12 dB. Geometrically,
you can see this is because the zero is closer to the evaluation point and so it has more
effect on the outcome.
Im
/4
H  (dB)
24
1.70
1.75
Re
12
0
–12
–24
–36
1.75  *
1
1.70
=  1.02  =  0.17  dB
1/4  Nyquist
fs/8
1/2  Nyquist
fs/4
Frequency
Nyquist
fs/2
 Figure 5.55:    The magnitude response at p is almost unity gain because the pole and zero
distances are almost the same. The tug of war ends in stalemate here at 0.17 dB of gain.
π /2
Im
1.40
1.30
Re
H  (dB)
24
12
0
–12
–24
–36
1.40 *
1
1.30
=  1.07  =  0.64  dB
1/4  Nyquist
fs/8
1/2  Nyquist
fs/4
Frequency
Nyquist
fs/2
 Figure 5.56:    With the pole slightly closer to the evaluation frequency, the magnitude response at
p/2 picks up a bit to 10.64 dB.
π
154  Chapter 5
π
Im
1.71
1.92
H  (dB)
24
12
0
–12
–24
–36
Re
1.92  *
1
1.71
=  1.12  =  1.00  dB
1/4 Nyquist
fs/8
½ Nyquist
fs/4
Frequency
Nyquist
fs/2
 Figure 5.57:    At p/4 the pole/zero ratio favors the pole and the response perks up to
1.0 dB; notice that this is the frequency where the pole is clearly dominating,
but just barely.
H  (dB)
24
12
0
–12
–24
–36
The Shelf
1/4  Nyquist
½  Nyquist
fs/ 8
fs/4
Frequency
Nyquist
fs/2
 Figure 5.58:    The composite frequency response plot shows a 212 dB low shelving ﬁ lter
response, a useful ﬁ lter in audio.
 First, get the transfer function in a form to use for all the evaluation frequencies
( Equation 5.83 ). Then evaluate at our four frequencies.
 H (z) 5
 5
a0 1 a1z21
1 1 b1z21
1 2 0.92z21
1 2 0.71z21
  Let z 5 ejv
H (v) 5
1 2 0.92e2j1v
1 2 0.71e2j1v
(5.83)

 Apply Euler’s equation:
  5.19.1  DC (0Hz)
Basic DSP Theory  155
(5.84)
(5.85)
 H (v) 5
H (v) 5
1 2 0.92e2j1v
1 2 0.71e2j1v
1 2 0.923cos(v) 2 j sin(v)4
1 2 0.713cos(v) 2 j sin(v)4
  H (v) 5
 5
 5
 5
1 2 0.923cos(v) 2 j sin(v)4
1 2 0.713cos(v) 2 j sin(v)4
1 2 0.923cos(0) 2 j sin(0)4
1 2 0.713cos(0) 2 j sin(0)4
1 2 0.9231 2 j04
1 2 0.7131 2 j04
0.08 1 j0
0.29 1 j0
   0 H(v) 0 5
 5
 5
0 0.08 1 j0 0
0 0.29 1 j0 0
"a2 1 b2
"a2 1 b2
"0.082
"0.292
5 0.276 5 2 11.2 dB
  Arg(H) 5 Arg(Num) 2 Arg(Denom)
 5 tan21(0/0.08) 2 tan21(0/0.29)
 5 08
 By now this should be getting very familiar; the only difference in this situation is that we
have to evaluate the numerator and denominator, but the method is the same.
   15.19.2  Challenge
 Finish the rest of the direct evaluation calculations on your own. The answers are in  Table 5.5 .
  Table 5.5:    Challenge answers.
Frequency (v)
Nyquist (p)
½ Nyquist (p/2)
¼ Nyquist (p/4)
zH(v)z
1.00 dB
0.82 dB
0.375 dB
Arg(H)
0.0o
7.23o
16.60o

156  Chapter 5
 Thus, once again the direct evaluation backs up the estimation from the  z -plane. Because
we have a feedback path, extracting the impulse response will be tedious but we can use
RackAFX’s pole/zero ﬁ lter module to analyze the impulse response.  Figure 5.59  shows the
measured impulse response, while  Figure 5.60  shows the frequency and phase responses.
1.000
0.707
0.500
0.000
–0.500
–0.707
–1.000
0
17
34
51
68
85
102
119
136
153
 Figure 5.59:    Impulse response of the ﬁ rst-order shelving ﬁ lter.
+12.0  dB
+6.0  dB
0.0  dB
–6.0  dB
–12.0  dB
–18.0  dB
–24.0  dB
+12.0  dB
+6.0  dB
0.0  dB
–6.0  dB
–12.0  dB
–18.0  dB
–24.0  dB
+90.0°
+60.0°
+30.0°
0.0°
–30.0°
–60.0°
–90.0°
2 kHz  4  kHz  6 kHz  8 kHz   10 kHz  12 kHz  14 kHz  16 kHz  18 kHz  20  kHz
10 Hz
100 Hz
1  kHz
10  kHz
10 Hz
100 Hz
1 kHz
10 kHz
 Figure 5.60:    Frequency and phase responses of the ﬁ rst-order shelving ﬁ lter.
x(n)
a0
∑
y (n)
Basic DSP Theory  157
–1
Z
z–1
a1
a2
–1 z
–1 z
–b1
–b2
 Figure 5.61:    The bi-quad.
 A ﬁ rst-order shelving ﬁ lter is a pole-zero design. The shelf will be located in the region
between where the zero dominates and where the pole dominates. When neither really
dominates, the response is approximately unity gain. The RackAFX frequency and phase
plots are shown in  Figure 5.60 ; the log frequency plot has been included to reveal the
textbook shelving ﬁ lter curve.
    5.20    The Bi-Quadratic Filter
 The last ﬁ lter topology to study is the bi-quadratic (or bi-quad) ﬁ lter. The bi-quad consists of
two second-order components: a second-order feed-forward and a second-order feed-back
ﬁ lter combined together as shown in the block diagram in  Figure 5.61 . The resulting transfer
function will have two quadratic equations, thus the name.
 The difference equation is as follows:
  y(n) 5 a0x(n) 1 a1x(n 2 1) 1 a2x(n 2 2) 2 b1y(n 2 1) 2 b2y(n 2 2)
(5.86)
  Steps 1 to 3: Take the  z  transform of the difference equation to get the transfer function,
then factor out a 0  as the scalar gain coefﬁ cient
y(n) 5 a0x(n) 1 a1x(n 2 1) 1 a2x(n 2 2) 2 b1y(n 2 1) 2 b2y(n 2 2)
Y(z) 5 a0X(z) 1 a1X(z)z21 1 a2X(z)z22 2 b1Y(z)z21 2 b2Y(z)z22
 Separate variables:
 Y(z) 1 b1Y(z)z21 1 b2Y(z)z22 5 a0X(z) 1 a1X(z)z21 1 a2X(z)z22
 Y(z)31 1 b1z21 1 b2z22 4 5 X(z)3a0 1 a1z21 1 a2z22 4
 Form transfer function
 H(z) 5
Y(z)
X(z)
5
a0 1 a1z21 1 a2z22
1 1 b1z21 1 b2z22

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
