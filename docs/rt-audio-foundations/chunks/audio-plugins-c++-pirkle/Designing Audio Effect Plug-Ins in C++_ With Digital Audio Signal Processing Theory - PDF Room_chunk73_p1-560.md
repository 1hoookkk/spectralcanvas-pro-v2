# Audio Plugins C++ (Pirkle) — Chunk 73/73

> Source: `Designing Audio Effect Plug-Ins in C++_ With Digital Audio Signal Processing Theory - PDF Room.pdf` · Pages ~1-560 · Extractor: pdfminer.six
> Target ≈ 2800 words, Overlap = 200 words · Actual = 3404

> Note: DSP textbook content for SpectralCanvasPro RT-audio knowledge base

---
524  Appendix B
  bool __stdcall CVolume::joystickControlChange(ﬂ oat fControlA,
  ﬂ oat fControlB,
  ﬂ oat fControlC,
  ﬂ oat fControlD,
  ﬂ oat fACMix,
  ﬂ oat fBDMix)
  {
  }
  // add your code here
  return true;
 You can see that RackAFX leaves you a comment on the coding of the joystick location. The
six variables arrive as function parameters and you use them however you wish. In  Figure B.5
you can also see four drop list controls to the right of the joystick. I have already set them up
to have something meaningful in them for the purpose of demonstration. These controls are
generally used for two purposes:
1.
2.
   They describe the current “meaning” of the apex lettering system; in  Figure B.5  the
A-corner is the “SINE” corner.
   They allow the user to change the meaning of the apex in the plug-in by selecting
different values.
 You set up the drop list boxes when you are in development mode by clicking on them and
choosing the item “Select To Setup,” which allows you to connect these drop list boxes
to different enumerated lists. They follow the same rules as the other enumerated lists,
however, like the LCD control, you can use the underscore (_) to create a blank space in
the string at run time. When the user selects an item from the box, the client calls your
userInterfaceChange() function, passing it the parameters as described in the
comment section for that method. You can then handle those messages as you wish.
  Joystick Drop List Boxes
Index
  -----------------------------------------------
  Drop List A
  Drop List B
  Drop List C
  Drop List D
60
61
62
63
 The joystick program is an advanced user’s control for automating movement of the joystick
in reaction to some event. Please see the website for tutorials and sample code for using the
program.
More RackAFX Controls and GUI Designer  525
   B.3    Using the sendUpdateGUI() Method
 This is a perfect place to explain how to use the sendUpdateGUI() method. You can force
the user interface (UI) (RackAFX or a custom graphical user interface [GUI]) to change its
settings based on your internal variable values. This is opposite the normal ﬂ ow where the GUI
calculates and alters your variables. In this case, you alter your own variable and tell the GUI
to update accordingly. This can be used to move sliders in real time or toggle radio buttons
programmatically. The joystick drop lists present an interesting use of this method. Suppose
you wanted to have four identical lists that represent the joystick corners, such as {A, B, C,
D} = {SINE, SAW, TRIANGLE, NOISE} and the user will move the joystick to produce
a mixture of low-frequency oscillators (LFOs) in your plug-in. You would like the lists to
contain the same values, but you would like to initialize them with other values than the ﬁ rst
one. For example, you might want the initial state of the boxes to show SINE for A, SAW for
B, NOISE for C, and TRIANGLE for D (notice this is different than the order of the list too).
When you set up the enumerated list, you give it a list variable to connect to (e.g., m_uApexA)
and a list that goes along with that. By clicking “Use Existing String List,” then double-
clicking on any of the items in the list, you can legally share your string lists. This is something
we did not cover in the book chapters because it is a more advanced feature. Then, in your
code (prepareForPlay() or wherever you need to) you can alter your own unsigned integer-type
(UINT) variables then call sendUpdateGUI() to show the changes. For example, to initialize
the drop lists as described above, you would add this code to your prepareForPlay() method:
  // set my variables
  m_uApexA = SINE;
  m_uApexB = SAW;
  m_uApexC = NOISE;
  m_uApexD = TRIANGLE;
  // tell the GUI to change accordingly
  sendUpdateGUI();
 Theoretically, you can do this from any point in the code  except the constructor  (because
the messaging pointer isn’t assigned until after construction), even from the joystick
handler if needed. However, you should never initiate client communication like this in the
processAudioFrame() method as it relies on a slow window-messaging system. This is true
for other plug-in formats as well. Remember that you can use the sendUpdateGUI() method
to alter the GUI for any of the RackAFX 4 Plug-In connected variables.
   B.4    Using GUI Designer
 RackAFX has a powerful GUI designer that lets you arrange visually appealing GUIs in
a matter of just a few moments. The GUI designer is always being updated with more
526  Appendix B
features, so always check the latest additions at the website. Depending on which version you
have, your GUI designer will look more or less like that shown in  Figure B.8 . The ﬂ ow of
operations is as follows:
1.
2.
3.
4.
5.
   In prototype view (the main RackAFX view), you assemble your plug-in. You create
controls, give them min, max, and initial values, connect them to variables, and so on.
Because this is a development mode, you will probably change some controls, add or
remove them, and so on. Also, because we are prototyping, we can set up a bunch of
controls we really would not want the user to be able to adjust in the ﬁ nal plug-in. An
excellent example is the reverb plug-in; once we’ve got those parallel comb ﬁ lters tuned,
we don’t want the user messing with them.
   After your plug-in is complete, debugged, and ready to go, you click on the GUI  Designer
tab to reveal the blank GUI surface, shown in  Figure B.8 .
   You drag and drop controls from the left side and arrange them however you like on the
surface. Because they have transparent backgrounds, they can be overlapped.
   For the slider, radio buttons, and knob controls, you must connect the control with the
variable in your plug-in that it will control.
   The LCD and vector joystick controls don’t need any additional setup.
 Suppose after testing we decide that the ﬁ nal GUI for the reverb project needs to have the
following controls, while hiding all the others:
(cid:129)
(cid:129)
(cid:129)
   Pre-delay time
   Pre-delay attenuation
   Bandwidth
Background
and  Text Color
Selection
Control
Palette
GUI  Control Surface
 Figure B.8:    The GUI Designer.
More RackAFX Controls and GUI Designer  527
 Figure B.9:    Customizing a knob by right-clicking it.
(cid:129)
(cid:129)
(cid:129)
   Damping
   RT60
   Wet/dry
 Any of the prototype’s sliders can be dragged and dropped either as knobs or sliders using the
palette on the left.  Figure B.9  shows the GUI designer after one knob control has been placed
and mapped to the pre-delay time. A second knob has just been dropped next to it. Right-
clicking on the knob itself pops up another customization box. Here you can do the following:
(cid:129)
(cid:129)
(cid:129)
(cid:129)
   Connect the knob to a variable via the drop list.
   Change the appearance of the knob and dot combination.
   Change the edit box behavior, hide it, or change its colors.
   Set all the other knobs to match this one.
 You can do similar things for the radio buttons, meter, or the slider control, as shown
in the pop-up boxes in  Figures B.10  and  B.11 . The current GUI designer rules are as
follows:
(cid:129)
(cid:129)
   For sliders, you link to a control variable, just like the knobs. You can customize many
aspects of the slider control’s appearance.
   When you run out of continuous controls (sliders) you can’t drag any more knobs/slider
controls.
528  Appendix B
 Figure B.10:    The slider, joystick, LCD, and meter GUI customization dialog boxes.
(cid:129)
(cid:129)
(cid:129)
(cid:129)
(cid:129)
(cid:129)
   Each time you drag a knob or slider, you must set it up before dragging any other knobs
or sliders.
   The joystick can have its colors altered and you can set the program button to look like
the assignable buttons. There can be only one joystick.
   You can change the IN and OUT button bitmaps for the assignable buttons. There can be
only one assignable button bank.
   You can add any continuous control (slider) to the GUI LCD group; it is a separate entity
from the one on your main RackAFX GUI. Here I added bandwidth and damping. There
can be only one LCD control.
   When you drag meter controls, RackAFX starts with the leftmost meter on the main GUI
and automatically links your control variable for you. It then moves from left to right as
you drag and drop meters. If you need to skip meters, add all the meters ﬁ rst, then remove
the ones you don’t want.
   When you add radio button controls , RackAFX starts with the topmost button bank on the
main GUI and automatically links your control variable for you. It then moves from top
to bottom as you drag the radio buttons. If you need to skip button banks, add all of them
ﬁ rst, then remove the ones you don’t want. The buttons banks are highly customizable.
More RackAFX Controls and GUI Designer  529
 Figure B.11:    The Radio and Assignable button GUI customization dialog boxes.
 Figure B.12:    The ﬁ nished reverb GUI.
  Figure B.12  shows my ﬁ nished reverb GUI. I chose the white background just to make it
easier to see in print. After you are done, go back to the prototype view and rebuild your
plug-in. You need this rebuild to embed the new GUI information into your code. The GUI
styles are coded into UINT arrays in your control variables as well as the plug-in object.
530  Appendix B
See the website for more details on the encoding. Now, load your plug-in. You will notice that
the blue knob button on the toolbar is now active. You can open your GUI with that button
or the menu item View S Custom Plug-In GUI.  Figure B.13  shows the ﬁ nished GUI. The
designer automatically crops it to ﬁ t the controls you’ve placed on it. When you alter the new
GUI controls, the RackAFX controls change as well (but not the other way around).
 Figure B.13:    The reverb GUI in action!
 If you have enabled your plug-in to be compiled to produce a Virtual Studio Technology  (VST)
plug-in (Appendix A) then your RackAFX custom GUI will be available in any Windows VST
 client that supports custom GUIs (which is just about all of them). You launch it in whatever
way the client digital audio workstation (DAW) requires.

  Index
 A
  absorbent all-pass ﬁ lters
(AAPFs)  390
  addition operation  17
  address generation unit
(AGU)  207
  all-pass ﬁ lter (APF)
reverberators  368–71
  alpha wheel GUI controls  519
  analog-to-digital
conversion  1, 170–8
  anti-aliasing ﬁ lters  3
  applications programming
interfaces (APIs)  27–9, 501
   functions typically required
of 29–30
   and RackAFX philosophy
31–3
  asynchronous operation  6
  attack times of envelope
detectors  428–9
  attenuation function and
attenuators  18–19
  AU programming and
processing  515–18
  audio ﬁ lter designs  253–87
  audio speciﬁ c ﬁ lters  188–96
 B
  beats-per-minute (BPM)
synchronization  235
  bilinear transform  172–5, 179–80
  bipolar/unipolar functionality
324–6
  bi-quadratic ﬁ lters (bi-quads)
157–62
   design of  181–8
   modiﬁ ed  189
  Bohn, D.  196
  book-keeping and book-keeping
records  13–15, 114–15
  Browne, S.  357
  Butterworth ﬁ lters  184–7, 475
 C
  C programming language  24
  C++ programming
language  24,  29, 31
  Cartesian coordinate system  100–1
  cascaded graphic ﬁ lters  194–6
  Chaigne, A.  385
  Chemistruck, M.  390
   child  plug-in objects  239
  chorus effect  331–4
  circular buffers  209–10, 273
  comb ﬁ lter reverberator  364–8
  comb ﬁ lters  221
  comparison of objects, GUIs and
functions  505–6
  complementary ﬁ lter design
266–70
  complex conjugates  133–4, 144
  complex numbers  100–1, 112
  complex sinusoid function  97–100
  compressor/limiter plug-in
design  457–65
  compressors  453–6
  conjugate poles of resonators
167–70
  constant-Q ﬁ lters  192–4
  convolution plug-ins  273–81
  convolution process  254–8, 274
   cooking  functions  57, 227, 230–1
 D
  Dahl, L.  390
  Dattorro, J.  381
  Dattorro plate reverb  382–5
531
  Decca chorus  354
  delay effects  207–9
   modulated  327–54
  delay operators  16
  delaying all-pass ﬁ lter
reverberators  368–71
  difference equations  18
    z  transform of  117–18
  digital audio technology,
history of  1
  digital delay line (DDL)  209–15,
327
   advanced form of
module  248
   module plug-in  224–33
  digital signal processing (DSP):
address generation unit  207
   basic theory of  97–122
   chips  4
   ﬁ lters  71–95
   practical application of
theory  123–62
   test signals  10–12
  digital-to-analog converters
(DACs)  3
  digitally-controlled ampliﬁ ers/
attenuators (DCAs)  454
  dimension-style choruses
351–3
  direct  z -plane design
techniques  163–4
  Doppler effect  328
  downward expander/gate plug-in
design  466–8
  dynamic link library (DLL): access
to  22
   building of  35
  dynamic linking  21–2
  dynamic processing  453–87
Index
 E
   early reﬂ ections   358
  echoes  360
  eigenfrequencies  361
  energy decay relief (EDR)
plots  362–4
  enumerated slider variables  245–8
  envelope detection  426–36
  envelope follower plug-in
design  425–8
  Euler’s equation  98–9, 104–7,
112, 138–9, 148, 152, 155,
215, 221
  expanders  453–6
 F
  feed-back ﬁ lters: ﬁ rst-order  88–95,
123–4
   second-order  142–9
  feed-forward ﬁ lters  119
   ﬁ rst-order  74–88, 94–5,
103–6
   second-order  132–41
  feedback  221–2
   effect on impulse
 response  218–19, 222–3
  feedback delay network (FDN)
reverbs  385–91
   generalized  385–9
  ﬁ nite impulse response (FIR)
ﬁ lters  161–2, 253, 262,
266–7, 281–7
  ﬂ anger/vibrato effect  328–31
  ﬂ anger/vibrato/chorus plug-in
334–42
  ﬂ oating-point data  9–10
  fractional delay  235–9
  Frenette, J.  381–2
  frequency response: direct
evaluation  127–30
   estimation  121, 133–8, 143–7,
  Gardner, W. G.  357, 366–8, 377–81
  Gardner’s nested APF
reverberators  377–81
   gates   453
  genetic algorithm (GA)  390–1
  Gerzon, M.A.  377
  Gordon–Smith oscillator  299–301
  graphical user interfaces
(GUIs)  25–8, 35
   for DDL Module  225–6
  Griesinger, D.  358, 360
 H
  half-wave envelope detectors  428
   hard-knee  compression curves  456
 I
  impulse response: effect of
feedback on  218–19,
222–3;  see also  room impulse
response
  inﬁ nite impulse response (IIR)
ﬁ lters  161–204, 207, 253
  interpolation functions  236–7
  interrupt-based designs  6–7
  inverse comb ﬁ ltering  217–18
 J
  Jot, J.-M.  385–90
 K
   knee  of the compression curve  456
  Korg Triton  250–1, 345, 350
  Kuttruff, H.  361–2
 L
  Lagrange interpolator  284
   late reverberations   358
  limiters  453
  linear interpolation  236–8
  linear phase ﬁ lters  83, 266–7
  Linkwitz–Riley ﬁ lters  186–7,
151–2, 158–60
475, 479
 M
  Mac programming  501, 503,
506, 515
  McGrath, D. S.  357
  Marcolini, K.  390
  Massberg, Michael  201
  Massberg analog-matched low-pass
ﬁ lter  201–4
  median ﬁ lter algorithm  284–7
  member objects  224
  mixing algorithms  19
  mod ﬁ lter plug-in design  412–25
  modal density  361–2
   modes  (room resonances)  360–1
  modulated APF reverberator  381–2
  modulated delay effects  327–54
  modulated delay line (MDL)  327
  modulated ﬁ lter effects  411–51
  Moorer, J. A.  375, 377
  Moorer’s reverberator  365–71
  moving average (MA)
interpolator  282–3
  multi-tap delay  249–50
  multi-unit LCR chorus plug-
in  345–50
  multiplication operation  16–17
 N
  National Semiconductor  438
  negative frequencies  104–5, 113
  nested ﬁ lter structures  379–81
  normalized frequency  104
  numerical coding of audio data
7–9
  numerical method FIR ﬁ lters
281–7
  Nyquist frequency  1–2, 11–12,
76–7, 95
 O
  one-sample delay  15–16
   optimal  method for designing
  frequency response plots  71–2,
  liquid crystal display (LCD) GUI
ﬁ lters  270–3
83–5, 104, 111
  frequency sampling method  263–6
  full-wave envelope detectors  428
 G
  gain control  122–3
  gain function  18
controls  519
  look-ahead compressor plug-in
design  468–71
  low-frequency oscillation
(LFO)  328, 331
  low-pass ﬁ lter–comb
reverberator  373–5
   order  of a ﬁ lter  114
  Orfanidis, S.  429, 455
  oscillators  289–326
   bipolar/unipolar
 functionality  324–6
   direct-form  289–99
   wave-table type  301–8
532
Index
 P
  panning  see  tremolo/panning
plug-in design
  parametric ﬁ lters  191–4
   parent  plug-ins  233, 239
  Parks–McClellan
algorithm  262, 270
  peaking ﬁ lters  see  parametric ﬁ lters
  phase inversion function  18
  phase response plots  71–2, 83–5,
101, 104, 111
  phaser plug-in design  436–45
  ping-pong delay  250
  plate reverb algorithm  382–5
  plug-in, deﬁ nition of  21
  polar notation  100–1
  pole-zero ﬁ lters  149–57
  poles of the transfer function
124–32, 145
  poles or zeros at inﬁ nity, effect
of  178–80
  polynomials, factoring of
119–20
  pre-delay  358
  Puckette, M.  385, 388
  ‘pumping and breathing’
artifacts  463
 R
  RackAFX software  29–70
   design of tone control
plug-ins  58–69
   design of volume control
 plug-ins  40–1, 54–8
   destruction of plug-in
 objects 38
   dynamic link library  35–6
   FIR ﬁ lter designer  262
   frequency sampling method
tool  267–9
   graphical user interfaces
36–7, 525–30
   impulse convolver  258–62
   optimal method tool  271–3
   processing audio  37–8
   setting up plug-ins for use and
setup preferences  41–54
   stock objects  394–8
   testing  38–40
   user menu  69–70
  reconstruction ﬁ lters  3
  Reilly, A.  357
  Reiss, J.  455, 486
  release times of envelope
detectors  428–9
  Remez exchange algorithm
271–3
  resonant LPF plug-in
196–201
  resonators  165–70
  reverb algorithms  357–409
  reverb modules  363
  reverb time  359–60
  ring modulator plug-in design
494–6
   soft-knee  compression curves
456–7
  software development kits
(SDKs)  501
  Sony DPS–M7
354
  spectral compressor/expander
plug-in design  475–86
  square wave form
317, 319
  static linking  21–2
  Stautner, J.  385, 388
  stereo crossed feedback delay
plug-in  244
  stereo digital delay plug-in
  Roland Dimension D chorus
239–44
351–3
  room impulse response
358–60
  room resonances  see  modes
  room reverb: design of  398–408
   example of  391–3
   RT  60  reverb time  359–60
 S
  Sabine, W.  359–60
  samples of audio signal, acquisition
of  1–3
  sampling theorem  2, 7, 106
  saw-tooth waveform  317–18
  scalar multiplication operation
16–17
  scattering matrices  389–90
  Schroeder, M.  357, 360–4, 368,
370, 376–7, 385
  Schroeder’s reverberator
372–3
  ‘sendUpdate GUI()’ method
525
  shelving ﬁ lters  156–7,
189–90, 201
  sigmoid functions  497
  signal-processing algorithms  13
  signal-processing ﬂ ow  6–7
  signal-processing systems  4–5
  single pole ﬁ lters  164–5
  slider controls  245
  Smith, Dave  522
  Smith, J. O.  389
  Smith-Angell improved
resonator  168–70
533
  stereo-linking a dynamics
processor  472–4
  stereo phaser with quad-phase
LFOs  446–51
  stereo quadrature ﬂ anger
plug-in  342–5
  stereo reverberation
376–7
   subsequent reverberations
358
  subtraction operation  17
  synchronous operation  5–6
 T
  through-zero ﬂ anging (TZF)
330
  time delay as a mathematical
operator  102
  transfer functions  101, 104
   evaluation of  106–12
  transverse delay line  257
  tremolo/panning plug-in
design  489–94
  trivial zeros  125
 U
  unitary feedback delay networks
(UFDNs)  386, 390
 V
  vector joystick control
519–24
  vibrato  see  ﬂ anger/vibrato/chorus
plug-in; ﬂ anger/vibrato effect
  virtual address space  22–3
Index
 W
  wave shaper plug-in design
497–500
  wave table oscillators  301–8
  wave tables: addition of
308–12
   band-limited  312–19
  waveguide reverberators
389
  wet/dry mix  214–15
  white noise  360
  ‘ willpirkle.com ’ website  519
  Windows operating system  501–3
  wrapping of plug-in objects  503–5,
512–14
 Z
   z  substitution  114
   z  transform  114–21, 133, 141–2
   of difference equations
117–18
   of impulse responses
118–19
   of signals  116–17
   zero frequencies  119–20,
145
  Zöler, U.  429, 455
534
