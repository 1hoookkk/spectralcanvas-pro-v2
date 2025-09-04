# Audio Plugins C++ (Pirkle) — Chunk 58/73

> Source: `Designing Audio Effect Plug-Ins in C++_ With Digital Audio Signal Processing Theory - PDF Room.pdf` · Pages ~1-560 · Extractor: pdfminer.six
> Target ≈ 2800 words, Overlap = 200 words · Actual = 2611

> Note: DSP textbook content for SpectralCanvasPro RT-audio knowledge base

---
Modulated Filter Effects
    Modulated ﬁ lter effects alter one or more ﬁ lter parameters with other signals called control
signals. The ﬁ lter parameters might include cutoff frequency,  Q , bandwidth, ﬁ lter type, or
overall gain. The control signals are usually periodic low-frequency oscillators (LFOs), a
signal envelope, an envelope generator (EG) or even another audio signal. In the modulated
delay effects, the LFO control signal changed the delay amount of the signal. In modulated
ﬁ lter effects, the ﬁ lter parameters are the ones that are changing. In this chapter you will
design and implement the following effects:
•
•
•
   Mod ﬁ lter using an LFO
   Envelope follower using an audio detector
   Phaser
 The phaser is a specialized effect that uses a bank of all-pass ﬁ lters (APFs) to try to brute
force delay the signal to make a ﬂ anging effect. An LFO modulates the depth of the APFs. In
 Figure 12.1  you can see a simple modulated ﬁ lter. The ﬁ lter type is a low-pass ﬁ lter (LPF).
The control signal is an LFO. The control parameter is the cutoff frequency of the ﬁ lter.
 In  Figure 12.2 , an EG moves the ﬁ lter parameter. An EG is triggered by some external event,
such as a note-on or -off event, or possibly triggered when the input level crosses a threshold.
A modulated ﬁ lter might have multiple parameters controlled by multiple control signals. For
example,  Figure 12.3  shows another LPF that has two modulation sources for two different
parameters.
   In  Figure 12.3  you can see a new module labeled envelope detector; it detects and follows the
peak, mean-squared (MS), or root-mean-squared (RMS) value of the control signal which is
the input signal here. The control signal could be taken from other sources such as another
audio signal or a complex LFO signal. The effect in  Figure 12.2  has a special name too: it is
called an envelope follower.
y(n)
x(n)
LPF
fc
LFO
 Figure 12.1:    A simple LFO-modulated LPF.
411
412  Chapter 12
x(n)
Trigger  Input
y(n)
LPF
fc
Envelope
Generator
 Figure 12.2:    An envelope generator can also modulate a ﬁ lter parameter.
x(n)
y(n)
LPF
Q
fc
Envelope
Detector
LFO
 Figure 12.3:    A doubly modulated LPF with both fc and Q controls.
  12.1    Design a Mod Filter Plug-In: Part I Modulated f c
 For our ﬁ rst effect design, we’ll start with a modulated second-order LPF and modulate the
cutoff frequency with an LFO. Then, we can increase the complexity by adding another LFO to
control the  Q  and even give the option to run the two LFOs in quadrature phase. We can use the
second-order digital resonant LPF you’ve already designed from  Chapter 6  for the ﬁ lter. Notice
that for this initial project, we will hold the LPF  Q  constant at 2.0. And, we will introduce a
built-in RackAFX object to handle the LFO for us. The block diagram is shown in  Figure 12.4 .
  The parameters are as follows:
•
•
•
•
•
•
     f  c  mod rate (0.2 to 10 Hz)
    f  c  mod depth (0 to 100%)
   LFO type (sine, tri, saw, square)
   LPF  Q : ﬁ xed at 2.0
    f  c  mod range: 100 Hz to 5 kHz
   LPF is ﬁ xed as second-order bi-quad variety
 By now, you should be getting really good at building RackAFX plug-ins. If you haven’t just
been cutting and pasting the code then you will have no problems with the next chapters. We
can use the digital resonant LPF from  Chapter 6  along with the built-in wave table oscillator
object to quickly implement the mod ﬁ lter effect. This project will use two built-in objects:
1.
2.
   CBiquad for the ﬁ lter
   CWaveTable for the LFO
Modulated Filter Effects  413
x(n)
LPF
y(n)
fc
LFO
 Figure 12.4:    The mod ﬁ lter block diagram.
 You used CBiquad in  Chapter 6  to begin your ﬁ ltering work. The CWaveTable object was
taken directly from your WTOscillator object. You can ﬁ nd the deﬁ nition and implementation
in the pluginconstants.h and pluginonbjects.cpp ﬁ les.  Table 12.1  shows the object’s members
and attributes. The ﬂ ow chart for processAudioFrame() is shown in  Figure 12.5 .
  12.1.1  Project: ModFilter
 Create the project; because we are using built-in objects for the ﬁ lter and LFO there are no
member objects to add.
   12.1.2  ModFilter GUI
 For the initial design, you will need the following slider controls in  Table 12.2 . Note that
these LFO waveform enumerations follow exactly that of the built-in CWaveTable object to
make mapping the button control easy.
   12.1.3  ModFilter.h File
 Declare instances of the BiQuad and WaveTable objects. You don’t need to  #include anything
since these are built-in:
  // Add your code here: ------------------------------------------------------- //
  // BiQuad Objects
   CBiquad m_LeftLPF;
   CBiquad m_RightLPF;
  // one LFO for the fc
   CWaveTable m_fc_LFO;
  // END OF USER CODE ---------------------------------------------------------- //
 We are also going to need some variables and functions to control the effect. Speciﬁ cally, we need:
•
•
•
•
   Minimum  f  c  variable
   Maximum  f  c  variable
   Function to calculate the current  f  c  given a LFO value
   Functions to calculate the bi-quad coefﬁ cients for left and right channels independently
(we want to calculate them separately for the third part of the project where we allow for
quad-phase LFOs)

414  Chapter 12
 Table 12.1:    The CWaveTable object interface.
Member Variables
ﬂ oat m_SinArray[1024];
ﬂ oat m_SawtoothArray[1024];
ﬂ oat m_TriangleArray[1024];
ﬂ oat m_SquareArray[1024];
ﬂ oat m_SawtoothArray_BL5[1024];
ﬂ oat m_TriangleArray_BL5[1024];
ﬂ oat m_SquareArray_BL5[1024];
ﬂ oat m_fReadIndex;
ﬂ oat m_fQuadPhaseReadIndex;
ﬂ oat m_f_inc;
int  m_nSampleRate;
ﬂ oat m_fFrequency_Hz;
bool m_bInvert;
UINT m_uOscType;
enum{sine,saw,tri,square};
UINT m_uTableMode;
enum{normal,bandlimit};
UINT m_uPolarity;
enum{bipolar,unipolar};
Member Functions
CWaveTable
Purpose
Arrays to store the non-band limited wave tables
Arrays to hold the band-limited wave tables
Indices for reading the wave table
Table increment value for current
output frequency
Sample rate
IMPORTANT: Needed for calculation of table inc; do not
forget it!
Oscillator frequency
Flag to invert output
Enumerated unsigned integer type (UINT) for osc
waveform type
Enumerated UINT for table mode (we will use
normal mode in this plug-in since the oscillator is
a LFO)
Enumerated UINT for osc polarity (we will use
unipolar for this plug-in)
void doOscillate(ﬂ oat* pYn, ﬂ oat* pYqn)
The oscillate function
Parameters:
•  float* pYn
•  ﬂ oat* pYqn
bool prepareForPlay()
Output: pYn is the normal output
Output: pYqn is the quadrature phase output
The prepareForPlay() function for the oscillator;
same use as a normal plug-in
void reset ()
Reset the pointers to top
void setSampleRate(int nSampleRate)
Called by parent to set the sample rate in Hz
Parameters:
•  int nSampleRate
void cookFrequency()
Input: the current sample rate
Calculates the new inc value for a changed oscillation
frequency
Modulated Filter Effects  415
Generate  LFO
Value
Calculate New
Mod Frequency
Based on LFO
Calculate Filter
Coefficients
Do Bi-Quad Filter
 Figure 12.5:    The ﬂ owchart for the mod ﬁ lter process function.
  Table 12.2:    GUI controls for the ModFilter.
Slider Property
Control Name
Units
Variable Type
Variable Name
Low Limit
High Limit
Initial Value
Value
Mod Depth fc
%
ﬂ oat
m_fMod_Depth_fc
0
100
50
Slider Property
Control Name
Units
Variable Type
Variable Name
Enum String
Slider Property
Value
Mod Rate fc
Hz
ﬂ oat
m_fModRate_fc
0.2
10
1.0
Control Name
Units
Variable Type
Variable Name
Low Limit
High Limit
Initial Value
Value
LFO
enum
m_uLFO_waveform
sine,saw,tri,square
 Add the following to the .h ﬁ le:
    // Add your code here: ------------------------------------------------------- //
    // BiQuad Objects
    CBiquad m_LeftLPF;
    CBiquad m_RightLPF;
    // one LFO for the fc
    CWaveTable m_fc_LFO;
    // min and max values (to make it easy to change later)
     ﬂ oat m_fMinCutoffFreq;
     ﬂ oat m_fMaxCutoffFreq;
    // function to calculate the fc given the LFO sample
     ﬂ oat calculateCutoffFreq(ﬂ oat fLFOSample);
    // calculate the coeffs for a given ﬁ lter
     void calculateLPFCoeffs(ﬂ oat fCutoffFreq, ﬂ oat fQ, CBiquad* pFilter);
    // END OF USER CODE ---------------------------------------------------------- //

416  Chapter 12
   12.1.4  ModFilter.cpp File
  Constructor
•
   Initialize the rate min and max values. The objects are self initializing upon creation.
  CModFilter::CModFilter()
  {
<SNIP SNIP SNIP>
// Finish initializations here
// set our Min and Max Modulation points
m_fMinCutoffFreq = 100.0;
m_fMaxCutoffFreq = 5000.0;
  }
 Write the calculateCutoffFrequency() function. This function behaves exactly like the
function in the ﬂ anger to calculate a new delay offset given the LFO sample and mod depth
values. Here, we simply calculate the cutoff frequency between the min and max values
according to the LFO sample and depth.
  ﬂ oat CModFilter::calculateCutoffFreq(ﬂ oat fLFOSample)
  {
return (m_fMod_Depth_fc/100.0)*(fLFOSample*(
 m_fMaxCutoffFreq − m_fMinCutoffFreq)) + m_fMinCutoffFreq;
  }
 Next, we need to implement the cooking function for the LPFs to set the new
coefﬁ cients for a given  f  c  and  Q  value. This is identical to the resonant LPF you designed
in  Chapter 6 :
  void CModFilter::calculateLPFCoeffs(ﬂ oat fCutoffFreq, ﬂ oat fQ, CBiquad* pFilter)
  {
// use same terms as book
ﬂ oat theta_c = 2.0*pi*fCutoffFreq/(ﬂ oat)m_nSampleRate;
ﬂ oat d = 1.0/fQ;
ﬂ oat fBetaNumerator =  1.0 − ((d/2.0)*(sin(theta_c)));
ﬂ oat fBetaDenominator = 1.0 + ((d/2.0)*(sin(theta_c)));
ﬂ oat fBeta = 0.5*(fBetaNumerator/fBetaDenominator);
ﬂ oat fGamma = (0.5 + fBeta)*(cos(theta_c));
ﬂ oat fAlpha = (0.5 + fBeta − fGamma)/2.0;
// apply to ﬁ lter
pFilter->m_f_a0 = fAlpha;
pFilter->m_f_a1 = 2.0*fAlpha;

Modulated Filter Effects  417
pFilter->m_f_a2 = fAlpha;
 pFilter->m_f_b1 = -2.0*fGamma;
pFilter->m_f_b2 = 2.0*fBeta;
  }
   prepareForPlay()
•
•
   Flush delays in LPFs.
   Set up the LFOs by setting the initial rate and LFO type (the LFO type will match our
LFO_waveform variable).
  bool __stdcall CModFilter::prepareForPlay()
  {
// Add your code here:
//
// Flush the LPFs
m_LeftLPF.ﬂ ushDelays();
m_RightLPF.ﬂ ushDelays();
// Note we do NOT need to init the cutoff and Q;
// these are done in processAudioFrame()
// setup LFO
m_fc_LFO.m_fFrequency_Hz = m_fModRate_fc;
m_fc_LFO.m_uPolarity = 1;  // 0 = bipolar, 1 = unipolar
m_fc_LFO.m_uTableMode = 0; // normal, no band limiting
m_fc_LFO.m_uOscType = m_uLFO_Waveform; // our own variable
m_fc_LFO.setSampleRate(m_nSampleRate); // really important!
// the LFO prepareForPlay() calls reset() and cookFrequency()
m_fc_LFO.prepareForPlay();
return true;
  }
   userInterfaceChange()
•
•
•
   If the user moves the rate slider, we need to change the LFO rate.
   If the user clicks on a radio button, we need to change the LFO waveform type.
   For now, simply recalculate everything  when any control is moved; you can streamline
this later.
  bool __stdcall CModFilter::userInterfaceChange(int nControlIndex)
  {
// brute force update all
m_fc_LFO.m_fFrequency_Hz = m_fModRate_fc;
m_fc_LFO.m_uOscType = m_uLFO_Waveform;

418  Chapter 12
// cook to calculate
m_fc_LFO.cookFrequency();
return true;
  }
   processAudioFrame()
•
•
•
•
   Calculate a new LFO value.
   Use the LFO value to calculate a new  f  c  value.
   Use the  f  c  value to calculate new ﬁ lter coefﬁ cients.
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
// call the LFO function; we only need ﬁ rst output
m_fc_LFO.doOscillate(&fYn, &fYqn);
// use the LFO value to calculate the updated fc value
ﬂ oat fc = calculateCutoffFreq(fYn);
// use the fc value and a preset Q = 2.0
// to calculate the LPF coefﬁ cients for each channel
calculateLPFCoeffs(fc, 2.0, &m_LeftLPF);
calculateLPFCoeffs(fc, 2.0, &m_RightLPF);
// do the BiQuad operation on the Left LPF
pOutputBuffer[0] = m_LeftLPF.doBiQuad(pInputBuffer[0]);
// Mono-In, Stereo-Out (AUX Effect)
if(uNumInputChannels == 1 && uNumOutputChannels == 2)
 pOutputBuffer[1] = pOutputBuffer[0];  // just copy
// Stereo-In, Stereo-Out (INSERT Effect)
if(uNumInputChannels == 2 && uNumOutputChannels == 2)
 pOutputBuffer[1] = m_RightLPF.doBiQuad(pInputBuffer[1]);
return true;
  }
 Build and test the plug-in. You will hear the  f  c  modulated by the LFO; adjust the LFO rate and
depth controls and try changing the LFO waveform type as well; triangle and sine seem to be
the most common choices, and the saw produces an interesting pulsing effect.

Modulated Filter Effects  419
     12.2    Design a Mod Filter Plug-In: Part II, Modulated f c  , Q
 In the second design, we will modify the current plug-in to include the modulation of the
LPF  Q  value. We will use a second, independent LFO for the  Q  but will share the same LFO
waveform type with the  f  c  LFO. The block diagram is shown in  Figure 12.6 .
 The parameters are as follows:
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
    f  c  mod range: 100 Hz to 5 kHz
    Q  mod range: 0.5 to 10
   LPF is ﬁ xed as second-order bi-quad
  12.2.1  ModFilter GUI
 Add more sliders to the user interface (UI) for the new LFO as per  Table 12.3 .
   Table 12.3:    Additional controls for the second LFO.
Slider Property
Value
Slider Property
Value
Control Name
Units
Variable Type
Variable Name
Low Limit
High Limit
Initial Value
Mod Depth Q
%
ﬂ oat
m_fMod_Depth_Q
0
100
50
Control Name
Units
Variable Type
Variable Name
Low Limit
High Limit
Initial Value
Mod Rate Q
Hz
ﬂ oat
m_fModRate_Q
0.2
10
0.5
   12.2.2  ModFilter.h File
 We need to add the following new functions and variables, basically just duplicating the ones
for the  f  c  modulation:
•
•
•
   LFO object for  Q  modulation
   calculateQ() to calculate the new  Q  value from the LFO sample value
   Min and max  Q  values
  // Add your code here: -------------------------------------------------------- //
  CBiquad m_LeftLPF;
  CBiquad m_RightLPF;

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
