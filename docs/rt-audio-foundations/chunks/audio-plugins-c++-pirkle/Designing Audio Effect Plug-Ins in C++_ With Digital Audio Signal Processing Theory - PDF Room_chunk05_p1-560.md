# Audio Plugins C++ (Pirkle) — Chunk 5/73

> Source: `Designing Audio Effect Plug-Ins in C++_ With Digital Audio Signal Processing Theory - PDF Room.pdf` · Pages ~1-560 · Extractor: pdfminer.six
> Target ≈ 2800 words, Overlap = 200 words · Actual = 3251

> Note: DSP textbook content for SpectralCanvasPro RT-audio knowledge base

---
Digital Audio
Signal Processing Principles
    The ﬁ rst affordable digital audio devices began appearing in the mid-1980s. Digital signal
processing (DSP) mathematics had been around since the 1960s and commercial digital
recordings ﬁ rst appeared in the early 1970s, but the technology did not become available
for widespread distribution until about 15 years later when the advent of the compact
disc (CD) ushered in the age of digital audio. Digital sampling refers to the acquisition
of data points from a continuous analog signal. The data points are sampled on a regular
interval known as the sample period or sample interval. The inverse of the sample period
is the sampling frequency. A compact disc uses a sampling frequency of 44,100 Hz,
producing 44,100 discrete samples per channel each second, with a sample interval of about
22.7 microseconds (µS). While digital sampling applies to many different systems, this book
is focused on only one of those applications: audio.
 During the course of this book, you will learn both DSP theory and applications. This is
accomplished by experimenting with your own DSP algorithms at the same time as you
learn the theory using the included software, RackAFX™. The goal is to understand how
the DSP algorithms translate into C11 code. The resulting plug-ins you write can be
used to process audio in real time. Because plug-ins are software variations on hardware
designs, it’s worth examining how the hardware systems operate, the typical audio coding
formats, and the algorithm building blocks you will be using to implement your own
software versions.
  1.1    Acquisition of Samples
 The incoming analog audio signal is sampled with an analog-to-digital converter (ADC
or A/D). A/Ds must accurately sample and convert an incoming signal, producing a valid
digitally coded number that represents the analog voltage within the sampling interval. This
means that for CD audio, a converter must produce an output every 22.7 mS.  Figure 1.1  shows
the block diagram of the input conversion system with LFP, A/D, and encoder.
 Violating the Nyquist criteria will create audible errors in the signal in the form of an
erroneously encoded signal. Frequencies higher than Nyquist will fold back into the
1
2  Chapter 1
 The  sampling theorem  states that a continuous analog signal can be sampled into discrete data
points and then reconstructed into the original analog signal without any loss of information—
including inter-sample fluctuations—if and only if the input signal has been band-limited so
that it contains no frequencies higher than one-half the sample rate, also known as the  Nyquist
frequency  or  Nyquist rate . Band-limited means low-pass filtered (LPF). Band-limiting the input
 signal prior to sampling is also known as adhering to the  Nyquist criteria .
spectrum. This effect is called aliasing because the higher-than-Nyquist frequencies are
encoded “in disguise” or as an “alias” of the actual frequency. This is easiest explained
with a picture of an aliased signal, shown in  Figure 1.2 .
Input
LPF
1
2
fs
ADC
Encoder
10011101
Bitstream
Output
 Figure 1.1:    The input conversion system ultimately results in
numerical coding of the band-limited input signal.
(a)  Nyquist
n
(b)  >Nyquist
n
 Figure 1.2:    (a) The Nyquist frequency is the highest frequency that can be encoded
with two samples per period. (b) Increasing the frequency above Nyquist but keeping the
sampling interval the same results in an obvious coding error—the aliased signal is the result.
Digital Audio Signal Processing Principles  3
 Once the aliased signal is created, it can never be removed and will remain as a permanent
error in the signal. The LPF that band-limits the signal at the input is called the  anti-aliasing
ﬁ lter . Another form of aliasing occurs in the movies. An analog movie camera takes 30 pictures
(frames) per second. However, it must often ﬁ lm objects that are rotating at much higher rates
than 30 per second, like helicopter blades or car wheels. The result is visually confusing—the
helicopter blades or car wheels appear to slow down and stop, then reverse directions and
speed up, then slow down and stop, reverse, and so on. This is the visual result of the high-
frequency rotation aliasing back into the movie as an erroneous encoding of the actual event.
   1.2    Reconstruction of the Signal
 The digital-to-analog converter (DAC or D/A) ﬁ rst decodes the bit-stream, then takes the
sampled data points or impulses and converts them into analog versions of those impulses.
The D/A output is then low-pass ﬁ ltered to produce the ﬁ nal analog output signal, complete
with all the inter-sample ﬂ uctuations. As with the A/D diagram, the decoder and D/A are
both inside the same device (a chip).  Figure 1.3  shows the conceptual block diagram of the
decoding end of the system.
 The output ﬁ lter is called the  reconstruction ﬁ lter  and is responsible for re-creating the
seemingly missing information that the original sampling operation did not acquire—all the
inter-sample ﬂ uctuations. The reason it works is that low-pass ﬁ ltering converts impulses into
smeared out, wiggly versions. The smeared out versions have the shape  f ( x ) 5 sin( x )/ x  which
is also called the sinc() function and somewhat resembles the proﬁ le of a sombrero, as shown
in  Figure 1.4 .
Input
10011101
Bitstream
Decoder
DAC
LPF
1
2
fs
Output
 Figure 1.3:    The digital bit-stream is decoded and converted into
an analog output using a low-pass ﬁ lter to reconstruct the analog signal.
Impulse
sin(x)
(x)
LPF
1
2
fs
 Figure 1.4:    The ideal reconstruction ﬁ lter creates a smeared output with a damped
oscillatory shape. The amplitude of the sin(x)/x shape is proportional to the
amplitude of the input pulse.
4  Chapter 1
LPF
1
fs
2
 Figure 1.5:    The sin(x)/x outputs of the LPF are summed together to reconstruct
the original band-limited input waveform; the inter-sample
information has been reconstructed.
 When a  series  of impulses is ﬁ ltered, the resulting set of sin( x )/ x  pulses overlap with
each other and their responses all add up linearly. The addition of all the smaller
curves and damped oscillations reconstructs the inter-sample curves and ﬂ uctuations
( Figure 1.5 ).
   1.3    Signal Processing Systems
  Signal processing systems  combine data acquisition devices with microprocessors to
run mathematical algorithms on the audio data. Those algorithms are the focus of this
book. Today’s plug-ins are descendents of stand-alone hardware effects processors which
are based on DSP chips. A DSP chip is a highly specialized processor designed mainly
to run DSP algorithms. A DSP would function poorly as a central processing unit (CPU)
for a personal computer because it only has a small but highly specialized instruction
set. DSP devices (or just “DSPs”) feature a core processor designed to multiply and
accumulate data because this operation is fundamental to DSP algorithms. Because this
process is repeated over and over, modern DSPs use pipelining to fetch the next data
while simultaneously processing the current data. This technique greatly improves the
efﬁ ciency of the system. A typical signal processing system consists of the following
components ( Figure 1.6 ):
•
•
•
•
•
   Data acquisition devices (A/D and D/A)
   A DSP chip
   Memory to store the algorithm program (program memory)
   Memory to store the algorithm data (data memory)
   A user interface (UI) with buttons, knobs, switches, and graphic displays
 Music synthesis devices are an embedded version of a CPU1DSP arrangement. The
microcontroller or microprocessor manages the UI and keyboard, while the DSP performs
the audio synthesis and effects algorithms. Practical implementations now involve multiple
DSPs, some of which are custom designed speciﬁ cally for sound synthesis. An example
block diagram is shown in  Figure 1.7 .
Input
ADC
Output
DAC
Digital Audio Signal Processing Principles  5
Data
Memory
Program
Memory
DSP
User  Interface
and
Control  Surface
 Figure 1.6:    A simple signal processing system. The algorithm in this case is
inverting the phase of the signal; the output is upside-down.
Output
DAC
DSP
Data
Memory
Program
Memory
Microcontroller
or
Microprocessor
User Interface
and
Control Surface
 Figure 1.7:    Block diagram of a synthesizer.
   1.4    Synchronization and Interrupts
 There are two fundamental modes of operation when dealing with incoming and outgoing
audio data: synchronous and asynchronous. In synchronous operation, all audio input and
output data words are synchronized to the same clock as the DSP. These are typically simple
6  Chapter 1
systems with a minimum of input/output (I/O) and peripherals. More-complex systems
usually involve asynchronous operation, where the audio data is not synchronized to the DSP.
Moreover, the audio itself might not be synchronous, that is, the input and output bit-streams
might not operate on the same clock. A purely synchronous system will be more foolproof,
but less ﬂ exible.
 An asynchronous system will almost always be interrupt-based. In an interrupt-based
design, the processor enters a wait-loop until a processor interrupt is toggled. The processor
interrupt is just like a doorbell. When another device such as the A/D has data ready to
deliver, it places the data in a predesignated buffer, and then it rings the doorbell by toggling
an interrupt pin. The processor services the interrupt with a function that picks up the data,
and then goes on with its processing code. The function is known as an  interrupt-service
routine  or an  interrupt handler . The interrupt-based system is the most efﬁ cient use of
processor time because the processor can’t predict the exact clock cycle when the data will
be ready at the input.
 Another source of interrupts is the UI. Each time the user changes a control, clicks a button,
or turns a knob, the updated UI control information needs to be sent to the DSP so it can alter
its processing to accommodate the new settings. This is also accomplished via interrupts and
interrupt handlers. The interrupt is sometimes labeled INT, INTR, or IRQ (interrupt request
line) in block diagrams or schematics.
   1.5    Signal Processing Flow
 Whether the processing is taking place on a DSP chip or in a plug-in, the overall processing
ﬂ ow, also known as the signal processing loop, remains basically the same. For a DSP chip,
the processes in the loop will be encoded in the program memory’s set of instructions, often
in a nearly pure-inline fashion for optimization of the processor’s cycles. For a plug-in, each
processing block more or less takes on the form of a function in code, allowing for maximum
ﬂ exibility.
 The processing loop in  Figure 1.8  consists of
•
•
•
•
•
•
   A one-time initialization function to set up the initial state of the processor and prepare
for the arrival of data interrupts
   An inﬁ nite wait-loop, which does nothing but wait for an interrupt to occur
   An interrupt handler which decodes the interrupt and decides how to process—or
 ignore—the doorbell
   Data reading and writing functions for both control and data information
   A processing function to manipulate and create the audio output
   A function to set up the variables for the next time around through the loop, altering
 variables if the UI control changes warrant it
Digital Audio Signal Processing Principles  7
Process
&
Create  Audio
Output  Data
Set  Up Variables
&  Data  for  Next
Loop
Write
Output
Data
Do  One-Time
Initialization
Wait  for  Data
Interrupt
INTR
Is Audio
INTR?
yes
Read  Input
Data
no
no
Is Control
INTR?
yes
Read
Control
Data
 Figure 1.8:    The ﬂ owchart for an audio signal processing system.
 For the sampling theorem to hold true, the audio data must be arriving and leaving on a strictly
timed interval, although it may be asynchronous with the DSP’s internal clock. This means
that when the DSP does receive an audio INTR it must do all of the audio processing and
handle any UI interrupts before the next audio INTR arrives in one sample interval. The
interrupt handling scheme is prioritized such that the audio data interrupt is the most
important. Thus, while servicing the audio data interrupt, the DSP might be set up to ignore
all other interrupts (except the reset interrupt) until the audio data is ﬁ nished processing.
   1.6    Numerical Representation of Audio Data
 The audio data can be numerically coded in several different ways. Basic digital audio
theory reveals that the number of quantization levels available for coding is found by
 Equation 1.1 .
  q 5 2N
(1.1)
 where N is the bit depth of the system.
 Thus, an 8-bit system can encode 2 8  values or 256 quantization levels. A 16-bit system can
encode 65,536 different values.  Figure 1.9  shows the hierarchy of encoded audio data. As a
system designer, you must ﬁ rst decide if you are going to deal with unipolar (unsigned) or
bipolar (signed) data. After that, you need to decide on the data types.

8  Chapter 1
Unipolar  or Bipolar
Integer
Fractional
Fixed-Point
Floating
Point
 Figure 1.9:    The hierarchy of numerical coding.
•
•
•
•
   Unipolar or  unsigned  data is in the range of 0 to 1Max, or 2Min to 0, and only has one
polarity (1 or 2) of data, plus the number zero (0).
   Bipolar or  signed  data varies from 2Min to 1Max and is the most common form today.
It also includes the number zero (0).
   Integer data is represented with integers and no decimal place. Unsigned  integer
 audio varies from 0 to 165,535 for 16-bit systems. Signed integer audio varies
from 232,768 to 132,767 for the same 16-bit audio. In both cases, there are 65,536
 quantization levels.
   Fractional data is encoded with an integer and fractional portion, combined as int.frac
with the decimal place separating the parts (e.g. 12.09).
 Within the fractional data subset are ﬁ xed- and ﬂ oating-point types. Fixed-point data ﬁ xes
the number of signiﬁ cant digits on each side of the decimal point and is combined as int-
sig-digits.frac-sig-digits . “8.16” data would have 8 signiﬁ cant digits before the decimal place
and 16 afterwards. Floating-point data has a moving mantissa and exponent which code the
values across a range predetermined by the Institute of Electrical and Electronics Engineers
(IEEE). The positive and negative portions are encoded in 2’s complement so that the addition
of exactly opposite values (e.g., 20.5 and 10.5) always results in zero.  Figure 1.10  reveals
how the quantization levels are encoded. The dashed lines represent the minimum and
maximum values.
 A fundamental problem is that the number of quantization levels will always be an even number
since 2  N   is always even. In bipolar systems, you would very much like to have the number zero
(0) encoded as the number 0. If you do that, then you use up one of your quantization levels for
it. This leaves an odd number of levels which cannot be split exactly in half. That creates the
anomaly you see in  Figure 1.10 —there are always more negative (2) quantization levels than
positive ones for bipolar coding. For the unipolar case, there is no value which exactly codes
the zero or midpoint level; in  Figure 1.10  it is midway between 127 and 128.
Digital Audio Signal Processing Principles  9
Fractional
Bipolar
16-Bit
Bipolar
8-Bit
Bipolar
8-Bit
Unipolar
+0.9999
+0.9998
32,767
32,766
127
126
255
254
0.0002
0.0001
0
–0.0001
–0.0002
2
1
0
– 1
–2
2
1
0
– 1
– 2
129
128
127
126
–0.9999
–1.0000
–32,767
–32,768
– 127
– 128
1
0
 Figure 1.10:    A comparison of several different types of data representations. The ﬂ oating-
point version is ﬁ xed for a range of 21.0 to 10.9999, though any range can be used.
 This slight skewing of the data range is unavoidable if you intend on using the number zero
in your algorithms, and that is almost guaranteed. In some systems the algorithm limits the
negative audio data to the second most negative value. This is because phase inversion is
common in processing algorithms, either on purpose or in the form of a negative-valued
coefﬁ cient or multiplier. If a sample came in with a value of 232,768 and it was inverted,
there would be no positive version of that value to encode. To protect against that, the
232,768 is treated as 232,767. The audio data that travels from the audio hardware adapter
(DSP and sound card) as well as that stored in WAV ﬁ les is signed-integer based. However,
for audio signal processing, we prefer to use ﬂ oating-point representation.
   1.7    Using Floating-Point Data
 In many audio systems, the DSP and plug-in data is formatted to lie on the range of 21.0 to
11.0 (which is simplifying the real range of 21.0 to 10.9999). In fact the plug-ins you code
in this book will all use data that is on that same range. The reason has to do with overﬂ ow.
In audio algorithms, addition and multiplication are both commonplace. With integer-based
numbers, you can get into trouble quickly if you mathematically combine two numbers that
result in a value that is outside the range of known numbers.
 Consider the 16-bit integer bipolar format on the range of 232,768 to 132,767. Most of the
values on this range, when multiplied together, will result in a product that is outside these
10  Chapter 1
limits. Addition and subtraction can cause this as well, but only for half the possible values.
However, numbers between 21.0 and 11.0 have the interesting property that their product
is always a number in that range. Converting an integer value to a fractional value along the
normalized range of 21.0 to 11.0 in an  N -bit digital audio system is easy, as is the reverse
conversion shown in  Equation 1.2 :
  Fraction 5
Integer
2N
 Integer 5 Fraction * 2N
(1.2)
 where N is the bit depth of the system.
   1.8    Basic DSP Test Signals
 You need to know the data sequences for several fundamental digital signals in order to begin
understanding how the DSP theory works. The basic signal set consists of
•
•
•
•
•
   Direct Current (DC) and step: DC is a 0Hz signal
   Nyquist
   ½ Nyquist
   ¼ Nyquist
   Impulse
 The ﬁ rst four of these signals are all you need to get a ballpark idea of the frequency response
of some basic DSP ﬁ lters. The good news is that all the sequences are simple to remember.
  1.8.1  DC and Step
 The DC or 0 Hz and step responses can both be found with the DC/step input sequence:
{…0, 0, 1, 1, 1, 1, 1, 1, 1, 1…}.
 This signal in  Figure 1.11  contains two parts: the step portion where the input changes from
0 to 1 and the DC portion where the signal remains at the constant level of 1.0 forever. When
DC
+ 1.0
Step
–1.0
 Figure 1.11     Representation of the DC/Step sequence.
