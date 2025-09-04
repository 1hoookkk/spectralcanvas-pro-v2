# Audio Plugins C++ (Pirkle) — Chunk 64/73

> Source: `Designing Audio Effect Plug-Ins in C++_ With Digital Audio Signal Processing Theory - PDF Room.pdf` · Pages ~1-560 · Extractor: pdfminer.six
> Target ≈ 2800 words, Overlap = 200 words · Actual = 2411

> Note: DSP textbook content for SpectralCanvasPro RT-audio knowledge base

---
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

Amplitude
Dynamics Processing  469
t
 Figure 13.8:    The detector output with the medium attack and medium decay times.
x(n)
Input
z D –
d(n)
DC A
Out
y(n)
Output
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
Threshold
d o
Ratio  Knee
ρ
W
 Figure 13.9:    The look-ahead compressor block diagram.
look-ahead technique. We can’t look ahead into the future of a signal, but we can delay the
present. If we insert a delay line in the forward signal path (not the side-chain) we can make
up for the detector delay so that the detector begins charging before the signal actually gets to
the DCA, as shown in  Figure 13.9 .
 You can use the CDelay object that comes as a “stock reverb object” and add it to your project
now for our look-ahead pre-delay. Do this by using the Edit button and edit the project. Check
the box marked “Stock Reverb Objects” and then hit OK. Your compiler will ask you if you
want to reload the project, so answer yes. You will then have the reverb objects included in
your project. We’ll add a single new slider to control the look-ahead time. Remember when
using the CDelay-based objects that you need to make sure you initialize the object to have
the same maximum delay time as your slider. The amount of look-ahead delay will depend on
the compressor settings. We’ll make the maximum value match our maximum attack time of
300 mSec.
  13.3.1  DynamicsProcessor: GUI
 Add a new slider for the look-ahead delay time in mSec using  Table 13.3 .
470  Chapter 13
    Table 13.3:    The look-ahead slider properties.
Slider Property
Control Name
Units
Variable Type
Variable Name
Low Limit
High Limit
Initial Value
Value
Look-Ahead
mSec
ﬂ oat
m_fLookAheadDelay_mSec
0
300
0
   13.3.2  DynamicsProcessor.h File
 Declare the delays for the look-ahead:
  // Add your code here: ------------------------------------------------------ //
  // envelope detectors
  CEnvelopeDetector m_LeftDetector;
  CEnvelopeDetector m_RightDetector;
  <SNIP SNIP SNIP>
  // Delay lines for a look-ahead compressor
   CDelay m_LeftDelay;
   CDelay m_RightDelay;
  // END OF USER CODE --------------------------------------------------------- //
   13.3.3  DynamicsProcessor.cpp File
  prepareForPlay()
(cid:129)
(cid:129)
(cid:129)
   Initialize the delays now that we know the sample rate.
   Set the current delay time in mSec.
   Flush the delays.
  bool __stdcall CDynamicsProcessor::prepareForPlay()
  {
          // init lookahead delays
           m_LeftDelay.init(0.3*(ﬂ oat)m_nSampleRate);
           m_RightDelay.init(0.3*(ﬂ oat)m_nSampleRate);
          // set the current value
           m_LeftDelay.setDelay_mSec(m_fLookAheadDelay_mSec);
           m_RightDelay.setDelay_mSec(m_fLookAheadDelay_mSec);
          // ﬂ ush delays
           m_LeftDelay.resetDelay();
           m_RightDelay.resetDelay();
          etc…

Dynamics Processing  471
   userInterfaceChange()
(cid:129)
(cid:129)
   Set the look-ahead delay time on the delay elements.
   Note: Make sure your control IDs match your GUI.
  bool __stdcall CDynamicsProcessor::userInterfaceChange(int nControlIndex)
  {
          <SNIP SNIP SNIP>
                   case 7:
                   {
                            m_LeftDelay.setDelay_mSec(m_fLookAheadDelay_mSec);
         m_RightDelay.setDelay_mSec(m_fLookAheadDelay_mSec);
         break;
                   }
          etc…
   processAudioFrame()
(cid:129)
   Delay the signal going to the DCA, not the detector side-chain.
  bool __stdcall CDynamicsProcessor::processAudioFrame( ﬂ oat* pInputBuffer, ﬂ oat*
  pOutputBuffer,
  UINT uNumInputChannels,
  UINT uNumOutputChannels)
  {
          <SNIP SNIP SNIP>
          // delay the left DCA after gain calculation
           ﬂ oat fLookAheadOut = 0;
           m_LeftDelay.processAudio(&pInputBuffer[0], &fLookAheadOut);
          // form left output and apply make up gain
           pOutputBuffer[0] = fGn*fLookAheadOut*fOutputGain;
                  <SNIP SNIP SNIP>
                  // delay the right DCA after gain calculation
                   ﬂ oat fLookAheadOut = 0;
                   m_RightDelay.processAudio(&pInputBuffer[1], &fLookAheadOut);
                   // form right output and apply make up gain
                   pOutputBuffer[1] = fGn*fLookAheadOut*fOutputGain;
                  // set the meter to track 1-gain value
                  m_fGRMeterValue_R = 1.0 - fGn;
          }
          return true;
    }
 Build and test the plug-in now and try the look-ahead feature. It will take some critical listening
to adjust the look-ahead value; start by matching the attack time, and then tweak it from there.
472  Chapter 13
     13.4    Stereo-Linking the Dynamics Processor
 You can see from the block diagram and your own code that our stereo dynamics processor
shares all of the controls: gains, attack, release, threshold, and ratio. However, the ultimate
value of the dynamic gain factor  G ( n ) really depends on the audio content of each channel,
which is fed into the independent left and right detectors. If the left and right channels
have very different signals, then the resulting very different  G ( n ) values for left and right
can create a confusing output. Stereo-linking a dynamics processor means that you tie
together the two detector outputs, sum and average them, and then apply that one signal
to the log converter. The resulting  G ( n ) value is applied to both left and right channels.
 Figure 13.10  shows how the right channel’s gain computation has been removed since it
now shares the left channel’s side-chain path. In order to modify the existing processor, we
need to do two things: modify the UI to add a stereo link switch and add more branching in
processAudioFrame() to do the linking.
  13.4.1 DynamicsProcessor: GUI
 For a change, I’ll use a slider control in enum mode to make a switch for the stereo link.
Remember that you can use sliders to make enumerated-list variables in addition to the
buttons. Use  Table 13.4  to set up the slider.
Left Input
x(n)  left
DCA
Out
Left Output
y(n)  left
In
Detector
d(n)  left
0.5
0.5
Attack
Release
Σ
log
Conversion
gain calc
(dB)
lin
Conversion
(cid:42)(cid:11)(cid:81)(cid:12)
Threshold  Ratio
ρ
do
Right Input
x(n) right
d(n) right
In
Detector
Attack
Release
G(n)(cid:3)(cid:3)
y(n) right
DCA
Out
Right Output
 Figure 13.10:    The stereo-linked dynamics processor.
 NOTE: The detectors are running in log output mode. This means that we need to convert their
values back to linear before summing them. After they are summed, they must be converted
back to the log domain prior to being used in the gain calculation function.
Dynamics Processing  473
  Table 13.4:    The stereo link slider properties.
Slider Property
Control Name
Variable Type
Variable Name
Enum String
Value
Stereo Link
enum
m_uStereoLink
ON, OFF
   13.4.2  DynamicsProcessor.cpp File
 There are many different ways to do this branching and you should deﬁ nitely try to
ﬁ gure out your own way. My code is for education purposes and not very streamlined
either. For the branching code, I chose to set up the stereo link and right channel detection
early on.
  bool __stdcall CDynamicsProcessor::processAudioFrame( ﬂ oat* pInputBuffer, ﬂ oat*
  pOutputBuffer,
  UINT uNumInputChannels,
  UINT uNumOutputChannels)
  {
  // calculate gains
  ﬂ oat fInputGain = pow(10.0, m_fInputGain_dB/20.0);
  ﬂ oat fOutputGain = pow(10.0, m_fOutputGain_dB/20.0);
  // setup stereo link
   ﬂ oat xn_L = fInputGain*pInputBuffer[0];
   ﬂ oat xn_R = 0.0; // for later
  // do left
   ﬂ oat fLeftDetector = m_LeftDetector.detect(xn_L);
   ﬂ oat fRightDetector = fLeftDetector; // use in case of mono ﬁ le
  // check for right side; can use again in right side code
   if(uNumOutputChannels == 2)
   {
  // get the right side
   xn_R = fInputGain*pInputBuffer[1];
  // detect it
   fRightDetector = m_RightDetector.detect(xn_R);
   }
  // start with Left
   ﬂ oat fLinkDetector = fLeftDetector;
   ﬂ oat fGn = 1.0;
  if(m_uStereoLink == ON) // this works even with mono ﬁ les
  {

474  Chapter 13
  // detectors output log values; convert to linear to sum them
   fLinkDetector = 0.5*(po w(10.0, fLeftDetector/20.0) + pow(10.0,
   fRightDetector/20.0));
  // convert back to log
   fLinkDetector = 20.0*log10(fLinkDetector);
  }
  // branch
  if(m_uProcessorType == COMP)
                       fGn = calcCompressorGain(  fLinkDetector , m_fThreshold, m_fRatio,
  else if( m_uProcessorType == LIMIT)
  m_fKneeWidth, false);
  fGn = calcCompressorGain(  fLinkDetector , m_fThreshold, m_fRatio,
  m_fKneeWidth, true);
   etc… same for other branches
  // delay the left DCA
  ﬂ oat fLookAheadOut = 0;
  m_LeftDelay.processAudio(&pInputBuffer[0], &fLookAheadOut);
  <SNIP SNIP SNIP>
  // Stereo-In, Stereo-Out (INSERT Effect)
  if(uNumInputChannels == 2 && uNumOutputChannels == 2)
  {
   ﬂ oat fGn = 1.0;
  // if not linked, overwrite the variable
   if(m_uSt ereoLink == OFF)
   fLinkDetector = fRightDetector;
  // branch
  if(m_uPr ocessorType == COMP)
  fGn = calcCompressorGain( fLinkDetector , m_fThreshold, m_fRatio,
                                m_fKneeWidth, false);
  else if( m_uProcessorType == LIMIT)
  fGn = calcCompressorGain( fLinkDetector , m_fThreshold, m_fRatio,
                                m_fKneeWidth, true);
  else if( m_uProcessorType == EXPAND)
  fGn = calcDownwardExpanderGain(fLinkDetector, m_fThreshold,
   etc… same for other branches
  // delay the righr DCA
  ﬂ oat fLookAheadOut = 0;
  m_RightDelay.processAudio(&pInputBuffer[1], &fLookAheadOut);
  etc…

Dynamics Processing  475
    13.5    Design a Spectral Compressor/Expander Plug-In
 In this project, you will design a spectral dynamics processor. A spectral processor splits
up the input signal into two or more frequency bands, then applies signal processing
independently to each band. Finally, the processed ﬁ ltered outputs are summed to produce the
ﬁ nal output. In this way, you can apply signal processing to only one band of frequencies, or
apply different types of processing to different bands. In this design, we will create a two-
band spectral compressor/expander. We will use complementary low-pass ﬁ lter (LPF) and
high-pass ﬁ lter (HPF) units to split the incoming audio into two bands: low-frequency (LF)
and high-frequency (HF). Then, we will process each band independently and recombine
the outputs as shown in  Figure 13.11 . You can compress, say, only the HF content to squash
cymbals and other sibilance. Or you can smooth out the bass by compressing it slightly, or
any combination of both that you like.
 Take a look at  Figure 13.8  and check out the features. The input LPF and HPF are adjusted
with a single  f  c  control. This means their cut-off points are always overlapping. The outputs
are processed through two independent dynamics processors with independent make-up
gains, then the results are summed back together. This means the UI is going to contain a lot
of sliders. But, RackAFX will be taking care of most of the GUI details. We need to focus on
the input ﬁ lter-bank ﬁ rst.
 For the input LPF and HPF we will be using a special variant on the Butterworth ﬁ lter
called the Linkwitz–Riley ﬁ lter ( Equation 13.3 ). It has the same second-order roll-off as the
Butterworth, but the cutoff point is −6 dB rather than −3 dB so that recombining the ﬁ ltered
signals results in a ﬂ at response. Remember that ﬁ lters alter the phase response as well, so
recombining them in parallel offers a special problem with how the phases recombine.  For the
two signals to recombine properly, you must invert one of the ﬁ lter outputs . It doesn’t matter
whether you invert the LPF or HPF output as long as you invert one of them. This is speciﬁ c
to the Linkwitz-Riley ﬁ lters. The ﬁ lter design equations are shown from  Chapter 6 .
Attack
Release  Threshold  Ratio  Knee
HG I
HPF
Dynamics  Processor
H G O
x(n)
fc
Σ
y(n)
LG  I
LPF
Dynamics  Processor
L G O
Attack
Release  Threshold   Ratio  Knee
 Figure 13.11:    A two-band spectral compressor.
476  Chapter 13
      LPF
  uc 5 p fc / fs
Vc 5 pfc
     HPF
  uc 5 pfc/fs
  Vc 5 pfc
2 1 2kVc
a0 5
      k 5
Vc
 tan (uc)
    d 5 k2 1 Vc
2
Vc
d
2
Vc
d
2
Vc
d
22k2 1 2Vc
2
d
a1 5 2
b1 5
a2 5
2 1 2kVc
   k 5
 a0 5
Vc
 tan (uc)
   d 5 k2 1 Vc
k2
d
22k2
d
k2
d
22k2 1 2Vc
2
d
 a1 5
b1 5
a2 5
(13.3)
b2 5
22kVc 1 k2 1 Vc
2
d
b2 5
22kVc 1 k2 1 Vc
2
d
 Because these are second-order ﬁ lters, we can use RackAFX’s built-in bi-quad to do the
ﬁ ltering; all we need to do is supply the calculation of the coefﬁ cients. Thus, we’re going to need
to design a stereo dynamics processor with independent attack, release, threshold, ratio, and
gain make-up controls. We will need a shared  f  c   control for the two ﬁ lters. Although this plug-in
shares most of the features of the previous project, for simplicity there are a few modiﬁ cations:
(cid:129)
(cid:129)
   This design will not include the look-ahead function.
   The knee width will be a common shared control that both bands will use.
  13.5.1  Project: SpectralDynamics
 Create a new project named “SpectralDynamics.” Since we are not implementing the look-
ahead feature you don’t have to add the stock objects.
   13.5.2  SpectralDynamics: GUI
 The GUI is basically a double version of the previous design without the look-ahead function.
However, we also need to add a cutoff frequency slider to control the point where the bands
 NOTE: Unlike the last plug-in, the processor will be hard-coded in stereo link mode. We can
then run the detectors in  linear  operation mode, perform the summation, and then convert the
linked detector output to dB prior to calling the gain calculation functions. This only serves to
simplify the processAudioFrame() method a bit.
