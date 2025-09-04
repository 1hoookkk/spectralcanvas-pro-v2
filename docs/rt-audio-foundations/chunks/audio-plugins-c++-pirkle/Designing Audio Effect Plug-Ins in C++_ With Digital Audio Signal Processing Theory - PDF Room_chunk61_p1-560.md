# Audio Plugins C++ (Pirkle) — Chunk 61/73

> Source: `Designing Audio Effect Plug-Ins in C++_ With Digital Audio Signal Processing Theory - PDF Room.pdf` · Pages ~1-560 · Extractor: pdfminer.six
> Target ≈ 2800 words, Overlap = 200 words · Actual = 2794

> Note: DSP textbook content for SpectralCanvasPro RT-audio knowledge base

---
436  Chapter 12
// do the BiQuads
pOutputBuffer[0] = m_LeftLPF.doBiQuad(pInputBuffer[0]);
// detect the other channel
ﬂ oat fDetectRight = m_RightDetector.detect(fGain*pInputBuffer[1]);
// set mod freq to minimum (un-triggered)
ﬂ oat fModFreqRight = m_fMinCutoffFreq;
// if threshold triggered, calculate new LPF cutoff
if(fDetectLeft >= m_fThreshold)
fModFreqRight = calculateCutoffFreq(fDetectRight);
// use the mod freq and user supplied-Q to calc the fc
calculateLPFCoeffs(fModFreqRight, m_fQ, &m_RightLPF);
// Mono-In, Stereo-Out (AUX Effect)
if(uNumInputChannels == 1 && uNumOutputChannels == 2)
pOutputBuffer[1] =  pOutputBuffer  [0];
// Stereo-In, Stereo-Out (INSERT Effect)
if(uNumInputChannels == 2 && uNumOutputChannels == 2)
         pOutputBuffer[1] = m_RightLPF.doBiQuad(pInputBuffer[1]);
return true;
  }
 Build and test the plug-in. Try all the controls. Note: Beware—because of the high pre-gain
values possible combined with high- Q  values, you can easily overload the audio. If you hear
distortion, adjust the pre-gain value. Test this out with guitar, keyboard, or drum loops. Ideas
for more plug-ins include:
•
•
•
•
•
   Change the ﬁ lter type (high-pass ﬁ lter, band-stop ﬁ lter, band-pass ﬁ lter).
   Modulate the  Q  in the same direction, or opposite direction, as the  f  c .
   Modulate the fc or  Q  with an LFO and the other parameter with the envelope.
   Allow the user to select the detection mode: peak, MS, or RMS.
   Allow the user to change between linear and log detection (see CEnvelopeDetector’s
init() method).
     12.6    Design a Phaser Plug-In
 The phaser effect will be on par with the reverb algorithms as among the more complex
in the book—the theory is not that difﬁ cult, but a nice analog sounding phaser requires
many nearly duplicated components, in this case APFs. The phaser effect was an attempt at
creating a ﬂ anging effect before the necessary analog  integrated circuits were available. A
ﬂ anger uses a modulated delay line to alter the delay time. A phaser uses APFs in an attempt

Modulated Filter Effects  437
to do the same. APFs have a delay response that is ﬂ at across the ﬁ rst one-third or so of
the  DC-to-Nyquist band. In fact, APFs can be designed to perform fractional delay as an
alternative to interpolation. The phaser didn’t sound like a ﬂ anger at all. The reason is that
in a ﬂ anger, the notches in the inverse-comb ﬁ ltering are mathematically related as simple
multiples of each other. This is not the case for the phaser. The phaser is still used today in
electronic music. Like the ﬂ anger, we will get the best results by ﬁ rst looking at the original
analog design. Our phaser topology will be based off the analog counterpart in  Figures
12.15  and  12.16 . Even if you can’t read schematics, it’s worth a moment to take a look at the
overall architecture since we’ll be emulating it in digital form. The complete block diagram
is shown in  Figure 12.17 .
 The phase of the signal is shifted by 90 degrees at the frequency given by  Equation 12.2 :
  fp 5
1
2pC1(R1//rDS)
(12.2)
 The positive control voltage is a triangle wave, which alters the resistance of the  ﬁ eld effect
transistor (FET) that is in parallel with  R  1 . This in turn changes the 90 degree phase shift
frequency. The values of  C  1  vary from 1 uF to 0.06 uF, and  r  DS  varies from 100 V (FET on)
to 10 K (FET off). The input signal is run through a cascade of such modules. The output of
the last module, or stage, is then mixed with the dry input signal to create the effect. You only
need to decide on the number of stages and the oscillator to complete the design.
 The things to note about the analog design are:
•
•
•
   There are six ﬁ rst-order APF modules.
   The APF modules are in series, one feeding the next.
   The depth control is a blend of the dry and ﬁ ltered signals; a depth of 100% is actually a
50/50 mix of wet and dry.
20K
–
+
V
In
20K
C1  1uF
10K  R1
rDS
out V
Positive
Control
Voltage
 Figure 12.15:    A voltage-controlled ﬁ rst-order all-pass ﬁ lter module.

438  Chapter 12
Input Buffer
Vin
1M
+
–
Output Mixer/Buffer
100  K
20 K
20 K
–
+
Vout
20 K
20 K
20 K
–
+
20 K
1uF
10K
20 K
20 K
–
+
0.47uF
Vc  10K
–
+
0.33uF
Vc  10K
Vc
6 Phase-Shift
Stages
20 K
20K
20 K
–
+
20K
0.16uF
10K
20 K
20K
–
+
0.1uF
Vc 10K
–
+
0.06uF
Vc  10K
10K
Vc
"Depth"
V+  +12
Low
Frequency
Oscillator
(0.05 -5Hz)
75 K
75 K
360 K
1M
+
–
"Rate"
250 K
2.4 K
Vc
+
100uF
Figure 12.16: The phaser circuit (from National Semiconductor).
Input
LFO
APF1
APF2
APF3
Output
1 -  Depth
APF4
APF5
APF6
Depth
FB
Figure 12.17: The block diagram of the analog phaser design. The feedback path has been added
to the original National Semiconductor design; the feedback will be named “Intensity”.
Modulated Filter Effects  439
•
•
   Other phaser models also included a feedback path from the ﬁ ltered output back to the
ﬁ lter chain input to increase intensity of the effect.
   The six APF modules have the frequency ranges in  Table 12.7 .
 Because this is a complex design, we will break it down into two parts: ﬁ rst, we’ll design an
abbreviated version in  Figure 12.18  with only three APF modules and no feedback control.
Then, we’ll add the other three APF stages and the feedback control.
 The design equations for ﬁ rst-order APFs from  Chapter 6  are given next.
 Table 12.7:    Minimum and maximum phase rotation frequencies for the phaser APFs.
APF
Minimum fc
Maximum fc
1
2
3
4
5
6
16 Hz
33 Hz
48 Hz
98 Hz
160 Hz
260 Hz
1.6 kHz
3.3 kHz
4.8 kHz
9.8 kHz
16 kHz
26 kHz*
   *  We will have to cut off at ½ Nyquist instead.
 Specify:
•
    f  c , corner frequency
2 2 1
2 1 1
(12.3)
uc 5 2pfc/fs
 a 5
 tan 1 pQ/fs
 tan 1 pQ/fs
 b 5 2 cos uc
a0 5 2a
a1 5 b1 12a2
a2 5 1.0
b1 5 b1 12a2
b2 5 21.0
 We’ll need to implement the APF using the built-in BiQuad object. The built-in oscillator will
be used for the LFO; many aspects of the design and code chunks can be borrowed from your
ModFilter projects. Speciﬁ c to this design is that it:

440  Chapter 12
Input
APF1
APF2
APF3
1 –  Depth
Depth
Σ
Output
LFO
 Figure 12.18:    The block diagram of the simpliﬁ ed, ﬁ rst part of the design.
•
•
•
   Needs multiple APFs, each with its own range of frequencies
   Needs only one LFO to control all APFs
   Needs to calculate all APF cutoff frequencies and coefﬁ cients at the same time
 Let’s get started with the usual steps.
  12.6.1  Project Phaser
 Create a new project. There are no member plug-ins to add.
   12.6.2  Phaser GUI
 Add the controls from  Table 12.8  to your GUI.
   Table 12.8:    The phaser GUI elements.
Slider Property
Control Name
Units
Variable Type
Variable Name
Low Limit
High Limit
Initial Value
Value
Rate
Hz
ﬂ oat
m_fModRate
0.02
10
0.5
Slider Property
Control Name
Units
Variable Type
Variable Name
Low Limit
High Limit
Initial Value
Value
Depth
%
ﬂ oat
m_fMod_Depth
0
100
50
Button Property
Control Name
Units
Variable Type
Variable Name
Enum String
Value
LFO Type
enum
m_uLFO_Waveform
sine, saw, tri, square
   12.6.3  Phaser.h File
 Declare instances of the BiQuad and WaveTable objects. Also, declare the functions
to calculate cutoff frequencies and ﬁ lter coefﬁ cients. We will calculate all the APF
Modulated Filter Effects  441
coefﬁ cients in one pair of functions (one for left and one for right). Because there is so
much repeated code, we will also add a helper function to calculate the APF coefﬁ cients
for each bi-quad. You don’t need to #include anything since these are built in. Add these
to your .h ﬁ les
    // Add your code here: ------------------------------------------------------- //
     CBiquad m_LeftAPF_1;
     CBiquad m_RightAPF_1;
     CBiquad m_LeftAPF_2;
     CBiquad m_RightAPF_2;
     CBiquad m_LeftAPF_3;
     CBiquad m_RightAPF_3;
    // function to calculate the new fc from the Envelope Value
     ﬂ oat calculateAPFCutoffFreq(ﬂ oat fLFOSample, ﬂ oat fMinFreq, ﬂ oat fMaxFreq);
    // Two functions to calculate the BiQuad Coeffs: APF
     void calculateFirstOrderLeftAPFCoeffs(ﬂ oat fLFOSample);
     void calculateFirstOrderRightAPFCoeffs(ﬂ oat fLFOSample);
    // helper function for APF Calculation
     void calculateFirstOrderAPFCoeffs(ﬂ oat fCutoffFreq,
CBiquad* pBiQuadFilter);
    // min/max variables
     ﬂ oat m_fMinAPF_1_Freq;
     ﬂ oat m_fMaxAPF_1_Freq;
     ﬂ oat m_fMinAPF_2_Freq;
     ﬂ oat m_fMaxAPF_2_Freq;
   ﬂ oat m_fMinAPF_3_Freq;
     ﬂ oat m_fMaxAPF_3_Freq;
   // LFO Stuff
     CWaveTable m_LFO;
   // END OF USER CODE ---------------------------------------------------------- //
   12.6.4  Phaser.cpp File
  Constructor
•
•
   Initialize the rate min and max values. The objects are self-initializing upon creation.
   Initialize the ﬁ rst three APF min/max pairs according to the table. We’ll add the others
once we have tested it and ﬁ nd it works properly.

442  Chapter 12
  CPhaser::CPhaser()
  {
<SNIP SNIP SNIP>
// Finish initializations here
// set our Min and Max Modulation points
m_fMinAPF_1_Freq = 16.0;
m_fMaxAPF_1_Freq = 1600.0;
m_fMinAPF_2_Freq = 33.0;
m_fMaxAPF_2_Freq = 3300.0;
m_fMinAPF_3_Freq = 48.0;
m_fMaxAPF_3_Freq = 4800.0;
  }
 Write the calculateAPFCutoffFreq() function. This function behaves exactly like the function
in the mod ﬁ lter except there is no mod depth. Also note that we will call this function
repeatedly for each APF unit, so we need to send it the min/max values to compute with; the
function itself is trivial and just like all the previous mod ﬁ lters:
   ﬂ oat CPhaser::calculateAPFCutoffFreq(ﬂ oat fLFOSample, ﬂ oat fMinFreq, ﬂ oat fMaxFreq)
  {
return fLFOSample*(fMaxFreq (cid:3) fMinFreq) + fMinFreq;
  }
 Implement the helper function for calculating the APF coefﬁ cients for a given  f  c .
  void CPhaser::calculateFirstOrderAPFCoeffs(ﬂ oat fCutoffFreq,
CBiquad* pBiQuadFilter)
  {
  }
// coeff calculation
ﬂ oat alpha_num = tan(pi*fCutoffFreq/(ﬂ oat)m_nSampleRate) (cid:3) 1.0;
ﬂ oat alpha_den = tan(pi*fCutoffFreq/(ﬂ oat)m_nSampleRate) + 1.0;
ﬂ oat alpha = alpha_num/alpha_den;
// set on target ﬁ lter
pBiQuadFilter->m_f_a0 = alpha;
pBiQuadFilter->m_f_a1 = 1.0;
pBiQuadFilter->m_f_a2 = 0.0;
pBiQuadFilter->m_f_b1 = alpha;
pBiQuadFilter->m_f_b2 = 0.0;
 Implement the cooking functions below for the right and left banks of APFs using the helper
function  above:
   void CPhaser::calculateFirstOrderLeftAPFCoeffs(ﬂ oat fLFOSample)
  {
// APF1 (cid:3) fc (cid:3)> Bi Quad
ﬂ oat fCutoffFreq = calculateAPFCutoffFreq(fLFOSample, m_fMinAPF_1_Freq,

Modulated Filter Effects  443
calculateFirstOrderAPFCoeffs(fCutoffFreq, &m_LeftAPF_1);
m_fMaxAPF_1_Freq);
// APF2 (cid:3) fc (cid:3)> Bi Quad
fCutoffFreq = calculateAPFCutoffFreq(fLFOSample, m_fMinAPF_2_Freq,
m_fMaxAPF_2_Freq);
calculateFirstOrderAPFCoeffs(fCutoffFreq, &m_LeftAPF_2);
// APF3 (cid:3) fc (cid:3)> Bi Quad
fCutoffFreq = calculateAPFCutoffFreq(fLFOSample, m_fMinAPF_3_Freq,
m_fMaxAPF_3_Freq);
calculateFirstOrderAPFCoeffs(fCutoffFreq, &m_LeftAPF_3);
  }
   void CPhaser::calculateFirstOrderRightAPFCoeffs(ﬂ oat fLFOSample)
  {
// APF1 (cid:3) fc (cid:3)> Bi Quad
ﬂ oat fCutoffFreq = calculateAPFCutoffFreq(fLFOSample, m_fMinAPF_1_Freq,
calculateFirstOrderAPFCoeffs(fCutoffFreq, &m_RightAPF_1);
m_fMaxAPF_1_Freq);
// APF2 (cid:3) fc (cid:3)> Bi Quad
fCutoffFreq = calculateAPFCutoffFreq(fLFOSample, m_fMinAPF_2_Freq,
m_fMaxAPF_2_Freq);
calculateFirstOrderAPFCoeffs(fCutoffFreq, &m_RightAPF_2);
// APF3 - fc -> Bi Quad
fCutoffFreq = calculateAPFCutoffFreq(fLFOSample, m_fMinAPF_3_Freq,
m_fMaxAPF_3_Freq);
calculateFirstOrderAPFCoeffs(fCutoffFreq, &m_RightAPF_3);
  }
   prepareForPlay()
•
•
   Flush the delays in the APFs.
   Set up the LFO by setting the initial rate, LFO type (the LFO type will match our
m_uLFOWaveform variable).
  bool __stdcall CPhaser::prepareForPlay()
  {
// Add your code here:
m_LeftAPF_1.ﬂ ushDelays();
m_RightAPF_1.ﬂ ushDelays();
m_LeftAPF_2.ﬂ ushDelays();
m_RightAPF_2.ﬂ ushDelays();
m_LeftAPF_3.ﬂ ushDelays();
m_RightAPF_3.ﬂ ushDelays();

444  Chapter 12
// setup LFO
m_LFO.m_fFrequency_Hz = m_fModRate;
m_LFO.m_uPolarity = 1; // 0 = bipolar, 1 = unipolar
m_LFO.m_uTableMode = 0; // normal, no band limiting
m_LFO.m_uOscType = m_uLFO_Waveform;
m_LFO.setSampleRate(m_nSampleRate); // really important!
// the LFO prepareForPlay() calls reset() and cookFrequency()
m_LFO.prepareForPlay();
return true;
  }
   userInterfaceChange()
•
•
   If the user moves the rate slider, we need to change the LFO rate and toggle the oscillator
type if they use the buttons.
   Note: Make sure your nControlIndex values match your UI.
  bool __stdcall CPhaser::userInterfaceChange(int nControlIndex)
  {
// decode the control index, or delete the switch and use brute force calls
switch(nControlIndex)
{
case 0:
{
m_LFO.m_fFrequency_Hz = m_fModRate;
m_LFO.cookFrequency();
break;
}
case 41:
{
m_LFO.m_uOscType = m_uLFO_Waveform;
break;
}
default:
break;
}
return true;
  }
   processAudioFrame()
•
•
   Calculate a new LFO value.
   Cascade our three APFs one after another, feeding the output of each into the input of the
next until done.

Modulated Filter Effects  445
•
•
   Use the depth control to mix the output.
   Calculate the depth so that 100% depth gives a 50/50 mix ratio (done by dividing depth
percent by 200.0 instead of 100).
  bool __stdcall CPhaser::processAudioFrame(ﬂ oat* pInputBuffer, ﬂ oat* pOutputBuffer, UINT
uNumInputChannels, UINT uNumOutputChannels)
  {
// Do LEFT (MONO) Channel; there is always at least one input, one output
 ﬂ oat fYn = 0;  // normal output
 ﬂ oat fYqn = 0;  // quad phase output
// mod depth is at 100% when the control is at 50% !!
ﬂ oat fDepth = m_fMod_Depth/200.0;
// call the LFO function; we only need ﬁ rst output
m_LFO.doOscillate(&fYn, &fYqn);
// use the LFO to calculate all APF banks
calculateFirstOrderLeftAPFCoeffs(fYn);
// do the cascaded APFs
ﬂ oat fAPF_1_Out = m_LeftAPF_1.doBiQuad(pInputBuffer[0]);
ﬂ oat fAPF_2_Out = m_LeftAPF_2.doBiQuad(fAPF_1_Out);
ﬂ oat fAPF_3_Out = m_LeftAPF_3.doBiQuad(fAPF_2_Out);
// form the output
pOutputBuffer[0] = fDepth*fAPF_3_Out + (1.0 (cid:3) fDepth)*pInputBuffer[0];
// calculate
calculateFirstOrderRightAPFCoeffs(fYn);
// do the cascade
fAPF_1_Out = m_RightAPF_1.doBiQuad(pInputBuffer[1]);
fAPF_2_Out = m_RightAPF_2.doBiQuad(fAPF_1_Out);
fAPF_3_Out = m_RightAPF_3.doBiQuad(fAPF_2_Out);
// Mono-In, Stereo-Out (AUX Effect)
if(uNumInputChannels == 1 && uNumOutputChannels == 2)
pOutputBuffer[1] = pOutputBuffer[0];
// Stereo-In, Stereo-Out (INSERT Effect)
if(uNumInputChannels == 2 && uNumOutputChannels == 2)
 pOutputBuffer[1] = fDepth*fAPF_3_Out + (1.0 (cid:3) fDepth)*pInputBuffer[1];
return true;
  }
 Build and test the plug-in. You should be able to easily hear the phaser effect, though it might
not be as strong as you are used to with other devices. We’re going to ﬁ x this next by added
more APF stages, feedback, and the option for quadrature phase LFO operation.

446  Chapter 12
     12.7    Design a Stereo Phaser with Quad-Phase LFOs
 Adding the extra APFs is just more of the same cut-and-paste operation; you only need
to change the range of frequencies and adjust the cooking and processing functions for
that. The addition of the feedback is simple—we need another slider on the UI and a
feedback  z  −1  storage device. The quadrature phase is easy to implement because our LFO
already produces this output for us; we only need a UI change and a branch statement in
the code.
  12.7.1  Phaser GUI
 Update the GUI with the new controls in  Table 12.9 .
   Table 12.9:    Additional controls for the quad-phase phaser plug-in.
Button Property
Control Name
Units
Variable Type
Variable Name
Enum String
Value
LFO Type
enum
m_uLFO_Phase
NORM, QUAD
Slider Property
Control Name
Units
Variable Type
Variable Name
Low Limit
High Limit
Initial Value
Value
Intensity
%
ﬂ oat
m_fFeedback
0
99
50
   12.7.2  Phaser.h File
 We need to add three more APF object sets (three for left, three for right) plus the feedback
storage elements, one each for left and right. Add the following to your .h ﬁ le:
    // Add your code here: -----------------------------------------------------------
  <SNIP SNIP SNIP>
    CBiquad m_LeftAPF_3;
    CBiquad m_RightAPF_3;
     CBiquad m_LeftAPF_4;
     CBiquad m_RightAPF_4;
     CBiquad m_LeftAPF_5;
     CBiquad m_RightAPF_5;
     CBiquad m_LeftAPF_6;
     CBiquad m_RightAPF_6;

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
