# Audio Plugins C++ (Pirkle) — Chunk 55/73

> Source: `Designing Audio Effect Plug-Ins in C++_ With Digital Audio Signal Processing Theory - PDF Room.pdf` · Pages ~1-560 · Extractor: pdfminer.six
> Target ≈ 2800 words, Overlap = 200 words · Actual = 2723

> Note: DSP textbook content for SpectralCanvasPro RT-audio knowledge base

---
388  Chapter 11
of the static attenuators. The LPF magnitudes are cleverly chosen in relation to the frequency-
dependent reverb time,  RT  60 (v) in  Equation 11.33 :
20 log 1 |h|2 5
260Ts
RT60
1 v 2  M
(11.33)
where
M 5 delay length
   h 5 the magnitude of the filter at some frequency v
 The problem with this setup is that the pole radii are no longer the same (circular in the  z
plane) and we know that this produces coloration in the signal. The solution (Jot 1992) was to
add a correction ﬁ lter at the end of the whole system,  T ( z ), whose magnitude is proportional
to the inverse square root of the frequency-dependent reverb time,  RT  60 (v). This is shown in
 Figure 11.34 .
 Finally,  Figure 11.35  shows a generalized version of the FDN. It does not include the absorptive
loss components (either static  k  values or  H  LPFs ) for clarity. The feedback matrix is always square,
 N  ×  N , where  N  is the number of delay lines. Because it is square, it can be made to be unitary.
 As an experiment, you can try to implement a four delay line version using the unitary matrix
 Equation 11.34  and gain coefﬁ cients proposed by Stautner and Puckette (1982), who were
researching multichannel reverb algorithms.
where
   G5
≥
g
"2
0   1   1      0
21   0   0   21
1   0   0   21
 0   1 21     0
¥
0 g 0 , 1.0
(11.34)
 You can also try to de-correlate the four delay line outputs by using Jot’s orthogonal matrix
and set the coefﬁ cients  c  N  according to  Equation 11.35 :
where
  C 5 g2
≥
1
21
1
21
¥
0 g2
0 , 1.0
(11.35)

Reverb Algorithms  389
∑
T(z)
∑
y(n)
g11  g21
g12  g22
Dry
c1
c2
x(n)
b1
 ∑
z–D1
H(z)
LP1
b2
∑
z –D2
H(z)
LP2
 Figure 11.34:    Addition of absorptive LPFs to the delay lines, plus a correction ﬁ lter  T ( z ) at the end.
g11  g21
g12  g22
gN1
gN2
g1N  g2N
gNN
Dry
c1
c2
cN
∑
y(n)
x(n)
b1
∑
∑
b2
bN
z–D1
z–D2
∑
z–DN
 Figure 11.35:    A generalized,  N -delay-line FDN.
   11.14    Other FDN Reverbs
 Smith (1985) developed a variation on the FDN theme with his waveguide reverberators.
Each waveguide consists of a two delay lines that move in opposite directions with
coupling coefﬁ cients for connecting waveguides together. Any number of waveguides may
be connected in any geometric pattern or shape, leading to a generalized set of scattering
matrices. The scattering matrices are similar to the feedback matrices—they contain the
coefﬁ cients that control the waveguide junction gains. The scattering matrices may also

390  Chapter 11
be made to be lossless by adhering to mathematical conditions involving the matrices’
eigenvalues and eigenvectors (Smith and Rocchesso 1994).
 Dahl and Jot (2000) proposed another UFDN type of reverb algorithm based on a structure
they call the  absorbent all-pass ﬁ lter  (AAPF).  Figure 11.36  shows the block diagram of the
AAPF which consists of a standard delaying APF with an LPF inserted in signal path. They
combined an early reﬂ ections block that consisted of a multi-tapped delay line with a late
reverberation block. In their late reverberation model, they use series AAPFs in a UFDN loop
as shown in  Figure 11.37 . The block marked M is the unitary feedback matrix that mixes and
creates  the inputs for the ﬁ lter loops.
 Chemistruck, Marcolini, and Pirkle (2012) also experimented with FDNs for reverb
algorithms. They used the genetic algorithm (GA) to generate coefﬁ cients for feedback delay
and ﬁ ltering blocks in their proposed system. A random unitary matrix generator was coded
to seed the GA so that the starting point is a UFDN. Their overall algorithm block diagram
consists of a four-delay line FDN followed by a diffusion block, as shown in  Figure 11.38 .
The GA was only used to ﬁ nd the delay network coefﬁ cients while the diffusion block
remained constant throughout.
x(n)
-D z
g
-g
LPF
a
y(n)
 Figure 11.36:    The absorbent APF features an LPF and attenuator in line with the delay element.
AAPF  L1
AAPF  L2
AAPF  L3
AAPF  L4
z-D
LPF
gL
AAPF  L5
AAPF  L6
Left  Input
Right  Input
M
AAPF  R1
AAPF  R2
AAPF  R3
AAPF  R4
Z-D
LPF
gR
AAPF  R5
A A P F R6
 Figure 11.37:    Dahl and Jot’s late reverberator using a unitary feedback matrix  M
and absorbent APFs.
Left  Output
Right  Output
Reverb Algorithms  391
x(n)
y(n)
Four-Delay  FDN
-D
Z
-DI2
Z
-D/3
Z
-D/4
Z
a0
Diffusion  Network
 Figure 11.38:    The FDN and diffusion network from the GA reverb.
 Two different FDNs were used; the normal four-delay line and the delay and LPF in series
( Figure 11.35 , but without the correction ﬁ lter on the output). An impulse response was taken
for the target listening environment. The GA ﬁ tness function used the time domain envelope
of the target impulse response for the matching criteria on the output of the system. During
each iteration, the top 35% of offspring candidates were kept and inter-mated. Mutation rates
of 10% for the delay coefﬁ cients and 5% for the LPF cutoff frequencies were used. More
details can be found in the original reference.
 We’ve analyzed many types of reverberator building blocks and complete algorithms. By now
you should be able to see that reverb design is fairly open-ended. An example room reverb
design follows.
   11.15    An Example Room Reverb
 For an example reverb, we have assembled a combination of reverberator modules to
implement a mono-in, stereo-out reverb. The reverb is based on many of the modules and
classic algorithms we’ve studied so far. The idea is to produce a realistic-sounding algorithm
that uses several of the more common reverberator modules so that you can use it as a
springboard for your own designs. It is a good place to start your experiments because it
features a variety of reverberator modules to help you get a better understanding of how
the parameters change the overall reverb sound.  Figure 11.39  shows the stereo impulse
response for the factory preset; notice the pre-delay, early reﬂ ections, and late reverb sections
are all present.  Figure 11.40  shows the block diagram and graphical user interface (GUI)
components for the example reverb. The complete algorithm consists of the following:
(cid:129)
(cid:129)
(cid:129)
(cid:129)
   Pre-delay with output attenuation.
   Input LPF (“bandwidth”).
   Two input APF diffusion modules.
   Two parallel comb ﬁ lter banks; four comb ﬁ lters per bank; one bank for left output, one
for right.
392  Chapter 11
0
(cid:23)(cid:23)10
8820
13230
17640
(cid:21)(cid:21)(cid:19)50
36-160
30870
35280
39690
1.000
0.707
0.500
0.000
(cid:177)0.500
(cid:177)0.707
(cid:177)1.000
1.000
0.707
0.500
0.000
(cid:177)0.500
–0.707
–1.000
0
4410
8820
13230
17640
22050
26460
30870
35280
39690
 Figure 11.39:    A stereo impulse response of the reverb plug-in, 100% wet output.
(cid:129)
(cid:129)
(cid:129)
(cid:129)
(cid:129)
(cid:129)
   Two comb ﬁ lters in each bank are LPF–comb ﬁ lters.
   Each parallel comb ﬁ lter bank feeds an output LPF (“damping”).
   The output LPF damping control also adjusts the LPF–comb ﬁ lter  g  values.
   Each output LPF feeds an output APF diffusion module.
   A single  RT   60  Control for all comb ﬁ lter gain variables.
   Individual APF and LPF controls for all parameters.
 You can see many familiar components of this algorithm. The inputs are summed together to
form a mono-signal and the left and right outputs are taken from two separate parallel comb
ﬁ lter echo generation units. The input bandwidth and output damping ﬁ lters are the same
as the Dattorro version but they also control the LPF–comb ﬁ lter cutoffs. The parallel comb
ﬁ lter banks are reminiscent of the Schroeder and Moorer algorithms. These reverb algorithms
often contain multiple instances of the same kinds of ﬁ lters—APF, Comb, LPFComb, and so
on. RackAFX provides you with several of these stock objects to use in your plug-ins. This
will allow you to quickly assemble variations on the algorithms. Here are some key points for
the design:
(cid:129)
(cid:129)
(cid:129)
   We follow Schroeder’s rule of about 1:1.5 ratio for shortest to longest comb ﬁ lter delay
(27.50 to 39.34 is our range, almost exactly 1:1.5).
   The  RT   60  time is used to calculate the gain variables for the comb ﬁ lters for a less colored
sound.
   The diffusion APFs alternate the sign of the  g  coefﬁ cient to create inverted/normal
 diffusion.
Reverb Algorithms  393
Left  In
0.5
Right  In
0.5
Bandwidth
Input  Diffusion
Pre-Delay
LPF1
APF1
APF2
Combl
c0
Comb2
c1
Damping
LPF2
APF3
Left  Out
LPFComb3
c2
LPFComb4
c3
Comb5
c3
Output  Diffusion
PComWDly
PCombTDly
PCombSDIy
Comb6
c5
Damping
LPF3
APF4
Right  Out
LPFComb7
c6
LPFComb8
c7
Figure 11.40: The room reverb algorithm.
•  Adjusting  the input APF delays has an effect  on early refl ections.
•  Adjusting  the output APF delays has a tonal coloration effect  on the fi nal reverb.
•  The key to less ringing/metallic  sound is getting the parallel comb fi lter delays just right,
and not violating the  1:1.5  rule.
•  The parallel comb fi lter attenuators, c0—c 7 are hard-coded  as 0.15, but there are several
ways you could experiment with this, or allow the user to control it. They also have alter
nating signs, which was found to greatly reduce pinging.
•  You can also provide unequal weighting such that the shorter (or longer) combs are more
(or less) emphasized.
394  Chapter 11
   11.16    RackAFX Stock Objects
 You can use any or all of the following stock objects in your plug-in—you just have to let
RackAFX know when you create your project initially. The stock objects consist of the following:
(cid:129)
(cid:129)
(cid:129)
(cid:129)
(cid:129)
   COnePoleLPF
   CDelay
   CCombFilter
   CLPFCombFilter
   CDelayAPF
 When you tell RackAFX to add them to your project, RackAFX automatically copies the ﬁ les,
#includes them, and adds them to your C11  project ﬁ le. Here’s a quick reference guide to the
objects. With these objects, coding our example reverb unit will be much easier than you think.
     11.16.1  COnePoleLPF
 Implements a one-pole LPF with a single coefﬁ cient,  g  in Figure 11.41. Table 11.4 shows
object members.
x(n)
1-g
∑
y(n)
z –1
g
 Figure 11.41:     The one-pole LPF.
  Table 11.4 :    The COnePoleLPF object.
Member Variables
ﬂ oat m_fLPF_g
ﬂ oat m_fLPF_z1
Member Functions
void setLPF_g(ﬂ oat fLPFg)
Parameters:
(cid:129)  ﬂ oat fLPFg
void init()
bool processAudio(ﬂ oat* pInput, ﬂ oat* pOutput)
Parameters:
(cid:129)  ﬂ oat* pInput
(cid:129)  ﬂ oat* pOutput
Purpose
Implements the one and only gain coefﬁ cient g.
Register to hold the single sample delay, z21.
set() function for gain coefﬁ cient.
Input: the new gain value.
Function to initialize and clear out the z21 register.
Process one input sample.
Input: pointer to a ﬂ oat input sample.
Output: pointer to the output sample destination.

11.16.2  CDelay

Implements a delay of  D  samples with an output attenuator. This is also the base class for
other stock objects in Figure 11.42. Table 11.5 shows object members.
Reverb Algorithms  395
Member Variables
ﬂ oat m_fDelayInSamples
ﬂ oat m_fOutputAttenuation
ﬂ oat* m_pBuffer
int m_nReadIndex
int m_nWriteIndex
int m_nBufferSize
int m_nSampleRate
ﬂ oat m_fDelay_ms
ﬂ oat m_fOutputAttenuation_dB
Member Functions
void init(int nDelayLength)
Parameters:
(cid:129)  int nDelayLength
void cookVariables()
void resetDelay()
void setDelay_mSec(ﬂ oat fmSec)
Parameters:
(cid:129)  ﬂ oat fmSec
void setSampleRate(int nFs)
Parameters:
(cid:129)  int nFs
void setOutputAttenuation_
dB(ﬂ oat fAttendB)
Parameters:
(cid:129)  ﬂ oat fAttendB
ﬂ oat readDelay()
ﬂ oat readDelayAt(ﬂ oat fmSec)
Parameters:
(cid:129)  ﬂ oat fmSec
void writeDelayAndInc(ﬂ oat
fDelayInput)
Parameters:
(cid:129)  ﬂ oat fDelayInput
bool processAudio(ﬂ oat* pInput,
ﬂ oat* pOutput)
Parameters:
(cid:129)  ﬂ oat* pInput
(cid:129)  ﬂ oat* pOutput
  Table 11.5:    The CDelay object  .
Purpose
Implements the one and only gain coefﬁ cient g
Output attenuation variable
Pointer to our dynamically declared buffer
Current read location
Current write location
Max buffer size
Sample rate
Delay in mSec, set by the parent plug-in
Output attenuation in dB, set by the parent plug-in
Declare the buffer and initialize it with 0.0s
Input: the buffer length in samples
Function to cook delay (nSec) and attenuation (dB) to the variables
actually used in the calculation
Reset the pointers to top, ﬂ ush buffer
Called by parent to set the delay in mSec
Input: the desired delay time
Set the sample rate; called by parent
Input: the sample rate integer
Set the output attenuator; called by parent
Input: the attenuation in dB
Read the delay with the user deﬁ ned delay time without incrementing
any pointers or writing input data
Read the delay at an arbitrary delay time without incrementing any
pointers or writing input data; caller must make sure that this does
not exceed max delay time
Input: the delay time to read
Write the input value and increment the pointer indices
Input: the input audio sample
Processes one input sample
Input: pointer to a ﬂ oat input sample
Output: pointer to the output sample destination
396  Chapter 11
x(n)
z –D
a0
y(n)
 Figure 11.42:      The delay with output attenuator.
x(n)
∑
z –D
y(n)
g
 Figure 11.43:     The comb ﬁ lter.
   11.16.3  CCombFilter
 Implements a  D -sample comb ﬁ lter with feedback coefﬁ cient  g  in Figure 11.43. Table 11.6
shows object members.
  Table 11.6:    The CCombFilter object.
Member Variables
ﬂ oat m_fComb_g
Member Functions
void setComb_g(ﬂ oat fCombg)
Parameters:
(cid:129)  ﬂ oat fCombg
void setComb_g_with_RTSixty(ﬂ oat fRT)
Parameters:
(cid:129)  ﬂ oat fRT
CDelay Base Class Overrides
bool processAudio(ﬂ oat* pInput, ﬂ oat* pOutput)
Parameters:
(cid:129)  ﬂ oat* pInput
(cid:129)  ﬂ oat* pOutput
   11.16.4  CLPFCombFilter
Purpose
The one and only feedback gain coefﬁ cient g
set() function for feedback gain coefﬁ cient
Input: the new feedback gain value
Set the feedback gain using the RT60 time
Input: RT60 time in mSec
Process one input sample
Input: pointer to a ﬂ oat input sample
Output: pointer to the output sample destination
 Implements a  D -sample comb ﬁ lter with LPF in feedback loop (with coefﬁ cient  g 2) and
feedback coefﬁ cient  g 1 in Figure 11.44. Table 11.7 shows object members.
x(n)
∑
z–D
y(n)
Reverb Algorithms  397
g1
∑
z–1
g2
 Figure 11.44:     The LPF–comb ﬁ lter.
  Table 11.7:    The CLPFCombFilter object.
Member Variables
ﬂ oat m_fComb_g
ﬂ oat m_fLPF_g
ﬂ oat m_fLPF_z1
Member Functions
void setComb_g(ﬂ oat fCombg)
Parameters:
(cid:129)  ﬂ oat fCombg
Purpose
The one and only feedback gain coefﬁ cient g1
The LPF gain coefﬁ cient, g2
Register for one pole LPF
set() function for feedback gain coefﬁ cient
Input: the new feedback gain value
void setComb_g_with_RTSixty(ﬂ oat fRT)
Parameters:
(cid:129)  ﬂ oat fRT
void setComb_g_with_RTSixty(ﬂ oat fRT)
Parameters:
ﬂ oat fRT
void setLPF_g(ﬂ oat fOverAllGain)
Parameters:
(cid:129)  ﬂ oat fOverAllGain
CDelay Base Class Overrides
void init(int nDelayLength)
Parameters:
(cid:129)  int nDelayLength
Set the LPF gain knowing an overall gain value; uses
Equation 11.27
Input: the overall gain in Equation 11.27
Dynamically create and init the buffer
Input: delay length in samples
bool processAudio(ﬂ oat* pInput, ﬂ oat* pOutput)
Parameters:
(cid:129)  ﬂ oat* pInput
(cid:129)  ﬂ oat* pOutput
Process one input sample
Input: pointer to a ﬂ oat input sample
Output: pointer to the output sample destination
