# Audio Plugins C++ (Pirkle) — Chunk 56/73

> Source: `Designing Audio Effect Plug-Ins in C++_ With Digital Audio Signal Processing Theory - PDF Room.pdf` · Pages ~1-560 · Extractor: pdfminer.six
> Target ≈ 2800 words, Overlap = 200 words · Actual = 2654

> Note: DSP textbook content for SpectralCanvasPro RT-audio knowledge base

---
Implements a delay of  D  samples with an output attenuator. This is also the base class for
other stock objects in Figure 11.42. Table 11.5 shows object members.
Reverb Algorithms  395
Member Variables
ﬂ oat m_fDelayInSamples
ﬂ oat m_fOutputAttenuation
ﬂ oat* m_pBuffer
int m_nReadIndex
int m_nWriteIndex
int m_nBufferSize
int m_nSampleRate
ﬂ oat m_fDelay_ms
ﬂ oat m_fOutputAttenuation_dB
Member Functions
void init(int nDelayLength)
Parameters:
(cid:129)  int nDelayLength
void cookVariables()
void resetDelay()
void setDelay_mSec(ﬂ oat fmSec)
Parameters:
(cid:129)  ﬂ oat fmSec
void setSampleRate(int nFs)
Parameters:
(cid:129)  int nFs
void setOutputAttenuation_
dB(ﬂ oat fAttendB)
Parameters:
(cid:129)  ﬂ oat fAttendB
ﬂ oat readDelay()
ﬂ oat readDelayAt(ﬂ oat fmSec)
Parameters:
(cid:129)  ﬂ oat fmSec
void writeDelayAndInc(ﬂ oat
fDelayInput)
Parameters:
(cid:129)  ﬂ oat fDelayInput
bool processAudio(ﬂ oat* pInput,
ﬂ oat* pOutput)
Parameters:
(cid:129)  ﬂ oat* pInput
(cid:129)  ﬂ oat* pOutput
  Table 11.5:    The CDelay object  .
Purpose
Implements the one and only gain coefﬁ cient g
Output attenuation variable
Pointer to our dynamically declared buffer
Current read location
Current write location
Max buffer size
Sample rate
Delay in mSec, set by the parent plug-in
Output attenuation in dB, set by the parent plug-in
Declare the buffer and initialize it with 0.0s
Input: the buffer length in samples
Function to cook delay (nSec) and attenuation (dB) to the variables
actually used in the calculation
Reset the pointers to top, ﬂ ush buffer
Called by parent to set the delay in mSec
Input: the desired delay time
Set the sample rate; called by parent
Input: the sample rate integer
Set the output attenuator; called by parent
Input: the attenuation in dB
Read the delay with the user deﬁ ned delay time without incrementing
any pointers or writing input data
Read the delay at an arbitrary delay time without incrementing any
pointers or writing input data; caller must make sure that this does
not exceed max delay time
Input: the delay time to read
Write the input value and increment the pointer indices
Input: the input audio sample
Processes one input sample
Input: pointer to a ﬂ oat input sample
Output: pointer to the output sample destination
396  Chapter 11
x(n)
z –D
a0
y(n)
 Figure 11.42:      The delay with output attenuator.
x(n)
∑
z –D
y(n)
g
 Figure 11.43:     The comb ﬁ lter.
   11.16.3  CCombFilter
 Implements a  D -sample comb ﬁ lter with feedback coefﬁ cient  g  in Figure 11.43. Table 11.6
shows object members.
  Table 11.6:    The CCombFilter object.
Member Variables
ﬂ oat m_fComb_g
Member Functions
void setComb_g(ﬂ oat fCombg)
Parameters:
(cid:129)  ﬂ oat fCombg
void setComb_g_with_RTSixty(ﬂ oat fRT)
Parameters:
(cid:129)  ﬂ oat fRT
CDelay Base Class Overrides
bool processAudio(ﬂ oat* pInput, ﬂ oat* pOutput)
Parameters:
(cid:129)  ﬂ oat* pInput
(cid:129)  ﬂ oat* pOutput
   11.16.4  CLPFCombFilter
Purpose
The one and only feedback gain coefﬁ cient g
set() function for feedback gain coefﬁ cient
Input: the new feedback gain value
Set the feedback gain using the RT60 time
Input: RT60 time in mSec
Process one input sample
Input: pointer to a ﬂ oat input sample
Output: pointer to the output sample destination
 Implements a  D -sample comb ﬁ lter with LPF in feedback loop (with coefﬁ cient  g 2) and
feedback coefﬁ cient  g 1 in Figure 11.44. Table 11.7 shows object members.
x(n)
∑
z–D
y(n)
Reverb Algorithms  397
g1
∑
z–1
g2
 Figure 11.44:     The LPF–comb ﬁ lter.
  Table 11.7:    The CLPFCombFilter object.
Member Variables
ﬂ oat m_fComb_g
ﬂ oat m_fLPF_g
ﬂ oat m_fLPF_z1
Member Functions
void setComb_g(ﬂ oat fCombg)
Parameters:
(cid:129)  ﬂ oat fCombg
Purpose
The one and only feedback gain coefﬁ cient g1
The LPF gain coefﬁ cient, g2
Register for one pole LPF
set() function for feedback gain coefﬁ cient
Input: the new feedback gain value
void setComb_g_with_RTSixty(ﬂ oat fRT)
Parameters:
(cid:129)  ﬂ oat fRT
void setComb_g_with_RTSixty(ﬂ oat fRT)
Parameters:
ﬂ oat fRT
void setLPF_g(ﬂ oat fOverAllGain)
Parameters:
(cid:129)  ﬂ oat fOverAllGain
CDelay Base Class Overrides
void init(int nDelayLength)
Parameters:
(cid:129)  int nDelayLength
Set the LPF gain knowing an overall gain value; uses
Equation 11.27
Input: the overall gain in Equation 11.27
Dynamically create and init the buffer
Input: delay length in samples
bool processAudio(ﬂ oat* pInput, ﬂ oat* pOutput)
Parameters:
(cid:129)  ﬂ oat* pInput
(cid:129)  ﬂ oat* pOutput
Process one input sample
Input: pointer to a ﬂ oat input sample
Output: pointer to the output sample destination

398  Chapter 11
x(n)
∑
–g
g
z–D
y(n)
∑
 Figure 11.45:     The delaying APF.
   11.16.5  CDelayAPF
 Implements a  D -sample delaying APF in Figure 11.45. Table 11.8 shows object members.
  Table 11.8:    The CDelayAPF object.
Member Variables
ﬂ oat m_fAPF_g
Member Functions
void setAPF_g(ﬂ oat fAPFg)
Parameters:
(cid:129)  ﬂ oat fAPFg
CDelay Base Class Overrides
Purpose
The one and only gain coefﬁ cient g
set() function for gain coefﬁ cient
Input: the new gain value
bool processAudio(ﬂ oat* pInput, ﬂ oat* pOutput)
Parameters:
(cid:129)  ﬂ oat* pInput
(cid:129)  ﬂ oat* pOutput
Process one input sample
Input: pointer to a ﬂ oat input sample
Output: pointer to the output sample destination
    11.17    Design the Room Reverb
  11.17.1  Project: Reverb
 Start a new project named “Reverb” and be sure to check the box to automatically include the
stock reverb objects.
   11.17.2  Reverb GUI
 The ﬁ rst thing to do is decide on a user interface to control the reverb’s modules. Since this
is a learning tool as well, we will give the user many more controls than they would normally
ﬁ nd on a reverb for the purpose of experimentation. Refer back to  Figure 11.37 , which has
the block diagram and GUI components with it. There are going to be a lot of sliders to set
up since this is a complex design ( Table 11.9 ). However the use of the stock reverb objects is
going to make the rest of the design easy.
Reverb Algorithms  399
  Table 11.9 :    UI Control Properties for the Room Reverb: Pre-Delay
Slider Property
Control Name
Units
Variable Type
Value
Pre Delay
mSec
ﬂ oat
Slider Property
Control Name
Units
Variable Type
Value
Pre Dly Atten
dB
ﬂ oat
Variable Name
m_fPreDelay_mSec
Variable Name
m_fPreDelayAtten_dB
Low Limit
High Limit
Initial Value
0
100
40
Low Limit
High Limit
Initial Value
–96
0
0
  Input Diffusion
Slider Property
Value
Control Name
Bandwidth
Units
Variable Type
Variable Name
Low Limit
High Limit
Initial Value
Value
APF1 Dly
mSec
ﬂ oat
Slider Property
Control Name
Units
Variable Type
Variable Name
m_fAPF_1_Delay_mSec
Low Limit
High Limit
Initial Value
0
100
13.28
ﬂ oat
m_fInputLPF_g
0
1
0.45
Slider Property
Control Name
Units
Variable Type
Variable Name
Low Limit
High Limit
Initial Value
Value
APF1 g
ﬂ oat
m_fAPF_1_g
21
1
0.7
400  Chapter 11
Slider Property
Control Name
Units
Variable Type
Value
APF2 Dly
mSec
ﬂ oat
Variable Name
m_fAPF_2_Delay_mSec
0
100
28.13
Slider Property
Control Name
Units
Variable Type
Variable Name
Low Limit
High Limit
Initial Value
  Parallel Comb Filter Bank 1
Value
PComb1 Dly
mSec
ﬂ oat
Slider Property
Control Name
Units
Variable Type
Low Limit
High Limit
Initial Value
Slider Property
Control Name
Units
Variable Type
Value
APF2 g
ﬂ oat
m_fAPF_2_g
21
1
20.7
Value
PComb2 Dly
mSec
ﬂ oat
Variable Name
m_fPComb_1_Delay_mSec
Variable Name
m_fPComb_2_Delay_mSec
Low Limit
High Limit
Initial Value
Slider Property
Control Name
Units
Variable Type
0
100
31.71
Value
PComb3 Dly
mSec
ﬂ oat
Low Limit
High Limit
Initial Value
Slider Property
Control Name
Units
Variable Type
0
100
37.11
Value
PComb4 Dly
mSec
ﬂ oat
Variable Name
m_fPComb_3_Delay_mSec
Variable Name
m_fPComb_4_Delay_mSec
Low Limit
High Limit
Initial Value
Slider Property
Control Name
Units
Variable Type
0
100
40.23
Low Limit
High Limit
Initial Value
0
100
44.14
  Parallel Comb Filter Bank 2
Value
PComb5 Dly
mSec
ﬂ oat
Slider Property
Control Name
Units
Variable Type
Value
PComb6 Dly
mSec
ﬂ oat
Variable Name
m_fPComb_5_Delay_mSec
Variable Name
m_fPComb_6_Delay_mSec
Low Limit
High Limit
Initial Value
0
100
30.47
Low Limit
High Limit
Initial Value
0
100
33.98
Reverb Algorithms  401
Slider Property
Control Name
Units
Variable Type
Value
PComb7 Dly
mSec
ﬂ oat
Slider Property
Control Name
Units
Variable Type
Value
PComb8 Dly
mSec
ﬂ oat
Variable Name
m_fPComb_7_Delay_mSec
Variable Name
m_fPComb_8_Delay_mSec
Low Limit
High Limit
Initial Value
0
100
41.41
Low Limit
High Limit
Initial Value
0
100
42.58
  Input Diffusion and Damping
Slider Property
Control Name
Units
Variable Type
Variable Name
Low Limit
High Limit
Initial Value
Value
APF3 Dly
mSec
ﬂ oat
Slider Property
Control Name
Units
Variable Type
Variable Name
m_fAPF_3_Delay_mSec
Low Limit
High Limit
Initial Value
Slider Property
Control Name
Units
Variable Type
0
100
9.38
Value
APF4 Dly
mSec
ﬂ oat
Variable Name
m_fAPF_4_Delay_mSec
Low Limit
High Limit
Initial Value
0
100
11
Value
Damping
ﬂ oat
m_fLPF2_g2
0
1
0.5
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
APF3 g
ﬂ oat
m_fAPF_3_g
21
1
20.6
Value
APF4 g
ﬂ oat
m_fAPF_4_g
21
1
0.6
402  Chapter 11
  Reverb Output
Slider Property
Value
Control Name
Reverb Time
Units
Variable Type
Variable Name
Low Limit
High Limit
Initial Value
mSec
ﬂ oat
m_fRT60
0
5000
1000
Slider Property
Control Name
Units
Variable Type
Variable Name
Low Limit
High Limit
Initial Value
Value
Wet/Dry
%
ﬂ oat
m_fWet_pct
0
100
50
 Build the UI using the list of controls above; the variable names I used in the project are
listed in the table . You can use your own variable names but you will need to make sure they
are correctly implemented.  Figure 11.46  shows my GUI; I grouped sets of controls together,
following the algorithm block diagram.
   11.17.3  Reverb.h
 In the .h ﬁ le, declare the member objects plus one cooking function that will change all
modules at once (it’s not streamlined but it is simple to cook all at once). There is a stock
object for each block in the algorithm.
I 40.0 mSec  I I  0.00 dB  II
Pre Dly
PD Atten
Bandwidth
APF1 Dly
APF1g
APF2 Dly
AP F2g
0.7
0.45
I
I I 28. 1 mSec  I I  - 0.54
11 13.28mSec l l
t l  + ~  l  ~  ~
~  ~  + + +
131.71  mSec I 137.11  mSec I 140.23 mSec I 144 .14 mSec I I  0.49
PComb1 Dly
PComb4 Dly
PComb2 Dly
PComb3 Dly
Damping
I
PComb5 Dly
PComb6 Dly
PComb7 Dly
PComb8 Dly
I  30.47
II  33.98
II  41.41
II  42.58
I
~  ~  ~  +
 Figure 11.46:     The completed reverb prototype GUI.
RT60
WeliDry
1 1800 mSec  I I  50.00%  I
+ +
APF3g
APF3 Dly
I 9.38  mSec  I I  -0.60
I
APF4 Dly
111 .00 mSec I I  0.60
APF4g
~  ~
~  l
I

    // Add your code here: –––––––––––––––––––––––––––- //
    //
    // Pre-Delay Block
     CDelay m_PreDelay;
Reverb Algorithms  403
    // input Diffusion
     COnePoleLPF m_InputLPF;
     CDelayAPF m_InputAPF_1;
     CDelayAPF m_InputAPF_2;
    // parallel Comb Bank 1
     CCombFilter m_ParallelCF_1;
     CCombFilter m_ParallelCF_2;
     CLPFCombFilter m_ParallelCF_3;
     CLPFCombFilter m_ParallelCF_4;
    // parallel Comb Bank 2
     CCombFilter m_ParallelCF_5;
     CCombFilter m_ParallelCF_6;
     CLPFCombFilter m_ParallelCF_7;
     CLPFCombFilter m_ParallelCF_8;
    // damping
     COnePoleLPF m_DampingLPF1;
     COnePoleLPF m_DampingLPF2;
    // output diffusion
     CDelayAPF m_OutputAPF_3;
     CDelayAPF m_OutputAPF_4;
    // function to cook all member object's variables at once
     void cookVariables();
    // END OF USER CODE ––––––––––––––––––––––––––––– //
   11.17.4  Reverb.cpp
 Write the only extra function, cookVariables():
  // function to cook all variables at once
   void CReverb::cookVariables()
   {
    // Pre-Delay
     m_PreDelay.setDelay_mSec(m_fPreDelay_mSec);
     m_PreDelay.setOutputAttenuation_dB(m_fPreDelayAtten_dB);
    // input diffusion
     m_InputAPF_1.setDelay_mSec(m_fAPF_1_Delay_mSec);
     m_InputAPF_1.setAPF_g(m_fAPF_1_g);

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
