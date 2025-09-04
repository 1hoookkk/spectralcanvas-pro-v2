# Audio Plugins C++ (Pirkle) — Chunk 68/73

> Source: `Designing Audio Effect Plug-Ins in C++_ With Digital Audio Signal Processing Theory - PDF Room.pdf` · Pages ~1-560 · Extractor: pdfminer.six
> Target ≈ 2800 words, Overlap = 200 words · Actual = 4797

> Note: DSP textbook content for SpectralCanvasPro RT-audio knowledge base

---
Miscellaneous Plug-Ins  491
  Table 14.1:    TremoloPanner graphical user interface (GUI) controls.
Slider Property
Control Name
Units
Variable Type
Variable Name
Low Limit
High Limit
Initial Value
Value
Mod Rate
Hz
ﬂ oat
m_ModRate
0.02
25
5
Slider Property
Control Name
Units
Variable Type
Variable Name
Low Limit
High Limit
Initial Value
Value
Mod Depth
%
ﬂ oat
m_fModDepth
0
100
50
Button Property
Control Name
Units
Variable Type
Variable Name
Enum String
Value
LFO
enum
m_uLFO_Waveform
sine, saw, tri, square
Button Property
Control Name
Units
Variable Type
Variable Name
Enum String
Value
Mode
enum
m_uMode
Tremolo, Panner
   TremoloPanner.cpp File
 The tremolo gain calculation will:
(cid:129)
(cid:129)
   Multiply the LFO sample by depth/100.0.
   Add the value 1.0-depth /100.0 to the result.
 That provides the mapping we need for the effect.
  ﬂ oat CTremoloPanner::calculateGainFactor(ﬂ oat fLFOSample)
  {
  // ﬁ rst multiply the value by depth/100
  ﬂ oat fOutput = fLFOSample*(m_fModDepth/100.0);
  // then add the value (1 — m_fModDepth/100.0)
  fOutput += 1 — m_fModDepth/100.0;
  return fOutput;
  }
  void CTremoloPanner::calculatePannerGainFactor(ﬂ oat fLFOSample,
  ﬂ oat* pLeftVolume, ﬂ oat* pRightVolume)
  {
  }
  // calc sin/cos quadrant location
  ﬂ oat fPan = ((m_fModDepth/100.0)*fLFOSample + 1)*pi/4.0;
  // equal power calculation
  *pLeftVolume = cos(fPan);
  *pRightVolume = sin(fPan);
492  Chapter 14
   prepareForPlay()
(cid:129)
(cid:129)
   Initialize the LFO.
   Forward the prepareForPlay() method to the LFO.
  bool __stdcall CTremoloPanner::prepareForPlay()
  {
  // Add your code here:
  m_LFO.m_fFrequency_Hz = m_fModRate;
  m_LFO.m_uPolarity = m_uMode == Panner ? 0 : 1; // 0 = bipolar, 1 = unipolar
  m_LFO.m_uTableMode = 0; // normal, no band limiting
  m_LFO.m_uOscType = m_uLFO_Waveform;
  m_LFO.setSampleRate(m_nSampleRate); // really important!
  // the LFO prepareForPlay() calls reset() and cookFrequency()
  m_LFO.prepareForPlay();
  return true;
  }
   userInterfaceChange()
(cid:129)
(cid:129)
   Decode the control index and update variables as needed.
   Note: Make sure you check your nControlIndex values to match your GUI.
  bool __stdcall CTremoloPanner::userInterfaceChange(int nControlIndex)
  {
  // decode the control index, or delete the switch and use brute force calls
  switch(nControlIndex)
  {
  case 0:
  {
  m_LFO.m_fFrequency_Hz = m_fModRate;
  m_LFO.cookFrequency();
  break;
  }
  // 41: LFO Waveform change
  case 41:
  {
  m_LFO.m_uOscType = m_uLFO_Waveform;
  break;
  }
  case 42:
  {
  if(m_uMode == Panner)
  m_LFO.m_uPolarity = 0;  // 0 = bipolar, 1 = unipolar
  else // is tremolo
  m_LFO.m_uPolarity = 1;  // 0 = bipolar, 1 = unipolar
  break;
  }
  }
  }
Miscellaneous Plug-Ins  493
   processAudioFrame()
(cid:129)
(cid:129)
   Generate the LFO value.
   Calculate the channel gain values according to the mode.
 In processAudioFrame() we need to generate a new LFO value, calculate the new gain factor,
then implement the gain function.
  bool __stdcall CTremoloPanner::processAudioFrame(ﬂ oat* pInputBuffer,
  {
  // Do LEFT (MONO) Channel; there is always at least one input/one output
  ﬂ oat fYn = 0; // normal output
   ﬂ oat fYqn = 0;  // quad phase output
  ﬂ oat* pOutputBuffer,
  UINT uNumInputChannels,
  UINT uNumOutputChannels)
  // call the LFO function; we only need ﬁ rst output
  m_LFO.doOscillate(&fYn, &fYqn);
  // setup necessary variables
  ﬂ oat fGnL = 1.0;
  ﬂ oat fGnR = 1.0;
  ﬂ oat fMonoIn = 0.0;
  // branch
  if(m_uMode ==      Tremolo)
  fGnL = calculateTremoloGainFactor(fYn);
   else  // panner sums inputs
  {
  if(uNumInputChannels == 1)
  fMonoIn = pInputBuffer[0];
  else
  fMonoIn = 0.5*(pInputBuffer[0] + pInputBuffer[1]);
  calculatePannerGainFactor(fYn, &fGnL, &fGnR);
  }
  // do MONO (Left) channel
  pOutputBuffer[0] = pInputBuffer[0]*fGnL;
  // Mono-In, Stereo-Out (AUX Effect)
  if(uNumInputChannels == 1 && uNumOutputChannels == 2)
  pOutputBuffer[1] = pOutputBuffer[0]; // just copy
  // Stereo-In, Stereo-Out (INSERT Effect)
  if(uNumInputChannels == 2 && uNumOutputChannels == 2)
  {
  // branch
  if(m_uMode == Tremolo)
  {
  // do right channel, use same gain as left
  pOutputBuffer[1] = pInputBuffer[1]*fGnL;
  }
494  Chapter 14
  else
  {
  }
  }
  return true;
  }
  // do right channel, its value
  pOutputBuffer[1] = pInputBuffer[1]*fGnR;
 Build and test the plug-in. Try raising the LFO rate maximum from 20 Hz to something like
200 or 2 kHz; the results are quite interesting.
     14.2    Design a Ring Modulator Plug-In
  Ring modulation  is accomplished by multiplying the input signal against a sinusoid called
the  carrier , as shown in  Figure 14.2 . This is technically amplitude modulation or frequency
mixing. The resulting signal contains sum and difference pairs of each frequency present in
the signal. For example, if the carrier is 250 Hz and the input signal is 1 kHz, the output will
consist of two frequencies, 1 kHz ] 250 Hz 5 750 Hz and 1 kHz 1 250 Hz 5 1.25 kHz. The
original input signal at 1 kHz is gone. For complex audio signals, this process happens for
every frequency component that the signal contains. The resulting output can sound nothing
like the input. Drums change their pitches and full spectrum signals become obliterated.
Because of the sum and difference pair, aliasing will occur when the sum signal goes outside
Nyquist. One way to handle the aliasing is to band-split the signal just like the spectral
compressor and only process the lower frequencies. Though not a perfect solution, it will
at least alleviate some of the aliasing. The ring modulator project here only implements the
frequency modulation without addressing aliasing problems.
  14.2.1  Project: RingModulator
 Create a project and name it “RingModulator.” There are no other objects or options to add.
   14.2.2  RingModulator: GUI
 We need frequency and depth controls for the carrier oscillator in  Table 14.2 . The depth
control will increase or decrease the amount of modulation.
Input
Output
Carrier
 Figure 14.2:    The ring modulator block diagram.
Miscellaneous Plug-Ins  495
  Table 14.2:    RingModulator plug-in GUI controls.
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
Mod Freq
Hz
ﬂ oat
m_fModFrequency
100
5000
1000
Control Name
Units
Variable Type
Variable Name
Low Limit
High Limit
Initial Value
Mod Depth
%
ﬂ oat
m_fModDepth
0
100
50
   14.2.3  RingModulator.h File
 We only need to add a single wave table object to implement the carrier oscillator.
  // Add your code here: -------------------------------------------------- //
  // the Carrier Oscillator
  CWaveTable m_Oscillator;
  // END OF USER CODE ----------------------------------------------------- //
   14.2.4  RingModulator.cpp File
  prepareForPlay()
(cid:129)
(cid:129)
   Initialize the oscillator.
   Forward the preareForPlay() method to the oscillator.
  bool __stdcall CRingModulator::prepareForPlay()
  {
  // Add your code here:
  m_Oscillator.m_fFrequency_Hz = m_fModFrequency;
  m_Oscillator.m_uPolarity = 0;  // 0 = bipolar, 1 = unipolar
  m_Oscillator.m_uTableMode = 0; // normal, no band limiting
  m_Oscillator.m_uOscType = 0; // 0 = sin()
  m_Oscillator.setSampleRate(m_nSampleRate); // really important!
  // the prepareForPlay() calls reset() and cookFrequency()
  m_Oscillator.prepareForPlay();
  return true;
  }
   userInterfaceChange()
(cid:129)
(cid:129)
   If the frequency slider moves, change the oscillator frequency and call the cooking
 function.
   The depth control is a direct-control variable, so it does not need any attention here.
496  Chapter 14
  bool __stdcall CRingModulator::userInterfaceChange(int nControlIndex)
  {
  // decode the control index, or delete the switch and use brute force calls
  switch(nControlIndex)
  {
  case 0:
  {
  m_Oscillator.m_fFrequency_Hz = m_fModFrequency;
  m_Oscillator.cookFrequency();
  break;
  }
  default:
  break;
  }
  return true;
  }
   processAudioFrame()
(cid:129)
(cid:129)
   Generate a new carrier oscillator value.
   Multiply the input signal against it.
  bool __stdcall CRingModulator::processAudioFrame(ﬂ oat* pInputBuffer, ﬂ oat*
  pOutputBuffer, UINT uNumInputChannels,
  UINT uNumOutputChannels)
  {
  // Do LEFT (MONO) Channel; there is always at least one input/one output
  // generate carrier
  ﬂ oat yn, yqn;
  m_Oscillator.doOscillate(&yn, &yqn);
  // multiply
  pOutputBuffer[0] = pInputBuffer[0]*yn*(m_fModDepth/100.0);
  // Mono-In, Stereo-Out (AUX Effect)
  if(uNumInputChannels == 1 && uNumOutputChannels == 2)
  pOutputBuffer[1] = pOutputBuffer[0];
  // Stereo-In, Stereo-Out (INSERT Effect)
  if(uNumInputChannels == 2 && uNumOutputChannels == 2)
  pOutputBuffer[1] = pInputBuffer[1]*yn*(m_fModDepth/100.0);
  return true;
  }
 Build and test the plug-in. You may ﬁ nd it to be a remarkable effect or you may think it
sounds awful. Some things you could do to modify this project include:
(cid:129)
(cid:129)
(cid:129)
   Use multiple carrier frequencies.
   Use series or parallel multiple modulation.
   Use an LFO to modulate the carrier frequency.
Miscellaneous Plug-Ins  497
     14.3    Design a Wave Shaper Plug-In
 Wave shaping is a method of nonlinear processing that adds harmonics to the input signal
as a result. It is used in synthesizers and distortion algorithms. Because harmonics are
added to the signal, wave shapers suffer from aliasing problems, like the ring modulator.
The process of wave shaping is simple because no memory elements are required. You
treat the input signal  x  as the argument to a function. The function is often a trigonometric
or polynomial function. An interesting function to use is the arctangent. It is part of a
family of functions called  sigmoid functions . Sigmoid functions produce S-shaped curves.
The amplitude transfer function of a vacuum tube generally resembles a sigmoid, so this
function can be used to try to mimic a tube sound.  Figure 14.3  shows the arctangent
function atan( kx ), where  k  controls the amplitude of the input value and thus the amount
of nonlinear processing applied. The exact equation is shown in  Equation 14.2 , where
a normalization factor has been added to restrict the output to the range of 21 to 11.
You can see in Figure 14.3 that with  k  5 1, the input/output relationship is nearly linear.
As  k  increases, the S-shaped curve emerges and adds gain. For example, with  k  5 5 and
 x ( n ) 5 0.25,  y ( n ) is about 0.6.
  y(n) 5
1
arctan(k)
 arctan(kx(n))
(14.2)
 Asymmetrical distortion can be implemented easily by using two different  k -values, one for
positive input samples and the other for negative ones. This asymmetrical distortion is found
in Class-A tube circuits. Cascading multiple stages will result in more harmonic distortion.
In some ampliﬁ ers, many stages are cascaded in series. Class A tube circuits also invert the
signal. This means that the asymmetrical distortion curves are also inverted in between each
stage. The resulting sound is quite different than simply cascading the modules without
y(n)
+1
y(n)
+1
–1
x(n)
+1
–1
x(n)
+1
–1
k = 1
–1
k = 5
 Figure 14.3 :    The input/output relationship for  Equation 14.1  with  k  5 1 and  k  5 5.
498  Chapter 14
inversion. We will design a plug-in that will implement the arctangent wave shaping and
allow the user to adjust the following:
(cid:129)
(cid:129)
(cid:129)
   The  k -value for both the positive and negative halves of the input signal
   The number of stages in series, up to 10
   Inverting or not inverting every other stage when cascaded
  14.3.1  Project: WaveShaper
 Create a project and name it “WaveShaper.” There are no other objects or options to add.
   14.3.2  WaveShaper: GUI
 Add the controls in  Table 14.3  to implement the GUI we will need for the WaveShaper
project.
  WaveShaper.h File
 There’s nothing to do in the .h ﬁ le as we require no additional variables, memory storage
spaces, methods, or objects!
   WaveShaper.cpp File
 The only method that needs to be altered is processAudioFrame().
  Table 14.3:    WaveShaper plug-in GUI controls.
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
+Atan Exp
ﬂ oat
m_fArcTanKPos
0.10
20
1
Value
Stages
ﬂ oat
m_nStages
1
10
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
Value
–Atan Exp
ﬂ oat
m_fArcTanKNeg
0.10
20
1
Value
Invert Stages
enum
m_uInvertStages
Enum String
OFF,ON
Miscellaneous Plug-Ins  499
  bool __stdcall CWaveShaper::processAudioFrame(ﬂ oat* pInputBuffer, ﬂ oat* pOutputBuffer,
  {
  // Do LEFT (MONO) Channel; there is always at least one input/one output
  // (I    NSERT Effect)
  ﬂ oat f_xn = pInputBuffer[0];
   UINT uNumInputChannels,
   UINT uNumOutputChannels)
  // cascaded stages
  for(int i=0; i<m_nStages; i++)
  {
  if(f_xn >= 0)
  f_xn = (1.0/atan(m_fArcTanKPos))*atan(m_fArcTanKPos*f_xn);
  else
  f_xn = (1.0/atan(m_fArcTanKNeg))*atan(m_fArcTanKNeg*f_xn);
  // invet every other stage
  if(m_uInvertStages == ON && i%2 == 0)
  f_xn *= -1.0;
  }
  pOutputBuffer[0] = f_xn;
  // Mono-In, Stereo-Out (AUX Effect)
  if(uNumInputChannels == 1 && uNumOutputChannels == 2)
  pOutputBuffer[1] = pOutputBuffer[0];
  // Stereo-In, Stereo-Out (INSERT Effect)
  if(uNumInputChannels == 2 && uNumOutputChannels == 2)
  {
  ﬂ oat f_xn = pInputBuffer[0];
  // cascaded stages
  for(int i=0; i<m_nStages; i++)
  {
  if(f_xn >= 0)
  else
  f_xn = (1.0/atan(m_fArcTanKPos))*atan(m_
fArcTanKPos*f_xn);
  f_xn = (1.0/atan(m_fArcTanKNeg))*atan(m_
fArcTanKNeg*f_xn);
  // invet every other stage
  if(m_uInvertStages == ON && i%2 == 0)
  f_xn *= −1.0;
  }
  pOutputBuffer[1] = f_xn;
  }
  return true;
  }
500  Chapter 14
 Build and test the plug-in. Note that in order to fully replicate a tube circuit you will need
much more than just the distortion component, as tube circuits are amplitude sensitive and
band limited. However, this plug-in is a good place to start for experimenting with nonlinear
processing.
   Bibliography
  Black, H. S. 1953.  Modulation Theory . New York: Van Nostrand-Reinhold.
  Dodge, C. and T. Jerse. 1997.  Computer Music Synthesis, Composition and Performance , Chapter 4. New York:
Schirmer.
  Roads, C. 1996.  The Computer Music Tutorial , Chapter 2. Cambridge, MA: The MIT Press.
  U.S. Navy. 1973.  Basic Electronics ,  Rate Training Manual NAVPES, 10087-C . New York: Dover Publications.
 The VST® and AU® Plug-In APIs
  APP EN DIX A
 Once you learn how to design plug-ins with RackAFX you will ﬁ nd that learning other
plug-in application programming interfaces (APIs) is fairly straightforward. The main
reason is that most of the plug-in APIs share very similar structuring and function calls as
RackAFX. An analogy might be learning to ﬂ y: once you have the fundamentals of solo
ﬂ ight internalized, learning to ﬂ y different  kinds  of airplanes is a matter of understanding
the peculiarities of each one. You don’t need to worry about the principles of lift, drag,
pitch, and yaw after you’ve mastered them. The current popular plug-in APIs include
Steinberg’s Virtual Studio Technology  (VST), Apple Computer’s Audio Units (AU) , and
Avid’s Avid Audio eXtension (AAX) ®  formats. If you want to write for these APIs, the
ﬁ rst step is to set up a developer account with the manufacturers via their websites. Once
you have an account you can download their API documentation and SDKs (software
development kits), which include sample code templates. The VST and AU APIs are
publicly documented, while AAX is not. Like RackAFX, VST plug-ins are written in
straight C++ without the need for external frameworks or foundation classes. AU plug-ins
are not and require a good working knowledge of Mac programming to develop. Once you
have a RackAFX plug-in, how can you make it work in other plug-in APIs?
  A.1    Compiling as a VST Plug-In in Windows
 The simplest way to use your RackAFX plug-in in a third-party client is to take advantage
of the fact that you can set your project to compile as a dynamic link library  (DLL) that
is compatible with both RackAFX and VST on the Windows operating system (OS). You
can enable this functionality when you create your project or later in development. If you
already have a project in development, use File S Edit Project or the Edit button on the
user interface (UI). On the edit project dialog, check the “Make VST Compatible” box, as
shown in  Figure A.1 . The resulting DLL will work with both RackAFX and any Windows
VST client. Just copy the DLL from your /PlugIns folder to the designated VST folder
for the client.
 The GUI options are as follows:
•
   Use VST client’s GUI: VST clients will provide you with a  default interface  whose
look and feel are determined by the client’s manufacturer. These range from a simple
501
502  Appendix A
Figure A.1: The VST Compatible switch reveals three options for the GUI.
•
•
bank of sliders to more elaborate GUIs. The client will query your plug-in for control
information and build the GUI at run time. You do not have to provide any extra code
for this option.
   Use RackAFX custom GUI: If you have the proper version of RackAFX, this  button
will be enabled. Choosing this option will allow the GUI you create with GUI
 designer ( Appendix B ) to be used by the VST client. It will look and work identically
to the one you create in RackAFX. You do not have to provide any extra code for
this option.
   I will write my own GUI: If you have the skills to do this, then you can check this box
and write your own GUI. The client will call a method on your plug-in to launch your
GUI. All details of the GUI are your responsibility and you must provide the code and
have the necessary resource editor.
 For the last option, ﬁ rst look in the PlugIn.h ﬁ le and ﬁ nd the ﬂ ag and three methods you need
to override.
  // set this to TRUE if you have your own GUI to use; not for RackAFX GUIs
   bool m_bUserCustomGUI;
 First, set the m_bUserCustomGUI ﬂ ag to “true” in your constructor. This will tell the client
that you have your own GUI. Next, override the following three methods:
  // for user (NOT RackAFX) generated resources/GUIs
  virtual void __stdcall showGUI(HWND hParentWnd);
  virtual void __stdcall hideGUI(HWND hParentWnd);
  virtual void __stdcall refreshGUI(HWND hParentWnd);
•
•
•
   The client calls showGUI() to show it.
   The client calls hideGUI() to hide it.
   The client calls refreshGUI() to update it. See  Appendix B  for more information about
updating.
The VST® and AU® Plug-In APIs  503
 In the three methods, the argument is the client’s window handle. You will need to create your
child window using the client’s handle as your parent. If you don’t know what that means,
you probably shouldn’t be using this option.
   A.2    Wrapping Your RackAFX Plug-In
 Your RackAFX plug-in will compile on Windows (Visual Studio) or Mac (Xcode) because it
is pure C++ with some # deﬁ ne statements added to split out the Windows- and Mac-speciﬁ c
components. The audio signal processing and GUI component objects are all done in C++
so your plug-in object can be wrapped with an outer C++ object that translates function calls
from the VST or AU client into RackAFX function calls. This is exactly what happens when
you choose the “Make VST Compatible” option; when the client requests a VST object, it
receives one with your plug-in embedded inside as a member object. The wrapper object
then forwards calls to your plug-in and returns information or audio data to the client, as
shown in  Figure A.2 . Your plug-in exposes an interface of public methods (gray boxes). The
wrapper exposes its API speciﬁ c methods (white boxes) which the client calls. The translation
usually involves a simple forwarding method call. Sometimes more than one wrapper method
connects to a single plug-in method. Other times, the wrapper method only needs to return a
success code or special string so there is nothing to call on the plug-in object.
 If you want to write a wrapper object, it is fairly straightforward, as you will see in Section A.3
when we compare the different API function calls. However, you need to know a bit more about
how your GUI objects are designed so you can use the GUI controls you set up in RackAFX.
Alternatively, you can implement your own scheme. When you set up a control in RackAFX,
Client
Wrapper  Plug-In
RackAFX  Plug-In
Constructor
Destructor
prepareForPlay()
userlnterfaceChange()
processAudioFrame()
Figure A.2: A wrapper object exposes a client speciﬁ c interface and routes calls to
and from the plug-in.
Wrapper  function
RackAFX  function
504  Appendix A
you are really creating a new C++ object that is added to a linked list. The linked list is a
member of your CPlugIn class named “m_UIControlList.” It contains a list of C++ CUICtrl
 objects. A CUICtrl object can represent one of the following:
•
•
•
   Slider
   Radio button bank
   Meter
 The simplest way to explain is by example. Look in your initUI() method for any plug-in and
you will see the instantiation and initialization of the GUI objects. Here’s the ﬁ rst part of the
volume slider code from the very ﬁ rst project. The highlights are in bold. Remember,  do not
ever edit this code manually .
   m_fVolume = 0.750000;
   CUICtrl ui0;
   ui0.uControlType = FILTER_CONTROL_CONTINUOUSLY_VARIABLE;
   ui0.uControlId = 0;
   ui0.fUserDisplayDataLoLimit = 0.000000;
   ui0.fUserDisplayDataHiLimit = 1.000000;
   ui0.uUserDataType = ﬂ oatData;
   ui0.fInitUserIntValue = 0;
   ui0.fInitUserFloatValue = 0.750000;
   ui0.fInitUserDoubleValue = 0;
   ui0.fInitUserUINTValue = 0;
   ui0.m_pUserCookedIntData = NULL;
   ui0.m_pUserCookedFloatData = &m_fVolume;
   ui0.m_pUserCookedDoubleData = NULL;
   ui0.m_pUserCookedUINTData = NULL;
   ui0.cControlUnits = “                          ”;
   ui0.cVariableName = “m_fVolume”;
   ui0.cEnumeratedList = “SEL1,SEL2,SEL3”;
   ui0.dPresetData[0] = 0.000000;ui0.dPresetData[1] <SNIP SNIP SNIP>
   ui0.cControlName = “Volume”;
 The very ﬁ rst line initializes the underlying variable, m_fVolume, as you set it up when you
created the slider. The control type is FILTER_CONTROL_CONTINUOUSLY_VARIABLE,
which means a continuous controller. The uControlID is 0—this is the value that is passed
to userInterfaceChange() when a control is manipulated. This “unique ID” paradigm for
identifying the control is universal among the other plug-in APIs. The connection to the
variable itself is via a pointer. Since this is a ﬂ oat variable, m_pUserCookedFloatData is set
to the address of the underlying variable. This is how RackAFX manipulates it for you. When
you wrap your plug-in you can use this to control the variable as well. Of course, you can
always use other schemes. The units variable codes several items and will need to be trimmed
of white spaces before you use it (do not alter its length in the object, however). Thus, you
can see the fundamentals about each variable: the ID, minimum, maximum, and initial value,
and the name and units. These are all used in the other plug-in APIs as well.
The VST® and AU® Plug-In APIs  505
 RackAFX automatically groups all the legal VST controls at the top of the control list. It
 provides you with these methods so you can iterate through your GUI objects when you write
your wrapper translation code.
 RackAFX automatically sorts the GUI component objects by type. The sliders and radio
button banks are supported by the default GUIs for the VST and AU formats but the meters
are not. The m_UIControlList is deﬁ ned in pluginconstants.h and exposes several methods for
you to use:
   int count();
   int countLegalVSTIF();
   CUICtrl* getAt(int nIndex);   // returns a pointer to a GUI object at a list index
 // returns the total number of GUI objects
// returns the number of legal VST Default GUI objects
 The CPlugIn object also has some helper functions for you to use. Once you know
the ID values for the controls, you can use the following CPlugIn methods to access
the objects:
   void setParameter (UINT index, ﬂ oat value);
   ﬂ oat getParameter (UINT index);
 // set a parameter 0 -> 1
 // get a parameter 0 -> 1
 These two methods are very important because they mimic the way a VST client will
be getting or setting values through the default interface.  The index value is the index in
the linked list and not the ID value . Finally, for Windows plug-ins, you will often need
to know the name of the directory where your DLL exists for dealing with presets or
loading/saving custom information. CPlugIn exposes the method for you to use to get
that folder name.
   char* getMyDLLDirectory();
// return the DLL’s owning directory
   A.3    Comparison of Objects/Methods/GUIs
 RackAFX, VST, and AU share many similarities. First, they all require that you derive your
plug-in from a base class (or classes) that the manufacturer speciﬁ es. They also implement
the same basic set of functionality that you’ve been coding all along. The base classes are
shown in  Table A.1 . The comparison of methods is shown in  Table A.2 .
  Table A.1:    Comparison of base classes in the APIs.
API
RackAFX
VST
AU
Base Class(es)
CPlugIn
AudioEffect, AudioEffectX
AUBase, AUEffectBase,
AUKernelBase
506  Appendix A
  Table A.2:    Comparison of the functions and the methods that implement them.
Function
Instantiation
RackAFX
CPlugIn()
VST
AudioEffectX()
Destruction
~CPlugIn()
~AudioEffectX()
Initialization
Prepare For Play
prepareForPlay()
prepareForPlay()
resume()
setSampleRate()
updateSampleRate()
resume()
Process Audio
processAudioFrame()
processReplacing()
AU
AUEffectBase()
AUKernelBase()
~AUEffectBase()
~AUKernelBase()
Initialize()
N/A
Process()
ProcessBufferList()
Get UI Control Value
Set UI Control Value
Update GUI (see
Appendix B)
getParameter()
(optional)
userInterfaceChange()
(optional)
getParameter()
GetParameter()
setParameter()
SetParameter()
sendUpdateGUI()
updateDisplay()
N/A
N/A
Get the DLL’s Directory
getMyDLLDirectory()
getDirectory()
Show a Custom GUI
showGUI()
dispatcher()
AudioUnitGetProperty()
 RackAFX, VST, and AU all provide for a default standard interface—a GUI that the user
does not have to code. For RackAFX, you can optionally design your own custom GUI
with the GUI designer ( Appendix B ) that requires no coding; alternatively, you can design
your own (see Section A.1). VST and AU also allow you to design your own GUI. VST
provides a GUI class of objects that you can optionally use in your coding that allows for
cross-platform compatibility. AU only runs on Mac and does not provide cross-platform
compatibility. The default interface setup for RackAFX involves you simply ﬁ lling in a
properties dialog for the components you want; the GUI object code is written for you. In
VST and AU, you must set up a way to describe your controls. In all three formats, when
you load your plug-in the client queries it for its default interface controls and then builds
the GUI at run time. In RackAFX, the client simply iterates through your linked list of
objects and sets up the GUI.
   A.4    VST Plug-in without Wrapping
 Let’s start by pretending to write a VST plug-in from scratch  without  wrapping any RackAFX
code—of course, you can always cut and paste processing or cooking functions directly
from your source, since it is pure C++. The pretend plug-in will be a delay with feedback
called CDelay.
The VST® and AU® Plug-In APIs  507
  A.4.1  Default GUI
 First, you deﬁ ne an enumerated list of unsigned integer-type (UINT) constant values in your
Plugin.h ﬁ le where “Plugin” is the name and you have derived it from the AudioEfffectX
base class. These constants are the control ID values the host will use when querying about,
getting, or setting a control variable. A delay plug-in might have delay time, feedback, and
wet/dry mix controls, so you might deﬁ ne your enum as follows:
   enum
   {
   };
 uDelayTime,
 uFeedBack,
 uWetDry,
 /* ID = 0 */
/* ID = 1 */
/* ID = 2 */
 uNumParams
/* Always last = 3 = number of controls */
 Next, you declare a  parallel set of member variables  that will be used in your plug-in. These
are for cooked data values.
   ﬂ oat m_fDelayTime;
   ﬂ oat m_fFeedBack;
   ﬂ oat m_fWetDry;
 Finally, you declare the minimum and maximum values for each control.
   ﬂ oat m_fMinDelayTime, m_fMaxDelayTime;
   ﬂ oat m_fMinFeedBack, m_fMaxFeedback;
   ﬂ oat m_fMinWetDry, m_fMaxWetDry;
 The last item in the enumeration keeps track of the control count by virtue of being last in the
list. During operation, the VST client will use these ID values passed as the variable “index”
in the following methods. First, there are three methods to ﬁ ll out that describe the name,
display, and units for each control.
1.
   getParameterName: You create the name of the control as a string.
   void CDelay::getParameterName (long index, char *label)
   {
 switch (index)
 {
 case uDelayTime:strcpy (label, “Delay”); break;
 case uFeedBack: strcpy (label, “FeedBack”); break;
 case uWetDry:     strcpy (label, “Wet/Dry”); break;
 }
2.
   }
   getParameterDisplay: You create a string that represents the value to display on
the UI; the ﬂ oat2String() method is a helper function that is provided in the VST
 object ﬁ le.

508  Appendix A
 All default VST controls use values from 0.0 to 1.0 regardless of how they map in your actual
plug-in. You must write the cooking and uncooking functions that will convert your internal
variable values to and from values from 0.0 to 1.0.
    void CDelay::getParameterDisplay (long index, char *text)
   {
 switch (index)
 {
 case uDelayTime: ﬂ oat2string (m_fDelayTime, text);  break;
break;
 case uFeedBack:  ﬂ oat2string (m_fFeedBack, text);
break;
 case uWetDry:     ﬂ oat2string (m_fWetDry, text);
 }
   }
   getParameterLabel: You create a string that represents the units for the control.
3.
    void CDelay::getParameterLabel (long index, char *label)
   {
 switch (index)
 {
 case uDelayTime:strcpy (label, "mSec");
 case uFeedBack: strcpy (label, "%");
 case uWetDry:   strcpy    (label, "%");
break;
break;
break;
 }
   }
 Next, there are two methods you need to implement to allow the client to get and set your
control variables. However, these methods deal with  raw VST client data .
1.
   getParameter: You return the raw data (0–1) for the control.
    ﬂ oat CDelay::getParameter (long index)
   {
   ﬂ oat fRawData = 0;
   switch (index)
   {
   case uDelayTime:
   {
   ﬂ oat fDiff = m_fMaxDelayTime – m_fMinDelayTime;
   fRawData = (m_fDelayTime – m_fMinDelayTime)/fDiff;
   break;
   }
   // same thing for the other variables
   }
   return fRawData;
   }
