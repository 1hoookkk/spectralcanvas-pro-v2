# Audio Plugins C++ (Pirkle) — Chunk 33/73

> Source: `Designing Audio Effect Plug-Ins in C++_ With Digital Audio Signal Processing Theory - PDF Room.pdf` · Pages ~1-560 · Extractor: pdfminer.six
> Target ≈ 2800 words, Overlap = 200 words · Actual = 2796

> Note: DSP textbook content for SpectralCanvasPro RT-audio knowledge base

---
Delay Effects and Circular Buffers   217
Im
+12.0dB
0.0dB
-12.0dB
-24.0dB
-36.0dB
-48.0dB
-60.0dB
Re
2kHz
4kHz
6kHz
8kHz  10kHz  12kHz  14kHz  16kHz  18kHz  20kHz
 Figure 7.11:     The  z -plane pole/zero plot and resulting frequency response.
Im
+12.0dB
0.0dB
-12.0dB
-24.0dB
-36.0dB
-48.0dB
-60.0dB
Re
2kHz
4kHz
6kHz
8kHz  10kHz  12kHz  14kHz  16kHz  18kHz  20kHz
 Figure 7.12:     The  z -plane pole/zero plot and resulting frequency response for  D  = 4 samples.
Im
+12.0dB
0.0dB
-12.0dB
-24.0dB
-36.0dB
-48.0dB
-60.0dB
Re
2kHz
4kHz
6kHz
8kHz
10kHz  12kHz  14kHz  16kHz  18kHz  20kHz
 Figure 7.13:     The  z -plane pole/zero plot and resulting frequency response for  D  = 5 samples.
    k 5 1, 3, 5, ..., D21
cos(Dv) 1 j sin(Dv) 5 21 1 j0
cos(5v) 1 j sin(5v) 5 21 1 j0
v 5 ; kp
5
,  ; 3p
v 5 ; p
5
5
,  ; 3p
v 5 ; p
5
5
,  ; 5p
5
,  ; p
(7.10)
 This kind of frequency response in  Figure 7.13  is called  inverse comb ﬁ ltering . As we add
more and more samples of delay, we add more and more notches to the response. You can use

218  Chapter 7
the built in module in RackAFX to experiment.  Figures 7.14  and  7.15  show the frequency
response for 32 samples of delay—it’s an inverse comb ﬁ lter with 16 zeros in the positive
frequency domain.
   7.2.2  The Effect of Feedback
 When you add feedback to the delay, two things happen: ﬁ rst, for long delays your ear will
hear discrete repeating echoes that decay away after the signal is removed. As the delay time
gets shorter and shorter, the echoes merge, begin to ping, and then the actual ﬁ ltering (which
has been going on all the time) now dominates what you hear.
 In  Figure 7.16 , you can see the effect of feedback on the impulse response. The initial
impulse is the one through the dry path, and the next identical-sized one is the ﬁ rst output
of the delay line. The echoes decay, being reduced 90% on each trip through the ﬁ lter.
To understand the effect on frequency response, write the transfer function and take the  z
transform. To make it easier to ﬁ nd the pole frequencies, let the feedback value become 100%
+12.0dB
0.0dB
-12.0dB
-24.0dB
-36.0dB
-48.0dB
-60.0dB
+12.0dB
0.0dB
-12.0dB
-24.0dB
-36.0dB
-48.0dB
-60.0dB
2kHz
4kHz
6kHz
8kHz
10kHz  12kHz  14kHz  16kHz  18kHz  20kHz
 Figure 7.14:     Frequency response (linear) with  D  = 32 samples.
10Hz
100Hz
1kHz
10kHz
 Figure 7.15:     Frequency response (log) with  D  = 32 samples.
Delay Effects and Circular Buffers   219
1.000
0.707
0.500
0.000
–0.500
–0.707
–1.000
0
102
204
306
408
510
612
714
816
918
 Figure 7.16:     Impulse response with 90% feedback, 32 samples of delay.
1
1
s(n)
y(n)
x(n)
x(n) + fb*s(n)
Z –D
fb
 Figure 7.17:     Block diagram of the DDL with feedback.
or 1.0, as shown in the block diagram in  Figure 7.17 ; even though we know this would result
in oscillation, it will make calculating the frequencies easier.
 The difference equation is as follows:
  y(n) 5 x(n) 1 x(n2D) 2 fb * x(n2d) 1 fb * y(n2D)
(7.11)
 To derive the difference equation, label the output of the delay line  s ( n ) ( Equation 7.12 ):
The input to the delay line 5 x(n) + fb* s(n)
therefore
s(n) 5 x(n2D) 1 fb * s(n2D)
and
y(n) 5 x(n) 1 s(n)
5 x(n) 1 x(n2D) 1 fb * s(n2D)
(7.12)

220  Chapter 7
rearranging:
s(n) 5 y(n) 2 x(n)
s(n 2 D) 5 y(n2D) 2 x(n2D)
(7.13)
 Substituting  Equation 7.13  into  Equation 7.12  gives you the following difference equation:
y(n) 5 x(n) 1 x(n2D) 1 fb *3y(n2D) 2 x(n2D)4
5 x(n) 1 x(n2D) 1 fb * y(n2D) 2 fb*x(n2D)
 To ﬁ nd the transfer function, separate variables and take the  z  transform:
y(n) 2 fb*y(n2D) 5 x(n) 1 x(n2D) 2 fb*x(n2D)
   Y(z) 2 fbY(z)z2D 5 X(z) 1 X(z)z2D 31 2 fb 4
    Y(z)31 2 fbz2D 4 5 X(z)31 1 z2D 2 fbz2D 4
                       H(z) 5
                       H(z) 5
1 1 z2D 2 fbz2D
12fbz2D
5
Y(z)
X(z)
1 1 (12fb)z2D
12fbz2D
 (7.14)
(7.15)
 The new transfer function has both zeros (which we already calculated) and poles. The poles
are caused by the feedback and will occur whenever the denominator becomes zero. If we let
 fb  5 1.0, then equation  Equation 7.15  reduces to  Equation 7.16 .
H(z) 5
1
1 2 z2D
     0 5 1 2 z2D
       0 5 zD 2 1
   zD 5 1
Let z 5 ejv
ejDv 5 1
Apply Euler
cos(Dv) 1 j sin(Dv) 5 1
cos(U) 1 j sin(U) 5 1 1 j0
if
U 5 0, ; 2p,  ; 4p,  ; 6p, etc...
(7.16)

Delay Effects and Circular Buffers   221
cos(Dv) 1 j sin (Dv) 5 1 1 j0
if
Dv 5 0,  ; 2p
D
,  ; 6p
D
,  ; 4p
D
, . . .,  ; Np
D
until
N . D21
or
poles at v 5 ; kp
D
   k 5 0, 2, 4, 6, . . ., D
(7.17)
  Equation 7.17  shows that the poles will occur at even multiples of p and DC (0 Hz) when
Euler’s equation becomes 11  j 0; the analysis is nearly identical to the zero case. Consider the
case of four samples:
   k 5 0, 2, 4, 6, ..., D
cos(Dv) 1 j sin(Dv) 5 1 1 j0
cos(4v) 1 j sin(4v) 5 1 1 j0
v 5 ; pk
D
v 5 0,  ; p
2
v 5 0,  ; p
2
,  ; 4p
4
,  ; p
(7.18)
  Figure 7.18  shows the effect of 100% feedback – the response is technically inﬁ nite at the
pole frequencies. It produces a comb ﬁ lter (with the teeth of the comb pointing up) rather
than the inverse comb ﬁ lter you saw when examining the zeros. The amount of feedback will
affect the radius of the poles and the zeros, but not the pole or zero frequencies, which are
Im
+36.0dB
+20.0dB
+4.0dB
- 1 2 . 0 dB
- 2 8 . 0 dB
- 4 4 . 0 dB
- 6 0 . 0 dB
Re
2 k Hz
4 k Hz
6 k Hz
8 k Hz
10kHz
12kHz
14kHz
16kHz
18kHz
20kHz
 Figure 7.18:     The  z -plane pole/zero plot and resulting frequency response for  D  = 4 samples with
 fb  = 1.0. The peaks theoretically go to inﬁ nity.

222  Chapter 7
only dependent on the amount of delay. Consider the transfer function with a feedback value
of 0.75:
H(z) 5
1 1 (12fb)z2D
12 fbz2D
H(z)|fb50.75 5
110.25z2D
120.75z2D
(7.19)
 The poles will have a radius of 0.75, while the zeros will have a radius of 0.25. This will
result in the  z -plane plot and frequency response in  Figure 7.19 . You can see that the lowered
radius results in less gain at the pole frequencies. The peaks are now softer and the overall
gain is reduced down to about 18 dB from inﬁ nity. If you continue to drop the feedback to
50% (0.5) then the poles and zeros will be distributed at equal radii, as shown in  Figure 7.20 .
 As the feedback gets smaller and smaller, the response will turn into the pure inverse comb
ﬁ ltering when the poles disappear (by converging on  z  5 0) and the response goes to 0.0 at
the zero frequencies. What would happen if we inverted the feedback? This would mean that
the feedback value is a negative percentage. Consider –50% feedback and look at the transfer
function in  Equation 7.20 :
Im
+12.0dB
0.0dB
–12.0dB
–24.0dB
–36.0dB
–48.0dB
–60.0dB
Re
2kHz
4kHz
6kHz
8kHz  10kHz  12kHz  14kHz  16kHz  18kHz  20kHz
 Figure 7.19:     The  z -plane pole/zero plot and resulting frequency response for  D  = 4 samples,
75% feedback.
Im
+12.0dB
0.0dB
–12.0dB
–24.0dB
–36.0dB
–48.0dB
–60.0dB
Re
2kHz
4kHz
6kHz
8kHz  10kHz  12kHz  14kHz  16kHz  18kHz  20kHz
 Figure 7.20:     The  z -plane pole/zero plot and resulting frequency response for  D  = 4 samples,
50% feedback.

Delay Effects and Circular Buffers   223
(7.20)
H(z) 5
1 1 (12fb)z2D
12fbz2D
H(z)|fb5 21 5
111.5z2D
110.5z2D
 If you look at  Equation 7.20 , you can ﬁ gure out that the pole frequencies are going to lie at
the zero frequencies (notice the signs of the coefﬁ cients). The zeros will be at a radius of 1.5,
while the poles will be at 0.5. A frequency lying on the unit circle will be under the inﬂ uence
of all four poles and zeros.
 For the four-sample delay, a feedback value of –62% will make the frequency response
perfectly ﬂ at, but with –3 dB of attenuation seen in  Figure 7.21 . The poles will have radii of
0.62 with the zeros at radii of 1.38. This means you can create a delay that has no comb/inverse
comb ﬁ ltering, but only at this particular value. Other negative feedback values will give
varying degrees of cancellation. In practice, the poles will dominate and small peaks can appear
at high inverted feedback values. In the time domain, the echoes will alternate between positive
and negative values each time they are ﬂ ipped in the feedback path, shown in  Figure 7.22 .
Im
+12.0dB
0.0dB
–12.0dB
–24.0dB
–36.0dB
–48.0dB
–60.0dB
Re
2kHz
4kHz
6kHz
8kHz
10kHz  12kHz  14kHz  16kHz  18kHz  20kHz
 Figure 7.21:     At –62% feedback with 4 samples of delay, the frequency response
becomes ﬂ at but slightly attenuated.
1.000
0.707
0.500
0.000
–0.500
–0.707
–1.000
0
102
204
306
408
510
612
714
816
918
 Figure 7.22:     The effect of inverted feedback on the impulse response; feedback
is –90% here.

224  Chapter 7
    7.3    Design a DDL Module Plug-In
 In the previous projects, it was easy enough to simply declare left and right delay elements
and coefﬁ cients for our simple ﬁ lters. However, as the ﬁ lters become more complex, this
becomes more tedious and is also bad coding practice since we have replicated some code.
For educational purposes, it is better to split the code out at ﬁ rst, but now it’s time to think
modularly. More complicated delay effects like the stereo cross feedback or LCR (left center
right) delay will be easier to implement if we have a ﬂ exible delay module to work with. In this
project, you will create a DDL module that you can use in many other projects. It is going to be
a slight modiﬁ cation on the above DDL + wet/dry control. Here are the speciﬁ cations:
•
•
•
•
•
•
   Implements an  n -sample delay line, user controllable, up to 2 seconds of delay.
   Delay is given in milliseconds.
   Has feedback capable of –100% to +100% operation.
   Has wet/dry mix ratio control; 50/50 is an equal mix while 100/0 is full wet (delay only).
   Allows the feedback sample to be taken from the delay line, or supplied “outside” the
module; the reason for this will become evident later.
   Allows access to the current feedback output sample. This allows you to insert other
 effects into the feedback path by using the switch to choose feedback_in; it also allows
for cross coupling the feedback paths of stereo modules.
 The block diagram is given in  Figure 7.23 .
 For this design, let’s implement a mono-only version. We can then make stereo versions
by making two of our member variables the module object. That’s right—you can use one
plug-in inside another by making it a member object. First, let’s get started on the DDL
module. Note: For the initial DDL modules, don’t worry about the feedback path switching.
We need to get the delay debugged and running ﬁ rst.
x (n)
fb_in(n)
fb_out(n)
–D
Z
fb
Dry
Wet
∑
y(n)
 Figure 7.23:     Our more ﬂ exible DDL module; a switch allows the user to choose where the
feedback sample is taken. Here it is using the normal output feedback path. The feedback output
is available for use at the fb_out pin.
Delay Effects and Circular Buffers   225
  7.3.1  Project: DDLModule
 By now, you should be getting good at RackAFX programming, so we can move more
quickly through the process. Create the project and add the sliders.
   7.3.2  DDLModule GUI
 Here is the ﬁ nal graphical user interface (GUI) for the DDL module in  Figure 7.24 . You can
use my variable names or make up your own. You will need the controls shown in  Table 7.1 .
We do not need a switch for the feedback option on the UI; it will only be needed by the
super plug-in that includes this module as a member object.
Delay
0.00 mSec
Feedback
0.00 %
Wet/Dry
0.00 %
 Figure 7.24:     The DDL Module GUI.
  Table 7.1:    GUI controls for the DDL module
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
Value
Delay
mSec
ﬂ oat
m_fDelay_ms
0
2000
0
Value
Feedback
%
ﬂ oat
m_f_Feedback_pct
–100
100
0
(continued)
226  Chapter 7
Table 7.1: GUI controls for the DDL module (Continued)
Slider Property
Control Name
Units
Variable Type
Variable Name
Low Limit
High Limit
Initial Value
Value
Wet/dry
%
ﬂ oat
m_f_WetLevel_pct
0
100
50
   7.3.3  DDLModule.h File
 In the .h ﬁ le, add the cooked variables, m_fDelayInSamples, m_fFeedback, and
m_fWetLevel:
  // Add your code here: --------------------------------------------------------- //
   ﬂ oat m_fDelayInSamples;
   ﬂ oat m_fFeedback;
   ﬂ oat m_fWetLevel;
  // END OF USER CODE ------------------------------------------------------------ //
  // ADDED BY RACKAFX -- DO NOT EDIT THIS CODE!!! -------------------------------- //
  //    **--0x07FD--**
   ﬂ oat m_fDelay_ms;
   ﬂ oat m_fFeedback_pct;
   ﬂ oat m_fWetLevel_pct;
  // **--0x1A7F--**
  // ----------------------------------------------------------------------------- //
 Note: I named the RackAFX controls with _ms and _pct appended to remind me that
these values need to be cooked to be used.
 Note: The delay time cooked variable is a ﬂ oat, m_fDelayInSamples rather than an integer
number of samples. This is because we will allow  fractional delay  in a future version of the
module. For now, though, we will treat it as an integer.
   7.3.4  DDLModule .cpp File
  Constructor
•
   Initialize variables.

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
