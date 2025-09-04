# Audio Plugins C++ (Pirkle) — Chunk 29/73

> Source: `Designing Audio Effect Plug-Ins in C++_ With Digital Audio Signal Processing Theory - PDF Room.pdf` · Pages ~1-560 · Extractor: pdfminer.six
> Target ≈ 2800 words, Overlap = 200 words · Actual = 2671

> Note: DSP textbook content for SpectralCanvasPro RT-audio knowledge base

---
Audio Filter Designs: IIR Filters  193
+12.0 dB
+9.0
dB
+6.0
dB
+3.0
dB
0.0
dB
-3.0
dB
-6.0
dB
10 Hz
100 Hz
1 kHz
10 kHz
 Figure 6.31:    The constant-Q peaking ﬁ lter preserves the bandwidth over most
of the range of boost/cut values.  f  c  = 1kHz,  Q  = 0.707.
 The design equations are as follows:
 K 5  tan(p fc /  fs)
V0 5 10boost/cut(dB)/20
K 1 K2
 d0 5 1 1
  e0 5 1 1
1
Q
1
V0Q
K 1 K2
a 5 1 1
V0
Q
K 1 K2
b 5 21 K2 2 12
g 5 1 2
d 5 1 2
V0
Q
K 1 K2
1
Q
K 1 K2
h 5 1 2
1
V0Q
K 1 K2
a0 5
Boost
a
d0
b
d0
g
d0
a1 5
a2 5
b1 5
b2 5
b
d0
d
d0
c0 5 1.0
d0 5 0.0
(6.43)
Cut
a0  5
d0
e0
b
e0
d
e0
b
e0
h
e0
a1 5
a2 5
b1 5
b2 5
c0 5 1.0
d0 5 0.0

194  Chapter 6
+12.0  dB
+8.0
dB
+4.0
dB
0.0
dB
- 4 .0
dB
- 8 .0
dB
-12.0
dB
10 Hz
100 Hz
1 kHz
10 kHz
 Figure 6.32 :    A set of responses for the constant-Q parametric/peaking ﬁ lter with a
variety of boost/cut settings.  f  c  = 1 kHz,  Q  = 2.
   6.7.5  Cascaded Graphic EQ: Non-Constant-Q
 Specify:
(cid:129)
(cid:129)
    f  c , center frequency
   Gain/attenuation in dB
 The graphic EQ is a ﬁ xed  Q  variation on the parametric EQ. In a graphic EQ, the user only
has control over the boost and cut of a set of ﬁ lters tuned to ﬁ xed center frequencies. The  Q
that is used will depend on the boost or cut settings as well as the number of bands of EQ.
For non-constant-Q graphic EQs, the method of ﬁ nding the proper  Q  can be difﬁ cult because
the bandwidth will change for various settings of the boost/cut. The value for  Q  is sometimes
chosen so that when all the controls are set to the maximum boost or cut values, the frequency
response is as ﬂ at as possible.
 The  Q  for constant-Q ﬁ lters is easy to calculate knowing the number of bands split across the
10 octaves of audio. It is a good starting point for developing a non-constant-Q EQ plug-in;
experimentation may be needed to ﬁ nd the ideal  Q  for your particular design. You might also
allow the user to adjust the value within reason. The equation relating  Q  to the number of EQ
bands is  Equation 6.44 .
N 5
Number  of  modules
10
Q 5
"2N
2N 2 1
(6.44)
 For a 10-band graphic EQ,  Q  5 1.414, while for a 31-band (aka 1/3 octave) EQ,  Q  5 4.32.
The center frequencies of the bands are usually found with the following International
Organization for Standardization (ISO)  standard equation:

Audio Filter Designs: IIR Filters  195
q 5
Number  of  bands
10
fn 5 1000*2n@q
n 5 0, 61, 62, 63, etc ...
(6.45)
 For a 10-band EQ, this works out (with rounding) to 32 Hz, 64 Hz, 125 Hz, 250 Hz, 500 Hz,
1 kHz, 2 kHz, 4 kHz, 8 kHz, and 16 kHz.
 The cascaded graphic EQ topology is shown in  Figure 6.33 . It consists of a simple series
cascade of each of the ﬁ lter modules. The non-constant-Q graphic uses the same non-
constant-Q peaking ﬁ lter algorithm, but with the  Q  ﬁ xed according to  Equation 6.42 . You can
get a good feel for how the  Q  affects the overall response by looking at a plot with all controls
set to the maximum boost (or cut). For a 10-band non-constant-Q graphic, we observe the
combined response shown in  Figure 6.34 .
   6.7.6  Cascaded Graphic EQ: Constant-Q
 Specify:
(cid:129)
(cid:129)
    f  c , center frequency
   Gain/attenuation in dB
x(n)
Non-
constant-Q
peaking
filter 1
fc = 64 Hz
Non-
constant-Q
peaking
filter 2
fc = 125 Hz
Non-
constant-Q
peaking
filter 3
fc = 250 Hz
Non-
constant-Q
peaking
filter 10
fc = 16 kHz
y(n)
 Figure 6.33:    The cascaded graphic EQ topology; this is for a 10-band design
using the ISO center frequencies. The design equations for each module use
the non-constant-Q algorithm above.
+36.0  dB
+30.0
dB
+24.0  dB
+18.0  dB
+12.0  dB
+6.0
dB
0.0  dB
10 Hz
100 Hz
1 kHz
10 kHz
 Figure 6.34:    At the prescribed constant-Q value of  Q  = 1.414 we observe rippling and
an increased high-frequency response with all controls at maximum boost.

196  Chapter 6
x(n)
constant-Q
peaking
filter 1
fc = 64 Hz
constant-Q
peaking
filter 2
fc = 125 Hz
constant-Q
peaking
filter 3
f c = 250 Hz
constant-Q
peaking
filter  10
f c = 16 kHz
y(n)
 Figure 6.35:    The cascaded graphic EQ topology; this is for a 10-band design using the
exact unrounded ISO center frequencies. The design equations for each module use the
constant-Q algorithm above.
+30.0 dB
+25.0 dB
+20.0 dB
+15.0 dB
+10.0 dB
+5.0 dB
0.0 dB
10 Hz
100 Hz
1 kHz
10 kHz
 Figure 6.36:    The 10-band constant-Q graphic EQ with the prescribed  Q  = 1.414
produces less low-frequency rippling and a decreased high-frequency response
with all controls at maximum boost.
 The constant-Q graphic EQ follows the same design pattern ( Figure 6.35 ) except that you
use the constant-Q peaking ﬁ lter in each of the modules. The equations for ﬁ nding the center
frequencies and  Q  are the same as above. Bohn (1986) recommends not rounding the ISO
center frequencies but rather use the exact values.  Figure 6.36  shows the composite response
with all controls set to full boost.
    6.8    Design a Resonant LPF Plug-In
 We’ll continue with a resonant LPF design. The ﬁ lter will use the second-order bilinear
transform design and the bi-quad structure for implementation. After that you will redesign
the ﬁ lter using the Massberg technique for analog emulation and examine the differences.
Note: This ﬁ lter is found in RackAFX’s module menu item under “HP/LP Filter” for you to
experiment with and use to check your own implementations.
 For this project, we’ll be using our ﬁ rst built-in object. I’ve already prepared a bi-quad object
for you to use in this (or any other) project. It is not a RackAFX plug-in but rather a simple
C11 object that accomplishes only one task: the bi-quad structure realization. The bi-quad
can be used to make any ﬁ rst- or second-order feed-forward or feed-back type ﬁ lter. Let’s take
a moment to check out the CBiquad object. You can ﬁ nd the interface in the pluginconstants.h
Audio Filter Designs: IIR Filters  197
 Table 6.2:    The CBiquad object interface.
CBiquad
Purpose
Implements the four delay elements needed for the bi-quad:
x(n − 1), x(n − 2), y(n − 1) and y(n − 2) in these protected
variables
The ﬁ lter coefﬁ cients
Initialize all delay elements with 0.0s
Member Variables
protected:
float m_f_Xz_1
float m_f_Xz_2
float m_f_Yz_1
float m_f_Yz_2
public:
float m_f_a0
float m_f_a1
float m_f_a2
float m_f_b1
float m_f_b2
Member Functions
void ﬂ ushDelays()
ﬂ oat doBiQuad(ﬂ oat f_xn)
Do one cycle of ﬁ lter operation; return value is y(n)
Parameters
(cid:129)  ﬂ oat f_xn
Input: the current input sample, x(n)
ﬁ le and the implementation in the pluginobjects.cpp ﬁ les, respectively. The CBiquad object
has the members and variables in  Table 6.2 .
 You can see that this object is simple; it only has two functions, one to reset and the other to
do the bi-quad calculation. The coefﬁ cients must be calculated externally—this object has
no knowledge of the ﬁ lter type that it is implementing, the sample rate, and so on. You will
see that this simple object will also save you a lot of coding for ﬁ lters in later chapters. We
will need to set up the user interface (UI) and then link the variables to the bi-quad objects in
userInterfaceChange().
  6.8.1  Project: ResonantLPF
 Create the project and name it “ResonantLPF,” then add the sliders for the graphical user
interface (GUI).
   6.8.2  ResonantLPF GUI
  Figure 6.37  shows what your ﬁ nal GUI will look like. You will need the controls shown in
 Table 6.3 .
198  Chapter 6
Fc
1000. 0 Hz
Q
0.71
 Figure 6.37:    The resonant LPF GUI.
  Table 6.3:    The GUI controls for the resonant LPF.
Slider Property
Control Name
Units
Variable Type
Variable Name
Low Limit
High Limit
Initial Value
Value
Fc
Hz
ﬂ oat
m_f_fc_Hz
100
5000
1000
Slider Property
Control Name
Units
Variable Type
Variable Name
Low Limit
High Limit
Initial Value
Value
Q
ﬂ oat
m_f_Q
0.5
20
0.707
   6.8.3  ResonantLPF.h File
 RackAFX provides you with the built-in bi-quad object named CBiquad and you don’t
need to do anything at all to add it to your project. You can just declare your bi-quad objects
directly in the .h ﬁ le like you would any other object:
  CBiquad m_LeftLPF; // or whatever you like to name it
 Here’s my .h ﬁ le with the left and right LPF objects declared:
  // Add your code here: ------------------------------------------------------- //
   CBiquad m_LeftLPF;
   CBiquad m_RightLPF;
  // END OF USER CODE ---------------------------------------------------------- //
 We’ll also need a function to calculate the bi-quad’s coefﬁ cients (a 0 , a 1 , a 2 , b 1 , and b 2 ) and we
can share this between the left and right objects (i.e., both LPFs will have the same cutoff and
 Q , so they will also have the same coefﬁ cients). Declare this now:
  // Add your code here: ------------------------------------------------------- //
   CBiquad m_LeftLPF;
   CBiquad m_RightLPF;
   void calculateLPFCoeffs(ﬂ oat fCutoffFreq, ﬂ oat fQ);
  // END OF USER CODE ---------------------------------------------------------- //
Audio Filter Designs: IIR Filters  199
   6.8.4  ResonantLPF.cpp File
 Write the calculateLPFCoeffs() function in the .cpp ﬁ le by using  Equation 6.35 . I have used
the same intermediate coefﬁ cient names here too.
  void CResonantLPF::calculateLPFCoeffs(ﬂ oat fCutoffFreq, ﬂ oat fQ)
  {
  // use same terms as in book:
   ﬂ oat theta_c = 2.0*pi*fCutoffFreq/(ﬂ oat)m_nSampleRate;
   ﬂ oat d = 1.0/fQ;
  // intermediate values
   ﬂ oat fBetaNumerator =  1.0 — ((d/2.0)*(sin(theta_c)));
   ﬂ oat fBetaDenominator = 1.0 + ((d/2.0)*(sin(theta_c)));
  // beta
   ﬂ oat fBeta = 0.5*(fBetaNumerator/fBetaDenominator);
  // gamma
   ﬂ oat fGamma = (0.5 + fBeta)*(cos(theta_c));
  // alpha
   ﬂ oat fAlpha = (0.5 + fBeta — fGamma)/2.0;
  // left channel
   m_LeftLPF.m_f_a0 = fAlpha;
   m_LeftLPF.m_f_a1 = 2.0*fAlpha;
   m_LeftLPF.m_f_a2 = fAlpha;
   m_LeftLPF.m_f_b1 = —2.0*fGamma;
   m_LeftLPF.m_f_b2 = 2.0*fBeta;
  // right channel
   m_RightLPF.m_f_a0 = fAlpha;
   m_RightLPF.m_f_a1 = 2.0*fAlpha;
   m_RightLPF.m_f_a2 = fAlpha;
   m_RightLPF.m_f_b1 = —2.0*fGamma;
   m_RightLPF.m_f_b2 = 2.0*fBeta;
  }
  prepareForPlay()
(cid:129)
(cid:129)
   Flush the bi-quad buffers.
   Cook the raw variables.
  bool __stdcall CResonantLPF::prepareForPlay()
  {
200  Chapter 6
  // Add your code here:
   m_LeftLPF.ﬂ ushDelays();
   m_RightLPF.ﬂ ushDelays();
  // calculate the initial values
   calculateLPFCoeffs(m_f_fc_Hz, m_f_Q);
  return true;
  }
   userInterfaceChange()
(cid:129)
(cid:129)
   If the user moves either slider then we have to recalculate all the coefﬁ cients because they
are interdependent on both  f  c  and  Q .
   There is no need to check the slider nControlIndex value here.
  bool __stdcall CResonantLPF::userInterfaceChange(int nControlIndex)
  {
  // update coeffs for next time
   calculateLPFCoeffs(m_f_fc_Hz, m_f_Q);
  return true;
  }
   processAudioFrame()
(cid:129)
(cid:129)
   Call the methods on the ﬁ lter objects to do the processing.
   They will automatically update their own delay lines and maintain themselves so the code
is simple.
   bool __stdcall CResonantLPF::processAudioFrame(ﬂ oat* pInputBuffer, ﬂ oat* pOutputBuffer,
  UINT uNumInputChannels, UINT uNumOutputChannels)
  {
  // Do LEFT (MONO) Channel; there is always at least one input/one output
  // (INSERT Effect)
   pOutputBuffer[0] = m_LeftLPF.doBiQuad(pInputBuffer[0]);
  // Mono-In, Stereo-Out (AUX Effect)
  if(uNumInputChannels == 1 && uNumOutputChannels == 2)
   pOutputBuffer[1] = pOutputBuffer[0]; // Just copying
  // Stereo-In, Stereo-Out (INSERT Effect)
  if(uNumInputChannels == 2 && uNumOutputChannels == 2)
   pOutputBuffer[1] = m_RightLPF.doBiQuad(pInputBuffer[1]);
  return true;
  }
 Build and test your resonant LPF. Use the analyzer’s frequency and phase response features
to verify that all is working as you would expect. You now have a working resonant LPF, but
Audio Filter Designs: IIR Filters  201
the Nyquist response is always clamped to 0.0, so let’s investigate a technique to make the
response more closely match the original analog ﬁ lter.
     6.9    The Massberg Analog-Matched Low-Pass Filter
 At the 131st Audio Engineering Society Convention in 2011, Michael Massberg presented a
solution to the clamping problem, producing ﬁ rst- and second-order low-pass ﬁ lters that very
closely match their analog counterparts near and at the Nyquist frequency. It takes advantage
of the fact that a shelving ﬁ lter with a very high upper shelf frequency has a roll-off portion
that very closely resembles the roll-off of a low-pass ﬁ lter. In a nutshell, the idea is to design
a shelving ﬁ lter whose upper shelf frequency is above Nyquist. In the range from DC to
Nyquist, it will resemble a low-pass ﬁ lter ( Figure 6.38 ). A unique matching scheme is applied
to force the two responses to match their gains exactly at a point halfway between the gain
at DC and the gain at Nyquist in decibels, with a slight error at points above that frequency.
The result is a ﬁ lter that has an excellent approximation to the analog response it is trying
to mimic.
 Massberg used ﬁ rst- and second-order shelving ﬁ lters to produce analog-matched low-
pass ﬁ lters applying the bilinear transform to produce the ﬁ nal result.  Figure 6.39  shows a
comparison of the standard digital LPF and the Massberg LPF.
  6.9.1  First-Order Massberg LPF
 Specify:
(cid:129)
    f  c , corner frequency
Amplitude
(dB)
0.0
½  Gain  at
Nyquist
Gain  at
Nyquist
Desired  Low-Pass
Response
Shelving  filter
with  high  shelf
frequency  above
Nyquist
Gain  Matching  Point
Error
1/4  Nyquist
½  Nyquist
fs/8
fs/4
Nyquist
fs/2
f
High  shelf
Frequency
 Figure 6.38 :    The Massberg ﬁ lter uses a shelving ﬁ lter as a prototype. In the range from
DC to Nyquist, it very closely resembles the desired low-pass ﬁ lter. The two curves are
displaced slightly.
202  Chapter 6
+30.0
dB
+15.0 dB
0.0 dB
-15.0 dB
-30.0 dB
-45.0 dB
-60.0 dB
Massberg
Unmodified  LPF
10 Hz
100 Hz
1 kHz
10 kHz
 Figure 6.39:    The Massberg and unmodiﬁ ed LPF responses with  f  c  = 5 kHz and  Q  = 10.
The difference in the high-frequency response is evident.
 The design equations are as follows:
   g1 5
2
 ac
2
fs
fc
b
É4 1 a
  gm 5 max 1"0.5, "g1
2
2pfc
"1 2 g2
m
  vm 5
gm
vm
2  fs
b
  Vm 5 tana
2
(6.46)
  Vs 5 Vm
"1 g2
2 1 1 2 g2
m
m 2 g2
1
1 2 g2
m
   g0 5 Vs  1 1
  a0 5 Vs  1 g1
  a1 5 Vs 2 g1
   b 5 Vs  2 1
a0
g0
a1
g0
   a0 5
   a1 5
   a2 5 0
b1
g0
   b2 5 0
   b1 5
