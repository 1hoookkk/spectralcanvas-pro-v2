# Audio Plugins C++ (Pirkle) — Chunk 47/73

> Source: `Designing Audio Effect Plug-Ins in C++_ With Digital Audio Signal Processing Theory - PDF Room.pdf` · Pages ~1-560 · Extractor: pdfminer.six
> Target ≈ 2800 words, Overlap = 200 words · Actual = 2770

> Note: DSP textbook content for SpectralCanvasPro RT-audio knowledge base

---
326  Chapter 9
1 .000
0.707
0.500
0.000
–0 . 5 00
–  0 .707
–    1 . 0 00
   {
   }
  }
0
17
34
51
68
85
102
119
136
153
 Figure 9.17:     Unipolar operation, sinusoid.
   *pYn /= 2.0;
   *pYn += 0.5;
   *pYqn /= 2.0;
   *pYqn += 0.5;
 Build and test the code;  Figure 9.17  shows the output for a unipolar sinusoid.
 Here are some projects to try:
•
•
•
   Higher-resolution tables; each octave of the piano ﬁ ngerboard gets its own table with as
many harmonics as the upper note of the octave permits.
   A two-oscillator synth module with a detuning control for one of the oscillators.
   An oscillator that only plays musical pitches.
 In the next chapter, we’ll use our new wave table oscillator to make modulated delay effects.
 Bibliography
  Dattorro, J. 2003. Effect design, Part 3, oscillators: Sinusoidal and pseudonoise.  Journal of the Audio Engineering
Society , 50(3): 114–146.
Gordon, J. W. and Smith, J. O. 1985. A sine generation algorithm for VLSI applications. Proceedings from the
International Computer Music Conference, pp. 165–168.
  Moore, R. 1990.  Elements of Computer Music , Chapter 3. Englewood Cliffs, NJ: Prentice-Hall.
  Roads, C. 1996.  The Computer Music Tutorial , Chapter 2. Cambridge, MA: The MIT Press.
   CHAP TE R 10
Modulated Delay Effects
    Perhaps the most interesting kinds of audio effects, from both listening and engineering
standpoints, are the modulated delay effects. They include the chorus and ﬂ anger/vibrato;
additionally, modulated delays are also found in some reverb algorithms. These time-varying
ﬁ lters are actually quite sophisticated and fun to implement, and we have gone to great
lengths to create useful sub-modules. These include the digital delay effect  or DDL (digital
delay line) module and the wave table oscillator. If you can design, build, and implement
modulated delay effects, then you are well on your way to proﬁ ciency in audio effects coding.
These effects have parameters and coefﬁ cients which change on every sample period.
  Figure 10.1a  shows the standard delay line or DDL. The output is fed back via the  fb
coefﬁ cient. The delay is constant until the user changes it to another ﬁ xed value. In the
modulated delay line (MDL) in  Figure 10.1b  the amount of delay is constantly changing over
time. The system only uses a portion of the total available delay amount.
x (n )
Σ
(a)
x(n)
Σ
(b)
Z – D
y (n)
fb
Modulation depth
Z – D
Read index
fb
y (n)
 Figure 10.1:    (a) A static delay and (b) modulated delay effect.
327
328  Chapter 10
 The  modulation depth  relates to the size of the portion of the delay line that is being read.
The  modulation rate  is the speed at which the read index moves back and forth over the
modulated delay section. A low-frequency oscillator  (LFO) is used to modulate the delay
amount and the LFO can be just about any kind of waveform, even noise. The most common
waveforms are triangle and sinusoid. In order to make a modulated delay effect, you ﬁ rst need
to make a delay line, and then modify it to constantly change and update the read location on
each sample interval.
  10.1    The Flanger/Vibrato Effect
 The ﬂ anger effect gets its name from the original analog method of running two tape
machines slightly out of sync with each other by placing a thumb on the ﬂ ange (the ring
that holds the tape) of one machine’s reel and applying pressure to slow down the tape
ever so slightly. Other variations include rubbing the ﬂ ange circularly. The effect creates
a time-varying delay that falls out of sync, then back in sync with the ﬁ rst tape machine.
The effect has been compared to the whoosh of a jet engine, or the sound of a phaser effect.
No matter how you deﬁ ne it, the ﬂ anger has a unique sound. The effect is heard on numerous
recordings.
 In order to simulate the ﬂ anger effect, you need a delay line whose read index moves away
from, then back to the top or starting point of the delay. When the read pointer is back to the
top, there is no delay and as it moves away from the write index, the pitch is modulated down.
When the index turns around and moves back, the pitch is modulated up—this is the Doppler
effect in practice. When the output of the modulated delay is sent out 100% wet, with no dry
signal, the effect is that of vibrato—the pitch shifts up and down as the pointer moves back
and forth. When the dry signal is mixed with the wet signal, the pitch shifting changes into
the ﬂ anging effect. The amount of delay time varies between 0 and about 7–10 mSec; at a
44,100 Hz sample rate, this corresponds to around 308 to 410 samples. Feedback is usually
added to increase the effect by creating resonances or anti-resonances, depending on whether
the feedback is normal or inverted.
  Figure 10.2  shows the two states of the modulator with increasing and decreasing pitch
shifting due to the Doppler effect.  Figure 10.3  shows alternate ways of diagramming the same
modulator. The vibrato and ﬂ anger are the same effect underneath the hood—it’s only the mix
ratio that determines which effect you have. Because the delay is constantly moving around
we are once again faced with options on how to handle the fractional delay that is required;
after you calculate the number of samples of delay (which will most likely have a fractional
component) you can
•
•
   Truncate the value and just use the integer part.
   Round the value up or down.
Modulated Delay Effects  329
x(n)
Σ
–410
Z
(a)
fb
Increasing delay,
decreasing pitch
x(n)
Σ
–410
Z
(b)
fb
Decreasing delay,
increasing pitch
y (n)
y( n)
 Figure 10.2:    (a) As the delay increases , the pitch drops. At the end of the delay, the
index turns around and begins heading back toward the write index (b), increasing
the pitch; the maximum delay here is about 10 mSec at 44,100 Hz.
x (n)
Σ
Z – D
(a)
fb
Σ
y( n)
Dry
Wet
Dry
x (n)
Σ
Z – D
Σ
y (n )
(b)
fb
LFO
Wet
 Figure 10.3:    (a) The simplest form of the f langer/vibrato effect. The dotted lines show the limits
of the delay modulation, from 0.0 (no delay) to the full range,  z2D  samples. (b) An alternate
version shows the LFO connection to the modulation index to the delay line.
330  Chapter 10
•
•
   Interpolate the delayed value using linear or polynomial interpolation.
   Use an all-pass ﬁ lter to make the fractional delay (see the Bibliography).
 The ﬂ anger/vibrato controls consist of:
•
•
•
•
•
   Modulation depth: how much of the available range is used.
   Modulation rate: the rate of the LFO driving the modulation.
   LFO type: usually sinusoidal or triangular, but may be anything.
   Feedback: as with the normal delay, the feedback has a big effect on the ﬁ nal
product.
   Wet/dry control: 100% wet 5 vibrato; 50/50 5 ﬂ anger.
 As shown in  Figure 10.4 , the ﬂ anger/vibrato technically always starts with 0.0 samples of
delay. Because of this, we’d like to have a LFO that can generate a unipolar output from
0.0 to 11.0, which we can then map to a range of delays, from 0 to about 410 samples or
so.  Figure 10.5  shows a stereo version of the ﬂ anger. A common LFO modulates both delay
lines, but the phases are off by 90 degrees, putting them in quadrature or quad-phase. The
effect is interesting as the two LFOs seem to chase each other, one on the left and the other
on the right.
 The flanger technically modulates the delay right down to 0 samples. For analog
tape flanging, this occurs when the two tape machines come back into perfect
synchronization. This is called  through-zero flanging  or TZF. This means that for an
instant, the output is double the input value. For low frequencies and/or high feedback
values, this can cause a problem when the flanger delay time is between 0 and about
0.1 mSec and the waveform is at a peak location. In this case, large valued samples
near each other on the waveform sum together, producing an increased and sometimes
distorted bass response.
Mod depth = Low
Mod depth = High
 Z –  D
Z – D
 Figure 10.4:    Two modulation depth settings; zero is always the minimum delay.
Left In
Σ
Z  – flanger
fb
fb
0 O
O
90
LFO
Right In
Σ
flanger
– Z
Modulated Delay Effects  331
Dry
Wet
Wet
Dry
Σ
Left Out
Σ
Right Out
 Figure 10.5:    A stereo quadrature ﬂ anger; the dotted lines at the LFO show its 90-degree
offset modulation. The max depth spans the entire delay line.
  Table 10.1:    Delay times for the ﬂ anger and chorus effects.
Min Delay (mSec)
Max Delay (mSec)
Flanger
Chorus
0.0
7
7–10
20–40
Feedback (%)
299 to 1 99
Typically 0
   10.2    The Chorus Effect
 The chorus effect appeared in the late 1970s and was used on countless recordings during the
1980s, becoming a signature sound effect of that decade. The chorusing effect can produce
a range of effects from a subtle moving sheen to a swirling sea-sick de-tuned madness. Like
the ﬂ anger, it is also based on a modulated delay line, driven by an LFO. Although different
manufacturers adopted different designs, the basic algorithm for a single chorusing unit is the
same. The differences between a ﬂ anger and chorus are shown in  Table 10.1 .
332  Chapter 10
•
•
   Analog chorus units typically do not use any feedback (because our DDL module already
has feedback, we can keep it for experimentation purposes).
   The range of delay times over which the device operates is the most signiﬁ cant
 difference.
 The read index is modulated about the center of the pointer but the center is set to avoid the
keep-out zone ( Figures 10.6  and  10.7 ). Also, the modulation depth and the location of the
center of the delay area vary across manufacturers ( Figure 10.8 ).
x (n)
(cid:153)
min
Z  D
fb
(cid:153)
y (n)
Dry
Wet
 Figure 10.6:    The basic chorus module. Note the feedback path is optional and not
found in typical analog chorus units. The gray area is the ﬂ anger keep-out zone.
x(n)
 ∑
min
Z  –D
Dry
∑
∑
Y(n)
fb
LFO
Wet
 Figure 10.7:    An alternative block diagram that describes the same chorus module.
Z – D
Z  D
–
(a)
(b)
 Figure 10.8:    (a) and (b) are two different chorus modules with the same
maximum depth (dotted lines) but a different center of operation.
Modulated Delay Effects  333
 Many variations on the basic chorus module exist, including:
•
•
•
•
•
   Multiple chorus modules with different centers of operation
   Different LFO frequency or LFO phases applied to different modules
   Series modules
   Parallel modules
   Bass modules with a low-pass ﬁ lter/high-pass ﬁ lter on the front end
  Figure 10.9  shows the stereo quad chorus with the left and right LFOs out of sync by
90 degrees. In this permutation, the maximum depths of the left and right channels are
independent, as well as the ability to adjust them. The ﬂ owchart for the basic modulated delay
effect for each processAudioFrame() is shown in  Figure 10.10 .
Left In
∑
– Z
chorus
fb
fb
0°
90°
LFO
Right In
∑
Z – chorus
Dry
Wet
Wet
Dry
∑
Left Out
∑
Right Out
 Figure 10.9:    A stereo quadrature chorus; feedback paths are optional.
Generate  LFO
value
Calculate new
delay  offset
based on LFO
DLLModule:
-Set  delay time
-Cook  variables
DDLModule:
processAudioFrame()
 Figure 10.10:    The ﬂ owchart for modulated delay effects.
334  Chapter 10
 You will use the DDL module to do most of the low-level work and generate the delayed
values. The LFO will generate values that will be used to calculate the current delay offset.
Since the LFO is varying, the delay offset will need to be recalculated on each sample
interval.
   10.3    Design a Flanger/Vibrato/Chorus Plug-In
 Now we get down to the business of coding these effects. Because of the large number of
variations, we should start by coding a module that can be used in larger parent plug-in
objects.  Figure 10.11  shows the block diagram.
 We will provide several user interface (UI) components to control the plug-in and their
variables are available for a parent plug-in to use as well. We will use the existing DDL
module and wave table oscillator plug-ins. Since the DDL module has feedback path control,
there will be many ways to combine these modules into sophisticated units. The controls we
will provide for the user are discussed next. The plan of operation is as follows:
•
•
•
•
   LFO: needs to be set up to run unipolar and with the non-band-limited tables.
   The chorus delay offset control will effectively move the center of operation for the
modulated delay.
   The vibrato mode will force the feedback value to 0.0% since it does not sound good with
feedback.
   prepareForPlay()
•
   Reset the LFO and start it by setting the m_bNoteOn ﬂ ag; also set up the LFO and
DDL modules.
   Set the DDL m_nSampleRate variable; it needs this value to initialize its buffer
 properly.
   Forward the prepareForPlay() call to the child objects, LFO and DDL.
•
•
x(n)
∑
Z  –D
∑
y(n)
Dry
fb
LFO
Wet
 Figure 10.11:    Generic modulated delay block diagram;
it can be any type of ﬂ anger/vibrato/chorus.

Modulated Delay Effects  335
•
•
   processAudioFrame()
•
•
•
•
   userInterfaceChange()
•
•
   Call the doOscillate() function on the LFO.
   Use the LFO value to calculate a delay offset in milliseconds for the DDL module.
   Write the new delay time to the DDL module and call its cooking function.
   Call processAudioFrame() on the DDL module to get the current output sample.
   Call the update functions for the LFO and DDL.
   Call the type switching function to change from ﬂ anger to vibrato and chorus.
 We’ll go ahead and plan on some helper functions to make this process easier. This is
not going to be the most streamlined or efﬁ cient code, but once you have it mastered you
can always go back and improve it. Although this is a difﬁ cult project, you only need two
member variables and four functions to accomplish the design.
 The member objects are as follows:
   CWTOscillator m_LFO;    // our LFO
   CDDLModule m_DDL;        // our delay line module
 The member variables are as follows:
   ﬂ oat m_fMinDelay_mSec;   // min delay time
   ﬂ oat m_fMaxDelay_mSec;   // max delay time
 The member functions are as follows:
   ﬂ oat updateLFO();   // change/set the type and rate
   ﬂ oat updateDDL();   // change/set the wet/dry and feedack
   ﬂ oat cookModType();   // set up the min/max delay times
   ﬂ oat calculateDelayOffset(ﬂ oat fLFOSample);   // convert the LFO to a delay value
  10.3.1  Project: ModDelayModule
 Start a new project and make sure to include your WTOscillator and DDLModule  objects
( Figure 10.12 ).
 Figure 10.12:    Project settings for the ModDelayModule.
336  Chapter 10
    Table 10.2  :    The GUI components, variable names, and values.
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
Button Property
Control Name
Units
Variable Type
Variable Name
Enum String
Value
Depth
%
ﬂ oat
m_fModDepth_pct
0
100
50
Value
Feedback
%
ﬂ oat
m_fFeedback_pct
-100
100
0
Value
Mod Type
enum
m_uModType
Flanger, Vibrato, Chorus
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
Button Property
Control Name
Units
Variable Type
Variable Name
Enum String
Value
Rate
Hz
ﬂ oat
m_fModFrequency_Hz
0.02
5
0.18
Value
Chorus Offset
mSec
ﬂ oat
m_fChorusOffset
0
30
0
Value
LFO
enum
m_uLFOType
tri,sin
 10.3.2  ModDelayModule GUI
 Set up the graphical user interface (GUI) in RackAFX with  Table 10.2 .
   10.3.3  ModDelayModule.h File
 Add the member objects, variables, and function declarations to the .h ﬁ le:
  // Add your code here: ------------------------------------------------------- //
   CWTOscillator m_LFO;   // our LFO
   CDDLModule m_DDL;         // our delay line module
  // these will depend on the type of mod
   ﬂ oat m_fMinDelay_mSec;
   ﬂ oat m_fMaxDelay_mSec;
