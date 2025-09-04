# Audio Plugins C++ (Pirkle) — Chunk 63/73

> Source: `Designing Audio Effect Plug-Ins in C++_ With Digital Audio Signal Processing Theory - PDF Room.pdf` · Pages ~1-560 · Extractor: pdfminer.six
> Target ≈ 2800 words, Overlap = 200 words · Actual = 2790

> Note: DSP textbook content for SpectralCanvasPro RT-audio knowledge base

---
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

458  Chapter 13
 You’ll notice a new feature in this plug-in: the ability to monitor a signal and display it on a
meter. We will start with the compressor/limiter function and build up from there.
  13.1.1  Project: DynamicsProcessor
 Create a new project called “Dynamics Processor.” You don’t need any stock objects or other
advanced features.
   13.1.2  DynamicsProcessor: GUI
 You need to add sliders and radio button controls to implement the eight control blocks
required for the plug-in.  Table 13.1  shows the graphical user interface (GUI) slider properties
for the design.
  Table 13.1:    The GUI controls for the dynamics processor plug-in.
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
Det Gain
dB
ﬂ oat
m_fInputGain_dB
−12
20
0
Value
Attack Time
mSec
ﬂ oat
m_fAttack_mSec
1
300
20
Value
Ratio
ﬂ oat
m_fRatio
1
20
1
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
Threshold
dB
ﬂ oat
m_fThreshold
−60
0
0
Value
Release Time
mSec
ﬂ oat
m_fRelease_mSec
20
5000
1000
Value
Output Gain
dB
ﬂ oat
m_fOutputGain_dB
0
20
0
Dynamics Processing  459
Slider Property
Control Name
Units
Variable Type
Variable Name
Low Limit
High Limit
Initial Value
Button Property
Control Name
Units
Variable Type
Variable Name
Enum String
Value
Knee Width
dB
ﬂ oat
m_fKneeWidth
0
20
0
Value
Processor
enum
m_uProcessorType
COMP, LIMIT, EXPAND, GATE
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
Digital, Analog
   13.1.3  DynamicsProcessor.h File
 We need to add the following new functions and variables; most can be taken directly
from your ModFilter project. There’s not much to the basic device. You will need the
following:
(cid:129)
(cid:129)
   An envelope detector for each channel (left, right).
   A function to calculate a compression gain value, given the detector input; this func-
tion will also implement the limiting function.
  // Add your code here: ------------------------------------------------------ //
  // envelope detectors
   CEnvelopeDetector m_LeftDetector;
   CEnvelopeDetector m_RightDetector;
  // calculate the compressor G(n) value from the Detector output
   ﬂ oat calcCompressorGain(ﬂ oat fDetectorValue, ﬂ oat fThreshold, ﬂ oat fRatio,
   ﬂ oat fKneeWidth, bool bLimit);
  // END OF USER CODE --------------------------------------------------------- //
460  Chapter 13
   13.1.4 DynamicsProcessor.cpp File
 We need to implement  Equation 13.1  and also need to add the code to provide the Lagrange
interpolation when the soft-knee function is enabled in the calcCompressorGain() function.
The gain calculations simply follow  Equation 13.1 . See the Lagrange interpolation function
in the pluginconstants.h ﬁ le for documentation. You provide it with  x  and  y  arrays for the end
points along with the order (two, since we have two endpoints) and the  x  value to calculate
the interpolated  y  value.
  // calculate the compressor G(n) value from the Detector output
   ﬂ oat CDynamicsProcessor::calcCompressorGain(ﬂ oat fDetectorValue, ﬂ oat fThreshold,
   ﬂ oat fRatio, ﬂ oat fKneeWidth, bool bLimit)
  {
    // slope variable
     ﬂ oat CS = 1.0 − 1.0/fRatio;  // [ Eq. 13.1 ]
    // limiting is inﬁ nite ratio thus CS->1.0
     if(bLimit)
              CS = 1;
    // soft-knee with detection value in range?
     if(fKneeWidth > 0 && fDetectorValue > (fThreshold - fKneeWidth/2.0) &&
          fDetectorValue < fThreshold + fKneeWidth/2.0)
     {
                // setup for Lagrange
                 double x[2];
                 double y[2];
                 x[0] = fThreshold − fKneeWidth/2.0;
                 x[1] = fThreshold + fKneeWidth/2.0;
                 x[1] = min(0, x[1]);  // top limit is 0dBFS
               y[0] = 0;      // CS = 0 for 1:1 ratio
               y[1] = CS;    // current CS
                // interpolate & overwrite CS
                 CS = lagrpol(&x[0], &y[0], 2, fDetectorValue);
     }
    // compute gain; threshold and detection values are in dB
     ﬂ oat yG = CS*(fThreshold − fDetectorValue);   // [ Eq. 13.1 ]
  // clamp; this allows ratios of 1:1 to still operate
     yG = min(0, yG);
    // convert back to linear
     return pow(10.0, yG/20.0);
  }

Dynamics Processing  461
  prepareForPlay()
(cid:129)
   Initialize the detector objects; note the boolean ﬂ ags that set the analog/digital and log or
linear calculation; we use log.
  bool __stdcall CDynamicsProcessor::prepareForPlay()
  {
           // Add your code here:
           // init the envelope detectors
           // set all params at once with this function; see function deﬁ nition
            if(m_uTimeConstant == Digital)
            {
                      m_LeftDetector.init((ﬂ oat)m_nSampleRate, m_fAttack_mSec, m_fRelease_mSec,
                        false, DETECT_MODE_RMS, true);
                     m_RightDetector.init((ﬂ oat)m_nSampleRate, m_fAttack_mSec,
                                                m_fRelease_mSec, false, DETECT_MODE_RMS, true);
            }
            else
            {
                      m_LeftDetector.init((ﬂ oat)m_nSampleRate, m_fAttack_mSec, m_fRelease_mSec,
                        true, DETECT_MODE_RMS, true);
                     m_RightDetector.init((ﬂ oat)m_nSampleRate, m_fAttack_mSec,
                                                m_fRelease_mSec, true, DETECT_MODE_RMS, true);
            }
           return true;
  }
   userInterfaceChange()
(cid:129)
(cid:129)
(cid:129)
(cid:129)
   Set the new attack time on the detectors.
   Set the new release time on the detectors.
   Set the time constant mode on the detectors.
   Note: Make sure you check your variable control ID values to match your UI.
  bool __stdcall CDynamicsProcessor::userInterfaceChange(int nControlIndex)
  {
           // decode the control index, or delete the switch and use brute force calls
           switch(nControlIndex)
           {
                     case 2:
                     {
                              m_LeftDetector.setAttackTime(m_fAttack_mSec);
                              m_RightDetector.setAttackTime(m_fAttack_mSec);
                              break;
                     }
462  Chapter 13
                     case 3:
                     {
                              m_LeftDetector.setReleaseTime(m_fRelease_mSec);
                              m_RightDetector.setReleaseTime(m_fRelease_mSec);
                              break;
                     }
                     case 42:
                     {
                              if(m_uTimeConstant == Digital)
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
   processAudioFrame()
(cid:129)
(cid:129)
(cid:129)
(cid:129)
(cid:129)
   Follow the block diagram to complete the operations.
   Apply input gain to detector.
   Detect input sample.
   Calculate gain.
   Apply dynamic gain reduction and static make-up gain.
  bool __stdcall CDynamicsProcessor::processAudioFrame(ﬂ oat* pInputBuffer, ﬂ oat*
  pOutputBuffer, UINT
  uNumInputChannels, UINT
  uNumOutputChannels)
  {
              //
              // output = input -- change this for meaningful processing
              //
              // Do LEFT (MONO) Channel; there is always at least one input, one output
              // calculate gains
               ﬂ oat fInputGain = pow(10.0, m_fInputGain_dB/20.0);
               ﬂ oat fOutputGain = pow(10.0, m_fOutputGain_dB/20.0);
              // detect left channel
               ﬂ oat fLeftDetector = m_LeftDetector.detect(pInputBuffer[0]);

Dynamics Processing  463
              // gain calc
               ﬂ oat fGn = 1.0;
              // branch
               if(m_uProcessorType == COMP)
                        fGn = calcCompressorGain(fLeftDetector, m_fThreshold, m_fRatio,
m_fKneeWidth, false);
             else if(m_uProcessorType == LIMIT)
                        fGn = calcCompressorGain(fLeftDetector, m_fThreshold, m_fRatio,
                     m_fKneeWidth, true);
              // form left output and apply make up gain
               pOutputBuffer[0] = fGn*pInputBuffer[0]*fOutputGain;
            // Mono-In, Stereo-Out (AUX Effect)
              if(uNumInputChannels == 1 && uNumOutputChannels == 2)
                        pOutputBuffer[1] = pOutputBuffer[0];
              // Stereo-In, Stereo-Out (INSERT Effect)
              if(uNumInputChannels == 2 && uNumOutputChannels == 2)
              {
                     // detect right channel
                      ﬂ oat fRightDetector = m_RightDetector.detect(pInputBuffer[1]);
                     // gain calc
                      ﬂ oat fGn = 1.0;
                     // branch
                      if(m_uProcessorType == COMP)
                             fGn = calcCompressorGain(fRightDetector, m_fThreshold, m_fRatio,
                      else if(m_uProcessorType == LIMIT)
                                        m_fKneeWidth, false);
                             fGn = calcCompressorGain(fRightDetector, m_fThreshold, m_fRatio,
                                    m_fKneeWidth, true);
                     // form right output and apply make up gain
                      pOutputBuffer[1] = fGn*pInputBuffer[1]*fOutputGain;
      }
      return true;
  }
 Build and test the plug-in. Try all the controls. Test the analog and digital time constants.
Try extreme settings and listen for artifacts called “pumping and breathing.” The soft-knee
function can sometimes be subtle, so you may have to try a variety of different audio ﬁ les to
hear it. It would be nice to have a visual way of seeing the gain reduction, so we will use an

464  Chapter 13
advanced RackAFX feature that will enable a metering device. To make it even better, we
can invert the meter so that gain reduction results in a downward meter view—common in
dynamics processor devices, both analog and digital.
 In RackAFX, right click on one of the LED meters in the meter section of the main UI, shown
in  Figure 13.7 . The meters accept a value between 0.0 and 1.0 and display accordingly. This
is perfect for us since our gain values are also ﬂ uctuating on that same range. You only need
to declare a ﬂ oating point variable to set the meter; RackAFX does everything else. A dialog
box will pop up that allows you to name the metering variable you are going to attach to the
meter as well as customize its look and behavior. You will be adding two meters, one for the
left and one for the right channel gain reduction.
 GRL stands for gain reduction for the left and GRR stands for gain reduction for the right
channel. Set up the two meters according to  Table 13.2 .
Input/Send (dB)  FX Routing  Output/Return (dB )
+ 12
0
–12
–24
–36
–48
–60
Insert
1–2 Aux
2–2 Aux
L  R
Unity
+ 12
0
–12
–24
–36
–48
–60
L  R
 Figure 13.7:    The 10 assignable meters in RackAFX.
  Table 13.2:    Meter settings for the dynamics processor.
Meter Property
Value
Meter Property
Value
Meter Blurb
Invert Meter
Enable Meter
Meter Cal
Detect Mode
Meter Variable
Attack Time
Release Time
GRL
true
true
linear
PEAK
m_fGRMeterValue_L
0
0
Meter Blurb
Invert Meter
Enable Meter
Meter Cal
Detect Mode
Meter Variable
Attack Time
Release Time
GRR
true
true
linear
PEAK
m_fGRMeterValue_R
0
0
Dynamics Processing  465
  The gain reduction value we will be metering is linear, from 0 to 1.0. The gain reduction
value will have its own attack and release times according to our settings, so we want to
monitor the PEAK  variable type with no attack or release time.
 All you need to do is make sure the variables m_fGRMeterValue_L and m_fGRMeterValue_R
are reset to the current gain reduction value on each sample interval, and the meter will track
it. The meter value needs to go up as the gain reduction goes down to visualize it properly. To
set the meter, add one line of code to your processAudioFrame() function for each of the left
and right channels.
    13.1.5 DynamicsProcessor.cpp File
  processAudioFrame()
(cid:129)
   Add the metering variables to track the gain reduction values.
  bool __stdcall CDynamicsProcessor::pr ocessAudioFrame(ﬂ oat* pInputBuffer, ﬂ oat*
  {
             // Do LEFT (MONO) Channel; there is always at least one input/one output
  pOutputBuffer, UINT uNumInputChannels,
  UINT uNumOutputChannels)
             <SNIP SNIP SNIP>
           // form left output and apply make up gain
             pOutputBuffer[0] = fGn*pInputBuffer[0]*fOutputGain;
             // set the meter to track 1-gain value
              m_fGRMeterValue_L = 1.0 (cid:2) fGn;
             <SNIP SNIP SNIP>
                    // form right output and apply make up gain
                    pOutputBuffer[1] = fGn*pInputBuffer[1]*fOutputGain;
                  // set the meter to track 1-gain value
                     m_fGRMeterValue_R = 1.0 (cid:2) fGn;
             }
             etc…
 Build and test the plug-in now and you’ll get a visual cue about how much gain reduction
is taking place. Try the analog limiter setting and adjust the attack, release, and threshold
values to give smooth limiting (note the ratio slider doesn’t work in limiter mode). Once you
have that running properly, we’ll ﬁ nish off the dynamics processor by adding the downward
expansion/gating capability.

466  Chapter 13
     13.2    Design a downward expander/gate plug-in
 We’ll add on to the current project. Here is what is needed:
(cid:129)
(cid:129)
   A function to calculate downward expander gain
   Branching in the processAudioFrame() function to implement all four dynamics
 processor operations
  13.2.1  DynamicsProcessor.h File
 Add the following function declaration for the downward expander calculation:
  // Add your code here: ------------------------------------------------------ //
  // envelope detectors
  CEnvelopeDetector m_LeftDetector;
  CEnvelopeDetector m_RightDetector;
  // calcualte the compressor G(n) value from the Detector output
   ﬂ oat calcCompressorGain(ﬂ oat fDetec torValue, ﬂ oat fThreshold, ﬂ oat fRatio,
   ﬂ oat fKneeWidth, bool bLimit);
// calculate the downward expander G(n) value from the Detector output
  ﬂ oat calcDownwardExpanderGain(ﬂ oat  fDetectorValue, ﬂ oat fThreshold, ﬂ oat
  fRatio, ﬂ oat fKneeWidth, bool bGate);
// END OF USER CODE --------------------------------------------------------- //
   13.2.2  DynamicsProcessor.cpp File
 Implement the downward expander function including the soft-knee operation.
  // calculate the downward expander G(n) value from the Detector output
   ﬂ oat CDynamicsProcessor::calcDownwardExpanderGain(ﬂ  oat fDetectorValue, ﬂ oat
   fThreshold, ﬂ oat fRatio, ﬂ oat
   fKneeWidth, bool bGate)
   {
  // slope variable
   ﬂ oat ES = 1.0/fRatio − 1; // [ Eq. 13.2 ]
  // gating is inﬁ nite ratio; ES -> −1.0
   if(bGate)
   ES = ─1;
  // soft-knee with detection value in range?
   if(fKneeWidth > 0 && fDetectorValue > (fThreshold ─ fKneeWidth/2.0) &&
   fDetectorValue < fThreshold + fKneeWidth/2.0)

Dynamics Processing  467
   {
   }
  // setup for Lagrange
   double x[2];
   double y[2];
   x[0] = fThreshold − fKneeWidth/2.0;
   x[1] = fThreshold + fKneeWidth/2.0;
   x[1] = min(0, x[1]);  // top limit is 0dBFS
   y[0] = ES;   // current ES
  y[1] = 0;  // 1:1 ratio
   // interpolate the value
   ES = lagrpol(&x[0], &y[0], 2, fDetectorValue);
  // compute gain; threshold and detection values are in dB
   ﬂ oat yG = ES*(fThreshold ─ fDetectorValue);  // [ Eq. 13.2 ]
  // clamp; this allows ratios of 1:1 to still operate
   yG = min(0, yG);
  // convert back to linear
   return pow(10.0, yG/20.0);
  }
  processAudioFrame()
(cid:129)
   Similar to the compressor, add the branching code to calculate the downward expansion
gain.
  bool __stdcall CDynamicsProcessor::processAudioFrame( ﬂ oat* pInputBuffer, ﬂ oat*
  {
           // Do LEFT (MONO) Channel; there is always at least one input/one output
  pOutputBuffer,
  UINT uNumInputChannels,
  UINT uNumOutputChannels)
           <SNIP SNIP SNIP>
           // gain calc
           ﬂ oat fGn = 1.0;
           // branch
           if(m_uPr ocessorType == COMP)
  fGn = calcCompressorGain( fLeftDetector, m_fThreshold, m_fRatio,
  m_fKneeWidth, false);
           else if( m_uProcessorType == LIMIT)
  fGn = calcCompressorGain( fLeftDetector, m_fThreshold, m_fRatio,
  m_fKneeWidth, true);
            else if( m_uProcessorType == EXPAND)
                                                             m_fKneeWidth, false);
   fGn = calcDownwardExpanderGain(fLeftDetector, m_fThreshold, m_fRatio,

468  Chapter 13
            else if( m_uProcessorType == GATE)
   fGn = calcDownwardExpanderGain(fLeftDetector, m_fThreshold, m_fRatio,
                                         m_fKneeWidth, true);
           // form left output and apply make up gain
           pOutputBuffer[0] = fGn*pInputBuffer[0]*fOutputGain;
           <SNIP SNIP SNIP>
         // Stereo-In, Stereo-Out (INSERT Effect)
           if(uNumInputChannels == 2 && uNumOutputChannels == 2)
           {
                   // detect right channel
                   ﬂ oat fRightDetector = m_RightDetector.detect(pInputBuffer[1]);
                   // gain calc
                   ﬂ oat fGn = 1.0;
                   // branch
                   if(m_uPr ocessorType == COMP)
  fGn = calcCompressorGain(fRightDetector, m_fThreshold, m_fRatio,
                               m_fKneeWidth, false);
                   else if( m_uProcessorType == LIMIT)
  fGn = calcCompressorGain(fRightDetector, m_fThreshold, m_fRatio,
                               m_fKneeWidth, true);
                    else if( m_uProcessorType == EXPAND)
   fGn = calcDownwardExpanderGain( fLeftDetector, m_fThreshold,
    m_fRatio, m_fKneeWidth, false);
                    else if( m_uProcessorType == GATE)
   fGn = calcDownwardExpanderGain( fLeftDetector, m_fThreshold,
   m_fRatio, m_fKneeWidth, true);
                   // form right output and apply make up gain
                   pOutputBuffer[1] = fGn*pInputBuffer[1]*fOutputGain;
                   etc…
 Build and test the plug-in now and try all four modes with several different input ﬁ les; drums
and guitars make good test ﬁ les. The analog time constant along with the soft-knee settings
generally provide a smoother, less obtrusive gain reduction function.
     13.3    Design a Look-Ahead Compressor Plug-In
 A problem with the detector circuit is that for any non-zero attack and release time settings,
the detector output lags behind the input in time. Remember from  Chapter 12  how the output
of the detector tracks the input signal, as shown in  Figure 13.8 . The dark line is the detector
output, which is clearly lagging behind the input.
 Since the detector output lags, the gain reduction actually misses the event that needs to
be attenuated. In the digital world, we can accommodate for this detector lag by using a
