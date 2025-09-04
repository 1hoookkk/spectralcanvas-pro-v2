# Audio Plugins C++ (Pirkle) — Chunk 45/73

> Source: `Designing Audio Effect Plug-Ins in C++_ With Digital Audio Signal Processing Theory - PDF Room.pdf` · Pages ~1-560 · Extractor: pdfminer.six
> Target ≈ 2800 words, Overlap = 200 words · Actual = 715

> Note: DSP textbook content for SpectralCanvasPro RT-audio knowledge base

---
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

Oscillators  323
  case saw:
  if(m_uTableMode == normal)       // normal
  {
  }
  else
  {
  fOutSample = dLinTerp(0, 1, m_sawtoothArray[nReadIndex],
  m_SawtoothArray[nReadIndexNext], fFrac);
   fQuadPhaseOutSample = dLinTerp(0, 1,
   m_SawtoothArray[nQuadPhaseReadIndex],
   m_SawtoothArray[nQuadPhaseReadIndexNext], fFrac);
// bandlimited
  fOutSample = dLinTerp(0, 1,
  m_SawtoothArray_BL5[nReadIndex],
  m_SawtoothArray_BL5[nReadIndexNext], fFrac);
   fQuadPhaseOutSample = dLinTerp(0, 1,
   m_SawtoothArray_BL5[nQuadPhaseReadIndex],
   m_SawtoothArray_BL5[nQuadPhaseReadIndexNext],
fFrac);
  }
  break;
  case tri:
  // ETC… FOLLOW THE SAME LOGIC AS THE OTHER TABLES //
  case SQUARE:
  // ETC… FOLLOW THE SAME LOGIC AS THE OTHER TABLES //
  // always need a default
  default:
  fOutSample = dLinTerp(0, 1, m_SinArray[nReadIndex],
  m_SinArray [nReadIndexNext], fFrac);
   fQuadPhaseOutSample = dLinTerp(0, 1,
   m_SinArray[nQuadPhaseReadIndex], m_SinArray
[nQuadPhaseReadIndexNext], fFrac);
  }
  break;
  // add the increment for next time
  m_fReadIndex += m_f_inc;
   m_fQuadPhaseReadIndex += m_f_inc;
  // check for wrap
  if(m_fReadIndex > 1024)
  m_fReadIndex = m_fReadIndex – 1024;
   if(m_fQuadPhaseReadIndex > 1024)
   m_fQuadPhaseReadIndex = m_fQuadPhaseReadIndex – 1024;
  // invert?
  if(m_bInvert)
  {
  fOutSample *= –1.0;
  fQuadPhaseOutSample *= –1.0;
  }

324  Chapter 9
  // write out
   *pYn = fOutSample;
   *pYqn = fQuadPhaseOutSample;
  }
  processAudioFrame()
•
   Use the doOscillate() function.
  bool __stdcall CWTOscillator::processAudioFrame(ﬂ oat* pInputBuffer, ﬂ oat*
pOutputBuffer, UINT uNumInputChannels, UINT uNumOutputChannels)
  {
  // Do LEFT (MONO) Channel
  // if not running, write 0s and bail
  if(!m_bNoteOn)
  {
  pOutputBuffer[0] = 0.0;
  if(uNumOutputChannels == 2)
  pOutputBuffer[1] = 0.0;
  return true;
  }
  // some intermediate variables for return
   ﬂ oat fY = 0;
   ﬂ oat fYq = 0;
// call the oscilator function, return values into fY and fYq
   doOscillate(&fY, &fYq);
  // write fY to the Left
   pOutputBuffer[0] = fY;
  // write fYq to the Right
  if(uNumOutputChannels == 2)
  {
   pOutputBuffer[1] = fYq;
  }
  return true;
  }
 Build and test the code.     Figure 9.16  clearly shows the quad phase output: the sin() in the left
channel, cos() is in the right channel.
     9.9    Bipolar/Unipolar Functionality
  9.9.1  WTOscillator GUI
 Add the bipolar/unipolar switch to the UI using the next bank of radio buttons with the
properties in  Table 9.5 . Edit it like you did with the previous buttons and give it the
enumerations. I named my variable “m_uPolarity” and used “bipolar” and “unipolar” as my
string/enums. The default will be bipolar.

Oscillators  325
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
1 .000
0.707
0.500
0.000
–0.500
–0.707
–1.000
1.000
0.707
0.500
0.000
–0.500
–0.707
–1.000
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
 Figure 9.16:    The quadrature-phase outputs of the left and right signals. Note: You must put the
scope in free running mode to see the phase differences between channels.
  Table 9.5:    The button properties for the polarity control.
Slider Property
Control Name
Units
Variable Type
Variable Name
Enum String
Value
Polarity
enum
m_uPolarity
bipolar,unipolar
   9.9.2  WTOscillator.cpp File
 Add the bipolar/unipolar functionality to the very last part of doOscillate() to divide by 2 then
shift by 0.5 as follows:
  void CWTOscillator::doOscillate(ﬂ oat* pYn, ﬂ oat* pYqn)
  {
  <SNIP SNIP SNIP>
  // write out
  *pYn = fOutSample;
  *pYqn = fQuadPhaseOutSample;
  // create unipolar; div 2 then shift up 0.5
   if(m_uPolarity == unipolar)
