# Audio Plugins C++ (Pirkle) — Chunk 23/73

> Source: `Designing Audio Effect Plug-Ins in C++_ With Digital Audio Signal Processing Theory - PDF Room.pdf` · Pages ~1-560 · Extractor: pdfminer.six
> Target ≈ 2800 words, Overlap = 200 words · Actual = 2627

> Note: DSP textbook content for SpectralCanvasPro RT-audio knowledge base

---
140  Chapter 5
 The exact magnitude is 3.08, which is pretty close to our estimated value of 3.1 using the
 z -plane graphical method.
   5.17.3  ½ Nyquist (p/2)
   H (v) 5 1 2 1.273cos(v) 2 j sin(v)4 1 0.813cos(2v) 2 j sin(2v)4
 5 1 2 1.273cos(p/2) 2 j sin(p/2)4 1 0.813cos(2p/2) 2 j sin(2p/2)4
 5 1 2 1.273cos(p/2) 2 j sin(p/2)4 1 0.813cos(p) 2 j sin(p)4
 5 1 2 1.2730 2 j14 1 0.81321 2 j04
 5 1 1 j1.27 2 0.81
 5 0.19 1 j1.27
   0 H (v) 0 5 "a2 1 b2
 5 "0.192 1 1.272
 5 1.28
  Arg(H) 5 tan21(b/a)
 5 tan21(1.27/0.19)
 5 828
(5.65)
(5.66)
 The exact magnitude is 1.28, which is pretty close to our estimated value of 1.26 using the
 z -plane graphical method.
   5.17.4  ¼ Nyquist (p/4)
   H(v) 5 1 2 1.273cos(v) 2 j sin(v)4 1 0.813cos(2v) 2 j sin(2v)4
 5 1 2 1.273cos(p/4) 2 j sin(p/4)4 1 0.813cos(2p/4) 2 j sin(2p/4)4
 5 1 2 1.273cos(p/4) 2 j sin(p/4)4 1 0.813cos(p/2) 2 j sin(p/2)4
 5 1 2 1.2730.707 2 j0.707 4 1 0.8132j14
 5 0.11 1 j0.08
(5.67)
   0 H(v) 0 5 "a2 1 b2
 5 "0.112 1 0.082
 5 0.136
  Arg(H) 5 tan21(b/a)
 5 tan21(0.08/0.11)
 5 368
(5.68)
 The exact magnitude is 0.136, which is pretty close to our estimated value of 0.14 using the
 z -plane graphical method.

Basic DSP Theory  141
  Step 6:  z  transform of impulse response
 This second-order feed-forward ﬁ lter is actually pretty easy to examine for its impulse
response. For an impulse stimulus, the impulse response  h ( n ) is  h ( n ) 5 {1.0, 21.27, 0.81}.
Taking the  z  transform of the impulse response is easy, as shown in  Equation 5.69 :
n51`
   H (z) 5 a
n52`
h(n)z2n
 5 1.0z0 2 1.27z21 1 0.81z22
(5.69)
 5 1 2 1.27z21 1 0.81z22
 This is exactly what we expect. This should help you understand two more very important
details about pure feed-forward ﬁ lters.
 In a pure feed-forward ﬁ lter:
 •    The coefficients {a 0 , a 1 , a 2 ,…} are the impulse response,  h ( n ).
 •    The transfer function is the  z  transform of the coefficients.
 Finally, I’ll use RackAFX to verify the frequency and phase response from our analysis by
using a plug-in I wrote for second-order feed-forward ﬁ lters.  Figure 5.41  shows the frequency
and phase response plots.
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
2 kHz  4 kHz  6 kHz  8 kHz  10 kHz  12 kHz  14 kHz  16 kHz  18 kHz  20 kHz
2 kHz  4 kHz  6 kHz  8 kHz  10 kHz  12 kHz  14 kHz  16 kHz  18 kHz  20 kHz
 Figure 5.41:    Plots using RackAFX’s  z -transform of the impulse response.

142  Chapter 5
    5.18    Second-Order Feed-Back Filter
 Analysis of the second-order feed-back ﬁ lter starts with the block diagram and difference
equation.  Figure 5.42  shows the topology of a second-order feed-back ﬁ lter.
 The difference equation is as follows:
  y(n) 5 a0x(n) 2 b1y(n 2 1) 2 b2y(n 2 2)
(5.70)
  Steps 1 to 3: Take the  z  transform of the difference equation to get the transfer function,
then factor out a 0  as the scalar gain coefﬁ cient
 We’ll continue to combine steps. Once again, the z transform can be taken by inspection using
the rules from  Section 5.8 , and then you need to get it into the form  Y ( z )/ X ( z ) for the transfer
function in  Equation 5.71 .
 Separate variables:
 Form transfer function:
 Factor out a0:
   y(n) 5 a0x(n) 2 b1y(n 2 1) 2 b2y(n 2 2)
  Y(z) 5 a0X(z) 2 b1Y(z)z21 2 b2Y(z)z22
  Y (z) 1 b1Y (z)z21 1 b2Y (z)z22 5 a0X (z)
 Y (z)31 1 b1z21 1 b2z22 4 5 a0X (z)
 H (z) 5
Y (z)
X (z)
5
a0
1 1 b1z21 1 b2z22
 H (z) 5 a0
1
1 1 b1z21 1 b2z22
x(n)
a0
y(n)
(5.71)
–1
Z
–1
Z
–b1
–b2
 Figure 5.42:    Second-order feed-back ﬁ lter.

Basic DSP Theory  143
  Step 4: Estimate the frequency response
 Now you can see why the feed-back ﬁ lter block diagrams have all the  b  coefﬁ cients negated.
It puts the quadratic denominator in the ﬁ nal transfer function in Equation 5.71  in the same
polynomial form as the numerator of the feed-forward transfer function. Thus, you can use
the same logic to ﬁ nd the poles of the ﬁ lter; since the coefﬁ cients b 1  and b 2  are real values, the
poles must be complex conjugates of each other, as shown in  Equation 5.72 :
 can be factored as
  H (z) 5 a0
1
1 1 b1z21 1 b2z22
 H (z) 5 a0
1
(1 2 P1z21)(1 2 P2z21)
 where
 therefore
 and
 P1 5 Reju
 P2 5 Re2ju
  1 1 2 P1z21 2 1 1 2 P1z21 2 5 1 2 2R cos(u)z21 1 R2z22
(5.72)
  H (z) 5 a0
1
1 1 b1z21 1 b2z22
  5 a0
1
(1 2 2R cos(u)z21 1 R2z22)
  b1 5 22R cos(u)
 b2 5 R2
 This results in two poles, P 1  and P 2 , located at complex conjugate positions in the  z -plane.
  Figure 5.43  shows an arbitrary conjugate pair of poles plotted in the  z -plane. You can see how
they are at complementary angles to one another with the same radii.
 To estimate we’ll need some coefﬁ cients to test with. Use the following: a 0  5 1.0, b 1  5 21.34,
b 2  5 0.902. Now, calculate the location of the poles from  Equation 5.72 :
 then
  R2 5 b2 5 0.902
 R 5 "0.902 5 0.95
 22R cos(u) 5 21.34
 2(0.95)cos(u) 5 1.34
(5.73)

144  Chapter 5
Im
R
R
θ
–θ
Re
 Figure 5.43:    A complementary pair of poles in the  z -plane.
Im
0.95
45°
–45°
0.95
Re
 Figure 5.44:    The poles of the ﬁ lter.
 cos(u) 5
1.34
2(0.95)
 u 5 arccos(0.707)
 u 5 458
  Figure 5.44  shows the complex conjugate pair of poles plotted in the  z -plane at angles 645 8
and radii of 0.95. Evaluating the frequency response of the complex pair is similar to before,
but with an extra step. When estimating the frequency response with more than one pole:
•
•
•
   Locate each evaluation frequency on the outer rim of the unit circle.
   Draw a line from the point on the circle to  each  pole and measure the length of these
vectors. Do it for each evaluation frequency.
   For each evaluation frequency, the magnitude of the transfer function is the  product of the
inverse lengths  of the two vectors to each pole pair.

 Mathematically, this last rule looks like  Equation 5.74 :
  0 H(ejv) 0
v 5 a0
1
N
q
i51
Vi
Basic DSP Theory  145
(5.74)
 where
  N 5 the filter order
Vi 5 the geometric length from the point (v) on the unit circle to the ith pole
 Thus, the process is the same as with the zeros, except that you take the inverse of the length
to the pole.
 For feed-forward ﬁ lters:
•
•
   The closer the frequency is to the zero, the more attenuation it receives.
   If the zero is  on  the unit circle, the magnitude would go to zero at that point.
 For feed-back ﬁ lters:
•
•
   The closer the evaluation frequency is to the pole, the more gain it receives.
   If a pole is  on  the unit circle, the magnitude would theoretically go to inﬁ nity, and it
would produce an oscillator, ringing forever at the pole frequency.
 You blew up the ﬁ rst-order feed-back ﬁ lter as an exercise in  Chapter 4 . All feed-
back ﬁ lters are prone to blowing up when their poles go outside the unit circle.
We can now continue with the estimation process for our standard four evaluation
frequencies. This time, we’ll convert the raw magnitude values into dB. The reason
for doing this is that there will be a very wide range of values that will be difﬁ cult to
sketch if we don’t use dB. Follow the evaluation sequence in  Figures 5.45  through
 5.48 . Finally, you can put it all together to form the frequency response plot in
 Figure 5.49 .
 In a digital filter:
 •    Zeros may be located anywhere in the  z -plane, inside, on, or outside the unit circle since the
filter is always stable; it’s output can’t go lower than 0.0.
 •    Poles must be located inside the unit circle.
 •    If a pole is on the unit circle, it produces an oscillator.
 •    If a pole is outside the unit circle, the filter blows up as the output goes to infinity.

146  Chapter 5
Im
H  (dB)
24
12
0
–12
–24
–36
0.71
0.71
Re
1
0.71
*
1
0.71
= 1.98 =  5.9dB
1/4  Nyquist
fs/8
½ Nyquist
fs/4
Frequency
Nyquist
fs/2
 Figure 5.45:    The magnitude response at 0 Hz (DC) is the product of the inverse of the
two vectors drawn to each zero, or (1/0.71)(1/0.71) 5 5.9 dB.
Im
π
4
0.05
Re
1.41
H
(dB)
24
12
0
–12
–24
–-36
1
0.05
*
1
1.41
= 14.2  =  23.0  dB
1/4  Nyquist
fs/8
1/2  Nyquist
fs/4
Frequency
Nyquist
fs/2
 Figure 5.46:    The magnitude response at 1/4 Nyquist is a whopping 123 dB since the
inverse of 0.05 is a large number.
π  2
Im
0.71
1.84
Re
H  (dB)
24
12
0
–12
–24
–36
1
0.71
*
1
1.84
=  0.76  =  -2.32dB
1/4  Nyquist
fs/8
1/2  Nyquist
fs/4
Frequency
Nyquist
fs/2
 Figure 5.47:    The magnitude response at p/2 is 22.98 dB.
71 2
Im
1.8
1.8
ji
H
(dB)
24
12
0
–12
–24
–36
Re
Basic DSP Theory  147
1
1.8
1
1.8
= 0.31 =–10.1 dB
1/4 Nyquist
fs/8
V-z Nyquist
fs/4
Frequency
Nyquist
fs/2
 Figure 5.48:    The magnitude response at Nyquist is 210.1 dB.
H
(dB)
24
12
0
–12
–24
–36
1/4 Nyquist
fs/8
½ Nyquist
fs/4
Frequency
Nyquist
fs/2
 Figure 5.49:    The composite magnitude response of the ﬁ lter shows that it is a resonant low-pass
ﬁ lter; the resonant peak occurs at the pole frequency.
  Step 5: Direct evaluation
 Now you can evaluate the ﬁ lter the same way as before using Euler’s equation to separate the
real and imaginary components from the transfer function. Evaluate at the following frequencies:
•
•
•
•
   DC: 0
   Nyquist: p
   ½ Nyquist: p/2
   ¼ Nyquist: p/4
 First, get the transfer function in a form to use for all the evaluation frequencies:
 H (z) 5 a0
1
1 1 b1z21 1 b2z22
1
1 2 1.34z21 1 0.902z22
 5
(5.75)

148  Chapter 5
  Let z 5 ejv.
 H (v) 5
1
1 2 1.34e2j1v 1 0.902e2j2v
 Apply Euler’s equation:
  H (v) 5
1
1 2 1.34e2j1v 1 0.902e2j2v
1
1 2 1.343cos(v) 2 j sin(v)4 1 0.902 3cos(2v) 2 j sin(2v)4
 H (v) 5
 Now evaluate for each of our four frequencies starting with DC.
  5.18.1  DC (0 Hz)
   H (v) 5
1
1 2 1.343cos(v) 2 1 2 j0 j sin(v)4 1 0.902 3cos(2v) 2 j sin(2v)4
 5
 5
 5
 5
1
1 2 1.343cos(0) 2 j sin(0)4 1 0.902 3cos(2*0) 2 j sin(2*0)4
1
1 2 1.3431 2 j04 1 0.902
1
1 2 1.34 1 0.902
1
0.562 1 j0
 H (v) 5
0 H (v) 0 5
 5
 5
1
0.562 1 j0
0 1 0
0 0.562 1 j0 0
1
"a2 1 b2
1
"0.5622
5 1.78 5 5.00 dB
   Arg (H) 5 Arg (Num) 2 Arg (Denom)
 5 tan21(0/1) 2 tan21(0/0.562)
 5 08
(5.76)
(5.77)
 Remember that for magnitudes of fractions, you need to take the magnitude of the numerator
and denominator separately; also for phase, the total is the difference of the Arg(num) and
Arg(denom). The direct evaluation yields 5.0 dB and shows our sketch evaluation was a little
off at 5.9 dB.

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
