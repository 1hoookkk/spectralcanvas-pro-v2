# Audio Plugins C++ (Pirkle) — Chunk 15/73

> Source: `Designing Audio Effect Plug-Ins in C++_ With Digital Audio Signal Processing Theory - PDF Room.pdf` · Pages ~1-560 · Extractor: pdfminer.six
> Target ≈ 2800 words, Overlap = 200 words · Actual = 2883

> Note: DSP textbook content for SpectralCanvasPro RT-audio knowledge base

---
How DSP Filters Work  75
3.
4.
5.
   ½ Nyquist
   ¼ Nyquist
   Impulse
 For each audio sample that enters there are two phases to the operation:
1.
2.
   Read phase: The sample is read in and the output is formed using the difference equation
and the previous sample in the delay register.
   Write phase: The delay element is overwritten with the input value—the sample stored in
the single  z 21 register is effectively lost.
 Start with the DC/step input and begin sequentially applying the samples into the ﬁ lter shown
in  Figures 4.7  through  4.10 .
 Now, observe the amplitude and phase shift of the input versus output—the output amplitude
eventually settles out to a constant 1.0 or unity gain condition, so at DC or 0 Hz, the output
Step/DC (0 Hz)
{...0,1,1,1,1,1,1...}
0
0.5
y(n)  =  (0.5) * (0.0)  +  (0.5) * (0.0)  =  0.0
0
Z
x(n-1)
0.5
0
0.5
y(n)
0
-1
Z
TRASH
0.5
 Figure 4.7:    On the ﬁ rst iteration the input sample is used in the difference
equation to create the output, x(n) 5 0, y(n) 5 0 and then the
input is shifted into the delay register.
 In a feed-forward filter, the amount of time smearing is equal to the maximum delayed path
through the feed-forward branches.
76  Chapter 4
1
0.5
y(n) = (0.5) * (1.0) + (0.5) * (0.0) = 0.5
0
-1
Z
x(n -1)
0.5
1
0.5
y(n)
0
-1
Z
TRASH
0.5
 Figure 4.8:    The process continues with each sample. Here the input 1.0 is
combined with the previous input; the second output y(n) 5 0.5.
equals the input. However, there is a one sample delay in the response, causing the leading
edge of the step-input to be smeared out by one sample interval. This time smearing is a
normal consequence of the ﬁ ltering.
 Next, repeat the process for the Nyquist frequency (DC and Nyquist are the easiest, so we’ll
do them ﬁ rst). The ﬁ lter behaves in an entirely different way when presented with Nyquist
( Figures 4.11  through  4.14 ).
 Now, make your observations about amplitude and phase. The amplitude at Nyquist
eventually becomes zero after the one-sample-delay time. The phase is hard to tell because the
signal has vanished. Why did the amplitude drop all the way to zero at Nyquist? The answer
is one of the keys to understanding digital ﬁ lter theory: the one-sample delay introduced
 Delay elements create phase shifts in the signal. The amount of phase shift depends on the
amount of delay as well as the frequency in question.
1
0.5
y(n) = (0.5) * (1.0) + (0.5) * (1.0) = 1. 0
How DSP Filters Work  77
1
-1
Z
x(n-1)
0.5
1
0.5
y(n)
1
-1
Z
TRASH
0.5
1
0.5
y(n) = (0.5) * (1.0) + (0.5) * (1.0)  =  1.0
1
-1
Z
x(n- 1)
0.5
Figure 4.9: The sequence continues until we observe a repeating pattern;
1.0 is repeating here.
exactly 180 degrees of phase shift at the Nyquist frequency and caused it to cancel out
when recombined with the input branch through a 0 .
 In the case of Nyquist, the one-sample delay is exactly enough to cancel out the original
signal when they are added together in equal ratios. What about other frequencies like
78  Chapter 4
1.0
1.0
Input
1.0
1.0
Output
 Figure 4.10:    The input and output sequences for the ﬁ lter in  Figure 4.6  at DC or 0 Hz.
Nyquist  Response
+ 1 , - 1 , + 1 , -1
1
0.5
y(n) = (0.5) * (+1.0) + (0.5) * (0.0) = 0.5
-1
0  z
x ( n -1)
0.5
+1
0.5
y(n)
0
-1
Z
TRASH
0.5
 Figure 4.11:    The Nyquist sequence is applied to the ﬁ lter. Notice how the delay
element has been zeroed out. The output for the ﬁ rst iteration is y(n) 5 0.5.
½ and ¼ Nyquist? They are a bit more laborious to work through but worth the effort. By
now you can see how the data moves through the ﬁ lter, so let’s use a table ( Table 4.1 ) and
move the data through  it  instead. The ½ Nyquist sequence is  x ( n ) 5 {…0, 11.0, 0.0, 21.0,
0.0, 11.0, 0.0, …}.
-1
0.5
y (n)  = (0.5) * (-1.0) + (0.5) * (+1.0) = 0.0
How DSP Filters Work  79
-1
+ 1  Z
x ( n -1)
0.5
-1
0.5
y(n)
+1
Z -1
TRASH
0.5
 Figure 4.12:    The second iteration at Nyquist produces an output y(n) 5 0.
  Table 4.1:    The manual labor continues as we work
through the ½ Nyquist frequency.
x(n)
x(n 2 1)
y(n) 5 0.5x(n) 1
0.5x(n 2 1)
0
1
0
21
0
1
0
21
0
0
0
1
0
21
0
1
0
21
0
0.5
0.5
20.5
20.5
0.5
0.5
20.5
20.5
 Can you see how  x ( n ) becomes  x ( n  2 1) for the next row? The  x ( n  2 1) column holds a one-
sample-delayed version of the input  x ( n ). The output is  y ( n ) 5 {0, 10.5, 10.5, 20.5, 20.5,
10.5, 10.5}.
80  Chapter 4
+1
0.5
y(n) = (0.5) * (+1.0) + (0.5) * (-1.0) = 0.0
-1  Z
x(n-1)
0.5
+1
0.5
y(n)
-1
-1
Z
TRASH
0.5
-1
0.5
y(n) = (0.5) * (-1.0) + (0.5) * (+1.0) = 0.0
+1  Z -1
x ( n -1)
0.5
 Figure 4.13:    Continuing the operation at Nyquist, we see that eventually the
output settles to a repeating 0, 0, 0, 0 sequence.
 Next we observe the amplitude and phase relationship from input to output in  Figure 4.15 .
At ﬁ rst it might seem difﬁ cult to ﬁ gure out the sequence {…20.5, 20.5, 10.5, 10.5, …}.
½ Nyquist is also encoded with a repeating sequence of four values (0, 1, 0, 21).
 Work through ¼ Nyquist the same way ( Table 4.2 ). The ¼ Nyquist frequency sequence is
 x ( n ) 5 {0, 0.707, 1, 0.707, 0, 20.707, 21, 20.707, 0, …}.
+ 1. 0
+1.0
How DSP Filters Work  81
Input
Output
-1.0
-1.0
 Figure 4.14:    The input and output sequences for the ﬁ lter in  Figure 4.6  at Nyquist.
  Table 4.2:    ¼ Nyquist input/output.
x(n)
x(n 2 1)
y(n) 5 0.5x(n) 1
0.5x(n 2 1)
0
0.707
1
0.707
0
20.707
21
20.707
0
0
0
0.707
1
0.707
0
20.707
21
20.707
0
10.354
10.854
10.854
10.354
20.354
20.854
20.854
20.354
 The output is  y ( n ) 5 {…10.354, 10.854, 10.854, 10.354, 20.354, 20.854, 20.854,
20.354, 10.354, …}. Analysis of the output sequence reveals the phase-shifted and slightly
attenuated output signal at ¼ Nyquist. Both the phase shift and the attenuation are smaller
than ½ Nyquist. As you can see in  Figure 4.16  there is also one sample of time smearing at
the start of the signal.
 Finally, apply the impulse sequence and ﬁ nd the impulse response. The impulse response
is the third analysis tool. The impulse response deﬁ nes the ﬁ lter in the time domain like the
frequency response deﬁ nes it in the frequency domain. The basic idea is that if you know how
the ﬁ lter reacts to a single impulse you can predict how it will react to a series of impulses of
varying amplitudes. Take a Fast Fourier Transform (FFT)  of the impulse response and you
get the frequency response. An inverse FFT converts the frequency response back into the
impulse response. For this ﬁ lter the impulse response is simple ( Table 4.3 ).
82  Chapter 4
+1.0
-1.0
+ 1. 0
Time
Smearing
-1.0
Phase Shift
Input
Output
 Figure 4.15:    The input/output relationship in time at ½ Nyquist. The ½ Nyquist
 frequency is attenuated almost by one-half. The output is also phase
shifted by 45 degrees. The leading edge of the ﬁ rst cycle is smeared
out by one sample's worth of time.
+ 1.0
-1.0
+ 1. 0
Time
Smearing
-1.0
Phase Shift
Input
Output
 Figure 4.16:    The input/output relationship at ¼ Nyquist.
How DSP Filters Work  83
  Table 4.3:    The impulse response input/output relationship.
x(n)
x(n 2 1)
y(n) 5 0.5x(n) 1
0.5x(n 2 1)
0
1
0
0
0
0
0
0
1
0
0
0
0
0.5
0.5
0
0
0
 Here you can see that the impulse is ﬂ attened and smeared out. It is actually two points on
a sin( x )/( x )-like curve, as shown in  Figure 4.17 . Now, you can combine all the frequency
amplitude and phase values into one big graph, as shown in  Figure 4.18 .
 We observe that this digital ﬁ lter is a low-pass variety with a typical low pass ﬁ lter (LPF)
magnitude response. However the phase response is quite interesting—it is linear instead of
nonlinear like the analog example at the beginning of the chapter. In fact, this simple ﬁ lter is
a  linear phase ﬁ lter .
  Figures 4.19  and  4.20  show the measured frequency and phase responses for this ﬁ lter.
Compare it with our estimation. Notice the linear phase produces a straight line only
when plotted on the linear frequency axis. What makes this ﬁ lter a low-pass ﬁ lter? It is a
combination of the coefﬁ cients and the ﬁ lter topology (ﬁ rst-order feed-forward). There are
three basic topologies: feed forward (FF), feed back (FB), and a combination of
FF/FB. Once the topology has been chosen, it’s really the  coefﬁ cients  that determine what
the ﬁ lter will do.
+1.0
+1.0
Input
Output
-1.0
-1.0
 Figure 4.17:    The time domain plots of the impulse response input/output.
 A feed-forward filter will be a linear phase filter if its coefficients are symmetrical about their
center. In this case (0.5, 0.5) is symmetrical. Another example would be (20.25, 0, 20.25).
84  Chapter 4
Magnitude
1.0
0.9
0.7
0.0
Phase
0°
-45°
-90°
1
Nyquist
4
fs/8
1
2
Nyquist
fs/4
Frequency
Nyquist
fs/2
Angle
 Figure 4.18:    Final frequency and phase response plots for the digital ﬁ lter in
 Figure 4.6 . Notice that this is a linear frequency plot since ½ Nyquist is
halfway across the x-axis. The phase at Nyquist pops back up to 0 degrees
since there is no Nyquist component present (output 5 0).
   4.2    Design a General First-Order Feed-Forward Filter
 To illustrate this and crystallize it as a concept, modify your SimpleHPF ﬁ lter so that two
sliders control the a 0  and a 1  coefﬁ cients directly. Also, alter the range of values they can
control to (21.0 to 11.0). Then, experiment with the two values and watch what happens in
the analyzer. How to do this is described next.
 Open your SimpleHPF project and modify the user interface (UI). First, change the values for
the a 1   slider to match the new low and high limits. As usual, you right-click inside the slider’s
bounding box and alter the limits and initial value (shown in bold) as in  Table 4.4 .
How DSP Filters Work  85
+12.0dB
0.0dB
–12.0dB
–24.0dB
–36.0dB
–48.0dB
–60.0dB
+90.0°
+60.0°
+30.0°
0.0°
–30.0°
–60.0°
–90.0°
2kHz
4kHz
6kHz
8kHz
10kHz  12kHz  14kHz  16kHz  18kHz  20kHz
2kHz
4kHz
6kHz
8kHz
10kHz  12kHz  14kHz  16kHz  18kHz  20kHz
 Figure 4.19:    Measured frequency and phase response plots for the ﬁ lter you just analyzed by
hand. These are plotted with a linear frequency base.
+12.0dB
0.0dB
–12.0dB
–24.0dB
–36.0dB
–48.0dB
–60.0dB
+90.0°
+60.0°
+30.0°
0.0°
–30.0°
–60.0°
–90.0°
10Hz
100Hz
1kHz
10kHz
10Hz
100Hz
1kHz
10kHz
 Figure 4.20:    Measured frequency and phase response plots with log frequency base.
86  Chapter 4
  Table 4.4:    The altered a 1  slider properties.
Slider Property
Control Name
Units
Variable Type
Variable Name
Low Limit
High Limit
Initial Value
Value
a1
ﬂ oat
m_fSlider_a1
21.0
11.0
0.0
 Now add a new slider for the a 0  coefﬁ cient just below the a 1  slider ( Table 4.5 ).
  Table 4.5:    The new a 0  slider properties.
Slider Property
Control Name
Units
Variable Type
Variable Name
Low Limit
High Limit
Initial Value
Value
a0
ﬂ oat
m_fSlider_a0
21.0
11.0
1.0
 Change the userInterfaceChange() function to directly map the slider values to the coefﬁ cient
values. The new slider has a control ID of 10; always check your nControlIndex value since it
might be different depending on your UI.
           Variable Name                          Index
  -----------------------------------------------
           m_fSlider_a1                          0
           m_fVolume_dB                          1
            m_fSlider_a0                         10
  -----------------------------------------------
  bool CSimpleHPF::userInterfaceChange(int nControlIndex)
  {
    switch(nControlIndex)
    {
How DSP Filters Work  87
    case 0:
      // direct map to the a1 Slider
      m_f_a1_left = m_fSlider_a1;
      m_f_a1_right = m_fSlider_a1;
      break;
    case 1:
      // cook the Volume Slider
      m_fVolume = pow(10.0, m_fVolume_dB/20.0);
      break;
     case 10:
      // direct map to the a0 Slider
       m_f_a0_left = m_fSlider_a0;
       m_f_a0_right = m_fSlider_a0;
       break;
    default:
      ; // do nothing
    }
    return true;
  }
 Rebuild the DLL  and load it. Place the volume control at 0 decibels (dB) (maximum) and set
a 0  and a 1  to 1.0 (maxima); then open the analyzer and hit the Frequency button—play with the
different values for a 0  and a 1 .  Figure 4.21  shows the response with various coefﬁ cient values.
 After you play around with the controls, there are several things to note from this experiment:
(cid:129)
   You can get a low-pass or high-pass response, or anything between, including a ﬂ at
response (a 0  5 1.0, a 1  5 0.0).
+12.0dB
0.0dB
–12.0dB
a0 = 1.0   a1 = 1.0
a0=1.0
a1 = -0.5
–24.0dB
a0 = 1.0  a1 = -0.9
–36.0dB
–48.0dB
–60.0dB
10Hz
100Hz
1kHz
10kHz
 Figure 4.21:    Three different combinations of coefﬁ cient settings
yield three different ﬁ lters.

88  Chapter 4
 The topology of the filter determines its difference equation. The coefficients (a N ) of a filter
determine its filter frequency and phase response and therefore its type (HPF, LPF, etc.) and
its sonic qualities. Your plug-in implements the difference equation in processAudioFrame().
Your plug-in calculates the coefficients in userInterfaceChange() in response to the user making
changes to the control surface.
(cid:129)
(cid:129)
(cid:129)
   Inverted settings give the identical frequency response but the phase responses will be
inverted (use the Phase button and have a look for yourself).
   You can get gain out of the ﬁ lter.
   You can also get attenuation out of the ﬁ lter.
   4.3    First-Order Feed-Back Filter
 A ﬁ rst-order feed-back ﬁ lter is shown in  Figure 4.22 .
 The difference equation is as follows:
  y(n) 5 a0 x(n) 2 b1y(n 2 1)
(4.2)
 You can see the feed-back nature of the ﬁ lter; the output  y ( n ) is fed back into the summer
through a one-sample-delay  z  21  element. Notice that the feedback coefﬁ cient has a negative
sign in front of it and the difference equation reﬂ ects this with the 2b 1  term. The negative
sign is for mathematical convenience and will make more sense in the next chapter when we
analyze the difference equations in more detail. Also notice that there is no b 0  coefﬁ cient—
there will not be a b 0  coefﬁ cient in any of the feed-back ﬁ lters. To analyze this without math
requires going through the same procedure as before, analyzing the amplitude and phase of
the basic test signals. Another option would be to code it as a plug-in in RackAFX and map
the coefﬁ cients directly to the sliders as you did in the previous example. Then, you can
experiment with the coefﬁ cients and see how they affect the ﬁ lter’s frequency, phase,
DC/step, and impulse responses.
a0x(n)
x (n)
a0
(cid:153)
y(n)
z -1
–b1y(n –1)
–b1
y(n – 1)
 Figure 4.22:    First-order feed-back ﬁ lter block diagram.
