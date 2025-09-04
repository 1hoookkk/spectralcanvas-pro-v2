# Audio Plugins C++ (Pirkle) — Chunk 17/73

> Source: `Designing Audio Effect Plug-Ins in C++_ With Digital Audio Signal Processing Theory - PDF Room.pdf` · Pages ~1-560 · Extractor: pdfminer.six
> Target ≈ 2800 words, Overlap = 200 words · Actual = 2406

> Note: DSP textbook content for SpectralCanvasPro RT-audio knowledge base

---
98  Chapter 5
A  Sine
Cosine
A
Sinusoid
t
t
 Figure 5.1:    Sine, cosine, and sinusoid signals.
 Euler’s equation is shown below:
   eju 5 cos(u) 1 jsin(u)
 j 5 "21
(5.2)
 You can see that it includes both sine and cosine functions. The  j   term is the imaginary
number, the square root of 21 (mathematicians call it  i  but since that represents current
in engineering, we rename it  j  instead). The  j  is known as the phase rotation operator;
multiplying a function by  j  rotates the phase by 90 degrees.
 Suppose you want to shift the phase of a waveform by 180 o , thereby inverting it. Mathematically,
you can do this by multiplying the waveform by 21, inverting the values of all its points. Suppose
you wanted to invert the waveform again (which would bring it back to its original shape)—you
could do that by multiplying by 21 again. But suppose that you only wanted to shift the phase by
90 o ? Is there a number (h) you could use as a multiplier to shift by 90 o ? In other words,
  90-degree  shifted  waveform  5  (original waveform) (h)
 You don’t know what h is yet, but you can ﬁ gure it out. Suppose you then wanted to shift the
waveform by another 90 degrees, which would be the same as shifting the original waveform
by 180 degrees. You would multiply it by h again. You’d then have the following :
  180-degree shifted waveform  5  (original waveform) (h)(h)
5  (original waveform) (h)2
5 (original waveform) (21)

Basic DSP Theory  99
 This leads to  Equation 5.3 :
   h2 5 21
      h 5 "21 5 j
 So, you can perform a conceptual 90-degree phase shift by multiplying a waveform by  j .
A 290-degree phase shift is accomplished by multiplying by 2 j . Some other useful
relationships with  j  are
   j2 5 21
1
j
5 2j
(5.3)
(5.4)
 Euler’s equation is complex and contains a real part (cos) and imaginary part (sin), and the
plus sign (1) in the equation is not a literal addition—you can’t add real and imaginary
numbers together. In a complex number of the format A 1  j B, the two parts coexist as part of
one complex number.
 For our purposes, we replace the u with vt instead, where v is the frequency in radians/
second and  t  is the time variable. Plug in various values for  t  and you get the plot in
 Figure 5.1  when you plot the sine and cosine in the same plane. So, we will reject using the
sin() and cos() functions independently and adopt the complex sinusoid as a prepackaged
mixture of the two. The reason is partly mathematical—as it turns out,  e  is simple to
deal with mathematically. You only need to learn four rules ( Equation 5.5 ) in addition
to Euler’s equation.
Euler’s equation:  ejvt 5 cos(vt) 1 jsin(vt)
 The four rules:
 eaeb 5 e(a1b) or eatebt 5 e(a1b)t
ea
eat
ebt 5 e(a2b)t
eb 5 e(a2b) or
d(eat)
dt
5 aeat
 3eatdt 5
1
a
eat
(5.5)
 So, what Euler’s equation is really describing is a sine and cosine pair of functions,
 coexisting in two planes that are 90 degrees apart. The word  orthogonal  is the engineering term
for  90 degrees apart .

100  Chapter 5
 The two equations in  Equation 5.5  demonstrate that  e  behaves like a polynomial (x 2  * x 5  5 x 7 )
even when the argument is a function of time,  t .  Equation 5.5  also shows how simply it behaves
in calculus—multiple derivatives or integrations are done by simply multiplying the argument’s
constant (a or 1/a) over and over. Before we leave this topic, make sure you remember how
to deal with complex numbers—you’ll need it to understand where the frequency and phase
responses come from.
   5.2    Complex Math Review
 Because a complex number has both real and imaginary parts, it cannot be plotted on a single
axis. In order to plot a complex number, you need two axes: a real and imaginary axis. The
two axes are aligned at right angles, just like the  x - and  y -axes in two-dimensional geometry.
They are orthogonal. The  x -dimension is the real axis (Re), and the  y -dimension is the
imaginary axis (Im). So, a complex number is plotted as a point on this  x-y  plane, also called
the  complex plane . Complex numbers are usually written in the form A 1  j B where A is the
real part and B is the imaginary part. Notice that the notation always stays in the form A 1  j B
even when common sense might contradict it. For the point in the second quadrant, you still
write 22 1  j 2 even though  j 2 2 2 might look nicer. Also notice that you write 2 2  j 1 instead
of just 2 2  j . The points are plotted the way you would plot ( x ,  y ) pairs in a plane ( Figure 5.2 ).
 This example plots points as ( x  1  jy ) pairs; this is called the Cartesian coordinate system.
You can also use polar notation to identify the same complex numbers. In polar notation, you
specify the radius (R) and angle (u) for the vector that results from drawing a line from the
origin of the axes out to the point in question. For example, consider the point 2 1  j 3 above.
You could plot this point in polar form as shown in  Figure 5.3 .
 We are leaving the Cartesian form (2 1  j 3) for reference. Normally, all you would see is R
and u. Most engineers prefer polar notation for dealing with complex numbers. The polar
number is often written as R < u. Fortunately for us, the conversion from Cartesian to polar
Im
2 +j3
-2  + j2
Re
2 -j1
 Figure 5.2 : Several points plotted in the complex plane using
the Cartesian (x, y) coordinate system.
Basic DSP Theory  101
Im
2 + j3
R
θ
Re
 Figure 5.3:    Plotting 2 1  j 3 in polar form.
notation is simple. Starting with a complex number in the form A 1  j B, you can ﬁ nd the
resulting radius and angle from  Equations 5.6  and  5.7 .
  R 5 "A2 1 B2
    u 5 tan21 c
B
A
d
(5.6)
(5.7)
 The radius (R) is sometimes called the  magnitude  and the angle u is called the  argument . You
sometimes only care about the square of the magnitude (called the “magnitude-squared” or
| R | 2 ) ( Equation 5.8 ).
  Equation 5.9  shows how to extract the magnitude and phase from a transfer function.
  0 R 0 2 5 A2 1 B2
then
and
  H 5
num
denom
0 H 0 5
0 num 0
0 denom 0
Arg(H) 5 Arg(num) 2 Arg(denom)
 (5.8)
(5.9)
 The frequency response plots of filters are actually magnitude responses of a complex function
called the  transfer function  of the filter. The phase response plots are actually argument responses
of this function. The transfer function is complex because it contains complex numbers; many
transfer functions are actually quite simple. We use the letter H to denote a transfer function.

102  Chapter 5
   5.3    Time Delay as a Math Operator
 The next piece of DSP theory you need to understand is the concept of time delay as a
mathematical operator. This will be pretty easy since we are going to exploit the simple
mathematical behavior of the  e jvt   function. First, consider a complex sinusoid and a delayed
complex sinusoid ( Figure 5.4 ).
 How does the delay of  n  seconds change the complex sinusoid equation? Since positive
time goes in the positive  x  direction, a delay of  n  seconds is a shifting of 2 n  seconds. In the
complex sinusoid equation, you would then replace  t  with  t2n . In other words, any point
on the delayed curve is the same as the nondelayed curve minus  n  seconds. Therefore, the
delayed sinusoid is
Delayed sinusoid  5  e
jv(t2n)
(5.10)
 But, by using the polynomial behavior of  e  and running  Equation 5.5  in reverse, you can
rewrite it as shown in  Equation 5.11 :
  ejv(t2n) 5 ejvte2jvn
(5.11)
 It’s a subtle mathematical equation but it says a lot: if you want to delay a complex sinusoid
by  n  seconds, multiply it by  e 2jvn  —this allows us to express time delay as a mathematical
operator.
 In the last two sections you’ve learned that phase rotation and time delay can both be
expressed as mathematical operators.
 Time delay can be expressed as a mathematical operator by multiplying the signal to be delayed
 n  seconds by  e 2jvn  . This is useful because  e 2jvn   is not dependent on the time variable. In
 discrete-time systems, the  n  refers to samples rather than seconds.
jωt
e
jω(t-n)
e
A
t
 Figure 5.4:    A complex sinusoid  e jvt   and another one delayed by  n  seconds.
n sec

Basic DSP Theory  103
   5.4    First-Order Feed-Forward Filter Revisited
 Being able to express delay as the mathematical operation of multiplication by  e 2jvn   means
you can take the block diagram and difference equation for a DSP ﬁ lter and apply a sinusoid
to the input in the form of  e jvt   rather than having to plug in sequences of samples as you did
in  Chapter 4 . Then, you can see what comes out of the ﬁ lter as a mathematical expression
and evaluate it for different values of v (where v 5 2p f  with  f  in Hz) to ﬁ nd the frequency
and phase responses directly rather than having to wait and see what comes out then try
to guesstimate the amplitude and phase offsets. Consider the ﬁ rst-order feed-forward ﬁ lter
from the last chapter but with  e jvt   applied as the input signal, shown in a block diagram
in  Figure 5.5 .
 The difference equation is as follows:
  y(t) 5 a0ejvt 1 a1
3ejvte2jv1 4
(5.12)
  Figure 5.5  shows the familiar block diagram but this time with the input  x ( t ) and output  y ( t )
instead of  x ( n ) and  y ( n ). Notice the delay element has been replaced by  e 2jv1   since there is
a one-sample delay. When you apply the complex sinusoid  e jvt   to the input, the difference
equation uses the delay-as-multiplication operation to produce the output. With a little math
you can arrive at  Equation 5.13 .
    y(t) 5 a0e
 5 e
jvt 1 a1
3ejvte2jv1 4
jvt1 a0 1 a1e2jv1 2
and the input x(t) 5 ejvt so
y(t) 5 x(t)  1 a0 1 a1e2jv1 2
(5.13)
 The transfer function is defined as the ratio of output to input, therefore
y(t)
x(t)
5 a0 1 a1e2jv1
x(t)
ejωt
e–jω1
e–jω1
ejωt
a0  ejωt
a0
a,
y(t)
ejωt
a0
+  a{  ejωt  e–jωt
ejωt
a1
e–jω1
 Figure 5.5:    Block diagram of a ﬁ rst-order feed-forward
ﬁ lter with signal analysis.

104  Chapter 5
 The  transfer function  of the filter is the ratio of output to input. The  frequency response  of the filter
is the magnitude of the transfer function evaluated at different frequencies across its spectrum.
The  phase response  of the filter is the argument (or angle) of the transfer function evaluated at
different frequencies across its spectrum.
 To produce the frequency and phase response graphs, you evaluate the function for various val-
ues of v then find the magnitude and argument at each frequency. The evaluation uses Euler’s
equation to replace the  e  term and produce the real and imaginary components.
 What is so signiﬁ cant about this is that the transfer function is not dependent on time
even though the input and output signals are functions of time. The transfer function
( Equation 5.14 ) is only dependent on frequency v, so we call it H(v).
  H(v) 5 a0 1 a1e2jv1
(5.14)
 Notice that the transfer function is complex.
 But what values of v are to be used in the evaluation? We know that v 5 2p f , but do we
really care about the frequency in Hz? In  Chapter 4  when you analyzed the same ﬁ lter, you
applied DC, Nyquist, ½ Nyquist, and ¼ Nyquist without thinking about the actual sampling
frequency. This is called  normalized frequency  and is usually the way you want to proceed in
analyzing DSP ﬁ lters. The actual sampling rate determines Nyquist but the overall frequency
range (0 Hz to Nyquist) is what we care about. To normalize the frequency, you let  f  5 1 Hz
in v 5 2p f , then v varies from 0 to 2p or across a 2p range. There is also one detail we have
to be aware of: negative frequencies.
  5.4.1  Negative Frequencies
 You may have never thought a frequency could be negative, but it can. When you ﬁ rst learned
about the concept of a waveform’s frequency, you were taught that the frequency is 1/T,
where T is the period, as shown in  Figure 5.6 .
 The reason the frequencies came out as positive numbers is because the period is deﬁ ned as
 t  2  2  t  1 , which makes it a positive number. But, there’s no reason you couldn’t deﬁ ne the
period to be the other way around: T 5  t  1  2  t  2 , except that it implies that time is running
backwards. Mathematically, time can run backwards. This means that for every positive
frequency that exists, there is also a negative “twin” frequency. When you look at a frequency
response plot you generally only look at the positive side.  Figure 5.7  shows a low-pass
response up to the highest frequency in the system, Nyquist.
 However, in reality, the ﬁ lter also operates on the negative frequencies just the same in
a mirror image. In this case, as the negative frequencies get higher and higher, they are
attenuated just like their positive counterparts ( Figure 5.8 ). And it makes sense too. If
