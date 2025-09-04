# Audio Plugins C++ (Pirkle) — Chunk 22/73

> Source: `Designing Audio Effect Plug-Ins in C++_ With Digital Audio Signal Processing Theory - PDF Room.pdf` · Pages ~1-560 · Extractor: pdfminer.six
> Target ≈ 2800 words, Overlap = 200 words · Actual = 2554

> Note: DSP textbook content for SpectralCanvasPro RT-audio knowledge base

---
Basic DSP Theory  131
1/0.1  = 10
Frequency
Nyquist
fs/2
Angle
1/1.9 =  0.5 2
1/1.8  =  0.5 5
1/1.3  =  0.74
1/4  Nyquist
fs/8
½ Nyquist
fs/4
 Figure 5.30:    The ﬁ nal frequency and phase response plots.
H
10. 0
0.74
0.55
0.52
0.0
o
90
o
45
o
0
1.000
0.707
0.500
0.000
-0.500
-0.707
-1.000
0
17
34
51
68
8 5
10 2
119
136
153
 Figure 5.31:    The impulse response of the ﬁ lter in question.
 In  Figure 5.32  we observe excellent agreement with our evaluation; the response is down 26
dB at DC (0.52) and 120 dB at Nyquist (10) and the phase is 45 degrees at p/2. You followed
six steps in evaluation of this ﬁ lter:
1.
2.
3.
   Take the  z  transform of the difference equation.
   Fashion the difference equation into a transfer function.
   Factor out a 0  as the scalar gain coefﬁ cient.
132  Chapter 5
+24.0 dB
+12.0 dB
0.0 dB
–
12.0 dB
– 24.0 dB
– 36.0 dB
– 48.0 dB
+90.0
°
+60.0
°
+30.0
°
0.0
°
°
°
–90.0 °
–30.0
–60.0
2 kHz  4 kHz  6 kHz  8 kHz  10 kHz  12 kHz  14 kHz  16 kHz  18 kHz  20 kHz
2 kHz  4 kHz  6 kHz  8 kHz  10 kHz  12 kHz  14 kHz  16 kHz  18 kHz  20 kHz
 Figure 5.32:    RackAFX’s frequency and phase responses are taken from the  z  transform of the
impulse response. Compare these measured results to the response we predicted using direct
evaluation in  Figure 5.30 —notice these are plotted in dB rather than raw magnitudes.
x(n)
a o
∑
y(n)
-1
Z
-1
Z
a 1
a 2
x (n –1 )
x(n– 2)
 Figure 5.33:    Second-order feed-forward ﬁ lter.
4.
5.
6.
   Estimate the frequency response.
   Direct evaluation of frequency response.
    z  transform of impulse response as a ﬁ nal check.
    5.17    Second-Order Feed-Forward Filter
 Analysis of the second-order feed-forward ﬁ lter proceeds much like the ﬁ rst-order ﬁ lters
you’ve seen so far, but there’s a bit more math we have to deal with. The topology of a
second-order feed-forward ﬁ lter is shown in the block diagram in  Figure 5.33 .
Basic DSP Theory  133
 The difference equation is as follows:
  y(n) 5 a0x(n) 1 a1x(n 2 1) 1 a2x(n 2 2)
(5.53)
  Steps 1 & 2: Take the  z  transform of the difference equation and fashion it into a
transfer function
 We can combine steps to save time. The  z  transform can be taken by inspection, using the
rules from  Section 5.8 , and then you need to get it into the form  Y ( z )/ X ( z ) for the transfer
function in  Equation 5.54 .
   y(n) 5 a0x(n) 1 a1x(n 2 1) 1 a2x(n 2 2)
 Y(z) 5 a0X(z) 1 a1X(z)z21 1 a2X(z)z22
 5 X(z)3a0 1 a1z21 1 a2z22 4
Form the transfer function H(z):
 H(z) 5
output
input
5
Y(z)
X(z)
5 a0 1 a1z21 1 a2z22
  Step 3: Factor out a 0  as the scalar gain coefﬁ cient
 We’ll need to make some substitutions to get this in the form we are used to, shown in
 Equation 5.55 :
H(z) 5 a0 1 a1z21 1 a2z22
Let a1 5
a1
a0
    a2 5
a2
a0
 H(z) 5 a0(1 1 a1z21 1 a2z22)
(5.54)
(5.55)
  Step 4: Estimate the frequency response
 First, this is a pure feed-forward ﬁ lter, so you know there will only be nontrivial
zeros; there are no poles to deal with. This transfer function is a second-order function
because of the  z  22  term. In fact, this is a quadratic equation. In order to ﬁ nd the poles
or zeros, you need to ﬁ rst factor this equation and ﬁ nd the roots. The problem is that
this is a complex equation, and the roots could be real, imaginary, or a combination
of both. The mathematical break that we get is that our coefﬁ cients a 1  and a 2  are  real
numbers . The only way that could work out is if the locations of the zeros are complex
conjugates of one another. When you multiply complex conjugates together the imaginary
component disappears. So, with some algebra, you can arrive at the deduction shown in
 Equation 5.56 :
H(z) 5 1 1 a1z21 1 a2z22
(5.56)

134  Chapter 5
 can be factored as
 where
H(z) 5 (1 2 Z1z21)(1 2 Z2z21)
ju 5 a 1 jb
  Z1 5 Re
 Z2 5 Re2ju 5 a 2 jb
 This analysis results in two zeros, Z 1  and Z 2 , located at complex conjugate positions in the
 z -plane.  Figure 5.34  shows an arbitrary conjugate pair of zeros plotted in the  z -plane. You can
see how they are at complementary angles to one another with the same radii. Remember, any
arbitrary point in the  z -plane is located at Re ju  and the outer rim of the circle is evaluated for
R 5 1 and u between 2p and 1p (or 0 to 2p).
 But, how do the complex conjugate pair of zeros at Re ju  and Re 2ju  relate to the coefﬁ cients
a 1  and a 2 ? The answer is to just multiply everything out, use Euler’s equation, and compare
functions as shown in  Equation 5.57 :
 H(z) 5 a0(1 1 a1z21 1 a2z22)
  5 a0(1 2 Z1z21)(1 2 Z2z21)
(5.57)
 where
ju
 Z1 5 Re
 Z2 5 Re2ju
    1 1 2 Z1z21 2 1 1 2 Z2z21 2 5 1 1 2 Re
 5 1 2 Re
juz21 2 1 1 2 Re2juz21 2
juz21 2 Re2juz21 1 R2 1 e
jue2ju 2 z22
Im
R
θ
–θ
R
Re
 Figure 5.34:    A complementary pair of zeros in the  z -plane.

Basic DSP Theory  135
 noting that (e
jue2ju) 5 e
ju 2ju 5 e0 5 1
ju 1 Re2ju)z21 1 R2z22
 5 1 2 (Re
 5 1 2 R(cos(u) 1 jsin(u) 1 cos(u) 2 jsin(u))z21 1 R2z22
 5 1 2 2Rcos(u)z21 1 R2z22
 compare functions:
 then
  H(z) 5 a0(1 1 a1z21 1 a2z22)
 5 a0(1 2 2Rcos(u)z21 1 R2z22)
  a1 5 22Rcos(u)
 a2 5 R2
  Equation 5.57  shows how the coefﬁ cients a 1  and a 2  create the zeros at the locations Re ju  and
Re 2ju . Once again you see that the coefﬁ cients  are  the ﬁ lter—they determine the locations of
the zeros, and these determine the frequency and phase responses of the ﬁ lter. To estimate,
we’ll need some coefﬁ cients to test with. Use the following: a 0  5 1.0, a 1  5 21.27, a 2  5 0.81.
Now, calculate the location of the zeros from  Equation 5.35 ; since a 0  5 1.0, then a 1  5 21.27
and a 2  5 0.81. Start with a 2  then solve for a 1  as shown in  Equation 5.58 . The zeros are
plotted in  Figure 5.35 .
 R2 5 a2 5 0.81
 R 5 "0.81 5 0.9
(5.58)
Im
0.9
45°
–45°
0.9
Re
 Figure 5.35:    The complementary pair of zeros in the  z -plane at radii 0.9 and
angles 645 degrees.

136  Chapter 5
  then
   22R cos(u) 5 21.27
2(0.9)cos(u) 5 1.27
 cos(u) 5
1.27
2 (0.9)
 u 5 arccos(0.705)
 u 5 458
 Evaluating the frequency response of the complex pair is similar to before, but with an extra
step. When estimating the frequency response with more than one zero:
•
•
•
   Locate each evaluation frequency on the outer rim of the unit circle.
   Draw a line from the point on the circle to  each  zero and measure the length of these
vectors. Do it for each evaluation frequency.
   For each evaluation frequency, the magnitude of the transfer function is the  product of
the two vectors  to each zero pair.
 Mathematically, this last rule looks like  Equation 5.59 :
  0 H(ejv) 0
N
v 5 a0 q
i51
Ui
(5.59)
 where
  N 5 filter order
Ui 5 geometric distance from the point v on the unit circle to the ith pole
 Follow the progression in  Figures 5.36  through  5.39  through the four evaluation frequencies,
starting at DC (0 Hz). Finally, put it all together in one plot and sketch the magnitude
response in  Figure 5.40 .
I m
0.7
0.7
Re
H
4.0
2.0
0.49
0.0
0.7*0.7  0.49
1 4  Nyquist
fs  8
1
2
Nyquist
fs 4
Frequency
Nyquist
fs 2
 Figure 5.36:    The magnitude response at 0 Hz (DC) is the product of the two vectors
drawn to each zero, or 0.49.

Basic DSP Theory  137
Im
π/4
0.1
1.4
Re
H
4.0
2.0
0.49
0.14
0.0
0.1*1.4=0.14
1/4 Nyquist
fs/8
½  Nyquist
fs/4
Frequency
Nyquist
fs/2
 Figure 5.37:    At ¼ Nyquist, the two vectors multiply out to 0.14.
π/2
Im
0.7
1.8
Re
H
4.0
2.0
1.26
0.49
0.14
0.0
0.7*1.8=1.26
1/4  Nyquist
 ½ Nyquist
fs/8
fs/4
Frequency
Nyquist
fs/2
 Figure 5.38:    At ½ Nyquist, the response reaches 1.26 as the vectors begin
to stretch out again.
Im
1.75
1.75
π
H
4.0
3.1
2.0
1.26
0.49
0.14
0.0
Re
1.75*1.75 = 3.1
1/4  Nyquist
fs/8
½ Nyquist
fs/4
Frequency
Nyquist
fs/2
 Figure 5.39:    At Nyquist, the response reaches a maximum of 3.7 as the vectors stretch out
to their longest possible lengths.
138  Chapter 5
H
4.0
3.1
2.0
1.26
0.49
0.14
0.0
1/4  Nyquist
fs/8
1/2 Nyquist
fs/4
Frequency
Nyquist
fs/2
 Figure 5.40:    The combined response reveals a band-stop (notch) type of ﬁ lter.   The minimum
amplitude occurs at the zero frequency, where the vector product is the lowest; this is where the
smallest vector is obtained when evaluating on the positive frequency arc.
  Step 5: Direct evaluation
 Now you can evaluate the ﬁ lter the same way as before using Euler’s equation to separate
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
 First, get the transfer function in the form of  Equation 5.60   to use for all the evaluation
frequencies:
         H(z) 5 a0(1 1 a1z21 1 a2z22)
a1
a0
a2
a0
Where: a1 5
       a2 5
         H(z) 5 1 2 1.27z21 1 0.81z22
 Let:  z 5 ejv
(5.60)
 H(v) 5 1 2 1.27e2j1v 1 0.81e2j2v

Basic DSP Theory  139
Apply Euler’s Equation:
 H1 v 2 5 1 2 1.273cos 1 v 2 2 j sin1 v 2 4 1 0.813cos 1 2v 2 2 j sin 1 2v 2 4
 Now evaluate for each of our four frequencies in  Equations 5.61  through  5.68 .
  5.17.1  DC (0 Hz)
  H (v) 5 1 2 1.273cos(v) 2 jsin(v)4 1 0.813cos(2v) 2 jsin(2v)4
 5 1 2 1.273cos(0) 2 j sin(0)4 1 0.813cos(2*0) 2 j sin(2*0)4
 5 1 2 1.2731 2 j04 1 0.8131 2 j04
 5 1 2 1.27 1 0.81
 5 0.54 1 j0
   0 H(v) 0 5 "a2 1 b2
 5 "0.542 1 02
 5 0.54
  Arg(H) 5 tan21(b/a)
 5 tan21(0/0.54)
 5 0.08
(5.61)
(5.62)
 The exact magnitude is 0.54, which is pretty close to our estimated value of 0.49 using the
 z -plane graphical method.
   5.17.2  Nyquist (p)
  H (v) 5 1 2 1.273cos(v) 2 j sin(v)4 1 0.813cos(2v) 2 j sin(2v)4
 5 1 2 1.273cos(p) 2 j sin(p)4 1 0.813cos(2p) 2 j sin(2p)4
 5 1 2 1.27321 2 j04 1 0.8131 2 j04
 5 1 1 1.27 1 0.81
 5 3.08
  0 H(v) 0 5 "a2 1 b2
 5 "3.082 1 02
 5 3.08
  Arg(H ) 5 tan21(b/a)
 5 tan21(0/3.08)
 5 0.08
(5.63)
(5.64)

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
