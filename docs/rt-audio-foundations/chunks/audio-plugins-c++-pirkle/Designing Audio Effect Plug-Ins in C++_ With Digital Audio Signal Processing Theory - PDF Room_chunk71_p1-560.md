# Audio Plugins C++ (Pirkle) — Chunk 71/73

> Source: `Designing Audio Effect Plug-Ins in C++_ With Digital Audio Signal Processing Theory - PDF Room.pdf` · Pages ~1-560 · Extractor: pdfminer.six
> Target ≈ 2800 words, Overlap = 200 words · Actual = 1750

> Note: DSP textbook content for SpectralCanvasPro RT-audio knowledge base

---
The VST® and AU® Plug-In APIs  517
  {
  // setup counter
   UInt32 nSampleFrames = inFramesToProcess;
  // setup pointers to I/O
   const Float32 *sourceP = inSourceP;
   Float32 *destP = inDestP;
  // use our own Get method
   Float32 gain = GetParameter( kParam_One );
  while (nSampleFrames–– > 0) {
  Float32 inputSample = *sourceP;
  sourceP += inNumChannels;
  // here's where you do your DSP work
   Float32 outputSample = inputSample * gain;
  *destP = outputSample;
  destP += inNumChannels;
  }
  }
 If you need to process multi-channel audio data independently, then you need to remove the
Process() method override and replace it by overriding ProcessBufferLists().
  ProcessBufferLists(AudioUnitRenderActionFlags &ioActionFlags,
  const AudioBufferList &inBuffer,
  AudioBufferList &outBuffer,
  UInt32 inFramesToProcess)
 This method passes data back and forth by using AudioBufferList objects. These objects
contain pointers so buffers of interleaved data along with a variable that tells you how many
channels the buffer holds. For example, for the input buffer:
  inBuffer.mNumberBuffers = the number of channels in this Buffer List
  inBuffer.mBuffers[0].mData = a ﬂ oat* to the buffer for channel 0
  inBuffer.mBuffers[1].mData = a ﬂ oat* to the buffer for channel 1, etc…
 You would typically write something like this to extract them (in this case, we know the
channel count is 2):
  // create the mini-frame arrays
  ﬂ oat **inputs = new ﬂ oat*[2 * sizeof(ﬂ oat*)];
  inputs[0] = (ﬂ oat*)inBuffer.mBuffers[0].mData;
  inputs[1] = (ﬂ oat*)inBuffer.mBuffers[1].mData;
  ﬂ oat **outputs = new ﬂ oat*[2 * sizeof(ﬂ oat*)];
  outputs[0] = (ﬂ oat*)outBuffer.mBuffers[0].mData;
  outputs[1] = (ﬂ oat*)outBuffer.mBuffers[1].mData;

518  Appendix A
  // call your processing function
  doProcess(inputs, outputs, inFramesToProcess);
 In this case, your processing function is just like the VST processReplacing()—it is taking
pointers to buffer pointers as arguments, then it would split out the data as in the VST version
above. Lastly, the AUKernelBase method Reset() is implemented to reset your plug-in to its
just-instantiated state.

APP E NDIX  B

More RackAFX Controls
and GUI Designer
 RackAFX has several optional graphical user interface (GUI) controls that were not covered
in the book chapters. It also has a GUI designer that lets you convert your slider-based default
GUI into a fully customized version. The website  www.willpirkle.com  is always the best
way to get the latest information on the GUI designer and any additional controls that may be
added at a later date. The two additional GUI controls are the alpha wheel and liquid crystal
display  (LCD) and the vector joystick control.
  B.1    The Alpha Wheel and LCD Control
 In the upper right of the main RackAFX UI is a control that represents an alpha wheel, LCD
matrix view, and the control knob. During the course of designing your project, you’ve noticed
that the LCD is used to give you feedback about the state of your project and compiler.
However, it can also be used as a GUI control just like the other sliders. The LCD control allows
you to store up to 1024 continuous slider controls inside it. Anything you can set up with a
normal slider control will work as one of the LCD embedded controls. And, the GUI designer
lets you place one of these controls on your own GUI. There are several uses for the control:
(cid:129)
(cid:129)
(cid:129)
   It’s a different way to store the continuous controls.
   If you run out of sliders, you can always put 1024 more of them in the LCD.
   You can use the enumerated list variable to create a set of presets or other global func-
tions or conﬁ guration controls.
 To set up the LCD control, right-click on it when you are in development mode (not with
a plug-in loaded). A box pops up as shown in  Figure B.1 . Click on New to open the very
familiar slider properties dialog. Fill it out just as you would any other slider control. After
adding a few more controls, you might have something resembling  Figure B.2 , with three
controls embedded. You can edit the controls, remove them, or move them up and down the
list (which changes their sequence in the LCD control).
 You treat these embedded slider controls just as any other slider, so there’s nothing to add to
your normal operational code. When you run your plug-in, use the alpha wheel to select one
519
520  Appendix B
 Figure B.1:    The New button pops up the slider properties to add a control to the LCD.
 Figure B.2:    The LCD is loaded with three controls.
of the controls and the value knob to adjust its value. Note there is currently no edit control
connected to the LCD for manual entry of control values.  Figure B.3  shows the LCD control
with the alpha wheel adjusted for the bass control.
 An alternative way to use the LCD control is for global operations (like effects
conﬁ gurations) or to store presets, either because you need more than RackAFX provides or
More RackAFX Controls and GUI Designer  521
Alpha
Wheel
Indicator:
Control 2 of 3
is selected
 Figure B.3:    The alpha wheel and LCD control is reminiscent of vintage synthesizer and rack
effects.
Value Knob
 Figure B.4:    The underscores are automatically removed from enumerated list variables in the
LCD control.
you need to write your own methods for loading and storing data. You can use the enumerated
list variable to create up to 256 individual string names. You can also take advantage of a
feature of the LCD control: You can use the underscore (_) to separate strings in your list (you
can’t have spaces in an enumerated name). When the LCD control uses them, it replaces the
underscores with single white spaces. This allows you much more ﬂ exibility than the radio
buttons or normal sliders provide. For example, you could set up a preset list with names
like “Heavy_Lead,” “Liquid_Pad,” “Lush_Space,” and so on. Then, you can use the up/
down arrows to place your “presets” control at the top of the control list. Now, as shown in
 Figure B.4 , the user can select one of your custom presets (remember, it’s up to you to code
your own preset format) and then scroll to adjust the other controls. The underscores have
been replaced with spaces.
   B.2    The Vector Joystick Control
 RackAFX also has a dedicated vector joystick control that communicates with your plug-in.
Dave Smith designed the ﬁ rst vector synthesizers for Sequential Circuits, Inc. and then later
for Yamaha and Korg. The vector joystick is a combination of a traditional  x / y  track-pad
and an exponential joystick. The calculation documentation is available on the website.
In the original synthesizers, the vector joystick was used to program or alter in real time the
mixture of two or four different waveforms. The position of the joystick determines the mix
522  Appendix B
Joystick
B
A
Vector Joystick
A
B
C
D
SINE
TRI
PITCH
SQUARE
C
Apex Drop
List Controls
D
A:0.25 C:0.25 B:0.25 D:0.25   Mix Blurb
JS Program
Joystick Program
 Figure B.5:    The RackAFX vector joystick.
ratios, which are not linear but rather exponential. In RackAFX, you can use the joystick to
alter up to four different parameters at once, exponentially. In addition, a linear  x / y  value of
the joystick location is available for your plug-in.  Figure B.5  shows the vector joystick and
components.
 The joystick is arranged in a diagonal with four corners labeled A, B, C, D, clockwise;
this is in keeping with Dave Smith’s original lingo. As the joystick moves around, a new
set of mix ratios is generated and appears in the mix blurb. The mix ratios will always
add up to 1.0. In the center, the ratios are all 0.25. A second set of values also exists, the
AC-mix and the BD-mix, which are linear. On the AC axis, 0 is at the A apex and 1.0 is at
the C corner. On the BD axis, 0 is at D and 1.0 is at B. In the center position, the AC-mix
is 0.5 and the BD-mix is also 0.5.  Figure B.6  shows the joystick roughly three-quarters
of the way from the origin to the A corner. However, the mix blurb shows the value of
A to be 0.79, but the other three have dropped off substantially. With the joystick all the
way in the A corner, the ratios are {A, B, C, D} = {1, 0, 0, 0} and interestingly, as  Figure
B.7  demonstrates, when the joystick is halfway between A and B on the outer edge of the
diamond, the A/B ratio is 50/50 while the rest are 0. In  Figure B.7 , the AC-mix is 0.25
and the BD-mix is 0.75.
 By sweeping around the ﬁ eld, you can generate many different combinations. You could
use these combinations as mix ratios for something in your plug-in. Or, you could use them
to control any parameter or other aspect of the plug-in that you like. You are responsible
for mapping the 0.0 to 1.0 values to your plug-in’s parameters. The vector joystick does
not connect to any variable in your plug-in; like the assignable buttons, these passive
controls simply tell you when they’ve moved. When the joystick moves, a function called
joystickControlChange() is called.
More RackAFX Controls and GUI Designer  523
B
Vector Joystick
A
C
A
B
C
D
SINE
TRI
PITCH
SQUARE
D
A:0.79 C:0.01 B:0.09 D:0.11
JS Program
 Figure B.6:    The exponential mix ratios are evident in the vector joystick.
B
Vector Joystick
A
C
A
B
C
D
SINE
TRI
PITCH
SQUARE
D
A:0.50 C:0.00 B:0.50 D:0.00
JS Program
 Figure B.7:    This position results in the 50/50 mix of A/B only. AC-mix = 0.25, BD-mix = 0.75.
  /* joystickControlChange
  Indicates the user moved the joystick point; the variables are the relative
mixes     of each axis; the values will add up to 1.0
  B
  |
A
x
C
  |
  D
  The point in the very center (x) would be:
  fControlA = 0.25
  fControlB = 0.25
  fControlC = 0.25
  fControlD = 0.25
  AC Mix = projection on X Axis (0 -> 1)
  BD Mix = projection on Y Axis (0 -> 1)
  */
