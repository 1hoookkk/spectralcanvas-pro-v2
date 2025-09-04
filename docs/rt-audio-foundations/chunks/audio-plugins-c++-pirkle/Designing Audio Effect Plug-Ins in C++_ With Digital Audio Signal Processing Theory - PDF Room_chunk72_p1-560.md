# Audio Plugins C++ (Pirkle) — Chunk 72/73

> Source: `Designing Audio Effect Plug-Ins in C++_ With Digital Audio Signal Processing Theory - PDF Room.pdf` · Pages ~1-560 · Extractor: pdfminer.six
> Target ≈ 2800 words, Overlap = 200 words · Actual = 3240

> Note: DSP textbook content for SpectralCanvasPro RT-audio knowledge base

---
More RackAFX Controls
and GUI Designer
 RackAFX has several optional graphical user interface (GUI) controls that were not covered
in the book chapters. It also has a GUI designer that lets you convert your slider-based default
GUI into a fully customized version. The website  www.willpirkle.com  is always the best
way to get the latest information on the GUI designer and any additional controls that may be
added at a later date. The two additional GUI controls are the alpha wheel and liquid crystal
display  (LCD) and the vector joystick control.
  B.1    The Alpha Wheel and LCD Control
 In the upper right of the main RackAFX UI is a control that represents an alpha wheel, LCD
matrix view, and the control knob. During the course of designing your project, you’ve noticed
that the LCD is used to give you feedback about the state of your project and compiler.
However, it can also be used as a GUI control just like the other sliders. The LCD control allows
you to store up to 1024 continuous slider controls inside it. Anything you can set up with a
normal slider control will work as one of the LCD embedded controls. And, the GUI designer
lets you place one of these controls on your own GUI. There are several uses for the control:
(cid:129)
(cid:129)
(cid:129)
   It’s a different way to store the continuous controls.
   If you run out of sliders, you can always put 1024 more of them in the LCD.
   You can use the enumerated list variable to create a set of presets or other global func-
tions or conﬁ guration controls.
 To set up the LCD control, right-click on it when you are in development mode (not with
a plug-in loaded). A box pops up as shown in  Figure B.1 . Click on New to open the very
familiar slider properties dialog. Fill it out just as you would any other slider control. After
adding a few more controls, you might have something resembling  Figure B.2 , with three
controls embedded. You can edit the controls, remove them, or move them up and down the
list (which changes their sequence in the LCD control).
 You treat these embedded slider controls just as any other slider, so there’s nothing to add to
your normal operational code. When you run your plug-in, use the alpha wheel to select one
519
520  Appendix B
 Figure B.1:    The New button pops up the slider properties to add a control to the LCD.
 Figure B.2:    The LCD is loaded with three controls.
of the controls and the value knob to adjust its value. Note there is currently no edit control
connected to the LCD for manual entry of control values.  Figure B.3  shows the LCD control
with the alpha wheel adjusted for the bass control.
 An alternative way to use the LCD control is for global operations (like effects
conﬁ gurations) or to store presets, either because you need more than RackAFX provides or
More RackAFX Controls and GUI Designer  521
Alpha
Wheel
Indicator:
Control 2 of 3
is selected
 Figure B.3:    The alpha wheel and LCD control is reminiscent of vintage synthesizer and rack
effects.
Value Knob
 Figure B.4:    The underscores are automatically removed from enumerated list variables in the
LCD control.
you need to write your own methods for loading and storing data. You can use the enumerated
list variable to create up to 256 individual string names. You can also take advantage of a
feature of the LCD control: You can use the underscore (_) to separate strings in your list (you
can’t have spaces in an enumerated name). When the LCD control uses them, it replaces the
underscores with single white spaces. This allows you much more ﬂ exibility than the radio
buttons or normal sliders provide. For example, you could set up a preset list with names
like “Heavy_Lead,” “Liquid_Pad,” “Lush_Space,” and so on. Then, you can use the up/
down arrows to place your “presets” control at the top of the control list. Now, as shown in
 Figure B.4 , the user can select one of your custom presets (remember, it’s up to you to code
your own preset format) and then scroll to adjust the other controls. The underscores have
been replaced with spaces.
   B.2    The Vector Joystick Control
 RackAFX also has a dedicated vector joystick control that communicates with your plug-in.
Dave Smith designed the ﬁ rst vector synthesizers for Sequential Circuits, Inc. and then later
for Yamaha and Korg. The vector joystick is a combination of a traditional  x / y  track-pad
and an exponential joystick. The calculation documentation is available on the website.
In the original synthesizers, the vector joystick was used to program or alter in real time the
mixture of two or four different waveforms. The position of the joystick determines the mix
522  Appendix B
Joystick
B
A
Vector Joystick
A
B
C
D
SINE
TRI
PITCH
SQUARE
C
Apex Drop
List Controls
D
A:0.25 C:0.25 B:0.25 D:0.25   Mix Blurb
JS Program
Joystick Program
 Figure B.5:    The RackAFX vector joystick.
ratios, which are not linear but rather exponential. In RackAFX, you can use the joystick to
alter up to four different parameters at once, exponentially. In addition, a linear  x / y  value of
the joystick location is available for your plug-in.  Figure B.5  shows the vector joystick and
components.
 The joystick is arranged in a diagonal with four corners labeled A, B, C, D, clockwise;
this is in keeping with Dave Smith’s original lingo. As the joystick moves around, a new
set of mix ratios is generated and appears in the mix blurb. The mix ratios will always
add up to 1.0. In the center, the ratios are all 0.25. A second set of values also exists, the
AC-mix and the BD-mix, which are linear. On the AC axis, 0 is at the A apex and 1.0 is at
the C corner. On the BD axis, 0 is at D and 1.0 is at B. In the center position, the AC-mix
is 0.5 and the BD-mix is also 0.5.  Figure B.6  shows the joystick roughly three-quarters
of the way from the origin to the A corner. However, the mix blurb shows the value of
A to be 0.79, but the other three have dropped off substantially. With the joystick all the
way in the A corner, the ratios are {A, B, C, D} = {1, 0, 0, 0} and interestingly, as  Figure
B.7  demonstrates, when the joystick is halfway between A and B on the outer edge of the
diamond, the A/B ratio is 50/50 while the rest are 0. In  Figure B.7 , the AC-mix is 0.25
and the BD-mix is 0.75.
 By sweeping around the ﬁ eld, you can generate many different combinations. You could
use these combinations as mix ratios for something in your plug-in. Or, you could use them
to control any parameter or other aspect of the plug-in that you like. You are responsible
for mapping the 0.0 to 1.0 values to your plug-in’s parameters. The vector joystick does
not connect to any variable in your plug-in; like the assignable buttons, these passive
controls simply tell you when they’ve moved. When the joystick moves, a function called
joystickControlChange() is called.
More RackAFX Controls and GUI Designer  523
B
Vector Joystick
A
C
A
B
C
D
SINE
TRI
PITCH
SQUARE
D
A:0.79 C:0.01 B:0.09 D:0.11
JS Program
 Figure B.6:    The exponential mix ratios are evident in the vector joystick.
B
Vector Joystick
A
C
A
B
C
D
SINE
TRI
PITCH
SQUARE
D
A:0.50 C:0.00 B:0.50 D:0.00
JS Program
 Figure B.7:    This position results in the 50/50 mix of A/B only. AC-mix = 0.25, BD-mix = 0.75.
  /* joystickControlChange
  Indicates the user moved the joystick point; the variables are the relative
mixes     of each axis; the values will add up to 1.0
  B
  |
A
x
C
  |
  D
  The point in the very center (x) would be:
  fControlA = 0.25
  fControlB = 0.25
  fControlC = 0.25
  fControlD = 0.25
  AC Mix = projection on X Axis (0 -> 1)
  BD Mix = projection on Y Axis (0 -> 1)
  */

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
