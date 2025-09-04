# Audio Plugins C++ (Pirkle) — Chunk 65/73

> Source: `Designing Audio Effect Plug-Ins in C++_ With Digital Audio Signal Processing Theory - PDF Room.pdf` · Pages ~1-560 · Extractor: pdfminer.six
> Target ≈ 2800 words, Overlap = 200 words · Actual = 2650

> Note: DSP textbook content for SpectralCanvasPro RT-audio knowledge base

---
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

Dynamics Processing  477
split and a knee-width control.  Tables 13.5  and  13.6  show all the controls you will need.
In  Table 13.5 , all of the settings are identical to the previous project so only the variable
names are shown.
  Table 13.5:    Duplicated sets of controls.
Control
Variable Name
Control
Variable Name
LF Detector gain
LF Threshold
LF Attack Time
LF Release Time
LF Ratio
LF Make Up Gain
m_LF_DetectorGain_dB
m_LF_Threshold
m_LF_Attack_mSec
m_LF_Release_mSec
m_LF_Ratio
m_LF_MakeUpGain_dB
HF Detector gain
HF Threshold
HF Attack Time
HF Release Time
HF Ratio
HF Make Up Gain
m_HF_DetectorGain_dB
m_HF_Threshold
m_HF_Attack_mSec
m_HF_Release_mSec
m_HF_Ratio
m_HF_MakeUpGain_dB
  Table 13.6 :    The two additional sliders for cutoff frequency and knee-width.
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
fc
Hz
ﬂ oat
m_fFilterBankCutoff
100
8000
1000
Control Name
Units
Variable Type
Variable Name
Low Limit
High Limit
Initial Value
Knee Width
dB
ﬂ oat
m_fKneeWidth
0
20
0
  13.5.3  Additional Slider Controls
  Note : These are duplicated sets of controls that are set up exactly like the previous project,
with the exception of the variable names.
  13.5.4  Spectral Dynamics Buttons
 You need the same two button banks as the previous project, one for the processing type and
the other for the time-constant mode.
   13.5.5  Spectral Dynamics Metering
 This plug-in will feature extensive monitoring using the LED meter bank to indicate the LF
and HF input, gain reduction, and output levels. The gain reduction meters are inverted. You
can also choose different color schemes for the meters. Other than color and inversion, the
meters are set up identically to the previous project. You will need to set up six meters to
connect to the following variables:
(cid:129)
(cid:129)
   m_fMeterLFIn
   m_fMeterHFIn
478  Chapter 13
(cid:129)
(cid:129)
(cid:129)
(cid:129)
   m_fMeterLFGr
   m_fMeterHFGr
   m_fMeterLFOut
   m_fMeterHFOut
 The metering is especially important for the spectral processing because of the differences in
energy in the bands; you will get a clearer picture of the equalization nature of this effect. The
middle two meters show the gain reduction and are colored green and red and inverted. The
input and output use the Volume Unit or “VU  meter” color scheme.  Figure 13.12  shows the
GUI after all the controls have been added.
   13.5.6  SpectralDynamics.h File
 Because the bulk of the work is in the UI code, you only need a few more components to
add to the original dynamics processor. You need the left and right detectors and the gain
calculation functions as before. You also need four bi-quads: an LPF/HPF pair for each of the
left and right channels. Finally, you need one function to calculate all the bi-quad coefﬁ cients
at once for a given  f  c  value.
  // Add your code here: ------------------------------------------------------ //
  // envelope detectors X2
   CEnvelopeDetector m_Left_LF_Detector;
FC
1000.0 Hz
HF Sens
HF Thresh
HF Attack
HF Rel
HF Ratio
0.00  dB
0.00
20.00 mS
1000 mS
1.00
HF Gain
0.00  dB
Processor
COMP
LIMIT
EXPAND
GATE
Knee Width
LF Sens
LF Thresh
LF Attack
LF Rel
LF Ratio
0.00
0.00  dB
0.00
20.00 mS
1000 mS
1.00
LF Gain
0.00  dB
T.C.
Digital
Analog
 Figure 13.12:    The ﬁ nal SpectralDynamics GUI sliders, buttons, and meters.
LFI  HFI
LR  HR
LFO HFO
Dynamics Processing  479
   CEnvelopeDetector m_Right_LF_Detector;
   CEnvelopeDetector m_Left_HF_Detector;
   CEnvelopeDetector m_Right_HF_Detector;
  // calculate the compressor G(n) value from the Detector output
   ﬂ oat calcCompressorGain(ﬂ oat fDetectorValue, ﬂ oat fThreshold, ﬂ oat fRatio);
// calculate the downward expander G(n) value from the Detector output
   ﬂ oat calcDownwardExpanderGain( ﬂ oat fDetectorValue, ﬂ oat fThreshold, ﬂ oat
    fRatio);
  // our input ﬁ lter banks X2
    CBiquad m_LeftLPF;
    CBiquad m_LeftHPF;
   CBiquad m_RightLPF;
   CBiquad m_RightHPF;
  // function to set all the ﬁ lter cutoffs
   void calculateFilterBankCoeffs(ﬂ oat fCutoffFreq);
  // END OF USER CODE --------------------------------------------------------- //
 The detector variables are the same as your previous dynamics processor but are split into LF
and HF sections or bands.
   13.5.7  SpectralDynamics.cpp File
 The calcCompressorGain() and calcDownwardExpanderGain() functions are  identical  to the
last project you did, so you can cut and paste them. The calculateFilterBankCoeffs() function
is pretty straightforward and follows the design equations for the Linkwitz–Riley ﬁ lters. Since
all the ﬁ lters share the same cutoff, they are pretty easy to deal with.
  // all ﬁ lters have same cutoff frequency; only need LPF and HPF calcs
   void CSpectralDynamics::calculateFilterBankCoeffs(ﬂ oat fCutoffFreq)
  {
  // Shared Factors:
   ﬂ oat omega_c = pi*fCutoffFreq;
   ﬂ oat theta_c = pi*fCutoffFreq/(ﬂ oat)m_nSampleRate;
   ﬂ oat k = omega_c/tan(theta_c);
   ﬂ oat k_squared = k*k;
 ﬂ oat omega_c_squared = omega_c*omega_c;
   ﬂ oat fDenominator = k_squared + omega_c_squared + 2.0*k*omega_c;

480  Chapter 13
   ﬂ oat fb1_Num = -2.0*k_squared + 2.0*omega_c_squared;
   ﬂ oat fb2_Num = -2.0*k*omega_c + k_squared + omega_c_squared;
  // the LPF coeffs
   ﬂ oat a0 = omega_c_squared/fDenominator;
   ﬂ oat a1 = 2.0*omega_c_squared/fDenominator;
   ﬂ oat a2 = a0;
   ﬂ oat b1 = fb1_Num/fDenominator;
   ﬂ oat b2 = fb2_Num/fDenominator;
  // set the LPFs
   m_LeftLPF.m_f_a0 = a0;
   m_LeftLPF.m_f_a1 = a1;
   m_LeftLPF.m_f_a2 = a2;
   m_LeftLPF.m_f_b1 = b1;
   m_LeftLPF.m_f_b2 = b2;
   // right
   m_RightLPF.m_f_a0 = a0;
   m_RightLPF.m_f_a1 = a1;
   m_RightLPF.m_f_a2 = a2;
   m_RightLPF.m_f_b1 = b1;
   m_RightLPF.m_f_b2 = b2;
  // the HPF coeffs
   a0 = k_squared/fDenominator;
   a1 = -2.0*k_squared/fDenominator;
   a2 = a0;
   b1 = fb1_Num/fDenominator;
   b2 = fb2_Num/fDenominator;
  // set the HPFs
   m_LeftHPF.m_f_a0 = a0;
   m_LeftHPF.m_f_a1 = a1;
   m_LeftHPF.m_f_a2 = a2;
   m_LeftHPF.m_f_b1 = b1;
   m_LeftHPF.m_f_b2 = b2;
  // right
   m_RightHPF.m_f_a0 = a0;
   m_RightHPF.m_f_a1 = a1;
   m_RightHPF.m_f_a2 = a2;
   m_RightHPF.m_f_b1 = b1;
   m_RightHPF.m_f_b2 = b2;
  }
  prepareForPlay()
(cid:129)
(cid:129)
(cid:129)
   Flush delays in all ﬁ lters.
   Calculate the initial setting of the ﬁ lters.
   Init the detectors—identical to the previous project except that there are more of them.

Dynamics Processing  481
  bool __stdcall CSpectralDynamics::prepareForPlay()
  {
        // Add your code here:
        // Flush the ﬁ lters
         m_LeftLPF.ﬂ ushDelays();
         m_LeftHPF.ﬂ ushDelays();
         m_RightLPF.ﬂ ushDelays();
         m_RightHPF.ﬂ ushDelays();
        // calculate the Coeffs all at once!
         calculateFilterBankCoeffs(m_fFilterBankCutoff);
        // init detectors
        // set all params at once with this function; false = Digital Time Constant NOT
         // NOTE: Setting detector for linear operaton so we can sum the results;
        //           we have to convert back to log manually
         if(m_uTimeConstant == Digital)
         {
                    m_Left_LF_Detector.init((ﬂ oat )m_nSampleRate, m_LF_Attack_mSec,
   m_LF_Release_mSec, false, DETECT_MODE_RMS ,
   false);
                    m_Right_LF_Detector.init((ﬂ oa t)m_nSampleRate, m_LF_Attack_mSec,
   m_LF_Release_mSec, false, DETECT_MODE_RMS,
   false);
                    m_Left_HF_Detector.init((ﬂ oat )m_nSampleRate, m_HF_Attack_mSec,
   m_HF_Release_mSec, false, DETECT_MODE_RMS,
   false);
                    m_Right_HF_Detector.init((ﬂ oa t)m_nSampleRate, m_HF_Attack_mSec,
   m_HF_Release_mSec, false, DETECT_MODE_RMS,
   false);
         }
         else
         {
                    m_Left_LF_Detector.init((ﬂ oa t)m_nSampleRate, m_LF_Attack_mSec,
   m_LF_Release_mSec, true, DETECT_MODE_RMS,
   false);
                    m_Right_LF_Detector.init((ﬂ o at)m_nSampleRate, m_LF_Attack_mSec,
   m_LF_Release_mSec, true, DETECT_MODE_RMS,
   false);
                    m_Left_HF_Detector.init((ﬂ oa t)m_nSampleRate, m_HF_Attack_mSec,
   m_HF_Release_mSec, true, DETECT_MODE_RMS,
   false);
                    m_Right_HF_Detector.init((ﬂ o at)m_nSampleRate, m_HF_Attack_mSec,
   m_HF_Release_mSec, true, DETECT_MODE_RMS,
   false);
         }
        return true;
  }

482  Chapter 13
   userInterfaceChange()
(cid:129)
(cid:129)
   Decode the nControlIndex and apply changes to ﬁ lter and detectors.
   Basically the same as the last project except the addition of the ﬁ lter and the fact that
there are four times as many detectors.
  bool __stdcall CSpectralDynamics::userInterfaceChange(int nControlIndex)
  {
    // decode the control index, or delete the switch and use brute force calls
    switch(nControlIndex)
    {
      // ﬁ lter cutoff
       case 0:
       {
           calculateFilterBankCoeffs(m_fFilterBankCutoff);
           break;
       }
      // HF Attack
       case 3:
       {
           m_Left_HF_Detector.setAttackTime(m_HF_Attack_mSec);
           m_Right_HF_Detector.setAttackTime(m_HF_Attack_mSec);
           break;
       }
      // LF Attack
       case 13:
       {
           m_Left_LF_Detector.setAttackTime(m_LF_Attack_mSec);
           m_Right_LF_Detector.setAttackTime(m_LF_Attack_mSec);
           break;
       }
      // HF Release
       case 4:
       {
           m_Left_HF_Detector.setReleaseTime(m_HF_Release_mSec);
           m_Right_HF_Detector.setReleaseTime(m_HF_Release_mSec);
           break;
       }
      // LF Release
       case 14:
       {
           m_Left_LF_Detector.setReleaseTime(m_LF_Release_mSec);
           m_Right_LF_Detector.setReleaseTime(m_LF_Release_mSec);
           break;
       }

Dynamics Processing  483
      // Time Constant
       case 42:
       {
         if(m_uTimeConstant == Digital)
         {
               m_Left_HF_Detector.setTCModeAnalog(false);
               m_Right_LF_Detector.setTCModeAnalog(false);
               m_Right_HF_Detector.setTCModeAnalog(false);
               m_Right_LF_Detector.setTCModeAnalog(false);
         }
         else
         {
               m_Left_HF_Detector.setTCModeAnalog(true);
               m_Right_LF_Detector.setTCModeAnalog(true);
               m_Right_HF_Detector.setTCModeAnalog(true);
               m_Right_LF_Detector.setTCModeAnalog(true);
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
(cid:129)
   Calculate input/output gain values from their dB UI versions.
   Filter the input into LF and HF components;  invert one ﬁ lter’s output .
   Detect the LF and HF components separately.
   Calculate the dynamic gain factors  G ( n ) for LF and HF components.
   Implement the gain reduction for each band.
   Recombine the outputs of each band.
  bool __stdcall CSpectralDynamics::processAudioFrame(ﬂ oat* pInputBuffer, ﬂ oat*
  pOutputBuffer, UINT
  uNumInputChannels, UINT
  uNumOutputChannels)
  {
      // Do LEFT (MONO) Channel; there is always at least one input/one output
       ﬂ oat fLeftInput = pInputBuffer[0];
       ﬂ oat fRightInput = uNumInputChannels == 1 ? fLeftInput : pInputBuffer[1];
      // setup the input and output gains
       ﬂ oat fLFGain = pow(10.0, m_LF_DetectorGain_dB/20.0);
       ﬂ oat fHFGain = pow(10.0, m_HF_DetectorGain_dB/20.0);
       ﬂ oat fLFOutputGain = pow(10.0, m_LF_MakeUpGain_dB/20.0);
       ﬂ oat fHFOutputGain = pow(10.0, m_HF_MakeUpGain_dB/20.0);

484  Chapter 13
      // split the signal into m_Left LF and HF parts
       ﬂ oat fLeft_LF_Out = m_LeftLPF.doBiQuad(fLeftInput*fLFGain);
       ﬂ oat fLeft_HF_Out = m_LeftHPF.doBiQuad(fLeftInput*fHFGain);
      // invert ONE of the outputs for proper recombination
       fLeft_HF_Out *= -1.0;
      // send these to the detectors: NOTE OUTPUTS ARE LINEAR
       ﬂ oat fLeft_LF_Detector = m_Left_LF_Detector.detect(fLeft_LF_Out);
       ﬂ oat fLeft_HF_Detector = m_Left_HF_Detector.detect(fLeft_HF_Out);
      // split the signal into m_Left LF and HF parts
       ﬂ oat fRight_LF_Out = m_RightLPF.doBiQuad(fRightInput*fLFGain);
       ﬂ oat fRight_HF_Out = m_RightHPF.doBiQuad(fRightInput*fHFGain);
      // invert ONE of the outputs for proper recombination
       fRight_HF_Out *= -1.0;
      // send these to the detectors: NOTE OUTPUTS ARE LINEAR
       ﬂ oat fRight_LF_Detector = m_Right_LF_Detector.detect(fRight_LF_Out);
       ﬂ oat fRight_HF_Detector = m_Right_HF_Detector.detect(fRight_HF_Out);
      // This is the stereo linking of the detector paths;
      // The detectors were set for linear operation so we could sum them
      // they must be converted back to dB before use in the gain calculation
       ﬂ oat LFDetectorSum = 0.5*(fRight_LF_Detector, fLeft_LF_Detector);
       ﬂ oat HFDetectorSum = 0.5*(fRight_HF_Detector, fLeft_HF_Detector);
      // convert back to dB
       LFDetectorSum = 20.0*log10(LFDetectorSum);
       HFDetectorSum = 20.0*log10(HFDetectorSum);
      // sum for input metering
       m_fMeterLFIn = 0.5*(fLeft_LF_Out + fRight_LF_Out);
       m_fMeterHFIn = 0.5*(fLeft_HF_Out + fRight_HF_Out);
      // calculate the gain factors
       ﬂ oat fGn = 1.0;
      // --- LF BAND ---------------
      // branch: all are LF: detect, thresh, ratio
       if(m_uProcessorType == COMP)
                 fGn = calcCompressorGain( LFDetectorSum, m_LF_Threshold, m_LF_Ratio,
   m_fKneeWidth, false);
       else if(m_uProcessorType == LIMIT)
                 fGn = calcCompressorGain( LFDetectorSum, m_LF_Threshold, m_LF_Ratio,
       else if(m_uProcessorType == EXPAND)
                  fGn = calcDownwardExpande rGain(LFDetectorSum, m_LF_Threshold,
m_LF_Ratio,       m_fKneeWidth, false);
   m_fKneeWidth, true);

       else if(m_uProcessorType == GATE)
                  fGn = calcDownwardExpanderGain(LFDetectorSum, m_LF_Threshold,
                         m_LF_Ratio,       m_fKneeWidth, true);
Dynamics Processing  485
      // create left and right LF outputs
       ﬂ oat fLFOutputL = fGn*fLeft_LF_Out*fLFOutputGain;
       ﬂ oat fLFOutputR = fGn*fRight_LF_Out*fLFOutputGain;
      // gain reduction meter
       m_fMeterLFGr = 1.0 - fGn;
      // --- HF BAND ---------------
      // branch: all are HF: detect, thresh, ratio
       if(m_uProcessorType == COMP)
                 fGn = calcCompressorGain( HFDetectorSum, m_HF_Threshold, m_HF_Ratio,
       else if(m_uProcessorType == LIMIT)
                 fGn = calcCompressorGain( HFDetectorSum, m_HF_Threshold, m_HF_Ratio,
   m_fKneeWidth, false);
   m_fKneeWidth, true);
       else if(m_uProcessorType == EXPAND)
                  fGn = calcDownwardExpanderGain(HFDetectorSum, m_HF_Threshold,
                         m_HF_Ratio,       m_fKneeWidth, false);
       else if(m_uProcessorType == GATE)
                 fGn = calcDownwardExpanderGain(HFDetectorSum, m_HF_Threshold,
                                    m_HF_Ratio,        m_fKneeWidth, true);
      // create left and right HF outputs
       ﬂ oat fHFOutputL = fGn*fLeft_HF_Out*fHFOutputGain;
       ﬂ oat fHFOutputR = fGn*fRight_HF_Out*fHFOutputGain;
      // meter output
       m_fMeterLFOut = 0.5*(fLFOutputL + fLFOutputR);
       m_fMeterHFOut = 0.5*(fHFOutputL + fHFOutputR);
      // meter GR
       m_fMeterHFGr = 1.0 - fGn;
      // sum outputs
       ﬂ oat fLeftOutput = fLFOutputL + fHFOutputL;
       ﬂ oat fRightOutput = fLFOutputR + fHFOutputR;
      // write the outputs
       pOutputBuffer[0] = fLeftOutput;
      // Mono-In, Stereo-Out (AUX Effect)
      if(uNum InputChannels == 1 && uNumOutputChannels == 2)
   pOutputBuffer[1] = fRightOutput;
      // Stereo-In, Stereo-Out (INSERT Effect)
      if(uNum InputChannels == 2 && uNumOutputChannels == 2)
   pOutputBuffer[1] = fRightOutput;
  }
