# Audio Plugins C++ (Pirkle) — Chunk 13/73

> Source: `Designing Audio Effect Plug-Ins in C++_ With Digital Audio Signal Processing Theory - PDF Room.pdf` · Pages ~1-560 · Extractor: pdfminer.six
> Target ≈ 2800 words, Overlap = 200 words · Actual = 2656

> Note: DSP textbook content for SpectralCanvasPro RT-audio knowledge base

---
64  Chapter 3
 Flush out delay elements in preparation for each play event in the plug-in. You generally do not
want old data sitting inside these storage registers. The only exceptions are delay-looping effects
where you exploit the old data. This is done in prepareForPlay().
   userInterfaceChange()
•
•
   Store the new a 1  values.
   Cook the slider data to get the a 0  values.
  bool __stdcall CSimpleHPF::userInterfaceChange(int nControlIndex)
  {
  switch(nControlIndex)
  {
  case 0:
  {
  // save a1
   m_f_a1_left = m_fSlider_a1;
   m_f_a1_right = m_fSlider_a1;
        // calculate a0
   m_f_a0_left = m_f_a1_left - 1.0;
   m_f_a0_right = m_f_a1_right - 1.0;
  break;
  }
  default:
  break;
  }
  return true;
  }
 Build and load the project, open an audio ﬁ le, and test your plug-in to make sure it’s working
properly. This plug-in is a simple low-cut ﬁ lter and with the slider all the way down, you should
hear no difference in the music. When you move the slider up, you will lose more and more bass
frequencies, allowing only the high frequencies to pass. The cut-off frequency is approximately
11 kHz and the slider controls the amount of low frequency cut. You should easily be able to hear
the effect, even over small speakers. Now, to get a little more information about the plug-in’s
operation, use RackAFX’s analysis tools. Stop the music from playing with the transport control.
Launch the analyzer window by clicking on the Analyzer button in the toolbar or choose View >
Analyzer. The analyzer pops up as shown in  Figure 3.18  (yours may look slightly different).
 The analyzer is a powerful tool for checking your plug-in’s performance. The basic controls are
1.
2.
3.
4.
   Scope/spectrum analyzer.
   Basic graphing options.
   Scope controls.
   Real-time response buttons, which let you measure the frequency, phase, impulse, and
step responses of the plug-in (audio must not be streaming to use these).
 Click on the Frequency button and you will get a frequency response plot of the ﬁ lter. Move
the slider all the way down and you should get a ﬂ at response, as shown in  Figure 3.19 . If you
move the slider all the way up so that a 1  5 0.49, you get the response in  Figure 3.20 .
Writing Plug-Ins with RackAFX   65
1
2
+12.0dB
0.0dB
-12.0dB
-24.0dB
-36.0dB
-48.0dB
-60.0dB
4
3
 Figure 3.18:    The audio analyzer.
10Hz
100Hz
1kHz
10kHz
 Figure 3.19:    A ﬂ at frequency response with a 1  = 0.0.
66  Chapter 3
+12.0dB
0.0dB
-12.0dB
-24.0dB
-36.0dB
-48.0dB
-60.0dB
10Hz
100Hz
1kHz
10kHz
 Figure 3.20:    A ﬁ lter that appears to boost high frequencies. You can see that
it is really cutting the low frequencies instead; this is something you might only
have realized by using the audio analyzer.
     3.12    Design a High-Frequency Tone Control with Volume Plug-In
 This ﬁ nal example will show you how to deal with more than one slider control by simply
adding a volume-in-dB control to the block diagram. The plan is to add another slider to
the existing plug-in; the new slider will control the overall volume of the plug-in in dB. You
already know how to implement both parts of it, so this exercise is really more about adding
new controls to an existing project.
  3.12.1  Project: SimpleHPF
 Open your SimpleHPF project in RackAFX using the Open button or the menu/toolbar items.
   3.12.2  SimpleHPF GUI
 Add the new volume slider: Right-click on the second slider group and add a new slider
for the volume control, in dB, and use the exact same variable name and settings as in the
VolumedB project. You should end up with a GUI like that in  Figure 3.21 .
   3.12.3  SimpleHPF.h File
 Add your own m_fVolume variable to handle the cooked volume data, just as before.
  // Add your code here: --------------------------------------------------------- //
  ﬂ oat m_f_a0_left;
  ﬂ oat m_f_a1_left;
  ﬂ oat m_f_a0_right;
  ﬂ oat m_f_a1_right;
  ﬂ oat m_f_z1_left;
  ﬂ oat m_f_z1_right;
   ﬂ oat m_fVolume;
Writing Plug-Ins with RackAFX   67
a1
0.00
Volume
-6.00
 Figure 3.21:    The new SimpleHPF GUI with added volume-in-dB control.
  // END OF USER CODE ------------------------------------------------------------ //
 // ADDED BY RACKAFX -- DO NOT EDIT THIS CODE!!! -------------------------------- //
  //   **--0x07FD--**
  ﬂ oat m_fSlider_a1;
  ﬂ oat m_fVolume_dB;
  // **--0x1A7F--**
  // ----------------------------------------------------------------------------- //
   3.12.4  SimpleHPF.cpp File
  Constructor
•
   Cook the volume data after the ﬁ lter initializations.
  CSimpleHPF::CSimpleHPF()
  {
  <SNIP SNIP SNIP>
  m_f_a0_left = −1.0;
  m_f_a1_left = 0.0;
  m_f_a0_right = −1.0;
  m_f_a1_right = 0.0;
  m_f_z1_left = 0.0;
  m_f_z1_right = 0.0;
   m_fVolume = pow(10.0, m_fVolume_dB/20.0);
  }
   prepareForPlay()
 There is nothing to add here because the volume variable does not need to be reset on each
play event.

68  Chapter 3
   processAudioFrame ()
•
   Add the volume control scaling  after  the ﬁ ltering operation.
  // Do LEFT (MONO) Channel; there is always at least one input/one output
  // (INSERT Effect)
  // Input sample is x(n)
  ﬂ oat xn = pInputBuffer[0];
  // READ: Delay sample is x(n−1)
  ﬂ oat xn_1 = m_f_z1_left;
  // Difference Equation
  ﬂ oat yn = m_f_a0_left*xn + m_f_a1_left*xn_1;
  // WRITE: Delay with current x(n)
  m_f_z1_left = xn;
  // Output sample is y(n)
   pOutputBuffer[0] = yn*m_fVolume;
  // Mono-In, Stereo-Out (AUX Effect)
  if(uNumInputChannels == 1 && uNumOutputChannels == 2)
   pOutputBuffer[1] = yn*m_fVolume;
  // Stereo-In, Stereo-Out (INSERT Effect)
  if(uNumInputChannels == 2 && uNumOutputChannels == 2)
  {
  // Input sample is x(n)
  ﬂ oat xn = pInputBuffer[1];
  // Delay sample is x(n-1)
  ﬂ oat xn_1 = m_f_z1_right;
  // Difference Equation
  ﬂ oat yn = m_f_a0_right*xn + m_f_a1_right*xn_1;
  // Populate Delay with current x(n)
  m_f_z1_right = xn;
  // Output sample is y(n)
   pOutputBuffer[1] = yn*m_fVolume;
  }
   userInterfaceChange()
•
•
   Cook the volume data.
   Make sure you check your control ID values in case you chose different sliders than I did.

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
