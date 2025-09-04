# Understanding DSP (Lyons) — Chunk 5/27

> Source: `UnderstandingDigitalSignalProcessing.pdf` · Pages ~1-858 · Extractor: pdfminer.six
> Target ≈ 2800 words, Overlap = 200 words · Actual = 15966

> Note: DSP textbook content for SpectralCanvasPro RT-audio knowledge base

---
Chapter One. Discrete Sequences and Systems
Digital signal processing has never been more prevalent or easier to perform. It wasn’t that long ago
when  the  fast  Fourier  transform  (FFT),  a  topic  we’ll  discuss  in  Chapter  4,  was  a  mysterious
mathematical process used only in industrial research centers and universities. Now, amazingly, the
FFT is readily available to us all. It’s even a built-in function provided by inexpensive spreadsheet
software for home computers. The availability of more sophisticated commercial signal processing
software now allows us to analyze and develop complicated signal processing applications rapidly
and reliably. We can perform spectral analysis, design digital filters, develop voice recognition, data
communication,  and  image  compression  processes  using  software  that’s  interactive  both  in  the  way
algorithms are defined and how the resulting data are graphically displayed. Since the mid-1980s the
same integrated circuit technology that led to affordable home computers has produced powerful and
inexpensive  hardware  development  systems  on  which  to  implement  our  digital  signal  processing
designs.† Regardless, though, of the ease with which these new digital signal processing development
systems and software can be applied, we still need a solid foundation in understanding the basics of
digital signal processing. The purpose of this book is to build that foundation.
†  During  a  television  interview  in  the  early  1990s,  a  leading  computer  scientist  stated  that  had  automobile  technology  made  the  same
strides as the computer industry, we’d all have a car that would go a half million miles per hour and get a half million miles per gallon.
The cost of that car would be so low that it would be cheaper to throw it away than pay for one day’s parking in San Francisco.
In this chapter we’ll set the stage for the topics we’ll study throughout the remainder of this book by
defining the terminology used in digital signal processing, illustrating the various ways of graphically
representing  discrete  signals,  establishing  the  notation  used  to  describe  sequences  of  data  values,
presenting  the  symbols  used  to  depict  signal  processing  operations,  and  briefly  introducing  the
concept of a linear discrete system.
1.1 Discrete Sequences and Their Notation
In  general,  the  term  signal  processing  refers  to  the  science  of  analyzing  time-varying  physical
processes.  As  such,  signal  processing  is  divided  into  two  categories,  analog  signal  processing  and
digital signal processing. The term analog is used to describe a waveform that’s continuous in time
and  can  take  on  a  continuous  range  of  amplitude  values.  An  example  of  an  analog  signal  is  some
voltage that can be applied to an oscilloscope, resulting in a continuous display as a function of time.
Analog signals can also be applied to a conventional spectrum analyzer to determine their frequency
content. The term analog  appears  to  have  stemmed  from  the  analog  computers  used  prior  to  1980.
These  computers  solved  linear  differential  equations  by  means  of  connecting  physical  (electronic)
differentiators and integrators using old-style telephone operator patch cords. That way, a continuous
voltage or current in the actual circuit was analogous to some variable in a differential equation, such
as  speed,  temperature,  air  pressure,  etc.  (Although  the  flexibility  and  speed  of  modern-day  digital
computers have since made analog computers obsolete, a good description of the short-lived utility of
analog computers can be found in reference [1].) Because present-day signal processing of continuous
radio-type  signals  using  resistors,  capacitors,  operational  amplifiers,  etc.,  has  nothing  to  do  with
analogies,  the  term  analog  is  actually  a  misnomer.  The  more  correct  term  is  continuous  signal
processing  for  what  is  today  so  commonly  called  analog  signal  processing.  As  such,  in  this  book
we’ll  minimize  the  use  of  the  term  analog  signals  and  substitute  the  phrase  continuous  signals
whenever appropriate.
The  term  discrete-time  signal  is  used  to  describe  a  signal  whose  independent  time  variable  is
quantized so that we know only the value of the signal at discrete instants in time. Thus a discrete-
time  signal  is  not  represented  by  a  continuous  waveform  but,  instead,  a  sequence  of  values.  In
addition  to  quantizing  time,  a  discrete-time  signal  quantizes  the  signal  amplitude.  We  can  illustrate
this  concept  with  an  example.  Think  of  a  continuous  sinewave  with  a  peak  amplitude  of  1  at  a
frequency fo described by the equation
(1-1)
The frequency fo  is  measured  in  hertz  (Hz).  (In  physical  systems,  we  usually  measure  frequency  in
units of hertz. One Hz is a single oscillation, or cycle, per second. One kilohertz (kHz) is a thousand
Hz, and a megahertz (MHz) is one million Hz.†) With t in Eq. 1-1 representing time in seconds, the fot
factor has dimensions of cycles, and the complete 2πfot term is an angle measured in radians.
†  The  dimension  for  frequency  used  to  be  cycles/second;  that’s  why  the  tuning  dials  of  old  radios  indicate  frequency  as
kilocycles/second  (kcps)  or  megacycles/second  (Mcps).  In  1960  the  scientific  community  adopted  hertz  as  the  unit  of  measure  for
frequency in honor of the German physicist Heinrich Hertz, who first demonstrated radio wave transmission and reception in 1887.
Plotting Eq. (1-1),  we  get  the  venerable  continuous  sinewave  curve  shown  in  Figure 1-1(a).  If  our
continuous sinewave represents a physical voltage, we could sample it once every ts seconds using
an analog-to-digital converter and represent the sinewave as a sequence of discrete values. Plotting
those individual values as dots would give us the discrete waveform in Figure 1-1(b). We say that
Figure  1-1(b)  is  the  “discrete-time”  version  of  the  continuous  signal  in  Figure  1-1(a).  The
independent variable t in Eq. (1-1) and Figure 1-1(a) is continuous. The independent index variable n
in Figure 1-1(b) is discrete and can have only integer values. That is, index n is used to identify the
individual elements of the discrete sequence in Figure 1-1(b).
Figure 1-1 A time-domain sinewave: (a) continuous waveform representation; (b) discrete sample
representation; (c) discrete samples with connecting lines.
Do  not  be  tempted  to  draw  lines  between  the  dots  in  Figure  1-1(b).  For  some  reason,  people
(particularly  those  engineers  experienced  in  working  with  continuous  signals)  want  to  connect  the
dots with straight lines, or the stair-step lines shown in Figure 1-1(c). Don’t fall into this innocent-
looking  trap.  Connecting  the  dots  can  mislead  the  beginner  into  forgetting  that  the  x(n)  sequence  is
nothing more than a list of numbers. Remember, x(n) is a discrete-time sequence of individual values,
and each value in that sequence plots as a single dot. It’s not that we’re ignorant of what lies between
the dots of x(n); there is nothing between those dots.
We can reinforce this discrete-time sequence concept by listing those Figure 1-1(b) sampled values
as follows:
(1-2)
where n represents the time index integer sequence 0, 1, 2, 3, etc., and ts is some constant time period
between  samples.  Those  sample  values  can  be  represented  collectively,  and  concisely,  by  the
discrete-time expression
(1-3)
(Here again, the 2πfonts term is an angle measured in radians.) Notice that the index n in Eq. (1-2)
started with a value of 0, instead of 1. There’s nothing sacred about this; the first value of n could just
as  well  have  been  1,  but  we  start  the  index  n  at  zero  out  of  habit  because  doing  so  allows  us  to
describe the sinewave starting at time zero. The variable x(n) in Eq. (1-3) is read as “the sequence x
of n.” Equations (1-1) and (1-3) describe what are also referred to as time-domain signals because
the independent variables, the continuous time t in Eq. (1-1), and the discrete-time nts values used in
Eq. (1-3) are measures of time.
With this notion of a discrete-time signal in mind, let’s say that a discrete system is a collection of
hardware  components,  or  software  routines,  that  operate  on  a  discrete-time  signal  sequence.  For
example,  a  discrete  system  could  be  a  process  that  gives  us  a  discrete  output  sequence  y(0),  y(1),
y(2), etc., when a discrete input sequence of x(0), x(1), x(2), etc., is applied to the system input as
shown in Figure 1-2(a). Again, to keep the notation concise and still keep track of individual elements
of the input and output sequences, an abbreviated notation is used as shown in Figure 1-2(b) where n
represents the integer sequence 0, 1, 2, 3, etc. Thus, x(n) and y(n) are general variables that represent
two  separate  sequences  of  numbers.  Figure 1-2(b)  allows  us  to  describe  a  system’s  output  with  a
simple expression such as
(1-4)
Figure 1-2 With an input applied, a discrete system provides an output: (a) the input and output are
sequences of individual values; (b) input and output using the abbreviated notation of x(n) and y(n).
Illustrating Eq. (1-4), if x(n) is the five-element sequence x(0) = 1, x(1) = 3, x(2) = 5, x(3) = 7, and
x(4) = 9, then y(n) is the five-element sequence y(0) = 1, y(1) = 5, y(2) = 9, y(3) = 13, and y(4) = 17.
Equation (1-4)  is  formally  called  a  difference  equation.  (In  this  book  we  won’t  be  working  with
differential equations or partial differential equations. However, we will, now and then, work with
partially difficult equations.)
The fundamental difference between the way time is represented in continuous and discrete systems
leads  to  a  very  important  difference  in  how  we  characterize  frequency  in  continuous  and  discrete
systems. To illustrate, let’s reconsider the continuous sinewave in Figure 1-1(a). If it represented a
voltage  at  the  end  of  a  cable,  we  could  measure  its  frequency  by  applying  it  to  an  oscilloscope,  a
spectrum analyzer, or a frequency counter. We’d have a problem, however, if we were merely given
the list of values from Eq. (1-2) and asked to determine the frequency of the waveform they represent.
We’d graph those discrete values, and, sure enough, we’d recognize a single sinewave as in Figure 1-
1(b).  We  can  say  that  the  sinewave  repeats  every  20  samples,  but  there’s  no  way  to  determine  the
exact sinewave frequency from the discrete sequence values alone. You can probably see the point
we’re leading to here. If we knew the time between samples—the sample period ts—we’d be able to
determine  the  absolute  frequency  of  the  discrete  sinewave.  Given that the ts  sample  period  is,  say,
0.05 milliseconds/sample, the period of the sinewave is
(1-5)
Because the frequency of a sinewave is the reciprocal of its period, we now know that the sinewave’s
absolute frequency is 1/(1 ms), or 1 kHz. On the other hand, if we found that the sample period was,
in  fact,  2  milliseconds,  the  discrete  samples  in  Figure  1-1(b)  would  represent  a  sinewave  whose
period is 40 milliseconds and whose frequency is 25 Hz. The point here is that when dealing with
discrete systems, absolute frequency determination in Hz is dependent on the sampling frequency
(1-5′)
We’ll be reminded of this dependence throughout the remainder of this book.
In  digital  signal  processing,  we  often  find  it  necessary  to  characterize  the  frequency  content  of
discrete  time-domain  signals.  When  we  do  so,  this  frequency  representation  takes  place  in  what’s
called the frequency domain. By way of example, let’s say we have a discrete sinewave sequence
x1(n)  with  an  arbitrary  frequency  fo  Hz  as  shown  on  the  left  side  of  Figure  1-3(a).  We  can  also
characterize x1(n) by showing its spectral content, the X1(m) sequence on the right side of Figure 1-
3(a), indicating that it has a single spectral component, and no other frequency content. Although we
won’t  dwell  on  it  just  now,  notice  that  the  frequency-domain  representations  in  Figure  1-3  are
themselves discrete.
Figure 1-3 Time- and frequency-domain graphical representations: (a) sinewave of frequency fo; (b)
reduced amplitude sinewave of frequency 2fo; (c) sum of the two sinewaves.
To  illustrate  our  time-  and  frequency-domain  representations  further,  Figure  1-3(b)  shows  another
discrete sinewave x2(n), whose peak amplitude is 0.4, with a frequency of 2fo. The discrete sample
values of x2(n) are expressed by the equation
(1-6)
When the two sinewaves, x1(n) and x2(n), are added to produce a new waveform xsum(n), its time-
domain equation is
(1-7)
and its time- and frequency-domain representations are those given in Figure 1-3(c). We interpret the
Xsum(m)  frequency-domain  depiction,  the  spectrum,  in  Figure  1-3(c)  to  indicate  that  xsum(n)  has  a
frequency component of fo Hz and a reduced-amplitude frequency component of 2fo Hz.
Notice three things in Figure 1-3. First, time sequences use lowercase variable names like the “x” in
x1(n),  and  uppercase  symbols  for  frequency-domain  variables  such  as  the  “X”  in  X1(m).  The  term
X1(m)  is  read  as  “the  spectral  sequence  X  sub  one  of  m.”  Second,  because  the  X1(m)  frequency-
domain representation of the x1(n) time sequence is itself a sequence (a list of numbers), we use the
index “m” to keep track of individual elements in X1(m). We can list frequency-domain sequences just
as we did with the time sequence in Eq. (1-2). For example, Xsum(m) is listed as
where the frequency index m is the integer sequence 0, 1, 2, 3, etc. Third, because the x1(n) + x2(n)
sinewaves have a phase shift of zero degrees relative to each other, we didn’t really need to bother
depicting this phase relationship in Xsum(m) in Figure 1-3(c). In general, however, phase relationships
in frequency-domain sequences are important, and we’ll cover that subject in Chapters 3 and 5.
A key point to keep in mind here is that we now know three equivalent ways to describe a discrete-
time  waveform.  Mathematically,  we  can  use  a  time-domain  equation  like  Eq.  (1-6).  We  can  also
represent a time-domain waveform graphically as we did on the left side of Figure 1-3, and we can
depict its corresponding, discrete, frequency-domain equivalent as that on the right side of Figure 1-3.
As it turns out, the discrete time-domain signals we’re concerned with are not only quantized in time;
their  amplitude  values  are  also  quantized.  Because  we  represent  all  digital  quantities  with  binary
numbers,  there’s  a  limit  to  the  resolution,  or  granularity,  that  we  have  in  representing  the  values  of
discrete  numbers.  Although  signal  amplitude  quantization  can  be  an  important  consideration—we
cover that particular topic in Chapter 12—we won’t worry about it just now.
1.2 Signal Amplitude, Magnitude, Power
Let’s define two important terms that we’ll be using throughout this book: amplitude and magnitude.
It’s not surprising that, to the layman, these terms are typically used interchangeably. When we check
our thesaurus, we find that they are synonymous.† In engineering, however, they mean two different
things, and we must keep that difference clear in our discussions. The amplitude of a variable is the
measure of how far, and in what direction, that variable differs from zero. Thus, signal amplitudes can
be either positive or negative. The time-domain sequences in Figure 1-3 presented the sample value
amplitudes  of  three  different  waveforms.  Notice  how  some  of  the  individual  discrete  amplitude
values were positive and others were negative.
†  Of  course,  laymen  are  “other  people.”  To  the  engineer,  the  brain  surgeon  is  the  layman.  To  the  brain  surgeon,  the  engineer  is  the
layman.
The magnitude of a variable, on the other hand, is the measure of how far, regardless of direction, its
quantity differs from zero. So magnitudes are always positive values. Figure 1-4 illustrates how the
magnitude  of  the  x1(n)  time  sequence  in  Figure 1-3(a)  is  equal  to  the  amplitude,  but  with  the  sign
always being positive for the magnitude. We use the modulus symbol (||) to represent the magnitude of
x1(n).  Occasionally,  in  the  literature  of  digital  signal  processing,  we’ll  find  the  term  magnitude
referred to as the absolute value.
Figure 1-4 Magnitude samples, |x1(n)|, of the time waveform in Figure 1-3(a).
When  we  examine  signals  in  the  frequency  domain,  we’ll  often  be  interested  in  the  power  level  of
those signals. The power of a signal is proportional to its amplitude (or magnitude) squared. If we
assume that the proportionality constant is one, we can express the power of a sequence in the time or
frequency domains as
or
(1-8)
(1-8′)
Very often we’ll want to know the difference in power levels of two signals in the frequency domain.
Because of the squared nature of power, two signals with moderately different amplitudes will have a
much larger difference in their relative powers. In Figure 1-3, for example, signal x1(n)’s amplitude
is 2.5 times the amplitude of signal x2(n), but its power level is 6.25 that of x2(n)’s power level. This
is illustrated in Figure 1-5 where both the amplitude and power of Xsum(m) are shown.
Figure 1-5 Frequency-domain amplitude and frequency-domain power of the xsum(n) time waveform
in Figure 1-3(c).
Because  of  their  squared  nature,  plots  of  power  values  often  involve  showing  both  very  large  and
very  small  values  on  the  same  graph.  To  make  these  plots  easier  to  generate  and  evaluate,
practitioners usually employ the decibel scale as described in Appendix E.
1.3 Signal Processing Operational Symbols
We’ll be using block diagrams to graphically depict the way digital signal processing operations are
implemented. Those block diagrams will comprise an assortment of fundamental processing symbols,
the most common of which are illustrated and mathematically defined in Figure 1-6.
Figure 1-6 Terminology and symbols used in digital signal processing block diagrams.
Figure 1-6(a) shows the addition, element for element, of two discrete sequences to provide a new
sequence. If our sequence index n begins at 0, we say that the first output sequence value is equal to
the sum of the first element of the b sequence and the first element of the c sequence, or a(0) = b(0) +
c(0). Likewise, the second output sequence value is equal to the sum of the second element of the b
sequence  and  the  second  element  of  the  c  sequence,  or  a(1)  =  b(1)  +  c(1).  Equation  (1-7)  is  an
example  of  adding  two  sequences.  The  subtraction  process  in  Figure  1-6(b)  generates  an  output
sequence that’s the element-for-element difference of the two input sequences. There are times when
we must calculate a sequence whose elements are the sum of more than two values. This operation,
illustrated  in  Figure 1-6(c),  is  called  summation  and  is  very  common  in  digital  signal  processing.
Notice how the lower and upper limits of the summation index k in the expression in Figure 1-6(c)
tell us exactly which elements of the b sequence to sum to obtain a given a(n) value. Because we’ll
encounter summation operations so often, let’s make sure we understand their notation. If we repeat
the summation equation from Figure 1-6(c) here, we have
This means that
(1-9)
(1-10)
We’ll begin using summation operations in earnest when we discuss digital filters in Chapter 5.
The  multiplication  of  two  sequences  is  symbolized  in  Figure  1-6(d).  Multiplication  generates  an
output sequence that’s the element-for-element product of two input sequences: a(0) = b(0)c(0), a(1)
= b(1)c(1), and so on. The last fundamental operation that we’ll be using is called the unit delay in
Figure 1-6(e). While we don’t need to appreciate its importance at this point, we’ll merely state that
the  unit  delay  symbol  signifies  an  operation  where  the  output  sequence  a(n)  is  equal  to  a  delayed
version of the b(n) sequence. For example, a(5) = b(4), a(6) = b(5), a(7) = b(6), etc. As we’ll see in
Chapter 6,  due  to  the  mathematical  techniques  used  to  analyze  digital  filters,  the  unit  delay  is  very
often depicted using the term z−1.
The symbols in Figure 1-6 remind us of two important aspects of digital signal processing. First, our
processing operations are always performed on sequences of individual discrete values, and second,
the  elementary  operations  themselves  are  very  simple.  It’s  interesting  that,  regardless  of  how
complicated  they  appear  to  be,  the  vast  majority  of  digital  signal  processing  algorithms  can  be
performed using combinations of these simple operations. If we think of a digital signal processing
algorithm as a recipe, then the symbols in Figure 1-6 are the ingredients.
1.4 Introduction to Discrete Linear Time-Invariant Systems
In  keeping  with  tradition,  we’ll  introduce  the  subject  of  linear  time-invariant  (LTI)  systems  at  this
early point in our text. Although an appreciation for LTI systems is not essential in studying the next
three  chapters  of  this  book,  when  we  begin  exploring  digital  filters,  we’ll  build  on  the  strict
definitions  of  linearity  and  time  invariance.  We  need  to  recognize  and  understand  the  notions  of
linearity and time invariance not just because the vast majority of discrete systems used in practice
are LTI systems, but because LTI systems are very accommodating when it comes to their analysis.
That’s good news for us because we can use straightforward methods to predict the performance of
any digital signal processing scheme as long as it’s linear and time invariant. Because linearity and
time  invariance  are  two  important  system  characteristics  having  very  special  properties,  we’ll
discuss them now.
1.5 Discrete Linear Systems
The term linear defines a special class of systems where the output is the superposition, or sum, of
the individual outputs had the individual inputs been applied separately to the system. For example,
we can say that the application of an input x1(n) to a system results in an output y1(n). We symbolize
this situation with the following expression:
(1-11)
Given a different input x2(n), the system has a y2(n) output as
(1-12)
For the system to be linear, when its input is the sum x1(n) + x2(n), its output must be the sum of the
individual outputs so that
(1-13)
One  way  to  paraphrase  expression  (1-13)  is  to  state  that  a  linear  system’s  output  is  the  sum  of  the
outputs of its parts. Also, part of this description of linearity is a proportionality characteristic. This
means that if the inputs are scaled by constant factors c1 and c2,  then  the  output  sequence  parts  are
also scaled by those factors as
(1-14)
In  the  literature,  this  proportionality  attribute  of  linear  systems  in  expression  (1-14)  is  sometimes
called the homogeneity property. With these thoughts in mind, then, let’s demonstrate the concept of
system linearity.
1.5.1 Example of a Linear System
To  illustrate  system  linearity,  let’s  say  we  have  the  discrete  system  shown  in  Figure 1-7(a)  whose
output is defined as
(1-15)
Figure 1-7 Linear system input-to-output relationships: (a) system block diagram where y(n) =
−x(n)/2; (b) system input and output with a 1 Hz sinewave applied; (c) with a 3 Hz sinewave applied;
(d) with the sum of 1 Hz and 3 Hz sinewaves applied.
that is, the output sequence is equal to the negative of the input sequence with the amplitude reduced
by a factor of two. If we apply an x1(n) input sequence representing a 1 Hz sinewave sampled at a
rate of 32 samples per cycle, we’ll have a y1(n) output as shown in the center of Figure 1-7(b). The
frequency-domain spectral amplitude of the y1(n) output is the plot on the right side of Figure 1-7(b),
indicating that the output comprises a single tone of peak amplitude equal to −0.5 whose frequency is
1 Hz. Next, applying an x2(n)  input  sequence  representing  a  3  Hz  sinewave,  the  system  provides  a
y2(n)  output  sequence,  as  shown  in  the  center  of  Figure  1-7(c).  The  spectrum  of  the  y2(n)  output,
Y2(m), confirming a single 3 Hz sinewave output is shown on the right side of Figure 1-7(c). Finally
—here’s where the linearity comes in—if we apply an x3(n) input sequence that’s the sum of a 1 Hz
sinewave and a 3 Hz sinewave, the y3(n) output is as shown in the center of Figure 1-7(d). Notice
how y3(n) is the sample-for-sample sum of y1(n) and y2(n). Figure 1-7(d) also shows that the output
spectrum Y3(m) is the sum of Y1(m) and Y2(m). That’s linearity.
1.5.2 Example of a Nonlinear System
It’s easy to demonstrate how a nonlinear system yields an output that is not equal to the sum of y1(n)
and y2(n) when its input is x1(n) + x2(n). A simple example of a nonlinear discrete system is that in
Figure 1-8(a) where the output is the square of the input described by
(1-16)
Figure 1-8 Nonlinear system input-to-output relationships: (a) system block diagram where y(n) =
[x(n)]2; (b) system input and output with a 1 Hz sinewave applied; (c) with a 3 Hz sinewave applied;
(d) with the sum of 1 Hz and 3 Hz sinewaves applied.
We’ll  use  a  well-known  trigonometric  identity  and  a  little  algebra  to  predict  the  output  of  this
nonlinear system when the input comprises simple sinewaves. Following the form of Eq. (1-3), let’s
describe a sinusoidal sequence, whose frequency fo = 1 Hz, by
(1-17)
Equation (1-17) describes the x1(n) sequence on the left side of Figure 1-8(b). Given this x1(n) input
sequence, the y1(n) output of the nonlinear system is the square of a 1 Hz sinewave, or
(1-18)
We can simplify our expression for y1(n) in Eq. (1-18) by using the following trigonometric identity:
(1-19)
Using Eq. (1-19), we can express y1(n) as
(1-20)
which is shown as the all-positive sequence in the center of Figure 1-8(b). Because Eq. (1-19) results
in a frequency sum (α + β) and frequency difference (α − β) effect when multiplying two sinusoids,
the y1(n) output sequence will be a cosine wave of 2 Hz and a peak amplitude of −0.5, added to a
constant value of 1/2. The constant value of 1/2 in Eq. (1-20) is interpreted as a zero Hz frequency
component, as shown in the Y1(m) spectrum in Figure 1-8(b). We could go through the same algebraic
exercise to determine that when a 3 Hz sinewave x2(n) sequence is applied to this nonlinear system,
the output y2(n) would contain a zero Hz component and a 6 Hz component, as shown in Figure 1-
8(c).
System nonlinearity is evident if we apply an x3(n) sequence comprising the sum of a 1 Hz and a 3 Hz
sinewave  as  shown  in  Figure  1-8(d).  We  can  predict  the  frequency  content  of  the  y3(n)  output
sequence by using the algebraic relationship
(1-21)
where a and b represent the 1 Hz and 3 Hz sinewaves, respectively. From Eq. (1-19), the a2 term in
Eq. (1-21) generates the zero Hz and 2 Hz output sinusoids in Figure 1-8(b). Likewise, the b2 term
produces in y3(n)  another  zero  Hz  and  the  6  Hz  sinusoid  in  Figure 1-8(c).  However,  the  2ab  term
yields additional 2 Hz and 4 Hz sinusoids in y3(n). We can show this algebraically by using Eq. (1-
19) and expressing the 2ab term in Eq. (1-21) as
(1-22)
† The first term in Eq. (1-22) is cos(2π · nts − 6π · nts) = cos(−4π · nts) = cos(−2π · 2 · nts). However, because the cosine function is
even, cos(−α) = cos(α), we can express that first term as cos(2π · 2 · nts).
Equation (1-22) tells us that two additional sinusoidal components will be present in y3(n) because of
the system’s nonlinearity, a 2 Hz cosine wave whose amplitude is +1 and a 4 Hz cosine wave having
an amplitude of −1. These spectral components are illustrated in Y3(m) on the right side of Figure 1-
8(d).
Notice  that  when  the  sum  of  the  two  sinewaves  is  applied  to  the  nonlinear  system,  the  output
contained  sinusoids,  Eq. (1-22),  that  were  not  present  in  either  of  the  outputs  when  the  individual
sinewaves  alone  were  applied.  Those  extra  sinusoids  were  generated  by  an  interaction  of  the  two
input sinusoids due to the squaring operation. That’s nonlinearity; expression (1-13) was not satisfied.
(Electrical  engineers  recognize  this  effect  of  internally  generated  sinusoids  as  intermodulation
distortion.) Although nonlinear systems are usually difficult to analyze, they are occasionally used in
practice.  References  [2],  [3],  and  [4],  for  example,  describe  their  application  in  nonlinear  digital
filters. Again, expressions (1-13) and (1-14) state that a linear system’s output resulting from a sum of
individual  inputs  is  the  superposition  (sum)  of  the  individual  outputs.  They  also  stipulate  that  the
output sequence y1(n) depends only on x1(n) combined with the system characteristics, and not on the
other input x2(n); i.e., there’s no interaction between inputs x1(n) and x2(n) at the output of a linear
system.
1.6 Time-Invariant Systems
A  time-invariant  system  is  one  where  a  time  delay  (or  shift)  in  the  input  sequence  causes  an
equivalent  time  delay  in  the  system’s  output  sequence.  Keeping  in  mind  that  n  is  just  an  indexing
variable we use to keep track of our input and output samples, let’s say a system provides an output
y(n) given an input of x(n), or
(1-23)
For a system to be time invariant, with a shifted version of the original x(n) input applied, x′(n), the
following applies:
(1-24)
where k is some integer representing k sample period time delays. For a system to be time invariant,
Eq. (1-24) must hold true for any integer value of k and any input sequence.
1.6.1 Example of a Time-Invariant System
Let’s look at a simple example of time invariance illustrated in Figure 1-9.  Assume  that  our  initial
x(n) input is a unity-amplitude 1 Hz sinewave sequence with a y(n) output, as shown in Figure 1-9(b).
Consider a different input sequence x′(n), where
(1-25)
Figure 1-9 Time-invariant system input/output relationships: (a) system block diagram, y(n) =
−x(n)/2; (b) system input/output with a sinewave input; (c) input/output when a sinewave, delayed by
four samples, is the input.
Equation (1-25) tells us that the input sequence x′(n) is equal to sequence x(n) shifted to the right by k
= −4 samples. That is, x′(4) = x(0), x′(5) = x(1), x′(6) = x(2), and so on as shown in Figure 1-9(c).
The discrete system is time invariant because the y′(n) output sequence is equal to the y(n) sequence
shifted to the right by four samples, or y′(n) = y(n−4). We can see that y′(4) = y(0), y′(5) = y(1), y′(6)
= y(2), and so on as shown in Figure 1-9(c). For time-invariant systems, the time shifts in x′(n) and y′
(n) are equal. Take careful notice of the minus sign in Eq. (1-25). In later chapters, that is the notation
we’ll use to algebraically describe a time-delayed discrete sequence.
Some authors succumb to the urge to define a time-invariant system as one whose parameters do not
change with time. That definition is incomplete and can get us in trouble if we’re not careful. We’ll
just stick with the formal definition that a time-invariant system is one where a time shift in an input
sequence results in an equal time shift in the output sequence. By the way, time-invariant systems in
the literature are often called shift-invariant systems.†
† An example of a discrete process that’s not time invariant is the downsampling, or decimation, process described in Chapter 10.
1.7 The Commutative Property of Linear Time-Invariant Systems
Although we don’t substantiate this fact until we reach Section 6.11, it’s not too early to realize that
LTI systems have a useful commutative property by which their sequential order can be rearranged
with no change in their final output. This situation is shown in Figure 1-10 where two different LTI
systems are configured in series. Swapping the order of two cascaded systems does not alter the final
output. Although the intermediate data sequences f(n) and g(n) will usually not be equal, the two pairs
of LTI systems will have identical y(n) output sequences. This commutative characteristic comes in
handy for designers of digital filters, as we’ll see in Chapters 5 and 6.
Figure 1-10 Linear time-invariant (LTI) systems in series: (a) block diagram of two LTI systems; (b)
swapping the order of the two systems does not change the resultant output y(n).
1.8 Analyzing Linear Time-Invariant Systems
As previously stated, LTI systems can be analyzed to predict their performance. Specifically, if we
know the unit impulse response of an LTI system, we can calculate everything there is to know about
the system; that is, the system’s unit impulse response completely characterizes the system. By “unit
impulse  response”  we  mean  the  system’s  time-domain  output  sequence  when  the  input  is  a  single
unity-valued  sample  (unit  impulse)  preceded  and  followed  by  zero-valued  samples  as  shown  in
Figure 1-11(b).
Figure 1-11 LTI system unit impulse response sequences: (a) system block diagram; (b) impulse input
sequence x(n) and impulse response output sequence y(n).
Knowing the (unit) impulse response of an LTI system, we can determine the system’s output sequence
for any input sequence because the output is equal to the convolution of the input sequence and the
system’s impulse response. Moreover, given an LTI system’s time-domain impulse response, we can
find the system’s frequency response by taking the Fourier transform in the form of a discrete Fourier
transform  of  that  impulse  response[5].  The  concepts  in  the  two  previous  sentences  are  among  the
most important principles in all of digital signal processing!
Don’t be alarmed if you’re not exactly sure what is meant by convolution, frequency response, or the
discrete Fourier transform. We’ll introduce these subjects and define them slowly and carefully as we
need them in later chapters. The point to keep in mind here is that LTI systems can be designed and
analyzed using a number of straightforward and powerful analysis techniques. These techniques will
become tools that we’ll add to our signal processing toolboxes as we journey through the subject of
digital signal processing.
In the testing (analyzing) of continuous linear systems, engineers often use a narrow-in-time impulsive
signal as an input signal to their systems. Mechanical engineers give their systems a little whack with
a  hammer,  and  electrical  engineers  working  with  analog-voltage  systems  generate  a  very  narrow
voltage  spike  as  an  impulsive  input.  Audio  engineers,  who  need  an  impulsive  acoustic  test  signal,
sometimes generate an audio impulse by firing a starter pistol.
In the world of DSP, an impulse sequence called a unit impulse takes the form
(1-26)
The value A  is  often  set  equal  to  one.  The  leading  sequence  of  zero-valued  samples,  before  the  A-
valued sample, must be a bit longer than the length of the transient response of the system under test in
order to initialize the system to its zero state. The trailing sequence of zero-valued samples, following
the A-valued sample, must be a bit longer than the transient response of the system under test in order
to capture the system’s entire y(n) impulse response output sequence.
Let’s further explore this notion of impulse response testing to determine the frequency response of a
discrete system (and take an opportunity to start using the operational symbols introduced in Section
1.3). Consider the block diagram of a 4-point moving averager shown in Figure 1-12(a). As the x(n)
input samples march their way through the system, at each time index n four successive input samples
are  averaged  to  compute  a  single  y(n)  output.  As  we’ll  learn  in  subsequent  chapters,  a  moving
averager behaves like a digital lowpass filter. However, we can quickly illustrate that fact now.
Figure 1-12 Analyzing a moving averager: (a) averager block diagram; (b) impulse input and impulse
response; (c) averager frequency magnitude response.
If we apply an impulse input sequence to the system, we’ll obtain its y(n) impulse response output
shown in Figure 1-12(b). The y(n) output is computed using the following difference equation:
(1-27)
If we then perform a discrete Fourier transform (a process we cover in much detail in Chapter 3) on
y(n), we obtain the Y(m) frequency-domain information, allowing us to plot the frequency magnitude
response  of  the  4-point  moving  averager  as  shown  in  Figure  1-12(c).  So  we  see  that  a  moving
averager indeed has the characteristic of a lowpass filter. That is, the averager attenuates (reduces the
amplitude of) high-frequency signal content applied to its input.
OK, this concludes our brief introduction to discrete sequences and systems. In later chapters we’ll
learn the details of discrete Fourier transforms, discrete system impulse responses, and digital filters.
References
[1] Karplus, W. J., and Soroka, W. W. Analog Methods, 2nd ed., McGraw-Hill, New York, 1959, p.
117.
[2] Mikami, N., Kobayashi, M., and Yokoyama, Y. “A New DSP-Oriented Algorithm for Calculation
of the Square Root Using a Nonlinear Digital Filter,” IEEE Trans. on Signal Processing, Vol. 40,
No. 7, July 1992.
[3]  Heinen,  P.,  and  Neuvo,  Y.  “FIR-Median  Hybrid  Filters,”  IEEE  Trans.  on  Acoust.  Speech,  and
Signal Proc., Vol. ASSP-35, No. 6, June 1987.
[4] Oppenheim, A., Schafer, R., and Stockham, T. “Nonlinear Filtering of Multiplied and Convolved
Signals,” Proc. IEEE, Vol. 56, August 1968.
[5] Pickerd, John. “Impulse-Response Testing Lets a Single Test Do the Work of Thousands,” EDN,
April 27, 1995.
Chapter 1 Problems
1.1  This  problem  gives  us  practice  in  thinking  about  sequences  of  numbers.  For  centuries
mathematicians  have  developed  clever  ways  of  computing  π.  In  1671  the  Scottish  mathematician
James Gregory proposed the following very simple series for calculating π:
Thinking of the terms inside the parentheses as a sequence indexed by the variable n, where n = 0,
1, 2, 3, . . ., 100, write Gregory’s algorithm in the form
replacing the “?” characters with expressions in terms of index n.
1.2 One of the ways to obtain discrete sequences, for follow-on processing, is to digitize a continuous
(analog)  signal  with  an  analog-to-digital  (A/D)  converter.  A  6-bit  A/D  converter’s  output  words
(6-bit  binary  words)  can  only  represent  26=64  different  numbers.  (We  cover  this  digitization,
sampling, and A/D converters in detail in upcoming chapters.) Thus we say the A/D converter’s
“digital”  output  can  only  represent  a  finite  number  of  amplitude  values.  Can  you  think  of  a
continuous time-domain electrical signal that only has a finite number of amplitude values? If so,
draw a graph of that continuous-time signal.
1.3 On the Internet, the author once encountered the following line of C-language code
PI = 2*asin(1.0);
whose purpose was to define the constant π. In standard mathematical notation, that line of code
can be described by
π = 2 · sin−1(1).
Under what assumption does the above expression correctly define the constant π?
1.4 Many times in the literature of signal processing you will encounter the identity
x0 = 1.
That is, x raised to the zero power is equal to one. Using the Laws of Exponents, prove the above
expression to be true.
1.5 Recall that for discrete sequences the ts sample period (the time period between samples) is the
reciprocal  of  the  sample  frequency  fs.  Write  the  equations,  as  we  did  in  the  text’s  Eq.  (1-3),
describing time-domain sequences for unity-amplitude cosine waves whose fo frequencies are
(a) fo = fs/2, one-half the sample rate,
(b) fo = fs/4, one-fourth the sample rate,
(c) fo = 0 (zero) Hz.
1.6 Draw the three time-domain cosine wave sequences, where a sample value is represented by a
dot, described in Problem 1.5. The correct solution to Part (a) of this problem is a useful sequence
used to convert some lowpass digital filters into highpass filters. (Chapter 5 discusses that topic.)
The  correct  solution  to  Part  (b)  of  this  problem  is  an  important  discrete  sequence  used  for
frequency  translation  (both  for  signal  down-conversion  and  up-conversion)  in  modern-day
wireless communications systems. The correct solution to Part (c) of this problem should convince
us that it’s perfectly valid to describe a cosine sequence whose frequency is zero Hz.
1.7 Draw the three time-domain sequences of unity-amplitude sinewaves (not cosine waves) whose
frequencies are
(a) fo = fs/2, one-half the sample rate,
(b) fo = fs/4, one-fourth the sample rate,
(c) fo = 0 (zero) Hz.
The correct solutions to Parts (a) and (c) show us that the two frequencies, 0 Hz and fs/2 Hz, are
special frequencies in the world of discrete signal processing. What is special about the sinewave
sequences obtained from the above Parts (a) and (c)?
1.8  Consider  the  infinite-length  time-domain  sequence  x(n)  in  Figure  P1-8.  Draw  the  first  eight
samples of a shifted time sequence defined by
xshift(n) = x(n+1).
Figure P1-8
1.9 Assume, during your reading of the literature of DSP, you encounter the process shown in Figure
P1-9. The x(n) input sequence, whose fs sample rate is 2500 Hz, is multiplied by a sinusoidal m(n)
sequence  to  produce  the  y(n)  output  sequence.  What  is  the  frequency,  measured  in  Hz,  of  the
sinusoidal m(n) sequence?
Figure P1-9
1.10  There  is  a  process  in  DSP  called  an  “N-point  running  sum”  (a  kind  of  digital  lowpass  filter,
actually) that is described by the following equation:
Write  out,  giving  the  indices  of  all  the  x()  terms,  the  algebraic  expression  that  describes  the
computations needed to compute y(9) when N=6.
1.11 A 5-point moving averager can be described by the following difference equation:
(P1-1)
The averager’s signal-flow block diagram is shown in Figure P1-11, where the x(n) input samples
flow through the averager from left to right.
Figure P1-11
Equation (P1-1) is equivalent to
(P1-2)
(a) Draw the block diagram of the discrete system described by Eq. (P1-2).
(b) The moving average  processes  described  by  Eqs. (P1-1)  and  (P1-2)  have  identical  impulse
responses. Draw that impulse response.
(c)  If  you  had  to  implement  (using  programmable  hardware  or  assembling  discrete  hardware
components) either Eq. (P1-1) or Eq. (P1-2), which would you choose? Explain why.
1.12 In this book we will look at many two-dimensional drawings showing the value of one variable
(y)  plotted  as  a  function  of  another  variable  (x).  Stated  in  different  words,  we’ll  graphically
display  what  are  the  values  of  a  y  axis  variable  for  various  values  of  an  x  axis  variable.  For
example, Figure P1-12(a)  plots  the  weight  of  a  male  child  as  a  function  of  the  child’s  age.  The
dimension  of  the  x  axis  is  years  and  the  dimension  of  the  y  axis  is  kilograms.  What  are  the
dimensions of the x and y axes of the familiar two-dimensional plot given in Figure P1-12(b)?
Figure P1-12
1.13 Let’s say you are writing software code to generate an x(n) test sequence composed of the sum
of two equal-amplitude discrete cosine waves, as
x(n) = cos(2πfonts + ϕ) + cos(2πfonts)
where ts is the time between your x(n) samples, and ϕ is a constant phase shift measured in radians.
An example x(n) when ϕ = π/2 is shown in Figure P1-13 where the x(n) sequence, represented by
the circular dots, is a single sinusoid whose frequency is fo Hz.
Figure P1-13
Using the trigonometric identity cos(α+β) + cos(α−β) = 2cos(α)cos(β), derive an equation for x(n)
that is of the form
where variables α and β are in terms of 2πfonts and ϕ.
x(n) = 2cos(α)cos(β)
1.14  In  your  engineering  education  you’ll  often  read  in  some  mathematical  derivation,  or  hear
someone say, “For small α, sin(α) = α.” (In fact, you’ll encounter that statement a few times in this
book.) Draw two curves defined by
x = α, and y = sin(α)
over the range of α = −π/2 to α = π/2, and discuss why that venerable “For small α, sin(α) = α”
statement is valid.
1.15 Considering two continuous (analog) sinusoids, having initial phase angles of α radians at time t
= 0, replace the following “?” characters with the correct angle arguments:
(a) sin(2πfot + α) = cos(?).
(b) cos(2πfot + α) = sin(?).
1.16  National  Instruments  Corp.  manufactures  an  A/D  converter,  Model  #NI  USB-5133,  that  is
capable  of  sampling  an  analog  signal  at  an  fs  sample  rate  of  100  megasamples  per  second  (100
MHz). The A/D converter has internal memory that can store up to 4×106 discrete samples. What is
the  maximum  number  of  cycles  of  a  25  MHz  analog  sinewave  that  can  be  stored  in  the  A/D
converter’s memory? Show your work.
1.17 In the first part of the text’s Section 1.5 we stated that for a process (or system) to be linear it
must satisfy a scaling property that we called the proportionality characteristic in the text’s Eq. (1-
14). Determine if the following processes have that proportionality characteristic:
(a) ya(n) = x(n−1)/6,
(b) yb(n) = 3 + x(n),
(c) yc(n) = sin[x(n)].
This problem is not “busy work.” Knowing if a process (or system) is linear tells us what signal
processing principles, and algorithms, can be applied in the analysis of that process (or system).
1.18 There is an often-used process in DSP called decimation, and in that process we retain some
samples of an x(n) input sequence and discard other x(n) samples. Decimation by a factor of two
can be described algebraically by
(P1-3)
where index m  =  0,1,2,3,.  .  .  The  decimation  defined  by  Eq. (P1-3)  means  that  y(m)  is  equal  to
alternate samples (every other sample) of x(n). For example:
y(0) = x(0), y(1) = x(2), y(2) = x(4), y(3) = x(6), . . .
and so on. Here is the question: Is that decimation process time invariant? Illustrate your answer by
decimating a simple sinusoidal x(n) time-domain sequence by a factor of two to obtain y(m). Next,
create a shifted-by-one-sample version of x(n) and call it xshift(n). That new sequence is defined by
(P1-4)
Finally,  decimate  xshift(n)  according  to  Eq. (P1-3)  to  obtain  yshift(m).  The  decimation  process  is
time  invariant  if  yshift(m)  is  equal  to  a  time-shifted  version  of  y(m).  That  is,  decimation  is  time
invariant if
yshift(m) = y(m+1).
1.19  In  Section  1.7  of  the  text  we  discussed  the  commutative  property  of  linear  time-invariant
systems. The two networks in Figure P1-19 exhibit that property. Prove this to be true by showing
that, given the same x(n) input sequence, outputs y1(n) and y2(n) will be equal.
Figure P1-19
1.20 Here we investigate several simple discrete processes that turn out to be useful in a number of
DSP  applications.  Draw  the  block  diagrams,  showing  their  inputs  as  x(n),  of  the  processes
described by the following difference equations:
(a) a 4th-order comb filter: yC(n) = x(n) − x(n−4),
(b) an integrator: yI(n) = x(n) + yI(n−1),
(c) a leaky integrator: yLI(n) = Ax(n) + (1−A)yLI(n−1) [the scalar value A is a real-valued constant
in the range 0 < A < 1],
(d) a differentiator: yD(n) = 0.5x(n) − 0.5x(n-2).
1.21 Draw the unit impulse responses (the output sequences when the input is a unit sample impulse
applied  at  time  n  =  0)  of  the  four  processes  listed  in  Problem  1.20.  Let  A  =  0.5  for  the  leaky
integrator. Assume that all sample values within the systems are zero at time n = 0.
1.22  DSP  engineers  involved  in  building  control  systems  often  need  to  know  what  is  the  step
response of a discrete system. The step response, ystep(n), can be defined in two equivalent ways.
One  way  is  to  say  that  ystep(n)  is  a  system’s  response  to  an  input  sequence  of  all  unity-valued
samples.  A  second  definition  is  that  ystep(n)  is  the  cumulative  sum  (the  accumulation,  discrete
integration) of that system’s unit impulse response yimp(n). Algebraically, this second definition of
step response is expressed as
In words, the above ystep(n) expression tells us: “The step response at time index n is equal to the
sum of all the previous impulse response samples up to and including yimp(n).” With that said, what
are  the  step  responses  of  the  four  processes  listed  in  Problem  1.20?  (Let  A  =  0.5  for  the  leaky
integrator.) Assume that all sample values within the system are zero at time n = 0.
1.23 Thinking about the spectra of signals, the ideal continuous (analog) squarewave s(t) in Figure
P1-23,  whose  fundamental  frequency  is  fo  Hz,  is  equal  to  the  sum  of  an  fo  Hz  sinewave  and  all
sinewaves whose frequencies are odd multiples of fo Hz. We call s(t) “ideal” because we assume
the amplitude transitions from plus and minus A occur instantaneously (zero seconds!). Continuous
Fourier  analysis  of  the  s(t)  squarewave  allows  us  to  describe  this  sum  of  frequencies  as  the
following infinite sum:
Figure P1-23
Using a summation symbol, we can express squarewave s(t) algebraically as
for n = odd integers only, showing s(t) to be an infinite sum of sinusoids.
(a)  Imagine  applying  s(t)  to  a  filter  that  completely  removes  s(t)’s  lowest-frequency  spectral
component. Draw the time-domain waveform at the output of such a filter.
(b)  Assume  s(t)  represents  a  voltage  whose  fo  fundamental  frequency  is  1  Hz,  and  we  wish  to
amplify  that  voltage  to  peak  amplitudes  of  ±2A.  Over  what  frequency  range  must  an  amplifier
operate  (that  is,  what  must  be  the  amplifier’s  passband width)  in  order  to  exactly  double  the
ideal 1 Hz squarewave’s peak-peak amplitude?
1.24  This  interesting  problem  illustrates  an  illegal  mathematical  operation  that  we  must  learn  to
avoid in our future algebraic activities. The following claims to be a mathematical proof that 4 = 5.
Which of the following steps is illegal? Explain why.
Proof that 4 = 5:
Step 1: 16 − 36 = 25 − 45
Step 2: 42 − 9 · 4 = 52 − 9 · 5
Step 3: 42 − 9 · 4 + 81/4 = 52 − 9 · 5 + 81/4
Step 4: (4 − 9/2)2 = (5 − 9/2)2
Step 5: 4 − 9/2 = 5 − 9/2
Step 6: 4 = 5

Chapter Two. Periodic Sampling
Periodic sampling, the process of representing a continuous signal with a sequence of discrete data
values, pervades the field of digital signal processing. In practice, sampling is performed by applying
a continuous signal to an analog-to-digital (A/D) converter whose output is a series of digital values.
Because sampling theory plays an important role in determining the accuracy and feasibility of any
digital signal processing scheme, we need a solid appreciation for the often misunderstood effects of
periodic sampling. With regard to sampling, the primary concern is just how fast a given continuous
signal  must  be  sampled  in  order  to  preserve  its  information  content.  We  can  sample  a  continuous
signal at any sample rate we wish, and we’ll obtain a series of discrete values—but the question is,
how well do these values represent the original signal? Let’s learn the answer to that question and, in
doing so, explore the various sampling techniques used in digital signal processing.
2.1 Aliasing: Signal Ambiguity in the Frequency Domain
There  is  a  frequency-domain  ambiguity  associated  with  discrete-time  signal  samples  that  does  not
exist  in  the  continuous  signal  world,  and  we  can  appreciate  the  effects  of  this  uncertainty  by
understanding the sampled nature of discrete data. By way of example, suppose you were given the
following sequence of values,
x(0) = 0
x(1) = 0.866
x(2) = 0.866
x(3) = 0
x(4) = −0.866
x(5) = −0.866
x(6) = 0,
and were told that they represent instantaneous values of a time-domain sinewave taken at periodic
intervals. Next, you were asked to draw that sinewave. You’d start by plotting the sequence of values
shown  by  the  dots  in  Figure 2-1(a).  Next,  you’d  be  likely  to  draw  the  sinewave,  illustrated  by  the
solid line in Figure 2-1(b), that passes through the points representing the original sequence.
Figure 2-1 Frequency ambiguity: (a) discrete-time sequence of values; (b) two different sinewaves
that pass through the points of the discrete sequence.
Another person, however, might draw the sinewave shown by the shaded line in Figure 2-1(b). We
see that the original sequence of values could, with equal validity, represent sampled values of both
sinewaves. The key issue is that if the data sequence represents periodic samples of a sinewave, we
cannot unambiguously determine the frequency of the sinewave from those sample values alone.
Reviewing the mathematical origin of this frequency ambiguity enables us not only to deal with it, but
to use it to our advantage. Let’s derive an expression for this frequency-domain ambiguity and, then,
look at a few specific examples. Consider the continuous time-domain sinusoidal signal defined as
(2-1)
This x(t) signal is a garden-variety sinewave whose frequency is fo Hz. Now let’s sample x(t) at a
rate of fs samples/second, i.e., at regular periods of ts seconds where ts = 1/fs. If we start sampling at
time t  =  0,  we  will  obtain  samples  at  times  0ts, 1ts, 2ts,  and  so  on.  So,  from  Eq. (2-1),  the  first  n
successive samples have the values
(2-2)
Equation (2-2) defines the value of the nth sample of our x(n) sequence to be equal to the original
sinewave at the time instant nts. Because two values of a sinewave are identical if they’re separated
by an integer multiple of 2π radians, i.e., sin(ø) = sin(ø+2πm) where m is any integer, we can modify
Eq. (2-2) as
(2-3)
If we let m be an integer multiple of n, m = kn, we can replace the m/n ratio in Eq. (2-3) with k so
that
(2-4)
Because fs = 1/ts, we can equate the x(n) sequences in Eqs. (2-2) and (2-4) as
(2-5)
The fo and (fo+kfs) factors in Eq. (2-5) are therefore equal. The implication of Eq. (2-5) is critical. It
means that an x(n) sequence of digital sample values, representing a sinewave of fo Hz, also exactly
represents  sinewaves  at  other  frequencies,  namely,  fo  +  kfs.  This  is  one  of  the  most  important
relationships in the field of digital signal processing. It’s the thread with which all sampling schemes
are woven. In words, Eq. (2-5) states:
When  sampling  at  a  rate  of  fs  samples/second,  if  k  is  any  positive  or  negative  integer,  we
cannot  distinguish  between  the  sampled  values  of  a  sinewave  of  fo  Hz  and  a  sinewave  of
(fo+kfs) Hz.
It’s true. No sequence of values stored in a computer, for example, can unambiguously represent one
and  only  one  sinusoid  without  additional  information.  This  fact  applies  equally  to  A/D-converter
output  samples  as  well  as  signal  samples  generated  by  computer  software  routines.  The  sampled
nature  of  any  sequence  of  discrete  values  makes  that  sequence  also  represent  an  infinite  number  of
different sinusoids.
Equation (2-5) influences all digital signal processing schemes. It’s the reason that, although we’ve
only  shown  it  for  sinewaves,  we’ll  see  in  Chapter  3  that  the  spectrum  of  any  discrete  series  of
sampled  values  contains  periodic  replications  of  the  original  continuous  spectrum.  The  period
between  these  replicated  spectra  in  the  frequency  domain  will  always  be  fs,  and  the  spectral
replications  repeat  all  the  way  from  DC  to  daylight  in  both  directions  of  the  frequency  spectrum.
That’s because k in Eq. (2-5)  can  be  any  positive  or  negative  integer.  (In  Chapters 5  and  6,  we’ll
learn  that  Eq.  (2-5)  is  the  reason  that  all  digital  filter  frequency  responses  are  periodic  in  the
frequency domain and is crucial to analyzing and designing a popular type of digital filter known as
the infinite impulse response filter.)
To illustrate the effects of Eq. (2-5), let’s build on Figure 2-1 and consider the sampling of a 7 kHz
sinewave  at  a  sample  rate  of  6  kHz.  A  new  sample  is  determined  every  1/6000  seconds,  or  once
every 167 microseconds, and their values are shown as the dots in Figure 2-2(a).
Figure 2-2 Frequency ambiguity effects of Eq. (2-5): (a) sampling a 7 kHz sinewave at a sample rate
of 6 kHz; (b) sampling a 4 kHz sinewave at a sample rate of 6 kHz; (c) spectral relationships showing
aliasing of the 7 and 4 kHz sinewaves.
Notice  that  the  sample  values  would  not  change  at  all  if,  instead,  we  were  sampling  a  1  kHz
sinewave.  In  this  example  fo  =  7  kHz,  fs  =  6  kHz,  and  k  =  −1  in  Eq. (2-5),  such  that  fo+kfs  =  [7+
(−1·6)] = 1 kHz. Our problem is that no processing scheme can determine if the sequence of sampled
values, whose amplitudes are represented by the dots, came from a 7 kHz or a 1 kHz sinusoid. If these
amplitude  values  are  applied  to  a  digital  process  that  detects  energy  at  1  kHz,  the  detector  output
would  indicate  energy  at  1  kHz.  But  we  know  that  there  is  no  1  kHz  tone  there—our  input  is  a
spectrally pure 7 kHz tone. Equation (2-5) is causing a sinusoid, whose name is 7 kHz, to go by the
alias  of  1  kHz.  Asking  someone  to  determine  which  sinewave  frequency  accounts  for  the  sample
values in Figure 2-2(a) is like asking, “When I add two numbers I get a sum of four. What are the two
numbers?” The answer is that there is an infinite number of number pairs that can add up to four.
Figure 2-2(b) shows another example of frequency ambiguity that we’ll call aliasing, where a 4 kHz
sinewave could be mistaken for a −2 kHz sinewave. In Figure 2-2(b), fo = 4 kHz, fs = 6 kHz, and k =
−1 in Eq. (2-5), so that fo+kfs = [4+(−1 · 6)] = −2 kHz. Again, if we examine a sequence of numbers
representing the dots in Figure 2-2(b), we could not determine if the sampled sinewave was a 4 kHz
tone  or  a  −2  kHz  tone.  (Although  the  concept  of  negative  frequencies  might  seem  a  bit  strange,  it
provides a beautifully consistent methodology for predicting the spectral effects of sampling. Chapter
8 discusses negative frequencies and how they relate to real and complex signals.)
Now,  if  we  restrict  our  spectral  band  of  interest  to  the  frequency  range  of  ±fs/2,  the  previous  two
examples  take  on  a  special  significance.  The  frequency  fs/2  is  an  important  quantity  in  sampling
theory and is referred to by different names in the literature, such as critical Nyquist, half Nyquist,
and folding frequency. A graphical depiction of our two frequency aliasing examples is provided in
Figure 2-2(c). We’re interested in signal components that are aliased into the frequency band between
−fs/2 and +fs/2. Notice in Figure 2-2(c) that within the spectral band of interest (±3 kHz, because fs =
6 kHz), there is energy at −2 kHz and +1 kHz, aliased from 4 kHz and 7 kHz, respectively. Note also
that  the  vertical  positions  of  the  dots  in  Figure 2-2(c) have no amplitude  significance  but  that  their
horizontal positions indicate which frequencies are related through aliasing.
A general illustration of aliasing is provided in the shark’s tooth pattern in Figure 2-3(a). Note how
the  peaks  of  the  pattern  are  located  at  integer  multiples  of  fs  Hz.  The  pattern  shows  how  signals
residing  at  the  intersection  of  a  horizontal  line  and  a  sloped  line  will  be  aliased  to  all  of  the
intersections of that horizontal line and all other lines with like slopes. For example, the pattern in
Figure 2-3(b) shows that our sampling of a 7 kHz sinewave at a sample rate of 6 kHz will provide a
discrete sequence of numbers whose spectrum ambiguously represents tones at 1 kHz, 7 kHz, 13 kHz,
19  kHz,  etc.  Let’s  pause  for  a  moment  and  let  these  very  important  concepts  soak  in  a  bit.  Again,
discrete  sequence  representations  of  a  continuous  signal  have  unavoidable  ambiguities  in  their
frequency  domains.  These  ambiguities  must  be  taken  into  account  in  all  practical  digital  signal
processing algorithms.
Figure 2-3 Shark’s tooth pattern: (a) aliasing at multiples of the sampling frequency; (b) aliasing of
the 7 kHz sinewave to 1 kHz, 13 kHz, and 19 kHz.
OK, let’s review the effects of sampling signals that are more interesting than just simple sinusoids.
2.2 Sampling Lowpass Signals
Consider  the  situation  of  sampling  a  signal  such  as  a  continuous  real-valued  lowpass  x(t)  signal
whose spectrum is shown in Figure 2-4(a). Notice that the spectrum is symmetrical around zero Hz,
and the spectral amplitude is zero above +B Hz and below −B Hz; i.e., the signal is band-limited.
(From  a  practical  standpoint,  the  term  band-limited  signal  merely  implies  that  any  signal  energy
outside the range of ±B Hz is below the sensitivity of our system.) The x(t) time signal is called a
lowpass signal because its spectral energy is low in frequency.
Figure 2-4 Spectral replications: (a) original continuous lowpass signal spectrum; (b) spectral
replications of the sampled lowpass signal when fs/2 > B; (c) frequency overlap and aliasing when
the sampling rate is too low because fs/2 < B.
Pausing for a moment, if the continuous x(t) signal were a voltage on a coax cable applied to the input
of an analog spectrum analyzer, we would only see the spectral energy over the positive-frequency
range of 0 to +B Hz on the analyzer’s screen. However, in our world of discrete signals (DSP) we
show  the  spectrum  of  real-valued  signals  as  having  both  positive-  and  negative-frequency  spectral
energy. Throughout this book we’ll repeatedly see why such spectral representations are often useful,
and  sometimes  mandatory  in  our  work.  The  mathematical  justification  for  two-sided  spectral
diagrams  is  provided  in  both  Chapters 3  and  8.  For  now,  we  request  the  reader’s  acceptance  that
Figure 2-4(a) is a valid representation of the spectrum of the continuous x(t) signal.
Given that the continuous x(t) signal, whose spectrum is shown in Figure 2-4(a), is sampled at a rate
of  fs  samples/second,  we  can  see  the  spectral  replication  effects  of  sampling  in  Figure  2-4(b)
showing the original spectrum in addition to an infinite number of replications. The period of spectral
replication  is  fs  Hz.  Figure  2-4(b)  is  the  spectrum  of  the  sequence  of  x(n)  sampled  values  of  the
continuous x(t) signal. (Although we stated in Section 1.1  that  frequency-domain  representations  of
discrete time-domain sequences are themselves discrete, the replicated spectra in Figure 2-4(b) are
shown  as  continuous  lines,  instead  of  discrete  dots,  merely  to  keep  the  figure  from  looking  too
cluttered. We’ll cover the full implications of discrete frequency spectra in Chapter 3.)
Let’s step back a moment and understand Figure 2-4 for all it’s worth. Figure 2-4(a) is the spectrum
of a continuous signal, a signal that can only exist in one of two forms. Either it’s a continuous signal
that  can  be  sampled,  through  A/D  conversion,  or  it  is  merely  an  abstract  concept  such  as  a
mathematical  expression  for  a  signal.  It  cannot  be  represented  in  a  digital  machine  in  its  current
band-limited  form.  Once  the  signal  is  represented  by  a  sequence  of  discrete  sample  values,  its
spectrum takes the replicated form of Figure 2-4(b).
The replicated spectra are not just figments of the mathematics; they exist and have a profound effect
on  subsequent  digital  signal  processing.†  The  replications  may  appear  harmless,  and  it’s  natural  to
ask,  “Why  care  about  spectral  replications?  We’re  only  interested  in  the  frequency  band  within
±fs/2.”  Well,  if  we  perform  a  frequency  translation  operation  or  induce  a  change  in  sampling  rate
through decimation or interpolation, the spectral replications will shift up or down right in the middle
of the frequency range of interest ±fs/2 and could cause problems[1]. Let’s see how we can control
the locations of those spectral replications.
† Toward the end of Section 5.9,  as  an  example  of  using  the  convolution  theorem,  another  derivation  of  periodic  sampling’s  replicated
spectra will be presented.
In practical A/D conversion schemes, fs is always greater than 2B to separate spectral replications at
the folding frequencies of ±fs/2. This very important relationship of fs ≥ 2B is known as the Nyquist
criterion. To illustrate why the term folding frequency is used, let’s lower our sampling frequency to
fs = 1.5B Hz. The spectral result of this undersampling is illustrated in Figure 2-4(c). The spectral
replications are now overlapping the original baseband spectrum centered at zero Hz. Limiting our
attention to the band ±fs/2 Hz, we see two very interesting effects. First, the lower edge and upper
edge of the spectral replications centered at +fs and −fs now lie in our band of interest. This situation
is  equivalent  to  the  original  spectrum  folding  to  the  left  at  +fs/2  and  folding  to  the  right  at  −fs/2.
Portions  of  the  spectral  replications  now  combine  with  the  original  spectrum,  and  the  result  is
aliasing errors. The discrete sampled values associated with the spectrum of Figure 2-4(c) no longer
truly represent the original input signal. The spectral information in the bands of −B to −B/2 and B/2
to B Hz has been corrupted. We show the amplitude of the aliased regions in Figure 2-4(c) as shaded
lines because we don’t really know what the amplitudes will be if aliasing occurs.
The  second  effect  illustrated  by  Figure  2-4(c)  is  that  the  entire  spectral  content  of  the  original
continuous signal is now residing in the band of interest between −fs/2 and +fs/2. This key property
was true in Figure 2-4(b) and will always be true, regardless of the original signal or the sample rate.
This  effect  is  particularly  important  when  we’re  digitizing  (A/D  converting)  continuous  signals.  It
warns  us  that  any  signal  energy  located  above  +B  Hz  and  below  −B  Hz  in  the  original  continuous
spectrum of Figure 2-4(a) will always end up in the band of interest after sampling, regardless of the
sample rate. For this reason, continuous (analog) lowpass filters are necessary in practice.
We illustrate this notion by showing a continuous signal of bandwidth B accompanied by noise energy
in Figure 2-5(a). Sampling this composite continuous signal at a rate that’s greater than 2B prevents
replications of the signal of interest from overlapping each other, but all of the noise energy still ends
up  in  the  range  between  −fs/2  and  +fs/2  of  our  discrete  spectrum  shown  in  Figure  2-5(b).  This
problem is solved in practice by using an analog lowpass anti-aliasing filter prior to A/D conversion
to  attenuate  any  unwanted  signal  energy  above  +B  and  below  −B  Hz  as  shown  in  Figure  2-6.  An
example  lowpass  filter  response  shape  is  shown  as  the  dotted  line  superimposed  on  the  original
continuous signal spectrum in Figure 2-6. Notice how the output spectrum of the lowpass filter has
been band-limited, and spectral aliasing is avoided at the output of the A/D converter.
Figure 2-5 Spectral replications: (a) original continuous signal-plus-noise spectrum; (b) discrete
spectrum with noise contaminating the signal of interest.
Figure 2-6 Lowpass analog filtering prior to sampling at a rate of fs Hz.
As a historical note, the notion of periodic sampling was studied by various engineers, scientists, and
mathematicians  such  as  the  Russian  V.  Kotelnikov,  the  Swedish-born  H.  Nyquist,  the  Scottish  E.
Whittaker, and the Japanese I. Someya[2]. But it was the American Claude Shannon, acknowledging
the work of others, that formalized the concept of periodic sampling as we know it today and brought
it  to  the  broad  attention  of  communications  engineers[3].  That  was  in  1948—the  birth  year  of  the
transistor, marshmallows, and this author.
This  completes  the  discussion  of  simple  lowpass  sampling.  Now  let’s  go  on  to  a  more  advanced
sampling topic that’s proven so useful in practice.
2.3 Sampling Bandpass Signals
Although  satisfying  the  majority  of  sampling  requirements,  the  sampling  of  lowpass  signals,  as  in
Figure  2-6,  is  not  the  only  sampling  scheme  used  in  practice.  We  can  use  a  technique  known  as
bandpass sampling  to  sample  a  continuous  bandpass  signal  that  is  centered  about  some  frequency
other than zero Hz. When a continuous input signal’s bandwidth and center frequency permit us to do
so,  bandpass  sampling  not  only  reduces  the  speed  requirement  of  A/D  converters  below  that
necessary with traditional lowpass sampling; it also reduces the amount of digital memory necessary
to capture a given time interval of a continuous signal.
By way of example, consider sampling the band-limited signal shown in Figure 2-7(a) centered at fc
=  20  MHz,  with  a  bandwidth  B  =  5  MHz.  We  use  the  term  bandpass sampling  for  the  process  of
sampling  continuous  signals  whose  center  frequencies  have  been  translated  up  from  zero  Hz.  What
we’re calling bandpass sampling goes by various other names in the literature, such as IF sampling,
harmonic  sampling[4],  sub-Nyquist  sampling,  and  undersampling[5].  In  bandpass  sampling,  we’re
more  concerned  with  a  signal’s  bandwidth  than  its  highest-frequency  component.  Note  that  the
negative frequency portion of the signal, centered at −fc, is the mirror image of the positive frequency
portion—as  it  must  be  for  real  signals.  Our  bandpass  signal’s  highest-frequency  component  is  22.5
MHz.  Conforming  to  the  Nyquist  criterion  (sampling  at  twice  the  highest-frequency  content  of  the
signal) implies that the sampling frequency must be a minimum of 45 MHz. Consider the effect if the
sample rate is 17.5 MHz shown in Figure 2-7(b). Note that the original spectral components remain
located at ±fc, and spectral replications are located exactly at baseband, i.e., butting up against each
other at zero Hz. Figure 2-7(b) shows that sampling at 45 MHz was unnecessary to avoid aliasing—
instead we’ve used the spectral replicating effects of Eq. (2-5) to our advantage.
Figure 2-7 Bandpass signal sampling: (a) original continuous signal spectrum; (b) sampled signal
spectrum replications when sample rate is 17.5 MHz.
Bandpass sampling performs digitization and frequency translation in a single process, often called
sampling  translation.  The  processes  of  sampling  and  frequency  translation  are  intimately  bound
together in the world of digital signal processing, and every sampling operation inherently results in
spectral replications. The inquisitive reader may ask, “Can we sample at some still lower rate and
avoid aliasing?” The answer is yes, but, to find out how, we have to grind through the derivation of an
important bandpass sampling relationship. Our reward, however, will be worth the trouble because
here’s where bandpass sampling really gets interesting.
Let’s assume we have a continuous input bandpass signal of bandwidth B. Its carrier frequency is fc
Hz,  i.e.,  the  bandpass  signal  is  centered  at  fc  Hz,  and  its  sampled  value  spectrum  is  that  shown  in
Figure 2-8(a). We can sample that continuous signal at a rate, say fs′ Hz, so the spectral replications of
the  positive  and  negative  bands,  Q  and  P,  just  butt  up  against  each  other  exactly  at  zero  Hz.  This
situation,  depicted  in  Figure  2-8(a),  is  reminiscent  of  Figure  2-7(b).  With  an  arbitrary  number  of
replications, say m, in the range of 2fc − B, we see that
(2-6)
Figure 2-8 Bandpass sampling frequency limits: (a) sample rate fs′ = (2fc − B)/6; (b) sample rate is
less than fs′; (c) minimum sample rate fs″ < fs′.
In Figure 2-8(a), m = 6 for illustrative purposes only. Of course m can be any positive integer so long
as fs′ is never less than 2B. If the sample rate fs′ is increased, the original spectra (bold) do not shift,
but all the replications will shift. At zero Hz, the P band will shift to the right, and the Q band will
shift  to  the  left.  These  replications  will  overlap  and  aliasing  occurs.  Thus,  from  Eq.  (2-6),  for  an
arbitrary m, there is a frequency that the sample rate must not exceed, or
(2-7)
If  we  reduce  the  sample  rate  below  the  fs′  value  shown  in  Figure  2-8(a),  the  spacing  between
replications will decrease in the direction of the arrows in Figure 2-8(b). Again, the original spectra
do  not  shift  when  the  sample  rate  is  changed.  At  some  new  sample  rate  fs″,  where  fs″  <  fs′,  the
replication P′ will just butt up against the positive original spectrum centered at fc as shown in Figure
2-8(c). In this condition, we know that
(2-8)
Should fs″ be decreased in value, P′ will shift further down in frequency and start to overlap with the
positive original spectrum at fc and aliasing occurs. Therefore, from Eq. (2-8) and for m+1, there is a
frequency that the sample rate must always exceed, or
(2-9)
We can now combine Eqs. (2-7) and (2-9) to say that fs may be chosen anywhere in the range between
fs″ and fs′ to avoid aliasing, or
(2-10)
where m is an arbitrary, positive integer ensuring that fs ≥ 2B. (For this type of periodic sampling of
real  signals,  known  as  real  or  1st-order  sampling,  the  Nyquist  criterion  fs  ≥  2B  must  still  be
satisfied.)
To appreciate the important relationships in Eq. (2-10), let’s return to our bandpass signal example,
where Eq. (2-10) enables the generation of Table 2-1. This table tells us that our sample rate can be
anywhere in the range of 22.5 to 35 MHz, anywhere in the range of 15 to 17.5 MHz, or anywhere in
the range of 11.25 to 11.66 MHz. Any sample rate below 11.25 MHz is unacceptable because it will
not satisfy Eq. (2-10) as well as fs ≥ 2B. The spectra resulting from several of the sampling rates from
Table 2-1 are shown in Figure 2-9 for our bandpass signal example. Notice in Figure 2-9(f) that when
fs equals 7.5 MHz (m = 5), we have aliasing problems because neither the greater-than relationships
in Eq. (2-10) nor fs ≥ 2B have been satisfied. The m = 4 condition is also unacceptable because fs ≥
2B  is  not  satisfied.  The  last  column  in  Table  2-1  gives  the  optimum  sampling  frequency  for  each
acceptable m  value.  Optimum  sampling  frequency  is  defined  here  as  that  frequency  where  spectral
replications  butt  up  against  each  other  at  zero  Hz.  For  example,  in  the  m  =  1  range  of  permissible
sampling frequencies, it is much easier to perform subsequent digital filtering or other processing on
the signal samples whose spectrum is that of Figure 2-9(b), as opposed to the spectrum in Figure 2-
9(a).
Table 2-1 Equation (2-10) Applied to the Bandpass Signal Example
Figure 2-9 Various spectral replications from Table 2-1: (a) fs = 35 MHz; (b) fs = 22.5 MHz; (c) fs =
17.5 MHz; (d) fs = 15 MHz; (e) fs = 11.25 MHz; (f) fs = 7.5 MHz.
2.4 Practical Aspects of Bandpass Sampling
Now  that  we’re  familiar  with  the  theory  of  bandpass  sampling,  let’s  discuss  a  few  aspects  of
bandpass sampling in practical applications.
2.4.1 Spectral Inversion in Bandpass Sampling
Some of the permissible fs values from Eq. (2-10) will, although avoiding aliasing problems, provide
a sampled baseband spectrum (located near zero Hz) that is inverted from the original analog signal’s
positive and negative spectral shapes. That is, the positive-frequency sampled baseband will have the
inverted shape of the negative half from the original analog spectrum. This spectral inversion happens
whenever m, in Eq. (2-10), is an odd integer, as illustrated in Figures 2-9(c) and 2-9(d). When the
original  positive  spectral  bandpass  components  are  symmetrical  about  the  fc  frequency,  spectral
inversion presents no problem and any nonaliasing value for fs from Eq. (2-10) may be chosen.
However, if spectral inversion is something to be avoided, for example, when single sideband signals
are being processed, the applicable sample rates to avoid spectral inversion are defined by Eq. (2-
10) with the restriction that m is an even integer and fs > 2B is satisfied.
Now  here’s  some  good  news.  With  a  little  additional  digital  processing  we  can  sample  at  rates
defined by Eq. (2-10) with odd m, with their spectral inversion, and easily reinvert the spectrum back
to its original orientation. The discrete spectrum of any digital signal can be inverted by multiplying
the signal’s discrete-time samples by a sequence of alternating plus ones and minus ones (1, −1, 1,
−1, etc.), indicated in the literature by the succinct expression (−1)n.
Although  multiplying  time  samples  by  (−1)n  is  explored  in  detail  in  Section  13.1,  all  we  need  to
remember at this point is the simple rule that multiplication of real signal samples by (−1)n flips the
positive-frequency band of interest, from zero to +fs/2 Hz, where the center of the flipping is fs/4 Hz.
Likewise,  the  multiplication  flips  the  negative  frequency  band  of  interest,  from  −fs/2  to  zero  Hz,
where  the  center  of  the  flipping  is  −fs/4  Hz  as  shown  in  Figure  2-10.  In  the  literature  of  DSP,
occasionally you’ll see the (−1)n sequence expressed by the equivalent expression cos(πn).
Figure 2-10 Spectral inversion through multiplication by (−1)n: (a) spectrum of original x(n); (b)
spectrum of (−1)n · x(n).
2.4.2 Positioning Sampled Spectra at fs/4
In many signal processing applications we’ll find it useful to use an fs  bandpass  sampling  rate  that
forces the sampled spectra to be centered exactly at ±fs/4 as shown in Figure 2-10(a). As we’ll see in
later  chapters,  this  scenario  greatly  simplifies  certain  common  operations  such  as  digital  filtering,
complex down-conversion, and Hilbert transformations.
To ensure that sampled spectra reside at ±fs/4, we select fs using
(2-11)
where fc is the center frequency of the original analog signal’s bandpass signal.
2.4.3 Noise in Bandpass-Sampled Signals
We have painted a rosy picture of bandpass sampling, with its analog signal capture capabilities at
reduced sample rates. However, there is a negative aspect associated with bandpass sampling. The
signal-to-noise ratio (SNR), the ratio of the power of a signal over the total background noise power,
of  our  digitized  signal  is  degraded  when  we  perform  bandpass  sampling.  (A  general  discussion  of
SNR is provided in Appendix D.)
Here’s  the  story.  The  spectrum  of  an  analog  lowpass  signal,  output  from  an  analog  anti-aliasing
lowpass  filter,  is  that  shown  in  Figure  2-11(a).  That  lowpass  signal  contains  some  amount  of
background  noise  power.  Now  if  an  analog  bandpass  signal  is  likewise  contaminated  with
background noise, as shown by the spectral plot in Figure 2-11(b), the bandpass-sampled signal will
have an increased level of background noise as shown in Figure 2-11(c).  That’s  because  all  of  the
background spectral noise in Figure 2-11(b) must now reside in the range of −fs/2 to fs/2 in Figure 2-
11(c). As such, the bandpass-sampled signal’s SNR is reduced (degraded).
Figure 2-11 Sampling SNR degradation: (a) analog lowpass signal spectral power; (b) analog
bandpass signal spectral power; (c) bandpass-sampled signal spectral power when m = 1.
As detailed in reference [6], if the analog bandpass signal’s background noise spectral power level is
relatively flat, as in Figure 2-11(b),  the  bandpass-sampled  background  noise  power  increases  by  a
factor of m + 1 (the denominator of the right-side ratio in Eq. (2-10)) while the desired signal power
P remains unchanged. As such, the bandpass-sampled signal’s SNR, measured in decibels, is reduced
by
(2-12)
below  the  SNR  of  the  original  analog  signal.  So  for  the  Figure  2-11  example,  when  m  =  1,  the
bandpass-sampled signal’s background noise power doubles, and the total bandpass-sampled signal’s
SNR is DSNR = 3 dB less than the analog bandpass signal’s SNR.
The notion of using decibels, a very convenient method of comparing the power of two signals (the
two signals, in this case, are our bandpass signal and the background noise signal), is discussed in
Appendix E.
References
[1]  Crochiere,  R.,  and  Rabiner,  L.  “Optimum  FIR  Digital  Implementations  for  Decimation,
Interpolation, and Narrow-band Filtering,” IEEE Trans. on Acoust. Speech, and Signal Proc., Vol.
ASSP-23, No. 5, October 1975.
[2] Luke, H. “The Origins of the Sampling Theorem,” IEEE Communications Magazine, April 1999,
pp. 106–109.
[3] Shannon, C. “A Mathematical Theory of Communication,” Bell Sys. Tech. Journal, Vol. 27, 1948,
pp. 379–423, 623–656.
[4] Steyskal, H. “Digital Beamforming Antennas,” Microwave Journal, January 1987.
[5] Hill, G. “The Benefits of Undersampling,” Electronic Design, July 11, 1994.
[6] Vaughan, R., et al., “The Theory of Bandpass Sampling,” IEEE Trans. on Signal Processing, Vol.
39, No. 9, September 1991, pp. 1973–1984.
Chapter 2 Problems
2.1 Suppose you have a mechanical clock that has a minute hand, but no hour hand. Next, suppose you
took  a  photograph  of  the  clock  when  the  minute  hand  was  pointed  at  12:00  noon  and  then  took
additional photos every 55 minutes. Upon showing those photos, in time order, to someone:
(a)  What  would  that  person  think  about  the  direction  of  motion  of  the  minute  hand  as  time
advances?
(b) With the idea of lowpass sampling in mind, how often would you need to take photos, measured
in photos/hour, so that the successive photos show proper (true) clockwise minute-hand rotation?
2.2 Assume we sampled a continuous x(t) signal and obtained 100 x(n) time-domain samples. What
important information (parameter that we need to know in order to analyze x(t)) is missing from the
x(n) sequence?
2.3  National  Instruments  Corporation  produces  an  analog-to-digital  (A/D)  converter  (Model  #NI-
5154) that can sample (digitize) an analog signal at a sample rate of fs = 2.0 GHz (gigahertz).
(a) What is the ts period of the output samples of such a device?
(b) Each A/D output sample is an 8-bit binary word (one byte), and the converter is able to store
256  million  samples.  What  is  the  maximum  time  interval  over  which  the  converter  can
continuously sample an analog signal?
2.4 Consider a continuous time-domain sinewave, whose cyclic frequency is 500 Hz, defined by
x(t) = cos[2π(500)t + π/7].
Write the equation for the discrete x(n) sinewave sequence that results from sampling x(t) at an fs
sample rate of 4000 Hz.
Note: This problem is not “busy work.” If you ever want to model the x(t) signal using software
(MathCAD, MATLAB, Octave, etc.), then it is the desired x(n) equation that you program into your
software.
2.5 If we sampled a single continuous sinewave whose frequency is fo Hz, over what range must ts
(the time between digital samples) be to satisfy the Nyquist criterion? Express that ts range in terms
of fo.
2.6 Suppose we used the following statement to describe the Nyquist criterion for lowpass sampling:
“When sampling a single continuous sinusoid (a single analog tone), we must obtain no fewer than
N discrete samples per continuous sinewave cycle.” What is the value of this integer N?
2.7  The  Nyquist  criterion,  regarding  the  sampling  of  lowpass  signals,  is  sometimes  stated  as  “The
sampling  rate  fs  must  be  equal  to,  or  greater  than,  twice  the  highest  spectral  component  of  the
continuous  signal  being  sampled.”  Can  you  think  of  how  a  continuous  sinusoidal  signal  can  be
sampled  in  accordance  with  that  Nyquist  criterion  definition  to  yield  all  zero-valued  discrete
samples?
2.8  Stock  market  analysts  study  time-domain  charts (plots) of the closing price  of  stock  shares.  A
typical plot takes the form of that in Figure P2-8, where instead of plotting discrete closing price
sample values as dots, they draw straight lines connecting the closing price value samples. What is
the ts period for such stock market charts?
Figure P2-8
2.9 Consider a continuous time-domain sinewave defined by
x(t) = cos(4000πt)
that was sampled to produce the discrete sinewave sequence defined by
x(n) = cos(nπ/2).
What is the fs sample rate, measured in Hz, that would result in sequence x(n)?
2.10 Consider the two continuous signals defined by
a(t) = cos(4000πt) and b(t) = cos(200πt)
whose product yields the x(t) signal shown in Figure P2-10. What is the minimum fs sample rate,
measured  in  Hz,  that  would  result  in  a  sequence  x(n)  with  no  aliasing  errors  (no  spectral
replication overlap)?
Figure P2-10
2.11 Consider a discrete time-domain sinewave sequence defined by
x(n) = sin(nπ/4)
that was obtained by sampling an analog x(t) = sin(2πfot) sinewave signal whose frequency is fo
Hz. If the sample rate of x(n) is fs = 160 Hz, what are three possible positive frequency values,
measured in Hz, for fo that would result in sequence x(n)?
2.12 In the text we discussed the notion of spectral folding that can take place when an xa(t) analog
signal  is  sampled  to  produce  a  discrete  xd(n)  sequence.  We  also  stated  that  all  of  the  analog
spectral  energy  contained  in  Xa(f)  will  reside  within  the  frequency  range  of  ±fs/2  of  the  Xd(f)
spectrum of the sampled xd(n) sequence. Given those concepts, consider the spectrum of an analog
signal shown in Figure P2-12(a) whose spectrum is divided into the six segments marked as 1 to 6.
Fill in the following table showing which of the A-to-F spectral segments of Xd(f), shown in Figure
P2-12(b), are aliases of the 1-to-6 spectral segments of Xa(f).
Figure P2-12
2.13 Consider the simple analog signal defined by x(t) = sin(2π700t) shown in Figure P2-13. Draw
the  spectrum  of  x(n)  showing  all  spectral  components,  labeling  their  frequency  locations,  in  the
frequency range −2fs to +2fs.
Figure P2-13
2.14 The Nançay Observatory, in France, uses a radio astronomy receiver that generates a wideband
analog s(t) signal whose spectral magnitude is represented in Figure P2-14. The Nançay scientists
bandpass sample the analog s(t) signal, using an analog-to-digital (A/D) converter to produce an
x(n) discrete sequence, at a sample rate of fs = 56 MHz.
Figure P2-14
(a) Draw the spectrum of the x(n) sequence, X(f), showing its spectral energy over the frequency
range −70 MHz to 70 MHz.
(b) What is the center frequency of the first positive-frequency spectral replication in X(f)?
(c) How is your solution to Part (b) related to the fs sample rate?
Hint: How is your solution to Part (b) related to fs/2?
2.15 Think about the continuous (analog) signal x(t) that has the spectral magnitude shown in Figure
P2-15.  What  is  the  minimum  fs  sample  rate  for  lowpass sampling  such  that  no  spectral  overlap
occurs in the frequency range of 2 to 9 kHz in the spectrum of the discrete x(n) samples?
Figure P2-15
2.16 If a person wants to be classified as a soprano in classical opera, she must be able to sing notes
in the frequency range of 247 Hz to 1175 Hz. What is the minimum fs sampling rate allowable for
bandpass sampling of the full audio spectrum of a singing soprano?
2.17  This  problem  requires  the  student  to  have  some  knowledge  of  electronics  and  how  a  mixer
operates inside a radio. (The definition of a bandpass filter is given in Appendix F.) Consider the
simplified version of what is called a superheterodyne digital radio depicted in Figure P2-17.
Figure P2-17
(a) For what local oscillator frequency, fLO, would an image (a copy, or duplication) of the w(t)
signal’s spectrum be centered at 15 MHz (megahertz) in signal u(t)?
(b) What is the purpose of the analog bandpass filter #2?
(c) Fill in the following table showing all ranges of acceptable fs bandpass sampling rates to avoid
aliasing errors in the discrete x(n) sequence. Also list, in the rightmost column, for which values
of m the sampled spectrum, centered at 15 MHz, will be inverted.
(d)  In  digital  receivers,  to  simplify  AM  and  FM  demodulation,  it  is  advantageous  to  have  the
spectrum of the discrete x(n) sequence be centered at one-quarter of the sample rate. The text’s
Eq. 2-11 describes how to achieve this situation. If we were constrained to have fs equal to 12
MHz, what would be the maximum fLO  local  oscillator  frequency  such  that  the  spectra  of  u(t),
x(t),  and  x(n)  are  centered  at  fs/4?  (Note:  In  this  scenario,  the  fc  center  frequency  of  analog
bandpass filter #2 will no longer be 15 MHz.)
2.18  Think  about  the  analog  anti-aliasing  filter  given  in  Figure  P2-18(a),  having  a  one-sided
bandwidth of B Hz. A wideband analog signal passed through that filter, and then sampled, would
have an |X(m)| spectrum as shown in Figure P2-18(b), where the dashed curves represent spectral
replications.
Figure P2-18
Suppose  we  desired  that  all  aliased  spectral  components  in  |X(m)|  over  our  B  Hz  bandwidth  of
interest  must  be  attenuated  by  at  least  60  dB.  Determine  the  equation,  in  terms  of  B  and  the  fs
sampling rate, for the frequency at which the anti-aliasing filter must have an attenuation value of
−60 dB. The solution to this problem gives us a useful rule of thumb we can use in specifying the
desired performance of analog anti-aliasing filters.
2.19  This  problem  demonstrates  a  popular  way  of  performing  frequency  down-conversion
(converting a bandpass signal into a lowpass signal) by way of bandpass sampling. Consider the
continuous 250-Hz-wide bandpass x(t) signal whose spectral magnitude is shown in Figure P2-19.
Draw  the  spectrum,  over  the  frequency  range  of  −1.3fs  to  +1.3fs,  of  the  x(n)  sampled  sequence
obtained when x(t) is sampled at fs = 1000 samples/second.
Figure P2-19
2.20 Here’s a problem to test your understanding of bandpass sampling. Think about the continuous
(analog) signal x(t) that has the spectral magnitude shown in Figure P2-20.
Figure P2-20
(a) What is the minimum fc center frequency, in terms of x(t)’s bandwidth B, that enables bandpass
sampling of x(t)? Show your work.
(b) Given your results in Part (a) above, determine if it is possible to perform bandpass sampling
of the full spectrum of the commercial AM (amplitude modulation) broadcast radio band in North
America. Explain your solution.
2.21  Suppose  we  want  to  perform  bandpass  sampling  of  a  continuous  5  kHz-wide  bandpass  signal
whose spectral magnitude is shown in Figure P2-21.
Figure P2-21
Fill  in  the  following  table  showing  the  various  ranges  of  acceptable  fs  bandpass  sampling  rates,
similar  to  the  text’s  Table  2-1,  to  avoid  aliasing  errors.  Also  list,  in  the  rightmost  column,  for
which values of m the sampled spectrum in the vicinity of zero Hz is inverted.
Acceptable Bandpass Sample Rate Ranges
2.22 I recently encountered an Internet website that allegedly gave an algorithm for the minimum fs
bandpass sampling rate for an analog bandpass signal centered at fc Hz, whose bandwidth is B Hz.
The algorithm is
where
 means the integer part of x. Here’s the problem: Is the above fs,min algorithm
In the above notation,
correct  in  computing  the  absolute minimum  possible  nonaliasing  fs  bandpass  sampling  rate  for  an
analog  bandpass  signal  centered  at  fc  Hz,  whose  bandwidth  is  B  Hz?  Verify  your  answer  with  an
example.
