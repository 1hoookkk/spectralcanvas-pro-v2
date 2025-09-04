# Audio Plugins C++ (Pirkle) — Chunk 39/73

> Source: `Designing Audio Effect Plug-Ins in C++_ With Digital Audio Signal Processing Theory - PDF Room.pdf` · Pages ~1-560 · Extractor: pdfminer.six
> Target ≈ 2800 words, Overlap = 200 words · Actual = 5508

> Note: DSP textbook content for SpectralCanvasPro RT-audio knowledge base

---
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

Audio Filter Designs: FIR Filters  271
Pass-band  ripple
Stop-band
attenuation
10 Hz
100 Hz
1 kHz
10 kHz
+12.0 dB
0.0 dB
-12.0 dB
-24.0 dB
-36.0
dB
-48.0 dB
-60.0 dB
 Figure 8.23:    An FIR LPF can be speciﬁ ed by the acceptable ripple in the pass band (barely
observable here) and the minimum stop-band attenuation.
an equal deviation from the ideal in both bands. This is called an  equiripple  design. The
speciﬁ cations for the optimal method are:
•
•
•
   The pass-band and stop-band frequencies
   The maximum pass-band ripple and minimum stop-band attenuation
   The order of the ﬁ lter (the number of zeros in the frequency response)
 The algorithm works by ﬁ rst distributing the zeros out equally across the frequency axis. This
is going to produce a series of humps in the pass band (the ripple) and lobes in the stop band.
The ripples and lobes will produce a set of maxima and minima frequencies. The problem is
that for a given speciﬁ cation, the locations of the extremal frequencies are not known before
hand. The optimal method must use a search algorithm to iteratively move the locations
around until a solution is found that is within the tolerance of the speciﬁ cations. The optimal
method uses an algorithm called the Remez exchange method to iteratively move the zeros
around until a solution converges. It must be noted that the Remez exchange algorithm is not
guaranteed to converge for a given set of speciﬁ cations.
 Once the Remez exchange method ﬁ nds the locations of the extremal frequencies, the
resulting  H ( f  ) graph is sampled using the frequency sampling method to produce the IR,
which is the set of FIR ﬁ lter coefﬁ cients. To summarize the method:
•
•
•
•
•
   Specify the ﬁ lter parameter and order.
   Equally distribute zeros across the spectrum.
   Set the pass-band and stop-band weights.
   Use the Remez exchange algorithm to iteratively ﬁ nd the location of the zeros.
   Sample the resulting frequency response to produce the ﬁ nal IR.
   8.9    Using RackAFX’s Optimal Method Tool
 RackAFX lets you design any of the four classical ﬁ lter types using the equiripple weighting
for the pass band and stop band. BPF and BSF ﬁ lters have an additional band edge to specify,
272  Chapter  8
but the meaning of pass-band ripple and stop-band attenuation is the same for all ﬁ lter types.
You can start with an LPF design by clicking on the Optimal button. Try the following ﬁ lter
speciﬁ cations:
•
•
•
•
   Type: LPF
   F_pass low: 1 kHz (the low edge cut-off frequency)
   F_stop low: 2 kHz (the lowest frequency that must receive the required stop band
 attenuation)
   Filter order: 16
 Now, use the Calculate button to generate the ﬁ lter. You can see from  Figure 8.24  that the
ﬁ lter is not performing exactly to speciﬁ cations. Although the pass band looks good, the stop
band does not. Next, begin increasing the order of the ﬁ lter using the slider or nudge buttons
until you get the stop-band attenuation you desire; for −48 dB of attenuation, you will need a
104 th -order ﬁ lter as shown in  Figure 8.25 . Finally, check the IR and you will see a truncated
version of the sin x ( x )/ x   function ( Figure 8.26 ).
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
 Figure 8.24:    The pass-band ripple is well within spec; however, the stop-band attenuation is only
getting to about −12 dB.
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
 Figure 8.25:    The 104 th -order optimal method LPF produces the desired stop-band attenuation.
Audio Filter Designs: FIR Filters  273
 The Remez exchange algorithm is not guaranteed to converge. You will receive an error message
if it does not converge. Even if the algorithm does converge, the resulting IR is not guaranteed
to be finite. Increasing the filter order will not necessarily produce a better design.
1.000
0.707
0.500
0.000
-0.500
-0.707
-1.000
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
 Figure 8.26:    The IR for the 104 th -order optimal method LPF.
 Experiment:
•
•
•
•
•
•
   Load a wave ﬁ le and audition the ﬁ lter.
   Try the other ﬁ lter types (HPF, BPF, BSF).
   Adjust the order control noting when the Remez exchange algorithm fails to converge or
the ﬁ lter blows up.
   Try to ﬁ nd the lowest possible ﬁ lter order to just match the desired speciﬁ cations.
   Save IR ﬁ les with various ﬁ lters you design.
   Copy and paste the IR code into your own convolution plug-in.
   8.10    Design a Convolution Plug-In
 In order to implement the convolution (FIR) algorithm you need to use the delay line theory
from the last chapter. The ﬁ lters often need hundreds or thousands of delay elements and you
know that a circular buffer works perfectly for storing and updating a sequence of  x ( n ),  x ( n  2 1),
 x ( n 2  2)… In addition, the IR will need to be stored in a buffer and accessed sequentially with a
pointer like the input buffer. The convolution equation in  Equation 8.2  accumulates from 2` to
1` which uses both past and future data. We can only use past data and so we only need half of
the equation. The generalized FIR convolution equation is  Equation 8.7 :
  y(n) 5 h0x(n) 1 h1x(n 2 1) 1 h2x(n 2 2) 1 ... 1 hM(n 2 M)
(8.7)
 The number of delay elements required is  M −1 since the ﬁ rst term h 0  x ( n ) operates on the
current, undelayed input signal. So, a 64-tap FIR requires 63 delay elements. Remember
from  Chapter 7  that when we access a circular buffer and write the current input sample,
we are overwriting the oldest sample,  x ( n  2  M ) but we can use this to our advantage in this
274  Chapter  8
case by using a 64-element circular buffer to implement a 64-tap FIR and by writing in the
ﬁ rst sample before doing the convolution operation. This will give us a buffer with  x ( n )
through  x ( n  2  M  2 1) lined up and ready for access. We will have an identically sized buffer
to store the IR,  h ( n ). During the convolution operation we will zip through both buffers at
the same time, accumulating the product of each operation. The only tricky thing is that the
IR buffer will be reading sequentially from top to bottom exactly once each sample period to
create the sequence  h (0),  h (1),  h (2), and so on. The input buffer will be circular and reading
 backwards  to create the sequence  x ( n ),  n ( n  2 1),  x ( n  2 2), and so on, shown graphically in
 Figure 8.27 .
 If you look at your base class ﬁ le, PlugIn.h, you will ﬁ nd the declarations of your built-in IR
buffers and variables:
  // impulse response buffers!
   ﬂ oat m_h_Left[1024];
   ﬂ oat m_h_Right[1024];
  // the length of the IR from 0 to 1024
   int m_nIRLength;
  // ﬂ ag to set to request impulse responses from the UI
   bool m_bWantIRs;
Input  Buffer x
Impulse  Buffer h
Input  Buffer x
Impulse  Buffer  h
x (n - 4)
x (n - 3)
x (n - 2)
x (n - 1)
x (n)
x (n - D - 1)
etc...
x (n - 5)
h(0)x(n)
h(0)
h(1)
h(2)
h(3)
h(4)
x(5)
etc...
h(D)
x (n - 4)
x (n - 3)
x (n - 2)
x (n - 1)
x (n)
x (n - D - 1)
etc...
x (n - 5)
h(0)
h(1)
h(2)
h(3)
h(4)
x(5)
etc...
h(D)
Input  Buffer x
Impulse  Buffer  h
Input  Buffer x
Impulse  Buffer  h
x (n - 4)
x (n - 3)
x (n - 2)
x (n - 1)
x (n)
x (n - D - 1)
etc...
x (n - 5)
h(2)x(n-2)
h(0)
h(1)
h(2)
h(3)
h(4)
x(5)
etc...
h(D)
x (n - 4)
x (n - 3)
x (n - 2)
x (n - 1)
x (n)
x (n - D - 1)
etc...
x (n - 5)
h(0)
h(1)
h(2)
h(3)
h(4)
x(5)
etc...
h(D)
h(1)x(n - 1)
h(3)x(n - 3)
 Figure 8.27:    The ﬁ rst four multiplication operations for a convolution operation shows how one
buffer reads backward while the other reads forward.
Audio Filter Designs: FIR Filters  275
Variable
m_h_Left[1024]
m_h_Right[1024]
m_nIRLength
m_bWantIRs
  Table 8.2:    IR variables
Description
The IR buffer for the left channel
The IR buffer for the right channel
The length of the current convolution
A ﬂ ag to tell RackAFX to populate your IR
buffers automatically whenever the user
loads an IR ﬁ le or creates a ﬁ lter with the
FIR Designer tool
 These variables are shown in  Table 8.2 .
  8.10.1  Project: Convolver
 Create a new RackAFX project; I named mine “Convolver.” It has no GUI elements
to set up.
   8.10.2  Convolver.h File
 Declare the variables you need to implement a stereo convolution and remember that the IR
buffers and length variables are already declared for you. We need the following:
•
•
•
•
•
   Buffer to hold the  x  input for the left channel
   Buffer to hold the  x  input for the right channel
   Read index for the IR delay line (buffers)
   Read index for the input ( x ) buffers
   Write index for sequentially writing input samples into the delay lines
  // Add your code here: -------------------------------------------------- //
  //
  // pointers to the left and right input buffers
   ﬂ oat* m_pBufferLeft;
   ﬂ oat* m_pBufferRight;
  // read index for delay lines (input x buffers)
   int m_nReadIndexDL;
  // read index for impulse response buffers
   int m_nReadIndexH;
  // write index for input x buffer
   int m_nWriteIndex;
276  Chapter  8
   8.10.3  Convolver.cpp File
  Constructor
•
•
•
   Create the buffers.
   Flush the buffers; reset to 0.0.
   Reset all indices.
  CConvolver::CConvolver()
  {
  <SNIP SNIP SNIP>
  // Finish initializations here
      // set our max buffer length for init
   m_nIRLength = 1024;   // 1024max
      // dynamically allocate the input x buffers and save the pointers
   m_pBufferLeft = new ﬂ oat[m_nIRLength];
   m_pBufferRight = new ﬂ oat[m_nIRLength];
      // ﬂ ush x buffers
   memset(m_pBufferLeft, 0, m_nIRLength*sizeof(ﬂ oat));
   memset(m_pBufferRight, 0, m_nIRLength*sizeof(ﬂ oat));
      // ﬂ ush IR buffers
   memset(&m_h_Left, 0, m_nIRLength*sizeof(ﬂ oat));
   memset(&m_h_Right, 0, m_nIRLength*sizeof(ﬂ oat));
  // reset all indices
   m_nReadIndexDL = 0;
   m_nReadIndexH = 0;
   m_nWriteIndex = 0;
  }
 You can see that we’ve allocated the memory for input buffers of maximum size 1024. We’ve
also ﬂ ushed out all buffers by setting all data to 0.0 with memset() and ﬁ nally reset the
indices to all point to the tops of the buffers.
   Destructor
•
   Since we allocated memory in the constructor, we need to remove it in the destructor:
  CConvolver::~CConvolver(void)
Audio Filter Designs: FIR Filters  277
  {
  }
  // free up our input buffers
   delete [] m_pBufferLeft;
   delete [] m_pBufferRight;
   prepareForPlay()
•
   We also need to ﬂ ush the buffers and reset the indices:
  bool __stdcall CConvolver::prepareForPlay()
  {
  }
  // Add your code here:
  // ﬂ ush buffers
  memset(m_pBufferLeft, 0, m_nIRLength*sizeof(ﬂ oat));
  memset(m_pBufferRight, 0, m_nIRLength*sizeof(ﬂ oat));
  // reset indices
  m_nReadIndexDL = 0;
  m_nReadIndexH = 0;
  m_nWriteIndex = 0;
  return true;
   processAudioFrame()
•
   Implement the convolution loop.
 The loop will need to do the following operations (these are not streamlined in the code for
ease of reading; you can always improve on the code by combining steps together). You are
implementing the operation in  Figure 8.27 :
•
•
•
•
•
•
•
   Read the current sample  x ( n ) and write it into the buffer.
   Reset the delay line read pointer to the current input sample,  x ( n ); the pointer needs to be
reset because its value will change and be destroyed later in the loop.
   Reset the IR read index to point to the top of the buffer at location 0.
   Set up the accumulator and create the convolution loop.
   After forming each product  h ( i )x ( n  2  i ),  increment  the IR buffer read pointer and
 decrement  the delay line read pointer.
   Check for a wrap after the delay line pointer is decremented.
   Write the output sample.
278  Chapter  8
•
•
   Process the second (right) channel the same way.
   Increment the delay line write index and wrap if necessary.
  bool __stdcall CCConvolver::processAudioFrame(ﬂ oat* pInputBuffer, ﬂ oat* pOutputBuffer,
  UINT uNumInputChannels, UINT uNumOutputChannels)
  {
  // Do LEFT (MONO) Channel; there is always at least one input/one output
  // Read the Input
   ﬂ oat xn = pInputBuffer[0];
      // write x(n) -- now have x(n) -> x(n–1023)
   m_pBufferLeft[m_nWriteIndex] = xn;
  // reset: read index for Delay Line -> write index
   m_nReadIndexDL = m_nWriteIndex;
  // reset: read index for IR - > top (0)
   m_nReadIndexH = 0;
  // accumulator
   ﬂ oat yn_accum = 0;
  // convolve:
   for(int i=0; i<m_nIRLength; i++)
   {
   }
  // do the sum of products
   yn_accum += m_pBufferLeft[m_nReadIndexDL]*m_h_Left[m_nReadIndexH];
  // advance the IR index
   m_nReadIndexH++;
  // decrement the Delay Line index
   m_nReadIndexDL--;
  // check for wrap of delay line (no need to check IR buffer)
   if(m_nReadIndexDL < 0)
   m_nReadIndexDL = m_ nIRLength -1;
  // write out
  pOutputBuffer[0] = yn_accum;
 Now you should try to write the code for the second (right) channel. After you’re done, check
it against the following code:
  // Mono-In, Stereo-Out (AUX Effect)
  if(uNumInputChannels == 1 && uNumOutputChannels == 2)
   pOutputBuffer[1] = pOutputBuffer[0]; // just copy
Audio Filter Designs: FIR Filters  279
  // Stereo-In, Stereo-Out (INSERT Effect)
  if(uNumInputChannels == 2 && uNumOutputChannels == 2)
  {
  // Read the Input
   xn = pInputBuffer[1];
  // write x(n) -- now have x(n) -> x(n–1023)
   m_pBufferRight[m_nWriteIndex] = xn;
  // reset: read index for Delay Line -> write index
   m_nReadIndexDL = m_nWriteIndex;
  // reset: read index for IR - > top (0)
   m_nReadIndexH = 0;
  // accumulator
   yn_accum = 0;
  // convolve:
   for(int i=0; i<m_nIRLength; i++)
   {
  // do the sum of products
   yn_accum +=
   m_pBufferRight[m_nReadIndexDL]*m_h_Right
[m_nReadIndexH];
  // advance the IR index
   m_nReadIndexH++;
  // decrement the Delay Line index
   m_nReadIndexDL--;
  // check for wrap of delay line (no need to check IR buffer)
   if(m_nReadIndexDL < 0)
   m_nReadIndexDL = m_nIRLength-1;
  }
  // write out
   pOutputBuffer[1] = yn_accum;
  }
  // incremnent the pointers and wrap if necessary
   m_nWriteIndex++;
   if(m_nWriteIndex >= m_nIRLength)
   m_nWriteIndex = 0;
  return true;
  }
280  Chapter  8
 Increment the buffer write pointer after the end of the right channel processing. The reason
is that it is shared between both left and right (just like the IR buffer read index is shared
between the left and right IR buffers). Build and test the code; in order to quickly test the
code, you can use a feature of the FIR Designer and copy an IR to the clipboard, then paste
it into the constructor. Open the FIR Designer and use the default optimal method LPF
design—it will automatically be calculated when you ﬁ rst see the FIR Designer interface. Hit
the button h(n) → Clipboard to copy the IR code to your Windows clipboard. Now, go to the
constructor and use the paste function Ctrl+V to load in the IR code. Do this at the end of all
the initialization stuff you wrote in Step 2. It should look like this:
   Constructor
  CConvolver::CConvolver()
  {
  <SNIP SNIP SNIP>
  // reset all indices
  m_nReadIndexDL = 0;
  m_nReadIndexH = 0;
  m_nWriteIndex = 0;
  // h(n) Impulse Response
  // Length = 1024
   m_nIRLength = 65;
   m_h_Left[0] = −0.00057406;
   m_h_Left[1] = −0.00016395;
   m_h_Left[2] = −0.00015001;
   m_h_Left[3] = −0.00007587;
   m_h_Left[4] = 0.00007552;
   m_h_Left[5] = 0.00029672;
   m_h_Left[6] = 0.00063462;
   m_h_Left[7] = 0.00108109;
  etc...
   m_h_Right[1018] = 0.00000000;
   m_h_Right[1019] = 0.00000000;
   m_h_Right[1020] = 0.00000000;
   m_h_Right[1021] = 0.00000000;
   m_h_Right[1022] = 0.00000000;
   m_h_Right[1023] = 0.00000000;
  }
Audio Filter Designs: FIR Filters  281
 This IR has 65 samples and will create a 65-tap FIR ﬁ lter. Compile the dynamic link
library  (DLL) and load it into RackAFX. Play a wave ﬁ le through it; it’s an LPF at
1 kHz, so this will be easy to verify by ear. Open the analyzer window and look at the
frequency and IRs—these will also be identical to the original design. Now that you have
veriﬁ ed that your convolution works properly, you can set the IR ﬂ ag in your constructor
to let RackAFX know to deliver your IRs when the user makes, loads, or calculates
them. You can also remove or comment-out the default IR code you pasted in to do the
initial testing:
  CConvolver::CConvolver()
  {
  }
  <SNIP SNIP SNIP>
  // reset all indices
  m_nReadIndexDL = 0;
  m_nReadIndexH = 0;
  m_nWriteIndex = 0;
  // set the ﬂ ag for RackAFX to load IRs into our convolver
   m_bWantIRs = true;
 Now, build and load your DLL. Next, open the analyzer window and you will see two
new buttons on the right below the IR/FIR design area. Because your plug-in wants
IRs, RackAFX reveals these two hidden buttons on the analyzer panel allowing you
to use either the FIR Designer or the IR directory to create or load IR ﬁ les. As soon
as this window opens, it delivers the current IR loaded, which is the optimal LPF
design by default. If you change the design it will remember it if you close the
analyzer.
 Load a wave ﬁ le and hit Play—this will play the optimal LPF design. Now, hit one of the
other buttons (HPF, BPF, or BSF) and the IR will automatically be calculated and loaded
into your plug-in, even if an audio ﬁ le is playing. Test the other IR tools, like the frequency
sampling method and the IR Directory, and listen to the resulting ﬁ lters.
     8.11    Numerical Method FIR Filters
 The last type of FIR ﬁ lters consists of numerical methods and is not based on IRs or
convolution, even though many of them implement a convolution loop. There are books full of
numerical methods for performing all kinds of operations, from integration and differentiation
to moving averages and interpolation. In this section we will examine a few algorithms and if
282  Chapter  8
it interests you, get a book on numerical methods and you will have a plethora of algorithms
to try out. Some of these ﬁ lters can be implemented as FIR topologies while others are better
suited for a straight mathematical function call.
  8.11.1  Moving Average Interpolator
 Choose:
   N  5 number of points to interpolate, minimum 5 2
 Calculate:
  ak 5
1
N
 , where  0 # k # N 2 1
(8.8)
 The moving average interpolator (or MA ﬁ lter) in  Figure 8.28  implements a sliding window
of  N  samples wide, over which the input samples are averaged. Each sample gets an equal
weight of 1/ N .
 Example: Design a ﬁ ve-point MA ﬁ lter.
 Solution:
  N  5 5, so a k  5 1/5 5 0.20
 To code this in RackAFX, you would ﬂ ush the IR buffers, then set the ﬁ rst ﬁ ve values of each
impulse array to 0.2 and set the m_nIRLength variable to 5:
  // disable RackAFX IRs for the MA ﬁ lter test
   m_bWantIRs = false;
  // h(n) Impulse Response
  // MA Filter N = 5
x(n)
-1 Z
-1 Z
-1 Z
-1
Z
0.2
0.2
0.2
0.2
0.2
(cid:153)
y(n)
 Figure 8.28:    The implementation of a ﬁ ve-point MA ﬁ lter.
