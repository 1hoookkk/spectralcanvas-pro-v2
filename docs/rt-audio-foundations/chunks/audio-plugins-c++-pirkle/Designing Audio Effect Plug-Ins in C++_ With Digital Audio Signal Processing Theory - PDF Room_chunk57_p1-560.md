# Audio Plugins C++ (Pirkle) — Chunk 57/73

> Source: `Designing Audio Effect Plug-Ins in C++_ With Digital Audio Signal Processing Theory - PDF Room.pdf` · Pages ~1-560 · Extractor: pdfminer.six
> Target ≈ 2800 words, Overlap = 200 words · Actual = 2390

> Note: DSP textbook content for SpectralCanvasPro RT-audio knowledge base

---
404  Chapter 11
     m_InputAPF_2.setDelay_mSec(m_fAPF_2_Delay_mSec);
     m_InputAPF_2.setAPF_g(m_fAPF_2_g);
    // output diffusion
     m_OutputAPF_3.setDelay_mSec(m_fAPF_3_Delay_mSec);
     m_OutputAPF_3.setAPF_g(m_fAPF_3_g);
     m_OutputAPF_4.setDelay_mSec(m_fAPF_4_Delay_mSec);
     m_OutputAPF_4.setAPF_g(m_fAPF_4_g);
    // comb ﬁ lters
    // set delays ﬁ rst...
     m_ParallelCF_1.setDelay_mSec(m_fPComb_1_Delay_mSec);
     m_ParallelCF_2.setDelay_mSec(m_fPComb_2_Delay_mSec);
     m_ParallelCF_3.setDelay_mSec(m_fPComb_3_Delay_mSec);
     m_ParallelCF_4.setDelay_mSec(m_fPComb_4_Delay_mSec);
     m_ParallelCF_5.setDelay_mSec(m_fPComb_5_Delay_mSec);
     m_ParallelCF_6.setDelay_mSec(m_fPComb_6_Delay_mSec);
     m_ParallelCF_7.setDelay_mSec(m_fPComb_7_Delay_mSec);
     m_ParallelCF_8.setDelay_mSec(m_fPComb_8_Delay_mSec);
    // …then calculate comb g's from RT60:
     m_ParallelCF_1.setComb_g_with_RTSixty(m_fRT60);
     m_ParallelCF_2.setComb_g_with_RTSixty(m_fRT60);
     m_ParallelCF_3.setComb_g_with_RTSixty(m_fRT60);
     m_ParallelCF_4.setComb_g_with_RTSixty(m_fRT60);
     m_ParallelCF_5.setComb_g_with_RTSixty(m_fRT60);
     m_ParallelCF_6.setComb_g_with_RTSixty(m_fRT60);
     m_ParallelCF_7.setComb_g_with_RTSixty(m_fRT60);
     m_ParallelCF_8.setComb_g_with_RTSixty(m_fRT60);
    // LPFs
     m_DampingLPF1.setLPF_g(m_fLPF2_g2);
     m_DampingLPF2.setLPF_g(m_fLPF2_g2);
     m_InputLPF.setLPF_g(m_fInputLPF_g);
    // LPF–comb ﬁ lters
     m_ParallelCF_3.setLPF_g(m_fLPF2_g2);
     m_ParallelCF_4.setLPF_g(m_fLPF2_g2);
     m_ParallelCF_7.setLPF_g(m_fLPF2_g2);
     m_ParallelCF_8.setLPF_g(m_fLPF2_g2);
   }
  Constructor
 There is nothing to do here because the child-object constructors handle these details.
Reverb Algorithms  405
   prepareForPlay()
(cid:129)
(cid:129)
(cid:129)
(cid:129)
(cid:129)
(cid:129)
   Initialize all the objects with their max delay times; all delay times except the pre-delay
have maximum values of 100 mSec.
   Initialize the pre-delay for its maximum of 2 seconds.
   Reset all delays.
   Set the sample rate for the delays.
   Cook all the variables at once.
   Init the delays to ﬂ ush buffers.
  bool __stdcall CReverb::prepareForPlay()
  {
  // Add your code here:
    // up to 2 seconds predelay
     m_PreDelay.init(2.0*(m_nSampleRate));
    // init up to 100 mSec
     m_InputAPF_1.init(0.1*(m_nSampleRate));
     m_InputAPF_2.init(0.1*(m_nSampleRate));
    // 100 mSec each max
     m_ParallelCF_1.init(0.1*(m_nSampleRate));
     m_ParallelCF_2.init(0.1*(m_nSampleRate));
     m_ParallelCF_3.init(0.1*(m_nSampleRate));
     m_ParallelCF_4.init(0.1*(m_nSampleRate));
     m_ParallelCF_5.init(0.1*(m_nSampleRate));
     m_ParallelCF_6.init(0.1*(m_nSampleRate));
     m_ParallelCF_7.init(0.1*(m_nSampleRate));
     m_ParallelCF_8.init(0.1*(m_nSampleRate));
    // 100 mSec each max
     m_OutputAPF_3.init(0.1*(m_nSampleRate));
     m_OutputAPF_4.init(0.1*(m_nSampleRate));
    // init the three LPFs
     m_InputLPF.init();
     m_DampingLPF1.init();
     m_DampingLPF2.init();
    // Call all delay resets
     m_PreDelay.resetDelay();
     m_InputAPF_1.resetDelay();
     m_InputAPF_2.resetDelay();
     m_ParallelCF_1.resetDelay();
     m_ParallelCF_2.resetDelay();
     m_ParallelCF_3.resetDelay();
     m_ParallelCF_4.resetDelay();
     m_ParallelCF_5.resetDelay();
406  Chapter 11
     m_ParallelCF_6.resetDelay();
     m_ParallelCF_7.resetDelay();
     m_ParallelCF_8.resetDelay();
     m_OutputAPF_3.resetDelay();
     m_OutputAPF_4.resetDelay();
    // set sample rates on combs (needed to calc g values)
     m_ParallelCF_1.setSampleRate(this->m_nSampleRate);
     m_ParallelCF_2.setSampleRate(this->m_nSampleRate);
     m_ParallelCF_3.setSampleRate(this->m_nSampleRate);
     m_ParallelCF_4.setSampleRate(this->m_nSampleRate);
     m_ParallelCF_5.setSampleRate(this->m_nSampleRate);
     m_ParallelCF_6.setSampleRate(this->m_nSampleRate);
     m_ParallelCF_7.setSampleRate(this->m_nSampleRate);
     m_ParallelCF_8.setSampleRate(this->m_nSampleRate);
    // ﬂ ush buffers
     m_InputLPF.init();
     m_DampingLPF1.init();
     m_DampingLPF2.init();
    // cook everything
     cookVariables();
    return true;
  }
   userInterfaceChange()
(cid:129)
   Call the one and only cooking function
  bool __stdcall CReverb::userInterfaceChange(int nControlIndex)
  {
    // add your code here
     cookVariables();
    return true;
  }
   processAudioFrame()
 In this function, you tie all the member objects together by creating variables to pass into/
out of their processAudio() functions. By stepping through the code, you can see just how the
series and parallel elements are connected. You can always go back and make this code more
streamlined by removing intermediate variables, and so on. The four outputs of each comb
ﬁ lter bank are mixed at 15% ratios with alternating signs.
  bool __stdcall CReverb::processAudioFrame(ﬂ oat* pInputBuffer, ﬂ oat* pOutputBuffer, UINT
              uNumInputChannels, UINT uNumOutputChannels)
  {
    //
    // output = input – change this for meaningful processing

Reverb Algorithms  407
    //
    // Form our input = L + R (if there is a R)
    //
     ﬂ oat fInputSample = pInputBuffer[0];
     if(uNumInputChannels == 2)
     {
       // mix
       fInputSample += pInputBuffer[1];
       // attenuate by 0.5
       fInputSample *= 0.5;
    }
    // begin the series/parallel signal push
    // Pre-Delay
     ﬂ oat fPreDelayOut = 0;
     m_PreDelay.processAudio(&fInputSample, &fPreDelayOut);
    // Pre-Delay Out -> fAPF_1_Out
     ﬂ oat fAPF_1_Out = 0;
     m_InputAPF_1.processAudio(&fPreDelayOut, &fAPF_1_Out);
    // fAPF_1_Out -> fAPF_2_Out
     ﬂ oat fAPF_2_Out = 0;
     m_InputAPF_2.processAudio(&fAPF_1_Out, &fAPF_2_Out);
    // fAPF_2_Out -> fInputLPF
     ﬂ oat fInputLPF = 0;
     m_InputLPF.processAudio(&fAPF_2_Out, &fInputLPF);
    // comb ﬁ lter bank
    // variables for each output
     ﬂ oat fPC_1_Out = 0;
     ﬂ oat fPC_2_Out = 0;
     ﬂ oat fPC_3_Out = 0;
     ﬂ oat fPC_4_Out = 0;
     ﬂ oat fPC_5_Out = 0;
     ﬂ oat fPC_6_Out = 0;
     ﬂ oat fPC_7_Out = 0;
     ﬂ oat fPC_8_Out = 0;
     ﬂ oat fC1_Out = 0;
     ﬂ oat fC2_Out = 0;
    // fInputLPF -> fPC_1_Out, fPC_2_Out, fPC_3_Out, fPC_4_Out
     m_ParallelCF_1.processAudio(&fInputLPF, &fPC_1_Out);
     m_ParallelCF_2.processAudio(&fInputLPF, &fPC_2_Out);
     m_ParallelCF_3.processAudio(&fInputLPF, &fPC_3_Out);
     m_ParallelCF_4.processAudio(&fInputLPF, &fPC_4_Out);
    // fInputLPF -> fPC_5_Out, fPC_6_Out, fPC_7_Out, fPC_8_Out
     m_ParallelCF_5.processAudio(&fInputLPF, &fPC_5_Out);
     m_ParallelCF_6.processAudio(&fInputLPF, &fPC_6_Out);

408  Chapter 11
     m_ParallelCF_7.processAudio(&fInputLPF, &fPC_7_Out);
     m_ParallelCF_8.processAudio(&fInputLPF, &fPC_8_Out);
    // form outputs: note attenuation by 0.15 for each and alternating signs
     fC1_Out = 0.15*fPC_1_Out - 0.15*fPC_2_Out + 0.15*fPC_3_Out - 0.15*fPC_4_Out;
     fC2_Out = 0.15*fPC_5_Out - 0.15*fPC_6_Out + 0.15*fPC_7_Out - 0.15*fPC_8_Out;
    // fC1_Out -> fDamping_LPF_1_Out
     ﬂ oat fDamping_LPF_1_Out = 0;
     m_DampingLPF1.processAudio(&fC1_Out, &fDamping_LPF_1_Out);
    // fC2_Out -> fDamping_LPF_2_Out
     ﬂ oat fDamping_LPF_2_Out = 0;
     m_DampingLPF2.processAudio(&fC2_Out, &fDamping_LPF_2_Out);
    // fDamping_LPF_1_Out -> fAPF_3_Out
     ﬂ oat fAPF_3_Out = 0;
     m_OutputAPF_3.processAudio(&fDamping_LPF_1_Out, &fAPF_3_Out);
    // fDamping_LPF_2_Out -> fAPF_4_Out
     ﬂ oat fAPF_4_Out = 0;
     m_OutputAPF_4.processAudio(&fDamping_LPF_2_Out, &fAPF_4_Out);
    // form output = (100-Wet)/100*x(n) + (Wet/100)*fAPF_3_Out
     pOutputBuffer[0] = ((100.0 - m_fWet_pct)/100.0)*fInputSample +
    (m_fWet_pct/100.0)*(fAPF_3_Out);
    // Do RIGHT Channel if there is one
    if(uNumOutputChannels == 2)
    {
      // form output = (100-Wet)/100*x(n) + (Wet/100)*fAPF_4_Out
       pOutputBuffer[1] = ((100.0 - m_fWet_pct)/100.0)*fInputSample +
    (m_fWet_pct/100.0)*(fAPF_4_Out);
    }
    return true;
   }
 Build and test the example reverb module. Use RackAFX’s impulse response tool in the
analyzer to tweak your settings. Adjust the impulse response time according to your  RT  60
times. Notice how the impulse response can drastically change with only a slight modiﬁ cation
of the parameters. Also, try adjusting the mix coefﬁ cients for the comb ﬁ lter outputs by
alternating signs or using different weightings. Visit the website  www.willpirkle.com  for
more example reverb algorithms and code.
     11.18  Challenge
 Design your own reverb. Start by implementing some of the classics (Schroeder, Moorer)
and some of the more recent versions (Gardner, Jot, Dattorro) combining different modules.

Reverb Algorithms  409
Or start with the reverb design here and modify it. For example, try replacing the APFs with
nested all-pass modules or add or remove comb ﬁ lters. You can easily identify ringing and
oscillations using the impulse response tool, so keep it open as you experiment.
 Bibliography
 Griesinger, D. 1995. How loud is my reverberation?  Journal of the Audio Engineering Society , preprint 3943, pp. 1–11.
 Roads, C. 1996.  The Computer Music Tutorial , Chapter 3. Cambridge, MA: The MIT Press.
     References
 Beranek, L. 1986.  Acoustics . New York: American Institute of Physics.
 Browne, S. 2001. “Hybrid reverberation algorithm using truncated impulse response convolution and recursive
ﬁ ltering.” Master’s diss., University of Miami, Miami, FL.
 Chemistruck, M., Marcolini, K., and Pirkle, W. 2012. Generating matrix coefﬁ cients for feedback delay networks
using genetic algorithms. Journal of the Audio Engineering Society, preprint N/A.
 Dahl, L. and Jot, J-M. 2000. A reverberator based on absorbent all-pass ﬁ lters.  Proceedings of the COST G-6
Convention on Digital Audio Effects , DAFX-00, pp. 1–6.
 Dattorro, J. 1997. Effect design part 1: Reverberators and other ﬁ lters.  Journal of the Audio Engineering Society  45(9):
660–665.
 Frenette, J. 2000. “Reducing artiﬁ cial reverberation requirements using time-variant feedback delay networks.”
Master’s diss., University of Miami, Miami, FL.
 Gardner, W. G. 1992. “The virtual acoustic room.” Master’s diss., Massachusetts Institute of Technology, Boston, MA.
 Gardner, W. G. 1995. Efﬁ cient convolution without input-output delay.  Journal of the Audio Engineering Society  43(3):
127–135.
 Gerzon, M. A. Synthetic stereo reverberation.  Studio Sound Magazine , January 14, 1972.
 Griesinger, D. 1989. Practical processors and programs for digital reverberation.  7th International Conference of
the Audio Engineering Society , pp. 187–195.
   Jot, J-M. and Chaigne, A. 1991. Digital delay networks for designing artiﬁ cial reverberators.  Journal of the Audio
Engineering Society , preprint 3030, pp. 1–16.
 Jot, J-M. 1992. “Design and implementation of a sound spatializer based on physical and perceptual models.”
PhD diss., Telecom, Paris.
Kahrs, M. and Brandenberg, K. 1998.  Applications of Digital Signal Processing to Audio and Acoustics , Chapter 3.
Boston: Kluwer Academic Publishers.
 Kuttruff, H. 1991.  Room Acoustics . New York: Elsevier.
 Moorer, J. A. 1979. About this reverberation business.  Computer Music Journal  3(2): 13–28.
 Reilly, A. and McGrath, D. S. 1995. Convolution processing for realistic reverb.  Journal of the Audio Engineering
Society , preprint 3977, pp. 1–8.
   Sabine, W. 1973 “Reverberation.” Lindsay, R. B., ed. ,  Acoustics: Historical and Philosophical Development .
Stroudsburg, PA: Dowden, Hutchinson & Ross.
 Schroeder, M. 1962. Natural-sounding artiﬁ cial reverberation.  Journal of the Audio Engineering Society  10(3):
209–213.
 Schroeder, M. 1984. Progress in architectural acoustics and artiﬁ cial reverberation: Concert hall acoustics and
number theory.  Journal of the Audio Engineering Society  32(4): 194–202.
 Smith, J. O. 1985. A new approach to digital reverberation using closed waveguide networks.  Proceedings of the
1985 International Computer Music Conference , pp. 47–53.
 Smith, J. O. and Rocchesso, D. 1994. Connections between feedback delay networks and waveguide networks for
digital reverberation.  Proceedings of the 1994 International Computer Music Conference, pp. 376–377.
 Stautner, J. and Puckette, M. 1982. Designing multi-channel reverberators.  Computer Music Journal  6(1): 52–65.

CHAP TE R 12

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
