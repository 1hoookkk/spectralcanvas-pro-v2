# Audio Plugins C++ (Pirkle) — Chunk 36/73

> Source: `Designing Audio Effect Plug-Ins in C++_ With Digital Audio Signal Processing Theory - PDF Room.pdf` · Pages ~1-560 · Extractor: pdfminer.six
> Target ≈ 2800 words, Overlap = 200 words · Actual = 2589

> Note: DSP textbook content for SpectralCanvasPro RT-audio knowledge base

---
244  Chapter 7
 Rebuild and test the project and you now have a stereo version of the previous project.
Hopefully, you have a better idea of how powerful it can be to create modules that are
combined, though it does take a bit of extra work on the module since you have to think
ahead and implement functions or variables that are not required for standalone (simple)
plug-in operation. Next, we’ll exercise the module by converting this plug-in to a crossed
feedback delay. We’re going to do this in a speciﬁ c way so we can later use an enumerated
variable to switch between normal and crossed-feedback operation.
     7.7    Design a Stereo Crossed-Feedback Delay Plug-In
 A crossed-feedback delay is implemented by crossing the feedback paths of the two delay
lines. We will add this functionality to the existing StereoDelay project. This is where we will
use the external feedback option that we built into the module. In  Figure 7.31  you can trace
the feedback paths to see that they are crossed into the opposite delay line. You will be
surprised at how easy it is to convert the delay into a crossed-feedback delay:
•
•
   Enable the external feedback function with the boolean ﬂ ag.
   Use the getCurrentFeedbackOutput() and setCurrentFeedbackInput() functions to “cross
the beams” of the feedback paths.
m_DDL_Left
Dry
x(n)  Left
∑
z  –D
Wet
∑
y(n)  Left
fb_in(n)
fb_out(n)
m_DDL_Right
x(n)  Right
∑
fb_in(n)
fb_out(n)
fb
z –D
fb
Dry
Wet
∑
y (n)  Right
Delay
Feedback
Wet/Dry
109.38 mSec
60.16%
50.00%
Type
CROSS
 Figure 7.31:    Block diagram of the crossed-feedback delay.

Delay Effects and Circular Buffers   245
   7.8    Enumerated Slider Variables
 You can see from  Figure 7.31  that there is a new slider control for the GUI to select
between normal and crossed-feedback operation. You can take advantage of RackAFX’s
enumerated UINT variable to make a slider that selects between multiple enumerated
values, like a multi-position switch. In our case, we only have two positions right now:
normal and cross.
 Right-click on a slider to bring up the properties dialog and add a new slider/variable
combination, as shown in  Figure 7.32 . Choose “enum” for the data type and create a variable
called m_uDelayType—the variable will be an unsigned integer type (UINT). The low, high,
and initial value cells will no longer be editable; RackAFX will ﬁ gure them out depending on
what you type in the enumerated variable box. You type in the values separated by commas.
The ﬁ rst value will be the default value.
 In the box at the bottom, type in the strings which will be shown in the slider control
(keep them short); you can have as many enumerated variables as you want, but in this
Slider Properties
Type
learn MIDI
Channel
--n/a-
Control Type
--n/a-
Control Name  --n/a--
UI Item
uControiType
uControiiD
Control Name
Units
Data Type
Variable Name
Control low limit
Control High limit
Initial Value
MIDI Control
MIDI Control Channel
Property
Slider
3
Type -enum
m_uDelayType
......
u.uu
(
\.
1.00
0.00
false
1
MIDI Control Type
Continous Controller
MIDI Controller Number/Name
3 Continuous controller #3
automatic (recommended)
Control Method
Menu Group
none - -
STRINGS <ep"atod by '""k OIWI  CROS_ : )
'
Enter enumerated list of
or use the default. All sp
aces will be removed from
m and Maximum values will be
strings. The Initial,  Minimu
I
automatically calculated.  NOTE.  max1mum IS  256 stnngs.
.
.
.
,
The the first STRING will be the Initial (default) value.
The strings will  switch as  the user moves the slider.
 Figure 7.32:     The enumerated UINT data type.
OK
I  I
I  I  Cancel
I
I
Copy Existing:
I none
[ ~
I Remove Ctrl  I

246  Chapter 7
case, we only need NORM and CROSS. Go to your plug-in’s .h ﬁ le to see the new
variables:
  // ADDED BY RACKAFX -- DO NOT EDIT THIS CODE!!! -------------------------------- //
  //  **--0x07FD--**
  ﬂ oat m_fDelay_ms;
  ﬂ oat m_fFeedback_pct;
  ﬂ oat m_fWetLevel_pct;
   UINT m_uDelayType;
   enum{NORM,CROSS};
  // **--0x1A7F--**
  // ----------------------------------------------------------------------------- //
 For enumerated variables, the ﬁ rst one in the list will be the default, with the slider at the
bottom. The other strings will appear in sequence when you move the slider up. The UINT
will automatically be updated. We can use this as a directly controlled variable so there’s
nothing to add in userInterfaceChange(). Make the edits discussed next.
  7.8.1  Constructor
•
   Initialize the delay type to NORM.
  CStereoDelay::CStereoDelay()
  {
    <SNIP SNIP SNIP>
    // Finish initializations here
    m_DDL_Left.m_bUseExternalFeedback = false;
    m_DDL_Right.m_bUseExternalFeedback = false;
    // set and (true) cook the delays
    setDelayVariables(true);
     // init the delay type
     m_uDelayType = NORM;
  }
   7.8.2  PrepareForPlay()
 Nothing to do; we don’t care what mode we’re in and don’t want to reset the mode each time
the user plays audio.
   7.8.3  UserInterfaceChange()
 Nothing to do; we are using this as a direct control variable.

Delay Effects and Circular Buffers   247
   7.8.4  ProcessAudioFrame()
•
•
   Use the enumerated variable in a switch/case statement to modify the feedback as
required. For CROSS operation:
•
•
•
   Call the processAudioFrame() functions on the DDL modules.
   Set the external feedback  ﬂ ag to true.
   Get the left feedback output and write it to the right feedback input.
   Get the right feedback output and write it to the left feedback input.
   bool __stdcall CStereoDelay::processAudioFrame(ﬂ oat* pInputBuffer, ﬂ oat*
 pOutputBuffer, UINT uNumInputChannels, UINT uNumOutputChannels)
  {
   switch(m_uDelayType)
   {
   case CROSS:
     // CROSS FB DELAY --- NOTE: MUST HAVE STEREO FILE! ---------------- //
     m_DDL_Left.m_bUseExternalFeedback = true;
     m_DDL_Right.m_bUseExternalFeedback = true;
     // cross the Feedbacks
      m_DDL_Left.setCurrentFeedbackInput(m_DDL_Right.getCurrent
FeedbackOutput());
      m_DDL_Right.setCurrentFeedbackInput(m_DDL_Left.getCurrent
FeedbackOutput());
     break;
   case NORM: // clear the ﬂ ags
     m_DDL_Left.m_bUseExternalFeedback = false;
     m_DDL_Right.m_bUseExternalFeedback = false;
     break;
   default:  // deault is NORM operation; clear the ﬂ ags
     m_DDL_Left.m_bUseExternalFeedback = false;
     m_DDL_Right.m_bUseExternalFeedback = false;
     break;
   }
  // SHARED CODE ----------------------------------------------------------- //
  // Do LEFT (MONO) Channel; there is always at least one input/one output
  // forward call to sub-object: pInput, pOutput, 1 input ch, 1 output ch
   m_DDL_Left.processAudioFrame(&pInputBuffer[0], &pOutputBuffer[0], 1, 1);
  // Mono-In, Stereo-Out (AUX Effect)
  if(uNumInputChannels == 1 && uNumOutputChannels == 2)
     pOutputBuffer[1] = pOutputBuffer[0]; // just copy

248  Chapter 7
  // Stereo-In, Stereo-Out (INSERT Effect)
  if(uNumInputChannels == 2 && uNumOutputChannels == 2)
    // forward call to sub-object pInput, pOutput, 1 input ch, 1 output ch
     m_DDL_Right.processAudioFrame(&pInputBuffer[1], &pOutputBuffer[1], 1, 1);
  return true;
  }
 Rebuild and test the plug-in; exercise the new delay type control as well.
 Recapping some of the things you learned:
•
•
•
•
   Declaring, ﬂ ushing, and manipulating delay lines as circular buffers.
   Implementing fractional delay times using interpolation.
   Using a plug-in as a child module for a parent plug-in.
   How to add an enumerated UINT variable for type selection or using a slider as a
 multi-pole switch.
    7.9    More Delay Algorithms
 Here are some ideas for a more complex DDL module and other delay plug-ins.
  7.9.1  Advanced DDL Module
 The more ﬂ exible module in  Figure 7.33  allows the parent to have access to the input and pre-
wet output of the delay line and the input and output of the feedback path. Another module
(low-pass ﬁ lter, for example) could be inserted here or it could be used for more crossed-
delay modes.
   7.9.2  Delay with LPF in Feedback Loop
 Analog delays suffer a high frequency loss on each path through the delay line/feedback
path. This can be modeled with a LPF in the feedback path, as shown in  Figure 7.34 . You can
x(n)
ext_x(n)
fb_in(n)
fb_out(n)
Dry
∑
Z  –D
Wet
∑
y(n)
 Figure 7.33:     A DDL module with more input and feedback path options.
fb
delay_out
fb_in2

Delay Effects and Circular Buffers   249
try ﬁ rst- or second-order ﬁ lters with or without resonance for a variety of interesting delay
effects. For other ideas, try changing the type (LPF, high-pass ﬁ lter, band-pass ﬁ lter, band-
stop ﬁ lter) or location (before or after the  fb  coefﬁ cient). Be careful with adding resonant or
high-gain ﬁ lters in the feedback loop. You could also inject some ﬁ ltered white noise into the
feedback path to simulate the noisy characteristics of typical analog delays.
   7.9.3  Multi-Tap Delay
 The multi-tap delay line shown in  Figure 7.35  produces four separate delays from one
unit. Only the longest delay is fed back in this version. You create the multiple taps by
simply declaring more read index values (m_nReadIndexTap_1, m_nReadIndexTap_2,
etc..); the multiple taps are used and incremented exactly the same as the single tap case.
Dry
x (n)
∑
Z –D
Wet
∑
y(n)
LPF
fb
 Figure 7.34:     An analog delay modeled with an LPF in the feedback loop.
x(n)
∑
fb
Z  –D
Dry
Wet
∑
y (n)
Tap1
Tap2
Tap3
Tap4
 Figure 7.35:     A four-tap multi-tap delay line.

250  Chapter 7
You can also experiment with multiple feedback paths, ﬁ lters, or setting your tap times using
BPM and musical rhythms (e.g., delays on eighth and quarter notes).
   7.9.4  Ping-Pong Delay
  Figure 7.36  shows how the ping-pong delay cycles the delay back and forth between the left
and right channels.
   7.9.5  LCR Delay
 The LCR delay in  Figure 7.37  based on the Korg Triton ®  has both LPF and HPF  in the
feedback path (both are switchable or bypass-able) for multiple feedback tone shaping
options.
Left x(n)
(cid:153)
Right x(n)
(cid:153)
LEFT
z  -D
fb
fb
RIGHT
z  -D
Dry
Wet
(cid:153)
Left y(n)
Wet
(cid:153)
Right y(n)
Dry
 Figure 7.36:     The ping-pong delay builds on the cross-feedback delay by crossing the
inputs as well as the feedback paths to produce the back and forth ping-pong
effect. You will probably want to design the advanced DDL module
ﬁ rst and use its input, output, and feedback ports.

Delay Effects and Circular Buffers   251
Dry
Left x(n)
LEFT
Z -D
Wet
(cid:153)
Left y(n)
(cid:153)
(cid:153)
CENTER
Z -D
Wet
LPF
HPF
fb
Right x(n)
RIGHT
Z -D
Wet
(cid:153)
Right y(n)
 Figure 7.37:     The LCR delay, based on the LCR delay in the Korg Triton ® .
Dry
    Bibliography
 Coulter, D. 2000.  Digital Audio Processing , Chapter 11. Lawrence, KS: R&D Books.
DSP56KFAM/AD. Schomberg, ON: Motorola, Inc.
Korg, Inc. 2000. Triton-Rack Parameter Guide, Tokyo: Korg, Inc.
Motorola, Inc. 1992. DSP56000 Digital Signal Processor Family Manual.
Roads, C. 1996. The Computer Music Tutorial, Chapter 3. Cambridge, MA: The MIT Press.

Audio Filter Designs: FIR Filters
   CHAP TE R 8
    Inﬁ nite impulse response (IIR) ﬁ lters have several attractive properties:
•
•
•
•
•
   They only require a few delay elements and math operations.
   You can design them directly in the  z -plane.
   You can use existing analog designs and convert them to digital with the Bilinear
z-Transform (BZT) ; the fact that IIR topologies somewhat resemble the signal ﬂ ow in
analog ﬁ lters emphasizes their relationship.
   You can get extreme resonance/gain by placing poles very near the unit circle.
   You can make ﬁ lters, EQs, and so on with controls that link to the coefﬁ cients directly or
indirectly for real-time manipulation of the plug-in.
 Their main drawback is that they can become unstable and blow up, or oscillate. Their
impulse responses (IRs) can be inﬁ nite. Finite impulse response (FIR) ﬁ lters have all zeros
and a ﬁ nite IR. They are unconditionally stable so their designs can never blow up. However,
they can put out all zeros, a constant value, a series of clicks or pulses, or other erroneous
output, but they don’t actually go unstable.
 You can also make a linear phase ﬁ lter with an FIR, just like the simple feed-forward (FF)
topology you analyzed in  Chapters 5  and  6 ; a linear phase ﬁ lter is impossible to make with
an IIR topology, although you can approximate it by adding phase compensation ﬁ lters.
However, the one thing that separates FIR ﬁ lters from all other kinds, including analog
counterpart ﬁ lters, is that their coefﬁ cients a 0 , a 1 , …, a N  are the IR of the ﬁ lter. You proved
that when you manually pushed an impulse through the simple FF ﬁ lter in  Chapter 4 , and
then again when taking the  z  transform of the IR of the same ﬁ lter in  Chapter 5 .
  8.1    The IR Revisited: Convolution
 In  Chapter 1  you saw how the digitized signal was reconstructed into its analog version by
ﬁ ltering through an ideal low-pass ﬁ lter (LPF). When the series of impulses is ﬁ ltered, the
resulting set of sin( x )/ x  pulses overlap with each other and their responses all add up linearly.
The addition of all the smaller curves and damped oscillations reconstructs the inter-sample
curves and damped ﬂ uctuations ( Figure 8.1 ).
253
254  Chapter  8
 Figure 8.1:    The sin( x )/ x  outputs of the LPF are summed together to reconstruct the original
band-limited input waveform.
 In the time domain, you can see how the IR of each sample is overlaid on the others and that
the summing together of the peaks and valleys of the sin( x )/ x  shape ultimately creates the
portions in between the samples which appeared to have been lost during the sampling process.
 The process of overlaying the IR on top of the input stream of impulses  x ( n ) and adding up the
results to get the ﬁ nal time domain output  y ( n ) is called convolution. Convolution is a mathematical
operation used in many ﬁ elds of science; digital audio processing is just one of them. The
mathematical symbol for convolution is * which can be confusing because this is used to represent
the multiplication operation in C/C++. In the above example, you convolved the input signal  x ( n )
with the IR  h ( n ) by overlaying the  h ( n ) signal on top of each input impulse, then summing up
signals to create the ﬁ nal output  y ( n ). Mathematically, you would write this as  Equation 8.1 :
  y(n) 5 x(n) * h(n)
(8.1)
 Visually, it’s easy to understand the concept of overlaying the signals and adding them to
get the ﬁ nal result, but how do you describe that mathematically? The answer is that this
kind of operation is a special circumstance of a more generalized operation of convolution.
Mathematically, convolution for discrete signals is described in  Equation 8.2 :
1`
   c(n) 5 f(n) * g(n) 5 a
m52`
f(n)g(n 2 m)
 In this case,  f  and  g  are two generalized signals and neither of them has to be an IR.
Convolution is commutative, so that  f  *  g  5  g  *  f , or  Equation 8.3 :
1`
c(n) 5 f(n) * g(n) 5 a
m52`
1`
c(n) 5 g(n) * f(n) 5 a
m52`
f(n)g(n 2 m)
g(n)f(n 2 m)
(8.2)
(8.3)
 If you know how a system affects one single impulse, you can exactly predict how it will affect
a stream of impulses (i.e., a signal) by doing the time domain overlay. If you have the IR of a
system, you have the  algorithm  for the system coded in a single function.
