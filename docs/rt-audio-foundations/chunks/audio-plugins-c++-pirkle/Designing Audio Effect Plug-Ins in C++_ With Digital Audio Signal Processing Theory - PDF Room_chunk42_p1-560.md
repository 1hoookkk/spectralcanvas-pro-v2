# Audio Plugins C++ (Pirkle) — Chunk 42/73

> Source: `Designing Audio Effect Plug-Ins in C++_ With Digital Audio Signal Processing Theory - PDF Room.pdf` · Pages ~1-560 · Extractor: pdfminer.six
> Target ≈ 2800 words, Overlap = 200 words · Actual = 2509

> Note: DSP textbook content for SpectralCanvasPro RT-audio knowledge base

---
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

296  Chapter 9
  // calc coeffs and initial conditions
   cookFrequency();
  retur true;
  }
   processAudioFrame()
•
•
   Implement the second-order feed-back ﬁ lter.
   Check the oscillator ﬂ ag. If the oscillator is not running you  must  send 0.0 samples to
the output stream and then return without further processing. This is done at the very
 beginning of the function.
  bool __stdcall CDirectOscillator::processAudioFrame(ﬂ oat* pInputBuffer, ﬂ oat*
  pOutputBuffer, UINT  uNumChannels)
  {
  //
  // output = input -- change this for meaningful processing
  //
  // Do LEFT (MONO) Channel
  //
  // if not running, write 0s and bail
   if(!m_bRunOscillator)
   {
  // zero it all out
   pOutputBuffer[0] = 0.0;
  // Mono-In, Stereo-Out (AUX Effect)
  if(uNumInputChannels == 1 && uNumOutputChannels == 2)
   pOutputBuffer[1] = 0.0;
  // Stereo-In, Stereo-Out (INSERT Effect)
  if(uNumInputChannels == 2 && uNumOutputChannels == 2)
   pOutputBuffer[1] = 0.0;
   return true;
   }
  // otherwise, do the oscillator
  // do difference equation y(n) = –b1y(n-2) – b2y(n–2)
   pOutputBuffer[0] = –m_f_b1*m_f_y_z1 – m_f_b2*m_f_y_z2;
  // Mono-In, Stereo-Out (AUX Effect)
  if(uNumInputChannels == 1 && uNumOutputChannels == 2)
   pOutputBuffer[1] = pOutputBuffer[0];
  // Stereo-In, Stereo-Out (INSERT Effect)
  if(uNumInputChannels == 2 && uNumOutputChannels == 2)
   pOutputBuffer[1] = pOutputBuffer[0];

Oscillators  297
  // shufﬂ e memory
   m_f_y_z2 = m_f_y_z1;
   m_f_y_z1 = pOutputBuffer[0];
   return true;
  }
   userInterfaceChange()
•
•
   Call the frequency cooking function and implement the start and stop functions.
   Make sure your nControlIndex values match your switch/case statement.
  bool __stdcall CDirectOscillator::userInterfaceChange(int nControlIndex)
  {
  // cook variables
   switch(nControlIndex)
   {
   case 0: // freq
   cookFrequency();
   break;
   case 50: // start
   m_bRunOscillator = true;
   break;
   case 51: // stop
   m_bRunOscillator = false;
   break;
   default:
   break;
   }
  return true;
  }
 Build and load the dynamic link library (DLL). Then, hit the Start and Stop buttons to control
your oscillator. Play with the frequency slider to change the value. You will notice a lot of
clicking due to the discontinuities that occur each time the oscillator is reset to re-trigger from
the 0.0 value. Other than that problem, the output should be a pure sinusoid with very low
distortion and noise.
    9.2.5 Improving the Oscillator Design
 One of the drawbacks of oscillators is their initial conditions usually must be met (there are
noise generators and other devices that can start up in a random state). In this case, we must
preload the delay elements with samples that would have occurred before the initial output
sample. If the user changes the frequency of oscillation, the initial states must change too. The
problem is that the oscillator can be in any state when the user changes the controls. Suppose
the user decreased the frequency of oscillation a bit and that the oscillator had some values in
298  Chapter 9
y(n –1) shared
Next y(n)
y(n(cid:3)–(cid:3)2)
newly
calculated
 Figure 9.7:     The new initial state samples,  y ( n  2 1) and  y ( n  2 2); we share the  y ( n  2 1)
sample as if it was from the new frequency and alter the  y ( n  2 2) value to
relocate it on the new sinusoid.
 y ( n  2 1) and  y ( n  2 2). Then to set up the new initial state, you can share the  y ( n  2 1) value
between the two sinusoids and calculate a new  y ( n  2 2) value. The next sample out of the
oscillator is at the new rate, using the new initial state you created ( Figure 9.7 ).
 The initial conditions from the design equations are in  Equation 9.4 :
Initial conditions:
y(n 2 1)  5  sin(vnT )
y(n 2 2)  5  sin(v(n 2 1)T )
(9.4)
 Our problem is that we do know the new frequency, v, and since the sample rate is also
known, then we also know v T , but we don’t know what sample interval  n  we are on.
However, we can ﬁ gure it out as follows:
•
•
•
•
•
   Take the inverse sin of the  y ( n  2 1) delay element.
   Find the value of  n  by dividing it by v T .
   arcsine() returns a value between –p/2 to +p/2 which is the rising edge of the sinusoid
like the original initial conditions.
   If we are on the rising edge, calculate the new  y ( n  2 2) value as sin(( n  – 1)v T ).
   If we are on the falling edge, calculate the new  y ( n  2 2) value as sin(( n  + 1)v T ).
 It is simple to change in the cookFrequency() function as follows, so make the change in your
code. Note the use of commenting out the original initial condition code in case you want to
refer to it later. Also note the logic to see if we are rising or falling. We need this because the
arcsine() function only returns the angle for half a sinusoid. If you think about it, this makes
sense. If you give it a value of 0.707, it can’t tell if the angle is 45 degrees or 135 degrees,
both of which produce a sine of 0.707.
  void CDirectOscillator::cookFrequency()
  {
  // Oscillation Rate = theta = wT = w/fs
  ﬂ oat f_wT = (2.0*pi*m_fFrequency_Hz)/(ﬂ oat)m_nSampleRate;

Oscillators  299
  // coefﬁ cients according to design equations
  m_f_b1 = –2.0*cos(f_wT);
  m_f_b2 = 1.0;
  // set initial conditions so that ﬁ rst sample out is 0.0
  //      m_f_y_z1 = sin(–1.0*f_wT);  // sin(wnT) = sin(w(–1)T)
  //      m_f_y_z2 = sin(–2.0*f_wT);  // sin(wnT) = sin(w(–2)T)
  // re calculate the new initial conditions
  // arcsine of y(n–1) gives us wnT
   double wnT1 = asin(m_f_y_z1);
  // ﬁ nd n by dividing wnT by wT
   ﬂ oat n = wnT1/f_wT;
  // re calculate the new initial conditions
  // asin returns values from –pi/2 to +pi/2 where the sinusoid
  // moves from –1 to +1 -- the leading (rising) edge of the
  // sinewave. If we are on that leading edge (increasing)
  // then we use the value 1T behind.
  //
  // If we are on the falling edge, we use the value 1T ahead
  // because it mimics the value that would be 1T behind
   if(m_f_y_z1 > m_f_y_z2)
   else
   n–=1;
   n+=1;
  // calculate the new (old) sample
   m_f_y_z2 = sin((n)*f_wT);
  }
 Build and load the DLL. Then, hit the Start and Stop buttons to control your oscillator. Play
with the frequency slider to change the value. If you change the slider too much you will still
hear a click on some of the discontinuities due to the fact that the frequencies are so far apart
that it produces a noticeable shift in the waveform.
 Now that you understand how to code a direct calculation oscillator in RackAFX, next are a
few more example oscillator designs for you to try out. Remember that you must set the ﬂ ag
m_bOutputOnlyPlugIn to make the oscillators work properly in RackAFX. You must provide
a start/stop mechanism as well. Sample code is included to help you along.
    9.3    The Gordon–Smith Oscillator
 The Gordon–Smith oscillator uses a pair of delay elements arranged in a circular conﬁ guration
to produce both sin and cos oscillations. The two outputs are almost perfectly in quadrature
phase (90 degrees apart) only differing by half a sample period. Its sinusoidal oscillation
is pure enough to be used in audio test equipment. And, because each output only uses one

300  Chapter 9
delay element, there are no initial states to update when the frequency is changed; the single
 y ( n  – 1) sample is shared with the new frequency the same way as in the direct form oscillator.
Only the coefﬁ cient e needs to be updated. A small amplitude variation is observed when the
oscillation frequency changes, but it is small enough to not cause clicks or pops in the output.
It sounds just as smooth as the direct form oscillator when the frequency is adjusted slowly.
 The two outputs are labeled  y ( n ) and  y  q ( n ) where the “q” stands for quadrature. Therefore, there
are two difference equations. The difference equation for  y  q ( n ) must be solved ﬁ rst because  y ( n )
is dependent on it. A Gordon–Smith oscillator block diagram is shown in  Figure 9.8 .
 The difference equations are as follows:
yq(n) 5 yq(n 2 1) 2 Py(n 2 1)
y(n) 5 Pyq 1 y(n 2 1)
 Specify:
•
    f  o , desired oscillation frequency
 The design equations are as follows:
u 5
2pfo
fs
P 5 2sin(u/2)
Initial conditions:
y(n 2 1) 5 sin(21u)
yq(n 2 1) 5 cos (21u)
–1
Z
yq(n(cid:3)(cid:177)(cid:3)1)
(cid:177)1
Z
y(n(cid:3)–1)
 Figure 9.8:    The Gordon–Smith Oscillator.
(9.5)
(9.6)
yq (n)
y(n)

 The C++ code for the Gordon–Smith oscillator looks as follows (two memory elements have
been declared, m_f_yn_z and m_f_yq_z, as well as a coefﬁ cient m_fGorSmithEpsilon). In
cookFrequency():
Oscillators  301
  // calculate HS Epsilon
   ﬂ oat f_wT = (2.0*pi*m_fFrequency_Hz)/(ﬂ oat)m_nSampleRate;
   m_fGorSmithEpsilon = 2.0*sin(f_wT/2.0);
 In processAudioFrame():
    // form yq(n) ﬁ rst
     ﬂ oat f_yqn = m_f_yq_z - m_fGorSmithEpsilon*m_f_yn_z;
    // y(n)
     ﬂ oat f_yn = m_fGorSmithEpsilon*f_yqn + m_f_yn_z;
    // shufﬂ e delays
     m_f_yq_z = f_yqn;
     m_f_yn_z = f_yn;
    // write out
     pOutputBuffer[0] = f_yn;
     etc…
   9.4    Wave Table Oscillators
 A wave table oscillator is, as its name implies, a table-based system for creating periodic
signals. A circular buffer is preloaded with one cycle, minus one sample, of a waveform.
The waveform may be mathematical (sin, cos, pulse, square, saw, triangle, etc.) or it might
be arbitrary or a recorded signal. The basic idea is that you read through the table and output
samples from it. Consider a table of 1024 samples consisting of one cycle of a sinusoid,
minus one sample ( Figure 9.9 ).
 Suppose you start at  i  5 0 and during each sample period, you read out one value and
advance to the next. At the end of the buffer, you wrap around and start all over. If you
did read out one value per sample period, what would be the resulting frequency of the
waveform?
 The answer is  f  table  5  f  s / L  when the index increment is exactly 1.0 through the table. For a
1024-point wave table at a 44,100 Hz sample rate, the table frequency is 43.06 Hz. If you
happen to really need a super precise sinusoid at exactly 43.06 Hz, then this method will
produce nearly perfect results. The only factor is the precision of the sinusoid loaded into the
table. If you had a saw-tooth waveform stored in the table, it too would have a fundamental
frequency of 43.06 Hz.
 Most likely, you are going to want to make any arbitrary frequency you like, perhaps with an
emphasis on musical pitch frequencies. With the exception of the note A, these are going to be
