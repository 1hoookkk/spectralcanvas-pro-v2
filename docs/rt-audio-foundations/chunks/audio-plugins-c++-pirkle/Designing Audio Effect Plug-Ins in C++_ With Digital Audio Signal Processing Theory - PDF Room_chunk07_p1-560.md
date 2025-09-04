# Audio Plugins C++ (Pirkle) — Chunk 7/73

> Source: `Designing Audio Effect Plug-Ins in C++_ With Digital Audio Signal Processing Theory - PDF Room.pdf` · Pages ~1-560 · Extractor: pdfminer.six
> Target ≈ 2800 words, Overlap = 200 words · Actual = 2741

> Note: DSP textbook content for SpectralCanvasPro RT-audio knowledge base

---
Digital Audio Signal Processing Principles  11
you apply this signal to your DSP ﬁ lter and examine the output, you will get two pieces of
information. The step portion will tell you the transient attack time and the DC portion will
give you the response at DC or 0 Hz.
   1.8.2  Nyquist
 The Nyquist input sequence represents the Nyquist frequency of the system and is
independent of the actual sample rate. The Nyquist sequence is {…21, 11, 21, 11, 21,
11, 21, 11…}.
 The Nyquist frequency signal in  Figure 1.12  is the highest frequency that can be encoded. It
contains the minimum of two samples per cycle with each sample representing the maximum
and minimum values. The two-sample minimum is another way of stating the Nyquist
frequency as it relates to the sampling theorem.
   1.8.3  ½ Nyquist
 The ½ Nyquist input sequence in  Figure 1.13  represents the ½ Nyquist frequency of the
system and is independent of the actual sample rate. The signal is encoded with four samples
+1.0
–1.0
 Figure 1.12     The Nyquist sequence.
+ 1.0
– 1.0
 Figure 1.13     The ½ Nyquist sequence has four samples per cycle.
12  Chapter 1
per cycle, twice as many as Nyquist. The ½ Nyquist sequence is {…21, 0, 11, 0, 21, 0, 11,
0, 21, 0, 11, 0, 21, 0, 11, 0…}.
   1.8.4  ¼ Nyquist
 The ¼ Nyquist input sequence in  Figure 1.14  represents the ¼ Nyquist frequency of the
system and is independent of the actual sample rate. It is encoded with eight samples
per cycle. The ¼ Nyquist sequence is {…0.0, 0.707, 11.0, 0.707, 0.0, 20.707, 21.0,
20.707, 0.0…}.
   1.8.5  Impulse
 The impulse shown in  Figure 1.15  is a single sample with the value 1.0 in an inﬁ nitely long
stream of zeros. The  impulse response  of a DSP algorithm is the output of the algorithm after
applying the impulse input. The impulse sequence is {…0, 0, 0, 0, 1, 0, 0, 0, 0,…}.
+1.0
–1.0
 Figure 1.14     ¼ Nyquist sequence has eight samples per cycle.
+1.0
–1.0
 Figure 1.15     The impulse is a single nonzero sample value in a sea of zeros.
Digital Audio Signal Processing Principles  13
    1.9    Signal Processing Algorithms
 In the broadest sense, an algorithm is a set of instructions that completes a predeﬁ ned task.
The signal processing loop in  Figure 1.8  is a picture of an algorithm for processing audio and
control (UI) data in real time. In the specialized case of audio signal processing, an algorithm
is a set of instructions that operates on data to produce an audio output bit-stream. Most of
the exercises in this book involve processing incoming audio data and transforming it into
a processed output. However, synthesizing a waveform to output also qualiﬁ es and in this
special case, there is no real-time audio input to process. Most of the plug-ins in this book
use the effects model, where an input sequence of samples is processed to create an output
sequence, as shown in  Figure 1.16 .
 Conventions and rules:
•
•
•
•
    x ( n ) is always the input sequence; the variable  n  represents the location of the  n th sample
of the  x -sequence.
    y ( n ) is always the output sequence; the variable  n  represents the location of the  n th
sample of the  y -sequence.
    h ( n ) is the impulse response of the algorithm; a special sequence that represents the
 algorithm output for a single sample input or impulse.
   For real-time processing, the algorithm must accept a new input sample (or set of
 samples), do the processing, then have the output sample(s) available before the next
input arrives; if the processing takes too long, clicks, pops, glitches, and noise will be
the real-time result.
   1.10    Bookkeeping
 You can see that there are already three sequences to deal with: the input, output, and
impulse response, all of which are coded with the same variable  n  to keep track of the
location of samples within the sequence. The ﬁ rst step is to decide how to use  n  to do
this bookkeeping task. Using it to represent the absolute position in the sequence would
quickly become tiresome—how do you deal with indexing numbers like  x (12,354,233)?
x(n)
Audio Signal
Processing Algorithm
h( n)
y(n)
 Figure 1.16:    An audio signal processing algorithm that converts
an input sequence x(n) into an output sequence y(n).
14  Chapter 1
x(3)
x(2)
The first input
sample
x(0)
x(1)
The  12th input
sample
x(11)
x(12)
x(10)
 Figure 1.17:    Using the absolute position within the sequence is one way to keep track,
but the index values are going to get very large very quickly.
 Figure 1.17  shows an input signal,  x ( n ), starting from  t  5 0 or  x (0). The  x (0) sample is the
ﬁ rst sample that enters the signal processing algorithm. In the grand scheme of things,  x (0)
will be the oldest input sample ever. Indexing the numbers with absolute position is going to
be a chore as the index values are going to become large, especially at very high sample rates.
 Another problem with dealing with the absolute position of samples is that algorithms
do not use the sample’s absolute position in their coding. Instead, algorithms use the
position of the current sample and make everything relevant to that sample. On the next
sample period, everything gets reorganized in relation to the current sample again. It
might sound confusing at ﬁ rst, but it is a much better method of keeping track of the
samples and more importantly, deﬁ ning the I/O characteristics of the algorithm, called
the  transfer function .  Figure 1.18  shows the input signal frozen at the current time,
 x ( n ), and the other samples are indexed based on its location. One sample period later
( Figure 1.19 ) you can see the frame of reference has shifted to the right and that  x ( n ) has
now become  x ( n  2 1).
 Bookkeeping rules:
•
•
•
•
   The current sample is labeled “ n .”
   Previous samples are negative, so  x ( n  2 1) would be the previous input sample.
   Future samples are positive, so  x ( n  1 1) would be the next input sample relative to the
current one.
   On the  next  sample interval, everything is shufﬂ ed and referenced to the new current
sample,  x ( n ).
Digital Audio Signal Processing Principles  15
x (n– 3)
Current  Input
Sample
x
(n– 4)
x (n–2)
x
(n–1)
(n + 4)
x
(n + 3)
x
x
(n+1 )
(n + 2)
x
The  Past
x
(n)
The  Future
 Figure 1.18:    DSP algorithms use the current sample location as the reference location
and all other samples are indexed based on that sample. Here you can see the current
state of the algorithm frozen in time at the current input sample x(n).
(n– 4)
x
Current  Input
Sample
x
(n– 5)
x (n– 3)
x
(n– 2)
x (n– 1)
x (n + 3)
x (n + 2 )
x  (n+1)
The Past
x (n)
The Future
 Figure 1.19:    One sample period later, everything has shifted. The previous x(n) is now indexed
as x(n − 1) and what was the next sample, x(n + 1) now becomes the current sample.
   1.11    The One-Sample Delay
 Whereas analog processing circuits like tone-controls use capacitors and inductors to alter the phase
and delay of the analog signal, digital algorithms use time-delay instead. You will uncover the
math and science behind this fact later on in Chapters 4 and 5 when you start to use it . In algorithm
16  Chapter 1
diagrams, a delay is represented by a box with the letter  z  inside. The  z -term will have an
exponent such as  z  25  or  z  12  or  z  0 —the exponent codes the delay in samples following the same
bookkeeping rules, with negative (2) exponents representing a delay in time backward (past
samples) and positive (1) representing delay in forward time (future samples). You call  z  the
 delay operator  and as it turns out, time-delay will be treated as a mathematical operation.
 You are probably asking yourself how you can have a positive delay toward the future, and
the answer is that you can’t for real-time signal processing. In real-time processing you
never know what sample is going to come in next. However, in non-real-time processing
(for example, an audio ﬁ le that you are processing ofﬂ ine) you do know the future samples
because they are in the ﬁ le.  Figure 1.20  shows two common ways to denote a one-sample
delay in an algorithm block diagram.
 Delay rules:
•
•
•
   Each time a sample goes into the delay register (memory location), the previously stored
sample is ejected.
   The ejected sample can be used for processing or deleted.
   The delay elements can be cascaded together with the output of one feeding the input of
the next to create more delay time.
 If a sample  x ( n ) goes into the one-sample delay element, then what do you call the sample
that is ejected? It’s the previous sample that came in, one sample interval in the past. So, the
output sample is  x ( n 21). In  Figure 1.21  you can see how delay elements can be cascaded
with outputs taken at multiple locations generating multiple samples to use in the algorithm.
   1.12    Multiplication
 The next algorithm building block is the  scalar multiplication  operation. It is a sample-by-
sample operator that simply multiplies the input samples by a coefﬁ cient. The multiplication
operator is used in just about every DSP algorithm around.  Figure 1.22  shows the multiplication
operator in action. The inputs are simply scaled by the coefﬁ cients.
In
In
z .-1
Out
z –1
 Figure 1.20:    Two common ways to draw a delay; the one-sample delay is
represented with the z−1. Both versions are equivalent.
Out
Digital Audio Signal Processing Principles  17
x(n)
x(n)
x(n)
z–1
z –1
x(n -1)
(a)
z–1
x(n–2)
(b)
x(n–1)
z–1
z–1
x(n–1)
x(n–2)
(c)
 Figure 1.21:    Three delay algorithms: (a) one-sample delay, (b) two one-sample
delays cascaded, producing two different outputs, x(n 2 1) and x(n 2 2),
notice that (c) is functionally identical to (b).
x (n)
Y(n)
a0
b 1
a0x(n)
b1Y(n)
 Figure 1.22:    The multiplication operator is displayed as a triangle and
a coefﬁ cient letter or number inside or just above it.
   1.13    Addition and Subtraction
 Addition and subtraction are really the same kind of operation—subtracting is the addition of a
negative number. There are several different algorithm symbols to denote addition and subtraction.
The operation of  mixing  signals is really the mathematical operation of addition.  Figure 1.23
shows several ways of displaying the addition and subtraction operation in block diagrams.
a(n)
∑
a(n)+ b(n)
a(n)
b(n)
a(n)
a(n)+ b(n)
a(n)
b(n)
a(n) – b(n)
a(n) – b(n)
∑
–
b(n)
+
–
b(n)
 Figure 1.23:    Addition and subtraction diagrams for two input sequences a(n) and b(n);
these are all commonly used forms of the same functions.
18  Chapter 1
   1.14    Algorithm Examples and the Difference Equation
 By convention, the output sequence of the DSP algorithm is named  y ( n ) and the mathematical
equation that relates it to the input is called the  difference equation . Combining the operations
will give you a better idea of what the difference equation looks like.  Figure 1.24  shows the
difference equations for several combinations of algorithm building blocks. The output  y ( n ) is
a mathematical combination of inputs.
   1.15    Gain, Attenuation, and Phase Inversion
 As shown in  Figure 1.25 , a simple coefﬁ cient multiplier will handle the three basic audio
processing functions of gain, attenuation, and inversion. If the coefﬁ cient is a negative
number, phase inversion will be the result. If the coefﬁ cient has a magnitude less than 1.0,
attenuation will occur, while ampliﬁ cation occurs if the magnitude is greater than 1.0.
Difference Equations
y(n) = x(n –1)
y(n) = a2  x (n)
y(n) = p(n) + q(n)
x(n)
x(n)
p(n)
z -1
a2
∑
q(n)
 Figure 1.24:    The difference equations relate the input(s)
to the output via mathematical operations
a
2.5
0.5
-1.0
x(n)
x(n)
x(n)
x(n)
y(n) = ax(n)
(a)
y(n) = 2.5x(n)
(b)
y(n) =  0.5x(n)
(c)
y(n) = –x(n)
(d)
 Figure 1.25:    Examples of simple multiplier algorithms. Notice the different notation with the
coefﬁ cient placed outside the triangle; this is another common way to designate it. (a) Simple
scalar multiplication by an arbitrary value “a”. (b) Gain is accomplished with a coefﬁ cient
magnitude greater than one. (c) Attenuation reduces the size of the input value with a coefﬁ cient
that is less than one. (d) Phase inversion turns the signal upside down by using a negative
coefﬁ cient; a value of 21.0 perfectly inverts the signal.

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
