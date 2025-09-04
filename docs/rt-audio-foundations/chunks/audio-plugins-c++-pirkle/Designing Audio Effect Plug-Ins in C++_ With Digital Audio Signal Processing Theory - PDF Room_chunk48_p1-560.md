# Audio Plugins C++ (Pirkle) — Chunk 48/73

> Source: `Designing Audio Effect Plug-Ins in C++_ With Digital Audio Signal Processing Theory - PDF Room.pdf` · Pages ~1-560 · Extractor: pdfminer.six
> Target ≈ 2800 words, Overlap = 200 words · Actual = 2657

> Note: DSP textbook content for SpectralCanvasPro RT-audio knowledge base

---
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

Modulated Delay Effects  337
  // functions to update the member objects
   void updateLFO();
   void updateDDL();
  // cooking function for mod type
   void cookModType();
  // convert a LFO value to a delay offset value
   ﬂ oat calculateDelayOffset(ﬂ oat fLFOSample);
  // END OF USER CODE ---------------------------------------------------------- //
   10.3.4  ModDelayModule.cpp File
  Constructor
•
   Initialize all variables.
  CModDelayModule::CModDelayModule()
  {
  <SNIP SNIP SNIP>
  // Finish initializations here
   m_fMinDelay_mSec = 0.0;
   m_fMaxDelay_mSec = 0.0;
   m_fChorusOffset = 0.0;
   m_LFO.m_fFrequency_Hz = 0;
   m_LFO.m_uOscType = m_uLFOType; // triangle  enum{tri,sin};
   m_DDL.m_bUseExternalFeedback = false;
   m_DDL.m_fDelay_ms = 0;
  }
 Write the update() functions:
  // LFO function to set:
  //                        - the LFO Frequency
  //                        - the oscillator type
   void CModDelayModule::updateLFO()
   {
           // set raw data
            m_LFO.m_fFrequency_Hz = m_fModFrequency_Hz;
            m_LFO.m_uOscType = m_uLFOType;
           // cook it
            m_LFO.cookFrequency();
   }
338  Chapter 10
  // DDL function to set:
  //                        - the DDL Feedback amount (disabled for Vibrato)
   void CModDelayModule::updateDDL()
   {
           // test and set if needed
            if(m_uModType != Vibrato)
                    m_DDL.m_fFeedback_pct = m_fFeedback_pct;
           // cook it
            m_DDL.cookVariables();
  }
 Write the cookModType() function using the information in  Table 10.3 —this method is called
when the user changes the effect type.
  // cookMod() function:
  /*
                  Min Delay (mSec)      Max delay (mSec)      Wet/Dry(%)      Feedback(%)
  Flanger      0                        7-10                      50/50            −100 to +100
  Vibrato      0                        7-10                      100/0            0
  Chorus        5                        20-40                    50/50            −100 to +100
  */
   void CModDelayModule::cookModType()
   {
           switch(m_uModType)
           {
                   case Flanger:
                   {
                             m_fMinDelay_mSec = 0;
                             m_fMaxDelay_mSec = 7;
                             m_DDL.m_fWetLevel_pct = 50.0;
                             m_DDL.m_fFeedback_pct = m_fFeedback_pct;
                             break;
                   }
                   case Vibrato:
                   {
                             m_fMinDelay_mSec = 0;
                             m_fMaxDelay_mSec = 7;
                             m_DDL.m_fWetLevel_pct = 100.0;
                             m_DDL.m_fFeedback_pct = 0.0;  // NOTE! no FB for vibrato
                             break;
                   }
  Table 10.3:    The default delay line settings for the various effects.
Min Delay (mSec)
Max Delay (mSec)
Wet/Dry (%)
Feedback (%)
Flanger
Vibrato
Chorus
0
0
7
7–10
7–10
20–40
50/50
100/0
50/50
2100 to 1100
0
Typically 0
Modulated Delay Effects  339
                   case Chorus:
                   {
                             m_fMinDelay_mSec = 5;
                             m_fMaxDelay_mSec = 30;
                             m_DDL.m_fWetLevel_pct = 50.0;
                             m_DDL.m_fInputAttenuation = 1.0;
                             break;
                   }
                   default:  // is Flanger
                 {
                             m_fMinDelay_mSec = 0;
                             m_fMaxDelay_mSec = 7;
                             m_DDL.m_fWetLevel_pct = 50.0;
                             m_DDL.m_fFeedback_pct = m_fFeedback_pct;
                             break;
                   }
           }
   }
 Write the calculateDelayOffset() function. It works by taking the LFO value from 0 to 1.0
and mapping it on to the range from min to max delay. Then, this value is scaled by the depth
percent value so that at 100% the maximum range is used. For the chorus, we effectively
move the operating range and chorus center by adding a delay offset to the minimum value,
then using that as the starting (reference) point in the calculation.
  // calculateDelayOffset():
  /*
           fLFOSample: a value from 0.0 to 1.0 from the LFO object
           returns: the calculated delay time in mSec for each effect
           NOTES: - the range for the ﬂ anger/vibrato is simply mapped from min to max
                       starting at min: fLFOSample*(m_fMaxDelay_mSec - m_fMinDelay_mSec)) +
                       m_fMinDelay_mSec
                     - the range for the Chorus includes the starting offset
                       fStart = m_fMinDelay_mSec + m_fChorusOffset;
  */
   if(m_uModType == Flanger || m_uModType == Vibrato)
   {
           // ﬂ anger 0->1 gets mapped to 0->maxdelay
            return (m_fModDepth_pct/100.0)*(fLFOSample*(m_fMaxDelay_mSec —
                  m_fMinDelay_mSec))       + m_fMinDelay_mSec;
            }
            else if(m_uModType == Chorus)
            {
  // chorus adds starting offset to move delay range
   ﬂ oat fStart = m_fMinDelay_mSec + m_fChorusOffset;
340  Chapter 10
            }
   }
   return (m_fModDepth_pct/100.0)*(fLFOSample*(m_fMaxDelay_mSec —
          m_fMinDelay_mSec)) + fStart;
    10.3.5  PrepareForPlay()
•
•
   Forward the call to prepareForPlay() on the member objects.
   Reset and start up the LFO in unipolar mode.
  bool __stdcall CModDelayModule::prepareForPlay()
  {
           // Add your code here:
            m_LFO.prepareForPlay();
            // DDL needs to know sample rate to initialize its buffer
            m_DDL.m_nSampleRate = m_nSampleRate;
            m_DDL.prepareForPlay();
            m_LFO.m_uPolarity = 1;    // 0 = bipolar, 1 = unipolar
            m_LFO.m_uTableMode = 0;   // normal, no band limiting
            m_LFO.reset();                // reset it
             // initialize
            cookModType();
            updateLFO();
            updateDDL();
             // start the LFO!
            m_LFO.m_bNoteOn = true;
           return true;
  }
  userInterfaceChange()
•
•
   Call the update functions and cook the mod type. We won’t bother with the switch/case
statement since changes to any slider will require updates of the LFO and DDL.
   Make sure your nControlIndex values match your GUI components.
  bool __stdcall CModDelayModule::userInterfaceChange(int nControlIndex)
  {
  // change the min/max limits; set wet/dry and Feedback
   if(nControlIndex == 41)  // 41 is mod type switch
   cookModType();
  // else just call the other updates which handle all the rest
  //
  // frequency and LFO type
   updateLFO();

Modulated Delay Effects  341
  // Wet/Dry and Feedback
   updateDDL();
  return true;
  }
   processAudioFrame()
•
•
•
•
   Call the doOscillate() function on the LFO.
   Use the LFO value to calculate a delay offset in milliseconds for the DDL module.
   Write the new delay time to the DDL module and call its cooking function.
   Call processAudioFrame() on the DDL module to get the current output sample.
  bool __stdcall CModDelayModule::processAudioFrame(ﬂ oat* pInputBuffer, ﬂ oat*
  pOutputBuffer, UINT uNumInputChannels,
UINT     uNumOutputChannels)
  {
  // Do LEFT (MONO) Channel
  //
  // 1. Get LFO Values, normal and quad phase
   ﬂ oat fYn = 0;
   ﬂ oat fYqn = 0;
   m_LFO.doOscillate(&fYn, &fYqn);
  // 2. calculate delay offset
   ﬂ oat fDelay = 0.0;
   if(m_uLFOPhase == quad)
   fDelay = calculateDelayOffset(fYqn); // quadrature LFO
   else
   fDelay = calculateDelayOffset(fYn); // normal LFO
  // 3. set the delay & cook
   m_DDL.m_fDelay_ms = fDelay;
   m_DDL.cookVariables();
  // 4. get the delay output one channel in/one channel out
   m_DDL.processAudioFrame(&pInputBuffer[0], &pOutputBuffer[0], 1, 1);
  // Mono-In, Stereo-Out (AUX Effect)
  if(uNumInputChannels == 1 && uNumOutputChannels == 2)
   pOutputBuffer[1] = pOutputBuffer[0];
  // Stereo-In, Stereo-Out (INSERT Effect)
  if(uNumInputChannels == 2 && uNumOutputChannels == 2)
   pOutputBuffer[1] = pOutputBuffer[0];
  return true;
  }
 Build and test the module. Be sure to listen to the ﬂ anger as the delay hits the zero point.

342  Chapter 10
    10.3.6  Challenge
 Add another radio button switch to turn on or off TZF that we employ. To turn off TZF,
set the minimum delay time for ﬂ anging to a nonzero value (try 0.1 mSec to begin with) in
the cookModType() method. You now have a working modulated delay module that can be
combined in many ways to create a wide variety of modulated delay effects. Let’s try a couple
of them now.
    10.4    Design a Stereo Quadrature Flanger Plug-In
 The block diagram of the stereo quad ﬂ anger is shown in  Figure 10.5  and it consists of
two identical ﬂ anger delay lines running off of LFO values that are in quadrature phase.
Since our mod delay module has a built-in LFO and delay, we can assemble this plug-in
quickly.
  10.4.1  Project: StereoQuadFlanger
 Make sure to add all the existing modules when you create the new StereoQuadFlanger
project:
•
•
•
   ModDelayModule.h.
   DDLModule.h (because ModDelayModule # includes it).
   WTOscillator.h (because DDLModule #includes it).
   10.4.2  StereoQuadFlanger GUI
 We will use a simpler UI consisting of the controls shown in  Table 10.4 .
   10.4.3  StereoQuadFlanger.h File
 In the .h ﬁ le, declare two instances of the ModDelayModule, one for the left and one for the
right. Also add one helper function to initialize and update the modules. These two member
variables and the one function are all that you need.
    // Add your code here: ------------------------------------------------------- //
     CModDelayModule m_ModDelayLeft;
     CModDelayModule m_ModDelayRight;
     void updateModDelays();
    // END OF USER CODE ---------------------------------------------------------- //
 Table 10.4  :    The GUI elements for the StereoQuadFlanger.
Modulated Delay Effects  343
Slider Property
Value
Control Name
Units
Variable Type
Variable Name
Low Limit
High Limit
Initial Value
Depth
%
ﬂ oat
m_fModDepth_pct
0
100
50
Slider Property
Control Name
Units
Variable Type
Variable Name
Low Limit
High Limit
Initial Value
Slider Property
Value
Button Property
Control Name
Units
Variable Type
Variable Name
Low Limit
High Limit
Initial Value
Rate
Hz
ﬂ oat
m_fModFrequency_Hz
0.02
5
0.18
Control Name
Units
Variable Type
Variable Name
Enum String
Value
Feedback
%
ﬂ oat
m_fFeedback_pct
2100
100
50
Value
LFO
enum
m_uLFOType
tri,sin
   10.4.4  StereoQuadFlanger.cpp File
 Add the one helper function updateModDelays(); this is also the function that forces the two
mod lines into quad phase:
  // update all parameters of the ModDelayModules:
  /*
  - setup for stereo quad phase
  - mod depth
  - mod rate
  - feedback
  - mod type = 0 = ﬂ anger
  - LFO type
  - call the update() functions to cook everything
  */
   void CStereoQuadFlanger::updateModDelays()
   {
    // setup quad phase
   m_ModDelayLeft.m_uLFOPhase = 0;   // 0: normal
   m_ModDelayRight.m_uLFOPhase = 1;  // 1: quad phase
   m_ModDelayLeft.m_fModDepth_pct = m_fModDepth_pct;
   m_ModDelayRight.m_fModDepth_pct = m_fModDepth_pct;

344  Chapter 10
   m_ModDelayLeft.m_fModFrequency_Hz = m_fModFrequency_Hz;
   m_ModDelayRight.m_fModFrequency_Hz = m_fModFrequency_Hz;
   m_ModDelayLeft.m_fFeedback_pct = m_fFeedback_pct;
   m_ModDelayRight.m_fFeedback_pct = m_fFeedback_pct;
    // FLANGER!
   m_ModDelayLeft.m_uModType = 0;
   m_ModDelayRight.m_uModType = 0;
   m_ModDelayLeft.m_uLFOType = m_uLFOType;
   m_ModDelayRight.m_uLFOType = m_uLFOType;
    // cook them
   m_ModDelayLeft.updateLFO();
   m_ModDelayLeft.updateDDL();
   m_ModDelayRight.updateLFO();
   m_ModDelayRight.updateDDL();
   }
  prepareForPlay()
•
•
   Forward the call to the member objects.
   updateModDelays().
  bool __stdcall CStereoQuadFlanger::prepareForPlay()
  {
  // Add your code here:
  // call forwarding!
   m_ModDelayLeft.prepareForPlay();
   m_ModDelayRight.prepareForPlay();
  // don’t leave this out – it inits and cooks
   updateModDelays();
  return true;
  }
   userInterfaceChange()
•
   updateModDelays().
  bool __stdcall CStereoQuadFlanger::userInterfaceChange(int nControlIndex)
  {
   }
  // just do a brute force update of all
   updateModDelays();

Modulated Delay Effects  345
   processAudioFrame()
•
   Forward the processAudioFrame() function to the member objects to do the processing.
  bool __stdcall CStereoQuadFlanger:: processAudioFrame(ﬂ oat* pInputBuffer, ﬂ oat*
  pOutputBuffer, UINT uNumInputChannels,
UINT     uNumOutputChannels)
  {
  // Do LEFT (MONO) Channel
   m_ModDelayLeft.processAudioFrame(&pInputBuffer[0], &pOutputBuffer[0], 1, 1);
  // Mono-In, Stereo-Out (AUX Effect)
  if(uNumInputChannels == 1 && uNumOutputChannels == 2)
   pOutputBuffer[1] = pOutputBuffer[0];
  // Stereo-In, Stereo-Out (INSERT Effect)
  if(uNumInputChannels == 2 && uNumOutputChannels == 2)
   m_ModDelayRight.processAudioFrame(&pInputBuffer[1], &pOutputBuffer[1],
1, 1);
  return true;
  }
 Build and test the project. The code is so sparse because we already did most of the work.
The only tricky part about this is making sure you have the objects properly initialized and
cooked and that you forward the calls correctly. If you have errors or crashes, it’s probably
because your objects are not properly initialized.
    10.4.5  Challenges
•
•
•
   Add a control to toggle TZF mode.
   Add a control to let you turn on and off quad phase (it’s currently hard-coded “on”).
   Add more LFO shapes.
    10.5    Design a Multi-Unit LCR Chorus Plug-In
 This LCR or left-center-right chorus (based on the Korg Triton ® ) uses three
chorusing modules run by three independent LFOs, each with its own depth and rate
controls ( Figure 10.13 ). If we play it right, we can code this with a minimum of effort,
but we have to be very careful about book-keeping since we have many variables here.
The UI will also be more complicated, with three sliders per chorus module: depth, rate,
and feedback. The LFO type is ﬁ xed as a triangle for all units. L is in quad phase, R is in
inverse-quad phase, and C is normal.

346  Chapter 10
Left In
0.5
∑
Left  Out
Chorus L
90°
Depth L
Rate L
LFO
∑
Chorus C
0°
LFO
Depth C
Rate C
0.5
Left In
Chorus R
∑
Right Out
–90°
LFO
Depth R
Rate R
 Figure 10.13:    The LCR chorus.
  10.5.1  Project: StereoLCRChorus
 Make sure to add all the existing modules when you create the new project:
•
•
•
   ModDelayModule.h.
   DDLModule.h (because ModDelayModule #includes it).
   WTOscillator.h (because DDLModule #includes it).
   10.5.2  StereoLCRChorus GUI
 This project contains repeated sets of controls. There are depth, rate, and feedback values for
each of the three modules. The sets of controls share the same min and max values. You will
have nine slider controls in all. Use  Table 10.5  to set up the controls.
   10.5.3  StereoLCRChorus.h File
 Declare your member objects in the .h ﬁ le. Also, declare an update() function to modify the
parameters all at once.
  // Add your code here: ------------------------------------------------------- //
  //
  // One module for each LCR

Modulated Delay Effects  347
 Table 10.5:    GUI elements for the LCR chorus.
Depth (%)
Min 5 0
Max 5 100
Initial L 5 50
Initial C 5 50
Initial R 5 50
Variable names:
m_fModDepth_pct_L
m_fModDepth_pct_C
m_fModDepth_pct_R
Rate (Hz)
Min 5 0.02
Max 5 5
Initial L 5 0.18
Initial C 5 0.27
Initial R 5 0.49
Variable names:
m_fModFrequency_Hz_L
m_fModFrequency_Hz_C
m_fModFrequency_Hz_R
Feedback (%)
Min 5 2100
Max 5 1100
Initial L 5 0
Initial C 5 0
Initial R 5 0
Variable names:
m_fFeedback_pct_L
m_fFeedback_pct_C
m_fFeedback_pct_R
   CModDelayModule m_ModDelayLeft;
   CModDelayModule m_ModDelayCenter;
   CModDelayModule m_ModDelayRight;
  // function to transfer out variables to it and cook
   void updateModules(); // you could split this out into smaller functions
  // END OF USER CODE ---------------------------------------------------------- //
   10.5.4  StereoLCRChorus.cpp File
  Constructor
 There is nothing to initialize in the constructor because we have no variables; the member
objects will initialize themselves at construction time. Implement the updateModules() function
next. We need to set up the LFO phases and invert ﬂ ags according to the block diagram. We
also need to put the modules in chorus mode and call the internal update functions:
  // updateModules()
  /*
  - set LFO Phases
  - set LFO inversion (right only)
  - Mod Depths
  - Mod Freqs
  - Feedback
  - Mod Types = chorus (2)
  - LFO Types = triangle (0)
  - call updaters
  */
  void CStereoLCRChorus::updateModules()
  {
  // setup quad phase
348  Chapter 10
   m_ModDelayLeft.m_uLFOPhase = 1;   // 1: quad phase
   m_ModDelayCenter.m_uLFOPhase = 0;  // 0: normal
   m_ModDelayRight.m_uLFOPhase = 1;  // 1: quad phase
m_ModDelayLeft.m_LFO.m_bInvert = false;
   m_ModDelayCenter.m_LFO.m_bInvert = false;
   m_ModDelayRight.m_LFO.m_bInvert = true;  // this one is inverted
   m_ModDelayLeft.m_fModDepth_pct = m_fModDepth_pct_L;
   m_ModDelayCenter.m_fModDepth_pct = m_fModDepth_pct_C;
   m_ModDelayRight.m_fModDepth_pct = m_fModDepth_pct_R;
   m_ModDelayLeft.m_fModFrequency_Hz = m_fModFrequency_Hz_L;
   m_ModDelayCenter.m_fModFrequency_Hz = m_fModFrequency_Hz_C;
   m_ModDelayRight.m_fModFrequency_Hz = m_fModFrequency_Hz_R;
   m_ModDelayLeft.m_fFeedback_pct = m_fFeedback_pct_L;
   m_ModDelayCenter.m_fFeedback_pct = m_fFeedback_pct_C;
   m_ModDelayRight.m_fFeedback_pct = m_fFeedback_pct_R;
    // CHORUS!
   m_ModDelayLeft.m_uModType = 2;
   m_ModDelayCenter.m_uModType = 2;
   m_ModDelayRight.m_uModType = 2;
   m_ModDelayLeft.m_uLFOType = 0; // triangle
   m_ModDelayCenter.m_uLFOType = 0;// triangle
   m_ModDelayRight.m_uLFOType = 0;// triangle
  // cook them
   m_ModDelayLeft.updateLFO();
   m_ModDelayCenter.updateLFO();
   m_ModDelayRight.updateLFO();
   m_ModDelayLeft.updateDDL();
   m_ModDelayCenter.updateDDL();
   m_ModDelayRight.updateDDL();
  }
   prepareForPlay()
•
   Forward the calls to prepareForPlay() to the member objects.
  bool __stdcall CStereoLCRChorus::prepareForPlay()
  {
  // Add your code here:
  // call forwarding!
   m_ModDelayLeft.prepareForPlay();
   m_ModDelayCenter.prepareForPlay();
   m_ModDelayRight.prepareForPlay();
