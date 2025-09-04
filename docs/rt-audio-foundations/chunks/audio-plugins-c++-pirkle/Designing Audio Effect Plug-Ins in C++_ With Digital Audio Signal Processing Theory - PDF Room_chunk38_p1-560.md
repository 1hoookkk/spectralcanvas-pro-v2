# Audio Plugins C++ (Pirkle) — Chunk 38/73

> Source: `Designing Audio Effect Plug-Ins in C++_ With Digital Audio Signal Processing Theory - PDF Room.pdf` · Pages ~1-560 · Extractor: pdfminer.six
> Target ≈ 2800 words, Overlap = 200 words · Actual = 5230

> Note: DSP textbook content for SpectralCanvasPro RT-audio knowledge base

---
Audio Filter Designs: FIR Filters  255
 The operation this equation is describing is not simple. On the right-hand side of
 Equation 8.3  the function  f ( n ) is one signal while the function  g ( n  2  m ) represents the
second signal reversed in time. The multiplication/summation of the two across 2` to
1` describes the process of sliding the two signals over each other to create overlapping
areas. On each iteration, the area under the curve of overlap between  g ( n  2  m ) and  f ( n )
is computed. This results in a third (output) signal  c ( n ). This signal  c ( n ) is made up of
the overlapping area of the two input signals. This operation is shown graphically in
 Figures 8.2  and  8.3 .
 Thus, the convolution of two arbitrary signals is quite involved mathematically. If the two
signals have any complexity at all, the resulting convolution signal is generally not
distinguishable as a linear combination of the two. If you know the IR of a system  h ( n ),
you can convolve it with the input signal  x ( n ) to produce the output signal  y ( n ). This is the
equivalent of multiplying the transfer function  H ( z ) with the input signal  X ( z ) to produce
Two signals f and g
Convolution c(2)
f
g
Convolution  c(0)
Convolution c(3)
C(2)
C(0)  = 0
Convolution c(1)
C(3)
Convolution c(4)
n
C(1)
C(4) = 0
n
n
n
 Figure 8.2:    Two signals  f  and  g  are convolved. These are discrete signals but the
sample symbols have been removed to make it easier to see; instead they are shown as
continuous. In the ﬁ rst step,  c (0), one of the signals is reversed and the two are pushed
up next to each other. As the convolution progresses through each step  c (1) to  c (4),
the overlapping areas are calculated and stored.
256  Chapter  8
the output  Y ( z ). Thus convolution in the time domain is multiplication in the frequency ( z )
domain ( Equation 8.4 ).
  y(n) 5 x(n) * h(n) 4 Y(z) 5 X(z)H(z)
(8.4)
 To understand how a FF ﬁ lter implements convolution ﬁ rst rearrange the block diagram. Let’s
consider a long FIR ﬁ lter with  N  1 1 coefﬁ cients in  Figure 8.4 .
f(n)
g(n)
c(n)
n
n
n
c(1)
c(2)
c(3)
 Figure 8.3:    The convolution of f and g results in  c ( n ), consisting of ﬁ ve samples  c (0) through
 c (4), which represent the overlap areas.
x(n)
a0
∑
y(n)
z–1
z–1
z–1
z–1
a 1
a 2
a3
a N
 Figure 8.4 :    The familiar FIR feed-forward structure expanded out to  N  delay taps with
 N  1 1 coefﬁ cients. It is important to see that there is one less delay element than coefﬁ cients
since a 0  is multiplied against the original undelayed signal.
Audio Filter Designs: FIR Filters  257
 Next, mentally rotate the structure so it looks like  Figure 8.5 . In  Figure 8.5  you can see that at
any given time, a portion of the input signal  x ( n ) is trapped in the delay line. On each sample
period, the input signal slides to the right and a summation is formed with the product of the
coefﬁ cients and the samples  x ( n  2  d ). The words “sliding, summation and product” are key
here—they’re the same words used to describe convolution.
 In  Figure 8.6 , the input signal  x ( n ) moves through the delay line being convolved with
the IR on each sample period. Since each sample in the delay line is an impulse, and each
impulse is symmetrical when reversed, this is the same as conceptually overlapping the IR
on top of each sample and scaling by the sample value. The result is the ﬁ nal summation of
all the peaks and valleys of the IR with the delayed signal  x ( n ). Thus, an FIR ﬁ lter exactly
implements discrete convolution in the time domain. This ultimately gives us a whole new
way to ﬁ lter a signal—by convolving against an impulse.
The input signal marches
through the delay line this way
x(n)
z -1
z -1
z -1
z -1
a0
a1
a2
a3
aN
∑
y(n)
One output sample is generated
each sample period; the
summation of the portion of the
signal "caught" in the delay line
 Figure 8.5:    This rotated version is sometimes called a transverse delay line or a
transverse structure.
x(n)
z -1
z -1
z -1
z -1
h(0)
h(1)
h(2)
h(N)
∑
y(n)
 Figure 8.6:    You can also think of the coefﬁ cients as being frozen in the  h ( n ) buffer while the input
signal marches one sample to the right on each iteration.
258  Chapter  8
 So, if an ideal LPF has an IR in the shape sin( x )/ x  and we sample the IR, we get a discrete
IR  h ( n ). The more samples we take, the more accurate our version of the IR becomes. This
introduces the ﬁ rst way to design a FIR ﬁ lter: ﬁ nd an IR you like, sample it, and convolve
with it to produce the ﬁ ltered output.
   8.2    Using RackAFX’s Impulse Convolver
 RackAFX has a built-in module to do impulse convolution and a directory of IR ﬁ les that you
can experiment with. The impulses are stored in a directory called IR1024 and they are all
1024-point IRs. Some of them came from RackAFX itself—you can save IRs of any plug-in
you make, then load them into the convolver module. You will also learn to write your own
convolution plug-in and tell RackAFX that your software would like to receive IRs any time a
user loads or creates one using the built-in tools.
 First, let’s look at the Impulse Convolver tool. Open the modules menu item in RackAFX.
There are two built-in FIR modules: the Impulse Convolver 1024 and the FIR Designer
located at the bottom of the list. Choose the Impulse Convolver 1024 module. The analyzer
will then appear with the IR directory populated and additional IR buttons enabled. These
will allow you to load an IR ﬁ le into the convolver.
  8.2.1  Loading IR Files
 On the right side of the analyzer ( Figure 8.7 ), you will see a box full of the IRs in your
IR1024 directory. You might not have the exact same list as this one but you will have the
ﬁ le “optimal.64.sir” in the list. All the IR ﬁ les are named with the “.sir” sufﬁ x and must
be created in RackAFX or loaded using the RackAFX IR ﬁ le format (see the website for
details). RackAFX automatically saves your IRs as .wav ﬁ les so you can use them in other
applications. You can ﬁ nd these .wav ﬁ les in your IR1024 directory. If you want to convolve
with your own .wav ﬁ les, see the website for code examples.
(cid:44)R1024 Files
freqsamp.sir
impulse.sir
impulse156fir.sir
impulse16fir.sir
impulse_HPF.sir
impulse_LPF.sir
optimal.64.sir
optimal.sir
 Figure 8.7:    The IR ﬁ les are visible in the analyzer window.
Audio Filter Designs: FIR Filters  259
 At the bottom right, you will see the buttons for loading and saving IR ﬁ les. The ﬁ rst two,
Save h(n) and Load h(n), will save and load the .sir ﬁ les from the IR1024 directory. The lower
two buttons save and load the IR to the clipboard. The IR is actually C++ code, and you can
use the clipboard to paste this code directly into your own source code. You might do this to
hard-code a particular IR or for testing purposes, or to set a default IR.
 To get started, click on the ﬁ le named “optimal.64.sir” and then click the Load h(n) File
button (you can also double-click on the ﬁ le name to load it). The IR for the ﬁ le will load
into the analyzer as well as the Impulse Convolver module itself. You will automatically be
switched into the impulse view to see the imported ﬁ le data ( Figure 8.8 ). Next, click on the
Frequency Response button to see the ﬁ lter’s frequency response ( Figure 8.9 ). This LPF was
designed to have a cutoff frequency of 500 Hz and a very steep roll-off of about 50 dB/oct.
Load a wave ﬁ le and play the ﬁ le through the convolver. It will implement this 500-Hz LPF
exactly. Try loading some other IR ﬁ les and playing audio through them.
   8.2.2  Creating IR Files
 You can capture the IR of any RackAFX plug-in, including your own. As an example let’s
capture the IR of a built-in module ﬁ rst. Open the module named “HP/LP Filter” from the
module menu. Then, open the analyzer, click on the Frequency button and adjust the slider
1.000
0.707
0.500
0.000
–0.500
–0.707
–1.000
+12.0dB
0.0 dB
–12.0 dB
–24.0 dB
–36.0 dB
–48.0 dB
–60.0 dB
0
102
204
306
408
510
612
714
816
918
 Figure 8.8:    The IR for the optimal.64.sir ﬁ le.
10 Hz
100 Hz
1 kHz
10  kHz
 Figure 8.9:    The frequency response for the optimal.64.sir ﬁ le.
260  Chapter  8
controls to give you a unique shape. For example, I will make a highly resonant LPF by
setting the  Q  to 12 ( Figure 8.10 ).
 Click on the Impulse button in the analyzer. The IR of the ﬁ lter is shown in  Figure 8.11 .
This IR completely captures the ﬁ lter at these particular settings (  f  c   5 1 kHz,  Q  5 12).
If we store the IR of the IIR ﬁ lter, we can load it into the convolver and turn it into a FIR
ﬁ lter instead.
 Click on the Save h(n) File button and name it. It will then appear in the list of available IRs. Play
a wave ﬁ le through the module and remember what it sounds like. Now, go back and open the
Impulse Convolver 1024 module from the module menu. You will see your freshly created IR
in the list. Double-click on it to load it and you will see the original IR. Click on the Frequency
button and you will see the original frequency response. Next, play a wave ﬁ le through the
convolver and you should hear something that is remarkably similar to the original IIR ﬁ lter.
 But, how similar is it? The IR convolver module can convolve up to 1024-point IRs. If the IR
of the original ﬁ lter is longer than 1024 points, then the resulting FIR ﬁ lter will not be exactly
perfect. However, if it is shorter than 1024 points (meaning that the IR becomes 0.0 and
remains that way at some point in the IR duration) then the resulting FIR will be a dead-on
accurate version of the original. If you look at the IR for the original ﬁ lter, you can see that it
+24.0 dB
+10.0 dB
–4.0 dB
–18.0dB
–32.0 dB
–46.0 dB
–60.0 dB
1.000
0.707
0.500
0.000
–0.500
–0.707
–1.000
10 Hz
100 Hz
1 kHz
10  kHz
 Figure 8.10:    The frequency response for the resonant LPF test ﬁ lter.
0
102
204
306
408
510
612
714
816
918
 Figure 8.11:    The ringing IR of the resonant LPF.
Audio Filter Designs: FIR Filters  261
is still just barely ringing right there at the end, so we are not getting an exact duplicate, but
upon listening you should hear that they are nearly identical.
   8.2.3  The IR File Format
 The IR ﬁ le actually contains C++ code and you can quickly understand how it works by using
the clipboard functions. In the analyzer window that you still have pulled up, click on the
button h(n) → Clipboard and after the success message, open a text editor or a C++ compiler.
Then, use the operating system (OS) paste function to paste the clipboard data into your
editor. You will see something like this at the top:
  // h(n) Impulse Response
  // Length 5 1024
  m_nIRLength 5 1024;
  m_h_Left[0] 5 0.00000000;
  m_h_Left[1] 5 0.00503618;
  m_h_Left[2] 5 0.01998402;
  m_h_Left[3] 5 0.03938961;
  m_h_Left[4] 5 0.05777339;
  m_h_Left[5] 5 0.07477701;
  m_h_Left[6] 5 0.09007415;
  m_h_Left[7] 5 0.10337673;
  etc…
 This is the IR data starting with the length (1024) and then the left and right channels
respectively. If you scroll to the bottom, you will see the last few samples of the IR:
  m_h_Right[1016] 5 -0.00000850;
  m_h_Right[1017] 5 0.00004104;
  m_h_Right[1018] 5 0.00008918;
  m_h_Right[1019] 5 0.00013495;
  m_h_Right[1020] 5 0.00017745;
  m_h_Right[1021] 5 0.00021589;
  m_h_Right[1022] 5 0.00024953;
  m_h_Right[1023] 5 0.00027775;
 This was my resonant LPF and you can see that it is still ringing, even after 1024 samples.
If you listen to a wave ﬁ le through a ﬁ lter like this, you can hear pinging sounds at the peak
resonant frequency. These pinging noises are the ringing of the ﬁ lter. If you look at the C++
code you can tell that the IR data appears to be some kind of member variable information for
a C++ object because of the “m_” Hungarian notation used to describe the data.
262  Chapter  8
 All RackAFX plug-ins already have two default IR arrays declared as m_h_Left[1024] and
m_h_Right[1024] and another variable m_nIRLength that deﬁ nes how much of the 1024 point
IR buffer is being used. The FIR designer will let you create IRs with variable sizes—in many
cases, you don’t need all 1024 points to describe the IR of the system. You tell RackAFX that
you want it to populate your IR arrays by setting a ﬂ ag m_bWantIRs in your plug-in constructor.
When a user loads or creates an IR in the analyzer, it is automatically delivered to your plug-in.
    8.3    Using RackAFX’s FIR Designer
 RackAFX has a powerful built-in module called FIR Designer that lets you use two popular
methods to design FIR ﬁ lters: the optimal method and the frequency sampling method. The
optimal method is sometimes called the “Parks–McClellan algorithm.” When the module ﬁ rst
opens, you will see the new controls at the right side, as shown in  Figure 8.12 .
 This module creates IRs. You can save the IRs to a ﬁ le or the clipboard using the same
buttons as before. If your plug-in has the m_bWantIRs ﬂ ag set, any time the user hits the
Calculate button to make a new IR, it will automatically be delivered and copied into the
plug-in’s default IR arrays. Even though the FIR Designer defaults to the optimal method for
design, let’s begin with the frequency sampling method since the optimal method relies on it.
Optimal
method
Frequency
sampling
method
Shared
controls
 Figure 8.12:    The FIR designer controls consist of three parts. The order slider and edit box
and the Calculate and Complement buttons are shared between both methods. You enable the
method of choice using the radio button controls.
Audio Filter Designs: FIR Filters  263
   8.4    The Frequency Sampling Method
 The frequency sampling method is really interesting because it lets you design any kind
of frequency response you want—it can be any arbitrary shape and it does not have to be
a classical ﬁ lter type (LPF, high-pass ﬁ lter [HPF], band-pass ﬁ lter [BPF], band-stop ﬁ lter
[BSF]), but you can make these types if you want to. The frequency sampling method
involves these steps:
1.
2.
3.
4.
   Decide on a desired frequency response and plot it in the frequency domain.
   Sample the frequency response at evenly spaced intervals determined by the ﬁ lter order
you choose.
   Take the inverse discrete cosine-transform (DCT) of the sampled frequency response to
get the sampled IR (the DCT is simply the real part of the fast Fourier transform  [FFT]).
   Load the sampled IR into a convolver and go.
  8.4.1  Linear-Phase FIR Using the Frequency Sampling Method
 Choose:
   N  5 number of coefficients
 Calculate:
  For  N  5 odd:
•
   ( N  1 1)/2 5 number of samples in frequency domain, starting at 0 Hz
  For  N  5 even
•
   ∆ f  5  f  s   / N  5 frequency spacing, starting at 0 Hz
    N /2 5 number of samples in frequency domain, starting at 0 Hz
 Calculate the ﬁ lter coefﬁ cients a 0  to a  N /2  with  Equation 8.5 :
For  N  5  odd
1
N
an 5
(N21)/2
cH(0) 1 2 a
i51
` H(i)| cos | c2pi cn 2
N 2 1
2
d ^N d ` d
(8.5)
For  N  5  even
1
N
N/221
cH(0) 1 2 a
i51
an 5
` H(i)| cos | c2pi cn 2
N 2 1
2
d ^N d ` d
 Note: This produces half the coefﬁ cients; the other half are a mirror image, as shown in the
example below. Because the IR is guaranteed to be symmetrical about its center, it produces a
linear phase FIR ﬁ lter every time.

264  Chapter  8
 Example: Design an LPF with a cutoff of 5.5 kHz,  f  s  5 44.1 kHz,  N  5 16.
 Solution:
1.
2.
    N  5 16, which produces eight sampled points in the frequency domain with a spacing
of 2.756 kHz.
   Sample the plot, producing the magnitude response,  H ( i ) ( Figure 8.13 ).
 For this plot notice that:
•
•
   The plot is linear in frequency. To design a frequency sampling method ﬁ lter by hand, it
is often easier to start with a linear frequency axis.
   The point at Nyquist is not used; it is there to pin down the response that would occur
after Nyquist.
 The sampled frequency response is read directly off the plot: (0 dB 5 1.0, −60 dB 5 0.001).
 u H ( i )u 5 {1.0, 1.0, 1.0, 0.001, 0.001, 0.001, 0.001, 0.001}
1.
   Use  Equation 8.5  to extract IR  h ( n ), which are the coefﬁ cients, a  n  .
  a 0  5 0.04858366
  a 1  5 0.00364087
  a 2  5 20.05199205
  a 3  5 20.07047625
  a 4  5 20.02194221
  a 5  5 0.08695625
  a 6  5 0.21101949
  a 7  5 0.29421023
  a 8  5 0.29421023
  a 9  5 0.21101949
  a 10  5 0.08695625
  a 11  5 20.02194221
  a 12  5 20.07047625
  a 13  5 20.05199205
  a 14  5 0.00364087
  a 15  5 0.04858366
 Notice the symmetry about a 7  to a 8  boundary.
2.
   Measure the response ( Figure 8.14 ).
 The resulting ﬁ lter is guaranteed to exactly match the desired frequency response at the
sampled points only. In between the sampled points, the frequency response can do anything;
Audio Filter Designs: FIR Filters  265
+12.0dB
0.0 dB
–12.0 dB
–24.0 dB
–36.0 dB
–48.0 dB
–60.0 dB
2 kHz
4 kHz  6 kHz  8 kHz
10 kHz  12 kHz  14 kHZ  16 kHz  18 kHz  20 kHz
0 Hz
2.7 kHz
5.5  kHz
8.2  kHz
11 kHz
13.7 kHz
16.5 kHz
19 kHz
Nyquist
 Figure 8.13:    Here is our prototype LPF with cut-off at 5.5 kHz; its response becomes −60 dB
(0.001) at 8.2 kHz, which is a steep roll-off.
rippling in the pass band and stop band can occur as shown here. You can see that this is a
pretty bad rendition of our desired response.
 The ﬁ rst lobe in the stop band only produces about −14 dB of attenuation, which is poor
considering that we wanted a ﬁ lter that would have a stop-band attenuation of −60 dB.
To improve the response, you have several choices:
•
•
•
   Relax the speciﬁ cations.
   Add more points to the desired response.
   Increase the ﬁ lter order.
 Relax the speciﬁ cations and roll off the response less steeply by changing the point at 8.2 kHz
to 212 dB (0.25) instead of 260 dB ( Figure 8.15 ). Now, the sampled frequency response is
 u H ( i )u 5 {1.0, 1.0, 1.0, 0.25, 0.0, 0.0, 0.0, 0.0}
 The resulting magnitude response | H ( f  )| is shown in  Figure 8.16 .
+12.0 dB
0.0 dE
–12.0 dB
–24.0 dB
–36.0 dB
–48.0 dB
–60.0 dB
2 kHz  4 kHz  6 kHz  8 kHz  10 kHz  12 kHz  14 kHz  16 kHz  18 kHz  20 kHz
 Figure 8.14:    The resulting low-pass ﬁ lter magnitude response.
266  Chapter  8
+12.0dB
0.0 dB
-12.0dB
-24.0 dB
-36.0 dB
-48.0 dB
-60.0 dB
2 kHz  4 kHz  6 kHz  8 kHz  10 kHz  12 kHz  14 kHz  16 kHz  18 kHz  20 kHz
 Figure 8.15:    The same design with speciﬁ cations relaxed; the slope is less steep.
+12.0 dB
0.0 dB
-12.0 dB
-24.0 dB
-36.0 dB
-48.0 dB
-60.0 dB
2 kHz  4 kHz  6 kHz  8 kHz  10 kHz  12 kHz  14 kHz  16 kHz  18 kHz  20 kHz
 Figure 8.16:    The relaxed magnitude response shows improved stop-band attenuation.
Now, the ﬁ rst lobe in the stop band has moved to a magnitude of about 230 dB, an
improvement of about 15 dB.
    8.5    Complementary Filter Design for Linear Phase FIR Filters
 This technique results in a complementary ﬁ lter, rotated about the center of the Nyquist
bandwidth, that is, rotated about Nyquist/2. To convert an LPF to HPF or vice versa on a linear
phase FIR:
  For  N  5 even
•
  For  N  5 odd
•
   Multiply the even-numbered coefﬁ cients by 21.
   Multiply the odd-numbered coefﬁ cients by 21.
 This will rotate the frequency response around Nyquist/2 such that an LPF will become an
HPF. However, they will not share the same cutoff frequency, but will rather be mirror images
of each other. Thus, the ﬁ rst ﬁ lter design above with a cutoff point of 5.5 kHz would produce
an HPF with a cutoff frequency 5.5 kHz  below Nyquist .  Table 8.1  shows the coefﬁ cients,
while  Figure 8.17  shows the frequency response.
Audio Filter Designs: FIR Filters  267
  Table 8.1:    The LPF and complementary HPF coefﬁ cients for the current design.
Low-Pass Filter
Complementary High-Pass Filter
a0 5 0.02598719
a1 5 0.01331121
a2 5 20.02018474
a3 5 20.05761180
a4 5 20.04827195
a5 5 0.04957144
a6 5 0.20692666
a7 5 0.33027202
a8 5 0.33027202
a9 5 0.20692666
a10 5 0.04957144
a11 5 20.04827195
a12 5 20.05761180
a13 5 20.02018474
a14 5 0.01331121
a15 5 0.02598719
a0 5 20.02598719
a1 5 0.01331121
a2 5 0.02018474
a3 5 20.05761180
a4 5 0.04827195
a5 5 0.04957144
a6 5 20.20692666
a7 5 0.33027202
a8 5 20.33027202
a9 5 0.20692666
a10 5 20.04957144
a11 5 20.04827195
a12 5 0.05761180
a13 5 20.02018474
a14 5 20.01331121
a15 5 0.02598719
+12.0 dB
0.0 dB
-12.0 dB
-24.0 dB
-36.0 dB
-48.0 dB
-60.0 dB
2  kHz
4  k Hz  6  kHz  8  kHz  10  kHz  12  kHz
14 kHz  16 kHz
18  kHz
20 kHz
 Figure 8.17:    The complementary HPF has its cut-off point at 16.5 kHz and you can see the
rotation about the ½ Nyquist frequency.
   8.6    Using RackAFX’s Frequency Sampling Method Tool
 RackAFX allows you to implement frequency sampling method ﬁ lters by drawing responses
directly on the analyzer’s frequency response plots. You can place as many points as you
like as well as choose the order (even only) of the ﬁ lter. The previous examples were done
using the frequency sampling method tool. Let’s do an example similar to the last one but
in the log frequency domain instead. First, click on the radio button marked frequency
sampling method and you will see a red horizontal line appear on the graphs. Make sure the
frequency axis is log. In this design, let’s try to make a resonant LPF with a peak frequency
around 1 kHz.
268  Chapter  8
 You will see two boxes on the red line, one at DC, the other at Nyquist. These points cannot
be removed. To enter points and move them, use the following rules:
•
•
•
•
   Right-click on the red line to add a new point.
   Click on the new point and drag it up or down.
   To remove a point, right-click on it and choose “delete point” from the pop-up menu.
   Hit the Calculate button when you are done to view the resulting ﬁ lter.
 Hit the Frequency Sampling button and note the ﬁ lter order of 64 (if it is not 64, change it
with the slider or the edit control). Create a resonant LPF with a peak frequency of 1 kHz. In
 Figure 8.18  you can see the bandwidth of the peak is 1.5 kHz and the response goes to 260
dB at 5 kHz. Next, hit the Calculate button to get  Figure 8.19 .
 You can use the order slider to slowly increase the ﬁ lter order until the response matches your
sampled points as closely as possible. You can move the slider, or type an order into the edit
box and hit the Tab button to inject it. You can also use the small up/down arrow buttons to
advance the order up or down in even increments. Typically, the way you would use this is to
ﬁ rst get close with the slider, then use the up/down arrows to ﬁ ne tune the design.  Figure 8.20
shows the same ﬁ lter with the order set to 164.
+12.0 dB
0.0 dB
-12.0  dB
-24.0  dB
-36.0  dB
-48.0  dB
-60.0  dB
+12.0 dB
0.0 dB
-12.0 dB
-24.0 dB
-36.0 dB
-48.0 dB
-60.0 dB
10 Hz
100 Hz
1 kHz
10 kHz
 Figure 8.18:    The prototype resonant LPF sampled points.
10 Hz
100 Hz
1 kHz
10 kHz
 Figure 8.19:    The 64-tap FIR ﬁ lter produces marginal results with poor stop-band attenuation.
Audio Filter Designs: FIR Filters  269
 Play an audio ﬁ le through the new ﬁ lter and listen to the resonant LPF characteristics. Here
are some interesting things you can do in RackAFX while the audio ﬁ le is playing or looping:
•
•
•
   You can move the order control; the ﬁ lter will be updated in real time and you can hear
the results.
   You can add or remove points on the desired response or move them around, then hit
Calculate to update the ﬁ lter in real time, and you can also hear the results.
   You can save the IR as a ﬁ le, then load it into the Impulse Convolver module just as
before.
   8.7    Designing a Complementary Filter
 You can convert any design into a complementary design by hitting the Complementary
button. With the current 164 th -order resonant LPF, ﬁ rst switch to the linear scale
( Figure 8.21 ).
 Hit the Complement button to create the complementary HPF ﬁ lter. The original design
points are left to show you the complementary nature of the ﬁ lter. You can clearly see the
rotation about ½ Nyquist here ( Figure 8.22 ). You can perform this operation while audio
+12.0 dB
0.0 dB
-12.0 dB
-24.0 dB
-36.0 dB
-48.0 dB
-60.0 dB
10 Hz
100 Hz
1 kHz
10 kHz
 Figure 8.20:    The 164th  -order FIR ﬁ lter produces an excellent match to the speciﬁ cations.
+12.0 dB
0.0 dB
-12.0 dB
-24.0 dB
-36.0 dB
-48.0 dB
-60.0 dB
2 kHz  4 kHz  6 kHz  8 kHz  10 kHz  12 kHz  14 kHz  16 kHz  18 kHz  20 kHz
 Figure 8.21:    The linear domain frequency response of the 1-kHz LPF.
270  Chapter  8
+12.0 dB
0.0 dB
-12.0  dB
-24.0  dB
-36.0  dB
-48.0  dB
-60.0  dB
2 kHz  4 kHz  6 kHz  8 kHz  10 kHz  12 kHz  14 kHz  16 kHz  18 kHz  20 kHz
 Figure 8.22:    The complementary HPF shows the mirror image nature of complementary ﬁ lter
designs; the new resonant peak is 1 kHz below Nyquist.
ﬁ les are playing in RackAFX as well. If you look at the IRs in the .sir ﬁ le you can see the
operation of negating the odd-numbered coefﬁ cients.
   8.8    The Optimal (Parks–McClellan) Method
 FIR ﬁ lters produce all zeros in the  z -plane. You can see the effect of these zeros in the
frequency sampling method ﬁ lters we designed—they create the notches and humps in the
frequency response. In general, we seek to minimize the humps in the pass band and set some
kind of limit for the largest lobe we see in the stop band. In the frequency sampling ﬁ lters,
we changed the design speciﬁ cations or the order of the ﬁ lter to get a response that satisﬁ ed
our initial desired response to some extent. The optimal method is an algorithm for designing
classical ﬁ lters (LPF, HPF, BPF, BSF). You can create ﬁ lters with extremely steep roll-off
edges that would be difﬁ cult to synthesize in the analog domain. The word “optimal” comes
from the engineering phrase “optimal in the Chebychev way,” which describes a ﬁ lter design
with a certain kind of rippling in the pass band and stop band.
 The LPF in  Figure 8.23  might be speciﬁ ed like this:
•
•
   Pass-band ripple: less than 0.1 dB
   Stop-band attenuation: greater than −24 dB
 The pass-band ripple (0.1 dB) and stop-band attenuation (−24 dB) are converted into
weighting values using  Equation 8.6 :
Pass-band weight  5 (10pb_ripple(dB)@20) 2 1
Stop-band weight  5 (10sb_atten(dB)@20)
(8.6)
 The optimal method uses these weighting values in its design calculation. The weightings
give acceptable error tolerances in each band. It the weights are equal, then there will be
