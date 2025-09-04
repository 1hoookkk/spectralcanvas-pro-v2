# Audio Plugins C++ (Pirkle) — Chunk 12/73

> Source: `Designing Audio Effect Plug-Ins in C++_ With Digital Audio Signal Processing Theory - PDF Room.pdf` · Pages ~1-560 · Extractor: pdfminer.six
> Target ≈ 2800 words, Overlap = 200 words · Actual = 2670

> Note: DSP textbook content for SpectralCanvasPro RT-audio knowledge base

---
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

Writing Plug-Ins with RackAFX   59
x(n)
a0
y(n)
z-1
a1
 Figure 3.16:    The HF tone control block diagram.
 You already know that the coefﬁ cients a 0  and a 1  will become ﬂ oat member variables in our
plug-in. But what about the one-sample-delay element,  z  21 ? In hardware, this would be a
register to store the sample for one clock period. In software, it simply becomes another ﬂ oat
variable, but it must be able to persist over the lifetime of a processing cycle. Therefore, like
the coefﬁ cients, it will become a ﬂ oat  member  variable of our plug-in object.
 You will do an example using arrays of ﬂ oats in Chapter 7  when you implement digital delay
lines that require long strings of  z  21  elements. For now, we need to implement the single
 z  21  element in the block diagram. Right away, we need to decide if we are going to support
multichannel (stereo) operation.
 The last rule is really important and it is easy to get into trouble if you do not follow it. Unless
the algorithm speciﬁ cally deals with multichannel data, you will need to implement a separate
algorithm for each channel, which means having separate coefﬁ cient and data (z 21 ) elements
for each channel. Even if you might be able to share the coefﬁ cients, you can never share the
delay elements. We will need to declare variables for the following:
•
•
•
•
   Left channel a 0  and a 1  variables
   Left channel  z  21  variable
   Right channel a 0  and a 1  variables
   Right channel  z  21  variable
 Delay elements will become  float member variables  in your plug-in object. For single-delay
 elements, you can simply assign separate variables. For multiple-sample-delay elements you
may also use float arrays to store the data.
 A DSP filtering algorithm, which is only described in mono or single channel format, that is,
one input,  x ( n ), and one output,  y ( n ), cannot share delay elements between multiple channels.
This means that you must duplicate your algorithms so that you have one set of variables for
the left channel and one for the right channel.
60  Chapter 3
  3.11.1  Project: SimpleHPF
 This plug-in is going to implement a very primitive HF (high frequency) tone control
that behaves like a high-pass ﬁ lter. It will attenuate low frequencies, leaving only the
highest frequencies intact. Using the same method as before, create a new project named
“SimpleHPF.” Check your compiler to make sure the project was created properly.
   3.11.2  SimpleHPF GUI
 This simple plug-in will only have one slider that will control the value of the a 1  coefﬁ cient.
The other coefﬁ cient is calculated from it. The speciﬁ cations show that a 1  varies between 0
and 0.49. Set up the slider according to the properties in  Table 3.2 .
   3.11.3  SimpleHPF.h File
 To ﬁ gure out what the CSimpleHPF object is going to have to do, ﬁ rst write the difference
equation. Examine it and ﬁ gure out which components are going to become coefﬁ cients
and which are going to be memory locations. Also, ﬁ gure out any intermediate variables
you might need. You can ﬁ gure out the difference equation by using the rules you learned in
 Chapter 1  to chart the input and output signals. Make sure you understand how this equation
is formed from the block diagram in  Figure 3.17 .
 The difference equation is as follows:
 y(n) 5 a0 x(n) 1 a1x(n 2 1)
(3.4)
 Next, ﬁ gure out which block diagram components become variables in your C11 code.
The coefﬁ cients a 0  and a 1  will become a ﬂ oat member variable in the code. Even though we
might be tempted to share the coefﬁ cients, these are separate left and right algorithms that
have separate delay elements, so let’s implement two sets, one each for the left and right
channels.
  Table 3.2:    The slider properties for the SimpleHPF project.
Slider Property
Control name
Units
Variable type
Variable name
Low limit
High limit
Initial value
Value
a1
ﬂ oat
m_fSlider_a1
0.0
0.49
0
Writing Plug-Ins with RackAFX   61
a0 x(n)
x(n)
a 0
y(n)
-1
z
x(n-1)
a 1
a1x(n-1)
 Figure 3.17:    The HF tone control block diagram with annotations showing the signal math.
I named mine:
  m_f_a0_left
  m_f_a1_left
  m_f_a0_right
  m_f_a1_right
 The  z  21  element will also need to become a member variable and we will deﬁ nitely need one
for each channel because these can never be shared. I named mine
  m_f_z1_left
  m_f_z1_right
 The slider will only modify its own m_fSlider_a1 value. We will calculate the values for the
other coefﬁ cients using it. We will need to modify the userInterfaceChange() function just
like the preceding example to wire the slider into the algorithm. Jump to your C11 compiler
and go to the SimpleHPF.h ﬁ le to add your member variables. Notice the variable that
RackAFX added in the code below:
  // 5. userInterfaceChange() occurs when the user moves a control.
  virtual bool userInterfaceChange(int nControlIndex);
  // Add your code here: ------------------------------------------------------- //
   ﬂ oat m_f_a0_left;
   ﬂ oat m_f_a1_left;
   ﬂ oat m_f_a0_right;
   ﬂ oat m_f_a1_right;
   ﬂ oat m_f_z1_left;
   ﬂ oat m_f_z1_right;
  // END OF USER CODE ---------------------------------------------------------- //
  // ADDED BY RACKAFX -- DO NOT EDIT THIS CODE!!! ------------------------------- //
  //  **--0x07FD--**
62  Chapter 3
  ﬂ oat m_fSlider_a1;
  // **--0x1A7F--**
  // --------------------------------------------------------------------------- //
   3.11.4  SimpleHPF.cpp File
  Constructor
•
•
•
   Set our coefﬁ cient values to match the initialized slider settings.
   Calculate the new a 0  values.
   Clear out the  z  21  variables.
  CSimpleHPF::CSimpleHPF()
  {
  <SNIP SNIP SNIP>
 // Finish initializations here
   m_f_a1_left = m_fSlider_a1;
   m_f_a1_right = m_fSlider_a1;
   m_f_a0_left = m_f_a1_left - 1.0;
   m_f_a0_right = m_f_a1_right - 1.0;
   m_f_z1_left = 0.0;
   m_f_z1_right = 0.0;
  }
   prepareForPlay()
  bool __stdcall CSimpleHPF::prepareForPlay()
  {
  // Add your code here:
   m_f_z1_left = 0.0;
   m_f_z1_right = 0.0;
  return true;
  }
   processAudioFrame()
 The logic for the signal processing of  one channel  will be as follows:
•
•
•
•
   Read the delayed value  x ( n 21) out of the  z  21  element.
   Implement the difference equation.
   Write the current input  x ( n ) into the delay variable; it will be  x ( n 21) next time around.
   Do this for both channels.

   bool __stdcall CSimpleHPF::processAudioFrame(ﬂ oat* pInputBuffer, ﬂ oat*
 pOutputBuffer, UINT uNumChannels)
  {
Writing Plug-Ins with RackAFX   63
  // Do LEFT (MONO) Channel
  //
   // Input sample is x(n)
   ﬂ oat xn = pInputBuffer[0];
   // READ: Delay sample is x(n-1)
   ﬂ oat xn_1 = m_f_z1_left;
   // Difference Equation
   ﬂ oat yn = m_f_a0_left*xn + m_f_a1_left*xn_1;
 // WRITE: Delay with current x(n)
   m_f_z1_left = xn;
 // Output sample is y(n)
   pOutputBuffer[0] = yn;
 OK, now it’s your turn to implement the other channel. Give it a try by yourself before
proceeding. You should have something like this for the rest of the function:
  // Mono-In, Stereo-Out (AUX Effect)
  if(uNumInputChannels == 1 && uNumOutputChannels == 2)
  pOutputBuffer[1] =  yn ;
 // Stereo-In, Stereo-Out (INSERT Effect)
  if(uNumInputChannels == 2 && uNumOutputChannels == 2)
  {
  // Input sample is x(n)
   ﬂ oat xn = pInputBuffer[1];
  // Delay sample is x(n-1)
   ﬂ oat xn_1 = m_f_z1_right;
  // Difference Equation
   ﬂ oat yn = m_f_a0_right*xn + m_f_a1_right*xn_1;
 // Populate Delay with current x(n)
   m_f_z1_right = xn;
  // Output sample is y(n)
   pOutputBuffer[1] = yn;
  }
  return true;
  }

64  Chapter 3
 Flush out delay elements in preparation for each play event in the plug-in. You generally do not
want old data sitting inside these storage registers. The only exceptions are delay-looping effects
where you exploit the old data. This is done in prepareForPlay().
   userInterfaceChange()
•
•
   Store the new a 1  values.
   Cook the slider data to get the a 0  values.
  bool __stdcall CSimpleHPF::userInterfaceChange(int nControlIndex)
  {
  switch(nControlIndex)
  {
  case 0:
  {
  // save a1
   m_f_a1_left = m_fSlider_a1;
   m_f_a1_right = m_fSlider_a1;
        // calculate a0
   m_f_a0_left = m_f_a1_left - 1.0;
   m_f_a0_right = m_f_a1_right - 1.0;
  break;
  }
  default:
  break;
  }
  return true;
  }
 Build and load the project, open an audio ﬁ le, and test your plug-in to make sure it’s working
properly. This plug-in is a simple low-cut ﬁ lter and with the slider all the way down, you should
hear no difference in the music. When you move the slider up, you will lose more and more bass
frequencies, allowing only the high frequencies to pass. The cut-off frequency is approximately
11 kHz and the slider controls the amount of low frequency cut. You should easily be able to hear
the effect, even over small speakers. Now, to get a little more information about the plug-in’s
operation, use RackAFX’s analysis tools. Stop the music from playing with the transport control.
Launch the analyzer window by clicking on the Analyzer button in the toolbar or choose View >
Analyzer. The analyzer pops up as shown in  Figure 3.18  (yours may look slightly different).
 The analyzer is a powerful tool for checking your plug-in’s performance. The basic controls are
1.
2.
3.
4.
   Scope/spectrum analyzer.
   Basic graphing options.
   Scope controls.
   Real-time response buttons, which let you measure the frequency, phase, impulse, and
step responses of the plug-in (audio must not be streaming to use these).
 Click on the Frequency button and you will get a frequency response plot of the ﬁ lter. Move
the slider all the way down and you should get a ﬂ at response, as shown in  Figure 3.19 . If you
move the slider all the way up so that a 1  5 0.49, you get the response in  Figure 3.20 .
Writing Plug-Ins with RackAFX   65
1
2
+12.0dB
0.0dB
-12.0dB
-24.0dB
-36.0dB
-48.0dB
-60.0dB
4
3
 Figure 3.18:    The audio analyzer.
10Hz
100Hz
1kHz
10kHz
 Figure 3.19:    A ﬂ at frequency response with a 1  = 0.0.
66  Chapter 3
+12.0dB
0.0dB
-12.0dB
-24.0dB
-36.0dB
-48.0dB
-60.0dB
10Hz
100Hz
1kHz
10kHz
 Figure 3.20:    A ﬁ lter that appears to boost high frequencies. You can see that
it is really cutting the low frequencies instead; this is something you might only
have realized by using the audio analyzer.
     3.12    Design a High-Frequency Tone Control with Volume Plug-In
 This ﬁ nal example will show you how to deal with more than one slider control by simply
adding a volume-in-dB control to the block diagram. The plan is to add another slider to
the existing plug-in; the new slider will control the overall volume of the plug-in in dB. You
already know how to implement both parts of it, so this exercise is really more about adding
new controls to an existing project.
  3.12.1  Project: SimpleHPF
 Open your SimpleHPF project in RackAFX using the Open button or the menu/toolbar items.
   3.12.2  SimpleHPF GUI
 Add the new volume slider: Right-click on the second slider group and add a new slider
for the volume control, in dB, and use the exact same variable name and settings as in the
VolumedB project. You should end up with a GUI like that in  Figure 3.21 .
   3.12.3  SimpleHPF.h File
 Add your own m_fVolume variable to handle the cooked volume data, just as before.
  // Add your code here: --------------------------------------------------------- //
  ﬂ oat m_f_a0_left;
  ﬂ oat m_f_a1_left;
  ﬂ oat m_f_a0_right;
  ﬂ oat m_f_a1_right;
  ﬂ oat m_f_z1_left;
  ﬂ oat m_f_z1_right;
   ﬂ oat m_fVolume;
Writing Plug-Ins with RackAFX   67
a1
0.00
Volume
-6.00
 Figure 3.21:    The new SimpleHPF GUI with added volume-in-dB control.
  // END OF USER CODE ------------------------------------------------------------ //
 // ADDED BY RACKAFX -- DO NOT EDIT THIS CODE!!! -------------------------------- //
  //   **--0x07FD--**
  ﬂ oat m_fSlider_a1;
  ﬂ oat m_fVolume_dB;
  // **--0x1A7F--**
  // ----------------------------------------------------------------------------- //
   3.12.4  SimpleHPF.cpp File
  Constructor
•
   Cook the volume data after the ﬁ lter initializations.
  CSimpleHPF::CSimpleHPF()
  {
  <SNIP SNIP SNIP>
  m_f_a0_left = −1.0;
  m_f_a1_left = 0.0;
  m_f_a0_right = −1.0;
  m_f_a1_right = 0.0;
  m_f_z1_left = 0.0;
  m_f_z1_right = 0.0;
   m_fVolume = pow(10.0, m_fVolume_dB/20.0);
  }
   prepareForPlay()
 There is nothing to add here because the volume variable does not need to be reset on each
play event.

68  Chapter 3
   processAudioFrame ()
•
   Add the volume control scaling  after  the ﬁ ltering operation.
  // Do LEFT (MONO) Channel; there is always at least one input/one output
  // (INSERT Effect)
  // Input sample is x(n)
  ﬂ oat xn = pInputBuffer[0];
  // READ: Delay sample is x(n−1)
  ﬂ oat xn_1 = m_f_z1_left;
  // Difference Equation
  ﬂ oat yn = m_f_a0_left*xn + m_f_a1_left*xn_1;
  // WRITE: Delay with current x(n)
  m_f_z1_left = xn;
  // Output sample is y(n)
   pOutputBuffer[0] = yn*m_fVolume;
  // Mono-In, Stereo-Out (AUX Effect)
  if(uNumInputChannels == 1 && uNumOutputChannels == 2)
   pOutputBuffer[1] = yn*m_fVolume;
  // Stereo-In, Stereo-Out (INSERT Effect)
  if(uNumInputChannels == 2 && uNumOutputChannels == 2)
  {
  // Input sample is x(n)
  ﬂ oat xn = pInputBuffer[1];
  // Delay sample is x(n-1)
  ﬂ oat xn_1 = m_f_z1_right;
  // Difference Equation
  ﬂ oat yn = m_f_a0_right*xn + m_f_a1_right*xn_1;
  // Populate Delay with current x(n)
  m_f_z1_right = xn;
  // Output sample is y(n)
   pOutputBuffer[1] = yn*m_fVolume;
  }
   userInterfaceChange()
•
•
   Cook the volume data.
   Make sure you check your control ID values in case you chose different sliders than I did.
