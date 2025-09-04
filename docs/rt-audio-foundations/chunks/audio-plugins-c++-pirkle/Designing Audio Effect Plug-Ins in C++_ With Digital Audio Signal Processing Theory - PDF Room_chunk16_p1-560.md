# Audio Plugins C++ (Pirkle) — Chunk 16/73

> Source: `Designing Audio Effect Plug-Ins in C++_ With Digital Audio Signal Processing Theory - PDF Room.pdf` · Pages ~1-560 · Extractor: pdfminer.six
> Target ≈ 2800 words, Overlap = 200 words · Actual = 2617

> Note: DSP textbook content for SpectralCanvasPro RT-audio knowledge base

---
88  Chapter 4
 The topology of the filter determines its difference equation. The coefficients (a N ) of a filter
determine its filter frequency and phase response and therefore its type (HPF, LPF, etc.) and
its sonic qualities. Your plug-in implements the difference equation in processAudioFrame().
Your plug-in calculates the coefficients in userInterfaceChange() in response to the user making
changes to the control surface.
(cid:129)
(cid:129)
(cid:129)
   Inverted settings give the identical frequency response but the phase responses will be
inverted (use the Phase button and have a look for yourself).
   You can get gain out of the ﬁ lter.
   You can also get attenuation out of the ﬁ lter.
   4.3    First-Order Feed-Back Filter
 A ﬁ rst-order feed-back ﬁ lter is shown in  Figure 4.22 .
 The difference equation is as follows:
  y(n) 5 a0 x(n) 2 b1y(n 2 1)
(4.2)
 You can see the feed-back nature of the ﬁ lter; the output  y ( n ) is fed back into the summer
through a one-sample-delay  z  21  element. Notice that the feedback coefﬁ cient has a negative
sign in front of it and the difference equation reﬂ ects this with the 2b 1  term. The negative
sign is for mathematical convenience and will make more sense in the next chapter when we
analyze the difference equations in more detail. Also notice that there is no b 0  coefﬁ cient—
there will not be a b 0  coefﬁ cient in any of the feed-back ﬁ lters. To analyze this without math
requires going through the same procedure as before, analyzing the amplitude and phase of
the basic test signals. Another option would be to code it as a plug-in in RackAFX and map
the coefﬁ cients directly to the sliders as you did in the previous example. Then, you can
experiment with the coefﬁ cients and see how they affect the ﬁ lter’s frequency, phase,
DC/step, and impulse responses.
a0x(n)
x (n)
a0
(cid:153)
y(n)
z -1
–b1y(n –1)
–b1
y(n – 1)
 Figure 4.22:    First-order feed-back ﬁ lter block diagram.

How DSP Filters Work  89
   4.4    Design a General First-Order Feed-Back Filter
  4.4.1  Project FeedBackFilter
 Create a new RackAFX project called “FeedBackFilter.” The plan is for this ﬁ lter to let you
directly control the coefﬁ cients with UI sliders and use the analyzer to check the resulting ﬁ lters.
   4.4.2  FeedBackFilter GUI
 The GUI will consist of two sliders, one for the a 0  coefﬁ cient and the other for the b 1
coefﬁ cient. You can use any two sliders you like, but make sure you keep track of the control
index values later on.  Figure 4.23  shows my version of the GUI. Right-click near the sliders
you want to use and set them up according to  Table 4.6 .
   4.4.3  FeedBackFilter.h File
 Add the  z  21  elements and a 0 , b 1  coefﬁ cient variables for the right and left channels in your .h ﬁ le:
  // Add your code here: ------------------------------------------------------- //
   ﬂ oat m_f_a0_left;
   ﬂ oat m_f_b1_left;
ao
I  1.00
b1
I  I  0.00
I
l  f
 Figure 4.23     The GUI for the FeedBackFilter project.
 Table 4.6 :    The a0 slider properties.
Slider Property
Control Name
Units
Variable Type
Variable Name
Low Limit
High Limit
Initial Value
Value
b1
ﬂ oat
m_fSlider_b1
21.0
11.0
0.0
Slider Property
Control Name
Units
Variable Type
Variable Name
Low Limit
High Limit
Initial Value
Value
a0
ﬂ oat
m_fSlider_a0
21.0
11.0
1.0

90  Chapter 4
   ﬂ oat m_f_a0_right;
   ﬂ oat m_f_b1_right;
   ﬂ oat m_f_z1_left;
   ﬂ oat m_f_z1_right;
  // END OF USER CODE ---------------------------------------------------------- //
   4.4.4  FeedBackFilter.cpp File
  Constructor
(cid:129)
(cid:129)
   Initialize the internal  a 0  and b 1  variables to match our GUI variables.
   Zero out the delay line elements.
  CFeedBackFilter::CFeedBackFilter()
  {
    // Added by RackAFX - DO NOT REMOVE
    //
    // Setup the RackAFX UI Control List and Initialize Variables
    initUI();
    // END InitUI
    <SNIP SNIP SNIP>
    // Finish initializations here
    //
    // setup our coefﬁ cients
     m_f_a0_left = m_fSlider_a0;
     m_f_b1_left = m_fSlider_b1;
     m_f_a0_right = m_fSlider_a0;
     m_f_b1_right = m_fSlider_b1;
    // ﬂ ush the memory registers
     m_f_z1_left = 0.0;
     m_f_z1_right = 0.0;
  }
   prepareForPlay()
(cid:129)
   Flush the  z   -1  storage registers.
  bool __stdcall CFeedBackFilter::prepareForPlay()
  {
    // Add your code here:
     m_f_z1_left = 0.0;
How DSP Filters Work  91
     m_f_z1_right = 0.0;
    return true;
  }
   processAudioFrame()
(cid:129)
   Implement the difference equation in processAudioFrame(); notice the (2) sign in the
difference equation too.
   bool __stdcall CFeedBackFilter::processAudioFrame(ﬂ oat* pInputBuffer, ﬂ oat*
pOutputBuffer, UINT uNumInputChannels, UINT uNumOutputChannels)
  {
    // Do LEFT (MONO) Channel; there is always at least one input/one output
    // (INSERT effect)
    // Input sample is x(n)
     ﬂ oat xn = pInputBuffer[0];
    // Delay sample is y(n-1)
     ﬂ oat yn_1 = m_f_z1_left;
    // Difference Equation
     ﬂ oat yn = m_f_a0_left*xn - m_f_b1_left*yn_1;
    // Populate Delay with current y(n)
     m_f_z1_left = yn;
    // Output sample is y(n)
     pOutputBuffer[0] = yn;
    // Mono-In, Stereo-Out (AUX effect) -- COPY for now
    if(uNumInputChannels == 1 && uNumOutputChannels == 2)
       pOutputBuffer[1] = yn;
    // Stereo-In, Stereo-Out (INSERT effect)
    if(uNumInputChannels == 2 && uNumOutputChannels == 2)
    {
      // Input sample is x(n)
       ﬂ oat xn = pInputBuffer[1];
      // Delay sample is x(n-1)
       ﬂ oat yn_1 = m_f_z1_right;

92  Chapter 4
      // Difference Equation
       ﬂ oat yn = m_f_a0_right*xn - m_f_b1_right*yn_1;
      // Populate Delay with current y(n)
       m_f_z1_right = yn;
      // Output sample is y(n)
       pOutputBuffer[1] = yn;
    }
    return true;
  }
   userInterfaceChange()
(cid:129)
(cid:129)
   Update the coefﬁ cients when the user changes a slider.
   Check your nControlIndex values to make sure you are processing the correct control.
  bool CFeedBackFilter::userInterfaceChange(int nControlIndex)
  {
    // add your code here
    switch(nControlIndex)
    {
     case 0:
       // map the a0 Slider
       m_f_a0_left = m_fSlider_a0;
       m_f_a0_right = m_fSlider_a0;
       break;
     case 1:
       // map the b1 Slider
       m_f_b1_left = m_fSlider_b1;
       m_f_b1_right = m_fSlider_b1;
       break;
    default:
      ; // do nothing
    }
    return true;
  }
 Build and load the plug-in into RackAFX. Open the analyzer and use the Frequency, Phase,
Impulse, and Step buttons to analyze the output. Play with the controls. Notice ﬁ rst that the a 0
slider only controls the gain and phase of the signal; in the frequency response this just moves
the curve up or down and it disappears when a 0  5 0.0, which makes sense. The real action is
with the b 1  control; try the examples in  Figures 4.24  through  4.26 .

How DSP Filters Work  93
10Hz
100Hz
1kHz
10kHz
+12.0dB
+8.0dB
+4.0dB
0.0dB
–4.0dB
–8.0dB
–12.0dB
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
 Figure 4.24:    a 0  5 1.0 and b 1  5 0.5; the frequency response is high-pass/low-shelf in nature
and has gain above 11 kHz, while the impulse response shows slight ringing.
+24.0dB
+18.0dB
+12.0dB
+6.0dB
0.0dB
–6.0dB
–12.0dB
1.000
0.707
0.500
0.000
–0.500
–0.707
–1.000
10Hz
100Hz
1kHz
10kHz
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
 Figure 4.25:    a 0  5 1.0 and b 1  5 0.9; the frequency response has a steeper
high-pass response than before and has gain of 120 dB at Nyquist,
while the impulse response shows considerable ringing.
94  Chapter 4
+12.0dB
+8.0dB
+4.0dB
0.0dB
–4.0dB
–8.0dB
–12.0dB
1.000
0.707
0.500
0.000
–0.500
–0.707
–1.000
10Hz
100Hz
1kHz
10kHz
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
 Figure 4.26:    a 0  5 1.0 and b 1  5 1.0; the frequency response has
blown up while the impulse response rings forever.
 What happened in that last ﬁ lter? Why was there no frequency response? The ﬁ lter became
unstable and  blew up . It blew up because the b 1  coefﬁ cient was 1.0, which introduced 100%
positive feedback into the loop. The output recirculated through the feedback loop forever
causing the inﬁ nite ringing seen in the step and impulse responses. Also notice that as the b 1
variable was increased, the gain at Nyquist also increased. With b 1  5 1.0, the gain at Nyquist
is actually inﬁ nite.
     4.5    Observations
 In doing these exercises, you have made a lot of progress—you know how to implement both
feed-forward and feed-back ﬁ lters in a plug-in. You also have a good intuitive knowledge
about how the coefﬁ cients control the ﬁ lter type. Plus you got to blow up a ﬁ lter, so that is
pretty cool. Here are some observations.
  4.5.1  General
(cid:129)
(cid:129)
(cid:129)
   Feed-forward and feed-back topologies can both make high-pass or low-pass-shaped
ﬁ lter responses.
   The coefﬁ cients of the ﬁ lter ultimately determine the kind of ﬁ lter and what it sounds like.
   The phase shift introduced by the delay element(s) is responsible for the ﬁ ltering operation.
How DSP Filters Work  95
   4.5.2  Feed-Forward Filters
(cid:129)
(cid:129)
(cid:129)
(cid:129)
   Operate by making some frequencies go to zero; in the case of a  0  5 1.0 and a 1  5 1.0, the
Nyquist frequency went to zero; this is called a  zero of transmission  or a  zero frequency
or just a  zero .
   The step and impulse responses show smearing. The amount of smearing is exactly equal
to the total amount of delay in the feed-forward branches.
   Don’t blow up.
   Are called  ﬁ nite impulse response  (FIR) ﬁ lters because their impulse responses, though
they may be smeared, are always ﬁ nite in length.
   4.5.3  Feed-Back Filters
(cid:129)
(cid:129)
(cid:129)
(cid:129)
   Operate by making some frequencies go to inﬁ nity; in the case of a  0  5 1.0 and b 1  5 1.0,
the Nyquist frequency went to inﬁ nity and with a 0  5 1.0 and b 1  5 21.0, DC or 0 Hz
went to inﬁ nity; this is called a  pole of transmission  or a  pole frequency  or just a  pole .
   The step and impulse responses show overshoot and ringing or smearing depending on
the coefﬁ cients. The amount of ringing or smearing is proportional to the amount of
feedback.
   Can blow up (or go unstable) under some conditions.
   Are called  inﬁ nite impulse response  (IIR) ﬁ lters because their impulse responses can
become inﬁ nite.
 The problem now is that we want to be able to specify the ﬁ lter in a way that makes sense
in audio—a low-pass ﬁ lter with a cut-off of 100 Hz or a band-pass ﬁ lter with a Q of 10 and
center frequency of 1.2 kHz. What we need is a better way to analyze the ﬁ ltering than just
randomly trying coefﬁ cients, and we need methods to come up with ﬁ lter coefﬁ cients based
on how we specify the ﬁ lter. In the next chapter, we’ll work on the basic DSP theory that will
make this happen.
      Bibliography
 Dodge, C. and Jerse, T. 1997.  Computer Music Synthesis, Composition and Performance , Chapters 3 and 6.
New York: Schirmer.
 Steiglitz, K. 1996.  A DSP Primer with Applications to Digital Audio and Computer Music , Chapter 4. Menlo Park,
CA: Addison-Wesley.

CHAP TE R 5

Basic DSP Theory
    You want to get a grip on the underlying digital signal processing (DSP) theory of
ﬁ lters for several reasons. It helps to understand the anatomy of the ﬁ lter because you
have to implement it in code; a deeper understanding of the theory can only help your
coding strategy. Also, the same DSP ﬁ lter analysis and mathematical models can be
applied to other  effects including delay, chorusing, reverb, and compression. In order to
intuitively understand the foundation of DSP theory, you need to review some math and
engineering concepts.
  5.1    The Complex Sinusoid
 The analysis and design of digital ﬁ lters uses the sinusoid as its basic stimulus function.
Since Fourier showed that a signal can be decomposed into sinusoids, if you know how the
system reacts to a sinusoidal stimulus at a bunch of different frequencies, you can plot the
frequency and phase responses like you did by hand in the last chapter. This is akin to the
impulse response—since the input signal is a train of impulses of varying amplitudes, if you
know how the ﬁ lter responds to a single impulse, you can ﬁ gure out how it will respond to
multiple impulses. You also did this by hand when you took the impulse response of the
low-pass ﬁ lter.
 Everyone is familiar with the sine and cosine functions—sine and cosine are related by
an offset of 90 degrees and the sine function starts at zero, whereas cosine starts at 1.0. In
 Figure 5.1  you can identify the sine and cosine waveforms by their starting position. But what
about the sine-like waveform that starts at an arbitrary time in the lower plot? Is it a sine that
has been phase shifted backwards or a cosine that has been phase shifted forward? You have
to be careful how you answer because sine and cosine have different mathematical properties;
their derivatives are not the same and it usually becomes difﬁ cult when you try to multiply
them or combine them in complex ways. Add a phase offset to the argument of the sin() or
cos() function and then it really turns into a mess when you do algebra and calculus with
them. What you need is a function that behaves in a sinusoidal manner, encapsulates both sine
and cosine functions, and is easy to deal with mathematically. Such a function exists, and it’s
called the complex sinusoid:
Complex sinusoid  5  e
jvt
(5.1)
97

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
