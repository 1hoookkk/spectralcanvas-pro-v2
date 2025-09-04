# Audio Plugins C++ (Pirkle) — Chunk 14/73

> Source: `Designing Audio Effect Plug-Ins in C++_ With Digital Audio Signal Processing Theory - PDF Room.pdf` · Pages ~1-560 · Extractor: pdfminer.six
> Target ≈ 2800 words, Overlap = 200 words · Actual = 4204

> Note: DSP textbook content for SpectralCanvasPro RT-audio knowledge base

---
bool __stdcall CSimpleHPF::userInterfaceChange(int nControlIndex)
  {
Writing Plug-Ins with RackAFX   69
  // decode the control index
  switch(nControlIndex)
  {
  case 0:
  {
  m_f_a1_left = m_fSlider_a1;
  m_f_a1_right = m_fSlider_a1;
  m_f_a0_left = m_f_a1_left - 1.0;
  m_f_a0_right = m_f_a1_right - 1.0;
  break;
  }
   case 1:
   {
   m_fVolume = pow(10.0, m_fVolume_dB/20.0);
   break;
   }
  default:
  break;
  }
  return true;
  }
 Build and test your code to make sure the plug-in works as expected. You now understand the
basics of writing a plug-in from a block diagram and design equations.
   Add some presets
 Now that you have a GUI with more than one control, try adding some presets. If you look
at the preset list in the toolbar, you will see the ﬁ rst one is named Factory Preset. This preset
contains your initial settings for the GUI controls. You cannot delete this preset and it is
automatically updated whenever you add, edit, or delete controls. This preset takes you back
to your initial state. With your plug-in loaded and (optionally) audio streaming through it,
adjust the controls to affect the signal. Hit the Save Preset button on the toolbar or choose
it from the plug-in menu. A box will pop up allowing you to name the preset. You can also
overwrite an existing preset. You can store up to 32 presets in your plug-in.
     3.13    The User Plug-In Menu in RackAFX
 As you write more plug-ins, you will notice that they begin to automatically populate the user
plug-in menu item in RackAFX. By now, you should have three plug-ins in this menu. There
are a few things you need to understand about this menu.
70  Chapter 3
•
•
•
   It allows you to play with the plug-ins without having to open your compiler and manu-
ally load and unload the DLLs.
   You can select different plug-ins from this menu while audio is streaming and they will
automatically slot in and out, so you can audition or show off your plug-ins quickly.
   It allows RackAFX to behave just like other plug-in clients by loading all the DLLs it
ﬁ nds in its PlugIns folder at once when you start the software.  This can be dangerous!
 That last item poses a problem during the course of development—if you write a DLL that
does bad things in the constructor, such as hold pointers with garbage values or try to access
memory that hasn’t been allocated, it may crash RackAFX when it ﬁ rst starts up. If your
DLL behaves really badly, you might even wound the OS too. This is a difﬁ cult issue to avoid
without complicated rules for commissioning and decommissioning the plug-in. Additionally,
you will have the same problem if you are developing a commercial plug-in and you are
using a third-party client; most of these are designed to ﬁ rst open all the DLLs in their plug-in
folder and check to make sure the plug-ins can be instantiated. If you write a bad DLL, you
might also crash these clients and/or the OS. In at least one commercially available client,
if your plug-in crashes during startup, it will not be loaded again in future launches. When
RackAFX loads your DLL, it does some error checking to try to make sure your plug-in is
legal, but it can’t check the validity of your construction code.
 If RackAFX crashes each time you open it, remove the last DLL you were working on from the
PlugIns folder. Alternatively, you can remove all the DLLs—you will want to copy them and
restore them later when you find the bad DLL that caused the crashing. Writing a DLL is chal-
lenging and fun, but since you are writing a component, you can wind up with crashes like this.
   CHAP TE R 4
How DSP Filters Work
    During the course of this book you will learn how to implement the following effects:
(cid:129)
(cid:129)
(cid:129)
(cid:129)
(cid:129)
(cid:129)
   EQs/tone controls
   Delay
   Flanger/chorus
   Compressor/limiter/tremolo
   Reverb
   Modulated ﬁ lters/phaser
 The EQ/tone control theory is the most difﬁ cult of all the effects to explain in a simple
way. These effects are based on DSP ﬁ lter theory which involves complex algebra, that
is, the algebra of complex numbers. Complex numbers contain real and imaginary parts.
There are two basic ways to explain basic DSP theory. The ﬁ rst is intuitive and involves
no complex math but requires some bookkeeping and can be tedious. The second method
uses complex algebra to solve the problem. We’ll start with the intuitive method, and then
(optionally, if you desire) make the leap into complex math. Don’t worry—we don’t want
to get mired down in theory and forget the fun part, which is making audio effects. If you
skip the second part, you will still be able to code EQ and tone control plug-ins, but you will
better understand where the equations come from if you know a little theory too. The transfer
functions you will learn along the way will reappear in many effects algorithms.
 In  Figure 4.1  you can see the 26 dB/octave roll-off indicative of a ﬁ rst-order ﬁ lter. A digital
version of the analog ﬁ lter should have the same shape and roll-off. A key difference is that
the digital ﬁ lter will not operate beyond the Nyquist frequency.
 The phase response plot shows the relative phases of different frequencies upon exiting the
ﬁ lter. During the ﬁ ltering process, the phases of different frequencies get shifted forward or
backward. In  Figure 4.2 , the 1 kHz frequency is shifted by 245 degrees compared to the
input. At very high frequencies, the phase shift approaches 290 degrees. This phase shift is
not a side effect of the ﬁ ltering but an integral part of how it works.
 To understand the ramiﬁ cations of the phase shifting, consider a complex waveform entering
the ﬁ lter. Fourier showed that a complex, continuous waveform could be decomposed into a
71
72  Chapter 4
+12.0dB
0.0dB
-12.0dB
-24.0dB
-36.0dB
-48.0dB
-60.0dB
-6dB/octave
10Hz
100Hz
1kHz
10kHz
 Figure 4.1:    The fundamental analysis tool for a DSP ﬁ lter is its frequency response plot.
This graph shows how the ﬁ lter ampliﬁ es or attenuates certain bands of frequencies.
+90.0°
+60.0°
+30.0°
0.0°
-30.0°
-60.0°
-90.0°
- 45 degrees
10Hz
100Hz
1kHz
10kHz
 Figure 4.2:    The phase response plot of the analog ﬁ lter in  Figure 4.1  shows how the
phase of the output is shifted across frequencies.
set of sinusoids with different frequencies and amplitudes.  Figure 4.3  shows a ﬁ lter in action.
The input is decomposed into four sinusoids, a fundamental, and three harmonics. The peak-
amplitudes of the components are shown as dark bars to the left of the  y  axes.
 We observe several features of the output:
(cid:129)
(cid:129)
(cid:129)
   The composite waveform is smoothed out.
   The amplitude and phase shift of the fundamental are unchanged.
   The three harmonics have decreasing amplitudes and more phase shift as you go higher
in frequency.
  Figure 4.4  shows the same ﬁ lter with the information plotted differently; here, the amplitudes
and phases are plotted against frequency rather than dealing with a set of time-domain
sinusoids. You can see by the output frequency response plot that this ﬁ lter is a kind of
low-pass ﬁ lter. Its curve is similar to  Figure 4.1 , the analog equivalent.
Amplitude
Amplitude
Time
FILTER
Time
How DSP Filters Work  73
Amplitude
f1
Amplitude
f2
Amplitude
f3
Amplitude
f4
Amplitude
f1
Amplitude
f
2
Amplitude
f3
Amplitude
f4
Time
Time
Time
Time
Time
Time
Time
Time
 Figure 4.3:    A complex waveform is ﬁ ltered into a smoothed output. The input and output are
decomposed into their Fourier-series components.
Amplitude
Amplitude
FILTER
f
1
f
2
f
3
f
4
Frequency
f
1
f
2
f
3
f
4
Frequency
Phase
Shift
0°
-45°
-90°
Phase
Shift
0°
-45°
-90°
f
1
f
2
f
3
f
4
Frequency
f
1
f
2
f
3
f
4
Frequency
 Figure 4.4:    The same information is plotted as frequency and phase responses.
74  Chapter 4
  4.1    First-Order Feed-Forward Filter
 In order to get a grip on the nature of digital ﬁ ltering, start with the ﬁ rst-order feed-forward
ﬁ lter shown in a block diagram in  Figure 4.5 . You’ve already seen a version of it in the HPF
tone control you coded in the last chapter.
 The difference equation is as follows:
  y(n) 5 a0x(n) 1 a1x(n 2 1)
(4.1)
 You can tell why it’s called feed forward—the input branches feed forward into the summer.
The signal ﬂ ows from input to output. There is no feedback from the output back to the input.
Now, suppose we let the coefﬁ cients a 0  and a 1  both equal 0.5 in  Figure 4.6 .
 In order to analyze this ﬁ lter we can go the easy but tedious route or the difﬁ cult but elegant
route. Let’s start with the easy way. In order to ﬁ gure out what this does, you apply the ﬁ ve
basic digital test signals you learned in  Chapter 1  to the ﬁ lter and then manually push the
values through and see what comes out. You only need a pencil and paper or a simple
calculator . The ﬁ ve waveforms we want to test are:
1.
2.
   DC (0 Hz)
   Nyquist
a0x(n)
x(n)
-1
Z
x(n-1)
a0
a1
Σ
y(n)
a1x(n-1)
 Figure 4.5:    The ﬁ rst-order feed-forward ﬁ lter.
x(n)
0.5
Σ
Y(n)
-1
Z
0.5
 Figure 4.6:    What kind of ﬁ lter is this? What are its frequency and phase responses?

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
