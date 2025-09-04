# Audio Plugins C++ (Pirkle) — Chunk 21/73

> Source: `Designing Audio Effect Plug-Ins in C++_ With Digital Audio Signal Processing Theory - PDF Room.pdf` · Pages ~1-560 · Extractor: pdfminer.six
> Target ≈ 2800 words, Overlap = 200 words · Actual = 2579

> Note: DSP textbook content for SpectralCanvasPro RT-audio knowledge base

---
124  Chapter 5
  Step 2: Fashion the difference equation into a transfer function
 Now apply some algebra to convert the transformed difference equation to  H ( z ). The
process is always the same: separate the  X ( z ) and  Y ( z ) variables, then form their quotient
( Equation 5.38 ).
 Separate variables:
 From H(z):
  Y(z) 5 a0X(z) 2 b1Y(z)z21
  Y(z) 1 b1Y(z)z21 5 a0X(z)
 Y(z)31 1 b1z21 4 5 a0X(z)
 Y(z) 5
a0X(z)
1 1 b1z21
H(z) 5
Y(z)
X(z)
5
a0
1 1 b1z21
(5.38)
  Step 3: Factor out a 0  as the scalar gain coefﬁ cient
 In this case, this step is simple since pulling a 0  out is trivial, as in  Equation 5.39 . However, in
more complex ﬁ lters this requires making substitutions as you did in the last section.
  H(z) 5
a0
1 1 b1z21
5 a0
1
1 1 b1z21
(5.39)
   5.16    The Poles of the Transfer Function
 The next step in the sequence is to do a quick estimation of the frequency response using
the graphical interpretation method in the  z -plane. The pure feed-forward ﬁ lter you analyzed
produced zeros of transmission or zeros at frequencies where its output becomes zero. A pure
feed-back ﬁ lter produces  poles  at frequencies where its output becomes inﬁ nite. We were able
to make this happen by applying 100% feed-back in the last chapter. For the simple ﬁ rst-order
case, ﬁ nding the poles is done by inspection.
 When the denominator of the transfer function is zero, the output is infinite. The complex
 frequency where this occurs is the pole frequency or pole.

 Examining the transfer function, we can ﬁ nd the single pole in  Equation 5.40 :
Basic DSP Theory  125
   H(z) 5 a0
 5 a0
 5 a0
1
1 1 b1z21
1
1 1
b1
z
z
z 1 b1
(5.40)
 By rearranging the transfer function, you can see that the denominator will be zero when
 z  5 2b 1  and so there is a pole at  z  5 2b 1 . You might also notice something interesting
about this transfer function—it has a  z  in the numerator. If  z  5 0, then this transfer function
has a zero at  z  5 0. This zero is called a  trivial zero  because it has no impact on the ﬁ lter’s
frequency response. Thus, you can ignore the zero at  z  5 0. In fact, you can also ignore poles
at  z  5 0 for the same reason.
 And, if you look back at the transfer function in the feed-forward ﬁ lter in  Equation 5.35  you
can see that it also had a pole in  Equation 5.41 :
 H(z) 5 a0
31 1 a1z21 4
 5 a0 c1 1
a1
z
d
 Pole at  z  5 0
(5.41)
 The poles are plotted in the  z -plane in the same manner as the zeros but you use an  x  to
indicate the pole frequency. In Equation 5.40, the pole is at 2b 1  1  j 0 and so it is a real pole
located on the real axis in the  z -plane. For this ﬁ lter let’s analyze it with a 0  5 1.0 and
b 1  5 0.9. You wrote a plug-in and implemented this ﬁ lter in the last chapter. The results
are shown in  Figure 5.27 .
 Let’s see how this is estimated ﬁ rst, and then we can do a direct evaluation as before.
  Step 4: Estimate the frequency response
 The single pole is plotted on the real axis at  z  5 20.9 1  j 0 and a trivial zero at  z  5 0 1  j 0
( Figure 5.28 ). In the future, we will ignore the trivial zeros or poles.
 A pole or zero at  z  5 0 is trivial and can be ignored for the sake of analysis since it has no
effect on the frequency response.

126  Chapter 5
+24.0 dB
+18.0 dB
+12.0 dB
+6.0 dB
0.0 dB
–6.0 dB
–12.0 dB
10 Hz
100 Hz
1 kHz
10 kHz
 Figure 5.27:    The frequency response of the ﬁ rst-order feed-back ﬁ lter with these coefﬁ cients.
Im
–0.9
Trivial zero
Re
 Figure 5.28:    The pole is plotted in the  z -plane along with the trivial zero.
 In the simplest case of only one pole, the method for estimating the frequency response is as
follows:
•
•
•
   Locate each evaluation frequency on the outer rim of the unit circle.
   Draw a line from the point on the circle to the pole and measure the length of this vector.
Do it for each evaluation frequency.
   The  inverse of the lengths  of the lines will be the  magnitudes  at each frequency in the
frequency response in  Equation 5.42 .
Magnitude  5
1
Lp
Lp 5  length from evaluation frequency to pole
(5.42)
 Thus, the mechanism is the same as for the one-zero case, except you take the inverse of the
length of the vector. This means that as you near the pole, the vector becomes shorter, but the

Basic DSP Theory  127
amplitude becomes larger—exactly opposite of the zero case. You can see from  Figure 5.29
that our estimate is pretty close to what we saw in the real world when we coded the ﬁ lter
and tested it. Note the gain at Nyquist is 10.0; convert that to dB and you get 120 dB of gain,
which is what we expect.
  Step 5: Direct evaluation of frequency response
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
 First, get the transfer function in a form from  Equation 5.43  to use for all the evaluation
frequencies:
H(z) 5 a0
1
1 1 b1z21
    a0 5 1.0       b1 5 0.9
H(z) 5
1
1 1 0.9z21
       z    5 ejv
(5.43)
 Next, let’s make a simpliﬁ cation in the magnitude response equation and use the reduced
form in  Equation 5.44 . Next, evaluate.
  0 a 1 jb 0 5 "(a 1 jb)(a 2 jb)
5 "a2 1 b2
Im
0.1
–0.9
1.9
Re
H
10.0
0.74
0.55
0.52
0.0
1/1.9 =  0.52
1/1.8 =  0.55
1/1.3 =  0.74
1/4 Nyquist
fs/8
1
/2 Nyquist
fs/4
(5.44)
1/0.1 = 10
Frequency
Nyquist
fs/2
 Figure 5.29:    Estimating the frequency response of the ﬁ rst-order feed-back design.

128  Chapter 5
  5.16.1  DC (0 Hz)
  5.16.2  Nyquist (p)
  H(v) 5
 5
 5
 5
 5
1
1 1 0.9e2jv1
1
1 1 0.9 3cos(v) 2 jsin(v)4
1
1 1 0.9 3cos(0) 2 jsin(0)4
1
1 1 0.9 31 2 j04
1
1.9
 5 0.526 1 j0
   0 H(v) 0 5 "a2 1 b2
 5 "0.5262 1 02
 5 0.526
   Arg(H ) 5 tan21(b/a)
 5 tan21(0/0.526)
 5 0.08
  H(v) 5
1
1 1 0.9e2jv1
 5
 5
1
1 1 0.9 3cos(v) 2 jsin(v)4
1
1 1 0.9 3cos(p) 2 jsin(p)4
(5.45)
(5.46)
(5.47)
 5
1
1 1 0.9 321 2 j04
1
0.1
 5 10 1 j0
 5

Basic DSP Theory  129
   0 H(v) 0 5 "a2 1 b2
 5 "10.02 1 02
 5 10.0
 Arg(H ) 5 tan21 (b/a)
(5.48)
 5 tan21 (0/10.0)
 5 0.08
  5.16.3  ½ Nyquist (p/2)
  H(v) 5
1
1 1 0.9e2jv1
 5
1
1 1 0.9 3cos(v) 2 jsin(v)4
 5
1
1 1 0.9 3cos 1 p/2 2 2 jsin1 p/2 2 4
 5
1
1 1 0.9 30 2 j14
 5
1
1 2 j0.9
 H(v) 5
 0 H(v) 0 5
 5
 5
1
1 2 j0.9
0 1 0
0 1 2 j0.9 0
1
"a2 1 b2
1
"1 1 0.81
 5 0.743
  Arg(H ) 5 Arg(Num) 2 Arg(Denom)
 5 tan21 (0/1) 2 tan21 (20.9/1)
5 1428
(5.49)
(5.50)

130  Chapter 5
  5.16.4  ¼ Nyquist (p/4)
   H(v) 5
1
1 1 0.9e2jv1
 5
 5
 5
 5
1
1 1 0.9 3cos(v) 2 jsin(v)4
1
1 1 0.9 3cos(p/4) 2 jsin(p/4)4
1
1 1 0.636 2 j0.636
1
1.636 2 j0.636
 H(v) 5
 0 H(v) 0 5
 5
1
1.636 2 j0.636
0 1 0
0 1.636 2 j0.636 0
1
"1.6362 1 0.6362
 5 0.57
  Arg(H ) 5 Arg(Num) 2 Arg(Denom)
 5 tan21(0/1) 2 tan21(20.636/1.636)
5 1218
(5.51)
(5.52)
 Make a special note about how we have to handle the magnitude of a fraction with numerator
and denominator. You need to use the two equations in  Equation 5.9  to deal with this. The
main issue is that the phase is the difference of the Arg(numerator) 2 Arg(denominator). If
the numerator was a complex number instead of 1.0, you would need to take the magnitude
of it separately then divide. The ﬁ nal composite frequency/phase response plot is shown in
 Figure 5.30 . You can see that the phase behaves linearly until it gets near the pole, and then it
behaves nonlinearly. This is not a linear phase ﬁ lter.
  Step 6:  z  transform of impulse response
 From  Chapter 4  you will recall that the impulse response rings for this particular set of
coefﬁ cients ( Figure 5.31 ). Finding the impulse response by hand is going to be tedious. There
are a lot of points to capture and it could take many lines of math before the impulse settles
out. Fortunately, you can use RackAFX to do the work for you. The frequency and phase
response plots are made using a  z  transform of the impulse response.

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
