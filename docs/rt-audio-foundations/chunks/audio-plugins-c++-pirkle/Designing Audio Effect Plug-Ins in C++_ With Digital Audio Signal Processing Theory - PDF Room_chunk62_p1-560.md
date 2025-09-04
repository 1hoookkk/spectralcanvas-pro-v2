# Audio Plugins C++ (Pirkle) — Chunk 62/73

> Source: `Designing Audio Effect Plug-Ins in C++_ With Digital Audio Signal Processing Theory - PDF Room.pdf` · Pages ~1-560 · Extractor: pdfminer.six
> Target ≈ 2800 words, Overlap = 200 words · Actual = 2391

> Note: DSP textbook content for SpectralCanvasPro RT-audio knowledge base

---
// function to calculate the new fc from the Envelope Value
    ﬂ oat calculateAPFCutoffFreq(ﬂ oat fLFOSample, ﬂ oat fMinFreq, ﬂ oat fMaxFreq);
Modulated Filter Effects  447
    <SNIP SNIP SNIP>
    ﬂ oat m_fMinAPF_3_Freq;
    ﬂ oat m_fMaxAPF_3_Freq;
     ﬂ oat m_fMinAPF_4_Freq;
     ﬂ oat m_fMaxAPF_4_Freq;
     ﬂ oat m_fMinAPF_5_Freq;
     ﬂ oat m_fMaxAPF_5_Freq;
     ﬂ oat m_fMinAPF_6_Freq;
     ﬂ oat m_fMaxAPF_6_Freq;
    // LFO Stuff
    CWaveTable m_LFO;
    // Feedback Storage
     ﬂ oat m_fFeedbackLeft;
     ﬂ oat m_fFeedbackRight;
    // END OF USER CODE --------------------------------------------------------------
   12.7.3  Phaser.cpp File
  Constructor
•
   Initialize the min and max APF values as per the table.
  CPhaser::CPhaser()
  {
<SNIP SNIP SNIP>
m_fMinAPF_3_Freq = 48.0;
m_fMaxAPF_3_Freq = 4800.0;
m_fMinAPF_4_Freq = 98.0;
m_fMaxAPF_4_Freq = 9800.0;
m_fMinAPF_5_Freq = 160.0;
m_fMaxAPF_5_Freq = 16000.0;
m_fMinAPF_6_Freq = 220.0;
m_fMaxAPF_6_Freq = 22000.0;
m_fFeedbackLeft = 0.0;
m_fFeedbackRight = 0.0;
  }

448  Chapter 12
 Modify the cooking functions to calculate the new APF coefﬁ cients:
  void CPhaser::calculateFirstOrderLeftAPFCoeffs(ﬂ oat fLFOSample)
  {
<SNIP SNIP SNIP>
// APF3 (cid:2) fc -> Bi Quad
fCutoffFreq = calculateAPFCutoffFreq(fLFOSample, m_fMinAPF_3_Freq,
calculateFirstOrderAPFCoeffs(fCutoffFreq, &m_LeftAPF_3);
m_fMaxAPF_3_Freq);
// APF4 (cid:2) fc -> Bi Quad
fCutoffFreq = calculateAPFCutoffFreq(fLFOSample, m_fMinAPF_4_Freq,
calculateFirstOrderAPFCoeffs(fCutoffFreq, &m_LeftAPF_4);
m_fMaxAPF_4_Freq);
// APF5 (cid:2) fc -> Bi Quad
fCutoffFreq = calculateAPFCutoffFreq(fLFOSample, m_fMinAPF_5_Freq,
calculateFirstOrderAPFCoeffs(fCutoffFreq, &m_LeftAPF_5);
m_fMaxAPF_5_Freq);
// APF6 - fc -> Bi Quad
fCutoffFreq = calculateAPFCutoffFreq(fLFOSample, m_fMinAPF_6_Freq,
calculateFirstOrderAPFCoeffs(fCutoffFreq, &m_LeftAPF_6);
m_fMaxAPF_6_Freq);
  }
  void CPhaser::calculateFirstOrderRightAPFCoeffs(ﬂ oat fLFOSample)
  {
<SNIP SNIP SNIP>
// APF3 - fc -> Bi Quad
fCutoffFreq = calculateAPFCutoffFreq(fLFOSample, m_fMinAPF_3_Freq,
calculateFirstOrderAPFCoeffs(fCutoffFreq, &m_RightAPF_3);
  m_fMaxAPF_3_Freq);
// APF4 - fc -> Bi Quad
fCutoffFreq = calculateAPFCutoffFreq(fLFOSample, m_fMinAPF_4_Freq,
calculateFirstOrderAPFCoeffs(fCutoffFreq, &m_RightAPF_4);
   m_fMaxAPF_4_Freq);
// APF5 - fc -> Bi Quad
fCutoffFreq = calculateAPFCutoffFreq(fLFOSample, m_fMinAPF_5_Freq,
calculateFirstOrderAPFCoeffs(fCutoffFreq, &m_RightAPF_5);
   m_fMaxAPF_5_Freq);
// APF6 - fc -> Bi Quad
fCutoffFreq = calculateAPFCutoffFreq(fLFOSample, m_fMinAPF_6_Freq,
calculateFirstOrderAPFCoeffs(fCutoffFreq, &m_RightAPF_6);
   m_fMaxAPF_6_Freq);
  }

Modulated Filter Effects  449
   prepareForPlay()
•
•
   Flush the new APF buffers.
   Reset the feedback variables.
  bool __stdcall CPhaser::prepareForPlay()
  {
// Add your code here:
<SNIP SNIP SNIP>
m_LeftAPF_3.ﬂ ushDelays();
m_RightAPF_3.ﬂ ushDelays();
m_LeftAPF_4.ﬂ ushDelays();
m_RightAPF_4.ﬂ ushDelays();
m_LeftAPF_5.ﬂ ushDelays();
m_RightAPF_5.ﬂ ushDelays();
m_LeftAPF_6.ﬂ ushDelays();
m_RightAPF_6.ﬂ ushDelays();
m_LFO.m_fFrequency_Hz = m_fModRate;
m_LFO.m_uPolarity = 1; // 0 = bipolar, 1 = unipolar
m_LFO.m_uTableMode = 0; // normal, no band limiting
m_LFO.m_uOscType = m_uLFO_Waveform;
m_LFO.setSampleRate(m_nSampleRate); // really important!
// the LFO prepareForPlay() calls reset() and cookFrequency()
m_LFO.prepareForPlay();
m_fFeedbackLeft = 0.0;
m_fFeedbackRight = 0.0;
return true;
  }
   processAudioFrame()
•
•
•
   Add more cascaded APFs to each channel.
   Store and use the feedback values.
   Check for quad-phase LFO and use the quad LFO output on right channel if needed.
  bool __stdcall CPhaser::processAudioFrame(ﬂ oat* pInputBuffer, ﬂ oat* pOutputBuffer, UINT
uNumInputChannels, UINT uNumOutputChannels)
  {
//
// output = input -- change this for meaningful processing
//
// Do LEFT (MONO) Channel; there is always at least one input, one output

450  Chapter 12
ﬂ oat fYn = 0; // normal output
ﬂ oat fYqn = 0; // quad phase output
// mod depth is at 100% when the control is at 50% !!
ﬂ oat fDepth = m_fMod_Depth/200.0;
ﬂ oat fFeedback = m_fFeedback/100.0;
// call the LFO function; we only need ﬁ rst output
m_LFO.doOscillate(&fYn, &fYqn);
// use the LFO to calculate all APF banks
calculateFirstOrderLeftAPFCoeffs(fYn);
// do the cascaded APFs
ﬂ oat fAPF_1_Out = m_LeftAPF_1.doBiQuad(pInputBuffer[0] +
m_fFeedbackLeft*fFeedback);
ﬂ oat fAPF_2_Out = m_LeftAPF_2.doBiQuad(fAPF_1_Out);
ﬂ oat fAPF_3_Out = m_LeftAPF_3.doBiQuad(fAPF_2_Out);
ﬂ oat fAPF_4_Out = m_LeftAPF_4.doBiQuad(fAPF_3_Out);
ﬂ oat fAPF_5_Out = m_LeftAPF_5.doBiQuad(fAPF_4_Out);
ﬂ oat fAPF_6_Out = m_LeftAPF_6.doBiQuad(fAPF_5_Out);
// for next sample period
m_fFeedbackLeft = fAPF_6_Out;
pOutputBuffer[0] = fDepth*fAPF_6_Out + (1.0 - fDepth)*pInputBuffer[0];
// use the fc and Q to calculate the Left LPF coeffs
if(m_uLFO_Phase == QUAD)
calculateFirstOrderRightAPFCoeffs(fYqn);
else
calculateFirstOrderRightAPFCoeffs(fYn);
// do the cascaded APFs
fAPF_1_Out = m_RightAPF_1.doBiQuad(pInputBuffer[1] +
m_fFeedbackRight*fFeedback);
fAPF_2_Out = m_RightAPF_2.doBiQuad(fAPF_1_Out);
fAPF_3_Out = m_RightAPF_3.doBiQuad(fAPF_2_Out);
fAPF_4_Out = m_RightAPF_4.doBiQuad(fAPF_3_Out);
fAPF_5_Out = m_RightAPF_5.doBiQuad(fAPF_4_Out);
fAPF_6_Out = m_RightAPF_6.doBiQuad(fAPF_5_Out);
// for next sample period
m_fFeedbackRight = fAPF_6_Out;
// Mono-In, Stereo-Out (AUX Effect)
if(uNumInputChannels == 1 && uNumOutputChannels == 2)
pOutputBuffer[1] = pOutputBuffer[0];

Modulated Filter Effects  451
// Stereo-In, Stereo-Out (INSERT Effect)
if(uNumInputChannels == 2 && uNumOutputChannels == 2)
  pOutputBuffer[1] = fDepth*fAPF_6_Out + (1.0 – fDepth)
*pInputBuffer[1];
return true;
  }
 Build and test the plug-in. Try out the different modes; turn up the intensity and depth
controls for very intense effects. This phaser should be one of the ﬁ nest you have heard as it
is not like some digital phasers that only use one giant APF. The cascaded APFs are the way
to go with this effect. The saw LFO with copious feedback sounds very much like a classic
pulsating “vibe” effect.
       Bibliography
  Anderton, C. 1981.  Electronic Projects for Musicians . London: Wise Publications.
 Dodge, C. and Jerse, T. 1997.  Computer Music Synthesis, Composition and Performance .  Chapter 2 . New York:
Schirmer.
 Giles, M., ed. 1980.  The Audio/Radio Handbook . Santa Clara, CA: National Semiconductor Corp.
   Roads, C. 1996.  The Computer Music Tutorial .  Chapter 2 . Cambridge, MA: The MIT Press.
References
  musicdsp.org . “Envelope follower with different attack and release.” Accessed 2008,  http://www.musicdsp.org/
archive.php?classid=0#205.
Orfanidis, S. 1996. Introduction to Signal Processing, Chapter 8. Englewood Cliffs, NJ: Prentice-Hall.
Zöler, U. 2011. DAFX—Digital Audio Effects, Chapter 4. West Sussex, U.K.: John Wiley & Sons.

CHAP TE R 13

Dynamics Processing
    Dynamics processors are designed to automatically control the amplitude, or gain, of an
audio signal and consist of two families: compressors and expanders. Technically speaking,
compressors and expanders both change the gain of a signal after its level rises  above  a
predetermined threshold value. A compressor reduces the gain of the signal once it goes over
the threshold. An expander raises the gain of a signal after it crosses above the threshold.
With the exception of noise reduction systems, true expanders are rare since they can easily
cause instabilities, runaway gain, and distortion. What is normally called an “expander” today
is technically a  downward expander . A downward expander reduces the gain of a signal after
it drops  below  the threshold. We will be designing a downward expander but will use the
common lingo and refer to it simply as an expander. Both compressors and expanders require
the user to decide on the threshold of operation as well as a  ratio  value that tells the device
how much gain reduction to implement.
  Figure 13.1  shows the input/output transfer functions for the compressor family. The line
marked 1:1 is unity gain. A given input  x (dB) results in the same output  y (dB). Above the
threshold, the output level is compressed according to the ratio. For example, above the
threshold on the 2:1 line, every increase in 1 dB at the input results in an increase of only
0.5 dB at the output. On the 4:1 line, each 1 dB input increase results in a 0.25 dB output
increase. On the `:1 line, increasing the input amplitude beyond the threshold results in no
output increase at all, thus  y (dB) is limited to the threshold value. This version of the device is
called a  limiter  and represents the most extreme form of compression.
  Figure 13.2  shows the input/output transfer functions for the expander family. The ratios
are reversed. You can see that as the signal falls below the threshold, it is attenuated by the
ratio amount. For example, below the threshold on the 1:2 line, every decrease in 1dB at the
input results in a decrease of 2 dB at the output. On the 1:` line, when the input falls below
the threshold it receives inﬁ nite attenuation, that is, it is muted. This version of the device is
called a  gate  (or noise gate) and represents the most extreme form of downward expansion.
Perhaps the most important aspect of both families is that their gain reduction curves are
linear for logarithmic axes. These devices operate in the  log domain . The two families of
dynamics processors yield four combinations: compression and limiting, and expansion and
gating. We will implement all four of them.
453
454  Chapter 13
yo OUT (dB)
1:1
2:1
A
y(dB)
4:1
∞ : 1
∆ x(dB)
Threshold
X IN   (dB)
 Figure 13.1:    The generalized input/output transfer curves for a compressor at various
reduction ratios. The ratio is in the form D x :D y .
y OUT (dB)
1:1
∆ y (dB)
1:2
1:4
1:  ∞
∆  x (dB)
Threshold
x IN
(dB)
 Figure 13.2:    The generalized input/output transfer curves for a downward expander at
various reduction ratios. The ratio is in the form D x :D y .
  Figure 13.3  shows the feed-forward and feedback topologies for dynamics processors. There
is some debate as to which is best; we’ll stay out of that argument and design the feed-forward
version ﬁ rst. Then, you can design a feedback version for yourself. Both designs include
a detector/computer and a  digitally controlled ampliﬁ er/attenuator  (DCA). The detector
analyzes the input signal and computes a new gain value to be applied to the signal. The
detector/computer block lies in the  side-chain  of the circuit.
 Because the compressor and downward expander are both gain reduction devices, an
additional stage of make-up gain is required. In some designs, the make-up gain is calculated
in the side-chain while in others it sits outside of the DCA . Both accomplish the same task.
We are going to add another gain stage at the input (similar to the mod ﬁ lter in  Chapter 12 )

Dynamics Processing  455
to drive the detector; this pre-gain block is useful when the source material is very low in
amplitude, making it difﬁ cult to trigger the detector.  Figure 13.4  shows the block diagram of
the ﬁ rst dynamic processor plug-in.
  Figure 13.4  labels the input and control signals as follows:
(cid:129)
(cid:129)
(cid:129)
(cid:129)
(cid:129)
    x ( n ),  y ( n ) 5 input/output
    d ( n ) 5 detector output
    G ( n )  5 gain value for current sample
    d  o  5 threshold in dB
   r 5 ratio
 The CEnvelopeDetector object you used in the last chapter has a log output mode so we
can combine the detector and log conversion blocks together, as shown in the dotted box in
 Figure 13.4 . These feed the gain calculation block, which uses the threshold (in dB) and the
ratio to set the gain for a given detector output. Orfanidis (1996 ), Zöler (2011), and Reiss
(2011) have given equations that describe the gain reduction calculation. The Zöler equations
( Equations 13.1  and  13.2 ) seem to be the most concise. These equations require additional
slope variables  ( CS  and  ES ) that are calculated from the ratio. In the compressor,  CS  varies
x(n)
DCA
y(n)
x(n)
DCA
y (n)
Detector/
Computer
Control
Signal
Detector/
Computer
Control
Signal
 Figure 13.3:    The feed-forward and feedback topologies for a dynamics processor.
A detector calculates the peak or RMS value of the signal to create a control signal.
The DCA attenuates the input signal according to its control value.
x(n)
Input
DC A
Out
y(n)
Output
d(n)
G(n)
In
RMS
Detector
log
Conversion
gain calc
(dB)
lin
Conversion
Attack
Release
CEnvelopeDetector
Threshold
dO
Ratio
ρ
 Figure 13.4:    A generalized feed-forward topology for the compressor/downward
expander families.
456  Chapter 13
between 0 and 1 as the ratio r varies from 1 to `. In the downward expander,  ES  varies
between 0 and −` as the ratio r varies from ` to 1.
  Compressor gain (dB):
  where
  G(n) =  f(d(n))
  CS 5 e
  f  5 e
1 2 1 1/r2     for compression
1.0                  for limiting
CS # 1 d0 2 d(n)2    if d(n) $ d0
0.0                         if d(n) , d0
  Downward expander gain (dB):
  G(n) =  f(d(n))
  where
  ES 5 e
  f  5 e
1 1/r2 2 1    for downward expansion
0.0                  for gating
ES # 1 d0 2 d(n)2    if d(n) , d0
0.0                         if d(n) $ d0
(13.1)
(13.2)
 The point in the input/output plot where the signal hits the threshold, thereby engaging the
device, is called the  knee  of the compression curve. With a hard knee the signal is either
above the threshold or below it and the device kicks in and out accordingly. The soft-knee
compressor allows the gain control to change more slowly over a width of dB around
the threshold called the  knee width  ( W  ). In this way, the device moves more smoothly
into and out of compression or downward expansion.  Figure 13.5  compares hard- and
soft-knee curves.
y OUT (dB)
Threshold
Soft knee
Hard knee
Knee width
X
IN
(dB)
 Figure 13.5:    Hard and soft-knee compression curves.

Dynamics Processing  457
y
OUT
(dB)
(-35, 0.75)
Ratio
1:4
=
CS = 0.75
Soft knee
Hard knee
W  = 10 dB
(-45, 0)
0
=
CS
-40 dB
 Figure 13.6:    Setting up the endpoints for interpolating the  CS  value for a
soft-knee compressor.
 There are several approaches to generating the soft-knee portion of the curve when
calculating the gain reduction value. Curve ﬁ tting by interpolating across the knee width
is popular. There are a variety of different ways to implement the curve ﬁ tting. We already
have a Lagrange interpolator in the software, so we can use this to ﬁ t a second-order
polynomial curve to the knee. This is done by altering the  CS  or  ES  values. The two
values above and below the threshold are the  x  values, while the two values of the slope
variable at the end points are the  y  components. When the detector signal is in the knee
zone around the threshold, we use its value to calculate a new slope.  Figure 13.6  shows
an example. The knee width ( W  ) is 10 dB with a −40 dB threshold. For a compression
ratio of 1:4,  CS  is 0.75. Therefore, the soft-knee  CS  will vary between 0 at −45 dB to 0.75
at −35 dB. The Lagrange interpolation will ﬁ nd the  CS  value for a given input detection
value across this range.
  13.1    Design a Compressor/Limiter Plug-In
 We are going to build a complete dynamics processor with the following features and
functions:
(cid:129)
(cid:129)
(cid:129)
(cid:129)
(cid:129)
(cid:129)
(cid:129)
(cid:129)
(cid:129)
   Compressor
   Limiter
   Expander
   Gate
   Analog or digital time constant
   Attack, release controls for detector
   Threshold, ratio controls for gain computer
   Knee-width control: 0  5 hard knee processing
   Gain reduction meter to view the reduction
