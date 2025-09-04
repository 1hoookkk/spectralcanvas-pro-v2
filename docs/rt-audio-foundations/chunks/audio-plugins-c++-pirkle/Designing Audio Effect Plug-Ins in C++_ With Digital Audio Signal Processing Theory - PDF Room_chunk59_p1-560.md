# Audio Plugins C++ (Pirkle) — Chunk 59/73

> Source: `Designing Audio Effect Plug-Ins in C++_ With Digital Audio Signal Processing Theory - PDF Room.pdf` · Pages ~1-560 · Extractor: pdfminer.six
> Target ≈ 2800 words, Overlap = 200 words · Actual = 2675

> Note: DSP textbook content for SpectralCanvasPro RT-audio knowledge base

---
420  Chapter 12
x(n)
LPF
y(n)
Q
fc
LFO
LFO
 Figure 12.6:    The mod ﬁ lter with second LFO for Q.
  CWaveTable m_fc_LFO;
   CWaveTable m_Q_LFO;
  ﬂ oat m_fMinCutoffFreq;
  ﬂ oat m_fMaxCutoffFreq;
   ﬂ oat m_fMinQ;
   ﬂ oat m_fMaxQ;
  // functions to calculate the fc or Q for a given LFO sample
  ﬂ oat calculateCutoffFreq(ﬂ oat fLFOSample);
   ﬂ oat calculateQ(ﬂ oat fLFOSample);
  // calculate the coeffs for a given ﬁ lter
  void calculateLPFCoeffs(ﬂ oat fCutoffFreq, ﬂ oat fQ, CBiquad* pFilter);
  // END OF USER CODE --------------------------------------------------------------
   12.2.3  ModFilter.cpp File
  Constructor
•
   Initialize the min and max  Q  values.
  CModFilter::CModFilter()
  {
m_PlugInName = "ModFilter";
// Default to Stereo Operation:
// Change this if you want to support more/less channels
m_uMaxInputChannels = 2;
m_uMaxOutputChannels = 2;
// Finish initializations here
// set our Min and Max Modulation points
m_fMinCutoffFreq = 100.0;
m_fMaxCutoffFreq = 5000.0;
m_fMinQ = 0.577;
m_fMaxQ = 10;
  }

Modulated Filter Effects  421
 Write the calculateQ() function, which behaves the same way as calculateCutoffFreq(); you
only need to change the mod depth source and min and max values:
  ﬂ oat CModFilter::calculateQ(ﬂ oat fLFOSample)
  {
return (m_fMod_Depth_Q/100.0)*(fLFOSample*(m_fMaxQ − m_fMinQ)) + m_fMinQ;
  }
   prepareForPlay()
•
   Set up the  Q  LFO by setting the initial rate and LFO type (the LFO type will match our
m_uLFOwaveform variable).
  bool __stdcall CModFilter::prepareForPlay()
  {
// Add your code here:
<SNIP SNIP SNIP>
// the LFO prepareForPlay() calls reset() and cookFrequency()
m_fc_LFO.prepareForPlay();
// now do the Q LFO
m_Q_LFO.m_fFrequency_Hz = m_fModRate_Q;
m_Q_LFO.m_uPolarity = 1; // 0 = bipolar, 1 = unipolar
m_Q_LFO.m_uTableMode = 0; // normal, no band limiting
m_Q_LFO.m_uOscType = m_uLFO_Waveform;
m_Q_LFO.setSampleRate(m_nSampleRate); // really important!
// this calls reset() and cookFrequency()
m_Q_LFO.prepareForPlay();
return true;
  }
   userInterfaceChange()
•
   Update the  Q  LFO when the user changes this value.
  bool __stdcall CModFilter::userInterfaceChange(int nControlIndex)
  {
// add your code here
// brute force update all
m_fc_LFO.m_fFrequency_Hz = m_fModRate_fc;
m_fc_LFO.m_uOscType = m_uLFO_Waveform;
m_Q_LFO.m_fFrequency_Hz = m_fModRate_Q;
m_Q_LFO.m_uOscType = m_uLFO_Waveform;
// cook to calculate
m_fc_LFO.cookFrequency();
m_Q_LFO.cookFrequency();
etc…
  }

422  Chapter 12
   processAudioFrame()
•
•
•
•
•
•
   Calculate a new  f  c  LFO value.
   Use the LFO value to calculate a new  f  c  value.
   Calculate a new  Q  LFO value.
   Use the LFO value to calculate a new  Q  value.
   Use the  f  c  and  Q  values to calculate new ﬁ lter coefﬁ cients.
   Do the bi-quad routines on the input samples.
  bool __stdcall CModFilter::processAudioFrame(ﬂ oat* pInputBuffer, ﬂ oat* pOutputBuffer,
UINT uNumInputChannels, UINT
uNumOutputChannels)
  {
//
// output = input -- change this for meaningful processing
//
ﬂ oat fYn = 0; // normal output
ﬂ oat fYqn = 0; // quad phase output
// call the fc LFO function; we only need ﬁ rst output
m_fc_LFO.doOscillate(&fYn, &fYqn);
// calculate fc
ﬂ oat fc = calculateCutoffFreq(fYn);
// call the Q LFO funciton
m_Q_LFO.doOscillate(&fYn, &fYqn);
// calculate the new Q
ﬂ oat fQ = calculateQ(fYn);
// use to calculate the LPF
calculateLPFCoeffs(fc, fQ, &m_LeftLPF);
calculateLPFCoeffs(fc, fQ, &m_RightLPF);
// do the BiQuads
pOutputBuffer[0] = m_LeftLPF.doBiQuad(pInputBuffer[0]);
// Mono-In, Stereo-Out (AUX Effect)
if(uNumInputChannels == 1 && uNumOutputChannels == 2)
pOutputBuffer[1] = pOutputBuffer[0]; // just copy
// Stereo-In, Stereo-Out (INSERT Effect)
if(uNumInputChannels == 2 && uNumOutputChannels == 2)
pOutputBuffer[1] = m_RightLPF.doBiQuad(pInputBuffer[1]);
return true;
  }

Modulated Filter Effects  423
 You can see that we only had to add a few lines of code to get the second modulation source
working. Build and test the plug-in. You will hear the  f  c  and  Q  modulated by the two LFOs;
adjust the LFO rate and depth controls and try changing the LFO waveform type too. The  Q
modulation can be more subtle to hear, but if you use RackAFX’s oscillator and pump white
noise through it, the modulation becomes clearly audible. We’ll ﬁ nish the plug-in by making
one more modiﬁ cation: the ability to place the right and left LPF modulation sources in
quadrature phase.
     12.3    Design a Mod Filter Plug-In: Part III, Quad-Phase LFOs
 In the third design iteration, we will modify the current plug-in to allow for quadrature phase
LFOs. The block diagram is given in  Figure 12.7 .
 The parameters are as follows:
•
•
•
•
•
•
•
•
•
    f  c  mod rate (0.2 to 10 Hz)
    f  c  mod depth (0 to 100%)
    Q  mod rate (0.2 to 10 Hz)
    Q  mod depth (0 to 100%)
   LFO type (sine, tri, saw, square)
   LFO phase (normal,quadrature)
    f  c  mod range: 100Hz to 5kHz
    Q  mod range: 0.577 to 10
   LPF is ﬁ xed as second-order bi-quad
 In order to switch the LFOs into quadrature phase, we only need to add one more radio button
control and make a slight modiﬁ cation to the processAudioFrame() function.
  12.3.1  ModFilter GUI
 Add the Radio Button Control in  Table 12.4 . Note: This is a direct control variable—there is
nothing extra to add in userInterfaceChange() or prepareForPlay() since we only use it in the
single processAudioFrame() function.
x(n)
LPF
y(n)
90° Q
0°  fc
LFO
LFO
 Figure 12.7:    The mod ﬁ lter with quad-phase LFOs.
424  Chapter 12
   12.3.2  ModFilter.cpp File
  processAudioFrame()
•
   Check the enumerated variable and use the appropriate LFO output sample.
  bool __stdcall CModFilter::processAudioFrame(ﬂ oat* pInputBuffer, ﬂ oat* pOutputBuffer,
UINT uNumInputChannels, UINT
uNumOutputChannels)
  {
//
// output = input -- change this for meaningful processing
//
ﬂ oat fYn = 0; // normal output
ﬂ oat fYqn = 0; // quad phase output
// call the LFO function; we only need ﬁ rst output
m_fc_LFO.doOscillate(&fYn, &fYqn);
// calculate both fc values (can be streamlined!)
ﬂ oat fc = calculateCutoffFreq(fYn);
ﬂ oat fcq = calculateCutoffFreq(fYqn);
// get the Q LFO output
m_Q_LFO.doOscillate(&fYn, &fYqn);
// calculate both Q values
ﬂ oat fQ = calculateQ(fYn);
ﬂ oat fQq = calculateQ(fYqn);
// use the fc and Q to calculate the Left LPF coeffs
calculateLPFCoeffs(fc, fQ, &m_LeftLPF);
calculateLPFCoeffs(fc, fQ, &m_RightLPF);
// test the Phase variable; if not NORM, use the
// quad phase LFO values on the Right channel
if(m_uLFO_Phase == NORM)
calculateLPFCoeffs(fc, fQ, &m_RightLPF);
else
calculateLPFCoeffs(fcq, fQ, &m_RightLPF);
 Table 12.4:    The quad phase control.
Button Property
Control Name
Units
Variable Type
Variable Name
Enum String
Value
LFO
enum
m_uLFO_Phase
NORM,QUAD

Modulated Filter Effects  425
// do the BiQuads
pOutputBuffer[0] = m_LeftLPF.doBiQuad(pInputBuffer[0]);
// Mono-In, Stereo-Out (AUX Effect)
if(uNumInputChannels == 1 && uNumOutputChannels == 2)
  pOutputBuffer[1] = pOutputBuffer[0]; // just copy
// Stereo-In, Stereo-Out (INSERT Effect)
if(uNumInputChannels == 2 && uNumOutputChannels == 2)
  pOutputBuffer[1] = m_RightLPF.doBiQuad(pInputBuffer[1]);
return true;
  }
 Build and test the new plug-in. When you change the phase to quadrature, it will be easy to hear
the two modulations chasing each other across the two speakers. Ideas for future plug-ins include:
•
•
   Change the ﬁ lter type (high-pass ﬁ lter, band-stop ﬁ lter, band-pass ﬁ lter).
   Use a double-modulator: modulate one LFO with  another  LFO, then use that to control a
parameter.
      12.4    Design an Envelope Follower Plug-In
 While the straight-up mod ﬁ lter is pretty interesting, it has a mechanical quality about it
due to the LFO modulations. This might not be an issue for a  synthesizer, but it does not
sound musically dynamic. We can correlate the modulation to the musical input signal by
implementing an envelope follower, which uses the amplitude envelope of the input signal
to control the ﬁ lter parameters. For example, we can code it so that when the input signal
has a high amplitude, the  f  c  is modulated  up  to a higher value, and of course as the plug-in
creators, we can also do the opposite kind of modulation. The envelope follower requires
an audio envelope detector, and you can use one built into RackAFX. We will use the same
second-order LPF as the ﬁ lter to modulate; then you can try different ﬁ lter types on your own.
First, take a look at the block diagram in  Figure 12.8 .
 The parameters are as follows:
•
•
•
•
•
•
•
•
   Pre-gain: 0 to 120 dB of pre-gain to drive the detection circuit
   Threshold (the value the signal has to cross to engage the detector)
   Envelope attack time (mSec)
   Envelope release time (mSec)
   Up/down mod control (determines whether positive amplitude excursions result in
 positive or negative  f  c  excursions)
   Digital/analog detection mode: changes detector time constant
   User-adjustable  Q
   LPF is ﬁ xed as second-order bi-quad

426  Chapter 12
x (n)
y(n)
LPF
fc
Envelope
Detector
 Figure 12.8:    The envelope detector block diagram.
 We can use the same CBiquad objects for the LFO, but we need another object to handle
the envelope detection. This object is built in; the interface is in pluginconstants.h and the
implementation is found in the pluginobjects.cpp ﬁ le. The CEnvelopeDetector has the
member variables and functions shown in  Table 12.5 .
 This is a pretty extensive plug-in with multiple controls and interactions. Let’s start with the
detector itself. The CEnvelopeDetector is based on a simple detector  listed in the references
(musicdsp.org). The detector mimics an analog peak, mean-square, and RMS detector all in
one. To use the detector, follow these steps:
1.
2.
3.
4.
   Sample rate 5 object’s m_nSampleRate
   Attack time 5 10 mSec
   Release time 5 250 mSec
   Analog time constant 5 true
   Detect RMS signal level
   Linear,  not  log detection
   Create an instance of the detector, for example, CEnvelopeDetector m_Detector.
   Initialize all parameters at once with the init() function, for example, m_Detector.
init((ﬂ oat)m_nSampleRate, 10.0, 250.0, true, DETECT_MODE_RMS, false).
   This code initializes the detector with the following parameters:
a.
b.
c.
d.
e.
f.
   To change any of the parameters on the ﬂ y, use the individual functions:
a.
b.
c.
d.
   m_Detector.setAttackTime()
   m_Detector.setReleaseTime()
   m_Detector.setDetectMode()
   m_Detector.setTCModeAnalog()
 The attack and release times simply adjust these common parameters. The detection mode can
be set for peak, MS, or RMS. Because audio signals must be on the range of 21.0 to 11.0 in
RackAFX, this means that the squaring function of the MS and RMS modes will produce a
value that is less than the instantaneous value. In RMS mode, the MS value is square-rooted,
which makes the resulting envelope even smaller. Thus, you need to use care with the MS and
RMS modes. In this envelope follower, we will use the RMS value to trigger our dynamic ﬁ lter.
 The time constant determines how fast the detector rises or falls to its maximum (1.0) and
minimum (0.0) values. In an analog circuit, the  resistor–capacitor (RC) rise and fall times are
Modulated Filter Effects  427
  Table 12.5:    CEnvelopeDetector object deﬁ nition.
CEnvelopeDetector
Function: Implements an Envelope Detection Unit
Purpose
A z21delay element for the detector
The attack and release times for the detector in mSec
Member Variables
int m_nSample;
ﬂ oat m_fAttackTime;
ﬂ oat m_fReleaseTime;
ﬂ oat m_fSampleRate;
UINT  m_uDetectMode;
ﬂ oat m_fEnvelope;
Member Functions
void init(arguments)
Parameters:
•  ﬂ oat samplerate
•  ﬂ oat attack_in_ms
•  ﬂ oat release_in_ms
•  bool bAnalogTC
•  UINT uDetect Mode
•  bool bLogDetector
void setTCModeAnalog(bool bAnalogTC)
Parameters:
•  bool bAnalogTC
void setAttackTime(ﬂ oat attack_in_ms)
Parameters:
•  ﬂ oat attack_in_ms
void setReleaseTime(ﬂ oat release_in_ms)
Parameters:
•  ﬂ oat release_in_ms
void setDetectMode(UINT uDetect)
Parameters:
•  UINT uDetect
void prepareForPlay()
ﬂ oat detect(ﬂ oat fInput)
Parameters:
•  ﬂ oat fInput
The sample rate, needed for detection timing
The detection mode according to these pre-coded enumerated
values:
DETECT_MODE_PEAK = 0
DETECT_MODE_MS = 1
DETECT_MODE_RMS = 2
The current envelope value (this is the output of the device, which
it saves for the next sample to compare with)
The initialization function, called once before prepareForPlay()
Input: the sample rate in Hz
Input: the attack time in mSec
Input: the release time in mSec
Input: If true: set the attack and release time constants using the
analog deﬁ nition (time to rise/fall to 36.7% of min/max) — If false: set
the attack and release time constants using the 1% deﬁ nition (time to
rise/fall to 1% of min/max); the digital version is about 4X faster
Input: uDetect mode can be one of three constants:
DETECT_MODE_PEAK
DETECT_MODE_MS
DETECT_MODE_RMS
Input: true if you want the detector to be logarithmic (usually only
used with LED metering)
Input: sets the time constant variable; see above for more info
Change the attack time on the ﬂ y
Input: the attack time in mSec.
Change the release time on the ﬂ y
Input: the release time in mSec
Change the detection mode on the ﬂ y
Input: the detect mode; see m_uDetectMode above
The prepareForPlay() function for this object
The detection function; it produces the current envelope output
for a given input, x(n)
Input: the current input sample x(n)
428  Chapter 12
only 63.3% of the edge. In digital systems, the value is usually 80% to 99%. The envelope
detector has a control to allow you to change the time constant from the very fast (digital)
version or the slower (analog) version. We’ll try both types in the envelope follower plug-in.
   12.5    Envelope Detection
 Software envelope detectors are based on the analog circuits that preceded them. The two
basic types of detectors are half-wave and full-wave, depending on whether they track either
the positive or negative portion of the input waveform (half-wave) or both portions (full-
wave). Many of the classic hardware devices used the half-wave version. Either the positive
or negative portion may be tracked. The circuit consists of a diode to half-wave-rectify the
signal, followed by a “tank” capacitor. The capacitor charges and discharges along with the
audio signal. The attack knob is a potentiometer in series with the diode; it controls the rate
at which the cap charges. The release knob is a potentiometer connected to ground through
which the cap discharges. The RC combinations (one for the charging side and the other for
the discharging side) create the attack and release times of the detector ( Figure 12.9 ). The
positive portion of the waveform is detected and tracks the input according to the attack and
release times ( Figures 12.10  and  12.11 ).
R
ATTACK
D1
Vin
V out
R DECAY
+
C1
 Figure 12.9:    A simple positive half-wave detector.
Amplitude
t
 Figure 12.10:    Detector output (solid dark line) with short attack and release times.
Amplitude
Modulated Filter Effects  429
t
 Figure 12.11:    Detector output (solid dark line) with medium attack and release times.
 In analog RC circuits, the cap charges and discharges exponentially, as shown in
 Figure 12.12 . The analog attack time is calculated as the time it takes to reach 63.2% of the
full charge, while the release time is the time to release from the fully charged state down to
36.8% of the full charge. Different combinations of R and C change these times. A digital
envelope detector differs in the percent levels; it uses 99% and 1% as the attack and release
destination values. The curves remain exponential.
 It is well worth noting that there are several approaches for designing a software algorithm for
the analog envelope detector.  Orfanidis (1996) suggests using a full-wave rectiﬁ er followed by
an LPF with separate attack and release time constants.  Zöler (2011) suggests a mean-squarer
followed by an LPF with only a single time constant. In either case, a running average is
constantly measured and updated on each sample interval. The CEnvelopeDetector ( Figure 12.13 )
implements a similar method using a half-wave rectiﬁ er followed by an LPF whose input is scaled
by the attack or release time, depending on whether the signal is rising above the current envelope
output or falling below it. You can ﬁ nd the original  code source in the references (musicdsp.org).
 The attack and release times are conveniently calculated from your controls in milliseconds.
The calculations are shown in  Equation 12.1 :
ta 5 eTC/(attack_in_mSec*SampleRate*0.001)
and
tr 5 eTC/(release_in_mSec*SampleRate*0.001)
where
TCanalog 5 log(0.368)
TCdigital 5 log(0.01)
(12.1)
 The code for the difference equation is:
  if(fInput> m_fEnvelope)
m_fEnvelope = m_fAttackTime * (m_fEnvelope - fInput) + fInput;
  else
m_fEnvelope = m_fReleaseTime * (m_fEnvelope - fInput) + fInput;
