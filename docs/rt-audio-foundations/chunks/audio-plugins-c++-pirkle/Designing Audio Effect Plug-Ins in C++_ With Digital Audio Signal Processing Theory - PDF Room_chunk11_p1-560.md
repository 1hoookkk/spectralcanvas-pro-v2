# Audio Plugins C++ (Pirkle) — Chunk 11/73

> Source: `Designing Audio Effect Plug-Ins in C++_ With Digital Audio Signal Processing Theory - PDF Room.pdf` · Pages ~1-560 · Extractor: pdfminer.six
> Target ≈ 2800 words, Overlap = 200 words · Actual = 2650

> Note: DSP textbook content for SpectralCanvasPro RT-audio knowledge base

---
50  Chapter 3
 In this case, check to verify that RackAFX added the ﬂ oat member variable m_fVolume as you
anticipated. Next, move on to the volume.cpp ﬁ le and have a look at it, starting from the top.
   3.9.5  Volume.cpp File
  Constructor and destructor
 The constructor is the One-Time-Init function and is set up to:
•
•
•
•
   call initUI(): This is where your GUI controls are initialized; m_fVolume is initialized
to 0.75 inside this function. It is important to make sure this remains the ﬁ rst line of the
constructor so that your GUI variables are always initialized ﬁ rst.
   Set the plug-in name variable: This is what you will see in the user plug-in menu and on
the GUI windows.
   Set the plug-in defaults (snipped out here); you will rarely need to change these variables.
   Give you a place to ﬁ nish any of your own initializations at the end.
 The destructor is empty because nothing has been allocated dynamically in this plug-in.
   CVolume::CVolume()
  {
          // Added by RackAFX - DO NOT REMOVE
          //
          // Setup the RackAFX UI Control List and Initialize Variables
           initUI();
          // END InitUI
          // built in initialization
           m_PlugInName = "Volume";
          // Default to Stereo Operation:
          // Change this if you want to support more/less channels
           <SNIP SNIP SNIP>
           // Finish initializations here
  }
  /* destructor()
          Destroy variables allocated in the contructor()
  */
   CVolume::~CVolume(void)
  {
  }
   prepareForPlay()
 There is nothing to write yet since there are no custom controls or other allocations.

Writing Plug-Ins with RackAFX   51
   processAudioFrame()
 This function is where the signal processing action happens. Above the deﬁ nition is a
comment block as a reminder of how to get the audio data samples into and out of the I/O
buffers. Currently, RackAFX only supports mono and stereo plug-ins. The left and right
channels are accessed using the normal array-indexed C11 pointer mechanism. Of special
note is the reminder that all values in and out are (and should be) between −1.0 and 11.0.
  /* processAudioFrame
  // ALL VALUES IN AND OUT ON THE RANGE OF -1.0 TO + 1.0
  LEFT INPUT = pInputBuffer[0];
  RIGHT INPUT = pInputBuffer[1]
  LEFT OUTPUT = pInputBuffer[0]
  LEFT OUTPUT = pOutputBuffer[1]
  */
  bool __stdcall CVolume::processAudioFrame(ﬂ oat* pInputBuffer, ﬂ oat* pOutputBuffer,
  UINT uNumInputChannels, UINT uNumOutputChannels)
  {
          // output = input -- change this for meaningful processing
          //
          // Do LEFT (MONO) Channel; there is always at least one input/one output
          (INSERT Effect)
          pOutputBuffer[0] = pInputBuffer[0];
          // Mono-In, Stereo-Out (AUX Effect)
          if(uNumInputChannels == 1 && uNumOutputChannels == 2)
                 pOutputBuffer[1] = pInputBuffer[0];
          // Stereo-In, Stereo-Out (INSERT Effect)
          if(uNumInputChannels == 2 && uNumOutputChannels == 2)
                 pOutputBuffer[1] = pInputBuffer[1];
          return true;
  }
 Take a look at the function as RackAFX wrote it for you—it is designed to pass audio through
unmodiﬁ ed. In this case, you simply write the output buffer with the data from the input
buffer. In your processAudioFrame() function, get used to always processing the ﬁ rst channel
then optionally processing the second one. This makes it easy to write mono/stereo capable
plug-ins and will also make it easier to extend when RackAFX has more channel options.
Because the code is already done, you could compile it right now and try it in RackAFX as a
sanity check to make sure your audio hardware is set up properly. In fact, we’ll do that right
after examining the last few functions in the ﬁ le (I promise you will write code shortly).
52  Chapter 3
   userInterfaceChange()
 Perhaps the second most important function is userInterfaceChange(), which is called when
the user changes a control on the control surface:
  /* ADDED BY RACKAFX -- DO NOT EDIT THIS CODE!!! ----------------------------- //
          **--0x2983--**
          Variable Name                Index
  -----------------------------------------------
           m_fVolume                     0
  -----------------------------------------------
          **--0xFFDD--**
  // ----------------------------------------------------------------------------- */
  // Add your UI Handler code here ----------------------------------------------- //
  //
 As with processAudioFrame(), there is a “hint” comment above the function deﬁ nition which
reminds you how RackAFX has mapped your variable to a control index. In this case, the
m_fVolume variable is mapped to index 0.
  bool __stdcall CVolume::userInterfaceChange(int nControlIndex)
  {
          // decode the control index, or delete the switch and use brute force calls
          switch(nControlIndex)
          {
                  case 0:
                  {
                          break;
                  }
                  default:
                          break;
          }
          return true;
  }
 userInterfaceChange() implements the ﬁ rst part of a switch/case statement in case you need
to decode the control index and do something to the data before ﬁ nally altering your code to
reﬂ ect the change. Often, you will have nothing to write here either.
 Build the plug-in. Since the default plug-in behavior is to simply pass audio unaffected,
you can build the project now and test it in RackAFX to make sure everything is working
properly. Rebuild your project from the compiler or from RackAFX’s Rebuild button. You
should get a clean build with no errors.
Writing Plug-Ins with RackAFX   53
 You should  always  build and test your brand-new project ﬁ rst before modifying any code! You
want to do this to make sure there are no C++ errors (you might have inadvertently hit a key or
changed some code), as well as to make sure your audio system is working and you can hear the
audio data.
 At this point, you have built a DLL which is designed to serve up CVolume objects when the
client requests them. The problem is that RackAFX doesn’t yet know your CVolume plug-in
is available. During the debugging phase, you need to get used to manually loading and
unloading the DLL. You do this with the Load button or the toolbar/menu item. After you
hit Load, RackAFX calls the appropriate functions to load your DLL into its address space.
You will see the control surface change to reﬂ ect that your plug-in is loaded. You will also
see the Load button change to read Unload. When it is time to go back to C11, modify, and
recompile, you’ll need to unload the project ﬁ rst so you can reload it in its later state.
 Use Audio > Load Audio File to load a test ﬁ le. Then use the transport control buttons to
play, loop, and pause or stop the ﬁ le. The volume control should have no effect since we
haven’t written any code yet. Make sure you get audio at this point before continuing; if you
don’t, check your audio adapter settings.
 In order to make the volume control slider work, we need to wire it into the processing code.
The volume slider is directly mapped to the volume coefﬁ cient m_fVolume; as the slider
moves from 0.0 to 1.0, so does the volume coefﬁ cient. So, the algorithm is simple to write:
just multiply the input audio sample by the volume coefﬁ cient and set the output to that value.
Switch to your C11 compiler and ﬁ nd the processAudioFrame() function. Modify it by doing
the multiplication described above, which implements the difference equation for the ﬁ lter.
  bool __stdcall CVolume::processAudioFrame(ﬂ oat* pInputBuffer, ﬂ oat* pOutputBuffer,
  UINT uNumInputChannels, UINT uNumOutputChannels)
  {
  // output = input -- change this for meaningful processing
  //
  // Do LEFT (MONO) Channel; there is always at least one input/one output
  // (INSERT Effect)
   pOutputBuffer[0] = pInputBuffer[0]*m_fVolume;
  // Mono-In, Stereo-Out (AUX Effect)
  if(uNumInputChannels == 1 && uNumOutputChannels == 2)
   pOutputBuffer[1] = pInputBuffer[0]*m_fVolume;
  // Stereo-In, Stereo-Out (INSERT Effect)
  if(uNumInputChannels == 2 && uNumOutputChannels == 2)
   pOutputBuffer[1] = pInputBuffer[1]*m_fVolume;
  return true;
  }
54  Chapter 3
Plug-In
processAudioFrame()
= pBuffer[0](cid:13)(cid:80)(cid:66)(cid:73)(cid:57)(cid:82)(cid:79)(cid:88)(cid:80)(cid:72)
Volume
0.75
 Figure 3.14:    Here is the connection between the slider and the variable in the
calculation. The processAudioFrame() function is using the raw slider data directly.
Raw Data:  0.0 to  1.0
 There are only three lines to modify, one for the ﬁ rst channel and another two for the other
routing combinations. The modiﬁ cation is shown in  bold  where you are scaling the input by
the volume coefﬁ cient. Can you see how this relates to the difference equation? If not, stop
now and go back to ﬁ gure it out. Now, rebuild the project and reload it into RackAFX. Try the
slider and you will see that the volume does indeed change. Congrats on your ﬁ rst plug-in!
 What makes this plug-in so easy and quick to develop is that the slider volume control
maps  directly  to a variable that is used in the processAudioFrame() function, as depicted in
 Figure 3.14 . This means the data coming from the slider can be used directly in the algorithm.
The data coming from the slider and controlling m_fVolume is said to be  raw data . You use
the raw value to affect the signal processing algorithm.
     3.10    Design a Volume-in-dB Plug-In
 This next example will show you how to  cook  your raw data to be used in the signal
processing algorithm. The VolumedB plug-in will also be a volume control, but will operate
in dB instead of using a raw multiplier. You may have noticed that your previous volume
control didn’t seem to do much in the upper half of the throw of the slider. This is because
your ears hear logarithmically and so linear deﬂ ections of the slider do not correspond to
linear changes in perceived loudness. To ﬁ x this, we’ll design another plug-in that will operate
in decibels (dB). The block diagram is identical to the ﬁ rst project, only the control range of
values has changed.
•
•
   a 2  5 −96 dB: Mute
   a 2  5 0 dB: Max volume
 Now, the volume control is speciﬁ ed in dB, so you need a formula to convert the dB value
to a scalar multiplier for the algorithm. You should memorize the dB equations now if you
Writing Plug-Ins with RackAFX   55
haven’t already, since they will reappear over and over in audio plug-ins. This is the  cooking
formula  that will take the raw data from the slider −96 to 0 dB and cook it into a variable we
can use in our scalar multiplication:
dB 5 20log(x)
x 5 10
dB
20
(3.2)
 You set up the cooking formula by implementing the userInterfaceChange() function which
is always called when the user changes anything on the control surface. When the user moves
the slider, you cook the data. This is shown conceptually in  Figure 3.15 .
 The cooking function simply converts the dB into a scalar multiplier. In this case, the
cooking function is short enough to simply leave inside the userInterfaceChange() function;
as the cooking functions become more complex, you will probably want to break them out
into separate functions, which are called from userInterfaceChange(). Remember the two
RackAFX rules you’ve learned so far:
  Coefﬁ cients  in a block diagram (or transfer function or algorithm) become  ﬂ oat member variables
in your plug-in code. Each slider or button control on the UI will map to and control a  member
variable  in your plug-in.
Plug-In
processAudioFrame()
=  pBuffer[0]*m_fVolume
Volume
–6.00 dB
userlnterfaceChange()
Cooking
Function
m_fVolume = 10
m_fVolume_dB /20
Cooked
Data:
~0.0 to  1.0
Raw Data: -96.0 to 0.0
 Figure 3.15:    The volume-in-dB plug-in will have a single slider that generates values
between −96 and 0.0 dB; you need to cook the raw dB values to use in your plug-in.

56  Chapter 3
 In the ﬁ rst plug-in, the variable was shared between the slider and the algorithm. Now
we need two variables, one for the raw slider data and the other for the cooked algorithm
processing. We will name them as follows:
•
•
   m_fVolume_dB: The raw slider data
   m_fVolume: The cooked value used in the algorithm
  3.10.1  Project: VolumedB
 Using the same method as before, create a new project named “VolumedB.” As before, you
don’t have to worry about the advanced options at the bottom of the new project window.
Your compiler will automatically start.
   3.10.2   VolumedB GUI
 We only need a single slider control. It will be adjustable from −96 to 0 dB. Set up the GUI
in RackAFX by choosing a slider and right-clicking inside the bounding box. Set the slider
according to  Table 3.1 .
   3.10.3   VolumedB.h File
 RackAFX has written the code and declared the variable ﬂ oat m_fVolume_dB but we still
need to declare our second variable named m_fVolume, which stores the cooked data. Open
the VolumedB.h ﬁ le and declare the variable in the user declarations area:
  // abstract base class for DSP ﬁ lters
  class CVolumedB : public CPlugIn
  {
  public:  // plug-in API Functions
  <SNIP SNIP SNIP>
  // Add your code here: ----------------------------------------------- //
   // our Cooked Volume Multiplier
   ﬂ oat m_fVolume;
  // END OF USER CODE -------------------------------------------------- //
  Table 3.1:    The slider properties for the VolumedB project.
Slider Property
Control name
Units
Variable type
Variable name
Low limit
High limit
Initial value
Value
Volume
dB
ﬂ oat
m_fVolume_dB
–96
0
–6

Writing Plug-Ins with RackAFX   57
   3.10.4   VolumedB.cpp File
  Constructor
•
•
   Cook and initialize the member variable.
   Use the pow() function.
  CVolumedB::CVolumedB()
  {
  // Added by RackAFX - DO NOT REMOVE
  //
  // Setup the RackAFX UI Control List and Initialize Variables
  initUI();
  // END InitUI
  <SNIP SNIP SNIP>
  // Finish initializations here
        // Cook the raw data:
   m_fVolume = pow(10.0, m_fVolume_dB/20.0);
  }
   prepareForPlay()
 There is nothing to do here because the volume variable does not need to be reset on each
play event.
   userInterfaceChange()
•
•
•
•
   Decode the control ID value.
   Cook the raw data using the formula.
   When the plug-ins get more complex, you can create separate cooking functions and then
share the functions as needed.
   Make sure you check your control ID values from the comments so that they match  properly.
  bool __stdcall CVolumedB::userInterfaceChange(int nControlIndex)
  {
  // decode the control index, or delete the switch and use brute force calls
  switch(nControlIndex)
  {
  case 0:
  {
  // Cook the raw data:
   m_fVolume = pow(10.0, m_fVolume_dB/20.0);
  }
  default:
  break;
    }
    return true;
  }
58  Chapter 3
   processAudioFrame()
•
   Implement the difference equation.
  bool __stdcall CVolumedB::processAudioFrame(ﬂ oat* pInputBuffer, ﬂ oat* pOutputBuffer,
  UINT uNumInputChannels, UINT uNumOutputChannels)
  {
  // output = input -- change this for meaningful processing
  //
  // Do LEFT (MONO) Channel; there is always at least one input/one output
  // (INSERT Effect)
   pOutputBuffer[0] = pInputBuffer[0]*m_fVolume;
  // Mono-In, Stereo-Out (AUX Effect)
  if(uNumInputChannels == 1 && uNumOutputChannels == 2)
   pOutputBuffer[1] = pInputBuffer[0]*m_fVolume;
 // Stereo-In, Stereo-Out (INSERT Effect)
  if(uNumInputChannels == 2 && uNumOutputChannels == 2)
   pOutputBuffer[1] = pInputBuffer[1]*m_fVolume;
  return true;
  }
 Build and test your plug-in. You should now hear a smooth volume transition as you move
the slider. Depending on your sound system and volume levels, you might not hear much
below −40 dB.
     3.11    Design a High-Frequency Tone Control Plug-In
 This example will show you how to implement the last of the digital signal processing (DSP)
algorithm building blocks: the delay element (z 2N ), where N is the number of samples of
delay. In this example, N 5 1, so we are dealing with a one-sample delay, a fundamental
building block for many DSP algorithms. Although we won’t get to the theory of how this
delay creates a tone control until the next chapter, it’s worth it to go ahead and build a plug-in
that uses it. After this exercise, you will be able to build almost any kind of DSP ﬁ lter that
uses one-sample-delay elements—and that’s a lot of DSP ﬁ lters. The later examples in the
book will not be as detailed regarding the operation of RackAFX and your C11 compiler, so
make sure you understand how to manipulate the software as needed to complete the project.
 Figure 3.16  shows the block diagram for the ﬁ lter.
 The design equation is as follows:
0.0 # a1 # 0.49
a0 5 a121.0
(3.3)
