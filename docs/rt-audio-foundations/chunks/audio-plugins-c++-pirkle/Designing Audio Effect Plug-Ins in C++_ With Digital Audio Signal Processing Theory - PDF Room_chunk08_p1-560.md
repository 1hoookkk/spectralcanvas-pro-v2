# Audio Plugins C++ (Pirkle) — Chunk 8/73

> Source: `Designing Audio Effect Plug-Ins in C++_ With Digital Audio Signal Processing Theory - PDF Room.pdf` · Pages ~1-560 · Extractor: pdfminer.six
> Target ≈ 2800 words, Overlap = 200 words · Actual = 8000

> Note: DSP textbook content for SpectralCanvasPro RT-audio knowledge base

---
Digital Audio Signal Processing Principles  19
   1.16    Practical Mixing Algorithm
 A problem with mixing multiple channels of digital audio is the possibility of overﬂ ow or
creating a sample value that is outside the range of the system. You saw that by limiting
the bipolar fractional system to the bounds of 21.0 to 11.0, multiplication of any of these
numbers always results in a number that is smaller than either, and always within the same
range of 21.0 to 11.0. However, addition of signals can easily generate values outside the 61
limits. In order to get around this problem, N-channel mixing circuits incorporate attenuators
to reduce the size of the inputs, where the attenuation value is 1/N. When mixing two channels,
the attenuators each have a value of ½ while a three-channel mixer would have attenuators with
a value of 1/3 on each mixing branch. If all channels happen to have a maximum or minimum
value at the same sample time, their sum or difference will still be limited to 61.  Figures 1.26
and  1.27  show the generalized mixing and weighted-sum algorithms.
p(n)
a
∑
y(n) = ap(n) + bq(n)
q(n)
b
(a)
p(n)
0.5
∑
y(n) = 0.5p(n) + 0.5q(n)
q(n)
0.5
(b)
 Figure 1.26:    (a) Generalized mixer/summer with a separate coefﬁ cient on each
line and (b) a normalized mixer which will not overﬂ ow or clip.
∑
y(n) = ap(n) + bq(n) + cr(n) + ds(n)
p(n)
q(n)
r(n)
s(n)
a
b
c
d
 Figure 1.27:    An example of a weighted-sum algorithm; each sample
from each channel has its own weighting coefﬁ cient, a–d.

20  Chapter 1
 In the next chapter, you will be introduced to the anatomy of a plug-in from a software point
of view. In Chapters 6 through 14 , you will learn how DSP theory allows you to combine
these building blocks into ﬁ lters, effects, and oscillators for use in your own plug-ins.
     Bibliography
 Ballou, G. 1987.  Handbook for Sound Engineers , pp. 898–906. Indiana : Howard W. Sams & Co.
 Jurgens, R. K., ed. 1997.  Digital Consumer Electronics Handbook , Chapter 2. New York: McGraw-Hill.
 Kirk, R. and Hunt, A. 1999.  Digital Sound Processing for Music and Multimedia , Chapter 1. Massachusetts :
Focal Press.
 KORG, Inc. 1991.  KORG Wavestation SR Service Manual. Tokyo, Japan: KORG Inc .
 Limberis, A. and Bryan, J. 1993. An architecture for a multiple digital-signal processor based music synthesizer
with dynamic voice allocation.  Journal of the Audio Engineering Society ,  Preprint No. 3699 .
 Pohlmann, K. C. 2011.  Principles of Digital Audio , pp. 16–30. New York: McGraw-Hill.
 Stearns, S. D. and Hush, D. R. 1990.  Digital Signal Analysis , pp. 44–52. Englewood Cliffs, NJ: Prentice-Hall.

CHAP TE R 2

Anatomy of a Plug-In
    A plug-in is a software component that interfaces with another piece of software called the
client in order to extend the client’s capabilities in some way. For example, Internet browsers
use plug-ins that implement added functions like searching and text messaging. In computer
audio systems, a plug-in is usually an audio effect of some kind. However, a plug-in could
also implement an oscilloscope or frequency analyzer function. Synthesizer plug-ins extend
the client’s functionality by working as musical instruments.
 In order to start writing plug-ins, you need to know how the plug-in connects to and
communicates with the client. Windows ®  plug-ins are typically packaged as dynamic link
library, or DLL, ﬁ les. Apple ®  plug-ins are packaged in a  bundle  which is conﬁ gured as a
 component . The main difference between the two is in the lingo their designers use to describe
them. Rather then try to accommodate both DLL and component labels during this discussion,
we will just use DLL to describe both in conceptual form. Speciﬁ c programmatic differences
in the formats will be addressed and can be found on the website  www.willpirkle.com .
  2.1    Static and Dynamic Linking
 C++ compilers include sets of precompiled libraries of functions for you to use in your
projects. Perhaps the most common of these is the math library. If you try to use the sin()
method you will typically get an error when you compile stating that “sin() is not deﬁ ned.”
In order to use this function you need to link to the library that contains it. The way you do this
is by placing #include <math.h> at the top of your ﬁ le. Depending on your compiler, you might
also need to tell it to link to math.lib. When you do this, you are statically linking to the math.h
library, a precompiled set of math functions in a .lib ﬁ le. Static linking is also called implicit
linking. When the compiler comes across a math function, it replaces the function call with the
precompiled code from the library. In this way, the extra code is compiled  into  your executable.
You cannot un-compile the math functions. Why would you do this? Suppose a bug is found
in the sin() function and the math.h library has to be re-compiled and redistributed. You would
then have to re-compile your software with the new math.h library to get the bug ﬁ x.
 The solution is to link to the functions at run time. This means that these precompiled
functions will exist in a separate ﬁ le, which your executable will know about and communicate
with, but only after it starts running. This kind of linking is called dynamic linking or explicit
21
22  Chapter 2
CLIENT  (.exe)
CLIENT  (.exe)
Calls
<math.h> functions
math.dll functions
Returns
(a) Static Linking
(b)  Dynamic Linking
 Figure 2.1:    (a) In static linking the functions are compiled inside the client.
(b) In dynamic linking the functions are located in a different ﬁ le.
This requires a communications mechanism between the
client and DLL to call functions and return information.
linking and is shown in  Figure 2.1 . The ﬁ le that contains the precompiled functions is the
DLL. It is more complicated because extra steps must be taken during run-time operation
rather than relying on code compiled directly into the executable. The advantage is that if a
bug is found in the library, you only need to redistribute the newly compiled ﬁ le rather than
re-compiling your executable. The other advantage is that the way this system is set up—a
client that connects to a component at run time—works perfectly as a way to extend the
functionality of a client without the client knowing anything about the component when it is
compiled. This is simpliﬁ ed when the DLL is loaded into the same virtual address space as the
client. If you already understand this, you can skip the next section; otherwise, read on.
   2.2    Virtual Address Space and DLL Access
 When you start a program (client) in a Windows 32-bit operating system (OS), the ﬁ rst
thing that happens is the OS creates a new virtual machine and virtual address space. The
maximum amount of memory that can be accessed by a 32-bit microprocessor is 2 32  or
about 4 gigabytes of memory. The client executable believes it has a full 4 gigabytes of
memory and that its executable is loaded into part of this memory block. This block of
memory is also called the process address space. The OS is responsible for maintaining this
illusion for the client as well as all the other executables that are running on the system. This
is done through virtualization of the microprocessor, memory, and peripherals.
Anatomy of a Plug-In  23
 While that topic could be the subject of another book, the main thing you need to know is
that typically when the client loads the DLL and begins the communication process, the
DLL is loaded into the same virtual address space as the client. This means that the client
code might as well have the DLL code compiled into it since the addressing requires no
translation. It should be noted that a DLL  can  be loaded into another process space or even on
another machine across a network. In this case it is called an “out of process DLL”; however,
inter-process communication is complicated and requires OS and/or network function calls.
We will not be considering out of process DLLs here. With the DLL in the same process
address space as the client, there is no extra overhead and the communication uses in-process
addressing. Both types are shown in  Figure 2.2 .
 In order to use the code in a DLL the client must perform two activities:
1.
2.
   Load the DLL into the process address space.
   Establish the communication mechanism for functions to be called.
Process Address  Space A
Process Address Space B
OxFFFFFFFF
The  Process A
CLIENT (A.exe)
The Process B
CLIENT (B.exe)
plugin_2.dll  functions
4 gigabytes
plugin_1.dll  functions
0x00000000
(a)  An  in-process  DLL
(b)  An  out-of-process  DLL
 Figure 2.2:    The process address space is a 4-gigabyte memory map starting at address
0x00000000 and going to 0xFFFFFFFF. When the client is launched it is placed inside the
memory space. (a) An in-process DLL communicates directly with the client while
(b) an out-of-process DLL communication requires OS intervention.
24  Chapter 2
 In the Windows OS, the three functions that are used to complete these tasks are
1.
2.
3.
   LoadLibrary(): Loads the DLL into the process address space.
   GetProcAddress(): Retrieves pointers to functions in the DLL.
   FreeLibrary(): Unloads the DLL from the process address space.
   2.3    C and C++ Style DLLs
 A DLL written in the C programming language consists of a set of stand-alone functions.
There is no  main () function. The functions can be deﬁ ned and implemented in one .c ﬁ le or
can be broken into an interface ﬁ le (.h) and implementation ﬁ le (.c)—either way, the DLL
performs a set of isolated functions. A problem with using the C programming language to
write a DLL is the persistence of data. In C (and C++) the curly brackets {} deﬁ ne the  scope
of a function.
 A fundamental problem is that the data declared inside a function cannot persist from one
call to the next. One solution involves using global variables, which is generally frowned
upon. A better solution to this problem is for the DLL to dynamically declare a data structure
(e.g., using malloc()) that will hold all the persistent variables and then pass a pointer to this
data structure back to the client to maintain. During subsequent calls to the DLL, the client
passes the pointer back to the DLL as a function argument so that it may operate on the
persistent data. This is shown in  Figure 2.3 . When the DLL is no longer needed, it clears the
memory by deleting the structure.
 In the C++ programming language, the  class  data type deﬁ nes an object that is a collection
of member variables and member functions which can operate on those variables. By
packaging a plug-in as a C++ class, you get several advantages; ﬁ rst, all of the beneﬁ ts of
C++ (encapsulation, polymorphism, etc.) are available during the coding process. Second,
rather than allocating a data structure and returning a pointer to it, the DLL can create a new
instance of the plug-in object and pass a pointer to the  object  to the client. Now, the client can
simply call functions on the object—it does not have to communicate with the DLL again
until it is time to either unload the DLL or, better yet, create another instance of the plug-in
object. This leads to a third advantage over the C-based DLL: the client can create multiple
plug-ins easily. The DLL can serve up multiple instances of the object. Sometimes, the
plug-in is referred to as a  server  and this becomes another kind of client-server system. This
is illustrated in  Figure 2.4 .
 Any variable declared inside a function, after the first open curly bracket { is only defined
for the duration of the function. After the closing curly bracket } is encountered, the variable
ceases to exist.
Anatomy of a Plug-In  25
CLIENT  (.exe)
SERVER  (.dll)
customDataStruct  *pData;
Call  DLL init()
init()
pData  =  new customDataStruct
CLIENT  (.exe)
SERVER  (.dll)
customDataStruct  *pData;
do_something(pData);
do_something(pData)
x = pData->oldSample ;
etc...
bSuccess
 Figure 2.3:    In a C-style DLL, the client ﬁ rst calls an initialization function
and the DLL returns a pointer to a dynamically allocated data structure
(pData), which the client stores. Later, the client calls a function
do_something() and passes the pointer back to the DLL as a
function argument. The DLL accesses the stored data, uses it,
and then typically responds with a success ﬂ ag.
   2.4    Maintaining the User Interface
 Most plug-ins have an associated graphical user interface (GUI or UI) with controls for
manipulating the device. There are several schemes, but in general, when a new instance of
the plug-in is created, a new child window is created with the GUI embedded in it. Whenever
a GUI control changes state, a function on the plug-in is called. The client or GUI passes the
plug-in information about which control changed and the new value or state of the control.
The plug-in then handles the message by updating its internal variables to affect the change in
signal processing. Generally, the GUI appearance (the position of the sliders or knobs or the
states of switches) is controlled automatically by the client or the GUI code itself. There are
three different ways the GUI can be handled:
1.
2.
3.
   The client creates, maintains, and destroys the GUI and forwards control-change
 messages to the plug-in.
   The client creates, maintains, and destroys the GUI but the GUI communicates directly
with the plug-in.
   The plug-in creates, maintains, destroys, and communicates directly with the GUI,
 independent of the client.
26  Chapter 2
CLIENT  (.exe)
CPIugln *pPlugln;
SERVER (.dll)
Call create()
create()
Return new  customPlugln;
SERVER (.dll)
SERVER (.dll)
CLIENT  (.exe)
CPIugln *pPlugln;
pPlugln->do_Something();
CLIENT (.exe)
CPIugln *pPlugln1;
CPIugln *pPlugln2;
pPlugln1->do_Something();
pPlugln2->do_Something();
 Figure 2.4:    In a C++ plug-in, the client calls a creation function and the DLL (server)
creates the object and passes a pointer to it back to the client. The client uses this
pointer for future calls to the plug-in without having to bother communicating
with the DLL. The client might also create multiple instances of the plug-in
and then use the resulting pointers to implement processing functions.
  Figures 2.5  through  2.7  show the three basic GUI scenarios. The ﬁ rst difference is in who
creates, maintains, and destroys the GUI. When the client creates and maintains the GUI, it
creates it as a child window which has the beneﬁ t that if the client is minimized or closed,
the child windows will be hidden or destroyed accordingly. Therefore, the ﬁ rst two scenarios
are more common. Some plug-ins can accommodate stand-alone GUI operation in addition
to the client-child conﬁ guration. The second difference is in how the communication ﬂ ows:
indirectly routed through the client or directly from the GUI to the plug-in. The RackAFX
software uses the second paradigm; the client creates the GUI but the GUI communicates
directly with the plug-in when controls are modiﬁ ed.
Anatomy of a Plug-In  27
Client
Message  Handler
CtrlChange(0xFCEE, 254)
Window  Control
Create,  Destroy
UI_Change(Slider 1, 0.75)
Plug-In
UI_Changed()
User  Interface
and
Control  Surface
 Figure 2.5:    In this scenario, the client maintains the GUI and receives control
change messages from it; the client then optionally translates that
information before forwarding it to the plug-in.
   2.5    The Applications Programming Interface
 In order for the client-server scheme to work, both the client and DLL/plug-in must agree on
the naming of the functions. This includes the creation function and all of the functions that
the client will be able to call on the plug-in object. The plug-in might have other functions
that the client doesn’t know about, but they must agree on a basic set of them. Additionally,
rules must be set up to deﬁ ne the sequence of function calls; the plug-in’s author (that’s you)
will need to understand how the client intends to use the object.
 The client must make sure that once it establishes these rules it adheres to them in future
versions to avoid breaking the plug-in. On the other hand, the plug-in must also promise to
implement the basic required functions to make the plug-in work. So, you can see that there
is an implied  contract  between the client and DLL server. This contract is the  applications
28  Chapter 2
Client
Window  Control
Create,  Destroy
Plug-In
User  Interface
and
Control  Surface
UI_Changed()
CtrlChange(0xFCEE.  254)
 Figure 2.6:    In this scenario, the client maintains the GUI,
which communicates directly with the plug-in.
Plug-In
UI_Changed()
Window Control
Create, Destroy
CtrlChange(0xFCEE, 254)
User Interface
and
Control Surface
 Figure 2.7:    In another scenario, the plug-in maintains and communicates
directly with the GUI without interference from the client.
Anatomy of a Plug-In  29
programming interface  or API. It is a deﬁ nition of the functions an object must implement
to be considered a proper plug-in as well as any additional functions that may be called
or overridden. It deﬁ nes the function prototypes and describes how the functions will be
called and used. The API is written by the client manufacturer and is made available to
programmers who want to create plug-ins for that target client. Some examples are Direct-X ® ,
VST ® , AU ® , and AAX ® . Each of these manufacturers publishes an API that describes the
contract with the plug-in developers. However, the basic operation and core functionality are
generally the same.
 C++ is especially useful here. Since the plug-in is an instance of a C++ object, the client
manufacturer can specify that the plug-in is a derived class of a special base class that it
deﬁ nes. The base class is made to be abstract, containing virtual functions that the derived
class overrides. These virtual functions provide the common functionality of the plug-in.
There are two options here:
1.
2.
   The manufacturer deﬁ nes the base class as abstract and then provides default
implementations of the virtual functions. Typically, the default implementations
do  nothing but return a success code. The plug-in authors then override whichever
methods they need to. For example, the plug-in might not care about responding
to Musical Instrument Digital Interface (MIDI)  messages, so the default implementation
of the MIDI function will sufﬁ ce.
   The manufacturer deﬁ nes the base class as a  pure   abstract  base class by making one or
more of the virtual functions  pure  virtual functions. A pure abstract base class cannot
be instantiated; only derived classes that implement all the pure virtual functions can.
This forms a binding contract between the plug-in developer and the client manufac-
turer since the derived class won’t work properly unless it implements the pure abstract
 functions that the client speciﬁ es.
 The RackAFX software uses the ﬁ rst method, supplying default implementations for all
virtual functions. As the plug-in author, you only override the functions you need to. But
what are the typical required functions and were do they come from?
   2.6    Typical Required API Functions
 Plug-ins are designed after the hardware devices that they replace. The audio processing
loop is the same as the hardware version you saw in  Chapter 1 .  Figure 2.8  shows a software
variation on that ﬂ owchart.
 Although the various plug-in APIs are different in their implementations, they share
a common set of basic operations.  Table 2.1  details the responsibility of each
function.
30  Chapter 2
Object Creation
Do One-Time
Initialization
Get Ready for
Audio Streaming
Wait for a
Function Call
Function  Called
Process
Audio?
ves
Read Input
Data
no
no
Control
Change?
yes
Read
Control
Data
Process
&
Create Audio
Output Data
Setup Variables
& Data for Next
Loop
Write
Output
Data
 Figure 2.8:    The software version of the DSP audio-processing loop. Most APIs also include
functions to get or set information about the controls.
  Table 2.1:    The typical core operations that plug-in APIs share.
Function
One-time initialization
Destruction
Prepare for streaming
Process audio
Get UI control info
Set UI control info
Get UI control value
Set UI control value
Responsibility
Called once when the plug-in is instantiated, this function implements any
one-time-only initialization, usually consisting of initializing the plug-in variables,
GUI, and allocating memory buffers dynamically.
Called when the plug-in is to be destroyed, this function de-allocates any memory
declared in the one-time initialization and/or in other functions that allocate
memory. If there are any owned child-windows, the plug-in destroys them here.
Called after the user has hit the play button or started audio streaming but before
the data actually ﬂ ows. This function is usually used to ﬂ ush buffers containing
old data or initialize any variables such as counters that operate on a per-play
basis (not found in some APIs).
The main function that does the actual signal processing. This function receives
audio data, processes it, and writes out the result. This is the heart of the plug-in.
Called to get information about a UI control—its name or label.
Called when the plug-in needs to change the control information like its name or
label.
Called to get the value for the UI control that will set its appearance on the
control surface.
Called when the user makes a change to the plug-in’s controls, this function is the
message handler to deal with the user input. It usually causes a change or update
in the plug-in’s internal variables.
Anatomy of a Plug-In  31
   2.7    The RackAFX Philosophy and API
 The fundamental idea behind the RackAFX software is to provide a platform for rapidly
developing real-time audio signal processing plug-ins with a minimum of coding, especially
with regard to the UI. In fact, most of the details of the connection between the RackAFX
plug-in and the RackAFX UI screen are hidden from the developer so that he or she may
concentrate more on the audio signal processing part and less on the UI details.
 The RackAFX API speciﬁ es that the plug-in must be written in the C++ language and
therefore takes advantage of the base class/derived class paradigm. The RackAFX API
speciﬁ es a base class called CPlugIn from which all plug-ins are derived.
(cid:129)
(cid:129)
(cid:129)
(cid:129)
(cid:129)
   RackAFX will automatically write C++ code for you to create a blank plug-in by creating
a derived class of CPlugIn.
   As you add and remove controls from the control surface, the RackAFX client will auto-
matically update your C++ code accordingly.
   This lets you focus on the signal processing and not the UI, making it a great tool for both
rapid plug-in development and for teaching how to write plug-ins.
   After learning RackAFX, you will be able to understand other companies’ APIs and learn
to write plug-ins in their formats quickly and easily.
   Because the plug-in objects you create are written in C++, you can easily move them
around between other APIs or computer platforms. You can wrap them to work easily in
other systems too.
 You only need to implement ﬁ ve functions in RackAFX to create a plug-in:
1.
2.
3.
4.
5.
   Constructor
   Destructor
   prepareForPlay()
   processAudioFrame()
   userInterfaceChange()
  Figure 2.9  shows where these functions ﬁ t into the real-time audio processing loop.
   2.7.1  __stdcall
 In the RackAFX code you will see the qualifier __stdcall preceding each function prototype as
well as implementation. The __stdcall calling convention is there for future compatibility with
other compilers as well as other third-party software. The __stdcall is a directive that lets the
compiler know how the stack will be cleaned up after function calls; it has no effect on the
math, logic, or audio processing, so you can essentially ignore it.
32  Chapter 2
Plug-In  Creation
Plug-In
Destruction
Constructor
prepareForPlay
Destructor
Wait for a  Function Call
processAudioFrame
Do Signal
Processing
userlnterfaceChange
 Figure 2.9:    The RackAFX C++ plug-in version of the
real-time audio processing loop in  Figure 2.8 .
 Here is part of the interface ﬁ le for the CPlugIn object plugin.h, which deﬁ nes the contract
or base class object interface for the primal six methods:
  /*
  */
  RackAFX(TM) Rapid Plug-In Development (RaPID) Client
  Applications Programming Interface
  Base Class Object Deﬁ nition
  Copyright(C) Will Pirkle 2002-2012
   In order to write a RackAFX Plug-In, you need to create a C++ object that is
  derived from the CPlugIn base class. Your Plug-In must implement the
constructor, destructor and virtual Plug-In API Functions below.
  // RackAFX abstract base class for DSP ﬁ lters
  class CPlugIn
  {
  public:
  // Plug-In API Member Methods:
  // The followung 5 methods must be impelemented for a meaningful Plug-In
  //
  // 1. One Time Initialization
   CPlugIn();
Anatomy of a Plug-In  33
  // 2. One Time Destruction
   virtual ~CPlugIn(void);
  // 3. The Prepare For Play Function is called just before audio streams
   virtual bool __stdcall prepareForPlay();
  // 4. processAudioFrame() processes an audio input to create an audio
output
   virtual bool __stdcall processAudioFrame(ﬂ oat* pInputBuffer,
   ﬂ oat* pOutputBuffer,
   UINT uNumInputChannels,
   UINT uNumOutputChannels);
  // 5. userInterfaceChange() occurs when the user moves a control.
   virtual bool __stdcall userInterfaceChange(int nControlIndex);
 The ﬁ ve functions in  Table 2.2  are the core RackAFX API—implement them and you
have a legitimate RackAFX plug-in. Best of all, the RackAFX plug-in designer will write
and provide default implementations of all these functions for you. You need only to go in
and alter them to change your plug-in’s behavior. See  Appendix A  for a comparison of the
RackAFX API and other commercially available formats as well as notes on using RackAFX
plug-in objects inside API wrappers for other formats.
RackAFX Function
CPlugIn ()
Parameters: •  none
~CPlugIn ()
Parameters: •  none
prepareForPlay()
Parameters: •  none
processAudioFrame()
Parameters:
•  pInputBuffer
•  pOutputBuffer
•  uNumInputChannels
•  uNumOutputChannels
  Table 2.2:    The RackAFX API core functions.
Remarks
The constructor for the plug-in object, this function is the one-time initialization
function.
The destructor for the plug-in.
Function is called just before audio begins streaming. The audio ﬁ le’s sample
rate, bit depth, and channel counts are extracted and then set by the client just
before calling this method.
The most important function in the API; this is where the audio processing is handled.
You might do all the processing in this function or use it to call  sub-functions. You are
responsible for writing the data to the output buffer via pOutputBuffer.
A pointer to one frame of audio input data. A frame is a set of channels as
deﬁ ned by uNumInputChannels.
A pointer to one frame of audio output data. A frame is a set of channels as
deﬁ ned by uNumOutputChannels.
The number of input channels in this frame of data. Currently, this value will be
either 1 (mono) or 2 (stereo).
The number of output channels in this frame of data. Currently, this value will be
either 1 (mono) or 2 (stereo).
userInterfaceChange()
Parameters:
•  nControlIndex
Function is called after the user changes a control in the RackAFX UI. RackAFX will
automatically update the variable linked to this control prior to calling this method.
The index of the control that was moved and whose value RackAFX has changed.
34  Chapter 2
     Bibliography
 Apple Computers, Inc. 2011.  The Audio Unit Programming Guide .   https://developer.apple.com/library/
mac/#documentation/MusicAudio/Conceptual/AudioUnitProgrammingGuide/Introduction/Introduction.html.
Accessed August 7, 2012.
 Bargen, B. and Donnelly, P. 1998.  Inside DirectX , Chapter 1. Redmond, WA: Microsoft Press.
 Coulter, D. 2000.  Digital Audio Processing , Chapters 7–8. Lawrence, KS: R&D Books.
 Petzold, C. 1999.  Programming Windows , Chapter 21. Redmond, WA: Microsoft Press.
 Richter, J. 1995.  Advanced Windows , Chapters 2 and 11. Redmond, WA: Microsoft Press.
 Rogerson, D. 1997.  Inside COM , Chapters 1–2. Redmond, WA: Microsoft Press.
  Steinberg.net .  The Steinberg VST API .  http://www.steinberg.net/nc/en/company/developer/sdk_download_portal.
html. (Note: you must create a free developer's account to download the API.) Accessed August 7, 2012.
   CHAP TE R 3
Writing Plug-Ins with
RackAFX
    The RackAFX plug-in designer will help you write your plug-in. When you create a new
RackAFX project, it will set up a new Visual C11 project folder for you and populate your
project with all the ﬁ les you will need. It will automatically create a new derived class based
on the name of your project. When you set up graphical user interface (GUI) controls like
sliders and buttons, it will write and maintain the code for you. You will be switching back
and forth between RackAFX and your C11 compiler. There are buttons on the RackAFX
GUI that will let you jump to the compiler as well as launch compiler functions like
rebuilding and debugging. You will use RackAFX to maintain your GUI and your compiler to
write the signal processing code.
  3.1    Building the DLL
 RackAFX sets up your compiler to deliver your freshly built dynamic link libraries (DLL)
to the /PlugIns folder in the RackAFX application directory. If you ever want to see, move,
or delete your DLL you can ﬁ nd this folder by using the menu item PlugIn > Open PlugIns
Folder. Each time you create a new project, RackAFX creates a pass-through plug-in by
default; you are urged to build and test the DLL right after creating the new project to check
your audio input/output (I/O) and any other connections. You then write over the pass-through
code with your own processing algorithm.
 After a successful build, you use RackAFX to test and debug the plug-in. You tell RackAFX
to load the DLL and create your plug-in. The client needs to handle four basic operations
during the lifecycle of your component:
1.
2.
3.
4.
   Creation of the plug-in
   Maintaining the UI
   Playing and processing audio through the plug-in
   Destruction of the plug-in
35
36  Chapter 3
RackAFX Client
Call creation method
CPIugln* pPlugln
Can now use pPlugln to
call  methods on your
object
.DLL
createObject()
Return new yourPlugln;
pYourPlugln
Constructor
initUI()
 Figure 3.1:    The new operator in createObject() dynamically creates your
plug-in, which calls your constructor; the constructor in turn calls initUI()
to create and initialize the user controls.
   3.2    Creation
 When you load a plug-in in RackAFX, you are actually passing the system a path to the DLL
you’ve created. RackAFX uses an operating system (OS) function call to load the DLL into its
process space. Once the DLL is loaded, RackAFX ﬁ rst runs a compatibility test, then requests
a pointer to the creation method called createObject(). It uses this pointer to call the method
and the DLL returns a newly created instance of your plug-in cast as the PlugIn*  base class
type. From that point on, the RackAFX client can call any of the base class methods on your
object.  Figure 3.1  shows the ﬂ ow of operation during the creation phase.
 Your constructor is where all your variables will be initialized. The very ﬁ rst line of code
in the constructor has been written for you; it calls initUI(), which is a method that handles
the creation and set up of your GUI controls. You never need to modify the initUI() method;
RackAFX maintains this code for you.
   3.3    The GUI
 When you set up GUI elements such as sliders and buttons, RackAFX adds member variables
to the .h ﬁ le of your derived plug-in class. Each slider or button group controls one variable
in your code. You set up each control with minimum, maximum, and initial values as well as
supplying the variable name and data type. As the user moves the control, RackAFX calculates
the variable’s new value and delivers it to your plug-in automatically, updating it in real time.
In some cases, this is all you will need and there is nothing left to write. In other cases, you will
ID = 2
Control
123.45
RackAFX Client
Calculate new variable value
Writing Plug-Ins with RackAFX   37
yourPlugln
Update in plug-in
(Hidden)
controlVariable
Notify  plug-in that
control with  ID = 2 has changed
userlnterfaceChange(2)
 Figure 3.2:    The sequence of events when the user moves the control with ID = 2 starts with a
hidden change to the underlying linked variable, followed by a call to userInterfaceChange()
passing the control’s ID value as the parameter.
need to perform more calculations or logic processing in addition to just changing the control
variable. So, in addition to changing and updating your internal GUI variable, RackAFX will
also call the userInterfaceChange() method on your plug-in, shown in  Figure 3.2 .
   3.4    Processing Audio
 When the user loads an audio ﬁ le and hits the Play button, a two-step sequence of events
occurs. First, the client calls prepareForPlay() on the plug-in. The plug-in will do any last
initializations it needs before audio begins streaming. prepareForPlay() is one of the most
important functions to deal with. Your plug-in has variables declared in it (see PlugIn.h) that
contain information about the currently selected audio ﬁ le:
  // information about the current playing-wave ﬁ le
  int m_nNumWAVEChannels;
  int m_nSampleRate;
  int m_nBitDepth;
 Just prior to calling prepareForPlay(), the client sets these values on your plug-in object.
The reason this is done at this point is that the user can load multiple audio ﬁ le types of
varying channels (mono or stereo), sample rates, and bit depths at any time; thus, this
is a per-play method. Many algorithms require these values to be known before certain
things can be created or initialized. Almost all ﬁ ltering plug-ins require the sample rate in
order to calculate their parameters correctly. After prepareForPlay() returns, audio begins
streaming. When audio streams, the client repeatedly calls processAudioFrame(), passing
it input and output buffer pointers as shown in  Figure 3.3 . This continues until the user hits
Pause or Stop.
38  Chapter 3
Play
RackAFX Client
yourPlugln
Load  audio file-specific data
Update in  plug-in
(Hidden)
m_nSampleRate
m_nBitDepth
m_nNumWAVEChannels
Call  prepareForPlay()
prepareForPlay()
Input
Audio
file
data
Enter audio processing loop
Input
Output
processAudioFrame()
Audio interface
 Figure 3.3:    The sequence of events during the play/process operation; audio data from the ﬁ le is
processed in the plug-in and sent to the audio adapter for monitoring.
   3.5    Destruction
 When the user unloads the DLL either manually or by loading another plug-in, the client ﬁ rst
deletes the plug-in object from memory, which calls the destructor. Any dynamically declared
variables or buffers need to be deleted here. After destruction, the client unloads the DLL
from the process space.
   3.6    Your First Plug-Ins
 You can break the audio processing down into two very fundamental types:
1.
2.
   Processing that only works on the current audio sample; requires no memory elements
   Processing that requires the current  and  past audio samples; requires memory elements
Writing Plug-Ins with RackAFX   39
 We’ll start with the ﬁ rst type and make a simple volume control. After that, we’ll design a
simple Audio Equalizer (EQ)  control that will require memory elements. You will need the
following installed on your computer:
•
•
•
   RackAFX
   Microsoft Visual C11 Express® 2008 or 2010 (both are free from Microsoft)
   Microsoft Visual C11 Professional® 2008 or 2010
 There is no advantage to having the full version of Visual C11 (aka VC11) in RackAFX
programming unless you plan on using your own GUI resources. Make sure that Visual
C11 is installed on the same machine as RackAFX. See the website  www.willpirkle.com
for updates on supported compiler platforms. Once you understand the ﬂ ow of writing and
testing your plug-ins, you will ﬁ nd that you can move easily and swiftly through the rest of
the book’s projects because they all follow the same design pattern and the design chapters
will use the same conventions for each project.
  3.6.1  Project: Yourplugin
 The ﬁ rst step will always be the creation of a new project. In this phase, RackAFX creates the
C11 project directory and ﬁ les along with a derived class based on the project name.
   3.6.2  Yourplugin GUI
 Next, you lay out your GUI controls based on the algorithm you are following and decide
on the variable data types and names that will connect the GUI elements to your plug-in.
This generally starts with writing the difference equation(s) for the algorithm. Variables in
the difference equation will map to member variables and GUI controls in your plug-in.
Abstracting the GUI from the algorithm requires that you decide which parameters you want
the user to be able to adjust, as well as the variable names, min, max, and initial values,
and data types. You can change your mind later and remove GUI elements or alter their
parameters. A full-featured GUI designer allows you to ﬁ nalize your plug-in’s controls and
package them in a neat and clean GUI. Often during the prototyping phase, you set up many
sliders and controls and work on the voicing of the plug-in. Then, you hide some of them for
the ﬁ nal product, only allowing the user access to a speciﬁ c set of controls over a speciﬁ c set
range of values. This last step is part of the voicing of the ﬁ nal product.
   3.6.3  Yourplugin.h File
 The next phase involves adding your own member variables and member methods to the
derived plug-in class. The variables and methods will depend on the algorithm you are
implementing. In the ﬁ rst plug-in there are no extra variables or methods to supply.
40  Chapter 3
   3.6.4  Yourplugin.cpp File
 In this step, you will add the new member method implementations (if you have any). Then, you
will step through the .cpp ﬁ le, altering and ﬁ lling in the base class methods, typically in this order:
1.
2.
3.
4.
   Constructor
   prepareForPlay()
   userInterfaceChange()
   processAudioFrame()
 Once these methods have been implemented, you will technically have a working plug-in.
   3.6.5  Building and Testing
 Finally, you will build the DLL, ﬁ nding and ﬁ xing any issues. After the build succeeds, you
can load it into the RackAFX client. You can use audio ﬁ les, the oscillator, or your sound
adapter input as the audio stimulus for your plug-in. You can run your plug-in in three modes:
Insert, Mono > Stereo AUX, or Stereo > Stereo AUX to mimic the various ways a plug-in is
used in a digital audio workstation  (DAW).
   3.6.6  Creating and Saving Presets
 The presets are created and maintained on the main RackAFX UI. After you load your plug-in
you can move the controls as you like and then save them as a preset. You use the Save Preset
button on the toolbar. The presets will be saved inside a ﬁ le until the next time you compile
your plug-in; after that, the presets will be built into the DLL. You can add, modify, or delete
the presets any time the plug-in is loaded.
   3.6.7  GUI Designer
 Once you have debugged and ﬁ nished your plug-in, you can optionally use the GUI designer
to create a compact, visually appealing GUI. See  Appendix B  and the website  www.
willpirkle.com  for the latest details on using the GUI designer tools. In general, the book
skips the GUI designer phase because it is so open ended; you are free to layout your ﬁ nal
GUI however you wish. Check the website often for the latest news and example GUIs as
well as video tutorials.
    3.7    Design a Volume Control Plug-In
 The easiest meaningful plug-in to write is a volume control, which uses a single scaling
variable, a 2  , depicted in  Figure 3.4 .
Writing Plug-Ins with RackAFX   41
x(n)
a2
y (n) = a2x(n)
 Figure 3.4:    The volume control
block diagram.
  Coefficients  in a block diagram (or transfer function or algorithm) become  float member variables
in your plug-in code.
•
•
   a 2  5 0: Mute
   a 2  5 1.0: Max volume
 The output samples  y ( n ) are a scaled version of the input  x ( n ) and the scaling factor is named
a 2 . a 2  is called a  coefﬁ cient  in the algorithm. The algorithm states that a 2  will vary between
0 (mute) and 1.0 (full volume).
   3.8    Set up RackAFX for Use
 Start the RackAFX software. You will start in prototype view, where you will see a blank
control surface as shown in  Figure 3.5 . Your GUI may look slightly different or have different
background images.
 The control surface is what you use to create your UI. It is full of assignable controls you can
connect to your plug-in’s variables. The surface consists of:
•
•
•
•
•
•
•
•
•
   40 assignable sliders (continuous controls).
   Universal LCD control with 1024 more continuous controls.
   Project controls (open, load, edit, rebuild, debug, jump to C11).
   Vector joystick (advanced, see website for more details).
   Assignable buttons.
   10 assignable LED meters.
   Plug-in routing controls to test insert and aux effect modes.
   Prototype tab, the main GUI.
   GUI designer tab, which opens the designer for editing; you must have GUI controls
declared ﬁ rst. See the website for more details.
 The menu and toolbar consist of two parts: the left and right side. The left side ( Figure 3.6 )
implements the majority of the software functionality, while the right side ( Figure 3.7 )
maintains lists.
42  Chapter 3
1
2
3
4
5
6
7
8
9
 Figure 3.5:    When you start RackAFX, it opens in prototype view. It features the control surface
and plug-in routing controls.
1
2
3
4
5
6
7
8
 Figure 3.6:    The menu and toolbar on the left handle most of your
plug-in development.
Preset List
MIDI Input List
Audio File List
 Figure 3.7:    The dropdown boxes on the right let you store and recall presets,
choose a MIDI input controller (advanced), and keep track of the audio ﬁ les
you have been using.
Writing Plug-Ins with RackAFX   43
Open
Load
Rebuild
Debug
Edit
C++
 Figure 3.8:    The Project/Compiler buttons make it easy to
work with projects and control your compiler.
 The menu items include:
•
•
•
•
•
•
•
   File: Manage projects by creating, editing or clearing the project.
   Modules: Built-in plug-ins that you can use for analysis and testing.
   User plug-ins: Each new plug-in you design gets loaded into this menu; you can audition
or show off your plug-in in a standalone fashion.
   Audio: Manage all audio commands.
   Plug-in: Tools for loading/unloading and presets.
   View: Access the different windows.
   Help: Help information.
 The toolbar items include:
1.
2.
3.
4.
5.
6.
7.
8.
   New project, open project folder, open audio ﬁ le
   Set up low-level audio
   Audio input mode: File, analog audio input, oscillator, user oscillator/synth
   Transport controls: Play, loop, pause, stop, bypass
   Additional windows: Analyzer, block diagram, status window
   Plug-in tools: Synchronize code, load, reset, unload
   GUI windows: Custom GUI, RackAFX MIDI Piano
   Presets: Save, delete
 Finally, there is a bank of buttons that allow you to manipulate your projects as well as
control the C11 compiler shown in  Figure 3.8 . The buttons are set up as follows:
•
•
•
•
•
•
   Open: Open an existing project.
   Load: Load/unload the DLL from process space.
   Edit: Change an existing project’s settings.
   Rebuild: Rebuild the project.
   Debug: Launch the debugger.
   ->C11: Jump to the C11 compiler and restore if minimized.
   3.9    Setup Preferences
 Before you start working on projects, take some time to conﬁ gure your preferences. This is
where you will choose your C11 compiler and set your default directories. Choose View >
Preferences to get the interface shown in  Figure 3.9 .
44  Chapter 3
1
2
3
4
 Figure 3.9:    The preferences interface.
 In the preferences you need to:
1.
2.
3.
4.
   Choose your default folders for projects, WAVE ﬁ les, and default WAVE ﬁ les. You can
use whatever directory you want for your project folder and you can also open projects
from any other folder at any time; the default is simply for conveniently grouping all your
projects together.
   Choose a Visual C11 compiler.
   Set the C11 options. Enable C11 Control/Switching should be left on for all but the
most advanced users. C11 control/switching allows RackAFX to control Visual Studio,
save ﬁ les, launch the debugger, and so on.
   Set up the edit options when entering information in the GUI slider/button dialogs.
  3.9.1  Project: Volume
 Create a new project with the menu, toolbar, or Ctrl1N and name it “Volume.” The New/Edit
Project window will appear and you can enter your project name. As you enter the name, the
.h and .cpp ﬁ les will be automatically named.
 Notice the two C11 ﬁ les listed in  Figure 3.10 —these are the interface (.h) and implementation
(.cpp) ﬁ les that RackAFX will create for you. They will contain a derived object named
CVolume which will contain the bulk of your plug-in code. When you hit OK, several things
happen. If you have C11 Control enabled in your preferences (it’s the default) then you will
Writing Plug-Ins with RackAFX   45
New project  name
C++ files for your derived class
 Figure 3.10:     The top section of the New/Edit Project window. Notice that your
project name becomes the name of a C11 object, so you will receive errors if you
name the project in a way that produces illegal C11 syntax. Below this are more
advanced settings that we will cover later.
see your C11 compiler start up. In Visual C11 you will see a new project and solution
named “Volume.” If you expand the Volume project then you can see the ﬁ les that RackAFX
wrote for you. Your derived class is contained in Volume.h and Volume.cpp. Before continuing,
it’s worth taking a peek into the RackAFXDLL.cpp ﬁ le and locating the creation mechanism
createObject():
  //RackAFX Creation Function
   DllExport CPlugIn* createObject()
   {
           CPlugIn* pOb = new CVolume; // ***
           return pOb;
   }
 This is the method that the client calls on your DLL—you create the plug-in object with the
 new  operator and return the pointer to the client. The DllExport speciﬁ er is OS-speciﬁ c for
calling a method in a DLL.
   3.9.2  Volume GUI
 You need to make sure you know the difference equation for each algorithm you want
to implement. The difference equation relates the input and output samples and is what
you are going to convert to C11 in order to make your plug-in. For this example, the
equation is
 y(n) 5 a2x(n)
(3.1)
