# Audio Plugins C++ (Pirkle) — Chunk 20/73

> Source: `Designing Audio Effect Plug-Ins in C++_ With Digital Audio Signal Processing Theory - PDF Room.pdf` · Pages ~1-560 · Extractor: pdfminer.six
> Target ≈ 2800 words, Overlap = 200 words · Actual = 2480

> Note: DSP textbook content for SpectralCanvasPro RT-audio knowledge base

---
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

+1.0
+1.0
Basic DSP Theory  119
Input
Output
–1.0
–1.0
 Figure 5.22:    The impulse response from the ﬁ rst-order feed-forward ﬁ lter.
n51`
  H(z) 5 a
n52`
h(n)z2n
(5.31)
 Try this on the ﬁ rst-order feed-forward ﬁ lter we’ve been working on; you already have the
impulse response “captured” from the last chapter ( Figure 5.22 ).
 The impulse response is  h ( n ) 5 {0.5, 0.5}. Applying the  z  transform yields  Equation 5.32 :
   H(z) 5 0.5z0 1 0.5z21
 5  0.5 1 0.5z21
(5.32)
 Notice that this is the identical result as taking the  z  transform of the difference equation and
the ﬁ lter coefﬁ cients (0.5, 0.5)  are  the impulse response {0.5, 0.5}.
   5.12    The Zeros of the Transfer Function
 When we used the coefﬁ cients a 0  5 a 1  5 0.5 we wound up with a ﬁ lter that completely
destroys the Nyquist frequency and you saw how its output became 0 in both the manual
and complex sinusoid evaluations. We noted that feed-forward ﬁ lters have  zeros of
transmission  or  zero frequencies  or just  zeros  when their output becomes zero. In both the
manual and complex sinusoid evaluations, we just got lucky when we stumbled upon this
value as Nyquist happened to be one of the signals we were testing or evaluating. There’s
a way to precisely ﬁ nd these critical frequencies by using the polynomial result of the  z
transform. You probably remember factoring polynomials in high school or college. When
you did that, you set the polynomial equal to 0 and then you factored to ﬁ nd the roots of
the polynomial. What you were really doing was ﬁ nding the zeros of the polynomial, that

120  Chapter 5
is, the values of the dependent variable that make the polynomial become zero. You can do
the same thing with the transfer function by setting it equal to zero and then factoring the
polynomial. Suppose a 0  5 a 1  5 0.5 and we factor the transfer function in  Equation 5.32  to
get  Equation 5.33 :
   H(z) 5 0.5 1 0.5z21
 5 0.5 1
0.5
z
(5.33)
 You can ﬁ nd the zero by inspection—it’s the value of  z  that forces  H ( z ) to be 0 and in
this case there is a zero at  z  5 21.0. But what does it mean to have a zero at 21.0?
This is where the concept of evaluating  e jv   comes into play. When you did that and
plotted the various points, noting they were making a unit circle in the complex plane,
you were actually working in the  z -plane, that is, the plane of  e jv  . The location of the
zero at  z  5 21.0 is really at the location  z  5 21.0 1  j 0 purely on the real axis and
at Nyquist. In  Figure 5.23  the zero is shown as a small circle sitting at the location
 z  5 21.0.
 There are several reasons to plot the zero frequencies. First, you can design a ﬁ lter directly
in the  z -plane by deciding where you want to place the zero frequencies ﬁ rst, then ﬁ guring
out the transfer function that will give you those zeros. Secondly, plotting the zeros gives
you a quick way to sketch the frequency response without having to evaluate the transfer
function directly. You can estimate a phase plot too, but it is a bit more involved. So,
you have two really good reasons for wanting to plot the zeros; one for design, the other
for analysis.
Im
Zero at +π
Re
 Figure 5.23:    Zero is plotted in the  z -plane at its
location on the real axis  z  5 21 1  j 0.

Basic DSP Theory  121
   5.13    Estimating the Frequency Response: Zeros
 An interesting property of the  z -plane and  z  transform is that you can measure the frequency
response graphically on the  z -plane. In the simplest case of only one zero, the method is as
follows:
•
•
•
   Locate each evaluation frequency on the outer rim of the unit circle.
   Draw a line from the point on the circle to the zero and measure the length of this vector.
Do it for each evaluation frequency.
   The  lengths  of the lines will be the  magnitudes  at each frequency in the frequency
 response.
 In  Figure 5.24  you can see the complete operation, ﬁ rst drawing and measuring the lines—
you can use graph paper and a ruler, if you want—then building the frequency response
plot from them. Notice also that the magnitude of a line drawn from Nyquist to the zero
at 21 has a length of zero. The lengths of the vectors are the mathematical deﬁ nition of
magnitude and you are evaluating the whole ﬁ lter at once. These  z -plane plots are going
to be useful for ﬁ lter design. You can also derive the phase response, which involves
measuring the angles of incidence of each vector on the zero. With multiple zeros, it
becomes cumbersome. But, estimating the frequency response is pretty simple, even for
more complex ﬁ lters.
 You might notice that even though this frequency response looks like the one we produced
earlier, the gain values are not the same. In this ﬁ lter, the gain is 2.0 at DC, and in ours,
it’s half of that. In fact, this ﬁ lter’s magnitudes at the evaluation frequencies are all twice
what ours are.
(cid:652)(cid:18)(cid:21)(cid:3)
Im
(cid:20)(cid:17)(cid:23)(cid:20)(cid:23)
(cid:20)(cid:17)(cid:27)(cid:23)(cid:25)
(cid:652)(cid:18)(cid:23)(cid:3)
(cid:62)H(cid:64)
2.0
1.8
(cid:14)(cid:652)(cid:3)
2.0
(cid:3)
0(cid:3)Hz
Re
1.4
0.0
2.0
1.846
1.414
1/4 (cid:3)Nyquist
fs/8
(cid:242) Nyquist
fs/4
Frequency
Nyquist
fs/2
 Figure 5.24:    The geometric interpretation shows how the length of each vector from the
evaluation frequency to the zero is really a magnitude in the response.
122  Chapter 5
   5.14    Filter Gain Control
 The last thing you need to do is remove the overall gain factor from the transfer function so that
overall ﬁ lter gain (or attenuation) can be controlled by just one variable. This is actually pretty
simple to do, but requires re-working the transfer function a bit. The idea is to pull out the a 0
variable as a multiplier for the whole function. This way, it behaves like a volume knob, gaining
the whole ﬁ lter up or down. The way you do it is to normalize the ﬁ lter by a 0  ( Equation 5.34 ):
    H(z) 5 a0 1 a1z21
a0
a0
a1
a0
 5
1
z21
a1
a0
z21 d
(5.34)
 5 a0 c1 1
a1
a0
   H(z) 5 a0
   Let a1 5
31 1 a1z21 4
 By normalizing by a 0  and using the a 1  variable you can produce a transfer function that looks
basically the same in the polynomial but pulls a 0  out as a scalar multiplier—a gain control.
Where is the zero of the new transfer function in  Equation 5.35 ?
   H(z) 5 a0
31 1 a1z21 4
a1
z
d
5 a0 c1 1
(5.35)
 Once again, by inspection we can tell that if  z  5 2a 1  then the function will become 0
regardless of the value of a 0 . This transfer function has a zero at 2a 1 . If you plug our values
of a 0  5 a 1  5 0.5, you still get the same zero at 21.0. The difference is in the gain of the ﬁ lter.
 After extracting the magnitude response from the  z -plane plot, scale it by your a 0  value. This makes
the response in  Figure 5.24  match our others because everything gets multiplied by a 0  5 0.5. The
idea of controlling the gain independently of the magnitude response is useful in audio ﬁ lters, so
we will keep it and use it in all future analyses.
 At this point, you have all the DSP theory tools you need to understand the rest of the
classical DSP ﬁ lters (ﬁ rst-order feed back, second-order feed forward and feed back) as well
as many ﬁ lter design techniques. The rest of the chapter will be devoted to applying these
same fundamentals to the other classical DSP ﬁ lters but we will move much more quickly,
 The  graphical interpretation  method of evaluating a filter in the  z -plane assumes the filter is nor-
malized so that a 0  5 1.0.

Basic DSP Theory  123
applying each analysis technique to the other algorithms. For example, we will dispense with
the evaluation of  e jv   terms and start off directly in the  z  transform of the difference equations.
   5.15    First-Order Feed-Back Filter Revisited
 Now let’s go through the same analysis technique on the ﬁ rst-order feed-back ﬁ lter from the
last chapter. We can move much more quickly now that we have the basic DSP theory down.
There will be many similarities but also several key differences when dealing with feed-back
designs. You already saw that the feed-back topology can blow up or ring forever and that
the feed-forward design cannot. We will ﬁ nd a way to ﬁ gure out if this is going to happen
and how to prevent it. Start with the original ﬁ rst-order feed-back ﬁ lter (block diagram in
 Figure 5.25 ) and its difference equation.
 The difference equation is as follows:
  y(n) 5 a0x(n) 2 b1y(n 2 1)
(5.36)
  Step 1: Take the  z  transform of the difference equation
 This can be done by inspection, using the rules from  Section 5.8  ( Figure 5.26 ). Therefore, the
 z  transform is shown in  Equation 5.37 .
  Y(z) 5 a0X(z) 2 b1Y(z)z21
(5.37)
x(n)
a0
y(n)
z-1
–b1
 Figure 5.25:    The ﬁ rst-order feed-back ﬁ lter.
a0X(z)
X(z)
a0
Y(z)
–b1Y(z)z–1
–b1
z-1
Y(z)z -1
 Figure 5.26:    Pushing the input  X ( z ) through the algorithm produces the  z  transform.

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
