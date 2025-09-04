# Audio Plugins C++ (Pirkle) — Chunk 66/73

> Source: `Designing Audio Effect Plug-Ins in C++_ With Digital Audio Signal Processing Theory - PDF Room.pdf` · Pages ~1-560 · Extractor: pdfminer.six
> Target ≈ 2800 words, Overlap = 200 words · Actual = 1736

> Note: DSP textbook content for SpectralCanvasPro RT-audio knowledge base

---
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

486  Chapter 13
 Build and test the plug-in. This is a complex device with many interactions. It can sound a lot
like an equalizer  because it is a very special case of one. Be careful with gain and threshold
settings.
 Ideas for future projects might include:
(cid:129)
(cid:129)
(cid:129)
(cid:129)
(cid:129)
(cid:129)
   Add separate left, right, HF, and LF meters instead of summing them.
   Add the look-ahead function.
   Split the input into three or more bands (tricky—make a band-pass ﬁ lter by cascading two
Linkwitz–Riley ﬁ lters, and be careful how you sum the band edges) and run  dynamics
processing on each band.
   Make a feed-back topology compressor.
   Add parallel compression—a dry path that mixes with the effected signal in a user- chosen
ratio.
   Try alternate side-chain conﬁ gurations (next).
     13.6    Alternate Side-Chain Conﬁ gurations
 As Reiss points out, there are several different conﬁ gurations for the contents of the side-
chain. The version we’ve used so far is a standard linear timing placement conﬁ guration.
One alternate conﬁ guration is called the biased conﬁ guration, in which the attack and release
levels start at the threshold, not zero.  Figure 13.13  shows this conﬁ guration. Reiss notes that
this has the effect of allowing the release part of the envelope to fade out when the input
drops below the threshold. If you want to try to implement this one, you will need to make
a modiﬁ cation to the built-in CEnvelopeDetector object to set the low limit levels of the
attack and release times—which depend on the linear magnitude of the signal—to match the
threshold (which is in dB).
 The other two conﬁ gurations involve the location of the attack and release controls. In
the post-gain conﬁ guration shown in  Figure 13.14   the attack and release controls follow
the output of the gain computer and are not intermixed with the RMS  detector. The post-
gain conﬁ guration can also be implemented in RackAFX. You will need to use two
CEnvelopeDetectors. The ﬁ rst one runs in RMS mode with zero attack and zero release times.
It feeds the gain calculator. The second detector follows the gain computer, runs in PEAK
mode and has the user-controlled attack and release controls connected to it. Example code is
available at the  www.willpirkle.com .
 A second variation called log-domain, shown in  Figure 13.15,  places the timing controls
after the gain computer but before the conversion to the linear domain. It could also be
implemented using an extra detector placed in the proper location in the side-chain.
Dynamics Processing  487
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
Threshold  Ratio  Knee
W
dO
ρ
 Figure 13.13:    In the biased conﬁ guration, the threshold is used to
set the attack and release starting levels.
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
Threshold  Ratio  Knee
ρ
W
do
Attack
Release
 Figure 13.14:    The post-gain conﬁ guration with timing controls
placed after the gain computer.
RMS
Detector
log
Conversion
gain calc
(dB)
Attack
Release
lin
Conversion
Threshold  Ratio
d O
ρ
Attack
Release
Knee
W
 Figure 13.15:    The log-domain timing conﬁ guration places the timing
controls after the gain computer but before linear conversion.
     Bibliography
 Ballou, G. 1987.  Handbook for Sound Engineers , pp. 850–860. Carmel, IN: Howard W. Sams & Co.
 Floru, F. 1998. “Attack and Release Time Constants in RMS-Based Feedback Compressors.”  Journal of the Audio
Engineering Society  preprint 4703: 1–4 .
   Pirkle, W. C. 1995. “Applications of a New High-Performance Digitally-Controlled Audio Attenuator in Dynamics
Processing.”  Journal of the Audio Engineering Society  preprint 3970: 1–8.
    References
 Orfanidis, S. 1996.  Introduction to Signal Processing , Chapters 10–11. Englewood Cliffs, NJ: Prentice-Hall.
 Reiss, J. 2011 (October). Under the hood of a dynamic range compressor. Paper presented at the 131st Audio
Engineering Society Convention, New York (Tutorials).
 Zöler, U. 2011.  DAFX—Digital Audio Effects , Chapter 4. West Sussex, U.K.: John Wiley & Sons.

CHAP TE R 14

Miscellaneous Plug-Ins
    There are a few effects left over that didn’t have an exact chapter to reside in, so they are
presented here. Interestingly, they are some of the simplest to implement but can have a
massive sonic impact on the audio they process. These effects include
(cid:129)
(cid:129)
(cid:129)
(cid:129)
   Tremolo
   Auto-panning
   Ring modulation
   Wave shaping
  14.1     Design a Tremolo/Panning Plug-In
 The tremolo is a modulated amplitude effect that uses a low-frequency oscillator (LFO) to
directly modulate the output. The LFO waveform is usually triangular or sinusoidal. If the
LFO is a square wave, it produces a gapping effect, where intermittent chunks of audio are
alternatively muted then restored to unity gain. An auto-panning algorithm pans the signal
from left to right following an LFO. Since they are both amplitude-based effects we can
combine them into one plug-in. The block diagrams are shown in  Figure 14.1 .
 We will allow the user to switch between tremolo and auto-panning modes. For the tremolo,
the LFO will be in unipolar mode, swinging between 0 and 1.0, and the LFO depth will
Input
DCA
Output
Input
DCA L
L Output
Pan
Calculation
LFO
(a)
LFO
(b)
DCA L
R Output
 Figure 14.1 :    (a) A tremolo effect uses an LFO to modulate the amplitude of
 the input. (b) The auto-panning plug-in uses the LFO to calculate
scaling values for the two digitally controlled ampliﬁ ers DCA L and DCA R
to pan the signal from the left to the right.
489
490  Chapter 14
modulate the amplitude of the input signal. The only tricky part is to decide how far below
1.0 the effect will reduce gain. For the tremolo, we deﬁ ne the depth as follows:
(cid:129)
(cid:129)
(cid:129)
   Depth  5 100%, the gain will swing from 0.0 to 1.0
   Depth  5 75%, the gain will swing from 0.25 to 1.0
   Depth  5 50%, the gain will swing from 0.5 to 1.0
 For auto-panning, a depth of 0 yields no effect while 100% pans the signal through the
full left–right stereo ﬁ eld. The panning is calculated using the constant power pan rule
( Equation 14.1 ). In fact, any plug-in you design that has a pan control on it needs to use the
same calculation. Instead of linearly adjusting the left and right gains, they follow the curves
of the ﬁ rst quarter cycle of a sin/cos pair. This way, they overlap at the 0.707 points. It should
be noted there are other panning schemes, but this one seems to be the most common. The
LFO must be in bipolar mode for this to work easily.
p 5
p
4
 1 LFO 1 12
GainL 5  cos 1 p2
GainR 5  sin 1 p2
LFO is bipolar
(14.1)
  14.1.1  Project: TremoloPanner
 Create the project and name it “TremoloPanner.” There are no other objects or options to add.
   14.1.2  TremoloPanner: GUI
 We need rate and depth controls for the LFO and buttons to change the LFO type and the
operational mode of the plug-in shown in  Table 14.1 .
  TremoloPanner.h File
 We need to add a wave table object and two gain calculation functions, one for the tremolo
and one for the panner; notice the pass-by-pointer method used to return the left and right
gain values for the panner.
  // Add your code here: ------------------------------------------------------ //
  // a single LFO
  CWaveTable m_LFO;
  // a function to calculate the amplitude based on LFO
  ﬂ oat calculateGainFactor(ﬂ oat fLFOSample);
  void calculatePannerGainFactor(ﬂ oat fLFOSample, ﬂ oat* pLeftVolume,
  ﬂ oat* pRightVolume);
  // END OF USER CODE --------------------------------------------------------- //
