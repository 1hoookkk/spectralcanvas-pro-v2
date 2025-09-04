# Audio Plugins C++ (Pirkle) — Chunk 60/73

> Source: `Designing Audio Effect Plug-Ins in C++_ With Digital Audio Signal Processing Theory - PDF Room.pdf` · Pages ~1-560 · Extractor: pdfminer.six
> Target ≈ 2800 words, Overlap = 200 words · Actual = 2626

> Note: DSP textbook content for SpectralCanvasPro RT-audio knowledge base

---
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

430  Chapter 12
Charge
100 %
63.2%
r
τ a
36.8%
0 %
Time
Charging
Discharging
 Figure 12.12:    Analog attack and release times.
x(n)
Detector Type
PEAK:  x
MS:  x  2
RMS: sqrt(MS)
CEnvelopeDetector
τ a  or τ
r
(cid:153)
(cid:153)
y(n)  =  envelope
z-1
 Figure 12.13:    The CEnvelopeDetector block diagram.
 Compare this to the block diagram in  Figure 12.13  and you can see how it implements the
detector.
  12.5.1  Project EnvelopeFollower
 Create the project; there are no member plug-ins to add.
   12.5.2  EnvelopeFollower GUI
 The GUI ( Table 12.6 ) needs to have controls for the envelope detector:
•
•
•
•
•
   Pre-gain
   Threshold
   Attack time
   Release time
   Time constant (analog or digital)
τ
   Table 12.6:    The GUI controls for the envelope follower plug-in.
Modulated Filter Effects  431
Slider Property
Control Name
Units
Variable Type
Variable Name
Low Limit
High Limit
Initial Value
Slider Property
Control Name
Units
Variable Type
Variable Name
Low Limit
High Limit
Initial Value
Slider Property
Control Name
Units
Variable Type
Variable Name
Low Limit
High Limit
Initial Value
Value
Pre-Gain
dB
ﬂ oat
m_fPreGain_dB
0
20
12
Value
Attack Time
mSec
ﬂ oat
m_fAttack_mSec
10
100
25
Value
Q
ﬂ oat
m_fQ
0.5
20
5
Slider Property
Control Name
Units
Variable Type
Variable Name
Low Limit
High Limit
Initial Value
Slider Property
Control Name
Units
Variable Type
Variable Name
Low Limit
High Limit
Initial Value
Value
Threshold
ﬂ oat
m_fThreshold
0
1
0.2
Value
Release Time
mSec
ﬂ oat
m_fRelease_mSec
20
250
50
Button Property
Value
Control Name
Units
Variable Type
Variable Name
Enum String
Time Constant
enum
m_uTimeConstant
analog, digital
Button Property
Control Name
Units
Variable Type
Variable Name
Enum String
Value
Direction
enum
m_uDirection
UP, DOWN
 And, it needs controls for the modulated ﬁ lter:
•
•
    Q
   Direction of modulation (up or down)
   12.5.3  EnvelopeFollower.h File
 We need to add the following new functions and variables; most can be taken directly from
your ModFilter project. We will need:
•
   An LPF bi-quad for each channel (left, right)
432  Chapter 12
•
•
•
•
   An envelope detector for each channel (left, right)
   A function to calculate a new LPF cutoff value from an envelope value
   Functions to calculate the bi-quad coefﬁ cients from the LPF cutoff and  Q  values
   Min/max LPF cutoff variables
 Add the following to your .h ﬁ le:
  // Add your code here: ------------------------------------------------------- //
  // two LPF BiQuads for the ﬁ lters
   CBiquad m_LeftLPF;
   CBiquad m_RightLPF;
  // function to calculate the new fc from the Envelope Value
   ﬂ oat calculateCutoffFreq(ﬂ oat fEnvelopeSample);
  // calculate the coeffs for a given ﬁ lter
   void calculateLPFCoeffs(ﬂ oat fCutoffFreq, ﬂ oat fQ, CBiquad* pFilter);
  // min/max variables
   ﬂ oat m_fMinCutoffFreq;
   ﬂ oat m_fMaxCutoffFreq;
  // envelope detectors
   CEnvelopeDetector m_LeftDetector;
   CEnvelopeDetector m_RightDetector;
  // END OF USER CODE ---------------------------------------------------------- //
 Notice that everything except the envelope detectors is straight from your last project.
   12.5.4  EnvelopeFollower.cpp File
  Constructor
•
   Initialize the rate min and max values. The member-objects are self initializing upon
creation.
  CEnvelopeFollower::CEnvelopeFollower()
  {
<SNIP SNIP SNIP>
// Finish initializations here
// set our Min and Max Modulation points
m_fMinCutoffFreq = 100.0;
m_fMaxCutoffFreq = 5000.0;
  }
 Write the calculateCutoffFrequency() function. It is similar to the previous modulation
functions except that we will allow both up and down directions of modulation. Can you
ﬁ gure out how the code works?

Modulated Filter Effects  433
  ﬂ oat CEnvelopeFollower::calculateCutoffFreq(ﬂ oat fEnvelopeSample)
  {
// modulate from min upwards
if(m_uDirection == UP)
return fEnvelopeSample*(m_fMaxCutoffFreq ─ m_fMinCutoffFreq) +
else // modulate from max downwards
m_fMinCutoffFreq;
return m_fMaxCutoffFreq ─ fEnvelopeSample*(m_fMaxCutoffFreq –
m_fMinCutoffFreq);
return m_fMinCutoffFreq;
  }
 Write the cooking functions for the LPFs to set the new coefﬁ cients for a given  f  c  and  Q
value. This is identical to the ModFilter project.
   void CEnvelopeFollower::calculateLPFCoeffs(ﬂ oat fCutoffFreq, ﬂ oat fQ, CBiquad*
             pFilter)
  {
/* use same code as the Mod Filter */
  }
   prepareForPlay()
•
•
   Flush delays in LPFs.
   Initialize the detector objects.
  bool __stdcall CEnvelopeFollower::prepareForPlay()
  {
// Add your code here:
m_LeftLPF.ﬂ ushDelays();
m_RightLPF.ﬂ ushDelays();
// init the envelope detectors
// set all params at once with this function;
// false = Digital Time Constant NOT Analog one
if(m_uTimeConstant == digital)
{
 m_LeftDetector.init((ﬂ oat)m_nSampleRate, m_fAttack_mSec, m_fRelease_mSec,
false, DETECT_MODE_RMS, false);
m_RightDetector.init((ﬂ oat)m_nSampleRate, m_fAttack_mSec,
 m_fRelease_mSec, false, DETECT_MODE_RMS, false);
 m_LeftDetector.init((ﬂ oat)m_nSampleRate, m_fAttack_mSec, m_fRelease_mSec,
 true, DETECT_MODE_RMS, false);
m_RightDetector.init((ﬂ oat)m_nSampleRate, m_fAttack_mSec,
 m_fRelease_mSec, true, DETECT_MODE_RMS, false);
}
else
{
}
return true;
  }

434  Chapter 12
 You can see that the detectors are initialized all at once, using the m_uTimeConstant to
control the boolean value passed to the init function. The detection is set as MS (DETECT_
MODE_MS).
   userInterfaceChange()
•
•
   Decode the control ID and then call the appropriate function; we simply pass our slider
values or radio button command to the functions.
   Note: Make sure your nControlIndex values match with your UI; these mappings are for
my project.
  bool __stdcall CEnvelopeFollower::userInterfaceChange(int nControlIndex)
  {
switch(nControlIndex)
{
case 2:  // Attack
{
m_LeftDetector.setAttackTime(m_fAttack_mSec);
m_RightDetector.setAttackTime(m_fAttack_mSec);
break;
}
case 3:  // Release
{
m_LeftDetector.setReleaseTime(m_fRelease_mSec);
m_RightDetector.setReleaseTime(m_fRelease_mSec);
break;
}
case 41:  // Time Constant
{
if(m_uTimeConstant == digital)
{
m_LeftDetector.setTCModeAnalog(false);
m_RightDetector.setTCModeAnalog(false);
}
else
{
m_LeftDetector.setTCModeAnalog(true);
m_RightDetector.setTCModeAnalog(true);
}
}
default:
break;
}
return true;
  }

Modulated Filter Effects  435
   processAudioFrame()
•
   The envelope follower’s processAudioFrame() function will operate in the sequence
shown in  Figure 12.14 .
  bool __stdcall CEnvelopeFollower::processAudioFrame(ﬂ oat* pInputBuffer, ﬂ oat*
pOutputBuffer, UINT
uNumInputChannels, UINT
uNumOutputChannels)
  {
//
// Do LEFT (MONO) Channel; there is always at least one input, one output
ﬂ oat fGain = pow(10, m_fPreGain_dB/20.0);
ﬂ oat fDetectLeft = m_LeftDetector.detect(fGain*pInputBuffer[0]);
// set mod freq to minimum (un-triggered)
ﬂ oat fModFreqLeft =m_fMinCutoffFreq;
// if threshold triggered, calculate new LPF cutoff
if(fDetectLeft >= m_fThreshold)
fModFreqLeft = calculateCutoffFreq(fDetectLeft);
// use the mod freq and user supplied-Q to calc the fc
calculateLPFCoeffs(fModFreqLeft, m_fQ, &m_LeftLPF);
Detect Left
Channel
Signal Above
Thresh?
No
Yes
Calculate  New
Mod  Frequency
Based on
Envelope
Mod Freq = Minimum
Calculate  Filter
Coefficients
Do Bi- Qua d Filter
 Figure 12.14:    The ﬂ owchart for the left channel of the envelope follower (right channel is same).

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
