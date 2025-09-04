# Audio Plugins C++ (Pirkle) — Chunk 34/73

> Source: `Designing Audio Effect Plug-Ins in C++_ With Digital Audio Signal Processing Theory - PDF Room.pdf` · Pages ~1-560 · Extractor: pdfminer.six
> Target ≈ 2800 words, Overlap = 200 words · Actual = 2294

> Note: DSP textbook content for SpectralCanvasPro RT-audio knowledge base

---
Delay Effects and Circular Buffers   227
  CDDLModule::CDDLModule()
  {
    <SNIP SNIP SNIP>
    // Finish initializations here
     m_fDelayInSamples = 0;
     m_fFeedback = 0;
     m_fDelay_ms = 0;
     m_fFeedback_pct = 0;
     m_fWetLevel = 0;
  }
 The formula for ﬁ guring out the delay time in samples from the delay time in milliseconds is
 Equation 7.21 .
  Samples delay  5  (D mSec) c
sample rate
1000
d
(7.21)
 Cooking the feedback value is easy—just divide by 100 to convert the percent to a raw
multiplier. The same thing is true for the wet level value. In this project, we’ll introduce the
concept of a cooking function to handle the work. Because we are planning on using this
plug-in as a module for future development, it will be a good idea. From this point on, you
should get in the habit of making a cooking function. In this case, we will keep it simple and
have the cooking function recalculate all the plug-in’s variables regardless of which ones
actually change. This is an effort to educate ﬁ rst. You can always go back and streamline
your functions and code after the plug-in has been tested and is functioning. First, declare the
cooking function in the .h ﬁ le:
  // Add your code here: --------------------------------------------------------- //
  ﬂ oat m_fDelayInSamples;
  ﬂ oat m_fFeedback;
  ﬂ oat m_fWetLevel;
   void cookVariables();
  // END OF USER CODE ------------------------------------------------------------ //
 Write the function:
  // function to cook the variables
   void CDDLModule::cookVariables()
   {
     m_fFeedback = m_fFeedback_pct/100.0;
     m_fWetLevel = m_fWetLevel_pct/100.0;
     m_fDelayInSamples = m_fDelay_ms*((ﬂ oat)m_nSampleRate/1000.0);
   }
 Then, we can add the cooking function to
•
•
•
   The end of the constructor()
   prepareForPlay()
   userInterfaceChange()

228  Chapter 7
   Constructor
  CDDLModule::CDDLModule()
  {
  <SNIP SNIP SNIP>
  // Finish initializations here
  <SNIP SNIP SNIP>
   m_fFeedback_pct = 0;
   m_fWetLevel = 0;
   cookVariables();
  }
   prepareForPlay()
  bool __stdcall CDDLModule::prepareForPlay()
  {
     // cook
     cookVariables();
    return true;
  }
 Notice the big change made here—rather than a switch/case statement, we just go ahead and
cook all the data.
   userInterfaceChange()
  bool __stdcall CDDLModule::userInterfaceChange(int nControlIndex)
  {
    // cook
     cookVariables();
    return true;
  }
 Now that the UI details are taken care of, we can get to the business of declaring the delay
line, initializing it, and ﬁ nally implementing the effect.
    7.3.5  Declare and Initialize the Delay Line Components
 For a delay line, you will need the following variables:
•
•
•
•
   A ﬂ oat* which points to a buffer of samples
   An integer read index
   An integer write index
   An integer that is the size of the buffer in samples

Delay Effects and Circular Buffers   229
 Add them to your .h ﬁ le:
  // Add your code here: --------------------------------------------------------- //
  ﬂ oat m_fDelayInSamples;
  ﬂ oat m_fFeedback;
  ﬂ oat m_fWetLevel;
   ﬂ oat* m_pBuffer;
   int m_nReadIndex;
   int m_nWriteIndex;
   int m_nBufferSize;
  // END OF USER CODE ------------------------------------------------------------ //
 The delay line will be created dynamically. It will be destroyed in the destructor. The problem
is that we don’t yet know what the sample rate will be; we won’t know that until the user
loads a new ﬁ le and begins playing it. Just before RackAFX calls your prepareForPlay()
function, it sets the sample rate on your plug-in’s m_nSampleRate variable. Therefore, we
will have to dynamically create and ﬂ ush out the buffer each time prepareForPlay() is called.
In the constructor, we set the m_pBuffer to NULL as a ﬂ ag to know that it is uninitialized, as
well as zero the buffer size and read and write index values.
  Constructor
  CDDLModule::CDDLModule()
  {
  <SNIP SNIP SNIP>
  m_fFeedback_pct = 0;
  m_fWetLevel = 0;
  // reset
   m_nReadIndex = 0;
   m_nWriteIndex = 0;
  // no buffer yet because we don’t have a sample rate yet
   m_pBuffer = NULL;
   m_nBufferSize = 0;
  // cook
  cookVariables();
  }
   prepareForPlay()
•
   Create the buffer now that we know the sample rate.
  bool __stdcall CDDLModule::prepareForPlay()
   {
     // setup our delay line
     m_nBufferSize = 2*m_nSampleRate;      // 2 seconds delay @ fs

230  Chapter 7
     // delete it if it exists
     if(m_pBuffer)
              delete [] m_pBuffer;
     // create the new buffer
     m_pBuffer = new ﬂ oat[m_nBufferSize];
    return true;
  }
   Destructor
•
   Delete the buffer.
  CDDLModule::~CDDLModule(void)
  {
    // delete buffer if it exists
     if(m_pBuffer)
       delete [] m_pBuffer;
  }
 To initialize the buffer with 0.0, use the the memset function. That memset() ﬂ ushes the
buffer of data, and we need to do this each time prepareForPlay() is called so we don’t play
out old data at the onset. We are going to be ﬂ ushing and resetting the buffer in several places
in code, so it  is also a good thing to make into a function.
    7.3.6  DDLModule.h File
  // Add your code here: --------------------------------------------------------- //
  ﬂ oat m_fDelayInSamples;
  ﬂ oat m_fFeedback;
  ﬂ oat m_fWetLevel;
  void cookVariables();
   void resetDelay();
  // END OF USER CODE ------------------------------------------------------------ //
   7.3.7  DDLModule.cpp File
  // function to ﬂ ush buffer and set Write pointer back to top
  // read pointer will be calculated based on write pointer location
   void CDDLModule::resetDelay()
   {
    // ﬂ ush buffer
     if(m_pBuffer)
       memset(m_pBuffer, 0, m_nBufferSize*sizeof(ﬂ oat));
    // init read/write indices
     m_nWriteIndex = 0; // reset the Write index to top
     m_nReadIndex = 0; // reset the Read index to top
  }

Delay Effects and Circular Buffers   231
 You can also modify the cooking function to add the code for updating the read index; set
the read and write indices depending on the amount of delay time, so you can use a modiﬁ ed
version of the code from  Section 7.1 . Note: The delay in samples is cast to an integer using
the casting method (int).
  // function to cook the variables
  void CDDLModule::cookVariables()
  {
    m_fFeedback = m_fFeedback_pct/100.0;
    m_fWetLevel = m_fWetLevel_pct/100.0;
    m_fDelayInSamples = m_fDelay_ms*((ﬂ oat)m_nSampleRate /1000.0);
     // subtract to make read index
     m_nReadIndex = m_nWriteIndex - (int)m_fDelayInSamples; // cast as int!
     //  check and wrap BACKWARDS if the index is negative
     if (m_nReadIndex < 0)
       m_nReadIndex += m_nBufferSize;      // amount of wrap is Read + Length
  }
  prepareForPlay()
•
   Reset the delay in prepareForPlay() after creating the buffer and before cookVariables().
  bool __stdcall CDDLModule::prepareForPlay()
   {
    // setup our delay line
    m_nBufferSize = 2*m_nSampleRate;                    // 2 seconds delay @ fs
    // delete it if it exists
    if(m_pBuffer)
      delete [] m_pBuffer;
    // create the new buffer
    m_pBuffer = new ﬂ oat[m_nBufferSize];
     // reset
     resetDelay();
    // then cook
    cookVariables();
    return true;
  }
   processAudioFrame()
•
   The ﬂ owchart for the processAudioFrame() function is shown in  Figure 7.25 .
 Note: We have one minor detail to deal with, and this is going to happen when we use the
delay line in a read-then-write fashion. If the user has chosen 0.00 mSec of delay, then

232  Chapter 7
y(n) = DDL[read]
DDL[write] =
x(n) + fb*y(n)
Output =
wet*y(n) + (1-wet)*x(n)
Increment:
read++
write++
Check for  wrap
 Figure 7.25:     Flowchart for processAudioFrame().
the write pointer and read pointer will be the same. This also occurs if the user selects the
 maximum  delay value since we want to read the oldest sample before writing it. So, we need
to make a check to see if there is no delay at all and deal with it accordingly.
  bool __stdcall CDDLModule::processAudioFrame(ﬂ oat* pInputBuffer, ﬂ oat* pOutputBuffer,
  UINT uNumInputChannels, UINT uNumOutputChannels)
  {
    // SYNC CODE: DO NOT REMOVE - DO NOT PLACE CODE BEFORE IT
    WaitForUIChangeDone();
    setProcessAudioDone(false);
     // END SYNC CODE
    // Do LEFT (MONO) Channel
     // Read the Input
     ﬂ oat xn = pInputBuffer[0];
     // Read the output of the delay at m_nReadIndex
     ﬂ oat yn = m_pBuffer[m_nReadIndex];
     // if zero delay, just pass the input to output
     if(m_fDelayInSamples == 0)
       yn = xn;
     // write the input to the delay
     m_pBuffer[m_nWriteIndex] = xn + m_fFeedback*yn;
     // create the wet/dry mix and write to the output buffer
     // dry = 1 - wet
     pOutputBuffer[0] = m_fWetLevel*yn + (1.0 - m_fWetLevel)*xn;
     // incremnent the pointers and wrap if necessary
     m_nWriteIndex++;
     if(m_nWriteIndex >= m_nBufferSize)
       m_nWriteIndex = 0;
     m_nReadIndex++;
     if(m_nReadIndex >= m_nBufferSize)
       m_nReadIndex = 0;
    // Mono-In, Stereo-Out (AUX Effect)
    if(uNumInputChannels == 1 && uNumOutputChannels == 2)
       pOutputBuffer[1] = pOutputBuffer[0]; // copy MONO!

Delay Effects and Circular Buffers   233
    // DDL Module is MONO - just do a copy here too
    // Stereo-In, Stereo-Out (INSERT Effect)
    if(uNumInputChannels == 2 && uNumOutputChannels == 2)
       pOutputBuffer[1] = pOutputBuffer[0];  // copy MONO!
    // SYNC CODE: DO NOT REMOVE
     setProcessAudioDone();
    return true;
  }
 Build and test the module in RackAFX to make sure it works. You should get a generic delay
effect with up to two seconds of delay. We only have two items to take care of to complete
this ﬁ rst version of the module. The ﬁ rst is easy: we need to provide the user of the module
with a way to choose external feedback and have access to the feedback variable. The second
is more difﬁ cult: we need to be able to handle fractional delay.
 In order for someone to use this module inside another plug-in and get access to the feedback
path, we need to provide some functions that will only be called by a plug-in parent. We also
need to modify the processAudioFrame() function to use an externally supplied feedback sample.
     7.4    Modifying the Module to Be Used by a Parent Plug-In
 It’s actually pretty easy to modify this module to work as an internal module for a plug-in. We
need to provide a variable for the feedback sample and allow the user to set this value. We also
need to provide a way to allow the user to get the current feedback value. Finally, we need a
switch to allow the user to select the external feedback mode. That switch will be in the form
of a boolean ﬂ ag. After we get the variables set up, we can modify the processAudioFrame()
function to use the external feedback sample.
  7.4.1  DDLModule.h File
 Declare the following new variables:
   bool  m_bUseExternalFeedback ; // ﬂ ag for enabling/disabling
   ﬂ oat m_fFeedbackIn ;           // the user supplied feedback sample value
 Also, declare and implement the get/set functions. They are so short you can just put them in
the .h ﬁ le rather than implementing them in the .cpp ﬁ le.
  // Add your code here: --------------------------------------------------------- //
  ﬂ oat m_fDelayInSamples;
  ﬂ oat m_fFeedback;
  ﬂ oat m_fWetLevel;
  void cookVariables();
  void resetDelayLine();

234  Chapter 7
  ﬂ oat* m_pBuffer;
  int m_nReadIndex;
  int m_nWriteIndex;
  int m_nBufferSize;
   bool  m_bUseExternalFeedback; // ﬂ ag for enabling/disabling
   ﬂ oat  m_fFeedbackIn;           // the user supplied feedback sample value
   // current FB is fb*output
   ﬂ oat getCurrentFeedbackOutput(){return m_fFeedback*m_pBuffer[m_nReadIndex];}
   // set the feedback sample
   void  setCurrentFeedbackInput(ﬂ oat f){m_fFeedbackIn = f;}
   // enable/disable external FB source
   void  setUsesExternalFeedback(bool b){m_bUseExternalFeedback = false;}
  // END OF USER CODE ------------------------------------------------------------ //
 The current feedback sample output is found at m_pBuffer[m_nReadIndex] and multiplied
by the feedback coefﬁ cient as per the block diagram. By allowing the user to get and set the
feedback, we allow them to break into the loop and insert other effects, or use the feedback
output for some other purpose altogether.
   7.4.2  DDLModule.cpp File
  processAudioFrame()
•
   Modify the function to allow the use of externally supplied feedback samples:
  bool __stdcall CDDLModule::processAudioFrame(ﬂ oat* pInputBuffer, ﬂ oat* pOutputBuffer,
  UINT uNumInputChannels, UINT uNumOutputChannels)
  {
    // Do LEFT (MONO) Channel
    // Read the Input
    ﬂ oat xn = pInputBuffer[0];
    // Read the output of the delay at m_nReadIndex
    ﬂ oat yn = m_pBuffer[m_nReadIndex];
    // if zero delay, just pass the input to output
    if(m_fDelayInSamples == 0)
        yn = xn;
     // write the input to the delay
     if(!m_bUseExternalFeedback)
             m_pBuffer[m_nWriteIndex] = xn + m_fFeedback*yn; // normal
     else
              m_pBuffer[m_nWriteIndex] = xn + m_fFeedbackIn; // external feedback

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
