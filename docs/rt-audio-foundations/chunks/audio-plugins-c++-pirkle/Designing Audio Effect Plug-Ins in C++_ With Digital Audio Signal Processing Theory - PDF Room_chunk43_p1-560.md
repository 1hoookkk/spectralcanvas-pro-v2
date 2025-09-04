# Audio Plugins C++ (Pirkle) — Chunk 43/73

> Source: `Designing Audio Effect Plug-Ins in C++_ With Digital Audio Signal Processing Theory - PDF Room.pdf` · Pages ~1-560 · Extractor: pdfminer.six
> Target ≈ 2800 words, Overlap = 200 words · Actual = 2632

> Note: DSP textbook content for SpectralCanvasPro RT-audio knowledge base

---
The C++ code for the Gordon–Smith oscillator looks as follows (two memory elements have
been declared, m_f_yn_z and m_f_yq_z, as well as a coefﬁ cient m_fGorSmithEpsilon). In
cookFrequency():
Oscillators  301
  // calculate HS Epsilon
   ﬂ oat f_wT = (2.0*pi*m_fFrequency_Hz)/(ﬂ oat)m_nSampleRate;
   m_fGorSmithEpsilon = 2.0*sin(f_wT/2.0);
 In processAudioFrame():
    // form yq(n) ﬁ rst
     ﬂ oat f_yqn = m_f_yq_z - m_fGorSmithEpsilon*m_f_yn_z;
    // y(n)
     ﬂ oat f_yn = m_fGorSmithEpsilon*f_yqn + m_f_yn_z;
    // shufﬂ e delays
     m_f_yq_z = f_yqn;
     m_f_yn_z = f_yn;
    // write out
     pOutputBuffer[0] = f_yn;
     etc…
   9.4    Wave Table Oscillators
 A wave table oscillator is, as its name implies, a table-based system for creating periodic
signals. A circular buffer is preloaded with one cycle, minus one sample, of a waveform.
The waveform may be mathematical (sin, cos, pulse, square, saw, triangle, etc.) or it might
be arbitrary or a recorded signal. The basic idea is that you read through the table and output
samples from it. Consider a table of 1024 samples consisting of one cycle of a sinusoid,
minus one sample ( Figure 9.9 ).
 Suppose you start at  i  5 0 and during each sample period, you read out one value and
advance to the next. At the end of the buffer, you wrap around and start all over. If you
did read out one value per sample period, what would be the resulting frequency of the
waveform?
 The answer is  f  table  5  f  s / L  when the index increment is exactly 1.0 through the table. For a
1024-point wave table at a 44,100 Hz sample rate, the table frequency is 43.06 Hz. If you
happen to really need a super precise sinusoid at exactly 43.06 Hz, then this method will
produce nearly perfect results. The only factor is the precision of the sinusoid loaded into the
table. If you had a saw-tooth waveform stored in the table, it too would have a fundamental
frequency of 43.06 Hz.
 Most likely, you are going to want to make any arbitrary frequency you like, perhaps with an
emphasis on musical pitch frequencies. With the exception of the note A, these are going to be

302  Chapter 9
Last sample is the
one just before 0.0
First sample  at 0.0
i = 0
i  =1023
 Figure 9.9:    One cycle minus one sample of a sinusoid. The table is indexed with the
value  i  which starts at 0; the last entry in the table is at  i  5 1023 and it is the
sample just before the waveform starts all over again.
ﬂ oating-point numbers with fractional parts. To make any frequency, you calculate the  inc
value with  Equation 9.7 :
  inc 5 L
fdesired
fs
(9.7)
  L  is the table length and  f  desired  is the target frequency. The increment value you get back will
be used to skip through the table, moving forward by  inc  during each sample interval. If  inc  is
less than 1.0, then the desired frequency is below the table frequency, and if it is above 1.0 it
must be greater than the table frequency. If the  inc  is 2.0, then the resulting frequency is twice
the table frequency. Most likely, the  inc  value is going to be noninteger and will therefore
consist of an integer part and a fractional part. For example, if  inc  5 24.9836 then the integer
part would be 24 and the fractional part 0.9836. Here, the integer part is called  int  and the
fractional part is called  frac .
 As with the delay module you built in  Chapter 8 , there are several options for dealing with the
fractional part of the increment value. You could:
•
•
•
   Truncate the value, and forget  frac .
   Linearly interpolate the table  frac  distance between  int  and  int +  1.
   Use polynomial interpolation or another interpolation method instead of linear
 interpolation.
 If you truncate the  inc  value, then you have multiple problems—the note you synthesize
won’t be exactly in tune. Additionally, it will be distorted because of the inaccuracy in the

Oscillators  303
transcription out of the table. Linear and polynomial interpolation both overcome these
problems, though there is still distortion in the output. The industry standard is a fourth-order
Lagrange interpolation on the wave table, where the neighboring four points (two to the left
and two to the right) of the target interpolated value are used.
 Before we code a wave table oscillator, there is one thing you need to be aware of: the danger
of a wave table is that the cycle of data stored inside might be full of aliasing components.
In other words, you can create a table of data that could have never made it past the input
low-pass ﬁ lter (LPF) if it were an analog signal being sampled. We will be doing just that
and creating some signals that do alias, on purpose. Then, we’ll examine a way to synthesize
common signals without aliasing. The only meaningful signal that won’t alias is a sinusoid,
so we can start there.
   9.5    Design a Wave Table Oscillator Plug-In
 To code a wave table oscillator you need to create a table of data and initialize it in the
constructor. A ﬂ oating-point index value is used for  inc  and it will keep track of the current
read location. Linear interpolation will be used to extract samples from the table. The circular
buffer will run as long as the oscillator is engaged. Thus, we need to start with a plug-in
design similar to the direct form oscillator. We can use the assignable buttons to trigger the
oscillator to simulate MIDI note-on and note-off messages, and we can provide a slider for
frequency control.
  9.5.1  Project: WTOscillator
 Create a new RackAFX project and make sure to check the synthesizer plug-in box (if you
forget, you can always change it later).
   9.5.2  WTOscillator GUI
 The ﬁ nal GUI will feature a frequency control and multiple radio button controls for
waveform, mode, and polarity. Ultimately, it will have the controls shown in  Figure 9.10 .
Frequency
440.0 Hz
Waveform
Sine
Mode
Normal
Polarity
Bipolar
Band-limit
Unipolar
Saw
Tri
Square
 Figure 9.10:     The ﬁ nal WTOscillator GUI.
304  Chapter 9
  Table 9.2:    GUI controls for the wave table oscillator plug-in.
Slider Property
Control Name
Units
Variable Type
Variable Name
Low Limit
High Limit
Initial Value
Value
Frequency
Hz
ﬂ oat
m_fFrequency_Hz
25
4200
440
 First, add a frequency slider to the UI and connect it to a variable named m_fFrequency_
Hz with the limits 25 Hz to 4.2 kHz and an initial setting of 440 Hz ( Table 9.2 ). The
limits are chosen as such because they are close to the lower and upper fundamental
frequencies of the notes on a standard (88 key) piano or synthesizer. Next, set up
the assignable buttons to create start and stop functions just like you did for the
DirectOscillator plug-in.
   9.5.3  WTOscillator.h File
 Declare the variables you need to implement the oscillator:
    // Add your code here: ------------------------------------------------------- //
    //
    // Array for the Table
     ﬂ oat m_SinArray[1024];
// 1024 Point Sinusoid
    // current read location
     ﬂ oat m_fReadIndex;
// NOTE its a FLOAT!
    // reset the read index
     void reset()
     {
           m_fReadIndex = 0.0;
    }
    // our inc value
     ﬂ oat m_f_inc;
    // our cooking function
     void cookFrequency();
    // our note on/off message
     bool m_bNoteOn;
    // END OF USER CODE ---------------------------------------------------------- //
 You can see that we’ve added the necessary ingredients (array, read index,  inc , note-on/off),
as well as two functions:
•
•
   cookFrequency() to update the  inc  value when the frequency changes.
   reset(), which just relocates the read index to the top of the buffer.
Oscillators  305
   9.5.4  WTOscillator.cpp File
  Constructor
•
   Turn off the oscillator by setting the ﬂ ag you declared in the .h ﬁ le.
  CWTOscillator::CWTOscillator()
  {
  <SNIP SNIP SNIP>
  // Finish initializations here
  // setup array
   for(int i = 0; i < 1024; i++)
   {
  // sample the sinusoid, 1024 points
  // sin(wnT) = sin(2pi*i/1024)
   m_SinArray[i] = sin( ((ﬂ oat)i/1024.0)*(2*pi) );
   }
  // clear variables
   m_fReadIndex = 0.0;
   m_f_inc = 0.0;
  // silent
   m_bNoteOn = false;
  // initialize inc
   cookFrequency();
  }
 In the constructor, you set the ﬂ ags and set up the wave table by sampling it for 1024 points.
This version is set up to produce the one-cycle-minus-one-sample waveform we desire. Write
the cooking function:
  void CWTOscillator::cookFrequency()
  {
  // inc = L*fd/fs
   m_f_inc = 1024.0*m_fFrequency_Hz/(ﬂ oat)m_nSampleRate;
  }
   prepareForPlay()
•
•
   Reset the oscillator.
   Cook the variables.
306  Chapter 9
  bool __stdcall CWTOscillator::prepareForPlay()
  {
  // Add your code here:
  // reset the index
   reset();
  // cook curent frequency
   cookFrequency();
  return true;
  }
   userInterfaceChange()
•
•
•
   Handle the slider control to cook the variables.
   Handle the start/stop buttons to turn the ﬂ ag on/off and cook variables if turning on.
   Make sure your nControlIndex variables match with your GUI controls.
  bool __stdcall CWTOscillator::userInterfaceChange(int nControlIndex)
  {
  // add your code here
  switch(nControlIndex)
  {
  case 0:
   cookFrequency();
  break;
  // note on
  case 50:
   reset();
   cookFrequency();
   m_bNoteOn = true;
  break;
  // note off
  case 51:
   m_bNoteOn = false;
  break;
  default:
  break;
  }
  return true;
  }
   processAudioFrame()
•
•
   Implement the table look up.
   Notice that as with the delay line, we shift the frame of reference of the interpolated
points to be between  n  = 0 and  n  = 1, then just use the  frac  component to ﬁ nd the value
between the samples.

  bool __stdcall CWTOscillator::processAudioFrame(ﬂ oat* pInputBuffer, ﬂ oat*
                            pOutputBuffer, UINT uNumChannels, UINT uNumOutputChannels)
  {
Oscillators  307
  // Do LEFT (MONO) Channel
  // if not running, write 0s and bail
   if(!m_bNoteOn)
  {
   pOutputBuffer[0] = 0.0;
  // Mono-In, Stereo-Out (AUX Effect)
  if(uNumInputChannels == 1 && uNumOutputChannels == 2)
   pOutputBuffer[1] = 0.0;
  // Stereo-In, Stereo-Out (INSERT Effect)
  if(uNumInputChannels == 2 && uNumOutputChannels == 2)
   pOutputBuffer[1] = 0.0;
   return true;
  }
  // our output value for this cycle
   ﬂ oat fOutSample = 0;
  // get INT part
   int nReadIndex = (int)m_fReadIndex;
  // get FRAC part
   ﬂ oat fFrac = m_fReadIndex – nReadIndex;
  // setup second index for interpolation; wrap the buffer if needed
   int nReadIndexNext = nReadIndex + 1 > 1023 ? 0 :  nReadIndex + 1;
  // interpolate tht output (x1,x2,y1,y2,frac) – notice the way we set x1 and x2
  // to 0 and 1, then only use the frac value while nReadIndex and nReadIndexNext
  // acquire the values from the table
   fOutSample = dLinTerp(0, 1, m_SinArray[nReadIndex], m_SinArray[nReadIndexNext],
                                       fFrac);
  // add the increment for next time
   m_fReadIndex += m_f_inc;
  // check for wrap
   if(m_fReadIndex > 1024)
   m_fReadIndex = m_fReadIndex – 1024;
  // write out
   pOutputBuffer[0] = fOutSample;
  // Mono-In, Stereo-Out (AUX Effect)
  if(uNumInputChannels == 1 && uNumOutputChannels == 2)
   pOutputBuffer[1] = fOutSample;
  // Stereo-In, Stereo-Out (INSERT Effect)

308  Chapter 9
  if(uNumInputChannels == 2 && uNumOutputChannels == 2)
   pOutputBuffer[1] = fOutSample;
  return true;
  }
 Build and run the oscillator and check the pitch using the analyzer window. Adjust the
oscillation frequency with the slider. Add a volume control slider if you wish.
     9.6    Adding More Wave Tables
 The sinusoidal oscillator is interesting, but you can implement any kind of table
you want. You will see that adding more tables is actually easy since they all use
the same frequency of oscillation equation. In this section we will add the following
table types:
•
•
•
   Saw-tooth
   Triangle
   Square
 These three waveforms will be mathematically as close as you can get in a discrete
time system, but they will alias when played back. However, we still want to
implement them because we can use the wave table oscillator as an LFO for
upcoming effects like chorus and flanger. To add more tables we need to do the
following:
•
•
•
   Add more arrays to store the tables.
   Initialize the arrays in the constructor.
   Provide the user with a way to switch oscillator types.
  9.6.1  WTOscillator.h File
 Start with the new arrays in the .h ﬁ le:
    // Add your code here: -----------------------------------------------------------
    //
    // Array for the Table
    ﬂ oat m_SinArray[1024];                // 1024 Point Sinusoid
     ﬂ oat m_SawtoothArray[1024];          // saw
     ﬂ oat m_TriangleArray[1024];          // tri
     ﬂ oat m_SquareArray[1024];              // sqr
    // current read location
    ﬂ oat m_fReadIndex;                        // NOTE its a FLOAT!
Oscillators  309
   9.6.2  WTOscillator.cpp File
  Constructor
•
   These three new wave tables will be bipolar like the sinusoid table and swing between
–1.0 and +1.0, and they will all start with the ﬁ rst sample 5 0.0.
  CWTOscillator::CWTOscillator()
  {
  m_PlugInName = "WTOscillator";
  // set our oscillator ﬂ ag:
  this->m_bOutputOnlyPlugIn = true;
  // slope and y-intercept values for the
  // Triangle Wave
  // rising edge1:
   ﬂ oat mt1 = 1.0/256.0;
   ﬂ oat bt1 = 0.0;
  // rising edge2:
   ﬂ oat mt2 = 1.0/256.0;
   ﬂ oat bt2 = –1.0;
  // falling edge:
   ﬂ oat mtf2 = –2.0/512.0;
   ﬂ oat btf2 = +1.0;
  // Sawtooth
  // rising edge1:
   ﬂ oat ms1 = 1.0/512.0;
   ﬂ oat bs1 = 0.0;
  // rising edge2:
   ﬂ oat ms2 = 1.0/512.0;
   ﬂ oat bs2 = –1.0;
  // setup arrays
  for(int i = 0; i < 1024; i++)
  {
  // sample the sinusoid, 1024 points
  // sin(wnT) = sin(2pi*i/1024)
  m_SinArray[i] = sin( ((ﬂ oat)i/1024.0)*(2*pi) );
  // saw, triangle and square are just logic mechanisms
  // can you ﬁ gure them out?
  // Sawtooth
   m_SawtoothArray[i] =  i < 512 ? ms1*i + bs1 : ms2*(i-511) + bs2;

310  Chapter 9
  // Triangle
   if(i < 256)
   m_TriangleArray[i] = mt1*i + bt1; // mx + b; rising edge 1
   else if (i >= 256 && i < 768)
   m_TriangleArray[i] = mtf2*(i–256) + btf2; // mx + b; falling edge
   else
   m_TriangleArray[i] = mt2*(i–768) + bt2; // mx + b; rising edge 2
  // square:
   m_SquareArray[i] = i < 512 ? +1.0 : –1.0;
  }
  etc…
    9.6.3  WTOscillator GUI
 In order to let the user adjust the table, you can use the enumerated unsigned integer type
(UINT) variable in the slider or the radio buttons. I will use the radio buttons and set them
up here—right-click in the area around the buttons you want to label and ﬁ ll in the properties
with  Table 9.3 . The enumerated UINT variable lets you make the switch between oscillators.
You set up the UINT variable name and the string of labels for the radio buttons here.
  Table 9.3:    The button properties for the oscillator type control.
Slider Property
Control Name
Units
Variable Type
Variable Name
Enum String
Value
Osc Type
enum
m_uOscType
sine,saw,tri,square
   Note : Here, we use a radio button list with an enumerated string.
   9.6.4  WTOscillator.h File
 In the .h ﬁ le, you can see where RackAFX added the variable:
    // ADDED BY RACKAFX -- DO NOT EDIT THIS CODE!!! -------------------------------//
    //  **--0x07FD--**
    ﬂ oat m_fFrequency_Hz;
     UINT m_uOscType;
     enum{sine,saw,tri,square};
    // **--0x1A7F--**
    // --------------------------------------------------------------------------- //

 The m_uOscType is the switch variable and the enumerated list {sin, saw, tri, square}
represents the integer set {0, 1, 2, 3}, which maintains the state of the switch.
Oscillators  311
   9.6.5  WTOscillator.cpp File
  processAudioFrame()
•
   A switch/case statement is used to choose the table to interpolate . Every other facet of the
operation is the same as before, only the table is different.
  bool __stdcall CDirectOscillator::processAudioFrame(ﬂ oat* pInputBuffer, ﬂ oat*
                            pOutputBuffer, UINT uNumChannels, UINT uNumOutputChannels)
  {
          // Do LEFT (MONO) Channel
          <SNIP SNIP SNIP>
          // setup second index for interpolation; wrap the buffer if needed
          int nReadIndexNext = nReadIndex + 1 > 1023 ? 0 :  nReadIndex + 1;
          // interpolate the output
           switch(m_uOscType)
          {
                    case sine:
                             fOutSample = dLinTerp(0, 1, m_SinArray[nReadIndex],
                                                      m_SinArray[nReadIndexNext], fFrac);
                             break;
                    case saw:
                             fOutSample = dLinTerp(0, 1, m_SawtoothArray[nReadIndex],
                                                      m_SawtoothArray[nReadIndexNext], fFrac);
                             break;
                    case tri:
                             fOutSample = dLinTerp(0, 1, m_TriangleArray[nReadIndex],
                                                      m_TriangleArray[nReadIndexNext], fFrac);
                             break;
                    case square:
                             fOutSample = dLinTerp(0, 1, m_SquareArray[nReadIndex],
                                                      m_SquareArray[nReadIndexNext], fFrac);
                             break;
                    // always need a default
                    default:
                             fOutSample = dLinTerp(0, 1, m_SinArray[nReadIndex],
                                                      m_SinArray [nReadIndexNext], fFrac);
                             break;
          }
          // add the increment for next time
          m_fReadIndex += m_f_inc;
          <SNIP SNIP SNIP>
          return true;
  }
 Build and test the oscillator. Use the analyzer window to view the waveforms in time and
frequency. Notice the large amount of upper harmonics in the non-sinusoidal waveforms—
