# Audio Plugins C++ (Pirkle) — Chunk 19/73

> Source: `Designing Audio Effect Plug-Ins in C++_ With Digital Audio Signal Processing Theory - PDF Room.pdf` · Pages ~1-560 · Extractor: pdfminer.six
> Target ≈ 2800 words, Overlap = 200 words · Actual = 2699

> Note: DSP textbook content for SpectralCanvasPro RT-audio knowledge base

---
+ 1.0
-1.0
Input
+1.0
-1.0
Basic DSP Theory  109
Output
 Figure 5.13:    The graphical results show the same information at Nyquist—the magnitude is 0
and there is no phase shift since there is nothing there to shift.
 The inverse tangent argument is 0/0 and the phase or Arg(H) is deﬁ ned to be 0 under this
condition. The C11 function you use is  arctan2 ( im,re ), which performs the inverse tangent
function; it will also evaluate to 0 in this case. Now, compare our results to the last chapter’s
graphical results ( Figure 5.13 ).
5.5.3     ½ Nyquist (p/2)
  H(v) 5 0.5 1 0.5e2jv1
 5 0.5 1 0.5(cos(v) 2 jsin(v))
 5 0.5 1 0.5(cos(p/2) 2 jsin(p/2))
 5 0.5 1 0.5(0 2 j1)
   5 0.5 2 j0.5
   0 H(v) 0 5 "(a 1 jb)(a 2 jb)
 5 "(0.5 1 j0.5)(0.5 2 j0.5)
 5 "0.25 1 0.25 5 "0.5
 5 0.707
   Arg(H  ) 5 tan21(b/a)
 5 tan21(20.5/0.5)
 5 245+
(5.20)
(5.21)
 Compare this to the last chapter’s graphical results ( Figure 5.14 ); the magnitude is 0.707 with
a phase shift of 245 degrees, and the results agree.
   5.5.4  1/4 Nyquist (p/4)
   H(v) 5 0.5 1 0.5e2jv1
 5 0.5 1 0.5(cos(v) 2 jsin(v))
(5.22)

110  Chapter 5
+ 1.0
-1.0
+ 1.0
Time
Smearing
-1.0
Phase Shift
Input
Output
 Figure 5.14:    Graphical results from the last
chapter at ½ Nyquist.
 5 0.5 1 0.5(cos(p/4) 2 jsin(p/4))
 5 0.5 1 0.5(0.707 2 j0.707)
 5 0.853 2 j0.353
 0 H(v) 0 5 "(a 1 jb)(a 2 jb)
 5 "(0.853 1 j0.353)(0.853 2 j0.353)
 5 "0.728 1 0.125 5 "0.853
 5 0.923
 Arg(H  ) 5  tan21(b/a)
 5  tan21(20.353/0.853)
 5 222.5+
(5.23)
 Compare to the last chapter’s graphical results ( Figure 5.15 ); you can see how much more
accuracy we get with the mathematical calculation. The magnitude and phase shift look about
right when compared to the graphs.
 Now, you can combine all the evaluations together and sketch out the frequency response of
the ﬁ lter ( Figure 5.16 ).

Basic DSP Theory  111
+1.0
-1.0
+1.0
Time
Smearing
-1.0
Phase Shift
Input
Output
 Figure 5.15:    Graphical results from the last chapter at ¼ Nyquist.
Magnitude
1.0
0.9
0.7
0.0
Phase
0°
-45°
-90°
1/4 Nyquist
fs/8
½ Nyquist
fs/4
Frequency
Nyquist
fs/2
Angle
 Figure 5.16:    The ﬁ nal composite frequency and phase response plots show the same results as
the last chapter, but with a lot less work.
112  Chapter 5
 Hopefully, this quick example has convinced you that it is better to do a little complex math
than have to analyze and design these ﬁ lters by brute force analysis of time domain input
sequences.
    5.6    Evaluating e  jv
 In the evaluation of the transfer function, you had to substitute values of v from 0 to p into
the  e  jv   terms of the equation. But what would the plot look like if you evaluate a single  e  jv
term? You saw that the use of Euler’s equation produced the real and imaginary components
of the term and now it’s time to plot them over the range of 0 to p.
 If you evaluate  e  jv   over more frequencies and plot the resulting values in the complex plane,
you get an interesting result. The frequencies in  Table 5.2  from 0 to 1p map to an arc that is
the top half of a circle with radius 5 1.0, shown in  Figure 5.17 . Remember, the magnitude
is the radius and the argument is the angle when using polar notation, which simpliﬁ es the
 Table 5.2:    The magnitude and angle of  e  jv   from DC to Nyquist.
Frequency p
DC (0Hz)
¼ Nyquist
½ Nyquist
Nyquist
e jv = cos(v) 1 jsin(v)
1 1 j0
0.707 1 j0.707
0 1 j1
21 1 j0
| e jv |
1.0
1.0
1.0
1.0
Arg(e jv)
0
p/4
p/2
p
π/2
Im
3π/4
π/4
j0.707
R = 1
π/4
+π
0Hz
0.707
Re
 Figure 5.17:    The positive frequencies map to
the upper half of the unit circle.
Basic DSP Theory  113
analysis. You don’t have to keep track of the real and imaginary parts. The evaluation at
v 5 p/4 is plotted on the curve. The circle this arc is laying over would have a radius of 1.0
and is called the  unit circle . If you evaluate  e jv   over the negative frequencies that correspond
to 0 to 2p, you get a similar but inverted table ( Table 5.3 ).
    This table translates to a mapping across the lower half of the same unit circle ( Figure 5.18 ).
The negative frequencies increase as you move clockwise from 0 Hz, the radius stays 1.0
during the entire arc.
 Why bother to evaluate  e jv  ? It will be useful very soon when we start picking apart the
transfer functions in an effort to ﬁ gure out how to design ﬁ lters. It also shows the limited
“frequency space” of the digital domain. All the frequencies that could exist from 2Nyquist
to 1Nyquist map to outline of a simple unit circle. In contrast the analog domain has an
inﬁ nitely long frequency axis and an inﬁ nite frequency space.
 Table 5.3:    The magnitude and angle of  e  jv   from DC to 2Nyquist.
Frequency v
DC (0Hz)
2¼ Nyquist
2½ Nyquist
2Nyquist
e jv 5 cos(v) 1 jsin(v)
1 1 j0
0.707 2 j0.707
0 2 j1
21 1 j0
| e jv |
1.0
1.0
1.0
1.0
Arg(e jv)
0
2p/4
2p/2
2p
Im
-π
Re
0Hz
- 3 π /4
- π /4
-π/2
 Figure 5.18:    The negative frequencies map to the
lower half of the unit circle.
114  Chapter 5
 The  order  of a filter is the order of the polynomial in the transfer function that describes it. The
order of the polynomial is the maximum absolute exponent value found in the equation.
   5.7    The z Substitution
 It’s going to get messy having to write  e jv   so often and we know  e jv   behaves like a polynomial
mathematically. So, we can simplify the equations by making a simple substitution of
 Equation 5.24 .
  z 5 ejv
(5.24)
 This is just a substitution right now and nothing else. Making the substitution in  Equation 5.24
and noting the resulting transfer function is now a function of  z , not v, we can write it like
 Equation 5.25 :
  H(z) 5 a0 1 a1z21
(5.25)
 The reason this is useful is that it turns the transfer function into an easily manipulated
polynomial in  z . In this case, the polynomial is a ﬁ rst-order polynomial (the highest exponent
absolute value is 1) and this is the real reason the ﬁ lter is named a ﬁ rst-order ﬁ lter—it’s the
polynomial order of the transfer function.
   5.8    The z Transform
 The  z  substitution does a good job at simplifying the underlying polynomial behavior of
 e jv   and it lets us use polynomial math to solve DSP problems. But, there is an interesting
application of  z  5  e jv   that simpliﬁ es the design and analysis of digital ﬁ lters. In the graph
from  Chapter 1  ( Figure 5.19 ) you can see how the indexing of the samples determines their
place in time. The future samples have positive indices and the past (delayed) samples have
negative indices.
 We’ve seen that  e jv   is the delay operator, so  e 2jv1   means one sample of delay, or one
sample  behind  the current one. Likewise,  e 2jv2   would be two samples behind and  e 1jv4
indicates 4 samples  ahead  or in the future. That means that  e jv   could also be used as a
book-keeping device since it can relate the position in time of something (a sample,
for us).
 The rules for implementing the  z  transform on a discrete signal or difference equation are
easy and can often be done by inspection. The current sample  x ( n ) or  y ( n ) transforms into the
signal  X ( z ) or  Y ( z ). Instead of thinking of the  sample x ( n ) you think of the  signal   X ( z ), where
 X ( z ) is the whole signal—past, present, and future.

Basic DSP Theory  115
Amplitude
x(n-3)
Current  input
sample
x(n-4)
x(n-2)
Future input
samples
X(n-1)
Past input
samples
x = The  input
sequence
x(n+4)
X(n+1)
The Past
x(n)
x(n + 3)
x(n + 2)
The  Future
 Figure 5.19:    Our book-keeping rules shown graphically.
 The  z transform  changes a sequence of samples in  n  to a sequence of samples in  z  by replacing
the indices …n21, n, n11… with …z 21 , z, z 11 … This works because multiplication by  z  =  e  jv
represents the operation of delay or time shift. The resulting transformed sequence now is a
function of the complex frequency  e  jv  , therefore it transforms things from the  time domain  into
the  complex frequency domain .
•
•
    x ( n ) →  X ( z )
    y ( n ) →  Y ( z )
 Instead of thinking of the sample  x ( n  2 1) as being delayed by one sample, you think of the
signal  X ( z ) delayed by one sample,  z  21 . The delayed signals are the result of the whole signal
multiplied by the  z  2 N   terms:
•
•
•
•
    x ( n 2  1) →  X ( z )  z  21
    y ( n  2 1) →  Y ( z )  z2  1
    x ( n  2 2) →  X ( z )  z  22
    y ( n  1 6) →  Y ( z )  z  16
116  Chapter 5
 You can see that this concept relies on the ability to express delay as a mathematical operator.
It not only allows us to express an  algorithm  based on  z , it also lets us express a  signal  based
on  z . Mathematically, the  z  transform is
n51`
  X(z) 5 a
n52`
x(n)z2n
(5.26)
   5.9    The z Transform of Signals
 Remember,  x ( n ) is the sequence of samples just like the ones you used for analysis.
 Figure 5.20  shows an example. This simple, ﬁ nite length signal consists of ﬁ ve samples.
The remaining zero samples don’t need to be counted.
  x(n)  5  50,  0.25,  0.5,  0.75,  1.06
 The sequence  x ( n ) could also be written  x ( n ) 5 { x (0),  x (1),  x (2),  x (3),  x (4)}, so using
 Equation 5.26  we transform x(n) into X(z) and write  Equation 5.27 :
  X(z) 5 0z0 1 0.25z21 1 0.5z22 1 0.75z23 1 1.0z24
(5.27 )
 You could read  Equation 5.27  as follows: “The whole signal  X ( z ) consists of a sample with
an amplitude of 0 at time 0 followed by a sample with an amplitude of 0.25 one sample later
and a sample with an amplitude of 0.5 two samples later and a sample with an amplitude of
0.75 three samples later and . . .” This should shed light on the fact that the transform really
involves the book-keeping of sample locations in time and that the result is a polynomial. You
can multiply and divide this signal with other signals by using polynomial math. You can mix
two signals by linearly combining the polynomials.
+1.0
0.75
0.5
0.25
–1.0
 Figure 5.20:    A simple signal for analysis.

Basic DSP Theory  117
+1.0
–1.0
 Figure 5.21:    The DC signal is inﬁ nite in length.
 Let’s do one more example regarding the transformation of an input signal. This
time, let’s choose the DC signal—it goes on forever.  Figure 5.21  shows the DC signal
with the ﬁ rst sample at 1.0 and all preceding samples at 0.0 with a sequence of
 x ( n ) 5 {1, 1, 1, 1, 1…}.
 Using  Equation 5.26  you can directly write the  z  transform as (remember, 1 z  0  5 1) in
 Equation 5.28 :
  X(z) 5 1 1 z21 1 z22 1 z23 1 c 1 z2`
(5.28)
 While that looks like an ugly, inﬁ nitely long equation, it can also be represented in a  closed
form . In fact, a closed form representation exists for this polynomial using a polynomial
series expansion, as shown in  Equation 5.29 :
   X(z) 5 1 1 z21 1 z22 1 z23 1 c 1 z2`
 5
 5
1
1 2 z21
z
z 2 1
(5.29)
   5.10    The z Transform of Difference Equations
 The  z  transform of signals is interesting, but something fascinating happens when you take
the  z  transform of a difference equation, converting it into a transfer function in  z  all at once.
And, the same easy rules apply. Let’s do that with the basic ﬁ rst-order feed-forward ﬁ lter. The
difference equation is
y(n) 5 a0x(n) 1 a1x(n 2 1)
(5.30)

118  Chapter 5
 Taking the z transform:
  Y(z) 5 a0X(z) 1 a1X(z)z21
      Y(z) 5 X(z)3a0 1 a1z21 4
Y(z)
X(z)
 H(z) 5
5 a0 1 a1z21
 This is a really useful result—you got the ﬁ nal transfer function in just a handful of steps,
using the simple  z  transform rules. Let’s recap what you had to do before you learned how to
take the  z  transform of a difference equation:
•
•
•
•
   Redraw the block diagram with  e 2jvn   operators in the  n -sample delay elements.
   Apply the complex sinusoid  e 2jvt   to the input.
   Find out what comes out,  y ( t ), and formulate the transfer function H(v).
   Apply the  z  substitution to the transfer function.
 Taking the  z  transform does all these steps at once and we’re left with the same simple
polynomial in  z . If we evaluate the transfer function for different values of  z  5  e jv  , we can
ﬁ nd the frequency and phase plots. You’ll get more practice taking the  z  transforms of more
difference equations soon.
   5.11    The z Transform of an Impulse Response
 The  z  transform of a difference equation results in the transfer function. But what if you
don’t have the difference equation? Suppose you only have a black box that performs some
kind of DSP algorithm and you’d like to ﬁ gure out the transfer function, evaluate it, and plot
the frequency and phase responses. It can be done without knowing the algorithm or any
details about it by taking the impulse response of the system. You apply the input sequence
 x ( n ) 5 {1, 0, 0, 0, 0…} and capture what comes out, which we’ll call  h ( n ). If you take
the  z  transform of the impulse response, you get the transfer function expanded out into a
series form.
 In fact, this is exactly what the RackAFX software’s audio analyzer does—it takes the
impulse response of the ﬁ lter and then runs a  z  transform on it to make the magnitude and
phase plots you see on the screen. Mathematically,  Equation 5.31  is identical to  Equation 5.26
except we’ve changed the signal from  X  to  H :
 The  z  transform of the impulse response  h ( n ) is the transfer function  H ( z ) as a series expansion.
Evaluate the transfer function to plot the frequency and phase responses.
