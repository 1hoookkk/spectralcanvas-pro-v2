# Audio Plugins C++ (Pirkle) — Chunk 49/73

> Source: `Designing Audio Effect Plug-Ins in C++_ With Digital Audio Signal Processing Theory - PDF Room.pdf` · Pages ~1-560 · Extractor: pdfminer.six
> Target ≈ 2800 words, Overlap = 200 words · Actual = 2671

> Note: DSP textbook content for SpectralCanvasPro RT-audio knowledge base

---
Modulated Delay Effects  347
 Table 10.5:    GUI elements for the LCR chorus.
Depth (%)
Min 5 0
Max 5 100
Initial L 5 50
Initial C 5 50
Initial R 5 50
Variable names:
m_fModDepth_pct_L
m_fModDepth_pct_C
m_fModDepth_pct_R
Rate (Hz)
Min 5 0.02
Max 5 5
Initial L 5 0.18
Initial C 5 0.27
Initial R 5 0.49
Variable names:
m_fModFrequency_Hz_L
m_fModFrequency_Hz_C
m_fModFrequency_Hz_R
Feedback (%)
Min 5 2100
Max 5 1100
Initial L 5 0
Initial C 5 0
Initial R 5 0
Variable names:
m_fFeedback_pct_L
m_fFeedback_pct_C
m_fFeedback_pct_R
   CModDelayModule m_ModDelayLeft;
   CModDelayModule m_ModDelayCenter;
   CModDelayModule m_ModDelayRight;
  // function to transfer out variables to it and cook
   void updateModules(); // you could split this out into smaller functions
  // END OF USER CODE ---------------------------------------------------------- //
   10.5.4  StereoLCRChorus.cpp File
  Constructor
 There is nothing to initialize in the constructor because we have no variables; the member
objects will initialize themselves at construction time. Implement the updateModules() function
next. We need to set up the LFO phases and invert ﬂ ags according to the block diagram. We
also need to put the modules in chorus mode and call the internal update functions:
  // updateModules()
  /*
  - set LFO Phases
  - set LFO inversion (right only)
  - Mod Depths
  - Mod Freqs
  - Feedback
  - Mod Types = chorus (2)
  - LFO Types = triangle (0)
  - call updaters
  */
  void CStereoLCRChorus::updateModules()
  {
  // setup quad phase
348  Chapter 10
   m_ModDelayLeft.m_uLFOPhase = 1;   // 1: quad phase
   m_ModDelayCenter.m_uLFOPhase = 0;  // 0: normal
   m_ModDelayRight.m_uLFOPhase = 1;  // 1: quad phase
m_ModDelayLeft.m_LFO.m_bInvert = false;
   m_ModDelayCenter.m_LFO.m_bInvert = false;
   m_ModDelayRight.m_LFO.m_bInvert = true;  // this one is inverted
   m_ModDelayLeft.m_fModDepth_pct = m_fModDepth_pct_L;
   m_ModDelayCenter.m_fModDepth_pct = m_fModDepth_pct_C;
   m_ModDelayRight.m_fModDepth_pct = m_fModDepth_pct_R;
   m_ModDelayLeft.m_fModFrequency_Hz = m_fModFrequency_Hz_L;
   m_ModDelayCenter.m_fModFrequency_Hz = m_fModFrequency_Hz_C;
   m_ModDelayRight.m_fModFrequency_Hz = m_fModFrequency_Hz_R;
   m_ModDelayLeft.m_fFeedback_pct = m_fFeedback_pct_L;
   m_ModDelayCenter.m_fFeedback_pct = m_fFeedback_pct_C;
   m_ModDelayRight.m_fFeedback_pct = m_fFeedback_pct_R;
    // CHORUS!
   m_ModDelayLeft.m_uModType = 2;
   m_ModDelayCenter.m_uModType = 2;
   m_ModDelayRight.m_uModType = 2;
   m_ModDelayLeft.m_uLFOType = 0; // triangle
   m_ModDelayCenter.m_uLFOType = 0;// triangle
   m_ModDelayRight.m_uLFOType = 0;// triangle
  // cook them
   m_ModDelayLeft.updateLFO();
   m_ModDelayCenter.updateLFO();
   m_ModDelayRight.updateLFO();
   m_ModDelayLeft.updateDDL();
   m_ModDelayCenter.updateDDL();
   m_ModDelayRight.updateDDL();
  }
   prepareForPlay()
•
   Forward the calls to prepareForPlay() to the member objects.
  bool __stdcall CStereoLCRChorus::prepareForPlay()
  {
  // Add your code here:
  // call forwarding!
   m_ModDelayLeft.prepareForPlay();
   m_ModDelayCenter.prepareForPlay();
   m_ModDelayRight.prepareForPlay();

  // don’t leave this out – it inits and cooks
   updateModules();
Modulated Delay Effects  349
  return true;
  }
   UserInterfaceChange()
•
   Just do a brute force update of all modules (you can streamline this later):
  bool __stdcall CStereoLCRChorus::userInterfaceChange(int nControlIndex)
  {
  // add your code here
  // brute force update
   updateModules();
  return true;
  }
   ProcessAudioFrame()
•
•
   Split and sum the L 1 R to feed the center chorus.
   Split and sum the outputs (L 5 L 1 C ) and (R 5 R 1 C).
  bool __stdcall CStereoLCRChorus:: processAudioFrame(ﬂ oat* pInputBuffer, ﬂ oat*
  pOutputBuffer, UINT
  uNumInputChannels, UINT
  uNumOutputChannels)
  {
  // declare some output variables
   ﬂ oat fChorusOut_L = 0;
   ﬂ oat fChorusOut_C = 0;
   ﬂ oat fChorusOut_R = 0;
  // get the left and right inputs; note the setup for mono
   ﬂ oat fLeftIn = pInputBuffer[0];
   ﬂ oat fRightIn = pInputBuffer[0];
  // if stereo
   if(uNumInputChannels == 2)
   fRightIn = pInputBuffer[1];
  // form the center channel
   ﬂ oat fCenterIn = 0.5*fLeftIn + 0.5*fRightIn;
  // call the processAudioFrame()
   m_ModDelayLeft.processAudioFrame(&fLeftIn, &fChorusOut_L, 1);
   m_ModDelayCenter.processAudioFrame(&fCenterIn, &fChorusOut_C, 1);
   m_ModDelayRight.processAudioFrame(&fRightIn, &fChorusOut_R, 1);

350  Chapter 10
  // sum to create Left Out
   pOutputBuffer[0] = fChorusOut_L + fChorusOut_C;
  // Mono-In, Stereo-Out (AUX Effect)
  if(uNumInputChannels == 1 && uNumOutputChannels == 2)
   pOutputBuffer[1] = pOutputBuffer[0];
  // Stereo-In, Stereo-Out (INSERT Effect)
  if(uNumInputChannels == 2 && uNumOutputChannels == 2)
   pOutputBuffer[1] = fChorusOut_R + fChorusOut_C;
  return true;
  }
 Build and test the code. Try to ﬁ nd the most interesting settings for different instruments
then go back and set your own initial values accordingly. Be sure to save lots of presets—this
plug-in can produce a wide variety of sounds.
     10.6    More Modulated Delay Algorithms
 In addition to the extra algorithms in Sections 10.1 and 10.2, next are some more designs for
you to experiment with. Remember to ﬁ gure out the UI ﬁ rst then code it using the modules
we’ve already built.
  10.6.1  Stereo Cross-Flanger/Chorus (Korg Triton ® )
 Like the quadrature version, the only difference between the ﬂ anger and chorus is in the
module setting—the same block diagram is used for both. The LFO can be engaged in normal
or quadrature phase ( Figure 10.14 ).
   10.6.2  Multi-Flanger (Sony DPS-M7 ® )
 The DPS-M7 has some intensely thick modulation algorithms. This one has two ﬂ anger
circuits that can be combined in parallel or series on either channel. The channels are also
cross mixable. Each module marked “ﬂ anger” contains a complete ﬂ anger module: depth,
rate, feedback, and wet/dry. Note also the use of pre-delays with feedback too. All LFOs and
pre-delays are independent and fully adjustable ( Figure 10.15 ).
   10.6.3  Bass Chorus
 The bass chorus in  Figure 10.16  splits the signal into low-frequency and high-frequency
components and then only processes the high-frequency component. This leaves the
fundamental intact. The comb ﬁ ltering of the chorus effect smears time according to how
Left In
∑
Z  –D
fb
fb
0°
90°
LFO
Right In
∑
z –D
Modulated Delay Effects  351
Dry
Wet
Wet
Dry
∑
Left  Out
∑
Right  Out
 Figure 10.14:    A stereo cross-ﬂ anger/chorus.
much delay is being used. For the bass guitar, this results in a decreased fundamental with an
ambiguous transient edge or starting point. Because bass players need to provide a deﬁ ned
beat, the bass chorus will preserve this aspect of the instrument. If you want to implement
this effect, use the Linkwitz–Riley low-pass and high-pass ﬁ lters to split the signal. Invert
the output of one of the ﬁ lters—it doesn’t matter which one—so that their phase responses
sum properly.
   10.6.4  Dimension-Style (Roland Dimension D ® )
 This chorus unit (Figure 10.17) is based on the Roland Dimension D ®  chorus. Known for
its subtle transparent sound, it features a shared but inverted LFO and an interesting output
section where each output is a combination of three signals:
1.
2.
3.
   Dry
   Chorus output
   Opposite channel chorus output, inverted and high-pass ﬁ ltered

352  Chapter 10
fb
LPF
LFO
Left  In
∑
Pre-Delay
z –D
Flanger  1
Out 1
Pan 1
∑
∑
Wet
Dry
∑
Left  Out
fb
LPF
LFO
∑
Pre-Delay
z –D
Flanger 2
Out 2
Pan 2
Parallel/series
∑
fb
LPF
LFO
∑
Pre-Delay
z –D
Flanger 3
Out 3
Pan 3
∑
fb
LPF
LFO
Right  In
∑
Pre-Delay
z –D
Flanger 4
Out 4
Pan 4
Parallel/series
∑
∑
Wet
Dry
∑
Right  Out
 Figure 10.15:    DPS M7 multi-ﬂ anger.
LPF
Bass In
Dry
HPF
∑
Z  –chorus
∑
Bass  Out
fb
Wet
 Figure 10.16:    A bass chorus.
LFO

Left In
Z  –chorus
Modulated Delay Effects  353
∑
Left Out
DL
WL
Rate
Depth
0°
–180°
–1
HPF
WCL
LFO
fc
–1
HPF
WCR
Right In
Z  –chorus
WL
DR
∑
Right Out
 Figure 10.17 :    A dimension-style chorus.
 The controls on the original unit consisted of four switches only; these were hardwired
presets and changed
•
•
•
   The LFO rate (either 0.25 Hz or 0.5 Hz)
   The depth
   For one preset, the wet/dry mix ratios for the two channels (W L  vs D L , W R  vs D R )
 Here I have added two more level controls for experimentation, W CL  and W CR , which are the
ﬁ ltered, inverted chorused signals from the opposite (crossed) channels. Experiment with
various rate, depth, and output mixture combinations as well as different high-pass ﬁ lter cut-
off frequencies.
354  Chapter 10
   10.6.5  Deca-Chorus (Sony DPS-M7 ® )
 The deca-chorus has 10 (deca) chorus units, 5 per channel. Each chorus has its own pre-delay,
LFO, gain, and pan control. It can also run in a mono mode (10 chorus units in parallel)
( Figure 10.18 ).
Left In
Pre-Delay 1
Chorus 1
Out 1
Pan 1
∑
∑
Wet
LFO 1
Dry
∑
Left Out
Pre-Delay 2
Chorus 2
Out2
Pan  2
LFO  2
Pre-Delay 3
Chorus 3
Out3
Pan 3
LFO 3
Pre-Delay 4
Chorus 4
Out4
Pan 4
LFO 4
Pre-Delay 5
Chorus 5
Out5
Pan  5
LFO 5
Pre-Delay 1
Chorus 1
Out1
Pan  1
LFO  1
Pre-Delay 2
Chorus 2
Out2
Pan  2
LFO  2
Pre-Delay 3
Chorus 3
Out3
Pan  3
LFO  3
Pre-Delay 4
Chorus 4
Out4
Pan 4
LFO 4
∑
∑
Right In
Pre-Delay 5
Chorus 5
Out5
Pan  5
LFO  5
∑
∑
Wet
Dry
∑
Right Out
 Figure 10.18:    DPS-M7 deca chorus.

Modulated Delay Effects  355
      Bibliography
 Cole, M. 2007. “Roland dimension C clone for Eventide 7000, Orville and H8000.” Accessed August 2012
from  http://www.eventidestompboxes.com/forummedia/PATCHES/Orville/ProFXalgorithms/Roland%20
Dimension%20C.doc .
 Coulter, D. 2000.  Digital Audio Processing , Chapter 11. Lawrence, KS: R&D Books.
 Dattorro, J. 1997. Effect design part 2: Delay line modulation and chorus.  Journal of the Audio Engineering
Society , 45(10): 764–786 .
Korg, Inc. 2000. Triton Rack Parameter Guide. Tokyo: Korg Inc.
 Phillips, D. 1991.  Wavestation SR Reference Guide.  Tokyo: Korg Inc.
 Roads, C. 1996.  The Computer Music Tutorial , Chapter 3. Cambridge, MA: The MIT Press.
 Roland Corporation.  1990. “Boss CH-1 schematics.” Accessed August 2012 from  http://superchorusmods.blogspot.
com/2009/08/schematics.html.
Sony, Corp. 1995. DPS-M7 Manual. Tokyo: Sony Corp.
 White, P. 2001. “Understanding and emulating vintage dffects.” Sound on Sound Magazine. Accessed August 2012
from  http://www.soundonsound.com/sos/jan01/articles/vintage.asp .

CHAP TE R 11

Reverb Algorithms
    Reverb algorithms might represent the Holy Grail of audio signal processing. They have an
appeal that seems universal, perhaps because we live in a reverberant world. Our ears are
time-integrating devices that use time domain cues and transients for information, so we are
sensitive to anything that manipulates these cues. In this chapter, we discuss reverb algorithms
as applied mainly to room simulation. There are two general ways to create the reverberation
effect:
(cid:129)
(cid:129)
   Reverb by direct convolution—the physical approach.
   Reverb by simulation—the perceptual approach.
 In the physical approach, the impulse response of a room is convolved with the input signal
in a large ﬁ nite impulse response (FIR) ﬁ lter. For large rooms, these impulse responses
might be 5 to 10 seconds. In the mid 1990s, Gardner developed a hybrid system for fast
convolution that combined direct convolution with block fast Fourier transform (FFT)
processing (Gardner 1995). Around the same time, Reilly and McGrath (1995)  described
a new commercially available system that could process 262,144-tap FIR ﬁ lters for
convolving impulses over 5 seconds in length. The required processing power is sometimes
too large to be practical in a plug-in. Aside from the computing expense, another drawback
to this approach is that an impulse response is frozen in time, and measures the room at one
location only, under a certain set of conditions. To create a reverb unit (or  reverberator )
that is general enough to provide many different reverbs of different spaces would require
a large library of impulse response ﬁ les. In addition, its parameters can’t be adjusted in
real time.
 The perceptual approach aims to simulate the reverberation with enough quality to fool the
ears and give the same perception of real reverb but with much less processing power. The
advantages are numerous, from the minimal processing required to the ability to vary many
parameters in real time. Browne (2001) proposed a hybrid system combining a short impulse
response convolution along with recursive ﬁ ltering, combining both approaches. There are
several key engineers who developed much of the theory still in use today. These include
Schroeder’s (1962) initial work in the early 1960s with continued research and contributions
from Moorer, Griesinger, Gerzon, Gardner, Jot, Chaigne, Smith, Roscchesso, and others
357
358  Chapter 11
across the decades. Most of this chapter is owed to their work in the ﬁ eld. We will focus on
the perceptual approach and try to ﬁ nd computationally efﬁ cient algorithms for interesting
reverberation effects.
 Griesinger (1989) states that it is impossible to perfectly emulate the natural reverberation of
a real room and thus the algorithms will always be approximations. It seems that the area of
reverberation design has the most empirically derived or trial-and-error research of just about
any audio signal processing ﬁ eld. There is no single “correct” way to implement a reverb
algorithm, so this chapter focuses on giving you many different reverberator modules to
experiment with.
  11.1    Anatomy of a Room Impulse Response
 The ﬁ rst place to start is by examining impulse responses of actual rooms. There are several
popular methods for capturing the impulse response, from cap pistols and balloons to
deconvolution of chirp signals and pseudo random sequences. The resulting time domain plot
is useful for investigating the properties of reverberation.
  Figure 11.1  shows the impulse response plots for two very different spaces; a large concert
hall and a cathedral. The initial impulse is followed by a brief delay called the  pre-delay . As
the impulse pressure wave expands, it comes into contact with the nearby structures—walls,
ﬂ oor, and ceiling—and the ﬁ rst echoes appear. These initial echoes, called  early reﬂ ections ,
are important to the simulation of reverb because of the auditory cues we get from them. The
pressure wave continues to expand and more reﬂ ections occur, with reﬂ ected signal upon
reﬂ ected signal piling on top of each other while decaying in energy. The resulting reverb
“tail” is called  late reverberation  or  subsequent reverberation .
 The top impulse response in  Figure 11.1  is a modern concert hall designed for a pleasing
reverberant quality. The initial reﬂ ections are from irregularly shaped back and side walls
and they pile up in amplitude; they are also piling up in density, shown as the impulse
gets “fatter” in the middle section. The dense reverberant tail follows, which decays in an
exponential fashion, and the sound pressure energy is absorbed by the seats and acoustical
room treatments. The cathedral is a different story. After a short pre-delay, a few large early
reﬂ ections arrive from the nearby structures but they don’t increase in amplitude or density
in the same way as the hall. The reverb’s decay is also much longer in time as there is little in
the cathedral to absorb the sound pressure.
 The block diagram in  Figure 11.2  shows the three components we observe. However, there
is debate as to whether or not this is a good way to break the problem down. Some feel
that the reverb algorithm should not need to separate out the early reﬂ ections from the late
reverberation. In other words, a good algorithm will create all of the reverberation aspects
at once.
Reverb Algorithms  359
Large  Concert  Hall
0
3087
6174
9261
12348
15435
18522
21609
24696
27783
Cathedral
1.000
0.707
0.500
0.000
–0.500
–0.707
–1.000
1.000
0.707
0.500
0.000
–0.500
–0.707
–1.000
0
3087
6174
9261
12348
15435
18522
21609
24696
27783
Early  Reflections
Late  Reverberation
 Figure 11.1:    The impulse responses of a large hall and cathedral.
Dry
ER
∑
Output
Input
Pre-Delay
Early  Reflections
Late  Reverberation
Wet
 Figure 11.2:    A generalized model of a reverb algorithm.
11.1.1    RT 60 : The Reverb Time
 The most common measurement for reverb is the  RT  60  reverb time. Reverb time is measured
by ﬁ rst stimulating the room into a reverberant state, then turning off the source and plotting
the pressure-squared level as a function of time. The amount of time it takes for this energy
decay curve to drop by 60 dB is the  reverb time , or  RT  60 . Sabine’s (1973) pioneering work in
this area leads to the following formula in  Equation 11.1 :
  RT60 5 0.5
VR
SRARAve
(11.1)
where
VR 5 volume of room in ft3
SR 5 surface area of room in ft2
ARAve 5 average absorption coefficient
