# Audio Plugins C++ (Pirkle) — Chunk 41/73

> Source: `Designing Audio Effect Plug-Ins in C++_ With Digital Audio Signal Processing Theory - PDF Room.pdf` · Pages ~1-560 · Extractor: pdfminer.six
> Target ≈ 2800 words, Overlap = 200 words · Actual = 2705

> Note: DSP textbook content for SpectralCanvasPro RT-audio knowledge base

---
Audio Filter Designs: FIR Filters  283
   m_nIRLength = 5;
   m_h_Left[0] = 0.2;
   m_h_Left[1] = 0.2;
   m_h_Left[2] = 0.2;
   m_h_Left[3] = 0.2;
   m_h_Left[4] = 0.2;
   m_h_Right[0] = 0.2;
   m_h_Right[1] = 0.2;
   m_h_Right[2] = 0.2;
   m_h_Right[3] = 0.2;
   m_h_Right[4] = 0.2;
  }
 Of course you could also write a function to calculate and populate the IR buffers, but this
one is short enough to code by hand if you want.
 The frequency response ( Figure 8.29 ) of an MA ﬁ lter is always an LPF. The more samples
that are averaged, the more stop-band attenuation and the more zeros get inserted into the
 z -plane. For this ﬁ lter there is a pair of complex conjugate zeros which produce two null
points in the response.
 You can see this is a pretty poor LPF compared to some of the optimal ﬁ lter designs, but these
ﬁ lters can be very useful, for example to smooth the response of an audio detector output or
to insert in the feedback path of a delay line module to gently roll off high frequencies on
each pass through the delay. You can get better stop-band attenuation by increasing the order
of the ﬁ lter. This will also effectively pull in the corner frequency, which looks like about
2.5 kHz here. Experiment with higher-order MA ﬁ lters, or add a slider to let the user adjust
the window size. This exact ﬁ lter already exists as a RackAFX module called a “Smoothing
Filter” for you to experiment with.
+12.0 dB
0.0 dB
-12.0 dB
-24.0 dB
-36.0 dB
-48.0 dB
-60.0 dB
10 Hz
100  Hz
1 kHz
10 kHz
 Figure 8.29:    The MA ﬁ lter with a window of 5 samples.
284  Chapter  8
   8.11.2  Lagrange Interpolator
 Choose:
   N  5 number of points to interpolate, minimum 5 2
 Calculate:
n
  aj 5    q
k 2  j
k 5  0
x 2 xk
xj 2 xk
(8.9)
 The Lagrange interpolator uses a polynomial of order  j  2 1 to interpolate across the
window of points that you give it. The window is of length  j  in the above equation. This is
a complex ﬁ lter because the coefﬁ cients change every sample period and they are based on
the window of input values, x 0  to x j . This ﬁ lter can be implemented as a pure math function
call. To facilitate your coding, a Lagrange interpolation function is implemented in your
pluginconstants.h ﬁ le:
  /*
  Function:  lagrpol()
implements n-order Lagrange Interpolation
  Inputs:      double* x
Pointer to an array containing the x-coordinates of the
input values
double* y
Pointer to an array containing the y-coordinates of the
input values
int n
The order of the interpolator, this is also the length of
the x,y input arrays
double xbar  The x-coorinates whose y-value we want to interpolate
  Returns      The interpolated value y at xbar. xbar ideally is between the middle two
values in the input array, but can be anywhere within the limits, which
is needed for interpolating the ﬁ rst few or last few samples in a table
with a ﬁ xed size.
  */
   8.11.3  Median Filter
 Choose:
   N  5 number of points in window 5 odd
 Calculate:
  Acquire samples in windows of  N  values, then sort and choose the median value as the
output.

Audio Filter Designs: FIR Filters  285
 The median ﬁ lter ( Figure 8.30 ) is a very interesting and somewhat strange algorithm. It has
no IR or frequency response. It smoothes an input signal, which is an LPF type of operation,
but it preserves transient edges, which is very un-LPF in nature. It has applications in noise
reduction without losing high-frequency transients. Its central algorithm uses a sorting
mechanism to sort the window of data by amplitude. The median value is chosen from the
sort operation as the output. When the next sample arrives, the window is re-sorted and the
next median value is obtained. To understand how it smoothes a signal without affecting
high-frequency transients, consider the following example.
 Example: Design a ﬁ ve-point median ﬁ lter and test with example.
 Consider this input sequence:  x  5 {1, 2, 1, 0, 1, 2, 3, 3, 2, 1, 9, 8, 9, 9, 7, 5, 5, 4} in
 Figure 8.31 .
x(n)
z -1
z -1
z -1
z -1
Sort  low to  high  and  find  median  value
y(n)
 Figure 8.30:    The block diagram of a ﬁ ve-point median ﬁ lter implementation.
9
8
7
6
5
4
3
2
1
0
0  1  1  1  2
0  1  1  2  2
0  1  1  2  3
0  1  2  3  3
Window 0 : 1
Window 1 : 1
Window 2 : 1
Window 3 : 2
Sample  (n)
 Figure 8.31:    The ﬁ rst four windows of the median ﬁ lter produce an output sequence {1,1,1,2}.
286  Chapter  8
 You can see a transient edge where the signal jumps from 1 to 9 and then another transient
where it drops from 9 to 7 to 5. The ﬁ rst window operates on the ﬁ rst ﬁ ve samples and
sorts them from low to high. Then, the median value is chosen as the output. The median
value is shown in a box in the center of each window. You can see the smoothing effect
immediately—the ﬁ rst three samples out of the ﬁ lter are all 1, even though the ﬁ rst three
samples vary from 1 to 2.  Figures 8.32  and  8.33  show the result of median ﬁ ltering the signal
in  Figure 8.31 .
 FIR ﬁ lters can be complicated to design and long convolutions in direct form are slow. You
can use the FIR design tools when you need to create linear-phase ﬁ lters with very steep
roll offs (optimal method) or ﬁ lters with arbitrary frequency responses (frequency sampling
method). You might also want to investigate other FIR designs such as the windowing method
and the recursive frequency sampling method.
9
8
7
6
5
4
3
2
1
0
0  1  1  1  2
0  1  1  2  2
1  8  9  9  9
7  8  9  9  9
Sample  (n)
0  1  1  2  3
0  1  2  3  3
1  2  2  3  3
5  7  8  9  9
5  5  7  9  9
3  5  5  7  9
1  2  2  3  3
1  2  3  3  9
1  2  3  8  9
1  2  8  9  9
 Figure 8.32:    The complete set of median ﬁ lter outputs for the sliding-window-of-ﬁ ve operation.
9
8
7
6
5
4
3
2
1
0
Audio Filter Designs: FIR Filters  287
Sample  (n)
 Figure 8.33:    Input and output sequences plotted together and shifted to show the smoothing of
the steady-state portions and preservation of the transient edges.
    Bibliography
Ifeachor, E. C. and Jervis, B. W. 1993. Digital Signal Processing, A Practical Approach, Chapters 4 and 6. Menlo
Park, CA: Addison-Wesley.
Kwakernaak, H. and Sivan, R. 1991. Modern Signals and Systems, Chapters 3 and 9. Englewood Cliffs, NJ:
Prentice-Hall.
Lindquist, C. 1999. Adaptive and Digital Signal Processing, Chapter 10. Miami: Steward & Sons.
Oppenheim, A. V. and Schafer, R. W. 1999. Discrete-Time Signal Processing, 2nd ed., Chapter 7. Englewood
Cliffs, NJ: Prentice-Hall.

CHAP TE R 9

Oscillators
   Oscillators ﬁ nd several uses in audio effects and plug-ins. The obvious use is as an audio test
signal like the one RackAFX provides on the main interface. Additive synthesis of musical
sounds uses multiple sinusoidal oscillators at harmonic frequencies to create complex
waveforms. Wavetable synthesis stores a periodic waveform in a table for interpolation
and playback when the musician strikes a key or a MIDI message is sent. Low-frequency
oscillators (LFOs) are used in the design of modulated delay lines and modulated ﬁ lters.
Oscillators broadly fall into two categories: direct calculation and table lookup. We desire
oscillators that have several important features:
•
•
•
•
•
   Stability over a wide range of frequencies
   No aliasing
   Purity of sinusoid (low THD+N) for sinusoidal oscillators
   Quadrature phase outputs
   Simplicity of calculation
  9.1    Direct Form Oscillator
 We can make a sinusoidal oscillator by placing a pair of poles directly on the unit circle in the
 z- plane. This produces a sinusoid at the pole angle (or frequency). The radius of the pole is
always 1.0. Using the fundamental digital signal processing (DSP)  z -plane equations, we can
directly write the transfer function and difference equations:
H(z) 5 a0 c
1
1 1 b1z21 1 b2z22
d
          5 a0 c
1
1 2 2Rcos(Q)z21 1 R2z22
d
   y(n) 5 2cos(Q)y(n 2 1) 2 y(n 2 2)
(9.1)
 Since the pole radius is 1.0, then the b 2  coefﬁ cient is 1.0 as well. The b 1  coefﬁ cient is then
 22cos(u), where u is the pole frequency from 0 to p.
289

290  Chapter 9
Im
1.0
θ
-θ
1.0
y(n)
Re
-1
(cid:61)
-1
(cid:61)
2cos( θ)
-b 1
–1.0
-b 2
 Figure 9.1:    The direct form sinusoidal oscillator  z -plane and block diagram.
 You can see that the block diagram in  Figure 9.1  has no input. Oscillators do not have inputs;
instead they have  initial conditions  which, once started, will cause eternal oscillation. DSP
theory suggests that kick-starting the system with an impulse will cause it to self-oscillate,
which it does. However, the amplitude and phase are dependent on the frequency, u. For an
oscillator, we would like to precisely control the amplitude and starting phase so that they are
exact. If we are trying to create a sinusoid with an initial phase offset of 0 degrees, then the
ﬁ rst sample out of our oscillator at  n  5 0 would be 0.0, and the second sample out would be
sin(v nT ) where  n  5 1 and  T  5 1/ f  s  .
  9.1.1 Initial Conditions
 Suppose we want to generate the above sinusoid, oscillating at 500 Hz. If we want the ﬁ rst
sample out to be 0.0, then continue rising after that, we need to initialize the delays as if a
sinusoid had been oscillating, up to the point when we turn on the oscillator. So we can set
our initial state by preloading those last two samples into the delays in our oscillator/ﬁ lter, as
shown in  Figure 9.2 .
 The sinusoid is oscillating at sin(v nT ) where  n  5 0, 1, 2, 3… after the oscillator starts and  T
is the sample period. The previous two samples with  f  o  5 500 Hz and  f  s  5 44,100 Hz would
be sin(v(21 T )) and sin(v(22 T )), which are –0.0712 and –0.1532, respectively. The direct
form oscillator block diagram is shown in  Figure 9.3 .
 The difference equation is as follows:
   y(n) 5 2b1y(n 2 1) 2 b2y(n 2 2)
(9.2)

Oscillators  291
IF the oscillator starts here
THEN these must have been
the previous two samples out of
the oscillator: y(n–1)  and y(n– 2)
 Figure 9.2:     Initial conditions that would have produced a sinusoid whose ﬁ rst output sample is 0.0.
y(n)
-1
z
-1
z
-b1
-b2
 Figure 9.3:     The direct form oscillator is really the feedback side of the bi-quad structure.
 Specify:
•
    f  o,  desired oscillation frequency
 The design equations are as follows:
u 5
2pfo
fs
b1 5 22cos(u)
b2 5 1.0
Initial conditions:
y(n 2 1) 5 sin(21u)
y(n 2 2) 5 sin(22u)
(9.3)

292  Chapter 9
    9.2    Design a Direct Form Oscillator Plug-In
 In our ﬁ rst version of the direct form oscillator we are going to make it as simple as possible
by restarting the oscillator when the user changes the oscillator frequency. This means we
are going to recalculate the initial conditions as if the oscillator was starting from a phase
of 0 degrees and the ﬁ rst sample out would have a value of 0.0. After we have that up and
running, we will modify it to change frequency on the ﬂ y, automatically back-calculating the
initial conditions for any given output sample. Here are the oscillator’s speciﬁ cations:
•
•
•
•
•
•
   Monophonic sinusoidal oscillator.
   We will need to implement a second-order feed-back block.
   We will need a slider for the user to control the oscillation frequency in Hz.
   We will need a cookFrequency() function to calculate our coefﬁ cients and reset our initial
conditions when the user moves the frequency slider.
   The oscillator’s range will be 200 Hz to 6 kHz.
   We need to assign Start and Stop buttons.
  9.2.1  Project: DirectOscillator
 Create a new project in RackAFX. On the new project dialog, make sure you check the box
“Output Only Synthesizer Plug-In”—you must select this for any synthesis plug-in; you will
not be able to play a wave ﬁ le through it ( Figure 9.4 ).
   9.2.2  DirectOscillator GUI
 Add a frequency slider ( Figure 9.5 ) to the user interface (UI) with the parameters in  Table 9.1 .
Next, set up the assignable buttons to create start and stop functions. Right-click inside the
box labeled “Assignable Buttons” but do not click on a button, only in the area in between.
A dialog pops up ( Figure 9.6 ) that lets you set the button names. Choose “Start,” “Stop,” and
make the last one blank with “ ” by entering the strings in the edit boxes. The buttons will
automatically use the text you enter. Do not check the “Latching” buttons. See the website for
notes on how to use latching buttons.
  Table 9.1:    The GUI control for the DirectOscillator plug-in
Slider Property
Control Name
Units
Variable Type
Variable Name
Low Limit
High Limit
Initial Value
Value
Frequency
Hz
ﬂ oat
m_fFrequency_Hz
200
6000
1000
Oscillators  293
 Figure 9.4:     Make sure you check the “Output Only Synthesizer Plug-In”
box for your oscillators.
 Figure 9.5:     The DirectOscillator GUI.
 These assignable buttons will trigger your userInterfaceChange() function with their
nControlIndex values of 50, 51, and 52. See the userInterfaceChange() function for more
details.
294  Chapter 9
 Figure 9.6:     The assignable button setup; notice that the last button is blank.
RackAFX will hide it when you load the plug-in.
   9.2.3  DirectOscillator.h File
 In the .h ﬁ le, declare the variables you need to implement the oscillator:
  // Add your code here: --------------------------------------------------------- //
  //
  // coefﬁ cients, 2nd Order FB
   ﬂ oat m_f_b1;
   ﬂ oat m_f_b2;
  // delay elements, 2nd Order FB
   ﬂ oat m_f_y_z1;
   ﬂ oat m_f_y_z2;
  // ﬂ ag to start/stop oscillator
   bool m_bRunOscillator;
  // function to cook the Frequency and set initial conditions
   void cookFrequency();
  //
  // END OF USER CODE ------------------------------------------------------------ //

9.2.4  DirectOscillator.cpp File

Constructor
•
   Turn off the oscillator by setting the ﬂ ag you declared in the .h ﬁ le. You will need to do
these two steps any time you make an oscillator or other output-only plug-in.
Oscillators  295
  CDirectOscillator::CDirectOscillator()
  {
  <SNIP SNIP SNIP>
  // Finish initializations here
  // turn off
   m_bRunOscillator = false;
  // set 0
   m_f_b1 = 0;
   m_f_b2 = 0;
  // ﬂ ush memory
   m_f_y_z1 = 0;
   m_f_y_z2 = 0;
  // call the cooking function to calc coeffs
  // and set initial states
   cookFrequency();
  }
 Implement the cooking function according to the design equations:
  void CDirectOscillator::cookFrequency()
  {
  // Oscillation Rate = theta = wT = w/fs
   ﬂ oat f_wT = (2.0*pi*m_fFrequency_Hz)/(ﬂ oat)m_nSampleRate;
  // coefﬁ cients according to design equations
   m_f_b1 = –2.0*cos(f_wT);
   m_f_b2 = 1.0;
  // set initial conditions so that ﬁ rst sample out is 0.0
   m_f_y_z1 = sin(–1.0*f_wT);  // sin(wnT) = sin(w(–1)T)
   m_f_y_z2 = sin(–2.0*f_wT);  // sin(wnT) = sin(w(–2)T)
  }
   prepareForPlay()
•
   Call the cooking functions to initialize the oscillator:
  bool __stdcall CDirectOscillator::prepareForPlay()
  {
  // Add your code here:
