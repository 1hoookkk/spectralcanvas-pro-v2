# Audio Plugins C++ (Pirkle) — Chunk 70/73

> Source: `Designing Audio Effect Plug-Ins in C++_ With Digital Audio Signal Processing Theory - PDF Room.pdf` · Pages ~1-560 · Extractor: pdfminer.six
> Target ≈ 2800 words, Overlap = 200 words · Actual = 2346

> Note: DSP textbook content for SpectralCanvasPro RT-audio knowledge base

---
The VST® and AU® Plug-In APIs  509
2.
   setParameter: You cook the raw data (0–1) for use in your plug-in.
    void CDelay::setParameter (long index, ﬂ oat value)
   {
   switch (index)
   {
   case uDelayTime:
   {
   m_fDelayTime  = (m_fMaxDelayTime – m_fMinDelayTime)* value +
     fMinDelayTime;
   }
   // same thing for the other variables
   }
   }
 Thus, every time you add or remove a variable, you have to update your enumerated list,
change the variables and min/max pairs, and update the ﬁ ve functions above.
   A.4.2  Signal Processing
 You will notice that you’ve already had to write a lot of code just to deal with the UI; you
have not written any code to do any meaningful processing. In RackAFX, you alter three
functions for signal processing:
1.
2.
3.
   Constructor
   prepareForPlay()
   processAudioFrame()
  Constructor
•
•
•
•
•
   Init all variables.
   Set our input and output channel count (this is also done in the CPlugIn constructor).
   Set a unique four-character identiﬁ er (required by VST).
   Call the base class’s constructor to pass it the number of presets (0 for now) and number
of controls.
   The audioMasterCallback you see is a mechanism for client 4 plug-in communication;
its details are hidden from developers.
  CDelay::CDelay (audioMasterCallback audioMaster)
   : AudioEffectX (audioMaster, 0, uNumParams) // 0 presets, uNumParams  controls
   {
   m_fDelayTime = 1000;
   m_fFeedback = 0;
   m_fWetDry = 50;
   // VST Speciﬁ c setup
   setNumInputs (2);

510  Appendix A
   setNumOutputs (2);
   setUniqueID ('WDly');
   }
   prepareForPlay() u setSampleRate() and resume()
 prepareForPlay() is special for RackAFX because it is called after the sample rate is
set. In VST, this is done in setSampleRate(). We also use prepareForPlay() to ﬂ ush out
our delay line buffers and do any other preparations for the next run of the plug-in.
The resume() function is called when the VST plug-in is turned on so we can ﬂ ush
buffers there.
   void CDelay::setSampleRate(ﬂ oat sampleRate)
   {
   // Left
   if(m_pLeftBuffer)
   delete m_pLeftBuffer;
  // create a 2 second buffer
   m_pLeftBuffer = new ﬂ oat[2*(int) sampleRate];
   // Right
   if(m_pRightBuffer)
   delete m_pRightBuffer;
   // create a 2 second buffer
   m_pRightBuffer = new ﬂ oat[2*(int) sampleRate];
   // ﬂ ush buffers
   memset (m_pLeftBuffer, 0, 2*(int) sampleRate * sizeof (ﬂ oat));
   memset (m_pRightBuffer, 0, 2*(int) sampleRate * sizeof (ﬂ oat));
   }
   void CDelay::resume()
   {
   // ﬂ ush buffer
   memset (m_pLeftBuffer, 0, 2*(int) sampleRate * sizeof (ﬂ oat));
   memset (m_pRightBuffer, 0, 2*(int) sampleRate * sizeof (ﬂ oat));
   // reset read/write indices, etc…
   m_nReadIndex = 0;
   m_nWriteIndex = 0;
   etc…
   }
   processAudioFrame() u processReplacing()
 The fundamental difference in the processing functions is in how the data arrives. In
RackAFX, the default mechanism is that it arrives in frames, where each frame is a sample

The VST® and AU® Plug-In APIs  511
Inputs
inputs[0]
inputs[1]
Outputs
outputs[0]
outputs[1]
Left  Input
Left  Output
Right  Input
Right  Output
Figure A.3: VST audio data is delivered in buffers of frames.
from each channel. The frames arrive as pointers to buffers. In VST, the audio is processed
in  buffers  of frames, rather than single frames.  Figure A.3  shows a two-channel in and
two-channel out VST setup.
 Instead of passing a pointer to a buffer, you pass a pointer to a buffer of pointers, or ﬂ oat**
variables. The processReplacing() method also passes you a count of the number of frames.
You use these variables to iterate through and access your audio data. VST was designed
to allow you to have any number of input and output channels, which you specify in the
constructor. The parameters that are passed to the processReplacing() method are as follows:
•
•
•
   ﬂ oat** inputs: A pointer to a buffer of pointers to input buffers
   ﬂ oat** outputs: A pointer to a buffer of pointers to output buffers
   Long sampleframes: The count of frames in each buffer
   void CDelay::processReplacing(ﬂ oat **inputs, ﬂ oat **outputs, long sampleframes)
   {
  // pick up pointers to buffers
   ﬂ oat *inL =  inputs[0];
   ﬂ oat *inR =  inputs[1];
   ﬂ oat *outL = outputs[0];
   ﬂ oat *outR = outputs[1];
   UINT uFrameCount = 0;
   while (––sampleFrames >= 0)
   {
  // pick up samples from buffers
   ﬂ oat fxnL = inL[uFrameCount];
   ﬂ oat fxnR = inR[uFrameCount];
  // calculate and write out
   outL[uFrameCount] = doLeftDelayFunction(fxnL);
   outR[uFrameCount] = doRightDelayFunction(fxnR);
512  Appendix A
  // inc frame counter
   uFrameCount++;
   }
   }
 You can now see the similarities and differences in RackAFX and VST:
•
•
•
   There’s more overhead for dealing with the GUI—even if you write your own GUI, you
must provide the default implementation functions because not all VST clients have
 custom GUI capability.
   VST uses setSampleRate() and resume() to change the sample rate or restart the
plug-in.
   VST processes in buffers of frames rather than frames.
     A.5    VST Plug-In with RackAFX Wrapping
 To wrap the RackAFX plug-in you ﬁ rst need to create a new AudioEffectX derived class.
Then, you make one of the member variables a RackAFX plug-in object. Suppose you have a
delay plug-in named “CStereoDelay” and you want to embed it into the VST plug-in. In the
.h ﬁ le you simply declare an instance of it: CStereoDelay m_Delay.
  A.5.1  Default GUI
 You still need to ﬁ ll in those ﬁ ve VST functions, but you will be using your plug-in to supply
the information.
  getParameterName
•
   Copy the name of object; you use the getAt() method to ﬁ nd the UI object in the
linked list.
   void CDelay::getParameterName (long index, char *label)
   {
  // get the GUI object
   CUICtrl* pUICtrl = m_Delay.m_UIControlList.getAt(index);
  if(!pUICtrl) return;
  // copy the ControlName
   strcpy (label, pUICtrl->cControlName);
  }
   getParameterDisplay
•
   Access the variable via the pointer; this example assumes all parameters are ﬂ oat
 variables, but you can always check the value of the uUserDataType variable to ﬁ nd out
what kind of variable the object stores and branch accordingly.
The VST® and AU® Plug-In APIs  513
   void CDelay::getParameterDisplay (long index, char *text)
   {
  // get the GUI object
   CUICtrl* pUICtrl = m_Delay.m_UIControlList.getAt(index);
  if(!pUICtrl) return;
  // use the helper function to copy the data
   ﬂ oat2string (*pUICtrl->m_pUserCookedFloatData, text);
   }
   getParameterLabel
•
   You create a  string  that represents the units for the control.
   void CDelay::getParameterLabel (long index, char *label)
   {
  // get the GUI object
   CUICtrl* pUICtrl = m_Delay.m_UIControlList.getAt(index);
  if(!pUICtrl) return;
  // copy the ControlUnits
   strcpy (label, pUICtrl->cControlUnits);
   }
   setParameter/getParameter
•
   Here is where the CPlugIn helper functions work for you; all you need to do is forward
the call.
   ﬂ oat CDelay::getParameter(long index)
   {
   return m_Delay.getParameter(index);
   }
   void CDelay::setParameter(long index, ﬂ oat value)
   {
   m_Delay.setParameter(index, value);
   }
   Constructor
 For the constructor you will need to handle one minor detail: you need to know how many
legal VST controls you have. Unfortunately, you need to pass these to the AudioEffect base
class constructor before your member object plug-in is constructed. You will need to count
your slider controls (including any embedded in the LCD control in  Appendix B ) and radio
button bank controls. You can also look through the initUI() method and count all control
objects with either the FILTER_CONTROL_CONTINUOUSLY_VARIABLE or FILTER_
CONTROL_RADIO_SWITCH_VARIABLE as the uControlType. In this case, we have three
legal VST controls.

514  Appendix A
  CDelay::CDelay (audioMasterCallback audioMaster)
   : AudioEffectX  (audioMaster, 0, 3) // 0 presets, 3 controls
  {
  }
  // child object is now instantiated
  //
  // VST Speciﬁ c setup; get info from embedded plug-in
   setNumInputs (m_Delay.m_uMaxInputChannels);
   setNumOutputs (m_Delay.m_uMaxOutputChannels);
   setUniqueID ('WDly');
   processReplacing
•
   In order to use processReplacing() you need to combine samples from the input/output
(I/O) buffers into frames to send the processAudioFrame() function. This version is for a
stereo insert effect.
   void CDelay::processReplacing(ﬂ oat **inputs, ﬂ oat **outputs, long sampleframes)
   {
  // pick up pointers to buffers
  ﬂ oat *inL =  inputs[0];
  ﬂ oat *inR =  inputs[1];
  ﬂ oat *outL = outputs[0];
  ﬂ oat *outR = outputs[1];
  UINT uFrameCount = 0;
  // mini frames
   ﬂ oat fInputFrame[2];
   ﬂ oat fOutputFrame[2];
   while (––sampleFrames >= 0)
   {
  // pick up samples from buffers
   fInputFrame[0] = inL[uFrameCount];
   fInputFrame[1] = inR[uFrameCount];
  // calculate and write out (…2, 2) = 2 in/2 out
   m_Delay.processAudioFrame(&fInputFrame[0], &fOutputFrame[0], 2, 2);
  // inc frame counter
  uFrameCount++;
   }
   }
     A.6    AU Overview
 VST was a bit more complex in that you had to do some of the GUI work yourself. Whether
you write one from scratch or wrap the RackAFX plug-in, you are still stuck with the default

The VST® and AU® Plug-In APIs  515
GUI. AU plug-ins are another order of magnitude in complexity. The actual audio processing
function is fairly simple. The large majority of the work goes into setting up the default GUI and
maintaining your presets. Unlike both RackAFX and VST, AU plug-ins are not ANSI C/C++;
they require the Audio Units and Core Foundation components, so you need to be familiar with
Mac programming if you are going to get into AU programming. AU uses two base class objects
together to implement a plug-in. When you create a new Audio Unit plug-in in XCode, it sets up
the derived classes for you, one from each base class. You then override methods on both of them.
•
•
   AUEffectBase handles the GUI controls and any other non-audio-processing details.
   AUKernelBase handles the processing and reset controls.
  A.6.1  Default GUI
 Suppose we have a simple volume plug-in with just one volume control. AU calls a control a
“parameter.” Similar to VST, you ﬁ rst deﬁ ne an enumerated list of UINT constant values in
your Plugin.h ﬁ le where “Plugin” is the name and you have derived it from the AUEffectBase
base class. These constants are the control ID values, just as before.
   enum {
   };
   kParam_One =0,
   // Add your parameters here... increment kNumberOfParameters
   kNumberOfParameters=1
 In the same ﬁ le, you create constant deﬁ nitions for the initial value and name.
  // set the initial value
   static const ﬂ oat kDefaultValue_ParamOne = 0.5;
  // set the name; CFStringRef is a Core Foundation String object
   static CFStringRef kParameterOneName = CFSTR(“Volume”);
 You declare your controls by ﬁ lling in the GetParameterInfo() method. It is beyond the
scope of this book to analyze the code line by line, but you can get a feel for how it works by
looking at the way the objects are set up.
  OSStatus AUVolume::GetParameterInfo(AudioUnitScope
inScope,
AudioUnitParameterID
AudioUnitParameterInfo  &outParameterInfo)
inParameterID,
  {
  <SNIP SNIP SNIP>
   switch(inParameterID)
   {
   case kParam_One:
     AUBase::FillInParameterName (outParameterInfo, kParameterOneName,
false);

516  Appendix A
   outParameterInfo.unit = kAudioUnitParameterUnit_LinearGain;
   outParameterInfo.minValue = 0.0;
   outParameterInfo.maxValue = 1;
   outParameterInfo.defaultValue = kDefaultValue_ParamOne;
     break;
    default:
     result = kAudioUnitErr_InvalidParameter;
     break;
    }
  etc…
 Here, when the Client queries the plug-in on its ﬁ rst parameter (kParam_One) it ﬁ lls
out a structure about the control. You can see the minimum, maximum, and default
values being set. The “unit” variable of the structure is set to kAudioUnitParameterUnit_
LinearGain. A linear gain control moves between 0 and 1.0. Another example is
kAudioUnitParameterUnit_Boolean, which speciﬁ es a boolean control variable. What is
interesting here is that you don’t declare the actual variable that stores the control data.
The variable is a ﬂ oat variable that the framework stores for you. You tell the framework
what kind of variable it represents. When you need to get or set the variable, you use
GetParameter() and SetParameter(), which get and set a ﬂ oat data type.
   A.6.2  Signal Processing
 The AU processing function is simply named Process(). The audio data moves in buffers. The
buffers can contain any arbitrary number of channels with the simplest systems being  N  x  N
or equal input/outputs. Although the inNumChannels variable is passed to the function, it is
always set to 1.0 for the current version of AU. This is OK for mono or when you don’t need
to process channels independently. A boolean ﬂ ag is passed to the method, indicating the
mute condition and the function simply returns without processing. The parameters that are
passed to the Process() method are as follows:
•
•
•
•
•
   const Float32: *inSourceP (a pointer to an input buffer)
   Float32: *inDestP (a pointer to the output buffer)
   UInt32: InSamplesToProcess (number of samples in the input buffer)
   UInt32: inNumChannels (number of channels to process)
   bool: &ioSilence (indicates no processing, if true)
 Here is the AU process function for a volume plug-in.
  void AUVolume::AUVolumeKernel::Process(const Float32
Float32
  UInt32
UInt32
bool
*inSourceP,
*inDestP,
inFramesToProcess,
inNumChannels,
&ioSilence)

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
