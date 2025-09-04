# Audio Plugins C++ (Pirkle) — Chunk 44/73

> Source: `Designing Audio Effect Plug-Ins in C++_ With Digital Audio Signal Processing Theory - PDF Room.pdf` · Pages ~1-560 · Extractor: pdfminer.six
> Target ≈ 2800 words, Overlap = 200 words · Actual = 2740

> Note: DSP textbook content for SpectralCanvasPro RT-audio knowledge base

---
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

312  Chapter 9
some of those harmonics are actually aliased backwards. We’ll need a way to get around the
aliasing if we want to synthesize these waveforms for listening; however, we can still use this
oscillator as a very effective LFO if we want.
     9.7    Band-Limited Additive Wave Tables
 If you looked at the analyzer’s fast Fourier transform  (FFT) window you saw an abundance of
upper harmonics in the saw-tooth, triangle, and square waves. That is because mathematically,
these waveforms contain harmonics out to inﬁ nite frequency.  Equations 9.8  through  9.10
show the Fourier series equations for these waveforms.
`
y(n)SAW 5 a
k51
(21)k111
k
sin(kvnT)
5 csin 1 vnT 2 2
sin 1 2vnT 2 1
1
2
sin 1 3vnT 2 2
1
3
1
4
sin 1 4vnT 2 1 ...d
 The saw-tooth waveform has both even and odd harmonics scaled according to (1/ k ):
  y1 n2
`
TRI 5 a
k50
1 212 k
1
1 2k 1 12 2
 sin 1 1 2k 1 12 vnT 2
1
9
sin(3vnT) 1
          5    csin(vnT) 2
1
25
 The triangle waveform has only odd harmonics. The (–1)  k   term alternates the signs of
the harmonics. The harmonic amplitudes drop off at a rate given by 1/(2 k  + 1) 2  which is
exponential in nature.
sin(7vnT) 1 cd
sin(5vnT) 2
1
49
(9.8)
   (9.9)
`
y(n)SQUARE 5 a
k50
1
2k 1 1
sin((2k 1 1)vnT)
5 csin(vnT) 1
1
3
sin(3vnT) 1
1
5
sin(5vnT) 1
1
7
sin(7vnT) 1 cd
(9.10)
 The square wave is also composed of odd harmonics like the triangle wave. The harmonic
amplitudes drop off at a rate of 1/(2 k  + 1), which is not as severe as the triangle wave.
Therefore, the square wave has higher-sounding harmonics and a more gritty texture.
 Using these Fourier series equations, you can implement Fourier synthesis (or additive
synthesis) to build up a waveform that is band-limited to Nyquist. Let’s modify the existing
wave table oscillator to optionally use band-limited tables instead. The ﬁ rst version will be
somewhat primitive, but will be a beginning source that you can use to make much more
advanced synthesizer oscillators. This version will also span the frequencies of the piano.
 The highest note on a piano is C8, vibrating at about 4186 Hz. Five times this value is
20,930 Hz, which is just barely below Nyquist. This means that we can synthesize our tables

Oscillators  313
with the fundamental plus the next ﬁ ve harmonics (also called partials) of the series given
in  Equations 9.8  through  9.10  and create our band-limited tables. The tables will therefore
potentially contain the fundamental, 2nd, 3rd, 4th, 5th, and 6th harmonics. In order to do this,
you need to modify the plug-in to allow for another mode: normal or band-limited. You can
do this with another enumerated UINT data type, using either a slider or radio-button bank.
I will use another radio button bank in this example.
  9.7.1  WTOscillator GUI
 Start with the UI and add another enumerated UINT by right-clicking inside the second bank
of radio buttons and ﬁ lling out the properties in  Table 9.4 .
  Table 9.4:    The button properties for the mode switches between “normal”
and “bandlimited” on the UI.
Button Property
Control Name
Units
Variable Type
Variable Name
Enum String
Value
Mode
enum
m_uTableMode
normal,bandlimit
   9.7.2  WTOscillator.h File
 This new information appears in the .h ﬁ le:
    // ADDED BY RACKAFX -- DO NOT EDIT THIS CODE!!! ------------------------------- //
    //  **--0x07FD--**
    ﬂ oat m_fFrequency_Hz;
    UINT m_uOscType;
    enum{sine,saw,tri,square};
     UINT m_uTableMode;
     enum{normal,bandlimit};
    // **--0x1A7F--**
    // --------------------------------------------------------------------------- //
 As long as we are in the .h ﬁ le, we need to add more arrays for our band-limited tables. We
want to keep these separate from the other tables to provide both modes of operation.
    // Add your code here: ------------------------------------------------------- //
    //
    // Array for the Table
314  Chapter 9
    ﬂ oat m_SinArray[1024];                      // 1024 Point Sinusoid
    ﬂ oat m_SawtoothArray[1024];                   // saw
    ﬂ oat m_TriangleArray[1024];                 // tri
    ﬂ oat m_SquareArray[1024];                     // sqr
    // band limited to 5 partials
     ﬂ oat m_SawtoothArray_BL5[1024];                // saw, BL = 5
     ﬂ oat m_TriangleArray_BL5[1024];                // tri, BL = 5
     ﬂ oat m_SquareArray_BL5[1024];                   // sqr, BL = 5
    // current read location
    ﬂ oat m_fReadIndex;                           // NOTE its a FLOAT!
   9.7.3  WTOscillator.cpp File
  Constructor
•
   Initialize the tables according to the Fourier series equations. One of the problems with
the additive method is that the ﬁ nal tables may be slightly outside the range of (21.0 to
11.0), so we need to make normalized tables by saving the maximum value, then divid-
ing each table by its maximum.
 Here is the code for the ﬁ ve-partial tables:
  CWTOscillator::CWTOscillator()
  {
          <SNIP SNIP SNIP>
          // rising edge2:
          ﬂ oat ms2 = 1.0/512.0;
          ﬂ oat bs2 = –1.0;
          // setup arrays
          // to keep track of max-es for normalization later
          ﬂ oat fMaxTri = 0;
          ﬂ oat fMaxSaw = 0;
          ﬂ oat fMaxSqr = 0;
          for(int i = 0; i < 1024; i++)
          {
                   // sample the sinusoid, 1024 points
                   // sin(wnT) = sin(2pi*i/1024)
                   m_SinArray[i] = sin( ((ﬂ oat)i/1024.0)*(2*pi) );
                   <SNIP SNIP SNIP>
                   // square: 1st sample is zero!
                   if(i==1)
                            m_SquareArray[i] = 0.0;

Oscillators  315
                   else
                            m_SquareArray[i] = i < 512 ? +1.0 : –1.0;
                   // zero to start, then loops build the rest
                    m_SawtoothArray_BL5[i] = 0.0;
                    m_SquareArray_BL5[i] = 0.0;
                    m_TriangleArray_BL5[i] = 0.0;
                   // sawtooth: += (–1)^g+1(1/g)sin(wnT)
                    for(int g=1; g<=6; g++)
                    {
                             double n = double(g);
                             m_SawtoothArray_BL5[i] += pow((ﬂ oat)–1.0,(ﬂ oat)(g+1))*
                                                                (1.0/n)*sin(2.0*pi*i*n/1024.0);
                    }
                   // triangle: += (–1)^g(1/(2g+1+^2)sin(w(2n+1)T)
                   // NOTE: the limit is 3 here because of the way the sum is constructed
                   // (look at the (2n+1) components
                    for(int g=0; g=3; g++)
                    {
                             double n = double(g);
                             m_TriangleArray_BL5[i] += pow((ﬂ oat)–1.0, (ﬂ oat)n)*
                                                                (1.0/pow((ﬂ oat)(2*n + 1),
                                                              (ﬂ oat)2.0))*
                                                                sin(2.0*pi*(2.0*n + 1)*i/1024.0);
                    }
                   // square: += (1/g)sin(wnT)
                    for(int g=1; g<=5; g+=2)
                    {
                             double n = double(g);
                             m_SquareArray_BL5[i] += (1.0/n)*sin(2.0*pi*i*n/1024.0);
                    }
                   // store the max values
                    if(i == 0)
                    {
                             fMaxSaw = m_SawtoothArray_BL5[i];
                             fMaxTri = m_TriangleArray_BL5[i];
                             fMaxSqr = m_SquareArray_BL5[i];
                    }
                    else
                    {
                            // test and store
                             if(m_SawtoothArray_BL5[i] > fMaxSaw)
                                     fMaxSaw = m_SawtoothArray_BL5[i];
                             if(m_TriangleArray_BL5[i] > fMaxTri)
                                     fMaxTri = m_TriangleArray_BL5[i];

316  Chapter 9
                             if(m_SquareArray_BL5[i] > fMaxSqr)
                                     fMaxSqr = m_SquareArray_BL5[i];
                    }
           }
          // normalize the bandlimited tables
           for(int i = 0; i < 1024; i++)
           {
                    // normalize it
                    m_SawtoothArray_BL5[i] /= fMaxSaw;
                    m_TriangleArray_BL5[i] /= fMaxTri;
                    m_SquareArray_BL5[i] /= fMaxSqr;
           }
          etc…
  }
 Because each sample in the table is built additively, you ﬁ rst zero out the value, then enter the
loops to accumulate the additive signal in the sample slot. To increase the number of partials,
you just increase the limit in the for() loops above.
   processAudioFrame()
•
   A switch/case statement is used to change between normal and band-limited versions.
  bool __stdcall CWTOscillator::processAudioFrame(ﬂ oat* pInputBuffer, ﬂ oat*
                                                        pOutputBuffer, UINT uNumInputChannels,
                                                        UINT uNumOutputChannels)
  {
  // SNIP SNIP SNIP>
  // setup second index for interpolation; wrap the buffer if needed
  int nReadIndexNext = nReadIndex + 1 > 1023 ? 0 :  nReadIndex + 1;
  // interpolate the output
   switch(m_uOscType)
   {
   case sine:
                                                        m_SinArray[nReadIndexNext], fFrac);
   fOutSample = dLinTerp(0, 1, m_SinArray[nReadIndex],
   break;
   case saw:
   if(m_uTableMode == normal)      // normal
                                   fOutSample = dLinTerp(0, 1, m_SawtoothArray[nReadIndex],
                                                              m_SawtoothArray[nReadIndexNext],
                                                              fFrac);
   else                             // bandlimited
                                   fOutSample = dLinTerp(0, 1,
                                                      m_SawtoothArray_BL5[nReadIndex],
                                                      m_SawtoothArray_BL5[nReadIndexNext],

Oscillators  317
                                                      fFrac);
   break;
   case tri:
   if(m_uTableMode == normal)   // normal
                                        fOutSample = dLinTerp(0, 1, m_TriangleArray[nReadIndex],
                                                               m_TriangleArray[nReadIndexNext],
                                                               fFrac);
   else                               // bandlimited
       fOutSample = dLinTerp(0, 1,
                                                                     m_TriangleArray_BL5[nReadIndex],
                                                                  m_TriangleArray_BL5[nReadIndexNext],
                                                                fFrac);
   break;
   case square:
   if(m_uTableMode == normal)    // normal
                                     fOutSample = dLinTerp(0, 1, m_SquareArray[nReadIndex],
                                                                 m_SquareArray[nReadIndexNext],
                                                                 fFrac);
   else                                // bandlimited
         fOutSample = dLinTerp(0, 1, m_SquareArray_BL5[nReadIndex],
                                                                 m_SquareArray_BL5[nReadIndexNext],
                                                              fFrac);
   break;
   // always need a default
   default:
                                                        m_SawtoothArray[nReadIndexNext], fFrac);
   fOutSample = dLinTerp(0, 1, m_SinArray[nReadIndex],
   break;
           }
          // add the increment for next time
          m_fReadIndex += m_f_inc;
          etc…
  }
 Build and test the oscillator. Switch between the different modes and listen to the results, then
use the analyzer to compare the oscillators in frequency and time. Next are some screenshots
showing the differences for the saw-tooth and square waves.
    9.7.4  Saw-Tooth
  Figure 9.11  shows a normal saw-tooth waveform.  Figure 9.12  shows a band-limited saw-tooth
waveform.
   9.7.5  Square Wave
  Figure 9.13  shows a normal square wave.  Figure 9.14  shows a band-limited square wave.
318  Chapter 9
1.000
0.707
0.500
0.000
-0.500
-0.707
-1.000
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
+12.0 dB
0.0 dB
-12.0 dB
-24.0 dB
-36.0 dB
-48.0 dB
-60.0 dB
10  Hz
100 Hz
1 kHz
10 kHz
 Figure 9.11:     The mathematically perfect saw-tooth waveform with  f  5 1 kHz in the time domain
(top) and frequency domain (bottom), which shows aliasing in the frequencies between
10 kHz and Nyquist.
1.000
0.707
0.500
0.000
-0.500
-0.707
-1.000
+12.0 dB
0.0 dB
-12.0 dB
-24.0 dB
-36.0 dB
-48.0 dB
-60.0 dB
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
10 Hz
100 Hz
1k Hz
10k Hz
 Figure 9.12:     The ﬁ ve-harmonic band-limited saw-tooth waveform with  f  5 1 kHz in the time
domain (top) and frequency domain (bottom). The aliasing is gone for this 1 kHz signal; it would
reappear when the frequency was raised to the point that the highest harmonic went outside the
Nyquist boundary. The fundamental plus the ﬁ ve harmonic peaks are clearly visible.
Oscillators  319
1.000
0.707
0.500
0.000
–0.500
–0.707
–1.000
+12.0 dB
0. 0 dB
–12. 0 dB
–24.0 dB
–36. 0 dB
–48. 0 dB
–60. 0 dB
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
10 H z
100 Hz
1  kHz
10 kHz
 Figure 9.13:     The mathematically perfect square wave with  f  5 1 kHz in the time
domain (top) and frequency domain (bottom), which shows aliasing in the
frequencies between 10 kHz and Nyquist. The square wave
contains only the odd harmonics.
1.000
0.707
0.500
0.000
–0.500
–0.707
–1.000
+12.0dB
0.0 dB
–12.0 dB
–24.0 dB
–36.0 dB
–48.0 dB
–60.0 dB
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
10 Hz
100 Hz
1 kHz
10 kHz
 Figure 9.14:     The ﬁ ve-harmonic band-limited square wave with  f  5 1 kHz
in the time domain (top) and frequency domain (bottom). The aliasing
is gone for this 1 kHz signal. There are only two harmonics (the third and ﬁ fth)
because the next harmonic would be outside our limit.
320  Chapter 9
    9.8    Additional Oscillator Features (LFO)
 For the modulated delay lines in the next chapters, we will need to use LFOs with a couple of
additional properties. Speciﬁ cally, we need
•
•
•
•
   A quadrature phase output
   Unipolar or bipolar operation
   Option to invert the output by 180 degrees
   A doOscillate() function that returns both outputs
 All of these are easy to add to the current project. To make a quadrature phase output, we
only need to declare a second read index which samples the table along with the current one.
The quad phase read index will be initialized one-quarter of the way through the table to set
up the quad phase output ( Figure 9.15 ).
  9.8.1  WTOscillator.h File
•
•
   Add a new variable for the inverted output option.
   Add a new variable for the quad phase read index; initialize it in reset().
  // Add your code here: -----------------------------------------------------------
  <SNIP SNIP SNIP>
  // current read location
  ﬂ oat m_fReadIndex;                        // NOTE its a FLOAT!
   ﬂ oat m_fQuadPhaseReadIndex;             // NOTE its a FLOAT!
y (n )
yq(n)
 Figure 9.15:    The concept of two read index values separated by one-quarter of the buffer;
as long as they advance together by the same distance they will stay in quad phase. We currently
have a bipolar oscillator which can easily be converted to a unipolar output by dividing by 2, then
shifting the whole waveform up by 0.5.

Oscillators  321
  // invert output
   bool m_bInvert;
  // reset the read index
  void reset()
  {
  m_fReadIndex = 0.0;                    // top of buffer
   m_fQuadPhaseReadIndex = 256.0;        // 1/4 of our 1024 point buffer
   }
   9.8.2  WTOscillator.cpp File
  Constructor
•
   Initialize the new index variable in the constructor; it will be reset to the proper value
later.
  CWTOscillator::CWTOscillator()
  {
  <SNIP SNIP SNIP>
  // clear variables
  m_fReadIndex = 0.0;
   m_fQuadPhaseReadIndex = 0.0;
  m_f_inc = 0.0;
  // silent
  m_bNoteOn = false;
  // norm phase
   m_bInvert = false;
  etc…
  }
    9.8.3  WTOscillator.h File
 Add the doOscillate() function. It needs to be called externally in future projects. It
also needs to provide both quad phase outputs. It will be a mono function. It requires
no input argument and we can test it from within the plug-in by sending the two phases
out to the left and right channels, then view it with the analyzer. In the .h ﬁ le, add the
following:
  // Add your code here: ----------------------------------------------------------//
  //
  // funciton to do the Oscillator; may be called by an external parent Plug-In too
  //  pYn is the normal output
  //  pYqn is the quad phase output
   void doOscillate(ﬂ oat* pYn, ﬂ oat* pYqn);
  // Array for the Table

322  Chapter 9
  ﬂ oat m_SinArray[1024];                        // 1024 Point Sinusoid
  ﬂ oat m_SawtoothArray[1024];                  // saw
  ﬂ oat m_TriangleArray[1024];                  // tri
  ﬂ oat m_SquareArray[1024];                      // sqr
  Etc…
   9.8.4  WTOscillator.cpp File
 Add the new function doOscillate() to the .cpp ﬁ le. Cut and paste the contents of the
processAudioFrame() for the left channel only. Just about everything is duplicated for the
quad phase output. Of course, you can go in and streamline this code for more efﬁ ciency, but
you can clearly see the operation of the two outputs here. Add the stuff in bold to the code
you copied from processAudioFrame():
  void CWTOscillator::doOscillate(ﬂ oat* pYn, ﬂ oat* pYqn)
  {
  // IMPORTANT: for future modules, bail out if no note-on event!
   if(!m_bNoteOn)
   {
   *pYn = 0.0;
   *pYqn = 0.0;
   return;
   }
  // our output value for this cycle
  ﬂ oat fOutSample = 0;
   ﬂ oat fQuadPhaseOutSample = 0;
  // get INT part
  int nReadIndex = (int)m_fReadIndex;
   int nQuadPhaseReadIndex = (int)m_fQuadPhaseReadIndex;
  // get FRAC part – NOTE no Quad Phase frac is needed because it will be the same!
  ﬂ oat fFrac = m_fReadIndex – nReadIndex;
  // setup second index for interpolation; wrap the buffer if needed
  int nReadIndexNext = nReadIndex + 1 > 1023 ? 0 :  nReadIndex + 1;
   int nQuadPhaseReadIndexNext = nQuadPhaseReadIndex + 1 > 1023 ? 0 :
   nQuadPhaseReadIndex + 1;
  // interpolate the output
  switch(m_uOscType)
  {
  case sine:
  fOutSample = dLinTerp(0, 1, m_SinArray[nReadIndex],
  m_SinArray[nReadIndexNext], fFrac);
   fQuadPhaseOutSample = dLinTerp(0, 1,
   m_SinArray[nQuadPhaseReadIndex],
   m_SinArray[nQuadPhaseReadIndexNext], fFrac);
  break;
