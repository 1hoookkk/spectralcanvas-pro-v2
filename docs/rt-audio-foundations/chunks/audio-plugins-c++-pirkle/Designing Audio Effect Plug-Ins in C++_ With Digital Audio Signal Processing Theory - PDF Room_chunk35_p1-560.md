# Audio Plugins C++ (Pirkle) — Chunk 35/73

> Source: `Designing Audio Effect Plug-Ins in C++_ With Digital Audio Signal Processing Theory - PDF Room.pdf` · Pages ~1-560 · Extractor: pdfminer.six
> Target ≈ 2800 words, Overlap = 200 words · Actual = 2718

> Note: DSP textbook content for SpectralCanvasPro RT-audio knowledge base

---
Delay Effects and Circular Buffers   235
    // create the wet/dry mix and write to the output buffer
    // dry = 1 - wet
    pOutputBuffer[0] = m_fWetLevel*yn + (1.0 - m_fWetLevel)*xn;
    etc…
 You can see that the change required is very minor—just a switch to change the feedback
sample value. Of course if the user enables this option, then they are responsible for placing
meaningful data in the fFeedbackIn variable.
 Rebuild and test the code to make sure it still works properly. In the next section, we will use
the module to make two different plug-ins:
1.
2.
   Stereo digital delay
   Stereo crossed-feedback delay
     7.5    Modifying the Module to Implement Fractional Delay
 Before we work on the bigger projects, we need to take care of the problem of
fractional delay. We would like the user to be able to get any amount of delay they want.
By implementing only sample-based delay, we are limiting the user to choosing delays
that are multiples of the sample period, about 23 uSec. You might think that is enough
accuracy; however, there are several instances where this won’t be enough. The ﬁ rst
is the case of a delay that is beats-per-minute (BPM) synchronized (e.g., delay is set
to a multiple of the song’s BPM to create synchronized echoes or percussion effects).
The second case is that of modulated delays like chorus and ﬂ anging, which require
smooth modulations from one delay time to another. Linear interpolation will provide
acceptable results.
 Suppose our delay is in a state where we have calculated our delay position to be at sample
location 23.7183 samples. We need to ﬁ nd the value of the data at the location 0.7183
between sample 23 and sample 24. In  Figure 7.26  you can see a graphic representation of
the interpolation method. Since it’s linear interpolation, a line is drawn between the adjacent
samples and the interpolated value is found on the line at 0.7183 the distance between the
two samples.
 In polynomial interpolation such as LaGrange interpolation, a curve is drawn between the
points (or a series of points), and then the interpolated value is found on that curve. There are
several ways to implement the linear interpolation but the easiest method is to treat it like a
DSP ﬁ lter. Another way of thinking about interpolation is as a weighted sum. For example,
if the interpolation point is 0.5 between samples 1 and 2, then the interpolated value is made
up of 50% of sample 1 plus 50% of sample 2. In the above case, our interpolated distance is
0.7183, so we can view the output as

236  Chapter 7
y = mx + b
23
24
23.7183
 Figure 7.26:     Linear interpolation of sample values.
Sample 24
x(n)
frac = 0.7183
Sample 23
x(n-1)
frac
1-frac
y(n)
 Figure 7.27:     Linear interpolation as a kind of feed-forward ﬁ lter. The  z –1 element
is removed since we do not know if the interpolated samples will always be
exactly one sample apart. For example, what if the next fractional delay
sample is at location 56.2394?
  interp_output 5 (0.7183)(Sample 2) 1 (0.2817)(Sample 1)
 Here is a linear interpolation function you can use; it is already declared in your
pluginconstants.h ﬁ le:
   ﬂ oat dLinTerp (ﬂ oat x1, ﬂ oat x2, ﬂ oat y1, ﬂ oat y2, ﬂ oat x);
 You give it a pair of data points ( x 1, y 1) and ( x 2, y 2), plus a distance between them on the
 x -axis ( x ), and it returns the interpolated value using the weighted sum method. The ﬁ rst part
of the code checks for a potential divide by zero fault that technically should not happen. You
should also be aware that linear interpolation is a form of feed-forward ﬁ ltering. The block
diagram of the above function would look like  Figure 7.27 .
 Thus, in the processAudioFrame(), we need to ﬁ nd the two sample values that our actual
delay time needs, then do the interpolation. It is pretty straightforward except the situation
Delay Effects and Circular Buffers   237
where we are interpolating across the wrap boundary (from the last sample in the buffer
to the ﬁ rst one). Suppose the user enters a delay time that corresponds to 2.4 samples of
delay. In the cookVariables() function, we locate the read index to be two samples before
the write pointer because we cast the value to an integer, stripping out the fractional part.
The actual delay we really want is 0.4 samples between the current sample and the one
before it. In other words,  x ( n 22.4) rather than  x ( n 22). You can see that we need a sample
that is between  x ( n 22) and  x ( n 23); in  Figure 7.28  it happens to be a distance of 0.4
between them.
 We need to modify our code as follows:
•
•
•
•
   We are already calculating and updating the ﬁ rst sample that we need to interpolate; it is
located with the index value at m_nReadIndex, so there is nothing to change in the cook-
ing function.
   In the processAudioFrame() function, we need to interpolate between our current read
location m_nReadIndex and the location just  before  it in the buffer, m_nReadIndex-1.
   Since we are only focused on a single pair of samples at any time and we know they are
one sample apart, we can use the values 0 and 1 for the interpolation function as  x 1 5 0,
 x 2 5 1; then we interpolate the fractional distance between them. You can use m_nRead-
Index but you will get in trouble when you are interpolating across the wrap boundary of
the buffer.
   We will need to check for a wrap condition backward if m_nReadIndex–1 takes us out-
side the top of the buffer.
 We can get the fractional value from our m_fDelayInSamples in several ways; here is one of
them:
  float fFracDelay 5 m_fDelayInSamples – (int)m_fDelayInSamples
Buffer
x(n –3)
x(n –2)
x(n–1)
x(n)
x(n – 2.4)
Current
input
sample
 Figure 7.28:     Fractional interpolation.
238  Chapter 7
 It really only comes down to locating the sample 1 behind our current read index, then using
the linear interpolation code to get the fractional value. There are two extreme conditions to
consider:
•
•
   At the maximum delay time, the read and write indices will be equal and the fractional
part of the delay in samples will be 0 so no interpolation will occur—this is OK.
   If the delay in samples is less than 1, the read and write indices will also be equal, but this
will be a problem. In this case, we need to interpolate between the current  x ( n ) and the
sample at  x ( n 21), one sample behind the read location. Branching will be necessary to
catch this event and handle it.
  7.5.1  DDLModule.cpp File
  processAudioFrame()
•
   Modify the code to do the interpolation.
  bool __stdcall CDDLModule::processAudioFrame(ﬂ oat* pInputBuffer, ﬂ oat* pOutputBuffer,
  UINT uNumInputChannels, UINT uNumOutputChannels)
  {
         // Do LEFT (MONO) Channel
         // Read the Input
         ﬂ oat xn = pInputBuffer[0];
         // Read the output of the delay at m_nReadIndex
         ﬂ oat yn = m_pBuffer[m_nReadIndex];
          // if delay < 1 sample, interpolate between input x(n) and x(n-1)
          if(m_nReadIndex == m_nWriteIndex && m_fDelayInSamples < 1.00)
          {
                  // interpolate x(n) with x(n-1), set yn = xn
                  yn = xn;
          }
          // Read the location ONE BEHIND yn at y(n-1)
          int nReadIndex_1 = m_nReadIndex - 1;
          if(nReadIndex_1 < 0)
                  nReadIndex_1 = m_nBufferSize-1; // m_nBufferSize-1 is last location
          // get y(n-1)
          ﬂ oat yn_1 = m_pBuffer[nReadIndex_1];
          // interpolate: (0, yn) and (1, yn_1) by the amount fracDelay
          ﬂ oat fFracDelay = m_fDelayInSamples - (int)m_fDelayInSamples;
          // linerp: x1, x2, y1, y2, x
          ﬂ oat fInterp = dLinTerp(0, 1, yn, yn_1, fFracDelay); // interp frac between them
          // if zero delay, just pass the input to output
          if(m_fDelayInSamples == 0)
                  yn = xn;

Delay Effects and Circular Buffers   239
          else
                  yn = fInterp;
         // write the intput to the delay
         if(!m_bUseExternalFeedback)
                 m_pBuffer[m_nWriteIndex] = xn + m_fFeedback*yn; // normfInterpal
         else
                  m_pB uffer[m_nWriteIndex] = xn + m_fFeedbackIn; // external feedback
  }
sample
 Now, build and test the module. Try a variety of delay settings. If you hear a repetitive
click in your output that is exactly the same rate as the delay time, then there is something
wrong during the interpolation across the wrap boundary of buffer[0] to buffer[size–1]. Only
advance to the next section when you have this project fully debugged and functional. We will
be including it in the next project, a stereo digital delay.
     7.6    Design a Stereo Digital Delay Plug-In
 In this project, we use two DDL modules in one parent plug-in. RackAFX makes it easy
to do this by allowing you to add other plug-in components (.h and .cpp ﬁ les) into a new
project. It will automatically #include  the components too. However, if you use external
modules or other ﬁ les you might need to manually #include these. In  Figure 7.29  you can
see that we now have two DDL modules declared as member objects of the new plug-in.
The plug-in implements its own interface of three sliders, which we use to control our
modules.
  7.6.1  Project: StereoDelay
 Create a project named “StereoDelay.” When you create the project, you have the option of
including other modules in your code, seen in  Figure 7.30 . RackAFX ﬁ nds all of the existing
RackAFX projects in the default directory you supply and lists them here. You use the Add
button to move them into your project. If you have a project located in another directory that
is not the default, you will need to move the ﬁ les on your own (copy them to the new project
directory and #include  them in the new <project>.h ﬁ le and add them into the compiler).
RackAFX will automatically copy them and #include whichever modules you choose. In
this case, choose the DDL module.
 When you use a plug-in as a module for another parent plug-in you must create and implement
a new UI. The child plug-in objects will  not  expose their sliders to RackAFX, but you can
manipulate the UI variables. All other aspects of the child objects work as expected. In this
plug-in, we will implement another UI to control the modules. See Appendix A.2 for advanced
control of the UI variables.

240  Chapter 7
m_DDL_Left
x(n)  Left
Z-D
fb_in(n)
fb_out(n)
m_DDL_Right
x(n)  Right
Z-D
fb_in(n)
fb_out(n)
fb
fb
Dry
Wet
Dry
Wet
y(n)  Left
y(n)  Right
Feedback
Wet/Dry
210.94 mSec
16.41%
50.00  %
Figure 7.29:  Block diagram of our stereo delay .
Existing Plug-Ins:
SimpleHPF.h
Volume.h
VolumedB.h
Your Project:
DDLModule.h
Add
You can use Stock Objects and/or existing Plug-Ins in your new Plug-In.
 Figure 7.30:     Adding existing modules can be done programmatically through RackAFX.
 You can see in  Figure 7.30  that I added “DDLModule.h” to the project. After completing the
new project dialog, check your new <plugin>.h ﬁ le:
  #pragma once
  #include "pluginconstants.h"

Delay Effects and Circular Buffers   241
   #include "DDLModule.h"
  #include "plugin.h"
  // abstract base class for DSP ﬁ lters
  class CStereoDelay : public CPlugIn
  {
  public:  // Plug-In API Functions
    //
    // 1. One Time Initialization
    CStereoDelay();
  etc …
   7.6.2  StereoDelay GUI
 Your GUI will look like that in  Figure 7.29  and you use the same setup from the DDLModule
in  Table 7.1 .
   7.6.3  StereoDelay.h File
 In the .h ﬁ le, declare two member objects of type CDDLModule. Also, add a function
called setDelayVariables() to transfer our global delay variables to the member objects and
optionally have the member objects cook the data:
  // Add your code here: --------------------------------------------------------- //
   CDDLModule m_DDL_Left;
   CDDLModule m_DDL_Right;
   // function to pass our variables to member delays
   void setDelayVariables(bool bCook);
  // END OF USER CODE ------------------------------------------------------------ //
 Our DDLModules are just C++ objects, so you can treat them as such and call their member
functions and set their member variables. Before implementing the function above, go ahead
and add your UI sliders, exactly the same as before with the same variable names. You will
have something like this:
  // ADDED BY RACKAFX -- DO NOT EDIT THIS CODE!!! -------------------------------- //
  // **--0x07FD--**
   ﬂ oat m_fDelay_ms;
   ﬂ oat m_fFeedback_pct;
   ﬂ oat m_fWetLevel_pct;
  // **--0x1A7F--**
  // ----------------------------------------------------------------------------- //

242  Chapter 7
   7.6.4  StereoDelay.cpp File
 Write the setDelayVariables() function. The Boolean ﬂ ag allows you to optionally call the
cooking functions on the members:
  void CStereoDelay::setDelayVariables(bool bCook)
  {
  // forward our variables over to the member objects
    m_DDL_Left.m_fDelay_ms = m_fDelay_ms;
    m_DDL_Right.m_fDelay_ms = m_fDelay_ms;
    m_DDL_Left.m_fFeedback_pct = m_fFeedback_pct;
    m_DDL_Right.m_fFeedback_pct = m_fFeedback_pct;
    m_DDL_Left.m_fWetLevel_pct = m_fWetLevel_pct;
    m_DDL_Right.m_fWetLevel_pct = m_fWetLevel_pct;
    // cook, if desired
    if(bCook)
    {
      m_DDL_Left.cookVariables();
      m_DDL_Right.cookVariables();
    }
  }
  Constructor
•
   Initialize the delay variables and cook them; this version does not use the external feed-
back option, so set the bool accordingly.
  CStereoDelay::CStereoDelay()
  {
    <SNIP SNIP SNIP>
    // Finish initializations here
     m_DDL_Left.m_bUseExternalFeedback = false;
     m_DDL_Right.m_bUseExternalFeedback = false;
     // set and (true) cook the delays
     setDelayVariables(true);
  }
   prepareForPlay()
•
•
   Set the delay variables.
   Forward the calls to prepareForPlay() on the member objects, which will cook them.
The DDL module will handle ﬂ ushing the buffers and so on.

Delay Effects and Circular Buffers   243
  bool __stdcall CStereoDelay::prepareForPlay()
  {
     setDelayVariables(false);
     m_DDL_Left.prepareForPlay();
     m_DDL_Right.prepareForPlay();
  return true;
  }
   userInterfaceChange()
•
   Set the delay variables and cook them whenever one of our controls changes. This could
be streamlined for better efﬁ ciency.
  bool __stdcall CStereoDelay::userInterfaceChange(int nControlIndex)
  {
     // set and cook the variables
     setDelayVariables(true);
    return true;
  }
   processAudioFrame()
•
•
   Forward the processAudioFrame() call to the member objects.
   Remember that we need to send it only one channel at a time and make sure it is the cor-
rect one. Note the “address of” operator (&) to point to the single memory location we
pass it.
  bool __stdcall CStereoDelay::processAudioFrame(ﬂ oat* pInputBuffer, ﬂ oat*
  pOutputBuffer, UINT uNumInputChannels, UINT uNumOutputChannels)
  {
    // Do LEFT (MONO) Channel; there is always at least one input/one output
    // forward call to sub-object: pInput, pOutput, 1 input ch, 1 output ch
     m_DDL_Left.processAudioFrame(&pInputBuffer[0], &pOutputBuffer[0], 1, 1);
    // Mono-In, Stereo-Out (AUX Effect)
     if(uNumInputChannels == 1 && uNumOutputChannels == 2)
       pOutputBuffer[1] = pOutputBuffer[0]; // just copy
    // Stereo-In, Stereo-Out (INSERT Effect)
    if(uNumInputChannels == 2 && uNumOutputChannels == 2)
      // forward call to sub-object pInput, pOutput, 1 input ch, 1 output ch
        m_DDL_Right.processAudioFrame(&pInputBuffer[1], &pOutputBuffer[1],  1,  1);
    return true;
  }

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
