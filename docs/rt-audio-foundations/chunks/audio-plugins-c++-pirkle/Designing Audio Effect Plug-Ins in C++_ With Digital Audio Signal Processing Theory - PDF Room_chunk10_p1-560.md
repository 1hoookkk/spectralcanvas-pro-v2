# Audio Plugins C++ (Pirkle) — Chunk 10/73

> Source: `Designing Audio Effect Plug-Ins in C++_ With Digital Audio Signal Processing Theory - PDF Room.pdf` · Pages ~1-560 · Extractor: pdfminer.six
> Target ≈ 2800 words, Overlap = 200 words · Actual = 1454

> Note: DSP textbook content for SpectralCanvasPro RT-audio knowledge base

---
46  Chapter 3
 Each slider or button control on the UI will map to and control a  member variable  in your plug-in.
 In RackAFX, you can see that all the sliders and buttons are disabled; the sliders don’t move
and the edit boxes won’t accept text. You ﬁ rst need to set up some controls to create your UI
or control surface.
 Now, decide how to handle the difference equation. Notice the use of Hungarian notation for
handling the variable names. See the website for more information if you are not familiar
with this kind of naming convention—you are certainly free to name your variables whatever
you like.
•
•
•
•
•
•
•
   Let’s have only one volume coefﬁ cient and share it between the channels so that each
channel has the same volume control.
   Let’s have one slider control the volume coefﬁ cient.
   The coefﬁ cient a 2  will become a ﬂ oat member variable in the code; let’s decide to name it
“m_fVolume.”
   We’ll name the slider “Volume” on the GUI and link it to the variable m_fVolume.
   Slider minimum 5 0.
   Slider maximum 5 1.0.
   Initial setting 5 0.75.
   3.9.3  Conﬁ gure a Slider Control
 Choose the ﬁ rst slider in the upper left and right-click just inside the outer bounding box; a
slider properties box appears ( Figure 3.11 ). Note: Your dialog may look slightly different.
 You need to ﬁ ll out the slider properties with the proper values. You will notice that the
uControlID value is 0 for this slider. This is the ID number that will link the slider to a
Control 0
Right-click
 Figure 3.11:     Right-click inside the bounding box of a slider and the slider properties
window appears. This is how you conﬁ gure the slider and link it to a variable.
Writing Plug-Ins with RackAFX   47
variable in the object. You cannot edit this cell. Start with the control name and enter
“Volume.” Hit Enter to advance to the next cell. For this version of the plug-in there are no
units, so use backspace to delete it. The next cell is one of the most important—it is the data
type for the variable that the slider will be linked with; the choices are available from a drop-
down list. As shown in  Figure 3.12 , you can select the data type with the mouse, or you can
just type the ﬁ rst letter (e.g., “d” for double) while the box is highlighted.
 You can create variables of the following types:
•
•
•
•
   ﬂ oat
   double
   integer
   enum: An enumerated Unsigned Integer (UINT)  using a list of strings for the enum, for
example {LPF, HPF, BPF, Notch}. We will work with enumerated UINTs later in the book.
 We decided to use the  ﬂ oat  data type and to name the variable m_fVolume—it is really
important to decide on a name and stick with it. Changing the variable name later can be
tricky, so do yourself a favor and plan ahead. The completed slider properties are shown in
 Figure 3.13 . The low and high limits are set and the initial value is set to 0.75. Do not worry
about the MIDI control or other properties for this slider; see the website for details on these
enhancements. After you are ﬁ nished, choose the OK button to save the code.
 Our control surface is pretty simple—it just consists of a single volume slider. If you realize
you’ve made a mistake or left something out, just right-click in the slider box and ﬁ x the problem.
You can also remove the slider by clicking the Remove Ctrl button on the properties window.
The dropdown list exposes
the choices for data type
 Figure 3.12:     The data type is selected from a dropdown list control.
I
48  Chapter 3
UI Item
uControiType
uControiiD
Control Name
Units
Data Type
Variable Name
Control Low Limit
Control High Limit
Initial Value
MIDI Control
MIDI Control Channel
Property
Slider
0
Volume
float
m_fVolume
0.00
1.00
0.75
false
1
MIDI Control Type
Continous Controller
MIDI Controller Number/Name
3 Continuous controller #3
Control Method
Menu Group
automatic (recommended)
none
 Figure 3.13:     The completed slider properties.
 As you add, edit, or remove controls from the main UI you will notice that RackAFX will ﬂ ash to
the compiler and back as it writes the code for you. You might use this ﬂ ashing as a signal that
the code update is synchronized. If you don’t like it, minimize the compiler and the ﬂ ashing will
not occur. There is a special check-box in View > Preferences to start the compiler minimized for
this very reason.
 Your plug-in code will use the index value 0 (uControlID in the properties dialog) to map to
the m_fVolume variable, which is controlled by the slider named “Volume” on the UI.
   3.9.4  Volume.h File
 Before we add the code, look around the plug-in object ﬁ les (volume.h and volume.cpp) to
get a better understanding of what’s going on and what you’ll need to modify. First, open the
volume.h ﬁ le and look inside:
  // RackAFX abstract base class for DSP ﬁ lters
  class CVolume : public CPlugIn
  {
  public:  // plug-in API Functions
            //

Writing Plug-Ins with RackAFX   49
            // 1. One Time Initialization
            CVolume();
             <SNIP SNIP SNIP>
            // 7. userInterfaceChange() occurs when the user moves a control.
            virtual bool userInterfaceChange(int nControlIndex);
             // Add your code here: ------------------------------------------------ //
             // END OF USER CODE --------------------------------------------------- //
            // ADDED BY RACKAFX -- DO NOT EDIT THIS CODE!!! ----------------------- //
            //  **--0x07FD--**
             ﬂ oat m_fVolume;
            // **--0x1A7F--**
            // -------------------------------------------------------------------- //
  };
 Aside from the main plug-in functions we discussed in  Chapter 2 , you will see some more
commented areas of code. In the ﬁ rst part marked  // Add your code here:  you can add more
variables or function deﬁ nitions just like you would in any .h ﬁ le. Try to keep your code in
the denoted area to make it easier to ﬁ nd and read. The area below that says:
   // ADDED BY RACKAFX—DO NOT EDIT THIS CODE!!!
 is very important—you will see your member variable m_fVolume declared in this area. This
is the portion of the .h ﬁ le that RackAFX modiﬁ es when you add, edit, or delete controls from
your control surface. It is imperative that you let RackAFX maintain this part of the C11
code. There are several other portions of code in the .cpp ﬁ le that have similar warnings and
interesting hex symbols (0x1A7F, etc.); do not edit the code contained between the hex codes
or commented areas.
 You will see the notation  <SNIP SNIP SNIP>  frequently in the printed code as a reminder that
code has been cut out for easier reading.
 RackAFX writes C++ code for you! But, you have to be careful not to alter the RackAFX C++
code in any way. You can always tell if the code is RackAFX code because there will be warning
comments and strange hex codes surrounding the RackAFX code. The RackAFX code is left for
you to see only as a backup to your debugging and should never be altered by anyone except
RackAFX.

50  Chapter 3
 In this case, check to verify that RackAFX added the ﬂ oat member variable m_fVolume as you
anticipated. Next, move on to the volume.cpp ﬁ le and have a look at it, starting from the top.
   3.9.5  Volume.cpp File
  Constructor and destructor
 The constructor is the One-Time-Init function and is set up to:
•
•
•
•
   call initUI(): This is where your GUI controls are initialized; m_fVolume is initialized
to 0.75 inside this function. It is important to make sure this remains the ﬁ rst line of the
constructor so that your GUI variables are always initialized ﬁ rst.
   Set the plug-in name variable: This is what you will see in the user plug-in menu and on
the GUI windows.
   Set the plug-in defaults (snipped out here); you will rarely need to change these variables.
   Give you a place to ﬁ nish any of your own initializations at the end.
 The destructor is empty because nothing has been allocated dynamically in this plug-in.
   CVolume::CVolume()
  {
          // Added by RackAFX - DO NOT REMOVE
          //
          // Setup the RackAFX UI Control List and Initialize Variables
           initUI();
          // END InitUI
          // built in initialization
           m_PlugInName = "Volume";
          // Default to Stereo Operation:
          // Change this if you want to support more/less channels
           <SNIP SNIP SNIP>
           // Finish initializations here
  }
  /* destructor()
          Destroy variables allocated in the contructor()
  */
   CVolume::~CVolume(void)
  {
  }
   prepareForPlay()
 There is nothing to write yet since there are no custom controls or other allocations.
