# Understanding DSP (Lyons) — Chunk 10/27

> Source: `UnderstandingDigitalSignalProcessing.pdf` · Pages ~1-858 · Extractor: pdfminer.six
> Target ≈ 2800 words, Overlap = 200 words · Actual = 51079

> Note: DSP textbook content for SpectralCanvasPro RT-audio knowledge base

---
Chapter Five. Finite Impulse Response Filters
The  filtering  of  digitized  data,  if  not  the  most  fundamental,  is  certainly  the  oldest  discipline  in  the
field  of  digital  signal  processing.  Digital  filtering’s  origins  go  back  50  years.  The  growing
availability of digital computers in the early 1950s led to efforts in the smoothing of discrete sampled
data  and  the  analysis  of  discrete  data  control  systems.  However,  it  wasn’t  until  the  early  to  mid-
1960s,  around  the  time  the  Beatles  came  to  America,  that  the  analysis  and  development  of  digital
equivalents of analog filters began in earnest. That’s when digital signal processing experts realized
that  computers  could  go  beyond  the  mere  analysis  of  digitized  signals  into  the  domain  of  actually
changing  signal  characteristics  through  filtering.  Today,  digital  filtering  is  so  widespread  that  the
quantity of literature pertaining to it exceeds that of any other topic in digital signal processing. In this
chapter,  we  introduce  the  fundamental  attributes  of  digital  filters,  learn  how  to  quantify  their
performance, and review the principles associated with the design of finite impulse response digital
filters.
So let’s get started by illustrating the concept of filtering a time-domain signal as shown in Figure 5-
1.
Figure 5-1 Filters: (a) an analog filter with a noisy tone input and a reduced-noise tone output; (b) the
digital equivalent of the analog filter.
In general, filtering is the processing of a time-domain signal resulting in some change in that signal’s
original spectral content. The change is usually the reduction, or filtering out, of some unwanted input
spectral  components;  that  is,  filters  allow  certain  frequencies  to  pass  while  attenuating  other
frequencies.  Figure  5-1  shows  both  analog  and  digital  versions  of  a  filtering  process.  Where  an
analog filter operates on a continuous signal, a digital filter processes a sequence of discrete sample
values.  The  digital  filter  in  Figure 5-1(b),  of  course,  can  be  a  software  program  in  a  computer,  a
programmable hardware processor, or a dedicated integrated circuit. Traditional linear digital filters
typically  come  in  two  flavors:  finite  impulse  response  (FIR)  filters  and  infinite  impulse  response
(IIR) filters. Because FIR filters are the simplest type of digital filter to analyze, we’ll examine them
in this chapter and cover IIR filters in Chapter 6.
5.1 An Introduction to Finite Impulse Response (FIR) Filters
Given  a  finite  duration  of  nonzero  input  values,  an  FIR  filter  will  always  have  a  finite  duration  of
nonzero output values, and that’s how FIR filters got their name. So, if the FIR filter’s input suddenly
becomes a sequence of all zeros, the filter’s output will eventually be all zeros. While not sounding
all that unusual, this characteristic is, however, very important, and we’ll soon find out why, as we
learn more about digital filters.
FIR  filters  use  addition  to  calculate  their  outputs  in  a  manner  much  the  same  as  the  process  of
averaging  uses  addition.  In  fact,  averaging  is  a  kind  of  FIR  filter  that  we  can  illustrate  with  an
example. Let’s say we’re counting the number of cars that pass over a bridge every minute, and we
need to know the average number of cars per minute over five-minute intervals; that is, every minute
we’ll calculate the average number of cars/minute over the last five minutes. If the results of our car
counting for the first ten minutes are those values shown in the center column of Table 5-1, then the
average  number  of  cars/minute  over  the  previous  five  one-minute  intervals  is  listed  in  the  right
column  of  the  table.  We’ve  added  the  number  of  cars  for  the  first  five  one-minute  intervals  and
divided  by  5  to  get  our  first  five-minute  average  output  value,  (10+22+24+42+37)/5  =  27.  Next
we’ve averaged the number of cars/minute for the second to the sixth one-minute intervals to get our
second five-minute average output of 40.4. Continuing, we average the number of cars/minute for the
third to the seventh one-minute intervals to get our third average output of 53.8, and so on. With the
number of cars/minute for the one-minute intervals represented by the dashed line in Figure 5-2, we
show  our  five-minute  average  output  as  the  solid  line.  (Figure 5-2  shows  cars/minute  input  values
beyond  the  first  ten  minutes  listed  in  Table  5-1  to  illustrate  a  couple  of  important  ideas  to  be
discussed shortly.)
Table 5-1 Values for the Averaging Example
Figure 5-2 Averaging the number of cars/minute. The dashed line shows the individual cars/minute,
and the solid line is the number of cars/minute averaged over the last five minutes.
There’s  much  to  learn  from  this  simple  averaging  example.  In  Figure  5-2,  notice  that  the  sudden
changes in our input sequence of cars/minute are flattened out by our averager. The averager output
sequence is considerably smoother than the input sequence. Knowing that sudden transitions in a time
sequence  represent  high-frequency  components,  we  can  say  that  our  averager  is  behaving  like  a
lowpass filter and smoothing sudden changes in the input. Is our averager an FIR filter? It sure is—no
previous averager output value is used to determine a current output value; only input values are used
to calculate output values. In addition, we see that, if the bridge were suddenly closed at the end of
the 19th minute, the dashed line immediately goes to zero cars/minute at the end of the 20th minute,
and the averager’s output in Figure 5-2 approaches and settles to a value of zero by the end of the
24th minute.
Figure 5-2 shows the first averager output sample occurring at the end of the 5th minute because that’s
when we first have five input samples to calculate a valid average. The 5th output of our averager can
be denoted as yave(5) where
(5-1)
In the general case, if the kth input sample is x(k), then the nth output is
(5-2)
Look at Eq. (5-2) carefully now. It states that the nth output is the average of the nth input sample and
the four previous input samples.
We can formalize the digital filter nature of our averager by creating the block diagram in Figure 5-3
showing how the averager calculates its output samples.
Figure 5-3 Averaging filter block diagram when the fifth input sample value, 37, is applied.
This  block  diagram,  referred  to  as  the  filter  structure,  is  a  physical  depiction  of  how  we  might
calculate our averaging filter outputs with the input sequence of values shifted, in order, from left to
right along the top of the filter as new output calculations are performed. This structure, implementing
Eqs. (5-1) and (5-2), shows those values used when the first five input sample values are available.
The  delay  elements  in  Figure 5-3,  called  unit  delays,  merely  indicate  a  shift  register  arrangement
where input sample values are temporarily stored during an output calculation.
In averaging, we add five numbers and divide the sum by 5 to get our answer. In a conventional FIR
filter implementation, we can just as well multiply each of the five input samples by the coefficient
1/5 and then perform the summation as shown in Figure 5-4(a). Of course, the two methods in Figures
5-3  and  5-4(a)  are  equivalent  because  Eq.  (5-2)  describing  the  structure  shown  in  Figure  5-3  is
equivalent to
(5-3)
Figure 5-4 Alternate averaging filter structure: (a) input values used for the fifth output value; (b)
input values used for the sixth output value; (c) input values used for the seventh output value.
which describes the structure in Figure 5-4(a).†
† We’ve used the venerable distributive law for multiplication and addition of scalars, a(b+c+d) = ab+ac+ad, in moving Eq. (5-2)’s factor
of 1/5 inside the summation in Eq. (5-3).
Let’s make sure we understand what’s happening in Figure 5-4(a). Each of the first five input values
is multiplied by 1/5, and the five products are summed to give the fifth filter output value. The left-to-
right sample shifting is illustrated in Figures 5-4(b) and 5-4(c). To calculate the filter’s sixth output
value,  the  input  sequence  is  right-shifted,  discarding  the  first  input  value  of  10,  and  the  sixth  input
value,  77,  is  accepted  on  the  left.  Likewise,  to  calculate  the  filter’s  seventh  output  value,  the  input
sequence is right-shifted, discarding the second value of 22, and the seventh input value, 89, arrives
on the left. So, when a new input sample value is applied, the filter discards the oldest sample value,
multiplies  the  samples  by  the  coefficients  of  1/5,  and  sums  the  products  to  get  a  single  new  output
value.  The  filter’s  structure  using  this  bucket  brigade  shifting  process  is  often  called  a  transversal
filter due to the cross-directional flow of the input samples. Because we tap off five separate input
sample values to calculate an output value, the structure in Figure 5-4 is called a 5-tap tapped-delay
line FIR filter, in digital filter vernacular.
One important and, perhaps, most interesting aspect of understanding FIR filters is learning how to
predict their behavior when sinusoidal samples of various frequencies are applied to the input, i.e.,
how  to  estimate  their  frequency-domain  response.  Two  factors  affect  an  FIR  filter’s  frequency
response:  the  number  of  taps  and  the  specific  values  used  for  the  multiplication  coefficients.  We’ll
explore these two factors using our averaging example and, then, see how we can use them to design
FIR  filters.  This  brings  us  to  the  point  where  we  have  to  introduce  the  C  word:  convolution.
(Actually, we already slipped a convolution equation in on the reader without saying so. It was Eq.
(5-3), and we’ll examine it in more detail later.)
5.2 Convolution in FIR Filters
OK,  here’s  where  we  get  serious  about  understanding  the  mathematics  behind  FIR  filters.  We  can
graphically depict Eq. (5-3)’s and Figure 5-4’s calculations as shown in Figure 5-5.  Also,  let’s  be
formal  and  use  the  standard  notation  of  digital  filters  for  indexing  the  input  samples  and  the  filter
coefficients by starting with an initial index value of zero; that is, we’ll call the initial input value the
0th sample x(0). The next input sample is represented by the term x(1), the following input sample is
called x(2), and so on. Likewise, our five coefficient values will be indexed from zero to four, h(0)
through  h(4).  (This  indexing  scheme  makes  the  equations  describing  our  example  consistent  with
conventional filter notation found in the literature.)
Figure 5-5 Averaging filter convolution: (a) first five input samples aligned with the stationary filter
coefficients, index n = 4; (b) input samples shift to the right and index n = 5; (c) index n = 6; (d) index
n = 7; (e) index n = 8.
In Eq. (5-3)  we  used  the  factor  of  1/5  as  the  filter  coefficients  multiplied  by  our  averaging  filter’s
input samples. The left side of Figure 5-5 shows the alignment of those coefficients, black squares,
with the filter input sample values represented by the white squares. Notice in Figures 5-5(a) through
5-5(e)  that  we’re  marching  the  input  samples  to  the  right,  and,  at  each  step,  we  calculate  the  filter
output sample value using Eq. (5-3). The output samples on the right side of Figure 5-5 match the first
five values represented by the black squares in Figure 5-2. The input samples in Figure 5-5 are those
values  represented  by  the  white  squares  in  Figure 5-2.  Notice  that  the  time  order  of  the  inputs  in
Figure 5-5 has been reversed from the input sequence order in Figure 5-2! That is, the input sequence
has been flipped in the time domain in Figure 5-5. This time order reversal is what happens to the
input data using the filter structure in Figure 5-4.
Repeating the first part of Eq. (5-3) and omitting the subscript on the output term, our original FIR
filter’s y(n)th output is given by
(5-4)
Because we’ll explore filters whose coefficients are not all the same value, we need to represent the
individual filter coefficients by a variable, such as the term h(k), for example. Thus we can rewrite
the averaging filter’s output from Eq. (5-4) in a more general way as
(5-5)
where  h(0)  through  h(4)  all  equal  1/5.  Equation  (5-5)  is  a  concise  way  of  describing  the  filter
structure in Figure 5-4 and the process illustrated in Figure 5-5.
Let’s take Eq. (5-5) one step further and say, for a general M-tap FIR filter, the nth output is
(5-6)
Well, there it is. Eq. (5-6)  is  the  infamous  convolution  equation  as  it  applies  to  digital  FIR  filters.
Beginners  in  the  field  of  digital  signal  processing  often  have  trouble  understanding  the  concept  of
convolution. It need not be that way. Eq. (5-6) is merely a series of multiplications followed by the
addition of the products. The process is actually rather simple. We just flip the time order of an input
sample sequence and start stepping the flipped sequence across the filter’s coefficients as shown in
Figure 5-5. For each new filter input sample, we sum a series of products to compute a single filter
output value.
Let’s  pause  for  a  moment  and  introduce  a  new  term  that’s  important  to  keep  in  mind,  the  impulse
response. The impulse response of a filter is exactly what its name implies—it’s the filter’s output
time-domain  sequence  when  the  input  is  a  single  unity-valued  sample  (impulse)  preceded  and
followed by zero-valued samples. Figure 5-6 illustrates this idea in the same way we determined the
filter’s output sequence in Figure 5-5. The left side of Figure 5-6  shows  the  alignment  of  the  filter
coefficients,  black  squares,  with  the  filter  input  impulse  sample  values  represented  by  the  white
squares. Again, in Figures 5-6(a) through 5-6(e) we’re shifting the input samples to the right, and, at
each step, we calculate the filter output sample value using Eq. (5-4). The output samples on the right
side of Figure 5-6 are the filter’s impulse response. Notice the key point here: the FIR filter’s impulse
response  is  identical  to  the  five  filter  coefficient  values.  For  this  reason,  the  terms  FIR  filter
coefficients  and  impulse  response  are  synonymous.  Thus,  when  someone  refers  to  the  impulse
response of an FIR filter, they’re also talking about the coefficients. Because there are a finite number
of coefficients, the impulse response will be finite in time duration (finite impulse response, FIR).
Figure 5-6 Convolution of filter coefficients and an input impulse to obtain the filter’s output impulse
response: (a) impulse sample aligned with the first filter coefficient, index n = 4; (b) impulse sample
shifts to the right and index n = 5; (c) index n = 6; (d) index n = 7; (e) index n = 8.
Returning to our averaging filter, recall that coefficients (or impulse response) h(0) through h(4) were
all equal to 1/5. As it turns out, our filter’s performance can be improved by using coefficients whose
values are not all the same. By “performance” we mean how well the filter passes desired signals
and attenuates unwanted signals. We judge that performance by determining the shape of the filter’s
frequency-domain response that we obtain by the convolution property of linear systems. To describe
this concept, let’s repeat Eq. (5-6) using the abbreviated notation of
(5-7)
where the * symbol means convolution. (Equation 5-7 is read as “y of n equals the convolution of h of
k and x of n.”)  The  process  of  convolution,  as  it  applies  to  FIR  filters,  is  as  follows:  the  discrete
Fourier transform (DFT) of the convolution of a filter’s impulse response (coefficients) and an input
sequence  is  equal  to  the  product  of  the  spectrum  of  the  input  sequence  and  the  DFT  of  the  impulse
response. The idea we’re trying to convey here is that if two time-domain sequences h(k) and x(n)
have  DFTs  of  H(m)  and  X(m),  respectively,  then  the  DFT  of  y(n)  =  h(k)  *  x(n)  is  H(m)  ·  X(m).
Making this point in a more compact way, we state this relationship with the expression
(5-8)
With  IDFT  indicating  the  inverse  DFT,  Eq.  (5-8)  indicates  that  two  sequences  resulting  from
h(k)*x(n) and H(m)·X(m)  are  Fourier  transform  pairs.  So  taking  the  DFT  of  h(k)*x(n)  gives  us  the
product  H(m)·X(m)  that  is  the  spectrum  of  our  filter  output  Y(m).  Likewise,  we  can  determine
h(k)*x(n) by taking the inverse DFT of H(m)·X(m). The very important conclusion to learn from Eq.
(5-8) is that convolution in the time domain is equivalent to multiplication in the frequency domain.
To help us appreciate this principle, Figure 5-7 sketches the relationship between convolution in the
time domain and multiplication in the frequency domain. The process of convolution with regard to
linear systems is discussed in more detail in Section 5.9. The beginner is encouraged to review that
material to get a general idea of why and when the convolution process can be used to analyze digital
filters.
Figure 5-7 Relationships of convolution as applied to FIR digital filters.
Equation  (5-8)  and  the  relationships  in  Figure  5-7  tell  us  what  we  need  to  do  to  determine  the
frequency response of an FIR filter. The product X(m)·H(m) is the DFT of the filter output. Because
X(m) is the DFT of the filter’s input sequence, the frequency response of the filter is then defined as
H(m), the DFT of the filter’s impulse response h(k).† Getting back to our original problem, we can
determine our averaging filter’s frequency-domain response by taking the DFT of the individual filter
coefficients  (impulse  response)  in  Eq. (5-4).  If  we  take  the  five  h(k)  coefficient  values  of  1/5  and
append 59 zeros, we have the sequence depicted in Figure 5-8(a). Performing a 64-point DFT on that
sequence,  and  normalizing  the  DFT  magnitudes,  gives  us  the  filter’s  frequency  magnitude  response
|H(m)|  in  Figure 5-8(b)  and  phase  response  shown  in  Figure  5-8(c).††  H(m)  is  our  old  friend,  the
sin(x)/x function from Section 3.13.
† We use the term impulse response here, instead of coefficients, because this concept also applies to IIR filters. IIR filter frequency
responses are also equal to the DFT of their impulse responses.
†† There’s nothing sacred about using a 64-point DFT here. We could just as well have appended only enough zeros to take a 16- or 32-
point  FFT.  We  chose  64  points  to  get  a  frequency  resolution  that  would  make  the  shape  of  the  response  in  Figure  5-8(b)  reasonably
smooth. Remember, the more points in the FFT, the finer the frequency granularity—right?
Figure 5-8 Averaging FIR filter: (a) filter coefficient sequence h(k) with appended zeros; (b)
normalized discrete frequency magnitude response |H(m)| of the h(k) filter coefficients; (c) phase-
angle response of H(m) in degrees.
Let’s  relate  the  discrete  frequency  response  samples  in  Figures  5-8(b)  and  5-8(c)  to  the  physical
dimension of the sample frequency fs. We know, from Section 3.5 and our experience with the DFT,
that the m = N/2 discrete frequency sample, m = 32 in this case, is equal to the folding frequency, or
half the sample rate, fs/2. Keeping this in mind, we can convert the discrete frequency axis in Figure
5-8  to  that  shown  in  Figure 5-9. In Figure 5-9(a),  notice  that  the  filter’s  magnitude  response  is,  of
course,  periodic  in  the  frequency  domain  with  a  period  of  the  equivalent  sample  rate  fs.  Because
we’re primarily interested in the filter’s response between 0 and half the sample rate, Figure 5-9(c)
shows  that  frequency  band  in  greater  detail,  affirming  the  notion  that  averaging  behaves  like  a
lowpass  filter.  It’s  a  relatively  poor  lowpass  filter  compared  to  an  arbitrary,  ideal  lowpass  filter
indicated by the dashed lines in Figure 5-9(c), but our averaging filter will attenuate higher-frequency
inputs relative to its response to low-frequency input signals.
Figure 5-9 Averaging FIR filter frequency response shown as continuous curves: (a) normalized
frequency magnitude response, |H(m)|; (b) phase-angle response of H(m) in degrees; (c) the filter’s
magnitude response between zero Hz and half the sample rate, fs/2 Hz.
We can demonstrate this by way of example. Suppose we applied a low-frequency sinewave to a 5-
point  averaging  FIR  filter  as  shown  by  the  white  squares  in  Figure 5-10(a).  The  input  sinewave’s
frequency is fs/32  Hz  and  its  peak  amplitude  is  unity.  The  filter’s  output  sequence  is  shown  by  the
black squares.
Figure 5-10 Averaging FIR filter input and output responses: (a) with an input sinewave of frequency
fs/32; (b) with an input sinewave of frequency 3fs/32.
Figure 5-10(a) is rich in information! First, the filter’s output is a sinewave of the same frequency as
the input. This is a characteristic of a linear system. We apply a single sinewave input, and the output
will be a single sinewave (shifted in phase and perhaps reduced in amplitude) of the same frequency
as  the  input.  Second,  notice  that  the  initial  four  output  samples  are  not  exactly  sinusoidal.  Those
output samples are the transient response of the filter. With tapped-delay line FIR filters, the sample
length of that transient response is equal to the number of filter unit-delay elements D, after which the
filter’s output begins its steady-state time response.
The above transient response property is important. It means that tapped-delay line FIR filter outputs
are not valid until D+1 input samples have been applied to the filter. That is, the output samples are
not valid until the filter’s delay line is filled with input data. So, for an FIR filter having D = 70 unit-
delay  elements  the  first  70  output  samples  are  not  valid  and  would  be  ignored  in  practice.
WARNING:  There  are  tapped-delay  line  FIR  filters,  used  in  practice,  that  have  more  unit-delay
elements  than  nonzero-valued  tap  coefficients.  The  transient  response  length  for  those  filters,
measured in samples, is equal to the number of unit-delay elements, D (and is unrelated to the number
of nonzero-valued tap coefficients).
The filter’s output sinewave peak amplitude is reduced to a value of 0.96 and the output sinewave is
delayed from the input by a phase angle of 22.5 degrees. Notice that the time delay between the input
and output sinewaves, in Figure 5-10(a), is two samples in duration. (Although we discuss this time
delay  topic  in  more  detail  later,  for  now  we’ll  just  say  that,  because  the  filter’s  coefficients  are
symmetrical,  the  input/output  delay  measured  in  samples  is  equal  to  half  the  number  of  unit-delay
elements in the filter’s tapped-delay line.)
Next,  if  we  applied  a  higher-frequency  sinewave  of  3fs/32  Hz  to  our  5-tap  FIR  filter  as  shown  in
Figure 5-10(b), the filter output is a sinewave of frequency 3fs/32 Hz and its peak amplitude is even
further  reduced  to  a  value  of  0.69.  That’s  the  nature  of  lowpass  filters—they  attenuate  higher-
frequency inputs more than they attenuate low-frequency inputs. As in Figure 5-10(a), the time delay
between the input and output sinewaves, in Figure 5-10(b), is two samples in duration (corresponding
to a phase-angle delay of 67.5 degrees). That property, where the input/output delay does not depend
on  frequency,  is  a  very  beneficial  property  of  FIR  filters  having  symmetrical  coefficients.  We’ll
discuss this important issue again later in this chapter. In Figure 5-10(b) we see that the nonsinusoidal
filter output transient response is even more obvious than it was in Figure 5-10(a).
Although the output amplitudes and phase delays in Figure 5-10 were measured values from actually
performing a 5-tap FIR filter process on the input sinewaves’ samples, we could have obtained those
amplitude  and  phase  delay  values  directly  from  Figures 5-8(b)  and  5-8(c).  The  point  is,  we  don’t
have  to  implement  an  FIR  filter  and  apply  various  sinewave  inputs  to  discover  what  its  frequency
response will be. We need merely take the DFT of the FIR filter’s coefficients (impulse response) to
determine the filter’s frequency response as we did for Figure 5-8.
Figure  5-11  is  another  depiction  of  how  well  our  5-tap  averaging  FIR  filter  performs,  where  the
dashed  line  is  the  filter’s  magnitude  response  |H(m)|,  and  the  shaded  line  is  the  |X(m)|  magnitude
spectrum of the filter’s input values (the white squares in Figure 5-2). The solid line is the magnitude
spectrum  of  the  filter’s  output  sequence,  which  is  shown  by  the  black  squares  in  Figure 5-2.  So  in
Figure  5-11,  the  solid  output  spectrum  is  the  product  of  the  dashed  filter  response  curve  and  the
shaded  input  spectrum,  or  |X(m)·H(m)|.  Again,  we  see  that  our  averager  does  indeed  attenuate  the
higher-frequency portion of the input spectrum.
Figure 5-11 Averaging FIR filter input magnitude spectrum, frequency magnitude response, and output
magnitude spectrum.
Let’s pause for a moment to let all of this soak in a little. So far we’ve gone through the averaging
filter example to establish that
• FIR filters perform time-domain convolution by summing the products of the shifted input samples
and a sequence of filter coefficients,
•  an  FIR  filter’s  output  sequence  is  equal  to  the  convolution  of  the  input  sequence  and  a  filter’s
impulse response (coefficients),
• an FIR filter’s frequency response is the DFT of the filter’s impulse response,
•  an  FIR  filter’s  output  spectrum  is  the  product  of  the  input  spectrum  and  the  filter’s  frequency
response, and
• convolution in the time domain and multiplication in the frequency domain are Fourier transform
pairs.
OK, here’s where FIR filters start to get really interesting. Let’s change the values of the five filter
coefficients  to  modify  the  frequency  response  of  our  5-tap  lowpass  filter.  In  fact,  Figure  5-12(a)
shows our original five filter coefficients and two other arbitrary sets of 5-tap coefficients. Figure 5-
12(b)  compares  the  frequency  magnitude  responses  of  those  three  sets  of  coefficients.  Again,  the
frequency responses are obtained by taking the DFT of the three individual sets of coefficients and
plotting  the  magnitude  of  the  transforms,  as  we  did  for  Figure  5-9(c).  So  we  see  three  important
characteristics in Figure 5-12.  First,  as  we  expected,  different  sets  of  coefficients  give  us  different
frequency  magnitude  responses.  Second,  a  sudden  change  in  the  values  of  the  coefficient  sequence,
such as the 0.2 to 0 transition in the first coefficient set, causes ripples, or sidelobes, in the frequency
response. Third, if we minimize the suddenness of the changes in the coefficient values, such as the
third set of coefficients in Figure 5-12(a), we reduce the sidelobe ripples in the frequency response.
However, reducing the sidelobes results in increasing the main lobe width of our lowpass filter. (As
we’ll see, this is exactly the same effect encountered in the discussion of window functions used with
the DFT in Section 3.9.)
Figure 5-12 Three sets of 5-tap lowpass filter coefficients: (a) sets of coefficients: 0.2, 0.2, 0.2, 0.2,
0.2; 0.1, 0.2, 0.2, 0.2, 0.1; and 0.04, 0.12, 0.2, 0.12, 0.04; (b) frequency magnitude response of three
lowpass FIR filters using those sets of coefficients.
To  reiterate  the  function  of  the  filter  coefficients,  Figure 5-13  shows  the  5-tap  FIR  filter  structure
using  the  third  set  of  coefficients  from  Figure  5-12.  The  implementation  of  constant-coefficient
transversal  FIR  filters  does  not  get  any  more  complicated  than  that  shown  in  Figure 5-13.  It’s  that
simple.  We  can  have  a  filter  with  more  than  5  taps,  but  the  input  signal  sample  shifting,  the
multiplications  by  the  constant  coefficients,  and  the  summation  are  all  there  is  to  it.  (By  constant
coefficients, we don’t mean coefficients whose values are all the same; we mean coefficients whose
values remain unchanged, or time invariant. There is a class of digital filters, called adaptive filters,
whose  coefficient  values  are  periodically  changed  to  adapt  to  changing  input  signal  parameters.
While  we  won’t  discuss  these  adaptive  filters  in  this  introductory  text,  their  descriptions  are
available in the literature[1–5].)
Figure 5-13 Five-tap lowpass FIR filter implementation using the coefficients 0.04, 0.12, 0.2, 0.12,
and 0.04.
So  far,  our  description  of  an  FIR  filter  implementation  has  been  presented  from  a  hardware
perspective. In Figure 5-13,  to  calculate  a  single  filter  output  sample,  five  multiplications  and  five
additions  must  take  place  before  the  arrival  of  the  next  input  sample  value.  In  a  software
implementation  of  a  5-tap  FIR  filter,  however,  all  of  the  input  data  samples  would  be  previously
stored in memory. The software filter routine’s job, then, is to access different five-sample segments
of the x(n) input data space, perform the calculations shown in Figure 5-13, and store the resulting
filter y(n) output sequence in an array of memory locations.†
†  In  reviewing  the  literature  of  FIR  filters,  the  reader  will  often  find  the  term  z−1  replacing  the  delay  function  in  Figure  5-13.  This
equivalence is explained in the next chapter when we study IIR filters.
Now that we have a basic understanding of what a digital FIR filter is, let’s see what effect is had by
using more than 5 filter taps by learning to design FIR filters.
5.3 Lowpass FIR Filter Design
OK,  instead  of  just  accepting  a  given  set  of  FIR  filter  coefficients  and  analyzing  their  frequency
response,  let’s  reverse  the  process  and  design  our  own  lowpass  FIR  filter.  The  design  procedure
starts  with  the  determination  of  a  desired  frequency  response  followed  by  calculating  the  filter
coefficients that will give us that response. There are two predominant techniques used to design FIR
filters: the window method and the so-called optimum method. Let’s discuss them in that order.
5.3.1 Window Design Method
The  window  method  of  FIR  filter  design  (also  called  the  Fourier  series  method)  begins  with  our
deciding  what  frequency  response  we  want  for  our  lowpass  filter.  We  can  start  by  considering  a
continuous lowpass filter, and simulating that filter with a digital filter. We’ll define the continuous
frequency response H(f) to be ideal, i.e., a lowpass filter with unity gain at low frequencies and zero
gain (infinite attenuation) beyond some cutoff frequency, as shown in Figure 5-14(a). Representing
this H(f) response by a discrete frequency response is straightforward enough because the idea of a
discrete  frequency  response  is  essentially  the  same  as  a  continuous  frequency  response—with  one
in  Sections  2.2  and  3.13,  discrete  frequency-domain
important  difference.  As  described
representations  are  always  periodic  with  the  period  being  the  sample  rate  fs.  The  discrete
representation of our ideal, continuous lowpass filter H(f) is the periodic response H(m) depicted by
the frequency-domain samples in Figure 5-14(b).
Figure 5-14 Lowpass filter frequency responses: (a) continuous frequency response H(f); (b)
periodic, discrete frequency response H(m).
We  have  two  ways  to  determine  our  lowpass  filter’s  time-domain  coefficients.  The  first  way  is
algebraic:
1. Develop an expression for the discrete frequency response H(m).
2. Apply that expression to the inverse DFT equation to get the time domain h(k).
3. Evaluate that h(k) expression as a function of time index k.
The second method is to define the individual frequency-domain samples representing H(m) and then
have  a  software  routine  perform  the  inverse  DFT  of  those  samples,  giving  us  the  FIR  filter
coefficients. In either method, we need only define the periodic H(m) over a single period of fs Hz.
As it turns out, defining H(m) in Figure 5-14(b) over the frequency span −fs/2 to fs/2 is the easiest
form  to  analyze  algebraically,  and  defining  H(m)  over  the  frequency  span  0  to  fs  is  the  best
representation if we use the inverse DFT to obtain our filter’s coefficients. Let’s try both methods to
determine the filter’s time-domain coefficients.
In  the  algebraic  method,  we  can  define  an  arbitrary  discrete  frequency  response  H(m)  using  N
samples  to  cover  the  −fs/2  to  fs/2  frequency  range  and  establish  K  unity-valued  samples  for  the
passband of our lowpass filter as shown in Figure 5-15. To determine h(k) algebraically we need to
take the inverse DFT of H(m) in the form of
(5-9)
Figure 5-15 Arbitrary, discrete lowpass FIR filter frequency response defined over N frequency-
domain samples covering the frequency range of fs Hz.
where our time-domain index is k. The solution to Eq. (5-9), derived in Section 3.13 as Eq. (3-59), is
repeated here as
(5-10)
If we evaluate Eq. (5-10) as a function of k, we get the sequence shown in Figure 5-16, taking the
form of the classic sin(x)/x function. By reviewing the material in Section 3.13, it’s easy to see the
great deal of algebraic manipulation required to arrive at Eq. (5-10) from Eq. (5-9). So much algebra,
in  fact,  with  its  many  opportunities  for  making  errors,  that  digital  filter  designers  like  to  avoid
evaluating Eq. (5-9) algebraically. They prefer to use software routines to perform inverse DFTs (in
the form of an inverse FFT) to determine h(k), and so will we.
Figure 5-16 Time-domain h(k) coefficients obtained by evaluating Eq. (5-10).
We can demonstrate the software inverse DFT method of FIR filter design with an example. Let’s say
we need to design a lowpass FIR filter simulating the continuous frequency response shown in Figure
5-17(a).  The  discrete  representation  of  the  filter’s  frequency  response  H(m)  is  shown  in  Figure  5-
17(b), where we’ve used N = 32 points to represent the frequency-domain variable H(f). Because it’s
equivalent to Figure 5-17(b) but avoids the negative values of the frequency index m, we represent
the discrete frequency samples  over  the  range  0  to  fs in Figure 5-17(c),  as  opposed  to  the  −fs/2  to
+fs/2 range in Figure 5-17(b). OK, we’re almost there. Using a 32-point inverse FFT to implement a
32-point inverse DFT of the H(m) sequence in Figure 5-17(c), we get the 32 h(k) values depicted by
the dots from k = −15 to k = 16 in Figure 5-18(a).† We have one more step to perform. Because we
want our final 31-tap h(k) filter coefficients to be symmetrical with their peak value in the center of
the coefficient sample set, we drop the k = 16 sample and shift the k index to the left from Figure 5-
18(a), giving us the desired sin(x)/x form of h(k) as shown in Figure 5-18(b). This shift of the index k
will not change the frequency magnitude response of our FIR filter. (Remember from our discussion
of the DFT shifting theorem in Section 3.6 that a shift in the time domain manifests itself only as a
linear phase shift in the frequency domain with no change in the frequency-domain magnitude.) The
sequence in Figure 5-18(b), then, is now the coefficients we use in the convolution process of Figure
5-5 to implement a lowpass FIR filter.
† If you want to use this FIR design method but only have a forward FFT software routine available, Section 13.6 shows a slick way to
perform an inverse FFT with the forward FFT algorithm.
Figure 5-17 An ideal lowpass filter: (a) continuous frequency response H(f); (b) discrete response
H(m) over the range −fs/2 to fs/2 Hz; (c) discrete response H(m) over the range 0 to fs Hz.
Figure 5-18 Inverse DFT of the discrete response in Figure 5-17(c): (a) normal inverse DFT
indexing for k; (b) symmetrical coefficients used for a 31-tap lowpass FIR filter.
It’s important to demonstrate that the more h(k) terms we use as filter coefficients, the closer we’ll
approximate  our  ideal  lowpass  filter  response.  Let’s  be  conservative,  just  use  the  center  nine  h(k)
coefficients, and see what our filter response looks like. Again, our filter’s magnitude response in this
case  will  be  the  DFT  of  those  nine  coefficients  as  shown  on  the  right  side  of  Figure 5-19(a).  The
ideal  filter’s  frequency  response  is  also  shown  for  reference  as  the  dashed  curve.  (To  show  the
details  of  its  shape,  we’ve  used  a  continuous  curve  for  |H(m)|  in  Figure  5-19(a),  but  we  have  to
remember that |H(m)| is really a sequence of discrete values.) Notice that using nine coefficients gives
us a lowpass filter, but it’s certainly far from ideal. Using more coefficients to improve our situation,
Figure 5-19(b) shows 19 coefficients and their corresponding frequency magnitude response that is
beginning to look more like our desired rectangular response. Notice that magnitude fluctuations, or
ripples, are evident in the passband of our H(m) filter response. Continuing, using all 31 of the h(k)
values  for  our  filter  coefficients  results  in  the  frequency  response  in  Figure  5-19(c).  Our  filter’s
response is getting better (approaching the ideal), but those conspicuous passband magnitude ripples
are still present.
Figure 5-19 Coefficients and frequency responses of three lowpass filters: (a) 9-tap FIR filter; (b)
19-tap FIR filter; (c) frequency response of the full 31-tap FIR filter.
It’s important that we understand why those passband ripples are in the lowpass FIR filter response in
Figure  5-19.  Recall  the  above  discussion  of  convolving  the  5-tap  averaging  filter  coefficients,  or
impulse  response,  with  an  input  data  sequence  to  obtain  the  averager’s  output.  We  established  that
convolution  in  the  time  domain  is  equivalent  to  multiplication  in  the  frequency  domain,  which  we
symbolized with Eq. (5-8) and repeat here as
(5-11)
This association between convolution in the time domain and multiplication in the frequency domain,
sketched in Figure 5-7, indicates that if two time-domain sequences h(k) and x(n) have DFTs of H(m)
and X(m), respectively, then the DFT of h(k) * x(n) is H(m) · X(m). No restrictions whatsoever need
be  placed  on  what  the  time-domain  sequences  h(k)  and  x(n)  in  Eq.  (5-11)  actually  represent.  As
detailed later in Section 5.9,  convolution  in  one  domain  is  equivalent  to  multiplication  in  the  other
domain, allowing us to state that multiplication in the time domain is equivalent to convolution in the
frequency domain, or
(5-12)
Now we’re ready to understand why the magnitude ripples are present in Figure 5-19.
Rewriting Eq. (5-12) and replacing the h(k) and x(n) expressions with h∞(k) and w(k), respectively,
(5-13)
Let’s  say  that  h∞(k)  represents  an  infinitely  long  sin(x)/x  sequence  of  ideal  lowpass  FIR  filter
coefficients and that w(k) represents a window sequence that we use to truncate the sin(x)/x terms as
shown in Figure 5-20. Thus, the w(k)  sequence  is  a  finite-length  set  of  unity  values  and  its  DFT  is
W(m). The length of w(k) is merely the number of coefficients, or taps, we intend to use to implement
our lowpass FIR filter. With h∞(k) defined as such, the product h∞(k) · w(k) represents the truncated
set of filter coefficients h(k) in Figures 5-19(a) and 5-19(b). So, from Eq. (5-13), the FIR filter’s true
frequency response H(m) is the convolution
(5-14)
Figure 5-20 Infinite h∞(k) sequence windowed by w(k) to define the final filter coefficients h(k).
We  depict  this  convolution  in  Figure 5-21  where,  to  keep  the  figure  from  being  so  busy,  we  show
H∞(m)  (the  DFT  of  the  h∞(k)  coefficients)  as  the  gray  rectangle.  Keep  in  mind  that  it’s  really  a
sequence of constant-amplitude sample values.
Figure 5-21 Convolution W(m)*H∞(m): (a) unshifted W(m) and H∞(m); (b) shift of W(m) leading to
ripples within H(m)’s positive-frequency passband; (c) shift of W(m) causing response roll-off near
H(m)’s positive cutoff frequency; (d) shift of W(m) causing ripples beyond H(m)’s positive cutoff
frequency.
Let’s  look  at  Figure 5-21(a)  very  carefully  to  see  why  all  three  |H(m)|s  exhibit  passband  ripple  in
Figure 5-19.  We  can  view  a  particular  sample  value  of  the  H(m)  =  H∞(m)  * W(m)  convolution  as
being the sum of the products of H∞(m) and W(m) for a particular frequency shift of W(m). H∞(m) and
the unshifted W(m) are shown in Figure 5-21(a.) With an assumed value of unity for all of H∞(m), a
particular H(m) value is now merely the sum of the W(m) samples that overlap the H∞(m) rectangle.
So,  with  a  W(m)  frequency  shift  of  0  Hz,  the  sum  of  the  W(m)  samples  that  overlap  the  H∞(m)
rectangle in Figure 5-21(a) is the value of H(m) at 0 Hz. As W(m) is shifted to the right to give us
additional  positive-frequency  H(m)  values,  we  can  see  that  the  sum  of  the  positive  and  negative
values of W(m)  under  the  rectangle  oscillates  during  the  shifting  of  W(m).  As  the  convolution  shift
proceeds, Figure 5-21(b) shows why there are ripples in the passband of H(m)—again, the sum of the
positive  and  negative  W(m)  samples  under  the  H∞(m)  rectangle  continues  to  vary  as  the  W(m)
function is shifted. The W(m) frequency shift, indicated in Figure 5-21(c), where the peak of W(m)’s
main lobe is now outside the H∞(m) rectangle, corresponds to the frequency where H(m)’s passband
begins  to  roll  off.  Figure 5-21(d)  shows  that,  as  the  W(m)  shift  continues,  there  will  be  ripples  in
H(m)  beyond  the  positive  cutoff  frequency.†  The  point  of  all  of  this  is  that  the  ripples  in  H(m)  are
caused by the sidelobes of W(m).
† In Figure 5-21(b), had we started to shift W(m) to the left in order to determine the negative-frequency portion of H(m), we would have
obtained the mirror image of the positive-frequency portion of H(m).
Figure 5-22 helps us answer the question “How many sin(x)/x coefficients do we have to use (or how
wide must w(k) be) to get nice sharp falling edges and no ripples in our H(m) passband?” The answer
is that we can’t get there from here. It doesn’t matter how many sin(x)/x coefficients (filter taps) we
use; there will always be filter passband ripple. As long as w(k) is a finite number of unity values
(i.e.,  a  rectangular  window  of  finite  width),  there  will  be  sidelobe  ripples  in  W(m),  and  this  will
induce passband ripples in the final H(m) frequency response. To illustrate that increasing the number
of sin(x)/x coefficients doesn’t reduce passband ripple, we repeat the 31-tap lowpass filter response
in Figure 5-22(a). The frequency response, using 63 coefficients, is shown in Figure 5-22(b), and the
passband  ripple  remains.  We  can  make  the  filter’s  transition  region  narrower  using  additional  h(k)
filter  coefficients,  but  we  cannot  eliminate  the  passband  ripple.  That  ripple,  known  as  Gibbs’s
phenomenon, manifests itself anytime a function (w(k) in this case) with an instantaneous discontinuity
is represented by a Fourier series[6–8]. No finite set of sinusoids will be able to change fast enough
to  be  exactly  equal  to  an  instantaneous  discontinuity.  Another  way  to  state  this  Gibbs’s  dilemma  is
that, no matter how wide our w(k) window is, W(m) will always have sidelobe ripples. As shown in
Figure 5-22(b), we can use more coefficients by extending the width of the rectangular w(k) to narrow
the filter transition region, but a wider w(k) does not eliminate the filter passband ripple, nor does it
even reduce their peak-to-peak ripple magnitudes, as long as w(k) has sudden discontinuities.
Figure 5-22 Passband ripple and transition regions: (a) for a 31-tap lowpass filter; (b) for a 63-tap
lowpass filter.
5.3.2 Windows Used in FIR Filter Design
OK. The good news is that we can minimize FIR passband ripple with window functions the same
way  we  minimized  DFT  leakage  in  Section  3.9.  Here’s  how.  Looking  back  at  Figure  5-20,  by
truncating the infinitely long h∞(k) sequence through multiplication by the rectangular w(k), our final
h(k)  exhibited  ripples  in  the  frequency-domain  passband.  Figure  5-21  shows  us  that  the  passband
ripples  were  caused  by  W(m)’s  sidelobes  that,  in  turn,  were  caused  by  the  sudden  discontinuities
from zero to one and one to zero in w(k). If we think of w(k) in Figure 5-20 as a rectangular window,
then  it  is  w(k)’s  abrupt  amplitude  changes  that  are  the  source  of  our  filter  passband  ripple.  The
window  FIR  design  method  is  the  technique  of  reducing  w(k)’s  discontinuities  by  using  window
functions other than the rectangular window.
Consider Figure 5-23 to see how a nonrectangular window function can be used to design low-ripple
FIR digital filters. Imagine if we replaced Figure 5-20’s rectangular w(k) with the Blackman window
function whose discrete values are defined as
(5-15)
Figure 5-23 Coefficients and frequency response of a 31-tap Blackman-windowed FIR filter: (a)
defining the windowed filter coefficients h(k); (b) low-ripple 31-tap frequency response; (c) low-
ripple 63-tap frequency response.
This situation is depicted for N = 31 in Figure 5-23(a), where Eq. (5-15)’s w(k) looks very much like
the  Hanning  window  function  in  Figure 3-17(a).  This  Blackman  window  function  results  in  the  31
smoothly  tapered  h(k)  coefficients  at  the  bottom  of  Figure  5-23(a).  Notice  two  things  about  the
resulting H(m)  in  Figure  5-23(b).  First,  the  good  news.  The  passband  ripples  are  greatly  reduced
from  those  evident  in  Figure 5-22(a)—so  our  Blackman  window  function  did  its  job.  Second,  the
price we paid for reduced passband ripple is a wider H(m) transition region. We can get a steeper
filter response roll-off by increasing the number of taps in our FIR filter. Figure 5-23(c) shows the
improved frequency response had we used a 63-coefficient Blackman window function for a 63-tap
FIR  filter.  So  using  a  nonrectangular  window  function  reduces  passband  ripple  at  the  expense  of
slower passband to stopband roll-off.
A  graphical  comparison  of  the  frequency  responses  for  the  rectangular  and  Blackman  windows  is
provided in Figure 5-24. (The curves in Figure 5-24 were obtained for the window functions defined
by  16  discrete  samples,  to  which  496  zeros  were  appended,  applied  to  a  512-point  DFT.)  The
sidelobe magnitudes of the Blackman window’s |W(m)| are too small to see on a linear scale. We can
see those sidelobe details by plotting the two windows’ frequency responses on a logarithmic scale
and normalizing each plot so that their main lobe peak values are both zero dB. For a given window
function, we can get the log magnitude response of WdB(m) by using the expression
(5-16)
Figure 5-24 Rectangular versus Blackman window frequency magnitude responses: (a) |W(m)| on a
linear scale; (b) normalized logarithmic scale of WdB(m).
(The |W(0)| term in Eq. (5-16) is the magnitude of W(m) at the peak of the main lobe when m = 0.)
Figure 5-24(b) shows us the greatly reduced sidelobe levels of the Blackman window and how that
window’s main lobe is almost three times as wide as the rectangular window’s main lobe.
Of course, we could have used any of the other window functions, discussed in Section 3.9, for our
lowpass FIR filter. That’s why this FIR filter design technique is called the window design method.
We pick a window function and multiply it by the sin(x)/x values from H∞(m) in Figure 5-23(a) to get
our final h(k) filter coefficients. It’s that simple. Before we leave the window method of FIR filter
design, let’s introduce two other interesting window functions.
Although the Blackman window and those windows discussed in Section 3.9 are useful in FIR filter
design, we have little control over their frequency responses; that is, our only option is to select some
window function and accept its corresponding frequency response. Wouldn’t it be nice to have more
flexibility  in  trading  off,  or  striking  a  compromise  between,  a  window’s  main  lobe  width  and
sidelobe  levels?  Fortunately,  there  are  two  popular  window  functions  that  give  us  this  opportunity.
Called the Chebyshev (or Dolph-Chebyshev) and the Kaiser window functions, they’re defined by the
following formidable expressions:
(5-17)
(5-18)
Two  typical  Chebyshev  and  Kaiser  window  functions  and  their  frequency  magnitude  responses  are
shown  in  Figure  5-25.  For  comparison,  the  rectangular  and  Blackman  window  functions  are  also
shown  in  that  figure.  (Again,  the  curves  in  Figure  5-25(b)  were  obtained  for  window  functions
defined by 32 discrete time samples, with 480 zeros appended, applied to a 512-point DFT.)
Figure 5-25 Typical window functions used with digital filters: (a) window coefficients in the time
domain; (b) frequency-domain magnitude responses in dB.
Equation  (5-17)  was  originally  based  on  the  analysis  of  antenna  arrays  using  the  mathematics  of
Chebyshev  polynomials[9–11].  Equation  (5-18)  evolved  from  Kaiser’s  approximation  of  prolate
spheroid  functions  using  zeroth-order  Bessel  functions[12–13].  For  each  sample  of  the  N-length
sequence  inside  the  brackets  of  the  numerator  of  Eq.  (5-18),  as  well  as  for  the  β  term  in  the
denominator, the Io(x) zeroth-order Bessel function values can be approximated using
(5-18′)
In  theory  the  upper  limit  of  the  summation  in  Eq.  (5-18′)  should  be  infinity  but,  fortunately,  25
summations give us sufficient accuracy when evaluating Io(x).
Don’t  be  intimidated  by  the  complexity  of  Eqs.  (5-17)  and  (5-18)—at  this  point,  we  need  not  be
concerned with the mathematical details of their development. We just need to realize that the γ and β
control parameters give us control over the Chebyshev and Kaiser windows’ main lobe widths and
the sidelobe levels.
Let’s see how this works for Chebyshev window functions, having four separate values of γ, and their
frequency  responses  shown  in  Figure  5-26.  FIR  filter  designers  applying  the  window  method
typically  use  predefined  software  routines  to  obtain  their  Chebyshev  window  coefficients.
Commercial  digital  signal  processing  software  packages  allow  the  user  to  specify  three  things:  the
window function (Chebyshev in this case), the desired number of coefficients (the number of taps in
the FIR filter), and the value of γ. Selecting different values for γ enables us to adjust the sidelobe
levels  and  see  what  effect  those  values  have  on  main  lobe  width,  a  capability  that  we  didn’t  have
with  the  Blackman  window  or  the  window  functions  discussed  in  Section  3.9.  The  Chebyshev
window function’s stopband attenuation, in dB, is equal to
(5-19)
Figure 5-26 Chebyshev window functions for various γ values: (a) window coefficients in the time
domain; (b) frequency-domain magnitude responses in dB.
So, for example, if we needed our sidelobe levels to be no greater than −60 dB below the main lobe,
we use Eq. (5-19) to establish a γ value of 3.0 and let the software generate the Chebyshev window
coefficients.†
† By the way, some digital signal processing software packages require that we specify AttenCheb in decibels instead of γ. That way, we
don’t have to bother using Eq. (5-19) at all.
The  same  process  applies  to  the  Kaiser  window,  as  shown  in  Figure  5-27.  Commercial  software
packages allow us to specify β in Eq. (5-18) and provide us with the associated window coefficients.
The curves in Figure 5-27(b), obtained for Kaiser window functions defined by 32 discrete samples,
show  that  we  can  select  the  desired  sidelobe  levels  and  see  what  effect  this  has  on  the  main  lobe
width.
Figure 5-27 Kaiser window functions for various β values: (a) window coefficients in the time
domain; (b) frequency-domain magnitude responses in dB.
Chebyshev or Kaiser, which is the best window to use? It depends on the application. Returning to
Figure 5-25(b), notice that, unlike the constant sidelobe  peak  levels  of  the  Chebyshev  window,  the
Kaiser  window’s  sidelobes  decrease  with  increased  frequency.  However,  the  Kaiser  sidelobes  are
higher  than  the  Chebyshev  window’s  sidelobes  near  the  main  lobe.  Our  primary  trade-off  here  is
trying  to  reduce  the  sidelobe  levels  without  broadening  the  main  lobe  too  much.  Digital  filter
designers typically experiment with various values of γ and β for the Chebyshev and Kaiser windows
to get the optimum WdB(m) for a particular application. (For that matter, the Blackman window’s very
low sidelobe levels outweigh its wide main lobe in many applications.) For some reason, algorithms
for  computing  Chebyshev  window  functions  are  not  readily  available  in  the  literature  of  DSP.  To
remedy  that  situation,  Appendix  I  presents  a  straightforward  procedure  for  computing  N-sample
Chebyshev window sequences.
To  conclude  this  section,  remember  that  different  window  functions  have  their  own  individual
advantages  and  disadvantages  for  FIR  filter  design.  Regardless  of  the  non-rectangular  window
function  used,  they  always  decrease  an  FIR  filter’s  passband  ripple  over  that  of  the  rectangular
window. For the enthusiastic reader, a thorough discussion of many window functions can be found in
reference [14].
5.4 Bandpass FIR Filter Design
The window method of lowpass FIR filter design can be used as the first step in designing a bandpass
FIR filter. Let’s say we want a 31-tap FIR filter with the frequency response shown in Figure 5-22(a),
but instead of being centered about zero Hz, we want the filter’s passband to be centered about fs/4
Hz.  If  we  define  a  lowpass  FIR  filter’s  coefficients  as  hlp(k),  our  problem  is  to  find  the  hbp(k)
coefficients  of  a  bandpass  FIR  filter.  As  shown  in  Figure  5-28,  we  can  shift  Hlp(m)’s  frequency
response by multiplying the filter’s hlp(k) lowpass coefficients by a sinusoid of fs/4 Hz. That sinusoid
is represented by the sshift(k) sequence in Figure 5-28(a), whose values are a sinewave sampled at a
rate of four samples per cycle. Our final 31-tap hbp(k) FIR bandpass filter coefficients are
(5-20)
Figure 5-28 Bandpass filter with frequency response centered at fs/4: (a) generating 31-tap filter
coefficients hbp(k); (b) frequency magnitude response |Hbp(m)|.
whose  frequency  magnitude  response  |Hbp(m)|  is  shown  as  the  solid  curves  in  Figure 5-28(b).  The
actual magnitude of |Hbp(m)| is half that of the original |Hlp(m)| because half the values in hbp(k) are
zero when sshift(k) corresponds exactly to fs/4. This effect has an important practical implication. It
means that, when we design an N-tap bandpass FIR filter centered at a frequency of fs/4 Hz, we only
need to perform approximately N/2 multiplications for each filter output sample. (There’s no reason
to multiply an input sample value, x(n−k), by zero before we sum all the products from Eq. (5-6) and
Figure 5-13, right? We just don’t bother to perform the unnecessary multiplications at all.) Of course,
when the bandpass FIR filter’s center frequency is other than fs/4, we’re forced to perform the full
number of N multiplications for each FIR filter output sample.
Notice, here, that the hlp(k) lowpass coefficients in Figure 5-28(a) have not been multiplied by any
window function. In practice, we’d use an hlp(k) that has been windowed prior to implementing Eq.
(5-20) to reduce the passband ripple. If we wanted to center the bandpass filter’s response at some
frequency other than fs/4, we merely need to modify sshift(k) to represent sampled values of a sinusoid
whose frequency is equal to the desired bandpass center frequency. That new sshift(k) sequence would
then be used in Eq. (5-20) to get the new hbp(k).
5.5 Highpass FIR Filter Design
Going one step further, we can use the bandpass FIR filter design technique to design a highpass FIR
filter.  To  obtain  the  coefficients  for  a  highpass  filter,  we  need  only  modify  the  shifting  sequence
sshift(k)  to  make  it  represent  a  sampled  sinusoid  whose  frequency  is  fs/2.  This  process  is  shown  in
Figure 5-29. Our final 31-tap highpass FIR filter’s hhp(k) coefficients are
(5-21)
Figure 5-29 Highpass filter with frequency response centered at fs/2: (a) generating 31-tap filter
coefficients hhp(k); (b) frequency magnitude response |Hhp(m)|.
whose |Hhp(m)| frequency response is the solid curve in Figure 5-29(b). Because sshift(k) in Figure 5-
29(a)  has  alternating  plus  and  minus  ones,  we  can  see  that  hhp(k)  is  merely  hlp(k)  with  the  sign
changed for every other coefficient. Unlike |Hbp(m)| in Figure 5-28(b), the |Hhp(m)| response in Figure
5-29(b) has the same amplitude as the original |Hlp(m)|.
Again,  notice  that  the  hlp(k)  lowpass  coefficients  in  Figure 5-29(a)  have  not  been  modified  by  any
window  function.  In  practice,  we’d  use  a  windowed  hlp(k)  to  reduce  the  passband  ripple  before
implementing Eq. (5-21).
5.6 Parks-McClellan Exchange FIR Filter Design Method
Let’s introduce one last FIR filter design technique that has found wide acceptance in practice. The
Parks-McClellan FIR filter design method (also called the Remez Exchange, or Optimal method†) is a
popular technique used to design high-performance FIR filters. To use this design method, we have to
visualize a desired frequency response Hd(m) like that shown in Figure 5-30.
† Remez is pronounced re-’m .
Figure 5-30 Desired frequency response definition of a lowpass FIR filter using the Parks-McClellan
Exchange design method.
We have to establish a desired passband cutoff frequency fpass and the frequency where the attenuated
stopband begins, fstop. In addition, we must establish the variables δp and δs that define our desired
passband and stopband ripple. Passband and stopband ripples, in decibels, are related to δp and δs
by[15]
and
(5-22)
(5-22′)
(Some  of  the  early  journal  papers  describing  the  Parks-McClellan  design  method  used  the  equally
valid expression −20 · log10(δp) to define the passband ripple in dB. However, Eq. (5-22) is the most
common  form  used  today.)  Next,  we  apply  these  parameters  to  a  computer  software  routine  that
generates the filter’s N time-domain h(k) coefficients where N is the minimum number of filter taps to
achieve the desired filter response.
On the other hand, some software Parks-McClellan routines assume that we want δp and δs to be as
small as possible and require us only to define the desired values of the Hd(m) response as shown by
the  solid  black  dots  in  Figure 5-31.  The  software  then  adjusts  the  values  of  the  undefined  (shaded
dots) values of Hd(m) to minimize the error between our desired and actual frequency response while
minimizing δp and δs.  The  filter  designer  has  the  option  to  define  some  of  the  Hd(m)  values  in  the
transition  band,  and  the  software  calculates  the  remaining  undefined  Hd(m)  transition  band  values.
With this version of the Parks-McClellan algorithm, the issue of most importance becomes how we
define  the  transition  region.  We  want  to  minimize  its  width  while,  at  the  same  time,  minimizing
passband  and  stopband  ripple.  So  exactly  how  we  design  an  FIR  filter  using  the  Parks-McClellan
Exchange  technique  is  specific  to  the  available  filter  design  software.  Although  the  mathematics
involved  in  the  development  of  the  Parks-McClellan  Exchange  method  is  rather  complicated,  we
don’t have to worry about that here[16–20]. Just remember that the Parks-McClellan Exchange design
method gives us a Chebyshev-type filter whose actual frequency response is as close as possible to
the desired Hd(m) response for a given number of filter taps.
Figure 5-31 Alternate method for defining the desired frequency response of a lowpass FIR filter
using the Parks-McClellan Exchange technique.
To illustrate the advantage of the Parks-McClellan method, the solid curve in Figure 5-32 shows the
frequency response of a 31-tap FIR designed using this technique. For comparison, Figure 5-32 also
shows  the  frequency  responses  of  two  31-tap  FIR  filters  for  the  same  passband  width  using  the
Chebyshev  and  Kaiser  windowing  techniques.  Notice  how  the  three  filters  have  roughly  the  same
stopband  sidelobe  levels,  near  the  main  lobe,  but  that  the  Parks-McClellan  filter  has  the  more
desirable (steeper) transition band roll-off.
Figure 5-32 Frequency response comparison of three 31-tap FIR filters: Parks-McClellan,
Chebyshev windowed, and Kaiser windowed.
The  Parks-McClellan  Exchange  filter  design  method  revolutionized  the  art  of,  and  has  become  the
predominant technique for, designing linear-phase FIR filters. As a historical note, when Profs. Parks
and McClellan (James McClellan was a graduate student at the time) developed their triumphant filter
design method in 1971, they submitted a paper to Electronics Letters to publicize their achievement.
Surprisingly,  the  editors  of  Electronics  Letters  rejected  the  paper  because  the  reviewers  didn’t
believe  that  such  a  flexible,  and  optimized,  FIR  design  procedure  was  possible.  A  description  of
Parks and McClellan’s revolutionary design method was eventually published in reference[17]. That
story  is  reminiscent  of  when  Decca  Records  auditioned  a  group  of  four  young  musicians  in  1961.
Decca  executives  decided  not  to  sign  the  group  to  a  contract.  You  may  have  heard  of  that  musical
group—they were called the Beatles.
5.7 Half-band FIR Filters
There’s  a  specialized  FIR  filter  that’s  proved  very  useful  in  signal  decimation  and  interpolation
applications[21–25]. Called a half-band FIR filter, its frequency magnitude response is symmetrical
about the fs/4 point as shown in Figure 5-33(a). As such, the sum of fpass and fstop is fs/2. When the
filter has an odd number of taps, this symmetry has the beautiful property that the filter’s time-domain
impulse  response  has  every  other  filter  coefficient  being  zero,  except  the  center  coefficient.  This
enables us to avoid approximately half the number of multiplications when implementing this kind of
filter. By way of example, Figure 5-33(b) shows the coefficients for a 31-tap half-band filter where
Δf was defined to be approximately fs/32 using the Parks-McClellan FIR filter design method.
Figure 5-33 Half-band FIR filter: (a) frequency magnitude response [transition region centered at
fs/4]; (b) 31-tap filter coefficients; (c) 7-tap half-band filter structure.
Notice  how  the  alternating  h(k)  coefficients  are  zero,  so  we  perform  17  multiplications  per  output
sample  instead  of  the  expected  31  multiplications.  Stated  in  different  words,  we  achieve  the
performance of a 31-tap filter at the computational expense of only 17 multiplies per output sample.
In  the  general  case,  for  an  N-tap  half-band  FIR  filter,  we’ll  only  need  to  perform  (N  +  1)/2  +  1
multiplications per output sample. (Section 13.7 shows a technique to further reduce the number of
necessary multiplies for linear-phase tapped-delay line FIR filters, including half-band filters.) The
structure of a simple seven-coefficient half-band filter is shown in Figure 5-33(c), with the h(1) and
h(5) multipliers absent.
Be aware, there’s a restriction on the number of half-band filter coefficients. To build linear-phase N-
tap  half-band  FIR  filters,  having  alternating  zero-valued  coefficients,  N  +  1  must  be  an  integer
multiple of four. If this restriction is not met, for example when N = 9, the first and last coefficients of
the filter will both be equal to zero and can be discarded, yielding a 7-tap half-band filter.
On a practical note, there are two issues to keep in mind when we use an FIR filter design software
package  to  design  a  half-band  filter.  First,  assuming  that  the  modeled  filter  has  a  passband  gain  of
unity,  ensure  that  your  filter  has  a  gain  of  0.5  (−6  dB)  at  a  frequency  of  fs/4.  Second,  unavoidable
numerical computation errors will yield alternate filter coefficients that are indeed very small but not
exactly zero-valued as we desire. So in our filter modeling efforts, we must force those very small
coefficient values to zero before we proceed to analyze half-band filter frequency responses.
You might sit back and think, “OK, these half-band filters are mildly interesting, but they’re certainly
not worth writing home about.” As it turns out, half-band filters are very important because they’re
widely  used  in  applications  with  which  you’re  familiar—like  pagers,  cell  phones,  digital
receivers/televisions, CD/DVD players, etc. We’ll learn more about half-band filter applications in
Chapter 10.
5.8 Phase Response of FIR Filters
Although we illustrated a couple of output phase shift examples for our original averaging FIR filter
in Figure 5-10, the subject of FIR phase response deserves additional attention. One of the dominant
features of FIR filters is their linear phase response which we can demonstrate by way of example.
Given the 25 h(k) FIR filter coefficients in Figure 5-34(a), we can perform a DFT to determine the
filter’s H(m) frequency response. The normalized real part, imaginary part, and magnitude of H(m)
are shown in Figures 5-34(b) and 5-34(c), respectively.† Being complex values, each H(m) sample
value can be described by its real and imaginary parts, or equivalently, by its magnitude |H(m)| and its
phase Hø(m) shown in Figure 5-35(a).
† Any DFT size greater than the h(k) width of 25 is sufficient to obtain H(m). The h(k) sequence was padded with 103 zeros to take a
128-point DFT, resulting in the H(m) sample values in Figure 5-34.
Figure 5-34 FIR filter frequency response H(m): (a) h(k) filter coefficients; (b) real and imaginary
parts of H(m); (c) magnitude of H(m).
Figure 5-35 FIR filter phase response Hø(m) in degrees: (a) calculated Hø(m); (b) polar plot of
Hø(m)’s first ten phase angles in degrees; (c) actual Hø(m).
The phase of a complex quantity is, of course, the arctangent of the imaginary part divided by the real
part, or ø = tan −1(imag/real). Thus the phase of Hø(m) is determined from the samples in Figure 5-
34(b).
The phase response in Figure 5-35(a) certainly looks linear over selected frequency ranges, but what
do we make of those sudden jumps, or discontinuities, in this phase response? If we were to plot the
angles of Hø(m) starting with the m = 0 sample on a polar graph, using the nonzero real part of H(0),
and the zero-valued imaginary part of H(0), we’d get the zero-angled Hø(0) phasor shown on the right
side  of  Figure  5-35(b).  Continuing  to  use  the  real  and  imaginary  parts  of  H(m)  to  plot  additional
phase angles results in the phasors going clockwise around the circle in increments of −33.75°. It’s at
the Hø(6) that we discover the cause of the first discontinuity in Figure 5-35(a). Taking the real and
imaginary parts of H(6),  we’d  plot  our  phasor  oriented  at  an  angle  of  −202.5°.  But  Figure  5-35(a)
shows that Hø(6)  is  equal  to  157.5°.  The  problem  lies  in  the  software  routine  used  to  generate  the
arctangent  values  plotted  in  Figure 5-35(a).  The  software  adds  360°  to  any  negative  angles  in  the
range of −180° > ø ≥ −360°, i.e., angles in the upper half of the circle. This makes ø a positive angle
in the range of 0° < ø ≤ 180° and that’s what gets plotted. (This apparent discontinuity between Hø(5)
and Hø(6)  is  called  phase wrapping.)  So  the  true  Hø(6)  of  −202.5°  is  converted  to  a  +157.5°  as
shown in parentheses in Figure 5-35(b). If we continue our polar plot for additional Hø(m) values,
we’ll  see  that  their  phase  angles  continue  to  decrease  with  an  angle  increment  of  −33.75°.  If  we
compensate  for  the  software’s  behavior  and  plot  phase  angles  more  negative  than  −180°,  by
unwrapping the phase, we get the true Hø(m) shown in Figure 5-35(c).
Notice that Hø(m) is, indeed, linear over the passband of H(m). It’s at Hø(17) that our particular H(m)
experiences a polarity change of its real part while its imaginary part remains negative—this induces
a  true  phase-angle  discontinuity  that  really  is  a  constituent  of  H(m)  at  m  =  17.  (Additional  phase
discontinuities occur each time the real part of H(m) reverses polarity, as shown in Figure 5-35(c).)
The  reader  may  wonder  why  we  care  about  the  linear  phase  response  of  H(m).  The  answer,  an
important one, requires us to introduce the notion of group delay.
Group delay is defined as the negative of the derivative of the phase with respect to frequency, or G =
−dø/df. For FIR filters, then, group delay is the slope of the Hø(m) response curve. When the group
delay  is  constant,  as  it  is  over  the  passband  of  all  FIR  filters  having  symmetrical  coefficients,  all
frequency components of the filter input signal are delayed by an equal amount of time G before they
reach the filter’s output. This means that no phase distortion is induced in the filter’s desired output
signal,  and  this  is  crucial  in  communications  signals.  For  amplitude  modulation  (AM)  signals,
constant group delay preserves the time waveform shape of the signal’s modulation envelope. That’s
important  because  the  modulation  portion  of  an  AM  signal  contains  the  signal’s  information.
Conversely,  a  nonlinear  phase  will  distort  the  audio  of  AM  broadcast  signals,  blur  the  edges  of
television video images, blunt the sharp edges of received radar pulses, and increase data errors in
digital  communications  signals.  (Group  delay  is  sometimes  called  envelope  delay  because  group
delay was originally the subject of analysis due to its effect on the envelope, or modulation signal, of
amplitude  modulation  AM  systems.)  Of  course  we’re  not  really  concerned  with  the  group  delay
outside  the  passband  because  signal  energy  outside  the  passband  is  what  we’re  trying  to  eliminate
through filtering.
Over the passband frequency range for a linear-phase, S-tap FIR filter, group delay has been shown to
be given by
(5-23)
where D = S−1  is  the  number  of  unit-delay  elements  in  the  filter’s  delay  line,  and  ts  is  the  sample
period (1/fs).† This group delay is measured in seconds. Eliminating the ts factor in Eq. (5-23) would
change its dimensions to samples. The value G, measured in samples, is always an integer for odd-tap
FIR filters and a noninteger for even-tap filters.
† As derived in Section 3.4 of reference [16], and page 597 of reference [19].
Although we used a 128-point DFT to obtain the frequency responses in Figures 5-34 and 5-35, we
could just as well have used N = 32-point or N  =  64-point  DFTs.  These  smaller  DFTs  give  us  the
phase  response  curves  shown  in  Figures  5-36(a)  and  5-36(b).  Notice  how  different  the  phase
response curves are when N = 32 in Figure 5-36(a) compared to when N = 128 in Figure 5-36(c).
The phase-angle resolution is much finer in Figure 5-36(c). The passband phase-angle resolution, or
increment Δø, is given by
(5-24)
Figure 5-36 FIR filter phase response Hø(m) in degrees: (a) calculated using a 32-point DFT; (b)
using a 64-point DFT; (c) using a 128-point DFT.
where N is the number of points in the DFT. So, for our S = 25-tap filter in Figure 5-34(a), G = 12,
and Δø is equal to −12 · 360°/32 = −135° in Figure 5-36(a), and Δø is −33.75° in Figure 5-36(c). If
we look carefully at the sample values in Figure 5-36(a), we’ll see that they’re all included within the
samples in Figures 5-36(b) and 5-36(c).
Let’s conclude this FIR phase discussion by reiterating the meaning of phase response. The phase, or
phase delay, at the output of an FIR filter is the phase of the first output sample relative to the phase of
the  filter’s  first  input  sample.  Over  the  passband,  that  phase  shift,  of  course,  is  a  linear  function  of
frequency. This will be true only as long as the filter has symmetrical coefficients. Figure 5-10 is a
good illustration of an FIR filter’s output phase delay.
For FIR filters, the output phase shift measured in degrees, for the passband frequency f = mfs/N, is
expressed as
(5-25)
We can illustrate Eq. (5-25) and show the relationship between the phase responses in Figure 5-36 by
considering the phase delay associated with the frequency of fs/32 in Table 5-2. The subject of group
delay is described further in Appendix F, where an example of envelope delay distortion, due to a
filter’s nonlinear phase, is illustrated.
Table 5-2 Values Used in Eq. (5-25) for the Frequency fs/32
5.9 A Generic Description of Discrete Convolution
Although  convolution  was  originally  an  analysis  tool  used  to  prove  continuous  signal  processing
theorems,  we  now  know  that  convolution  affects  every  aspect  of  digital  signal  processing.
Convolution influences our results whenever we analyze or filter any finite set of data samples from a
linear time-invariant system. Convolution not only constrains DFTs to be just approximations of the
continuous  Fourier  transform;  it  is  the  reason  that  discrete  spectra  are  periodic  in  the  frequency
domain.  It’s  interesting  to  note  that,  although  we  use  the  process  of  convolution  to  implement  FIR
digital filters, convolution effects induce frequency response ripple, preventing us from ever building
a perfect digital filter. Its influence is so pervasive that to repeal the law of convolution, quoting a
phrase from Dr. Who, would “unravel the entire causal nexus” of digital signal processing.
Convolution has always been a somewhat difficult concept for the beginner to grasp. That’s not too
surprising  for  several  reasons.  Convolution’s  effect  on  discrete  signal  processing  is  not  intuitively
obvious  for  those  without  experience  working  with  discrete  signals,  and  the  mathematics  of
convolution  does  seem  a  little  puzzling  at  first.  Moreover,  in  their  sometimes  justified  haste,  many
authors  present  the  convolution  equation  and  abruptly  start  using  it  as  an  analysis  tool  without
explaining  its  origin  and  meaning.  For  example,  this  author  once  encountered  what  was  called  a
tutorial article on the FFT in a professional journal that proceeded to define convolution merely by
presenting something like that shown in Figure 5-37 with no further explanation!
Figure 5-37 One very efficient, but perplexing, way of defining convolution.
Unfortunately, few beginners can gain an understanding of the convolution process from Figure 5-37
alone.  Here,  we  avoid  this  dilemma  by  defining  the  process  of  convolution  and  gently  proceed
through a couple of simple convolution examples. We conclude this chapter with a discussion of the
powerful  convolution  theorem  and  show  why  it’s  so  useful  as  a  qualitative  tool  in  discrete  system
analysis.
5.9.1 Discrete Convolution in the Time Domain
Discrete  convolution  is  a  process  whose  input  is  two  sequences  and  that  provides  a  single  output
sequence. Convolution inputs can be two time-domain sequences giving a time-domain output, or two
frequency-domain  input  sequences  providing  a  frequency-domain  result.  (Although  the  two  input
sequences  must  both  be  in  the  same  domain  for  the  process  of  convolution  to  have  any  practical
meaning, their sequence lengths need not be the same.) Let’s say we have two input sequences h(k) of
length P and x(k) of length Q in the time domain. The output sequence y(n) of the convolution of the
two inputs is defined mathematically as
(5-26)
Let’s examine Eq. (5-26) by way of example, using the h(k) and x(k) sequences shown in Figure 5-38.
In this example, we can write the terms for each y(n) in Eq. (5-26) as
Figure 5-38 Convolution example input sequences: (a) first sequence h(k) of length P = 4; (b) second
sequence x(k) of length Q = 3.
(5-27)
With P = 4 and Q = 3, we need evaluate only 4 + 3 − 1 = 6 individual y(n) terms. Because h(4) and
h(5) are zero, we can eliminate some of the terms in Eq. (5-27) and evaluate the remaining x(n−k)
indices, giving the following expressions for y(n) as
(5-28)
Looking  at  the  indices  of  the  h(k)  and  x(k)  terms  in  Eq. (5-28),  we  see  two  very  important  things
occurring. First, convolution is merely the summation of a series of products—so the process itself is
not very complicated. Second, notice that, for a given y(n), h(k)’s index is increasing as x(k)’s index
is  decreasing.  This  fact  has  led  many  authors  to  introduce  a  new  sequence  x(−k)  and  use  that  new
sequence to graphically illustrate the convolution process. The x(−k) sequence is simply our original
x(k) reflected about the 0 index of the k axis as shown in Figure 5-39. Defining x(−k) as such enables
us to depict the products and summations of Eq. (5-28)’s convolution as in Figure 5-40; that is, we
can now align the x(−k) samples with the samples of h(k) for a given n index to calculate y(n). As
shown in Figure 5-40(a), the alignment of h(k) and x(n−k), for n = 0, yields y(0) = 1. This is the result
of the first line in Eq. (5-28) repeated on the right side of Figure 5-40(a). The calculation of y(1), for
n = 1, is depicted in Figure 5-40(b), where x(n−k) is shifted one element to the right, resulting in y(1)
=  3.  We  continue  this  x(n−k)  shifting  and  incrementing  n  until  we  arrive  at  the  last  nonzero
convolution result of y(5) shown in Figure 5-40(f). So, performing the convolution of h(k) and x(k)
comprises
1. plotting both the h(k) and x(k) sequences,
2. flipping the x(k) sequence around the k = 0 sample to obtain x(−k),
3. summing the products of h(k) and x(0−k) for all k to yield y(0),
4. shifting the x(−k) sequence one sample to the right,
5. summing the products of h(k) and x(1−k) for all k to obtain y(1), and
6. continuing to shift x(−k) and sum products until there’s no overlap of h(k) and the shifted x(n−k),
in which case all further y(n) output samples are zero and we’re done.
Figure 5-39 Convolution example input sequence: (a) second sequence x(k) of length 3; (b) reflection
of the second sequence about the k = 0 index.
The  full  convolution  of  our  h(k)  and  x(k)  is  the  y(n)  sequence  on  the  right  side  of  Figure  5-40(f).
We’ve  scanned  the  x(−k)  sequence  across  the  h(k)  sequence  and  summed  the  products  where  the
sequences overlap. By the way, notice that the y(n) sequence in Figure 5-40(f) has six elements where
h(k) had a length of four and x(k) was of length three. In the general case, if h(k) is of length P and
x(k) is of length Q, the length of y(n) will have a sequence length of L, where
(5-29)
Figure 5-40 Graphical depiction of the convolution of h(k) and x(k) in Figure 5-38.
At  this  point,  it’s  fair  for  the  beginner  to  ask,  “OK,  so  what?  What  does  this  strange  convolution
process have to do with digital signal processing?” The answer to that question lies in understanding
the effects of the convolution theorem.
5.9.2 The Convolution Theorem
The  convolution  theorem  is  a  fundamental  constituent  of  digital  signal  processing.  It  impacts  our
results anytime we filter or Fourier transform discrete data. To see why this is true, let’s simplify the
notation of Eq. (5-26) and use the abbreviated form
(5-30)
where, again, the “*” symbol means convolution. The convolution theorem may be stated as follows:
If two time-domain sequences h(k) and x(k) have DFTs of H(m) and X(m), respectively, then the DFT
of h(k) * x(k) is the product H(m) · X(m). Likewise, the inverse DFT of H(m) · X(m) is h(k) * x(k).
We can represent this relationship with the expression
(5-31)
Equation (5-31) tells us that two sequences resulting from h(k) * x(k) and H(m) · X(m) are Fourier
transform pairs. So, taking the DFT of h(k) * x(k) always gives us H(m) · X(m). Likewise, we can
determine h(k) * x(k) by taking the inverse DFT of H(m) · X(m). The important point to learn from Eq.
(5-31) is that convolution in the time domain is equivalent to multiplication in the frequency domain.
(We  won’t  derive  the  convolution  theorem  here  because  its  derivation  is  readily  available  to  the
interested reader[26–29].) To help us appreciate this principle, Figure 5-41 sketches the relationship
between convolution in the time domain and multiplication in the frequency domain.
Figure 5-41 Relationships of the convolution theorem.
We can easily illustrate the convolution theorem by taking 8-point DFTs of h(k) and x(k) to get H(m)
and X(m), respectively, and listing these values as in Table 5-3. (Of course, we have to pad h(k) and
x(k) with zeros, so they both have lengths of 8 to take 8-point DFTs.) Tabulating the inverse DFT of
the product H(m) · X(m) allows us to verify Eq. (5-31), as listed in the last two columns of Table 5-3,
where the acronym IDFT again means inverse DFT. The values from Table 5-3 are shown in Figure
5-42. (For simplicity, only the magnitudes of H(m), X(m), and H(m) · X(m) are shown in the figure.)
We need to become comfortable with convolution in the time domain because, as we’ve learned, it’s
the  process  used  in  FIR  filters.  As  detailed  in  Section 5.2,  we  perform  discrete  time-domain  FIR
filtering by convolving an input sequence, x(n) say, with the impulse response h(k) of a filter, and for
FIR  filters  that  impulse  response  happens  to  also  be  the  filter’s  coefficients.†  The  result  of  that
convolution  is  a  filtered  time-domain  sequence  whose  spectrum  is  modified  (multiplied)  by  the
filter’s frequency response X(m). Section 13.10 describes a clever scheme to perform FIR filtering
efficiently using the FFT algorithm to implement convolution.
†  As  we’ll  see  in  Chapter  6,  the  coefficients  used  for  an  infinite  impulse  response  (IIR)  filter  are  not  equal  to  that  filter’s  impulse
response.
Table 5-3 Convolution Values of h(k) and x(k) from Figure 5-38
Figure 5-42 Convolution relationships of h(k), x(k), H(m), and X(m) from Figure 5-38.
Because  of  the  duality  of  the  convolution  theorem,  we  could  have  swapped  the  time  and  frequency
domains  in  our  discussion  of  convolution  and  multiplication  being  a  Fourier  transform  pair.  This
means that, similar to Eq. (5-31), we can also write
(5-32)
So the convolution theorem can be stated more generally as Convolution in one domain is equivalent
to multiplication in the other domain. Figure 5-43 shows the relationship between multiplication in
the  time  domain  and  convolution  in  the  frequency  domain.  Equation  (5-32)  is  the  fundamental
relationship used in the process of windowing time-domain data to reduce DFT leakage, as discussed
in Section 3.9.
Figure 5-43 Relationships of the convolution theorem related to multiplication in the time domain.
5.9.3 Applying the Convolution Theorem
The convolution theorem is useful as a qualitative tool in predicting the effects of different operations
in discrete linear time-invariant systems. For example, many authors use the convolution theorem to
show  why  periodic  sampling  of  continuous  signals  results  in  discrete  samples  whose  spectra  are
periodic in the frequency domain. Consider the real continuous time-domain waveform in Figure 5-
44(a),  with  the  one-sided  spectrum  of  bandwidth  B. Being  a  real  signal,  of  course,  its  spectrum  is
symmetrical about 0 Hz. (In Figure 5-44, the large right-pointing arrows represent Fourier transform
operations.)  Sampling  this  waveform  is  equivalent  to  multiplying  it  by  a  sequence  of  periodically
spaced  impulses,  Figure  5-44(b),  whose  values  are  unity.  If  we  say  that  the  sampling  rate  is  fs
samples/second,  then  the  sample  period  ts  =  1/fs  seconds.  The  result  of  this  multiplication  is  the
sequence  of  discrete  time-domain  impulses  shown  in  Figure  5-44(c).  We  can  use  the  convolution
theorem  to  help  us  predict  what  the  frequency-domain  effect  is  of  this  multiplication  in  the  time
domain. From our theorem, we now realize that the spectrum of the time-domain product must be the
convolution  of  the  original  spectra.  Well,  we  know  what  the  spectrum  of  the  original  continuous
waveform  is.  What  about  the  spectrum  of  the  time-domain  impulses?  It  has  been  shown  that  the
spectrum of periodic impulses, whose period is ts seconds, is also periodic impulses in the frequency
domain with a spacing of fs Hz as shown in Figure 5-44(b)[30].
Figure 5-44 Using convolution to predict the spectral replication effects of periodic sampling.
Now,  all  we  have  to  do  is  convolve  the  two  spectra.  In  this  case,  convolution  is  straightforward
because both of the frequency-domain functions are symmetrical about the zero-Hz point, and flipping
one of them about zero Hz is superfluous. So we merely slide one of the functions across the other and
plot  the  product  of  the  two.  The  convolution  of  the  original  waveform  spectrum  and  the  spectral
impulses results in replications of the waveform spectrum every fs Hz, as shown in Figure 5-44(c).
This discussion reiterates the fact that the DFT is always periodic with a period of fs Hz.
Here’s another example of how the convolution theorem can come in handy when we try to understand
digital signal processing operations. This author once used the theorem to resolve the puzzling result,
at  the  time,  of  a  triangular  window  function  having  its  first  frequency  response  null  at  twice  the
frequency of the first null of a rectangular window function. The question was “If a rectangular time-
domain function of width T has its first spectral null at 1/T Hz, why does a triangular time-domain
function of width T have its first spectral null at 2/T Hz?” We can answer this question by considering
convolution in the time domain.
Look  at  the  two  rectangular  time-domain  functions  shown  in  Figures  5-45(a)  and  5-45(b).  If  their
widths  are  each  T  seconds,  their  spectra  are  shown  to  have  nulls  at  1/T  Hz  as  depicted  in  the
frequency-domain functions in Figures 5-45(a) and 5-45(b).  We  know  that  the  frequency  magnitude
responses  will  be  the  absolute  value  of  the  classic  sin(x)/x  function.†  If  we  convolve  those  two
rectangular  time-domain  functions  of  width  T,  we’ll  get  the  triangular  function  shown  in  Figure  5-
45(c). Again, in this case, flipping one rectangular function about the zero time axis is unnecessary. To
convolve  them,  we  need  only  scan  one  function  across  the  other  and  determine  the  area  of  their
overlap.  The  time  shift  where  they  overlap  the  most  happens  to  be  a  zero  time  shift.  Thus,  our
resultant convolution has a peak at a time shift of zero seconds because there’s 100 percent overlap. If
we  slide  one  of  the  rectangular  functions  in  either  direction,  the  convolution  decreases  linearly
toward zero. When the time shift is T/2 seconds, the rectangular functions have a 50 percent overlap.
The convolution is zero when the time shift is T seconds—that’s when the two rectangular functions
cease to overlap.
† The sin(x)/x function was introduced in our discussion of window functions in Section 3.9  and  is  covered  in  greater  detail  in  Section
3.13.
Figure 5-45 Using convolution to show that the Fourier transform of a triangular function has its first
null at twice the frequency of the Fourier transform of a rectangular function.
Notice that the triangular convolution result has a width of 2T, and that’s really the key to answering
our question. Because convolution in the time domain is equivalent to multiplication in the frequency
domain, the Fourier transform magnitude of our 2T-width triangular function is the |sin(x)/x| in Figure
5-45(a)  times  the  |sin(x)/x|  in  Figure  5-45(b),  or  the  (sin(x)/x)2  function  in  Figure  5-45(c).  If  a
triangular function of width 2T has its first frequency-domain null at 1/T Hz, then the same function of
width T must have its first frequency null at 2/T Hz as shown in Figure 5-45(d), and that’s what we
set  out  to  show.  Comparison  of  Figures  5-45(c)  and  5-45(d)  illustrates  a  fundamental  Fourier
transform  property  that  compressing  a  function  in  the  time  domain  results  in  an  expansion  of  its
corresponding frequency-domain representation.
We cannot overemphasize the importance of the convolution theorem as an analysis tool. As an aside,
for years I thought convolution was a process developed in the second half of the twentieth century to
help  us  analyze  discrete-time  signal  processing  systems.  Later  I  learned  that  statisticians  had  been
using convolution since the late 1800s. In statistics the probability density function (PDF) of the sum
of two random variables is the convolution of their individual PDFs.
5.10 Analyzing FIR Filters
There are two popular ways to analyze tapped-delay line, nonrecursive FIR filters. The first way uses
continuous-time  Fourier  algebra,  and  the  second  way  uses  the  discrete  Fourier  transform.  (By
“analyze  an  FIR  filter”  we  mean  determining  the  FIR  filter’s  frequency  response  based  on  known
filter coefficients.) Let’s quickly review the two FIR filter analysis methods.
5.10.1 Algebraic Analysis of FIR Filters
The  algebraic  method  used  to  analyze  nonrecursive  FIR  filters  uses  the  discrete-time  Fourier
transform  (DTFT)  equation.  Linear  system  theory  tells  us  that  the  frequency  response  of  a  linear
system (our filter) is the Fourier transform of that system’s impulse response. Because a tapped-delay
line  FIR  filter’s  impulse  response  is  equal  to  its  coefficient  values,  we  proceed  by  expressing  the
Fourier  transform  of  the  filter’s  coefficients.  In  Section 3.14  we  learned  that  we  can  describe  the
continuous Fourier transform of a discrete sequence using the DTFT expressed as
(5-33)
Modifying Eq. (5-33)’s  notation  to  correspond  to  the  DTFT  of  an  FIR  filter  having  N  coefficients
(impulse response) represented by h(k), where index k = 0, 1, 2, ..., N−1, we can express the filter’s
complex frequency response as
(5-34)
H(ω) is an (N−1)th-order polynomial, and this is why, for example, a 6-tap FIR filter is often called a
5th-order FIR filter. In Eq. (5-34) the digital frequency variable ω is continuous and ranges from 0 to
2π radians/sample, corresponding to a continuous-time frequency range of 0 to fs Hz.
Let’s see how Eq. (5-34)  is  used  to  determine  the  frequency  response  of  an  FIR  filter.  Assume  we
have a 4-tap FIR filter whose coefficients are h(k) = [0.2, 0.4, 0.4, 0.2]. In this case our continuous
H(ω) equation becomes
(5-35)
We can, if we wish, write the complex 3rd-order Eq. (5-35) in rectangular form as
(5-36)
Evaluating  Eq.  (5-35),  or  Eq.  (5-36),  and  plotting  the  magnitude  of  the  continuous  complex  H(ω)
function results in the curve in Figure 5-46(a). To compute the continuous Hø(ω) phase function, we
merely take the arctangent of the ratio of the imaginary part over the real part of H(ω), yielding the
Hø(ω) phase response in Figure 5-46(b).
Figure 5-46 FIR filter frequency response: (a) magnitude; (b) phase.
In practice, evaluating Eq. (5-34) would be performed using some sort of commercial math software,
where code must be written to compute a sampled version of the continuous H(ω). Rather than writing
the  code  to  implement  Eq.  (5-34),  fortunately  we  can  conveniently  compute  an  FIR  filter’s  H(ω)
frequency response using software that performs the discrete Fourier transform. That’s the subject we
discuss next.
5.10.2 DFT Analysis of FIR Filters
The most convenient way to determine an FIR filter’s frequency response is to perform the discrete
Fourier transform (DFT) of the filter’s coefficients. This analysis method is popular because the DFT
is  built  into  most  commercial  signal  processing  software  packages  such  as  MathCAD,  LabView,
MATLAB, etc. (In fact, in a pinch, we can even compute DFTs with Microsoft Excel.) The DFT of an
FIR filter’s coefficients is computed using
(5-37)
which we normally implement with the high-speed fast Fourier transform (FFT) algorithm. Variables
m and n both range from 0 to N-1.
Ah, but there’s trouble in paradise because Eq. (5-37) poses a problem. If we perform a 4-point DFT
of the above 4-tap FIR filter coefficients, h(k) = [0.2, 0.4, 0.4, 0.2] as shown in Figure 5-47(a), we
obtain  the  |H(m)|  samples  in  Figure  5-47(b).  That  |H(m)|  sequence  reveals  very  little  about  the
frequency response of the 4-tap FIR filter. We need more |H(m)| frequency-domain information. That
is, we need improved frequency resolution.
Figure 5-47 Four-tap FIR filter: (a) impulse response; (b) 4-point DFT frequency magnitude
response.
Fortunately we can obtain a finer-granularity version of H(m) by zero padding the h(k) coefficients
with  zero-valued  samples  and  performing  a  larger-sized  DFT.  Figure 5-48(a)  shows  the  4-tap  FIR
filter’s coefficients padded with 60 zero-valued samples. Performing a 64-point DFT on that padded
h(k)  sequence  yields  the  higher-resolution  discrete  |H(m)|  magnitude  response  sequence  shown  in
Figure 5-48(b). Sequence |H(m)| is, of course, computed using
(5-38)
Figure 5-48 High-resolution FIR filter frequency response: (a) zero-padded h(k); (b) discrete
magnitude response; (c) phase response.
where Hreal(m) and Himag(m) are the real and imaginary parts computed using Eq. (5-37). The circular
white dots in Figure 5-48(b) correspond to the square dots in Figure 5-47(b).
Remember, now, a filter’s complex H(m) frequency response sequence is
(5-38′)
comprising  a  real-valued  |H(m)|  magnitude  response  times  a  complex  ejHø(m)  phase  response.  The
real-valued phase-angle samples, shown in Figure 5-48(c), are computed using
(5-39)
So,  our  FIR  filter  analysis  rule  of  thumb  is  to  append  a  sequence  of  zero-valued  samples  (whose
length is, say, 10N) to an N-tap filter’s h(k) impulse response. Appending those zero-valued samples
is  called  zero padding  the  h(k)  sequence.  Next  we  compute  the  DFT  of  that  padded  sequence.  Of
course the final zero-padded sequence should have a length that is an integer power of two so that we
can use the FFT to compute the high-resolution H(m).
By the way, it doesn’t matter if the zero-valued samples are placed before or after the original h(k)
coefficients  prior  to  performing  the  DFT.  The  computed  high-resolution  |H(m)|  magnitude  sequence
will be the same in either case, and the resulting Hø(m) phase samples in the two cases will differ
only by a constant phase angle. (The DFT shifting theorem discussed in Section 3.6 explains why this
is true.)
5.10.3 FIR Filter Group Delay Revisited
We mentioned in Section 5.8 how a constant time delay, what we formally refer to as group delay,
through a filter was crucial in many applications. A constant group delay means a filter has a linear
phase response over its passband and will induce no phase distortion in its output signals. Here we
explore the concept of group delay a bit further.
The  group  delay,  as  a  function  of  frequency,  of  a  filter  having  a  frequency  response  of  H(ω)  =
|H(ω)|ejHø(ω)  is  the  negative  of  the  derivative  of  the  filter’s  Hø(ω)  phase  response  with  respect  to
frequency ω and is expressed as
(5-40)
where digital frequency ω is continuous and ranges from −π to π radians/sample, corresponding to a
continuous-time  frequency  range  of  −fs/2 to fs/2  Hz.  Because  the  dimensions  of  Hø(ω)  are  radians,
and the dimensions of ω are radians/sample, the dimensions of group delay G(ω) are time measured
in samples. We graphically depict the notion of the group delay, for a lowpass filter, in Figure 5-49.
Figure 5-49 FIR filter group delay derived from a filter’s phase response.
For example, the complex-valued frequency response of a K-tap moving average filter is
(5-41)
where the subscript “ma” means moving average. As such, from Eq. (5-41) the phase response of a K
= 5-tap moving average filter is
(5-42)
Using Eq. (5-40), the group delay of a K = 5-tap moving average filter is
(5-43)
Luckily for us, Eq. (5-40) becomes very simple to evaluate if an N-tap FIR filter’s h(k) coefficients
(impulse response samples) are symmetrical. By “symmetrical” we mean h(k) coefficients that abide
by
(5-44)
where 0≤k≤(N−1)/2 when N is odd, and 0≤k≤(N/2)−1 when N is even. Equation (5-44) merely means
that the first coefficient equals the last coefficient, the second coefficient equals the next to the last
coefficient, and so on. All of the FIR filters we’ve discussed, so far, fall into this category.
OK, here’s the point we’re making. For symmetrical-coefficient FIR filters that comply with Eq. (5-
44), their group delay is simple to compute. The group delay of such filters, measured in samples, is a
constant equal to half the number of delay elements in the filter’s tapped-delay line structure. That is,
(5-45)
where  D  is  the  number  of  unit-delay  elements  in  the  filter’s  delay  line.  Measured  in  seconds,  a
symmetrical FIR filter’s group delay is
(5-46)
where ts is the reciprocal of the filter’s fs input signal sample rate.
We can now make the following all-encompassing statement: The group delay of a tapped-delay line
FIR digital filter, whose impulse response is symmetric, is equal to
(5-47)
For our purposes, we view a linear-phase FIR filter’s group delay as simply the time delay through
the  filter.  That  is,  if  the  group  delay  of  a  filter  is  G  samples,  then  the  filter’s  output  sequence  is
delayed by G samples relative the filter’s input sequence.
It’s worth mentioning at this point that although we have not yet discussed such filter networks, if a
tapped-delay line (FIR) network has an antisymmetrical impulse response defined by
(5-48)
where 0≤k≤(N−1)/2  when  N  is  odd  and  0≤k≤(N/2)−1  when  N  is  even,  such  a  network  also  has  a
linear phase response and its group delay is also described by Eq. (5-47). Digital differentiators and
Hilbert transformers, discussed in later chapters, fall into this category.
At this point, looking at Eq. (5-45), the DSP novice may wonder, “If D is an odd number, how is it
possible to have a discrete signal sequence delayed by a noninteger number of samples?” The answer
to this sensible question is illustrated in Figure 5-50, where x(n) is a sinusoidal sequence applied to a
symmetrical FIR filter having 6 taps (D = 5 delay elements in the tapped-delay line). There we see
that the sinusoidal sequence is preserved at the filter’s y(n) output and delayed relative to input x(n)
by a group delay value of exactly 5/2 samples. In the lingo of digital filters, the behavior in Figure 5-
50 is called fractional delay.
Figure 5-50 Group delay of a 6-tap (5 delay elements) FIR filter.
Again,  constant  group  delay—linear  phase—is  a  desirable  filter  property  because  the  spectral
components in the filter’s output signal will suffer no phase distortion. Stated in different words: all
spectral components within a linear-phase filter’s passband will be delayed by the same amount of
time  as  they  pass  through  the  filter.  If  a  linear-phase  filter’s  input  is  a  complicated  digital
communications signal, rich in spectral-phase complexity representing digital data, the spectral-phase
relationships and the digital data are preserved undistorted at the filter’s output. Their linear-phase
property is the reason we use FIR filters!
5.10.4 FIR Filter Passband Gain
One  FIR  filter  property  that  is  of  interest  is  the  filter’s  passband gain.  The  standard  definition  of
passband gain is that it is the filter’s passband magnitude response level around which the passband
ripple fluctuates, as shown by the lowpass filter in Figure 5-51 where the passband gain equals unity.
In practice we design filters to have very small passband ripple, so a lowpass filter’s passband gain
is roughly equal to its DC gain (gain at zero Hz), which is the sum of the filter’s impulse response
sequence,  i.e.,  the  sum  of  the  FIR  filter’s  coefficients.  (We  leave  the  proof  of  this  as  a  homework
problem.) Most commercial FIR filter design software packages compute filter coefficients such that
their passband gain is unity.
Figure 5-51 FIR filter passband gain definition.
5.10.5 Estimating the Number of FIR Filter Taps
Our final topic regarding the analysis of FIR filters is: How do we estimate the number of filter taps
(coefficients),  N,  that  can  satisfy  a  given  frequency  magnitude  response  of  an  FIR  filter?  Several
authors  have  proposed  empirical  relationships  for  estimating  N  for  traditional  tapped-delay  line
lowpass FIR filters based on the desired passband ripple, stopband attenuation, and transition region
width[24,31–33].  A  particularly  simple  expression  proposed  by  Prof.  Fred  Harris  for  N,  giving
results consistent with other estimates for passband ripple values near 0.1 dB, is
(5-49)
where  Atten  is  the  filter’s  desired  stopband  attenuation  measured  in  dB,  and  fpass  and  fstop  are
frequencies normalized to the fs sample rate in Hz as illustrated in Figure 5-52. For example, fpass =
0.2 means that the continuous-time frequency of fpass is 0.2fs Hz.
Figure 5-52 Example FIR filter frequency definitions.
As an example, let’s obtain a rough estimate of the number of lowpass FIR filter taps (coefficients)
needed to achieve the magnitude response shown in Figure 5-52. Assuming fs = 1000 Hz, we want the
end of a lowpass filter’s passband to be at 250 Hz, the beginning of the stopband is 350 Hz, and we
need a stopband attenuation of 48 dB. Applying those values to Eq. (5-49), we have
(5-50)
Taking the integer closest to 21.8, i.e., 22, we then state that the lowpass filter in Figure 5-52 can be
built using a 22-tap FIR filter. We’ll use Eq. (5-49) many times in later chapters of this book.
References
[1] Shynk, J. J. “Adaptive IIR Filtering,” IEEE ASSP Magazine, April 1989.
[2]  Laundrie,  A.  “Adaptive  Filters  Enable  Systems  to  Track  Variations,”  Microwaves  &  RF,
September 1989.
[3] Bullock, S. R. “High Frequency Adaptive Filter,” Microwave Journal, September 1990.
[4] Haykin, S. S. Adaptive Filter Theory, Prentice Hall, Englewood Cliffs, New Jersey, 1986.
[5]  Goodwin,  G.  C.,  and  Sin,  K.  S.  Adaptive  Filtering  Prediction  and  Control,  Prentice  Hall,
Englewood Cliffs, New Jersey, 1984.
[6] Gibbs, J. W. Nature, Vol. 59, 1899, p. 606.
[7]  Stockham,  T.  G.  “High-Speed  Convolution  and  Correlation  with  Applications  to  Digital
Filtering,” Chapter 7 in Digital Processing of Signals, ed. by B. Gold et al., McGraw-Hill, New
York, 1969, pp. 203–232.
[8]  Wait,  J.  V.  “Digital  Filters,”  in  Active  Filters:  Lumped,  Distributed,  Integrated,  Digital,  and
Parametric, ed. by L. P. Huelsman, McGraw-Hill, New York, 1970, pp. 200–277.
[9]  Dolph,  C.  L.  “A  Current  Distribution  for  Broadside  Arrays  Which  Optimizes  the  Relationship
Between Beam Width and Side-Lobe Level,” Proceedings of the IRE, Vol. 35, June 1946.
[10]  Barbiere,  D.  “A  Method  for  Calculating  the  Current  Distribution  of  Chebyshev  Arrays,”
Proceedings of the IRE, Vol. 40, January 1952.
[11] Cook, C. E., and Bernfeld, M. Radar Signals, Academic Press, New York, 1967, pp. 178–180.
[12] Kaiser, J. F. “Digital Filters,” in System Analysis by Digital Computer, ed. by F. F. Kuo and J.
F. Kaiser, John Wiley and Sons, New York, 1966, pp. 218–277.
[13] Williams, C. S. Designing Digital Filters, Prentice Hall, Englewood Cliffs, New Jersey, 1986,
p. 117.
[14]  Harris,  F.  J.  “On  the  Use  of  Windows  for  Harmonic  Analysis  with  the  Discrete  Fourier
Transform,” Proceedings of the IEEE, Vol. 66, No. 1, January 1978.
[15] McClellan, J. H., Parks, T. W., and Rabiner, L. R. “A Computer Program for Designing Optimum
FIR Linear Phase Digital Filters,” IEEE Trans. on Audio and Electroacoustics, Vol. AU-21, No. 6,
December 1973, p. 515.
[16]  Rabiner,  L.  R.,  and  Gold,  B.  Theory  and  Application  of  Digital  Signal  Processing,  Prentice
Hall, Englewood Cliffs, New Jersey, 1975, p. 136.
[17] Parks, T. W., and McClellan, J. H. “Chebyshev Approximation for Nonrecursive Digital Filters
with Linear Phase,” IEEE Trans. on Circuit Theory, Vol. CT-19, March 1972.
[18] McClellan, J. H., and Parks, T. W. “A Unified Approach to the Design of Optimum FIR Linear
Phase Digital Filters,” IEEE Trans. on Circuit Theory, Vol. CT-20, November 1973.
[19] Rabiner, L. R., McClellan, J. H., and Parks, T. W. “FIR Digital Filter Design Techniques Using
Weighted Chebyshev Approximation,” Proceedings of the IEEE, Vol. 63, No. 4, April 1975.
[20]  Oppenheim,  A.  V.,  and  Schafer,  R.  W.  Discrete  Time  Signal  Processing,  Prentice  Hall,
Englewood Cliffs, New Jersey, 1989, p. 478.
[21] Funderburk, D. M., and Park, S. “Implementation of a C-QUAM AM-Stereo Receiver Using a
General Purpose DSP Device,” RF Design, June 1993.
[22] Harris Semiconductor Inc. “A Digital, 16-Bit, 52 Msps Halfband Filter,” Microwave Journal,
September 1993.
[23] Ballanger, M. G. “Computation Rate and Storage Estimation in Multirate Digital Filtering with
Half-Band  Filters,”  IEEE  Trans.  on  Acoust.  Speech,  and  Signal  Proc.,  Vol.  ASSP-25,  No.  4,
August 1977.
[24]  Crochiere,  R.  E.,  and  Rabiner,  L.  R.  “Decimation  and  Interpolation  of  Digital  Signals—A
Tutorial Review,” Proceedings of the IEEE, Vol. 69, No. 3, March 1981, p. 318.
[25] Ballanger, M. G., Daguet, J. L., and Lepagnol, G. P. “Interpolation, Extrapolation, and Reduction
of Computational Speed in Digital Filters,” IEEE Trans. on Acoust. Speech, and Signal Proc., Vol.
ASSP-22, No. 4, August 1974.
[26]  Oppenheim,  A.  V.,  Willsky,  A.  S.,  and  Young,  I.  T.  Signals  and  Systems,  Prentice  Hall,
Englewood Cliffs, New Jersey, 1983, p. 212.
[27] Stearns, S. Digital Signal Analysis, Hayden Book Co., Rochelle Park, New Jersey, 1975, p. 93.
[28]  Oppenheim,  A.  V.,  and  Schafer,  R.  W.  Discrete  Time  Signal  Processing,  Prentice  Hall,
Englewood Cliffs, New Jersey, 1989, p. 58.
[29]  Rabiner,  L.  R.,  and  Gold,  B.  Theory  and  Application  of  Digital  Signal  Processing,  Prentice
Hall, Englewood Cliffs, New Jersey, 1975, p. 59.
[30]  Oppenheim,  A.  V.,  Willsky,  A.  S.,  and  Young,  I.  T.  Signals  and  Systems,  Prentice  Hall,
Englewood Cliffs, New Jersey, 1983, p. 201.
[31] Rorabaugh, C. DSP Primer, McGraw-Hill, New York, 1999, pp. 278–279.
[32]  Kaiser,  J.  “Nonrecursive  Digital  Filter  Design  Using  Io-Sinh  Window  Function,”  Proc.  1974
IEEE Int. Symp. Circuits Systems, April 1974, pp. 20–23.
[33]  Harris,  F.  Multirate  Signal  Processing  for  Communication  Systems,  Prentice  Hall,  Upper
Saddle River, New Jersey, 2004, pp. 56–57.
Chapter 5 Problems
5.1  We  first  introduced  the  notion  of  impulse  response  in  Chapter  1,  and  here  in  Chapter  5  we
discussed the importance of knowing the impulse response of FIR filter networks. With that said, if
the y(n) output of a discrete system is equal to the system’s x(n) input sequence:
(a) Draw the unit impulse response of such a system.
(b) Draw the block diagram (structure) of that system.
(c) What is the frequency magnitude response of such a system? Prove your answer.
5.2 Consider a simple analog signal defined by x(t) = cos(2π800t) shown in Figure P5-2. The FIR
lowpass  filter  has  a  passband  extending  from  −400  Hz  to  +400  Hz,  a  passband  gain  of  unity,  a
transition region width of 20 Hz, and a stopband attenuation of 60 dB.
(a) Draw the spectral magnitude of x(n) showing all spectral components in the range of −2fs to
+2fs.
(b) Draw the spectral magnitude of y(n) showing all spectral components in the range of −2fs to
+2fs.
(c) What is the time-domain peak amplitude of the sinusoidal y(n) output?
Figure P5-2
5.3 Assume we want to filter the audio signal from a digital video disc (DVD) player as shown in
Figure P5-3.  The  filtered  audio  signal  drives,  by  way  of  a  digital-to-analog  (D/A)  converter,  a
speaker. For the audio signal to have acceptable time synchronization with the video signal, video
engineers have determined that the time delay of the filter must be no greater than 6×10−3 seconds.
If the fs sample rate of the audio is 48 kHz, what is the maximum number of taps in the FIR filter
that will satisfy the time delay restriction? (Assume a linear-phase FIR filter, and zero time delay
through the D/A converter.)
Figure P5-3
5.4  There  are  times  when  we  want  to  build  a  lowpass  filter  and  a  highpass  filter  that  are
complementary.  By  “complementary”  we  mean  that  a  highpass  filter’s  passband  covers  the
frequency range defined by a lowpass filter’s stopband range. This idea is illustrated in Figure P5-
4(a).  An  example  of  such  filters  is  an  audio  system,  shown  in  Figure  P5-4(b),  where  the  low-
frequency spectral components of an x(n) audio signal drive, by way of a digital-to-analog (D/A)
converter, a low-frequency speaker (woofer). Likewise, the high-frequency spectral components of
x(n) drive a high-frequency speaker (tweeter). Audio enthusiasts call Figure P5-4(b) a “crossover”
network. Assuming that the lowpass filter is implemented with a 15-tap FIR filter whose hLow(k)
coefficients  are  those  in  Figure  P5-4(c),  the  complementary  highpass  filter  will  have  the
coefficients shown in Figure P5-4(d). Highpass coefficients hHigh(k) are defined by
Figure P5-4
Here is the problem: Draw a block diagram of a system that performs the process in P5-4(b) where
only the hLow(k) lowpass FIR filter need be implemented.
5.5 Think about a discrete System A, shown in Figure P5-5, that has an undesirable amplitude (gain)
loss by a factor 0.5 (−6 dB), whose output requires lowpass linear-phase filtering. What can we do
in the design of the lowpass FIR filter so the filter has an amplitude gain of 2 to compensate for
System A’s amplitude loss?
Figure P5-5
5.6  Let’s  assume  we  have  an  x(n)  time  sequence,  whose  fs  sample  rate  is  20  kHz,  and  its  |X(f)|
spectral  magnitude  is  that  shown  in  Figure  P5-6(a).  We  are  required  to  design  a  linear-phase
lowpass FIR filter that will attenuate the undesired high-frequency noise indicated in Figure  P5-
6(a). So we design a lowpass FIR filter whose frequency magnitude response is the |H(f)| shown in
Figure P5-6(b)  and  assume  our  filter  design  exercise  is  complete.  Sometime  later,  unfortunately,
we learn that the original x(n) sequence’s sample rate was not 20 kHz, but is in fact 40 kHz.
Figure P5-6
Here is the problem: What must we do to our lowpass filter’s h(k) coefficients, originally designed
based  on  a  20  kHz  sample  rate,  so  that  they  will  still  attenuate  x(n)’s  undesired  high-frequency
noise when the fs sample rate is actually 40 kHz?
5.7 Here is an interesting little problem. Think about applying the sinusoidal input sequence shown in
Figure  P5-7(a)  to  an  8-point  moving  average  FIR  filter.  The  filter’s  output  sequence  is  that
depicted in Figure P5-7(b).
Figure P5-7
(a) What characteristic of the filter’s frequency response causes the filter’s output sequence to go
to all zeros as shown in Figure P5-7(b)?
(b) In Figure P5-7(b), what do we call those initial nonzero-valued filter output samples?
5.8 Are abrupt (sudden) changes in the amplitude of a continuous, or discrete, signal associated with
low or high frequencies?
5.9 Consider an FIR filter whose impulse response is shown in Figure P5-9(a). Given the x(n) filter
input sequence shown in Figure P5-9(b):
(a) What is the length, measured in samples, of the nonzero-valued samples of the filter’s output
sequence?
(b) What is the maximum sample value of the filter’s output sequence?
Figure P5-9
5.10 Consider an FIR filter whose impulse response is that shown in Figure P5-10(a). Given the x(n)
filter input sequence shown in Figure P5-10(b), draw the filter’s output sequence.
Figure P5-10
5.11 Regarding the material in this chapter, it’s educational to revisit the idea of periodic sampling
that  was  presented  in  Chapter 2.  Think  about  a  continuous  x(t)  signal  in  Figure  P5-11(a)  whose
spectrum is depicted in Figure P5-11(b). Also, consider the continuous periodic infinitely narrow
impulses,  s(t),  shown  in  Figure  P5-11(c).  Reference  [28]  provides  the  algebraic  acrobatics  to
show that the spectrum of s(t) is the continuous infinitely narrow impulses, S(f), shown in Figure
P5-11(d). If we multiply the x(t) signal by the s(t) impulses, we obtain the continuous y(t) = s(t)x(t)
impulse signal shown by the arrows in Figure P5-11(e).
Figure P5-11
Now, if we use an analog-to-digital converter to represent those y(t) impulse values as a sequence
of discrete samples, we obtain the y(n) sequence shown in Figure P5-11(f). Here is the problem:
Briefly  discuss  what  we  learned  in  this  Chapter 5  that  tells  us  the  spectrum  of  the  y(n)  samples
comprises  periodic  replications  of  the  X(f)  in  Figure  P5-11(b).  Your  brief  discussion  should
confirm  the  material  in  Chapter  2  which  stated  that  discrete-time  sequences  have  periodic
(replicated) spectra.
5.12 Now that we’re familiar with the powerful convolution theorem, think about the discrete system
shown in Figure P5-12(a).
Figure P5-12
Given that x(n)’s spectrum is the X(m) shown in Figure P5-12(b):
(a)  Draw  the  Y(m)  spectrum  of  sequence  y(n).  (We’re  not  worried  about  the  vertical  axis  scale
here, merely the frequency axis and spectral shape of Y(m).)
(b) Will aliasing errors occur in the y(n) = x(n)2 output? (That is, will spectral replications in Y(m)
overlap each other?)
(c) What is x(n)’s maximum one-sided bandwidth that will avoid aliasing errors in y(n)? (Stated in
different words, what is the maximum one-sided bandwidth of x(n) that will avoid overlapped
spectral replications in Y(m)?)
5.13 It’s likely that you have heard of the process called linear interpolation. It’s a computationally
simple (but not terribly accurate) scheme for estimating sample values of a continuous function in
between some given x(n) sample values of that function. For the x(n) time samples in Figure P5-
13(a), linear interpolation is the process of computing the intermediate y(n) samples shown as the
black squares in Figure P5-13(b).  That  is,  the  interpolated  sample  y(1)  is  the  value  lying  on  the
center of the straight line connecting x(0) and x(1), the interpolated sample y(2) is the value lying
on the center of the straight line connecting x(1) and x(2), and so on. Given this process of linear
interpolation:
(a) What is the equation defining y(n) in terms of the x(n) samples?
(b)  The  implementation  of  linear  interpolation  is  often  called  a  filter  because  we  build
interpolators using tapped-delay line structures, just like standard FIR filter structures. Draw the
block diagram of a linear interpolation filter that computes y(n) from the input x(n) sequence.
Figure P5-13
5.14 Consider a linear-phase lowpass FIR filter whose coefficients are
h1(k) = [−0.8, 1.6, 25.5, 47, 25.5, 1.6, −0.8],
and whose DC gain, H1(0), is equal to 99.6. If we change those coefficients to
h2(k) = [−0.8, 1.6, Q, 47, Q, 1.6, −0.8],
we obtain a new DC gain equal to 103.6. What is the value of Q?
5.15 Figure P5-15 shows a linear-phase 5-tap FIR filter.
Figure P5-15
DSP engineers always seek to reduce the number of multipliers in their systems. Redesign the filter
in Figure P5-15 to a form that reduces the number of necessary multiplications per output sample.
Draw the block diagram of your new design.
Hint:  Write  the  difference  equation  for  the  y(n)  output  sequence,  and  recall  the  relationships
between the filter’s coefficients.
5.16 The two linear-phase lowpass filters in Figure P5-16 have very similar frequency responses, but
those responses are not identical except at a single frequency. If we replaced Filter h1 with Filter
h2  to  reduce  our  filtering  computational  workload,  determine  the  frequency,  ωo,  where  the  two
H1(ω) and H2(ω) frequency responses are equal.
Hint: Begin by creating closed-form equations for H1(ω) and H2(ω) using the discrete-time Fourier
transform (DTFT).
Figure P5-16
5.17 The following is a useful problem regarding the 3-tap nonrecursive FIR filter shown in Figure
P5-17(a). The problem’s solution shows us how to design computationally efficient narrowband-
noise  reduction  filters.  If  |h1|  ≤  2,  the  filter  will  have  an  |H(ω)|  frequency  magnitude  response
having  two  nulls  at  ±ωn  as  shown  in  Figure  P5-17(b).  (Here,  the  frequency  axis  value  of  π
radians/sample corresponds to a cyclic frequency of half the sample rate, fs/2.)
(a)  Assume  we  have  a  low-frequency  signal  of  interest  that’s  contaminated  with  high-level
narrowband  noise  located  at  ±3.35  MHz  when  the  sample  rate  is  fs  =  8.25  MHz  as  shown  in
Figure P5-17(c). To attenuate that noise, for what value of h1 will the 3-tap FIR filter’s nulls be
located at the noise center frequency of ±3.35 MHz? Show your work.
Hint: Use the discrete-time Fourier transform (DTFT) of the filter’s impulse response to create a
closed-form equation for the filter’s H(ω) frequency response in terms of the coefficient h1 and
frequency ω. Next, obtain the expression for h1 in terms of the filter’s null frequency ωn.
(b) What is the DC gain (gain at zero Hz) of our 3-tap FIR filter?
(c) Explain why the filter has a linear, or nonlinear, phase response.
Figure P5-17
5.18  What  characteristic  must  the  coefficients  of  an  FIR  filter  have  to  ensure  that  its  frequency-
domain phase response is a linear function of frequency (i.e., linear phase)?
5.19 Quickfilter Technologies Inc. produces a tapped-delay line FIR filter chip (Part #QF1D512) that
has an astounding N = 512 taps. When a new filter input sample is applied to the chip, how many
addition operations must this chip perform to compute a single filter output sample?
5.20  Intersil  Corp.  produces  an  HSP5021  down-converter  integrated  circuit  containing  a
symmetrical-coefficient  FIR  filter  having  255  taps.  If  the  down-converter  chip’s  input  signal
sample rate is fs = 8 MHz, what is the group delay (delay through the filter) of their 255-tap FIR
filter measured in samples?
5.21 Assume we have digitized an analog signal at an fs sample rate of 2×106 samples/second. Next
we pass the samples through a 70-tap linear-phase lowpass FIR filter whose cutoff frequency (end
of  the  passband)  is  600  kHz.  What  would  be  the  time  delay,  measured  in  seconds,  between  the
lowpass filter’s input and output for a sinusoidal tone whose frequency is 200 kHz?
5.22  Think  about  two  linear-phase  FIR  filters  whose  frequency  magnitude  responses  are  shown  in
Figure P5-22.
Figure P5-22
(a) Let’s assume that filter H1(f) is a 17-tap FIR filter. What is the group delay of this linear-phase
filter measured in samples?
(b) Next, let’s assume that filter H2(f) is a 17-tap half-band FIR filter. H2(f), like all half-band FIR
filters, has a gain of 0.5 at the frequency f = fs/4.  What  is  the  group  delay  of  this  linear-phase
H2(f) filter, measured in samples?
5.23  Reverberation,  a  kind  of  echo,  is  a  popular  audio  effect  applied  to  guitar  music.  (Most
commercial electric guitar amplifiers have a reverberation capability.) In the world of continuous
signals reverberation is implemented with an analog delay line as shown in Figure P5-23(a). That
analog delay line is typically a kind of speaker at one end of a coiled metal spring, and a kind of
microphone at the other end of the spring. However, analog reverberation units have no convenient
way to control the amount of time delay, and unfortunately their hardware is physically large.
Figure P5-23
Making use of digital signal processing on the other hand, the process of reverberation seems easy
to  implement  using  a  delay  line  network  like  that  shown  in  Figure  P5-23(b).  For  the  digital
reverberation process to be usable, however, it must have a constant gain, where
over the full operating frequency range of the system. That is, we want our reverberator to have a
flat frequency magnitude response. (By “Gain” we mean the steady-state gain after the delay line is
filled with input samples.)
(a) Assume we have the Figure P5-23(b) delay line with N = 8 delay elements. What is the N = 8
digital reverberator’s h(n) time-domain impulse response?
(b) What is the equation for the digital reverberator’s |H(ω)| frequency magnitude response?
Hint: Use what you learned in Section 3.14, and don’t forget your trigonometric identities.
(c)  Draw  a  rough  sketch  of  the  |H(ω)|  frequency  magnitude  response  from  Part  (b).  (This  curve
shows us how well simple digital delay-line reverberators work.)
5.24 There are digital filtering schemes that use the process conceptually shown in Figure P5-24(a).
In that parallel-path filter the x(n) input is filtered to generate sequence w(n). The network’s y(n)
output is the x(n) input sequence minus the w(n) sequence. The w(n) sequence is defined by
w(n) = x(n) + 2x(n−2) + x(n−4).
Figure P5-24
The  actual  implementation  of  such  a  parallel-path  filter  is  shown  in  Figure  P5-24(b)  where  the
multi-element  delay  line  in  the  upper  path  of  Figure  P5-24(b)  is  needed  for  time  alignment  to
compensate for the time delay of the bottom-path FIR filter. How many unit-delay elements must be
used in the upper path in Figure P5-24(b)?
5.25 As we stated in Section 5.10, a lowpass FIR filter’s frequency magnitude response at zero Hz
(DC) is equal to the sum of the filter’s impulse response samples (sum of the filter’s coefficients).
Prove this important lowpass FIR filter property.
5.26 Although we didn’t state it explicitly in the text, the continuous frequency magnitude response of
a symmetrical 7-tap FIR filter (for example, an FIR filter whose h(k) coefficients are indexed as
shown in Figure P5-26) can be computed using
Figure P5-26
(The normalized frequency range is −π ≤ ω ≤ π where ω is a continuous normalized angle with ω =
π corresponding to a cyclic frequency of fs/2 Hz.) There are two reasons we introduce the above
|H(ω)| expression:
•  Such  |H(ω)|  equations  can  be  used  to  compute  the  magnitude  responses  of  linear-phase  FIR
filters, having an odd number of taps, when no FFT software routine is available.
• You won’t be surprised when you see, in the literature of DSP, FIR filter frequency magnitude
response equations such as the above summation of cosine functions.
Derive  the  general  equation  for  the  |H(ω)|  for  an  N-tap  symmetrical  FIR  filter’s  magnitude
response, when N is odd. Show your work.
Hint: Use what you learned in Section 3.14, and pay careful attention to the range of the k index in
Figure P5-26.  Notice  how  h(0)  is  the  center  coefficient!  Also,  don’t  forget  our  friend  Leonhard
Euler.
5.27 Assume a commercial data acquisition device has the ability to implement a 191-tap digital FIR
filter.  What  is  the  narrowest  transition  region  width  (fstop  −  fpass),  stated  in  terms  of  fs,  we  can
expect to achieve for a lowpass FIR filter using this device if we desire at least 55 dB of stopband
attenuation?
5.28 Texas Instruments Inc. produces a video processing chip (Part #TMS320DM646x) containing the
FIR  filter  shown  in  Figure  P5-28.  Coefficient  b,  defined  by  the  user,  controls  the  frequency
magnitude response of the filter.
(a) What is the time-domain difference equation for the filter?
(b) Does the filter have a linear-phase frequency response? Justify your answer.
(c) What is the group delay of the filter measured in samples?
Figure P5-28
5.29  Here  is  a  fun  problem  proving  that  you  have  actually  been  performing  convolutions  since  you
were a child. Show how the multiplication (computing the product) of the two numbers 24 and 13
can be performed by convolving their digits.

Chapter Six. Infinite Impulse Response Filters
Infinite  impulse  response  (IIR)  digital  filters  are  fundamentally  different  from  FIR  filters  because
practical IIR filters always require feedback. Where FIR filter output samples depend only on past
input samples, each IIR filter output sample depends on previous input samples and previous filter
output samples. IIR filters’ memory of past outputs is both a blessing and a curse. As in all feedback
systems, perturbations at the IIR filter input could, depending on the design, cause the filter output to
become unstable and oscillate indefinitely. This characteristic of possibly having an infinite duration
of  nonzero  output  samples,  even  if  the  input  becomes  all  zeros,  is  the  origin  of  the  phrase  infinite
impulse response.  It’s  interesting  at  this  point  to  know  that,  relative  to  FIR  filters,  IIR  filters  have
more  complicated  structures  (block  diagrams),  are  harder  to  design  and  analyze,  and  do  not  have
linear phase responses. Why in the world, then, would anyone use an IIR filter? Because they are very
efficient.  IIR  filters  require  far  fewer  multiplications  per  filter  output  sample  to  achieve  a  given
frequency  magnitude  response.  From  a  hardware  standpoint,  this  means  that  IIR  filters  can  be  very
fast,  allowing  us  to  build  real-time  IIR  filters  that  operate  over  much  higher  sample  rates  than  FIR
filters.†
† At the end of this chapter, we briefly compare the advantages and disadvantages of IIR filters relative to FIR filters.
To illustrate the utility of IIR filters, Figure 6-1 contrasts the frequency magnitude responses of what’s
called  a  4th-order  lowpass  IIR  filter  and  the  19-tap  FIR  filter  of  Figure  5-19(b)  from  Chapter  5.
Where the 19-tap FIR filter in Figure 6-1 requires 19 multiplications per filter output sample, the 4th-
order  IIR  filter  requires  only  9  multiplications  for  each  filter  output  sample.  Not  only  does  the  IIR
filter give us reduced passband ripple and a sharper filter roll-off, it does so with less than half the
multiplication workload of the FIR filter.
Figure 6-1 Comparison of the frequency magnitude responses of a 19-tap lowpass FIR filter and a
4th-order lowpass IIR filter.
Recall from Section 5.3 that to force an FIR filter’s frequency response to have very steep transition
regions,  we  had  to  design  an  FIR  filter  with  a  very  long  impulse  response.  The  longer  the  impulse
response, the more ideal our filter frequency response will become. From a hardware standpoint, the
maximum number of FIR filter taps we can have (the length of the impulse response) depends on how
fast  our  hardware  can  perform  the  required  number  of  multiplications  and  additions  to  get  a  filter
output value before the next filter input sample arrives. IIR filters, however, can be designed to have
impulse responses that are longer than their number of taps! Thus, IIR filters can give us much better
filtering for a given number of multiplications per output sample than FIR filters. With this in mind,
let’s take a deep breath, flex our mathematical muscles, and learn about IIR filters.
6.1 An Introduction to Infinite Impulse Response Filters
Given a finite duration of nonzero input values, an IIR filter will have an infinite duration of nonzero
output  samples.  So,  if  the  IIR  filter’s  input  suddenly  becomes  a  sequence  of  all  zeros,  the  filter’s
output could conceivably remain nonzero forever. This peculiar attribute of IIR filters comes about
because of the way they’re realized, i.e., the feedback structure of their delay units, multipliers, and
adders.  Understanding  IIR  filter  structures  is  straightforward  if  we  start  by  recalling  the  building
blocks of an FIR filter. Figure 6-2(a) shows the now familiar structure of a 4-tap FIR digital filter that
implements the time-domain FIR equation
(6-1)
Figure 6-2 FIR digital filter structures: (a) traditional FIR filter structure; (b) rearranged, but
equivalent, FIR filter structure.
Although  not  specifically  called  out  as  such  in  Chapter  5,  Eq.  (6-1)  is  known  as  a  difference
equation.  To  appreciate  how  past  filter  output  samples  are  used  in  the  structure  of  IIR  filters,  let’s
begin  by  reorienting  our  FIR  structure  in  Figure  6-2(a)  to  that  of  Figure  6-2(b).  Notice  how  the
structures in Figure 6-2 are computationally identical, and both are implementations, or realizations,
of Eq. (6-1).
We can now show how past filter output samples are combined with past input samples by using the
IIR  filter  structure  in  Figure  6-3.  Because  IIR  filters  have  two  sets  of  coefficients,  we’ll  use  the
standard notation of the variables b(k) to denote the feedforward coefficients and the variables a(k)
to  indicate  the  feedback  coefficients  in  Figure 6-3.  OK,  the  difference  equation  describing  the  IIR
filter in Figure 6-3 is
Figure 6-3 IIR digital filter structure showing feedforward and feedback calculations.
(6-2)
Look at Figure 6-3 and Eq. (6-2) carefully. It’s important to convince ourselves that Figure 6-3 really
is  a  valid  implementation  of  Eq.  (6-2)  and  that,  conversely,  difference  equation  Eq.  (6-2)  fully
describes the IIR filter structure in Figure 6-3. Keep in mind, now, that the sequence y(n) in Figure 6-
3 is not the same y(n) sequence that’s shown in Figure 6-2. The d(n) sequence in Figure 6-3 is equal
to the y(n) sequence in Figure 6-2.
By  now  you’re  probably  wondering,  “Just  how  do  we  determine  those  a(k)  and  b(k)  IIR  filter
coefficients  if  we  actually  want  to  design  an  IIR  filter?”  Well,  fasten  your  seat  belt  because  this  is
where  we  get  serious  about  understanding  IIR  filters.  Recall  from  the  last  chapter  concerning  the
window method of lowpass FIR filter design that we defined the frequency response of our desired
FIR  filter,  took  the  inverse  Fourier  transform  of  that  frequency  response,  and  then  shifted  that
transform  result  to  get  the  filter’s  time-domain  impulse  response.  Happily,  due  to  the  nature  of
transversal FIR filters, the desired h(k) filter coefficients turned out to be exactly equal to the impulse
response  sequence.  Following  that  same  procedure  with  IIR  filters,  we  could  define  the  desired
frequency  response  of  our  IIR  filter  and  then  take  the  inverse  Fourier  transform  of  that  response  to
yield  the  filter’s  time-domain  impulse  response.  The  bad  news  is  that  there’s  no  direct  method  for
computing the IIR filter’s a(k) and b(k) coefficients from the impulse response! Unfortunately, the FIR
filter  design  techniques  that  we’ve  learned  so  far  simply  cannot  be  used  to  design  IIR  filters.
Fortunately  for  us,  this  wrinkle  can  be  ironed  out  by  using  one  of  several  available  methods  of
designing IIR filters.
Standard  IIR  filter  design  techniques  fall  into  three  basic  classes:  the  impulse  invariance,  bilinear
transform,  and  optimization  methods.  These  design  methods  use  a  discrete  sequence,  mathematical
transformation process known as the z-transform whose origin is the Laplace transform traditionally
used in the analyzing of continuous systems. With that in mind, let’s start this IIR filter analysis and
design discussion by briefly reacquainting ourselves with the fundamentals of the Laplace transform.
6.2 The Laplace Transform
The  Laplace  transform  is  a  mathematical  method  of  solving  linear  differential  equations  that  has
proved  very  useful  in  the  fields  of  engineering  and  physics.  This  transform  technique,  as  it’s  used
today, originated from the work of the brilliant English physicist Oliver Heaviside.† The fundamental
process of using the Laplace transform goes something like the following:
†  Heaviside  (1850–1925),  who  was  interested  in  electrical  phenomena,  developed  an  efficient  algebraic  process  of  solving  differential
equations.  He  initially  took  a  lot  of  heat  from  his  contemporaries  because  they  thought  his  work  was  not  sufficiently  justified  from  a
mathematical  standpoint.  However,  the  discovered  correlation  of  Heaviside’s  methods  with  the  rigorous  mathematical  treatment  of  the
French  mathematician  Marquis  Pierre  Simon  de  Laplace’s  (1749–1827)  operational  calculus  verified  the  validity  of  Heaviside’s
techniques.
Step 1: A time-domain differential equation is written that describes the input/output relationship of a
physical system (and we want to find the output function that satisfies that equation with a given
input).
Step 2: The differential equation is Laplace transformed, converting it to an algebraic equation.
Step 3: Standard algebraic techniques are used to determine the desired output function’s equation in
the Laplace domain.
Step  4:  The  desired  Laplace  output  equation  is,  then,  inverse  Laplace  transformed  to  yield  the
desired time-domain output function’s equation.
This procedure, at first, seems cumbersome because it forces us to go the long way around, instead
of just solving a differential equation directly. The justification for using the Laplace transform is that
although solving differential equations by classical methods is a very powerful analysis technique for
all  but  the  most  simple  systems,  it  can  be  tedious  and  (for  some  of  us)  error  prone.  The  reduced
complexity  of  using  algebra  outweighs  the  extra  effort  needed  to  perform  the  required  forward  and
inverse  Laplace  transformations.  This  is  especially  true  now  that  tables  of  forward  and  inverse
Laplace  transforms  exist  for  most  of  the  commonly  encountered  time  functions.  Well-known
properties of the Laplace transform also allow practitioners to decompose complicated time functions
into combinations of simpler functions and, then, use the tables. (Tables of Laplace transforms allow
us to translate quickly back and forth between a time function and its Laplace transform—analogous
to, say, a German-English dictionary if we were studying the German language.†) Let’s briefly look at
a  few  of  the  more  important  characteristics  of  the  Laplace  transform  that  will  prove  useful  as  we
make our way toward the discrete z-transform used to design and analyze IIR digital filters.
†  Although  tables  of  commonly  encountered  Laplace  transforms  are  included  in  almost  every  system  analysis  textbook,  very
comprehensive tables are also available[1–3].
The  Laplace  transform  of  a  continuous  time-domain  function  f(t),  where  f(t)  is  defined  only  for
positive time (t > 0), is expressed mathematically as
(6-3)
F(s) is called “the Laplace transform of f(t),” and the variable s is the complex number
(6-4)
A  more  general  expression  for  the  Laplace  transform,  called  the  bilateral or two-sided  transform,
uses negative infinity (−∞) as the lower limit of integration. However, for the systems that we’ll be
interested in, where system conditions for negative time (t  <  0)  are  not  needed  in  our  analysis,  the
one-sided Eq. (6-3)  applies.  Those  systems,  often  referred  to  as  causal  systems,  may  have  initial
conditions  at  t  =  0  that  must  be  taken  into  account  (velocity  of  a  mass,  charge  on  a  capacitor,
temperature of a body, etc.), but we don’t need to know what the system was doing prior to t = 0.
In Eq. (6-4), σ is a real number and ω is frequency in radians/second. Because e−st is dimensionless,
the exponent term s must have the dimension of 1/time, or frequency. That’s why the Laplace variable
s is often called a complex frequency.
To put Eq. (6-3) into words, we can say that it requires us to multiply, point for point, the function f(t)
by the complex function e−st for a given value of s. (We’ll soon see that using the function e−st here is
not  accidental;  e−st  is  used  because  it’s  the  general  form  for  the  solution  of  linear  differential
equations.) After the point-for-point multiplications, we find the area under the curve of the function
f(t)e−st by summing all the products. That area, a complex number, represents the value of the Laplace
transform for the particular value of s = σ + jω chosen for the original multiplications. If we were to
go through this process for all values of s, we’d have a full description of F(s) for every value of s.
I  like  to  think  of  the  Laplace  transform  as  a  continuous  function,  where  the  complex  value  of  that
function for a particular value of s is a correlation of f(t) and a damped complex e−st sinusoid whose
frequency is ω and whose damping factor is σ. What do these complex sinusoids look like? Well, they
are rotating phasors described by
(6-5)
From  our  knowledge  of  complex  numbers,  we  know  that  e−jωt  is  a  unity-magnitude  phasor  rotating
clockwise around the origin of a complex plane at a frequency of ω radians/second. The denominator
of Eq. (6-5) is a real number whose value is one at time t = 0. As t increases, the denominator eσt gets
larger  (when  σ  is  positive),  and  the  complex  e−st  phasor’s  magnitude  gets  smaller  as  the  phasor
rotates on the complex plane. The tip of that phasor traces out a curve spiraling in toward the origin
of the complex plane. One way to visualize a complex sinusoid is to consider its real and imaginary
parts individually. We do this by expressing the complex e−st sinusoid from Eq. (6-5) in rectangular
form as
(6-5′)
Figure 6-4 shows the real parts (cosine) of several complex sinusoids with different frequencies and
different damping factors. In Figure 6-4(a), the complex sinusoid’s frequency is the arbitrary ω′, and
the  damping  factor  is  the  arbitrary  σ′.  So  the  real  part  of  F(s),  at  s  =  σ′  +  jω′,  is  equal  to  the
correlation of f(t) and the wave in Figure 6-4(a). For different values of s, we’ll correlate f(t) with
different complex sinusoids as shown in Figure 6-4. (As we’ll see, this correlation is very much like
the  correlation  of  f(t)  with  various  sine  and  cosine  waves  when  we  were  calculating  the  discrete
Fourier transform.) Again, the real part of F(s), for a particular value of s, is the correlation of f(t)
with a cosine wave of frequency ω and a damping factor of σ, and the imaginary part of F(s) is the
correlation of f(t) with a sinewave of frequency ω and a damping factor of σ.
Figure 6-4 Real part (cosine) of various e−st functions, where s = σ + jω, to be correlated with f(t).
Now, if we associate each of the different values of the complex s variable with a point on a complex
plane, rightfully called the s-plane,  we  could  plot  the  real  part  of  the  F(s)  correlation  as  a  surface
above (or below) that s-plane and generate a second plot of the imaginary part of the F(s) correlation
as  a  surface  above  (or  below)  the  s-plane.  We  can’t  plot  the  full  complex  F(s)  surface  on  paper
because that would require four dimensions. That’s because s is complex, requiring two dimensions,
and F(s) is itself complex and also requires two dimensions. What we can do, however, is graph the
magnitude |F(s)| as a function of s because this graph requires only three dimensions. Let’s do that as
we demonstrate this notion of an |F(s)| surface by illustrating the Laplace transform in a tangible way.
Say, for example, that we have the linear system shown in Figure 6-5. Also, let’s assume that we can
relate the x(t) input and the y(t) output of the linear time-invariant physical system in Figure 6-5 with
the following messy homogeneous constant-coefficient differential equation:
Figure 6-5 System described by Eq. (6-6). The system’s input and output are the continuous-time
functions x(t) and y(t) respectively.
(6-6)
We’ll  use  the  Laplace  transform  toward  our  goal  of  figuring  out  how  the  system  will  behave  when
various types of input functions are applied, i.e., what the y(t) output will be for any given x(t) input.
Let’s slow down here and see exactly what Figure 6-5 and Eq. (6-6) are telling us. First, if the system
is time invariant, then the an and bn coefficients in Eq. (6-6) are constant. They may be positive or
negative,  zero,  real  or  complex,  but  they  do  not  change  with  time.  If  the  system  is  electrical,  the
coefficients might be related to capacitance, inductance, and resistance. If the system is mechanical
with  masses  and  springs,  the  coefficients  could  be  related  to  mass,  coefficient  of  damping,  and
coefficient  of  resilience.  Then,  again,  if  the  system  is  thermal  with  masses  and  insulators,  the
coefficients would be related to thermal capacity and thermal conductance. To keep this discussion
general, though, we don’t really care what the coefficients actually represent.
OK, Eq. (6-6) also indicates that, ignoring the coefficients for the moment, the sum of the y(t) output
plus derivatives of that output is equal to the sum of the x(t) input plus the derivative of that input. Our
problem is to determine exactly what input and output functions satisfy the elaborate relationship in
Eq. (6-6).  (For  the  stout-hearted,  classical  methods  of  solving  differential  equations  could  be  used
here,  but  the  Laplace  transform  makes  the  problem  much  simpler  for  our  purposes.)  Thanks  to
Laplace, the complex exponential time function of est is the one we’ll use. It has the beautiful property
that it can be differentiated any number of times without destroying its original form. That is,
(6-7)
If we let x(t) and y(t) be functions of est, x(est) and y(est), and use the properties shown in Eq. (6-7),
Eq. (6-6) becomes
(6-8)
Although it’s simpler than Eq. (6-6), we can further simplify the relationship in the last line in Eq. (6-
8) by considering the ratio of y(est) over x(est) as the Laplace transfer function of our system in Figure
6-5. If we call that ratio of polynomials the transfer function H(s),
(6-9)
To indicate that the original x(t) and y(t) have the identical functional form of est, we can follow the
standard Laplace notation of capital letters and show the transfer function as
where the output Y(s) is given by
(6-10)
(6-11)
Equation (6-11) leads us to redraw the original system diagram in a form that highlights the definition
of the transfer function H(s) as shown in Figure 6-6.
Figure 6-6 Linear system described by Eqs. (6-10) and (6-11). The system’s input is the Laplace
function X(s), its output is the Laplace function Y(s), and the system transfer function is H(s).
The cautious reader may be wondering, “Is it really valid to use this Laplace analysis technique when
it’s strictly based on the system’s x(t) input being some function of est, or x(est)?” The answer is that
the Laplace analysis technique, based on the complex exponential x(est), is valid because all practical
x(t) input functions can be represented with complex exponentials, for example,
• a constant, c = ce0t,
• sinusoids, sin(ωt) = (ejωt − e−jωt)/2j or cos(ωt) = (ejωt + e−jωt)/2,
• a monotonic exponential, eat, and
• an exponentially varying sinusoid, e−at cos(ωt).
With that said, if we know a system’s transfer function H(s), we can take the Laplace transform of any
x(t) input to determine X(s), multiply that X(s) by H(s) to get Y(s), and then inverse Laplace transform
Y(s)  to  yield  the  time-domain  expression  for  the  output  y(t).  In  practical  situations,  however,  we
usually don’t go through all those analytical steps because it’s the system’s transfer function H(s) in
which we’re most interested. Being able to express H(s) mathematically or graph the surface |H(s)| as
a function of s will tell us the two most important properties we need to know about the system under
analysis: is the system stable, and if so, what is its frequency response?
“But wait a minute,” you say. “Equations (6-10) and (6-11) indicate that we have to know the Y(s)
output  before  we  can  determine  H(s)!”  Not  really.  All  we  really  need  to  know  is  the  time-domain
differential  equation  like  that  in  Eq. (6-6).  Next  we  take  the  Laplace  transform  of  that  differential
equation and rearrange the terms to get the H(s) ratio in the form of Eq. (6-10). With practice, systems
designers can look at a diagram (block, circuit, mechanical, whatever) of their system and promptly
write the Laplace expression for H(s). Let’s use the concept of the Laplace transfer function H(s) to
determine the stability and frequency response of simple continuous systems.
6.2.1 Poles and Zeros on the s-Plane and Stability
One of the most important characteristics of any system involves the concept of stability. We can think
of a system as stable if, given any bounded input, the output will always be bounded. This sounds like
an easy condition to achieve because most systems we encounter in our daily lives are indeed stable.
Nevertheless,  we  have  all  experienced  instability  in  a  system  containing  feedback.  Recall  the
annoying howl when a public address system’s microphone is placed too close to the loudspeaker. A
sensational  example  of  an  unstable  system  occurred  in  western  Washington  when  the  first  Tacoma
Narrows Bridge began oscillating on the afternoon of November 7, 1940. Those oscillations, caused
by 42 mph winds, grew in amplitude until the bridge destroyed itself. For IIR digital filters with their
built-in feedback, instability would result in a filter output that’s not at all representative of the filter
input; that is, our filter output samples would not be a filtered version of the input; they’d be some
strange oscillating or pseudo-random values—a situation we’d like to avoid if we can, right? Let’s
see how.
We  can  determine  a  continuous  system’s  stability  by  examining  several  different  examples  of  H(s)
transfer functions associated with linear time-invariant systems. Assume that we have a system whose
Laplace transfer function is of the form of Eq. (6-10), the coefficients are all real, and the coefficients
b1 and a2 are equal to zero. We’ll call that Laplace transfer function H1(s), where
(6-12)
Notice that if s = −a0/a1, the denominator in Eq. (6-12) equals zero and H1(s) would have an infinite
magnitude. This s = −a0/a1 point on the s-plane is called a pole, and that pole’s location is shown by
the “x” in Figure 6-7(a). Notice that the pole is located exactly on the negative portion of the real σ
axis. If the system described by H1 were at rest and we disturbed it with an impulse like x(t) input at
time t = 0, its continuous time-domain y(t) output would be the damped exponential curve shown in
Figure 6-7(b). We can see that H1(s) is stable because its y(t) output approaches zero as time passes.
By the way, the distance of the pole from the σ = 0 axis, a0/a1 for our H1(s), gives the decay rate of
the y(t) impulse response. To illustrate why the term pole is appropriate, Figure 6-8(b) depicts the
three-dimensional surface of |H1(s)| above the s-plane. Look at Figure 6-8(b) carefully and see how
we’ve reoriented the s-plane axis. This new axis orientation allows us to see how the H1(s) system’s
frequency  magnitude  response  can  be  determined  from  its  three-dimensional  s-plane  surface.  If  we
examine  the  |H1(s)|  surface  at  σ  =  0,  we  get  the  bold  curve  in  Figure 6-8(b).  That  bold  curve,  the
intersection  of  the  vertical  σ  =  0  plane  (the  jω  axis  plane)  and  the  |H1(s)|  surface,  gives  us  the
frequency magnitude response |H1(ω)| of the system—and that’s one of the things we’re after here. The
bold |H1(ω)| curve in Figure 6-8(b) is shown in a more conventional way in Figure 6-8(c). Figures 6-
8(b)  and  6-8(c)  highlight  the  very  important  property  that  the  Laplace  transform  is  a  more  general
case of the Fourier transform because if σ = 0, then s = jω. In this case, the |H1(s)| curve for σ = 0
above the s-plane becomes the |H1(ω)| curve above the jω axis in Figure 6-8(c).
Figure 6-7 Descriptions of H1(s): (a) pole located at s = σ + jω = −a0/a1 + j0 on the s-plane; (b)
time-domain y(t) impulse response of the system.
Figure 6-8 Further depictions of H1(s): (a) pole located at σ = −a0/a1 on the s-plane; (b) |H1(s)|
surface; (c) curve showing the intersection of the |H1(s)| surface and the vertical σ = 0 plane. This is
the conventional depiction of the |H1(ω)| frequency magnitude response.
Another  common  system  transfer  function  leads  to  an  impulse  response  that  oscillates.  Let’s  think
about an alternate system whose Laplace transfer function is of the form of Eq. (6-10), the coefficient
b0  equals  zero,  and  the  coefficients  lead  to  complex  terms  when  the  denominator  polynomial  is
factored. We’ll call this particular 2nd-order transfer function H2(s), where
(6-13)
(By  the  way,  when  a  transfer  function  has  the  Laplace  variable  s  in  both  the  numerator  and
denominator, the order of the overall function is defined by the largest exponential order of s in either
the numerator or the denominator polynomial. So our H2(s) is a 2nd-order transfer function.) To keep
the following equations from becoming too messy, let’s factor its denominator and rewrite Eq. (6-13)
as
(6-14)
where A = b1/a2, p = preal + jpimag, and p* = preal − jpimag (complex conjugate of p). Notice that if s is
equal to −p or −p*, one of the polynomial roots in the denominator of Eq. (6-14) will equal zero, and
H2(s) will have an infinite magnitude. Those two complex poles, shown in Figure 6-9(a), are located
off the negative portion of the real σ axis. If the H2 system were at rest and we disturbed it with an
impulselike  x(t)  input  at  time  t  =  0,  its  continuous  time-domain  y(t)  output  would  be  the  damped
sinusoidal  curve  shown  in  Figure  6-9(b).  We  see  that  H2(s)  is  stable  because  its  oscillating  y(t)
output,  like  a  plucked  guitar  string,  approaches  zero  as  time  increases.  Again,  the  distance  of  the
poles  from  the  σ  =  0  axis  (−preal)  gives  the  decay  rate  of  the  sinusoidal  y(t)  impulse  response.
Likewise, the distance of the poles from the jω = 0 axis (±pimag) gives the frequency of the sinusoidal
y(t) impulse response. Notice something new in Figure 6-9(a). When s = 0, the numerator of Eq. (6-
14)  is  zero,  making  the  transfer  function  H2(s)  equal  to  zero.  Any  value  of  s  where  H2(s)  =  0  is
sometimes of interest and is usually plotted on the s-plane as the little circle, called a zero, shown in
Figure 6-9(a). At this point we’re not very interested in knowing exactly what p and p* are in terms of
the coefficients in the denominator of Eq. (6-13). However, an energetic reader could determine the
values  of  p  and  p*  in  terms  of  a0,  a1,  and  a2  by  using  the  following  well-known  quadratic
factorization formula: Given the 2nd-order polynomial f(s) = as2 + bs + c, then f(s) can be factored as
Figure 6-9 Descriptions of H2(s): (a) poles located at s = preal ± jpimag on the s-plane; (b) time-
domain y(t) impulse response of the system.
(6-15)
Figure 6-10(b) illustrates the |H2(s)| surface above the s-plane. Again, the bold |H2(ω)| curve in Figure
6-10(b)  is  shown  in  the  conventional  way  in  Figure  6-10(c)  to  indicate  the  frequency  magnitude
response of the system described by Eq. (6-13). Although the three-dimensional surfaces in Figures
6-8(b)  and  6-10(b)  are  informative,  they’re  also  unwieldy  and  unnecessary.  We  can  determine  a
system’s stability merely by looking at the locations of the poles on the two-dimensional s-plane.
Figure 6-10 Further depictions of H2(s): (a) poles and zero locations on the s–plane; (b) |H2(s)|
surface; (c) |H2(ω)| frequency magnitude response curve.
To further illustrate the concept of system stability, Figure 6-11 shows the s-plane pole locations of
several example Laplace transfer functions and their corresponding time-domain impulse responses.
We  recognize  Figures  6-11(a)  and  6-11(b),  from  our  previous  discussion,  as  indicative  of  stable
systems. When disturbed from their at-rest condition, they respond and, at some later time, return to
that  initial  condition.  The  single  pole  location  at  s  =  0  in  Figure  6-11(c)  is  indicative  of  the  1/s
transfer  function  of  a  single  element  of  a  linear  system.  In  an  electrical  system,  this  1/s  transfer
function could be a capacitor that was charged with an impulse of current, and there’s no discharge
path  in  the  circuit.  For  a  mechanical  system,  Figure 6-11(c)  would  describe  a  kind  of  spring  that’s
compressed  with  an  impulse  of  force  and,  for  some  reason,  remains  under  compression.  Notice,  in
Figure 6-11(d), that if an H(s) transfer function has conjugate poles located exactly on the jω axis (σ =
0), the system will go into oscillation when disturbed from its initial condition. This situation, called
conditional stability,  happens  to  describe  the  intentional  transfer  function  of  electronic  oscillators.
Instability is indicated in Figures 6-11(e) and 6-11(f). Here, the poles lie to the right of the jω axis.
When  disturbed  from  their  initial  at-rest  condition  by  an  impulse  input,  their  outputs  grow  without
bound.† See how the value of σ, the real part of s, for the pole locations is the key here? When σ < 0,
the system is well behaved and stable; when σ = 0, the system is conditionally stable; and when σ >
0, the system is unstable. So we can say that when σ is located on the right half of the s-plane, the
system is unstable. We show this characteristic of linear continuous systems in Figure 6-12. Keep in
mind that real-world systems often have more than two poles, and a system is only as stable as its
least stable pole. For a system to be stable, all of its transfer-function poles must lie on the left half of
the s-plane.
† Impulse response testing in a laboratory can be an important part of the system design process. The difficult part is generating a true
impulselike input. If the system is electrical, for example, although somewhat difficult to implement, the input x(t) impulse would be a very
short-duration  voltage  or  current  pulse.  If,  however,  the  system  were  mechanical,  a  whack  with  a  hammer  would  suffice  as  an  x(t)
impulse input. For digital systems, on the other hand, an impulse input is easy to generate; it’s a single unity-valued sample preceded and
followed by all zero-valued samples.
Figure 6-11 Various H(s) pole locations and their time-domain impulse responses: (a) single pole at
σ < 0; (b) conjugate poles at σ < 0; (c) single pole located at σ = 0; (d) conjugate poles located at σ =
0; (e) single pole at σ > 0; (f) conjugate poles at σ > 0.
Figure 6-12 The Laplace s–plane showing the regions of stability and instability for pole locations
for linear continuous systems.
To  consolidate  what  we’ve  learned  so  far:  H(s)  is  determined  by  writing  a  linear  system’s  time-
domain  differential  equation  and  taking  the  Laplace  transform  of  that  equation  to  obtain  a  Laplace
expression  in  terms  of  X(s), Y(s),  s,  and  the  system’s  coefficients.  Next  we  rearrange  the  Laplace
expression terms to get the H(s) ratio in the form of Eq. (6-10). (The really slick part is that we do not
have  to  know  what  the  time-domain  x(t)  input  is  to  analyze  a  linear  system!)  We  can  get  the
expression for the continuous frequency response of a system just by substituting jω for s in the H(s)
equation. To determine system stability, the denominator polynomial of H(s) is factored to find each
of its roots. Each root is set equal to zero and solved for s to find the location of the system poles on
the  s-plane.  Any  pole  located  to  the  right  of  the  jω  axis  on  the  s-plane  will  indicate  an  unstable
system.
OK, returning to our original goal of understanding the z-transform, the process of analyzing IIR filter
systems requires us to replace the Laplace transform with the z-transform and to replace the s-plane
with  a  z-plane.  Let’s  introduce  the  z-transform,  determine  what  this  new  z-plane  is,  discuss  the
stability of IIR filters, and design and analyze a few simple IIR filters.
6.3 The z-Transform
The z-transform is the discrete-time cousin of the continuous Laplace transform.† While the Laplace
transform  is  used  to  simplify  the  analysis  of  continuous  differential  equations,  the  z-transform
facilitates the analysis of discrete difference equations. Let’s define the z-transform and explore its
important characteristics to see how it’s used in analyzing IIR digital filters.
† In the early 1960s, James Kaiser, after whom the Kaiser window function is named, consolidated the theory of digital filters using a
mathematical description known as the z-transform[4,5]. Until that time, the use of the z-transform had generally been restricted to the
field of discrete control systems[6–9].
The z-transform of a discrete sequence h(n), expressed as H(z), is defined as
(6-16)
where  the  variable  z  is  complex.  Where  Eq.  (6-3)  allowed  us  to  take  the  Laplace  transform  of  a
continuous signal, the z-transform is performed on a discrete h(n) sequence, converting that sequence
into a continuous function H(z) of the continuous complex variable z. Similarly, as the function e−st is
the  general  form  for  the  solution  of  linear  differential  equations,  z−n  is  the  general  form  for  the
solution of linear difference equations. Moreover, as a Laplace function F(s) is a continuous surface
above the s-plane, the z-transform function H(z) is a continuous surface above a z-plane. To whet your
appetite, we’ll now state that if H(z) represents an IIR filter’s z-domain transfer function, evaluating
the H(z)  surface  will  give  us  the  filter’s  frequency  magnitude  response,  and  H(z)’s  pole  and  zero
locations will determine the stability of the filter.
We can determine the frequency response of an IIR digital filter by expressing z in polar form as z =
rejω, where r is a magnitude and ω is the angle. In this form, the z-transform equation becomes
(6-16′)
Equation (6-16′) can be interpreted as the Fourier transform of the product of the original sequence
h(n)  and  the  exponential  sequence  r−n.  When  r  equals  one,  Eq.  (6-16′)  simplifies  to  the  Fourier
transform. Thus on the z-plane, the contour of the H(z) surface for those values where |z| = 1 is the
Fourier transform of h(n). If h(n) represents a filter impulse response sequence, evaluating the H(z)
transfer function for |z| = 1 yields the frequency response of the filter. So where on the z-plane is |z| =
1? It’s a circle with a radius of one, centered about the z = 0 point. This circle, so important that it’s
been given the name unit circle,  is  shown  in  Figure 6-13.  Recall  that  the  jω  frequency  axis  on  the
continuous Laplace s-plane was linear and ranged from − ∞ to + ∞ radians/second. The ω frequency
axis  on  the  complex  z-plane,  however,  spans  only  the  range  from  −π  to  +π  radians.  With  this
relationship between the jω axis on the Laplace s-plane and the unit circle on the z-plane, we can see
that the z-plane frequency axis is equivalent to coiling the s-plane’s jω axis about the unit circle on the
z-plane as shown in Figure 6-14.
Figure 6-13 Unit circle on the complex z–plane.
Figure 6-14 Mapping of the Laplace s–plane to the z–plane. All frequency values are in
radians/second.
Then, frequency ω on the z-plane is not a distance along a straight line, but rather an angle around a
circle. With ω in Figure 6-13 being a general normalized angle in radians ranging from −π to +π, we
can  relate  ω  to  an  equivalent  fs  sampling  rate  by  defining  a  new  frequency  variable  ωs  =  2πfs  in
radians/second.  The  periodicity  of  discrete  frequency  representations,  with  a  period  of  ωs  =  2πfs
radians/second  or  fs  Hz,  is  indicated  for  the  z-plane  in  Figure  6-14.  Where  a  walk  along  the  jω
frequency axis on the s-plane could take us to infinity in either direction, a trip on the ω frequency
path on the z-plane leads us in circles (on the unit circle). Figure 6-14 shows us that only the −πfs to
+πfs  radians/second  frequency  range  for  ω  can  be  accounted  for  on  the  z-plane,  and  this  is  another
example  of  the  universal  periodicity  of  the  discrete  frequency  domain.  (Of  course,  the  −πfs  to  +πfs
radians/second range corresponds to a cyclic frequency range of −fs/2 to +fs/2.) With the perimeter of
the unit circle being z = ejω,  later,  we’ll  show  exactly  how  to  substitute  ejω for z  in  a  filter’s  H(z)
transfer function, giving us the filter’s frequency response.
6.3.1 Poles, Zeros, and Digital Filter Stability
One of the most important characteristics of the z-plane is that the region of filter stability is mapped
to the inside of the unit circle on the z-plane. Given the H(z) transfer function of a digital filter, we
can examine that function’s pole locations to determine filter stability. If all poles are located inside
the unit circle, the filter will be stable. On the other hand, if any pole is located outside the unit circle,
the filter will be unstable.
For example, if a causal filter’s H(z) transfer function has a single pole at location p on the z-plane,
its transfer function can be represented by
(6-17)
and the filter’s time-domain impulse response sequence is
(6-17′)
where u(n) represents a unit step (all ones) sequence beginning at time n = 0. Equation (6-17′) tells us
that as time advances, the impulse response will be p raised to larger and larger powers. When the
pole location p has a magnitude less than one, as shown in Figure 6-15(a), the h(n) impulse response
sequence is unconditionally bounded in amplitude. And a value of |p| < 1 means that the pole must lie
inside the z-plane’s unit circle.
Figure 6-15 Various H(z) pole locations and their discrete time-domain impulse responses: (a) single
pole inside the unit circle; (b) conjugate poles located inside the unit circle; (c) conjugate poles
located on the unit circle; (d) single pole outside the unit circle; (e) conjugate poles located outside
the unit circle.
Figure  6-15  shows  the  z-plane  pole  locations  of  several  example  z-domain  transfer  functions  and
their  corresponding  discrete  time-domain  impulse  responses.  It’s  a  good  idea  for  the  reader  to
compare the z-plane and discrete-time responses of Figure 6-15 with the s-plane and the continuous-
time responses of Figure 6-11. The y(n)  outputs  in  Figures 6-15(d)  and  6-15(e)  show  examples  of
how  unstable  filter  outputs  increase  in  amplitude,  as  time  passes,  whenever  their  x(n)  inputs  are
nonzero.  To  avoid  this  situation,  any  IIR  digital  filter  that  we  design  should  have  an  H(z)  transfer
function with all of its individual poles inside the unit circle. Like a chain that’s only as strong as its
weakest link, an IIR filter is only as stable as its least stable pole.
The ωo oscillation frequency of the impulse responses in Figures 6-15(c) and 6-15(e) is, of course,
proportional  to  the  angle  of  the  conjugate  pole  pairs  from  the  zreal  axis,  or  ωo  radians/second
corresponding to fo = ωo/2π Hz. Because the intersection of the −zreal axis and the unit circle, point z =
−1,  corresponds  to  π  radians  (or  πfs radians/second = fs/2 Hz), the ωo  angle  of  π/4  in  Figure  6-15
means that fo = fs/8 and our y(n) will have eight samples per cycle of fo.
6.4 Using the z-Transform to Analyze IIR Filters
We have one last concept to consider before we can add the z-transform to our collection of digital
signal processing tools. We need to determine how to represent Figure 6-3’s delay operation as part
of our z-transform  filter  analysis  equations.  To  do  this,  assume  we  have  a  sequence  x(n)  whose  z-
transform  is  X(z)  and  a  sequence  y(n)  =  x(n−1)  whose  z-transform  is  Y(z)  as  shown  in  Figure  6-
16(a). The z-transform of y(n) is, by definition,
(6-18)
Figure 6-16 Time- and z-domain delay element relationships: (a) single delay; (b) multiple delays.
Now if we let k = n−1, then Y(z) becomes
which we can write as
(6-19)
(6-20)
Thus, the effect of a single unit of time delay is to multiply the z-transform of the undelayed sequence
by z−1.
6.4.1 z-Domain IIR Filter Analysis
Interpreting a unit time delay to be equivalent to the z−1 operator leads us to the relationship shown in
Figure 6-16(b), where we can say X(z)z0 = X(z) is the z-transform of x(n), X(z)z−1 is the z-transform
of x(n) delayed by one sample, X(z)z−2 is the z-transform of x(n) delayed by two samples, and X(z)z
−k is the z-transform of x(n) delayed by k  samples.  So  a  transfer  function  of  z−k  is  equivalent  to  a
delay of kts seconds from the instant when t = 0, where ts is the period between data samples, or one
over the sample rate. Specifically, ts = 1/fs. Because a delay of one sample is equivalent to the factor
z−1, the unit time delay symbol used in Figures 6-2 and 6-3 is usually indicated by the z−1 operator as
in Figure 6-16(b).
Let’s  pause  for  a  moment  and  consider  where  we  stand  so  far.  Our  acquaintance  with  the  Laplace
transform with its s-plane, the concept of stability based on H(s) pole locations, the introduction of
the z-transform with its z-plane poles, and the concept of the z−1 operator signifying a single unit of
time  delay  has  led  us  to  our  goal:  the  ability  to  inspect  an  IIR  filter  difference  equation  or  filter
structure  (block  diagram)  and  immediately  write  the  filter’s  z-domain  transfer  function  H(z).
Accordingly, by evaluating an IIR filter’s H(z) transfer function appropriately, we can determine the
filter’s frequency response and its stability. With those ambitious thoughts in mind, let’s develop the
z-domain  equations  we  need  to  analyze  IIR  filters.  Using  the  relationships  of  Figure  6-16(b),  we
redraw Figure 6-3 as a general Mth-order IIR filter using the z−1 operator as shown in Figure 6-17.
(In hardware, those z−1  operations  are  memory  locations  holding  successive  filter  input  and  output
sample  values.  When  implementing  an  IIR  filter  in  a  software  routine,  the  z−1  operation  merely
indicates  sequential  memory  locations  where  input  and  output  sequences  are  stored.)  The  IIR  filter
structure in Figure 6-17 is called the Direct Form I structure. The time-domain difference equation
describing the general Mth-order IIR filter, having N feedforward stages and M feedback stages, in
Figure 6-17 is
(6-21)
Figure 6-17 General (Direct Form I) structure of an Mth-order IIR filter, having N feedforward stages
and M feedback stages, with the z−1 operator indicating a unit time delay.
In the z-domain, that IIR filter’s output can be expressed by
(6-22)
where Y(z) and X(z)  represent  the  z-transform  of  y(n)  and  x(n).  Look  Eqs. (6-21)  and  (6-22)  over
carefully  and  see  how  the  unit  time  delays  translate  to  negative  powers  of  z  in  the  z-domain
expression. A more compact notation for Y(z) is
(6-23)
OK,  now  we’ve  arrived  at  the  point  where  we  can  describe  the  transfer  function  of  a  general  IIR
filter. Rearranging Eq. (6-23), to collect like terms, we write
(6-24)
Finally, we define the filter’s z-domain transfer function as H(z) = Y(z)/X(z), where H(z) is given by
(6-25)
Just as with Laplace transfer functions, the order of our z-domain transfer function and the order of
our filter are defined by the largest exponential order of z in either the numerator or the denominator
in Eq. (6-25).
There are two things we need to know about an IIR filter: its frequency response and whether or not
the filter is stable. Equation (6-25) is the origin of that information. We can evaluate the denominator
of  Eq.  (6-25)  to  determine  the  positions  of  the  filter’s  poles  on  the  z-plane  indicating  the  filter’s
stability. Next, from Eq. (6-25) we develop an expression for the IIR filter’s frequency response.
Remember, now, just as the Laplace transfer function H(s) in Eq. (6-9) was a complex-valued surface
on the s-plane, H(z)  is  a  complex-valued  surface  above,  or  below,  the  z-plane.  The  intersection  of
that H(z)  surface  and  the  perimeter  of  a  cylinder  representing  the  z  =  ejω  unit  circle  is  the  filter’s
complex frequency response. This means that substituting ejω for z in Eq. (6-25)’s  transfer  function
gives us the expression for the filter’s H(ω) frequency response as
(6-26)
In  rectangular  form,  using  Euler’s  identity,  e−jω  =  cos(ω)−jsin(ω),  the  filter’s  H(ω)  frequency
response is
(6-27)
Shortly, we’ll use the above expressions to analyze an actual IIR filter.
Pausing a moment to gather our thoughts, we realize that H(ω) is the ratio of complex functions and
we can use Eq. (6-27) to compute the magnitude and phase response of IIR filters as a function of the
frequency ω. And again, just what is ω? It’s the normalized frequency represented by the angle around
the unit circle in Figure 6-13, having a range of −π≤ω≤+ω radians/sample. In terms of our old friend
fs Hz, Eq. (6-27) applies over the equivalent frequency range of −fs/2 to +fs/2 Hz. So, for example, if
digital data is arriving at the filter’s input at a rate of fs =1000 samples/second, we could use Eq. (6-
27) to plot the filter’s frequency magnitude response over the frequency range of −500 Hz to +500 Hz.
6.4.2 IIR Filter Analysis Example
Although  Eqs.  (6-25)  and  (6-26)  look  somewhat  complicated  at  first  glance,  let’s  illustrate  their
simplicity and utility by analyzing the simple 2nd-order lowpass IIR filter in Figure 6-18(a) whose
positive cutoff frequency is ω = π/5 (fs/10 Hz).
Figure 6-18 Second-order lowpass IIR filter example.
By inspection, we can write the filter’s time-domain difference equation as
(6-28)
There are two ways to obtain the z-domain expression of our filter. The first way is to look at Eq. (6-
28) and by inspection write
(6-29)
The second way to obtain the desired z-domain expression is to redraw Figure 6-18(a) with the z-
domain notation as in Figure 6-18(b). Then by inspection of Figure 6-18(b) we could have written
Eq. (6-29).
A piece of advice for the reader to remember: although not obvious in this IIR filter analysis example,
it’s  often  easier  to  determine  a  digital  network’s  transfer  function  using  the  z-domain  notation  of
Figure 6-18(b)  rather  than  using  the  time-domain  notation  of  Figure 6-18(a).  (Writing  the  z-domain
expression  for  a  network  based  on  the  Figure  6-18(b)  notation,  rather  than  writing  a  time-domain
expression based on the Figure 6-18(a) time notation, generally yields fewer unknown variables in
our  network  analysis  equations.)  Over  the  years  of  analyzing  digital  networks,  I  regularly  remind
myself,  “z-domain  produces  less  pain.”  Keep  this  advice  in  mind  if  you  attempt  to  solve  the
homework problems at the end of this chapter.
Back to our example: We can obtain the desired H(z) filter transfer function by rearranging Eq. (6-
29), or by using Eq. (6-25). Either method yields
(6-30)
Replacing z with ejω, we see that the frequency response of our example IIR filter is
(6-31)
We’re almost there. Remembering Euler’s equations and that cos(0) = 1 and sin(0) = 0, we can write
the rectangular form of H(ω) as
(6-32)
Equation (6-32) is what we’re after here, and if we compute that messy expression’s magnitude over
the frequency range of −π≤ω≤π, we produce the |H(ω)| shown as the solid curve in Figure 6-19(a).
For comparison purposes we also show a 5-tap lowpass FIR filter magnitude response in Figure 6-
19(a). Although both filters require the same computational workload, five multiplications per filter
output  sample,  the  lowpass  IIR  filter  has  the  superior  frequency  magnitude  response.  Notice  the
steeper magnitude response roll-off and lower sidelobes of the IIR filter relative to the FIR filter. (To
make this IIR and FIR filter comparison valid, the coefficients used for both filters were chosen so
that each filter would approximate the ideal lowpass frequency response shown in Figure 5-17(a).)
Figure 6-19 Performances of the example IIR filter (solid curves) in Figure 6-18 and a 5-tap FIR
filter (dashed curves): (a) magnitude responses; (b) phase responses; (c) IIR filter impulse response;
(d) IIR filter poles and zeros.
A  word  of  warning  here.  It’s  easy  to  inadvertently  reverse  some  of  the  signs  for  the  terms  in  the
denominator  of  Eq.  (6-32),  so  be  careful  if  you  attempt  these  calculations  at  home.  Some  authors
avoid  this  problem  by  showing  the  a(k)  coefficients  in  Figure 6-17  as  negative  values,  so  that  the
summation  in  the  denominator  of  Eq.  (6-25)  is  always  positive.  Moreover,  some  commercial
software IIR design routines provide a(k) coefficients whose signs must be reversed before they can
be applied to the IIR structure in Figure 6-17. (If, while using software routines to design or analyze
IIR filters, your results are very strange or unexpected, the first thing to do is reverse the signs of the
a(k) coefficients and see if that doesn’t solve the problem.)
The  solid  curve  in  Figure  6-19(b)  is  our  IIR  filter’s  ø(ω)  phase  response.  Notice  its  nonlinearity
relative to the FIR filter’s phase response. (Remember, now, we’re only interested in the filter phase
responses over the lowpass filter’s passband. So those phase discontinuities for the FIR filter are of
no consequence.) Phase nonlinearity is inherent in IIR filters and, based on the ill effects of nonlinear
phase  introduced  in  the  group  delay  discussion  of  Section  5.8,  we  must  carefully  consider  its
implications whenever we decide to use an IIR filter instead of an FIR filter in any given application.
The question any filter designer must ask and answer is “How much phase distortion can I tolerate to
realize  the  benefits  of  the  reduced  computational  workload  and  high  data  rates  afforded  by  IIR
filters?”
Figure 6-19(c) shows our filter’s time-domain h(k) impulse response. Knowing that the filter’s phase
response is nonlinear, we should expect the impulse response to be asymmetrical as it indeed is. That
figure  also  illustrates  why  the  term  infinite impulse response  is  used  to  describe  IIR  filters.  If  we
used  infinite-precision  arithmetic  in  our  filter  implementation,  the  h(k)  impulse  response  would  be
infinite in duration. In practice, of course, a filter’s output samples are represented by a finite number
of binary bits. This means that a stable IIR filter’s h(k) samples will decrease in amplitude, as time
index k increases, and eventually reach an amplitude level that’s less than the smallest representable
binary value. After that, all future h(k) samples will be zero-valued.
To determine our IIR filter’s stability, we must find the roots of the 2nd-order polynomial of H(z)’s
denominator in Eq. (6-30). Those roots are the z-plane poles of H(z) and if their magnitudes are less
than one, the IIR filter is stable. To determine the two pole locations, p0 and p1, first we multiply H(z)
by z2/z2 to obtain polynomials with positive exponents. After doing so, H(z) becomes
(6-33)
Factoring Eq. (6-33) using the quadratic factorization formula from Eq. (6-15), we obtain the ratio of
factors
(6-34)
So when z = p0 = 0.597 − j0.282, or when z = p1 = 0.597 + j0.282, the filter’s H(z) transfer function’s
denominator is zero and |H(z)| is infinite. We show the p0 and p1  pole  locations  in  Figure  6-19(d).
Because  those  pole  locations  are  inside  the  unit  circle  (their  magnitudes  are  less  than  one),  our
example  IIR  filter  is  unconditionally  stable.  The  two  factors  in  the  numerator  of  Eq.  (6-34)
correspond to two z-plane zeros at z = z0 = z1 = −1 (at a continuous-time frequency of ±fs/2), shown
in Figure 6-19(d).
To help us understand the relationship between the poles/zeros of H(z) and the magnitude of the H(z)
transfer function, we show a crude depiction of the |H(z)| surface as a function of z in Figure 6-20(a).
Figure 6-20 IIR filter’s |H(z)| surface: (a) pole locations; (b) frequency magnitude response.
Continuing to review the |H(z)| surface, we can show its intersection with the unit circle as the bold
curve in Figure 6-20(b). Because z = rejω, with r restricted to unity, then z = ejω and the bold curve is
|H(z)||z|=1 = |H(ω)|, representing the lowpass filter’s frequency magnitude response on the z-plane. If
we were to unwrap the bold |H(ω)| curve in Figure 6-20(b)  and  lay  it  on  a  flat  surface,  we  would
have the |H(ω)| curve in Figure 6-19(a). Neat, huh?
6.5 Using Poles and Zeros to Analyze IIR Filters
In the last section we discussed methods for finding an IIR filter’s z-domain H(z) transfer function in
order to determine the filter’s frequency response and stability. In this section we show how to use a
digital  filter’s  pole/zero  locations  to  analyze  that  filter’s  frequency-domain  performance.  To
understand this process, first we must identify the two most common algebraic forms used to express
a filter’s z-domain transfer function.
6.5.1 IIR Filter Transfer Function Algebra
We have several ways to write the H(z) = Y(z)/X(z) z-domain transfer function of an IIR filter. For
example, similar to Eq. (6-30), we can write H(z) in the form of a ratio of polynomials in negative
powers of z. For a 4th-order IIR filter such an H(z) expression would be
(6-35)
Expressions  like  Eq.  (6-35)  are  super-useful  because  we  can  replace  z  with  ejω  to  obtain  an
expression for the frequency response of the filter. We used that substitution in the last section.
On the other hand, multiplying Eq. (6-35) by z4/z4, we can express H(z) in the polynomial form
(6-36)
Expressions  in  the  form  of  Eq.  (6-36)  are  necessary  so  we  can  factor  (find  the  roots  of)  the
polynomials to obtain values (locations) of the numerator zeros and denominator poles, such as in the
following factored form:
(6-37)
Such an H(z) transfer function has four zeros (z0, z1, z2, and z3) and four poles (p0, p1, p2, and p3).
We’re  compelled  to  examine  a  filter’s  H(z)  transfer  function  in  the  factored  form  of  Eq.  (6-37)
because the pk pole values tell us whether or not the IIR filter is stable. If the magnitudes of all pk
poles are less than one, the filter is stable. The filter zeros, zk, do not affect filter stability.
As an aside, while we won’t encounter such filters until Chapter 7 and Chapter 10, it is possible to
have  a  digital  filter  whose  transfer  function,  in  the  factored  form  of  Eq.  (6-37),  has  common
(identical) factors in its numerator and denominator. Those common factors produce a zero and a pole
that  lie  exactly  on  top  of  each  other.  Like  matter  and  anti-matter,  such  zero-pole  combinations
annihilate each other, leaving neither a zero nor a pole at that z-plane location.
Multiplying the factors in Eq. (6-37), a process called “expanding the transfer function” allows us to
go from the factored form of Eq. (6-37) to the polynomial form in Eq. (6-36). As such, in our digital
filter analysis activities we can translate back and forth between the polynomial and factored forms of
H(z).
Next we review the process of analyzing a digital filter given the filter’s poles and zeros.
6.5.2 Using Poles/Zeros to Obtain Transfer Functions
As  it  turns  out,  we  can  analyze  an  IIR  filter’s  frequency-domain  performance  based  solely  on  the
filter’s poles and zeros. Given that we know the values of a filter’s zk zeros and pk poles, we can
write the factored form of the filter’s transfer function as
(6-38)
where G = G1/G2 is an arbitrary gain constant. Thus, knowing a filter’s zk zeros and pk poles, we can
determine the filter’s transfer function to within a constant scale factor G.
Again, filter zeros are associated with decreased frequency magnitude response, and filter poles are
associated with increased frequency magnitude response. For example, if we know that a filter has no
z-plane zeros, and one pole at p0 = 0.8, we can write its transfer function as
(6-39)
The characteristics of such a filter are depicted in Figure 6-21(a). The |H1(ω)| frequency magnitude
response in the figure is normalized so that the peak magnitude is unity. Because the p0 pole is closest
to the ω = 0 radians/sample frequency point (z = 1) on the unit circle, the filter is a lowpass filter.
Additionally, because |p0| is less than one, the filter is unconditionally stable.
Figure 6-21 IIR filter poles/zeros and normalized frequency magnitude responses.
If a filter has a zero at z0 = 1, and a pole at p0 = −0.8, we write its transfer function as
(6-40)
The characteristics of this filter are shown in Figure 6-21(b). Because the pole is closest to the ω = π
radians/sample frequency point (z = −1) on the unit circle, the filter is a highpass filter. Notice that the
zero located at z = 1 causes the filter to have infinite attenuation at ω = 0 radians/sample (zero Hz).
Because  this  pole/zero  filter  analysis  topic  is  so  important,  let  us  look  at  several  more  pole/zero
examples.
Consider a filter having two complex conjugate zeros at −0.707 ± j0.707,  as  well  as  two  complex
conjugate poles at 0.283 ± j0.283. This filter’s transfer function is
(6-41)
The properties of this H3(z) filter are presented in Figure 6-21(c). The two poles on the right side of
the z-plane make this a lowpass filter having a wider passband than the above H1(z) lowpass filter.
Two  zeros  are  on  the  unit  circle  at  angles  of  ω  =  ±3π/4  radians,  causing  the  filter  to  have  infinite
attenuation at the frequencies ω = ±3π/4 radians/sample (±3fs/8 Hz) as seen in the |H3(ω)| magnitude
response.
If we add a z-plane zero at z = 1 to the above H3(z), we create an H4(z) filter whose transfer function
is
(6-42)
The  characteristics  of  this  filter  are  shown  in  Figure  6-21(d).  The  zero  at  z  =  1  yields  infinite
attenuation at ω = 0 radians/sample (zero Hz), creating a bandpass filter. Because the p0 and p1 poles
of H4(z) are oriented at angles of θ = ±π/4 radians, the filter’s passbands are centered in the vicinity
of frequencies ω = ±π/4 radians/sample (±fs/8 Hz).
Next, if we increase the magnitude of the H4(z) filter’s poles, making them equal to 0.636 ± j0.636,
we position the conjugate poles much closer to the unit circle as shown by the H5(z) characteristics in
Figure 6-21(e). The H5(z) filter transfer function is
(6-43)
There are two issues to notice in this scenario. First, poles near the unit circle now have a much more
profound effect on the filter’s magnitude response. The poles’ infinite gains cause the H5(z) passbands
to be very narrow (sharp). Second, when a pole is close to the unit circle, the center frequency of its
associated passband can be accurately estimated to be equal to the pole’s angle. That is, Figure 6-
21(e)  shows  us  that  with  the  poles’  angles  being  θ  =  ±π/4  radians,  the  center  frequencies  of  the
narrow passbands are very nearly equal to ω = ±π/4 radians/sample (±fs/8 Hz).
For  completeness,  one  last  pole/zero  topic  deserves  mention.  Consider  a  finite  impulse  response
(FIR) filter—a digital filter whose H(z)  transfer  function  denominator  is  unity.  For  an  FIR  filter  to
have linear phase each z-plane zero located at z = z0 = Mejα, where M ≠ 1, must be accompanied by a
zero having an angle of −α and a magnitude of 1/M. (Proof of this restriction is available in reference
[10].) We show this restriction in Figure 6-21(f) where the z0 zero is accompanied by the z3 zero. If
the FIR filter’s transfer function polynomial has real-valued bk coefficients, then a z0 zero not on the
z-plane’s real axis will be accompanied by a complex conjugate zero at z = z2. Likewise, for the FIR
filter to have linear phase the z2 zero must be accompanied by the z1 zero. Of course, the z1 and the z3
zeros are complex conjugates of each other.
To conclude this section, we provide the following brief list of z-plane pole/zero properties that we
should keep in mind as we work with digital filters:
• Filter poles are associated with increased frequency magnitude response (gain).
• Filter zeros are associated with decreased frequency magnitude response (attenuation).
• To be unconditionally stable all filter poles must reside inside the unit circle.
• Filter zeros do not affect filter stability.
•  The  closer  a  pole  (zero)  is  to  the  unit  circle,  the  stronger  will  be  its  effect  on  the  filter’s  gain
(attenuation) at the frequency associated with the pole’s (zero’s) angle.
• A pole (zero) located on the unit circle produces infinite filter gain (attenuation).
• If a pole is at the same z-plane location as a zero, they cancel each other.
• Poles or zeros located at the origin of the z-plane do not affect the frequency response of the filter.
• Filters whose transfer function denominator (numerator) polynomial has real-valued coefficients
have  poles  (zeros)  located  on  the  real  z-plane  axis,  or  pairs  of  poles  (zeros)  that  are  complex
conjugates of each other.
• For an FIR filter (transfer function denominator is unity) to have linear phase, any zero on the z-
plane  located  at  z0  =  Mejα,  where  z0  is  not  on  the  unit  circle  and  α  is  not  zero,  must  be
accompanied by a reciprocal zero whose location is 1/z0 = e−jα/M.
• What the last two bullets mean is that if an FIR filter has real-valued coefficients, is linear phase,
and has a z-plane zero not located on the real z-plane axis or on the unit circle, that z-plane zero is
a member of a “gang of four” zeros. If we know the z-plane location of one of those four zeros,
then we know the location of the other three zeros.
6.6 Alternate IIR Filter Structures
In  the  literature  of  DSP,  it’s  likely  that  you  will  encounter  IIR  filters  other  than  the  Direct  Form  I
structure of the IIR filter in Figure 6-17. This point of our IIR filter studies is a good time to introduce
those alternate IIR filter structures (block diagrams).
6.6.1 Direct Form I, Direct Form II, and Transposed Structures
The  Direct  Form  I  structure  of  the  IIR  filter  in  Figure  6-17  can  be  converted  to  several  alternate
forms. It’s easy to explore this idea by assuming that there are two equal-length delay lines, letting M
= N = 2 as in Figure 6-22(a), and thinking of the feedforward and feedback portions as two separate
filter  stages.  Because  both  stages  of  the  filter  are  linear  and  time  invariant,  we  can  swap  them,  as
shown in Figure 6-22(b), with no change in the y(n) output.
Figure 6-22 Rearranged 2nd-order IIR filter structures: (a) Direct Form I; (b) modified Direct Form
I; (c) Direct Form II; (d) transposed Direct Form II.
The two identical delay lines in Figure 6-22(b) provide the motivation for this reorientation. Because
the sequence g(n) is being shifted down along both delay lines in Figure 6-22(b), we can eliminate
one of the delay paths and arrive at the simplified Direct Form II filter structure shown in Figure 6-
22(c),  where  only  half  the  delay  storage  registers  are  required  compared  to  the  Direct  Form  I
structure.
Another  popular  IIR  structure  is  the  transposed  form  of  the  Direct  Form  II  filter.  We  obtain  a
transposed form by starting with the Direct Form II filter, convert its signal nodes to adders, convert
its  adders  to  signal  nodes,  reverse  the  direction  of  its  arrows,  and  swap  x(n)  and  y(n).  (The
transposition  steps  can  also  be  applied  to  FIR  filters.)  Following  these  steps  yields  the  transposed
Direct Form II structure given in Figure 6-22(d).
All the filters in Figure 6-22 have the same performance just so long as infinite-precision arithmetic
is  used.  However,  using  quantized  binary  arithmetic  to  represent  our  filter  coefficients,  and  with
truncation or rounding being used to combat binary overflow errors, the various filters in Figure 6-22
exhibit different quantization noise and stability characteristics. In fact, the transposed Direct Form II
structure  was  developed  because  it  has  improved  behavior  over  the  Direct  Form  II  structure  when
fixed-point binary arithmetic is used. Common consensus among IIR filter designers is that the Direct
Form I filter has the most resistance to coefficient quantization and stability problems. We’ll revisit
these finite-precision arithmetic issues in Section 6.7.
By the way, because of the feedback nature of IIR filters, they’re often referred to as recursive filters.
Similarly,  FIR  filters  are  often  called  nonrecursive  filters.  A  common  misconception  is  that  all
recursive  filters  are  IIR.  This  not  true  because  FIR  filters  can  be  implemented  with  recursive
structures. (Chapters 7 and 10 discuss filters having feedback but whose impulse responses are finite
in duration.) So, the terminology recursive and nonrecursive should be applied to a filter’s structure,
and the terms IIR and FIR should only be used to describe the time duration of the filter’s impulse
response[11,12].
6.6.2 The Transposition Theorem
There is a process in DSP that allows us to change the structure (the block diagram implementation)
of  a  linear  time-invariant  digital  network  without  changing  the  network’s  transfer  function  (its
frequency  response).  That  network  conversion  process  follows  what  is  called  the  transposition
theorem. That theorem is important because a transposed version of some digital network might be
easier to implement, or may exhibit more accurate processing, than the original network.
We primarily think of the transposition theorem as it relates to digital filters, so below are the steps to
transpose a digital filter (or any linear time-invariant network for that matter):
1. Reverse the direction of all signal-flow arrows.
2. Convert all adders to signal nodes.
3. Convert all signal nodes to adders.
4. Swap the x(n) input and y(n) output labels.
An  example  of  this  transposition  process  is  shown  in  Figure 6-23.  The  Direct  Form  II  IIR  filter  in
Figure 6-23(a)  is  transposed  to  the  structure  shown  in  Figure 6-23(b).  By  convention,  we  flip  the
network in Figure 6-23(b) from left to right so that the x(n) input is on the left as shown in Figure 6-
23(c).
Figure 6-23 Converting a Direct Form II filter to its transposed form.
Notice that the transposed filter contains the same number of delay elements, multipliers, and addition
operations as the original filter, and both filters have the same transfer function given by
(6-44)
When implemented using infinite-precision arithmetic, the Direct Form II and the transposed Direct
Form II filters have identical frequency response properties. As mentioned in Section 6.6.1, however,
the  transposed  Direct  Form  II  structure  is  less  susceptible  to  the  errors  that  can  occur  when  finite-
precision binary arithmetic, for example, in a 16-bit processor, is used to represent data values and
filter  coefficients  within  a  filter  implementation.  That  property  is  because  Direct  Form  II  filters
implement  their  (possibly  high-gain)  feedback  pole  computations  before  their  feedforward  zeros
computations,  and  this  can  lead  to  problematic  (large)  intermediate  data  values  which  must  be
truncated.  The  transposed  Direct  Form  II  filters,  on  the  other  hand,  implement  their  zeros
computations first followed by their pole computations.
6.7 Pitfalls in Building IIR Filters
There’s an old saying in engineering: “It’s one thing to design a system on paper, and another thing to
actually build one and make it work.” (Recall the Tacoma Narrows Bridge episode!) Fabricating a
working system based on theoretical designs can be difficult in practice. Let’s see why this is often
true for IIR digital filters.
Again, the IIR filter structures in Figures 6-18 and 6-22 are called Direct Form implementations of an
IIR  filter.  That’s  because  they’re  all  equivalent  to  directly  implementing  the  general  time-domain
expression  for  an  Mth-order  IIR  filter  given  in  Eq.  (6-21).  As  it  turns  out,  there  can  be  stability
problems and frequency response distortion errors when Direct Form implementations are used for
high-order filters. Such problems arise because we’re forced to represent the IIR filter coefficients
and  results  of  intermediate  filter  calculations  with  binary  numbers  having  a  finite  number  of  bits.
There are three major categories of finite-word-length errors that plague IIR filter implementations:
coefficient quantization, overflow errors, and roundoff errors.
Coefficient quantization (limited-precision coefficients) will result in filter pole and zero shifting on
the z-plane, and a frequency magnitude response that may not meet our requirements, and the response
distortion worsens for higher-order IIR filters.
Overflow,  the  second  finite-word-length  effect  that  troubles  IIR  filters,  is  what  happens  when  the
result of an arithmetic operation is too large to be represented in the fixed-length hardware registers
assigned to contain that result. Because we perform so many additions when we implement IIR filters,
overflow is always a potential problem. With no precautions being taken to handle overflow, large
nonlinearity errors can result in our filter output samples—often in the form of overflow oscillations.
The most common way of dealing with binary overflow errors is called roundoff, or rounding, where
a data value is represented by, or rounded off to, the b-bit binary number that’s nearest the unrounded
data value. It’s usually valid to treat roundoff errors as a random process, but conditions occur in IIR
filters  where  rounding  can  cause  the  filter  output  to  oscillate  forever  even  when  the  filter  input
sequence  is  all  zeros.  This  situation,  caused  by  the  roundoff  noise  being  highly  correlated  with  the
signal,  going  by  the  names  limit  cycles  and  deadband  effects,  has  been  well  analyzed  in  the
literature[13,14]. We can demonstrate limit cycles by considering the 2nd-order IIR filter in Figure 6-
24(a) whose time-domain expression is
(6-45)
Figure 6-24 Limit cycle oscillations due to rounding: (a) 2nd-order IIR filter; (b) one possible time-
domain response of the IIR filter.
Let’s  assume  this  filter  rounds  the  adder’s  output  to  the  nearest  integer  value.  If  the  situation  ever
arises where y(−2) = 0, y(−1) = 8, and x(0) and all successive x(n) inputs are zero, the filter output
goes into endless oscillation as shown in Figure 6-24(b).  If  this  filter  were  to  be  used  in  an  audio
application, when the input signal went silent the listener could end up hearing an audio tone instead
of  silence.  The  dashed  line  in  Figure  6-24(b)  shows  the  filter’s  stable  response  to  this  particular
situation if no rounding is used. With rounding, however, this IIR filter certainly lives up to its name.
(Welcome to the world of binary arithmetic!)
There are several ways to reduce the ill effects of coefficient quantization errors and limit cycles. We
can  increase  the  word  widths  of  the  hardware  registers  that  contain  the  results  of  intermediate
calculations.  Because  roundoff  limit  cycles  affect  the  least  significant  bits  of  an  arithmetic  result,
larger  word  sizes  diminish  the  impact  of  limit  cycles  should  they  occur.  To  avoid  filter  input
sequences of all zeros, some practitioners add a dither sequence to the filter’s input signal sequence.
A dither sequence is a sequence of low-amplitude pseudo-random numbers that interferes with an IIR
filter’s  roundoff  error  generation  tendency,  allowing  the  filter  output  to  reach  zero  should  the  input
signal  remain  at  zero.  Dithering,  however,  decreases  the  effective  signal-to-noise  ratio  of  the  filter
output[12]. Finally, to avoid limit cycle problems, we can just use an FIR filter. Because FIR filters
by definition have finite-length impulse responses, and have no feedback paths, they cannot support
output oscillations of any kind.
As for overflow errors, we can eliminate them if we increase the word width of hardware registers
so  overflow  never  takes  place  in  the  IIR  filter.  Filter  input  signals  can  be  scaled  (reduced  in
amplitude by multiplying signals within the filter by a factor less than one) so overflow is avoided.
We  discuss  such  filter  scaling  in  Section  6.9.  Overflow  oscillations  can  be  avoided  by  using
saturation  arithmetic  logic  where  signal  values  aren’t  permitted  to  exceed  a  fixed  limit  when  an
overflow condition is detected[15,16]. It may be useful for the reader to keep in mind that when the
signal  data  is  represented  in  two’s  complement  arithmetic,  multiple  summations  resulting  in
intermediate overflow errors cause no problems if we can guarantee that the final magnitude of the
sum of the numbers is not too large for the final accumulator register. Of course, standard floating-
point  number  formats  can  greatly  reduce  the  errors  associated  with  overflow  oscillations  and  limit
cycles[17]. (We discuss floating-point number formats in Chapter 12.)
These quantized coefficient and overflow errors, caused by finite-width words, have different effects
depending  on  the  IIR  filter  structure  used.  Referring  to  Figure 6-22,  practice  has  shown  the  Direct
Form II structure to be the most error-prone IIR filter implementation.
The most popular technique for minimizing the errors associated with finite-word-length widths is to
design IIR filters comprising a cascade string, or parallel combination, of low-order filters. The next
section tells us why.
6.8 Improving IIR Filters with Cascaded Structures
Filter  designers  minimize  IIR  filter  stability  and  quantization  noise  problems  in  high-performance
filters by implementing combinations of cascaded lower-performance filters. Before we consider this
design idea, let’s review several important issues regarding the behavior of combinations of multiple
filters.
6.8.1 Cascade and Parallel Filter Properties
Here  we  summarize  the  combined  behavior  of  linear  time-invariant  filters  (be  they  IIR  or  FIR)
connected in cascade and in parallel. As indicated in Figure 6-25(a), the resultant transfer function of
two cascaded filter transfer functions is the product of those functions, or
(6-46)
Figure 6-25 Combinations of two filters: (a) cascaded filters; (b) parallel filters.
with an overall frequency response of
(6-47)
It’s also important to know that the resultant impulse response of cascaded filters is
(6-48)
where “*” means convolution.
As shown in Figure 6-25(b), the combined transfer function of two filters connected in parallel is the
sum of their transfer functions, or
with an overall frequency response of
(6-49)
(6-50)
The resultant impulse response of parallel filters is the sum of their individual impulse responses, or
(6-51)
While  we  are  on  the  subject  of  cascaded  filters,  let’s  develop  a  rule  of  thumb  for  estimating  the
combined  passband  ripple  of  the  two  cascaded  filters  in  Figure  6-25(a).  The  cascaded  passband
ripple  is  a  function  of  each  individual  filter’s  passband  ripple.  If  we  represent  an  arbitrary  filter’s
peak passband ripple on a linear (not dB) vertical axis as shown in Figure 6-26, we can begin our
cascaded ripple estimation.
Figure 6-26 Definition of filter passband ripple R.
From Eq. (6-47), the upper bound (the peak) of a cascaded filter’s passband response, 1 + Rcas, is the
product of the two H1(ω) and H2(ω) filters’ peak passband responses, or
(6-52)
For small values of R1 and R2, the R1R2 term becomes negligible, and we state our rule of thumb as
(6-53)
Thus,  in  designs  using  two  cascaded  filters  it’s  prudent  to  specify  their  individual  passband  ripple
values to be roughly half the desired Rcas ripple specification for the final combined filter, or
(6-54)
6.8.2 Cascading IIR Filters
Experienced  filter  designers  routinely  partition  high-order  IIR  filters  into  a  string  of  2nd-order  IIR
filters arranged in cascade because these lower-order filters are easier to design, are less susceptible
to coefficient quantization errors and stability problems, and their implementations allow easier data
word scaling to reduce the potential overflow effects of data word size growth.
Optimizing  the  partitioning  of  a  high-order  filter  into  multiple  2nd-order  filter  sections  is  a
challenging task, however. For example, say we have the 6th-order Direct Form I filter in Figure 6-
27(a) that we want to partition into three 2nd-order sections. In factoring the 6th-order filter’s H(z)
transfer  function,  we  could  get  up  to  three  separate  sets  of  feedforward  coefficients  in  the  factored
H(z)  numerator:  b′(k),  b″(k),  and  b′′′(k).  Likewise,  we  could  have  up  to  three  separate  sets  of
feedback  coefficients  in  the  factored  denominator:  a′(k), a″(k),  and  a′′′(k).  Because  there  are  three
2nd-order sections, there are three factorial, or six, ways of pairing the sets of coefficients. Notice in
Figure 6-27(b) how the first section uses the a′(k) and b′(k) coefficients, and the second section uses
the a″(k) and b″(k) coefficients. We could just as well have interchanged the sets of coefficients so
the first 2nd-order section uses the a′(k) and b″(k) coefficients, and the second section uses the a″(k)
and b′(k)  coefficients.  So,  there  are  six  different  mathematically  equivalent  ways  of  combining  the
sets of coefficients. Add to this the fact that for each different combination of low-order sections there
are three factorial distinct ways those three separate 2nd-order sections can be arranged in cascade.
Figure 6-27 IIR filter partitioning: (a) initial 6th-order IIR filter; (b) three 2nd-order sections.
This means if we want to partition a 2M-order IIR filter into M distinct 2nd-order sections, there are
M factorial squared, (M!)2, ways to do so. As such, there are then (3!)2 = 24 different cascaded filters
we could obtain when going from Figure 6-27(a) to Figure 6-27(b). To further complicate this filter
partitioning problem, the errors due to coefficient quantization will, in general, be different for each
possible  filter  combination.  Although  full  details  of  this  subject  are  outside  the  scope  of  this
introductory text, ambitious readers can find further material on optimizing cascaded filter sections in
references [14] and [18], and in Part 3 of reference [19].
One  simple  (although  perhaps  not  optimum)  method  for  arranging  cascaded  2nd-order  sections  has
been proposed[18]. First, factor a high-order IIR filter’s H(z) transfer function into a ratio of the form
(6-55)
with  the  zk  zeros  in  the  numerator  and  pk  poles  in  the  denominator.  (Ideally  you  have  a  signal
processing software package to perform the factorization.) Next, the 2nd-order section assignments go
like this:
1. Find the pole, or pole pair, in H(z) closest to the unit circle.
2. Find the zero, or zero pair, closest to the pole, or pole pair, found in Step 1.
3. Combine those poles and zeros into a single 2nd-order filter section. This means your first 2nd-
order section may be something like
(6-56)
4. Repeat Steps 1 to 3 until all poles and zeros have been combined into 2nd-order sections.
5. The final ordering (cascaded sequence) of the sections is based on how far the sections’ poles
are from the unit circle. Order the sections in either increasing or decreasing pole distances from
the unit circle.
6. Implement your filter as cascaded 2nd-order sections in the order from Step 5.
In digital filter vernacular, a 2nd-order IIR filter is called a biquad for two reasons. First, the filter’s
z-domain  transfer  function  includes  two  quadratic  polynomials.  Second,  the  word  biquad  sounds
cool.
By the way, we started our 2nd-order sectioning discussion with a high-order Direct Form I filter in
Figure  6-27(a).  We  chose  that  filter  form  because  it’s  the  structure  most  resistant  to  coefficient
quantization and overflow problems. As seen in Figure 6-27(a), we have redundant delay elements.
These can be combined, as shown in Figure 6-28, to reduce our temporary storage requirements as
we did with the Direct Form II structure in Figure 6-22.
Figure 6-28 Cascaded Direct Form I filters with reduced temporary data storage.
There’s  much  material  in  the  literature  concerning  finite  word  effects  as  they  relate  to  digital  IIR
filters. (References [18], [20], and [21] discuss quantization noise effects in some detail as well as
providing extensive bibliographies on the subject.)
6.9 Scaling the Gain of IIR Filters
In order to impose limits on the magnitudes of data values within an IIR filter, we may wish to change
the passband gain of that filter[22,23].
For example, consider the 1st-order lowpass IIR filter in Figure 6-29(a) that has a DC gain (gain at
zero  Hz)  of  3.615.  (This  means  that,  just  as  with  FIR  filters,  the  sum  of  the  IIR  filter’s  impulse
response samples is equal to the DC gain of 3.615.)
Figure 6-29 Lowpass IIR filters: (a) DC gain = 3.615; (b) DC gain = 1.
The DC gain of an IIR filter is the sum of the filter’s feedforward coefficients divided by 1 minus the
sum  of  the  filter’s  feedback  coefficients.  (We  leave  the  proof  of  that  statement  as  a  homework
problem.) That is, the DC gain of the Figure 6-29(a) 1st-order filter is
(6-57)
Now let’s say we want, for some reason, the filter’s DC gain to be one (unity gain). This is easy to
accomplish. We merely divide the filter’s feedforward coefficients by the original DC gain as
(6-58)
Doing  so  gives  us  a  new  filter  whose  feedforward  coefficients  are  those  shown  in  Figure  6-29(b).
That new lowpass filter has a DC gain of one. Changing a filter’s coefficients in this way is called
filter scaling. Happily, this filter scaling does not change the shape of the original filter’s frequency
magnitude or phase response.
Likewise,  to  force  the  passband  gain  of  a  highpass  filter  to  be  unity,  we  divide  the  filter’s
feedforward  coefficients  by  the  original  filter’s  frequency  magnitude  response  at  fs/2  (half  the
sampling rate).
Unlike  passive  analog  (continuous-time)  filters  that  operate  by  attenuating  spectral  energy  in  their
stopbands, digital IIR filters operate by amplifying spectral energy in their passbands. Because of this
positive  passband  gain  behavior,  there  is  another  type  of  IIR  filter  scaling  that’s  used  in  many
situations. It is possible that an IIR filter may have a passband gain so high that the filter generates
internal sample values too large to be accommodated by the hardware, with its internal binary number
format,  used  to  implement  the  filter.  Stated  in  different  words,  it’s  possible  for  a  filter  to  generate
internal data values so large that they overflow the registers in which the data is to be stored. This
situation can also occur when multiple 2nd-order IIR filters are cascaded as discussed in Section 6.8.
In such cases, should we wish to reduce the passband gain of an IIR filter without changing the shape
of its frequency magnitude or phase responses, we can do so by implementing one of the filters shown
in Figure 6-30.
Figure 6-30 Scaled IIR filter structures: (a) Direct Form I; (b) Direct Form II.
If an IIR filter has an original passband gain of GIIR, we can change that passband gain by modifying
the  original  filter’s  coefficients  using  the  scalar  G1  and  G2  gain  factors  shown  in  Figure  6-30.
Changing a filter’s coefficients in this way is also called filter scaling. The passband gain of a scaled
filter is
(6-59)
The general philosophy in these matters is to choose factors G1 and G2 so that we preserve the filter’s
output  signal  quality  (called  the  signal-to-noise  ratio,  SNR,  as  discussed  in  Chapter  12  and
Appendix D) as much as possible. This means keeping all internal sample values as large as can be
accommodated by the filter hardware registers. The problem is, there’s no simple way to determine
the values of G1 and G2. The suggested procedure is to select one of the Figure 6-30 implementations
and  apply  the  expected  input  signal  to  the  filter.  Next  we  experiment  with  different  values  for  gain
factors  G1  and  G2  from  Eq.  (6-59)  until  the  final  filter  gain,  GIIR-scaled,  is  an  acceptable  value.
Following  that,  we  select  an  alternate  Figure  6-30  filter  structure  and  experiment  with  different
values for gains G1 and G2 to see if we can improve on the previous scaled-filter structure.
For computational efficiency reasons, if we’re able to set G2 to be the reciprocal of an integer power
of  two,  then  we  can  eliminate  one  of  the  multiplies  in  Figure  6-30.  That  is,  in  this  scenario  the
multiply  by  G2  operation  can  then  be  implemented  with  binary  right  shifts.  Then  again,  perhaps
factors G1 and G2  can  be  chosen  so  that  one  of  the  modified  filter  coefficients  is  unity  in  order  to
eliminate a multiply operation.
Now  that  we  have  some  understanding  of  the  performance  and  implementation  structures  of  IIR
filters,  let’s  briefly  introduce  three  filter  design  techniques.  These  IIR  design  methods  go  by  the
impressive names of impulse invariance, bilinear transform, and optimized methods. The first two
methods use analytical, pencil and paper algebra, filter design techniques to approximate continuous
analog filters. (By “analog filters” we mean those hardware filters made up of resistors, capacitors,
and perhaps operational amplifiers.)
Because analog filter design methods are very well understood, designers can take advantage of an
abundant variety of analog filter design techniques to design, say, a digital IIR Butterworth filter with
its very flat passband response, or perhaps go with a Chebyshev filter with its fluctuating passband
response and sharper passband-to-stopband cutoff characteristics. The optimized methods (by far the
most  popular  way  of  designing  IIR  filters)  comprise  linear  algebra  algorithms  available  in
commercial filter design software packages.
The  impulse  invariance,  bilinear  transform  filter  design  methods  are  somewhat  involved,  so  a  true
DSP beginner is justified in skipping those subjects upon first reading this book. However, those filter
design topics may well be valuable sometime in your future as your DSP knowledge, experience, and
challenges grow.
6.10 Impulse Invariance IIR Filter Design Method
The  impulse  invariance  method  of  IIR  filter  design  is  based  upon  the  notion  that  we  can  design  a
discrete filter whose time-domain impulse response is a sampled version of the impulse response of a
continuous  analog  filter.  If  that  analog  filter  (often  called  the  prototype  filter)  has  some  desired
frequency response, then our IIR filter will yield a discrete approximation of that desired response.
The impulse response equivalence of this design method is depicted in Figure 6-31, where we use the
conventional  notation  of  δ  to  represent  an  impulse  function  and  hc(t)  is  the  analog  filter’s  impulse
response. We use the subscript “c” in Figure 6-31(a) to emphasize the continuous nature of the analog
filter. Figure 6-31(b)  illustrates  the  definition  of  the  discrete  filter’s  impulse  response:  the  filter’s
time-domain output sequence when the input is a single unity-valued sample (impulse) preceded and
followed by all zero-valued samples. Our goal is to design a digital filter whose impulse response is
a sampled version of the analog filter’s continuous impulse response. Implied in the correspondence
of the continuous and discrete impulse responses is the property that we can map each pole on the s-
plane for the analog filter’s Hc(s) transfer function to a pole on the z-plane for the discrete IIR filter’s
H(z) transfer function. What designers have found is that the impulse invariance method does yield
useful  IIR  filters,  as  long  as  the  sampling  rate  is  high  relative  to  the  bandwidth  of  the  signal  to  be
filtered. In other words, IIR filters designed using the impulse invariance method are susceptible to
aliasing  problems  because  practical  analog  filters  cannot  be  perfectly  band-limited.  Aliasing  will
occur in an IIR filter’s frequency response as shown in Figure 6-32.
Figure 6-31 Impulse invariance design equivalence of (a) analog filter continuous impulse response;
(b) digital filter discrete impulse response.
Figure 6-32 Aliasing in the impulse invariance design method: (a) prototype analog filter magnitude
response; (b) replicated magnitude responses where HIIR(ω) is the discrete Fourier transform of h(n)
= hc(nts); (c) potential resultant IIR filter magnitude response with aliasing effects.
From what we’ve learned in Chapter 2 about the spectral replicating effects of sampling, if Figure 6-
32(a)  is  the  spectrum  of  the  continuous  hc(t)  impulse  response,  then  the  spectrum  of  the  discrete
hc(nts) sample sequence is the replicated spectra in Figure 6-32(b).
In Figure 6-32(c) we show the possible effect of aliasing where the dashed curve is a desired HIIR(ω)
frequency magnitude response. However, the actual frequency magnitude response, indicated by the
solid curve, can occur when we use the impulse invariance design method. For this reason, we prefer
to  make  the  sample  frequency  fs  as  large  as  possible  to  minimize  the  overlap  between  the  primary
frequency response curve and its replicated images spaced at multiples of ±fs Hz.
Due  to  the  aliasing  behavior  of  the  impulse  invariance  design  method,  this  filter  design  process
should  never  be  used  to  design  highpass  digital  filters.  To  see  how  aliasing  can  affect  IIR  filters
designed with this method, let’s list the necessary impulse invariance design steps and then go through
a lowpass filter design example.
There are two different methods for designing IIR filters using impulse invariance. The first method,
which we’ll call Method 1, requires that an inverse Laplace transform as well as a z-transform be
performed[24,25].  The  second  impulse  invariance  design  technique,  Method  2,  uses  a  direct
substitution  process  to  avoid  the  inverse  Laplace  and  z-transformations  at  the  expense  of  needing
partial  fraction  expansion  algebra  necessary  to  handle  polynomials[20,21,26,27].  Both  of  these
methods seem complicated when described in words, but they’re really not as difficult as they sound.
Let’s compare the two methods by listing the steps required for each of them. The impulse invariance
design Method 1 goes like this:
Method  1,  Step  1:  Design  (or  have  someone  design  for  you)  a  prototype  analog  filter  with  the
desired  frequency  response.†  The  result  of  this  step  is  a  continuous  Laplace  transfer
function Hc(s) expressed as the ratio of two polynomials, such as
† In a lowpass filter design, for example, the filter type (Chebyshev, Butterworth, elliptic), filter order (number of poles), and the cutoff
frequency are parameters to be defined in this step.
(6-60)
which is the general form of Eq. (6-10) with N < M, and a(k) and b(k) are constants.
Method 1, Step 2: Determine the analog filter’s continuous time-domain impulse response hc(t) from
the  Hc(s)  Laplace  transfer  function.  I  hope  this  can  be  done  using  Laplace  tables  as
opposed to actually evaluating an inverse Laplace transform equation.
Method  1,  Step  3:  Determine  the  digital  filter’s  sampling  frequency  fs,  and  calculate  the  sample
period as ts = 1/fs. The fs sampling rate is chosen based on the absolute frequency, in Hz,
of  the  prototype  analog  filter.  Because  of  the  aliasing  problems  associated  with  this
impulse invariance design method, later, we’ll see why fs should be made as large as is
practical.
Method  1,  Step  4:  Find  the  z-transform  of  the  continuous  hc(t)  to  obtain  the  IIR  filter’s  z-domain
transfer function H(z) in the form of a ratio of polynomials in z.
Method 1, Step 5: Substitute the value (not the variable) ts for the continuous variable t in the H(z)
transfer function obtained in Step 4. In performing this step, we are ensuring, as in Figure
6-31,  that  the  IIR  filter’s  discrete  h(n)  impulse  response  is  a  sampled  version  of  the
continuous filter’s hc(t) impulse response so that h(n) = hc(nts), for 0 ≤ n ≤ ∞.
Method 1, Step 6: Our H(z) from Step 5 will now be of the general form
(6-61)
Because the process of sampling the continuous impulse response results in a digital filter
frequency  response  that’s  scaled  by  a  factor  of  1/ts,  many  filter  designers  find  it
appropriate to include the ts factor in Eq. (6-61). So we can rewrite Eq. (6-61) as
(6-62)
Incorporating  the  value  of  ts  in  Eq.  (6-62),  then,  makes  the  IIR  filter  time-response
scaling independent of the sampling rate, and the discrete filter will have the same gain as
the prototype analog filter.†
†  Some  authors  have  chosen  to  include  the  ts  factor  in  the  discrete  h(n)  impulse  response  in  the  above  Step  4,  that  is,  make  h(n)  =
tshc(nts)[20, 28]. The final result of this, of course, is the same as that obtained by including ts as described in Step 6.
Method 1, Step 7: Because Eq. (6-61) is in the form of Eq. (6-25), by inspection, we can express the
filter’s time-domain difference equation in the general form of Eq. (6-21) as
(6-63)
Choosing to incorporate ts, as in Eq. (6-62), to make the digital filter’s gain equal to the
prototype analog filter’s gain by multiplying the b(k) coefficients by the sample period ts
leads to an IIR filter time-domain expression of the form
(6-64)
Notice  how  the  signs  changed  for  the  a(k)  coefficients  from  Eqs. (6-61)  and  (6-62)  to
Eqs.  (6-63)  and  (6-64).  These  sign  changes  always  seem  to  cause  problems  for
beginners, so watch out. Also, keep in mind that the time-domain expressions in Eqs. (6-
63) and (6-64) apply only to the filter structure in Figure 6-18. The a(k) and b(k), or ts ·
b(k), coefficients, however, can be applied to the improved IIR structure shown in Figure
6-22 to complete our design.
Before  we  go  through  an  actual  example  of  this  design  process,  let’s  discuss  the  other  impulse
invariance design method.
The  impulse  invariance  Design  Method  2,  also  called  the  standard  z-transform  method,  takes  a
different  approach.  It  mathematically  partitions  the  prototype  analog  filter  into  multiple  single-pole
continuous filters and then approximates each one of those by a single-pole digital filter. The set of M
single-pole digital filters is then algebraically combined to form an M-pole, Mth-ordered IIR filter.
This process of breaking the analog filter to discrete filter approximation into manageable pieces is
shown in Figure 6-33. The steps necessary to perform an impulse invariance Method 2 design are:
Figure 6-33 Mathematical flow of the impulse invariance design Method 2.
Method 2, Step 1: Obtain the Laplace transfer function Hc(s)  for  the  prototype  analog  filter  in  the
form of Eq. (6-60). (Same as Method 1, Step 1.)
Method 2, Step 2: Select an appropriate sampling frequency fs and calculate the sample period as ts
= 1/fs. (Same as Method 1, Step 3.)
Method 2, Step 3: Express the analog filter’s Laplace transfer function Hc(s) as the sum of single-
pole filters. This requires us to use partial fraction expansion methods to express the ratio
of polynomials in Eq. (6-60) in the form of
(6-65)
where M > N, the individual Ak factors are constants, and the kth pole is located at −pk
on the s-plane. We’ll denote the kth single-pole analog filter as Hk(s), or
(6-66)
Method 2, Step 4: Substitute 1 − e−pkts z−1 for s + pk in Eq. (6-65). This mapping of each Hk(s) pole,
located  at  s  =  −pk  on  the  s-plane,  to  the  z  =  e−pkts  location  on  the  z-plane  is  how  we
approximate  the  impulse  response  of  each  single-pole  analog  filter  by  a  single-pole
digital  filter.  (The  reader  can  find  the  derivation  of  this  1  −  e−pkts  z−1  substitution,
illustrated  in  our  Figure  6-33,  in  references  [20],  [21],  and  [26].)  So,  the  kth  analog
single-pole filter Hk(s)  is  approximated  by  a  single-pole  digital  filter  whose  z-domain
transfer function is
(6-67)
The  final  combined  discrete  filter  transfer  function  H(z)  is  the  sum  of  the  single-poled
discrete filters, or
(6-68)
Keep in mind that the above H(z) is not a function of time. The ts factor in Eq. (6-68) is a
constant equal to the discrete-time sample period.
Method 2, Step 5: Calculate the z-domain transfer function of the sum of the M single-pole digital
filters in the form of a ratio of two polynomials in z. Because the H(z) in Eq. (6-68) will
be  a  series  of  fractions,  we’ll  have  to  combine  those  fractions  over  a  common
denominator to get a single ratio of polynomials in the familiar form of
(6-69)
Method  2,  Step  6:  Just  as  in  Method  1,  Step  6,  by  inspection,  we  can  express  the  filter’s  time-
domain equation in the general form of
(6-70)
Again,  notice  the  a(k)  coefficient  sign  changes  from  Eq.  (6-69)  to  Eq.  (6-70).  As
described in Method 1, Steps 6 and 7, if we choose to make the digital filter’s gain equal
to  the  prototype  analog  filter’s  gain  by  multiplying  the  b(k)  coefficients  by  the  sample
period ts, then the IIR filter’s time-domain expression will be in the form
(6-71)
yielding a final H(z) z-domain transfer function of
(6-71′)
Finally, we can implement the improved IIR structure shown in Figure 6-22 using the a(k)
and b(k) coefficients from Eq. (6-70) or the a(k) and ts·b(k) coefficients from Eq. (6-71).
To provide a more meaningful comparison between the two impulse invariance design methods, let’s
dive in and go through an IIR filter design example using both methods.
6.10.1 Impulse Invariance Design Method 1 Example
Assume  that  we  need  to  design  an  IIR  filter  that  approximates  a  2nd-order  Chebyshev  prototype
analog lowpass filter whose passband ripple is 1 dB. Our fs sampling rate is 100 Hz (ts = 0.01), and
the  filter’s  1  dB  cutoff  frequency  is  20  Hz.  Our  prototype  analog  filter  will  have  a  frequency
magnitude response like that shown in Figure 6-34.
Figure 6-34 Frequency magnitude response of the example prototype analog filter.
Given the above filter requirements, assume that the analog prototype filter design effort results in the
Hc(s) Laplace transfer function of
(6-72)
It’s the transfer function in Eq. (6-72) that we intend to approximate with our discrete IIR filter. To
find  the  analog  filter’s  impulse  response,  we’d  like  to  get  Hc(s)  into  a  form  that  allows  us  to  use
Laplace  transform  tables  to  find  hc(t).  Searching  through  systems  analysis  textbooks,  we  find  the
following Laplace transform pair:
(6-73)
Our intent, then, is to modify Eq. (6-72) to get it into the form on the left side of Eq. (6-73). We do
this by realizing that the Laplace transform expression in Eq. (6-73) can be rewritten as
(6-74)
If we set Eq. (6-72) equal to the right side of Eq. (6-74), we can solve for A, α, and ω. Doing that,
Solving Eq. (6-75) for A, α, and ω, we first find
so
and
(6-75)
(6-76)
(6-77)
(6-78)
(6-79)
OK, we can now express Hc(s) in the desired form of the left side of Eq. (6-74) as
(6-80)
Using the Laplace transform pair in Eq. (6-73),  the  time-domain  impulse  response  of  the  prototype
analog filter becomes
(6-81)
OK, we’re ready to perform Method 1, Step 4, to determine the discrete IIR filter’s z-domain transfer
function H(z) by performing the z-transform of hc(t). Again, scanning through digital signal processing
textbooks  or  a  good  math  reference  book,  we  find  the  following  z-transform  pair  where  the  time-
domain expression is in the same form as Eq. (6-81)’s hc(t) impulse response:
(6-82)
Remember, now, the α and ω in Eq. (6-82) are generic and are not related to the α and ω values in
Eqs. (6-76) and (6-78). Substituting the constants from Eq. (6-81) into the right side of Eq. (6-82), we
get the z-transform of the IIR filter as
(6-83)
Performing Method 1, Step 5, we substitute the ts value of 0.01 for the continuous variable t in Eq. (6-
83), yielding the final H(z) transfer function of
(6-84)
OK,  hang  in  there;  we’re  almost  finished.  Here  are  the  final  steps  of  Method  1.  Because  of  the
transfer function H(z) = Y(z)/X(z), we can cross-multiply the denominators to rewrite the bottom line
of Eq. (6-84) as
(6-85)
By inspection of Eq. (6-85), we can now get the time-domain expression for our IIR filter. Performing
Method 1, Steps 6 and 7, we multiply the x(n−1) coefficient by the sample period value of ts = 0.01
to allow for proper scaling as
(6-86)
and  there  we  (finally)  are.  The  coefficients  from  Eq. (6-86)  are  what  we  use  in  implementing  the
improved IIR structure shown in Figure 6-22 to approximate the original 2nd-order Chebyshev analog
lowpass filter.
Let’s see if we get the same result if we use the impulse invariance design Method 2 to approximate
the example prototype analog filter.
6.10.2 Impulse Invariance Design Method 2 Example
Given the original prototype filter’s Laplace transfer function as
(6-87)
and the value of ts = 0.01 for the sample period, we’re ready to proceed with Method 2’s Step 3. To
express Hc(s) as the sum of single-pole filters, we’ll have to factor the denominator of Eq. (6-87) and
use partial fraction expansion methods. For convenience, let’s start by replacing the constants in Eq.
(6-87) with variables in the form of
(6-88)
where  b  =  137.94536,  and  c  =  17410.145.  Next,  using  Eq.  (6-15)  with  a  =  1,  we  can  factor  the
quadratic denominator of Eq. (6-88) into
(6-89)
If we substitute the values for b and c in Eq. (6-89), we’ll find that the quantity under the radical sign
is negative. This means that the factors in the denominator of Eq. (6-89) are complex. Because we
have lots of algebra ahead of us, let’s replace the radicals in Eq. (6-89) with the imaginary term jR,
where
 and R = |(b2−4c)/4|, such that
(6-90)
OK, partial fraction expansion methods allow us to partition Eq. (6-90) into two separate fractions of
the form
(6-91)
where the K1 constant can be found to be equal to jc/2R and constant K2 is the complex conjugate of
K1, or K2 = −jc/2R. (To learn the details of partial fraction expansion methods, the interested reader
should investigate standard college algebra or engineering mathematics textbooks.) Thus, Hc(s) can
be of the form in Eq. (6-65) or
(6-92)
We  can  see  from  Eq. (6-92)  that  our  2nd-order  prototype  filter  has  two  poles,  one  located  at  p1  =
−b/2 − jR and the other at p2 = −b/2 + jR. We’re now ready to map those two poles from the s-plane
to the z-plane as called out in Method 2, Step 4. Making our 1 − e−pkts z−1 substitution for the s + pk
terms in Eq. (6-92), we have the following expression for the z-domain single-pole digital filters:
(6-93)
Our objective in Method 2, Step 5, is to massage Eq. (6-93) into the form of Eq. (6-69), so that we
can determine the IIR filter’s feedforward and feedback coefficients. Putting both fractions in Eq. (6-
93) over a common denominator gives us
(6-94)
Collecting like terms in the numerator and multiplying out the denominator gives us
(6-95)
Factoring the exponentials and collecting like terms of powers of z in Eq. (6-95),
(6-96)
Continuing to simplify our H(z) expression by factoring out the real part of the exponentials,
(6-97)
We now have H(z) in a form with all the like powers of z combined into single terms, and Eq. (6-97)
looks  something  like  the  desired  form  of  Eq. (6-69).  Knowing  that  the  final  coefficients  of  our  IIR
filter must be real numbers, the question is “What do we do with those imaginary j terms in Eq. (6-
97)?” Once again, Euler to the rescue.† Using Euler’s equations for sinusoids, we can eliminate the
imaginary exponentials and Eq. (6-97) becomes
† From Euler, we know that sin(ø) = (ejø − e−jø)/2j, and cos(ø) = (ejø + e−jø)/2.
(6-98)
If we plug the values c = 17410.145, b = 137.94536, R = 112.48517, and ts = 0.01 into Eq. (6-98),
we get the following IIR filter transfer function:
(6-99)
Because  the  transfer  function  H(z)  =  Y(z)/X(z),  we  can  again  cross-multiply  the  denominators  to
rewrite Eq. (6-99) as
(6-100)
Now we take the inverse z-transform of Eq. (6-100), by inspection, to get the time-domain expression
for our IIR filter as
(6-101)
One final step remains. To force the IIR filter gain to be equal to the prototype analog filter’s gain, we
multiply the x(n−1) coefficient by the sample period ts as suggested in Method 2, Step 6. In this case,
there’s only one x(n) coefficient, giving us
(6-102)
That compares well with the Method 1 result in Eq. (6-86). (Isn’t it comforting to work a problem
two different ways and get the same result?)
Figure  6-35  shows,  in  graphical  form,  the  result  of  our  IIR  design  example.  The  s-plane  pole
locations of the prototype filter and the z-plane poles of the IIR filter are shown in Figure 6-35(a).
Because the s-plane poles are to the left of the origin and the z-plane poles are inside the unit circle,
both the prototype analog and the discrete IIR filters are stable. We find the prototype filter’s s-plane
pole locations by evaluating Hc(s) in Eq. (6-92). When s = −b/2 − jR,  the  denominator  of  the  first
term in Eq. (6-92) becomes zero and Hc(s) is infinitely large. That s = −b/2 − jR value is the location
of the lower s-plane pole in Figure 6-35(a). When s = −b/2 + jR, the denominator of the second term
in Eq. (6-92) becomes zero and s = −b/2 + jR is the location of the second s-plane pole.
Figure 6-35 Impulse invariance design example filter characteristics: (a) s-plane pole locations of
prototype analog filter and z-plane pole locations of discrete IIR filter; (b) frequency magnitude
response of the discrete IIR filter.
The IIR filter’s z-plane pole locations are found from Eq. (6-93). If we multiply the numerators and
denominators of Eq. (6-93) by z,
(6-103)
In Eq. (6-103), when z is set equal to e(−b/2  +  jR)ts, the denominator of the first term in Eq.  (6-103)
becomes zero and H(z) becomes infinitely large. The value of z of
(6-104)
defines  the  location  of  the  lower  z-plane  pole  in  Figure  6-35(a).  Specifically,  this  lower  pole  is
located at a distance of e−bts/2 = 0.5017 from the origin, at an angle of θ = −Rts radians, or −64.45°.
Being conjugate poles, the upper z-plane pole is located the same distance from the origin at an angle
of θ = Rts radians, or +64.45°. Figure 6-35(b) illustrates the frequency magnitude response of the IIR
filter in Hz.
Two  different  implementations  of  our  IIR  filter  are  shown  in  Figure  6-36.  Figure  6-36(a)  is  an
implementation  of  our  2nd-order  IIR  filter  based  on  the  general  IIR  structure  given  in  Figure  6-22,
and Figure 6-36(b)  shows  the  2nd-order  IIR  filter  implementation  based  on  the  alternate  structure
from Figure 6-21(b). Knowing that the b(0) coefficient on the left side of Figure 6-36(b) is zero, we
arrive at the simplified structure on the right side of Figure 6-36(b). Looking carefully at Figure 6-
36(a) and the right side of Figure 6-36(b), we can see that they are equivalent.
Figure 6-36 Implementations of the impulse invariance design example filter.
the
two  reasons:  (1)
Although both impulse invariance design methods are covered in the literature, we might ask, “Which
one is preferred?” There’s no definite answer to that question because it depends on the Hc(s) of the
prototype analog filter. Although our Method 2 example above required more algebra than Method 1,
if the prototype filter’s s-domain poles were located only on the real axis, Method 2 would have been
much simpler because there would be no complex variables to manipulate. In general, Method 2 is
more  popular  for
inverse  Laplace  and  z-transformations,  although
straightforward in our Method 1 example, can be very difficult for higher-order filters, and (2) unlike
Method 1, Method 2 can be coded in a software routine or a computer spreadsheet.
Upon examining the frequency magnitude response in Figure 6-35(b), we can see that this 2nd-order
IIR  filter’s  roll-off  is  not  particularly  steep.  This  is,  admittedly,  a  simple  low-order  filter,  but  its
attenuation slope is so gradual that it doesn’t appear to be of much use as a lowpass filter.† We can
also  see  that  the  filter’s  passband  ripple  is  greater  than  the  desired  value  of  1  dB  in  Figure  6-34.
What we’ll find is that it’s not the low order of the filter that contributes to its poor performance, but
the sampling rate used. That 2nd-order IIR filter response is repeated as the shaded curve in Figure 6-
37.  If  we  increased  the  sampling  rate  to  200  Hz,  we’d  get  the  frequency  response  shown  by  the
dashed curve in Figure 6-37.  Increasing  the  sampling  rate  to  400  Hz  results  in  the  much  improved
frequency response indicated by the solid line in the figure. Sampling rate changes do not affect our
filter order or implementation structure. Remember, if we change the sampling rate, only the sample
period ts changes in our design equations, resulting in a different set of filter coefficients for each new
sampling  rate.  So  we  can  see  that  the  smaller  we  make ts  (larger  fs),  the  better  the  resulting  filter
when  either  impulse  invariance  design  method  is  used  because  the  replicated  spectral  overlap
indicated in Figure 6-32(b) is reduced due to the larger fs sampling rate. The bottom line here is that
impulse invariance IIR filter design techniques are most appropriate for narrowband filters, that is,
lowpass filters whose cutoff frequencies are much smaller than the sampling rate.
† A piece of advice: whenever you encounter any frequency representation (be it a digital filter magnitude response or a signal spectrum)
that has nonzero values at +fs/2, be suspicious—be very suspicious—that aliasing is taking place.
Figure 6-37 IIR filter frequency magnitude response, on a linear scale, at three separate sampling
rates. Notice how the filter’s absolute cutoff frequency of 20 Hz shifts relative to the different fs
sampling rates.
The  second  analytical  technique  for  analog  filter  approximation,  the  bilinear  transform  method,
alleviates  the  impulse  invariance  method’s  aliasing  problems  at  the  expense  of  what’s  called
frequency warping. Specifically, there’s a nonlinear distortion between the prototype analog filter’s
frequency  scale  and  the  frequency  scale  of  the  approximating  IIR  filter  designed  using  the  bilinear
transform. Let’s see why.
6.11 Bilinear Transform IIR Filter Design Method
There’s a popular analytical IIR filter design technique known as the bilinear transform method. Like
the impulse invariance method, this design technique approximates a prototype analog filter defined
by  the  continuous  Laplace  transfer  function  Hc(s)  with  a  discrete  filter  whose  transfer  function  is
H(z). However, the bilinear transform method has great utility because
• it allows us simply to substitute a function of z for s in Hc(s) to get H(z), thankfully eliminating the
need  for  Laplace  and  z-transformations  as  well  as  any  necessity  for  partial  fraction  expansion
algebra;
• it maps the entire s-plane to the z-plane,  enabling  us  to  completely  avoid  the  frequency-domain
aliasing problems we had with the impulse invariance design method; and
• it induces a nonlinear distortion of H(z)’s frequency axis, relative to the original prototype analog
filter’s frequency axis, that sharpens the final roll-off of digital lowpass filters.
Don’t worry. We’ll explain each one of these characteristics and see exactly what they mean to us as
we go about designing an IIR filter.
If the transfer function of a prototype analog filter is Hc(s), then we can obtain the discrete IIR filter z-
domain transfer function H(z) by substituting the following for s in Hc(s)
(6-105)
where, again, ts is the discrete filter’s sampling period (1/fs). Just as in the impulse invariance design
method, when using the bilinear transform method, we’re interested in where the analog filter’s poles
end up on the z-plane after the transformation. This s-plane to z-plane mapping behavior is exactly
what makes the bilinear transform such an attractive design technique.†
† The bilinear transform is a technique in the theory of complex variables for mapping a function on the complex plane of one variable to
the complex plane of another variable. It maps circles and straight lines to straight lines and circles, respectively.
Let’s investigate the major characteristics of the bilinear transform’s s-plane to z-plane mapping. First
we’ll show that any pole on the left side of the s-plane will map to the inside of the unit circle in the
z-plane. It’s easy to show this by solving Eq. (6-105) for z in terms of s. Multiplying Eq. (6-105) by
(ts/2)(1 + z−1) and collecting like terms of z leads us to
(6-106)
If we designate the real and imaginary parts of s as
(6-107)
where the subscript in the radian frequency ωa signifies analog, Eq. (6-106) becomes
(6-108)
We see in Eq. (6-108) that z is complex, comprising the ratio of two complex expressions. As such, if
we denote z as a magnitude at an angle in the form of z = |z|∠θz, we know that the magnitude of z is
given by
(6-109)
OK, if σ is negative (σ < 0), the numerator of the ratio on the right side of Eq. (6-109) will be less
than  the  denominator,  and  |z|  will  be  less  than  1.  On  the  other  hand,  if  σ  is  positive  (σ  >  0),  the
numerator will be larger than the denominator, and |z| will be greater than 1. This confirms that when
using the bilinear transform defined by Eq. (6-105), any pole located on the left side of the s-plane (σ
< 0) will map to a z-plane location inside the unit circle. This characteristic ensures that any stable s-
plane pole of a prototype analog filter will map to a stable z-plane pole for our discrete IIR filter.
Likewise, any analog filter pole located on the right side of the s-plane (σ > 0) will map to a z-plane
location outside the unit circle when using the bilinear transform. This reinforces our notion that to
avoid  filter  instability,  during  IIR  filter  design,  we  should  avoid  allowing  any  z-plane  poles  to  lie
outside the unit circle.
Next, let’s show that the jωa axis of the s-plane maps to the perimeter of the unit circle in the z-plane.
We can do this by setting σ = 0 in Eq. (6-108) to get
(6-110)
Here, again, we see in Eq. (6-110) that z is a complex number comprising the ratio of two complex
numbers, and we know the magnitude of this z is given by
(6-111)
The magnitude of z in Eq. (6-111) is always 1. So, as we stated, when using the bilinear transform,
the  jωa  axis  of  the  s-plane  maps  to  the  perimeter  of  the  unit  circle  in  the  z-plane.  However,  this
frequency  mapping  from  the  s-plane  to  the  unit  circle  in  the  z-plane  is  not  linear.  It’s  important  to
know why this frequency nonlinearity, or warping, occurs and to understand its effects. So we shall,
by  showing  the  relationship  between  the  s-plane  frequency  and  the  z-plane  frequency  that  we’ll
designate as ωd.
If we define z on the unit circle in polar form as z = re−jωd as we did for Figure 6-13, where r is 1 and
ωd is the angle, we can substitute z = ejωd in Eq. (6-105) as
(6-112)
If we show s in its rectangular form and partition the ratio in brackets into half-angle expressions,
(6-113)
Using Euler’s relationships of sin(ø) = (ejø − e−jø)/2j and cos(ø) = (ejø + e−jø)/2, we can convert the
right side of Eq. (6-113) to rectangular form as
(6-114)
If we now equate the real and imaginary parts of Eq. (6-114), we see that σ = 0, and
(6-115)
The analog frequency ωa (radians/second) can have any value and its equivalent fa cyclic frequency
is
(6-115′)
Rearranging Eq. (6-115) to give us the useful expression for the z-domain frequency ωd, in terms of
the s-domain frequency ωa, we write
(6-116)
It’s  critical  to  notice  that  the  range  of  ωd  is  ±π,  and  the  dimensions  of  digital  frequency  ωd  are
radians/sample (not radians/second). The important relationship in Eq. (6-116), which accounts for
the so-called frequency warping due  to  the  bilinear  transform,  is  illustrated  in  Figure 6-38.  Notice
that  because  tan−1(ωats/2)  approaches  π/2  as  ωa  becomes  large,  ωd  must  then  approach  twice  that
value, or π. This means that no matter how large the s-plane’s analog ωa becomes, the z-plane’s ωd
will never be greater than π radians/sample (fs/2 Hz).
Figure 6-38 Nonlinear relationship between the z-domain frequency ωd and the s-domain frequency
ωa.
Remember  how  we  considered  Figure 6-14  and  stated  that  only  the  −π  fs  to  +π  fs  radians/second
frequency range for ωa can be accounted for on the z-plane? Well, our new mapping from the bilinear
transform maps the entire s-plane to the z-plane, and not just the primary strip of the s-plane shown in
Figure 6-14. Now, just as a walk along the jωa frequency axis on the s-plane takes us to infinity in
either direction, a trip halfway around the unit circle in a counterclockwise direction takes us from ωa
= 0 to ωa = +∞ radians/second. As such, the bilinear transform maps the s-plane’s entire jωa axis onto
the unit circle in the z-plane. We illustrate these bilinear transform mapping properties in Figure 6-39.
Figure 6-39 Bilinear transform mapping of the s-plane to the z-plane.
In an attempt to show the practical implications of this frequency warping, let’s relate the s-plane and
z-plane frequencies to a more practical measure of frequencies in Hz. Because a ωd frequency of ωd =
π  radians/sample  corresponds  to  a  cyclic  frequency  of  fs/2  Hz,  we  relate  ωd  and  a  digital  cyclic
frequency fd using
(6-117)
Substituting Eq. (6-117) into Eq. (6-115), and recalling that ωa = 2πfa, gives us
Solving Eq. (6-118) for fd yields
(6-118)
(6-119)
Equation (6-119) is plotted in Figure 6-40(a). Equations (6-118) and (6-119) are very useful! They
relate the analog s-plane frequency fa in Hz to the digital z-plane’s warped frequency fd in Hz. This
important  nonlinear  relationship  is  plotted  in  Figure  6-40(b).  There  we  see  that  the  fd  frequency
warping (compression) becomes more severe as fd approaches fs/2.
Figure 6-40 Nonlinear relationship between the fd and fa frequencies: (a) frequency warping curve;
(b) s-domain frequency response transformation to a z-domain frequency response; (c) example
|Ha(fa)| and transformed |Hd(fd)|.
So  what  does  all  this  fa  to  fd  mapping  rigmarole  mean?  It  means  two  things.  First,  if  a  bandpass
analog filter’s upper cutoff frequency is fa1 Hz, a bilinear-transform-designed digital bandpass filter
operating at a sample rate of fs Hz will have an upper cutoff frequency of fd1 Hz as shown in Figure 6-
40(c). Likewise if a bilinear-transform-designed digital bandpass filter is desired to have an upper
cutoff  frequency  of  fd1  Hz,  then  the  original  prototype  analog  bandpass  filter  must  be  designed
(prewarped) to have an upper cutoff frequency of fa1 Hz using Eq. (6-118).
Second,  no  IIR  filter  response  aliasing  can  occur  with  the  bilinear  transform  design  method.  No
matter  what  the  shape,  or  bandwidth,  of  the  |Ha(fa)|  prototype  analog  filter,  none  of  the  |Hd(fd)|
magnitude responses can extend beyond half the sampling rate of fs/2 Hz—and that’s what makes the
bilinear transform IIR filter design method as popular as it is.
The  steps  necessary  to  perform  an  IIR  filter  design  using  the  bilinear  transform  method  are  as
follows:
Step 1: Obtain the Laplace transfer function Hc(s) for the prototype analog filter in the form of Eq.
(6-43).
Step  2:  Determine  the  digital  filter’s  equivalent  sampling  frequency  fs  and  establish  the  sample
period ts = 1/fs.
Step 3: In the Laplace Hc(s) transfer function, substitute the expression
(6-120)
for the variable s to get the IIR filter’s H(z) transfer function.
Step 4: Multiply the numerator and denominator of H(z) by the appropriate power of (1 + z−1) and
grind through the algebra to collect terms of like powers of z in the form
(6-121)
Step  5:  Just  as  in  the  impulse  invariance  design  methods,  by  inspection,  we  can  express  the  IIR
filter’s time-domain equation in the general form of
(6-122)
Although  the  expression  in  Eq.  (6-122)  only  applies  to  the  filter  structure  in  Figure  6-18,  to
complete our design, we can apply the a(k) and b(k) coefficients to the improved IIR structure
shown in Figure 6-22.
To show just how straightforward the bilinear transform design method is, let’s use it to solve the IIR
filter design problem first presented for the impulse invariance design method.
6.11.1 Bilinear Transform Design Example
Again, our goal is to design an IIR filter that approximates the 2nd-order Chebyshev prototype analog
lowpass filter, shown in Figure 6-26, whose passband ripple is 1 dB. The fs sampling rate is 100 Hz
(ts = 0.01), and the filter’s 1 dB cutoff frequency is 20 Hz. As before, given the original prototype
filter’s Laplace transfer function as
(6-123)
and  the  value  of  ts  =  0.01  for  the  sample  period,  we’re  ready  to  proceed  with  Step  3.  For
convenience, let’s replace the constants in Eq. (6-123) with variables in the form of
(6-124)
where b = 137.94536 and c = 17410.145. Performing the substitution of Eq. (6-120) in Eq. (6-124),
(6-125)
To simplify our algebra a little, let’s substitute the variable a for the fraction 2/ts to give
(6-126)
Proceeding with Step 4, we multiply Eq. (109)’s numerator and denominator by (1 + z−1)2 to yield
(6-127)
Multiplying through by the factors in the denominator of Eq. (6-127), and collecting like powers of z,
(6-128)
We’re almost there. To get Eq. (6-128) into the form of Eq. (6-121) with a constant term of one in the
denominator, we divide Eq. (6-128)’s numerator and denominator by (a2 + ab + c), giving us
(6-129)
We now have H(z) in a form with all the like powers of z combined into single terms, and Eq. (6-129)
looks  something  like  the  desired  form  of  Eq.  (6-121).  If  we  plug  the  values  a  =  2/ts  =  200,  b  =
137.94536, and c = 17410.145 into Eq. (6-129), we get the following IIR filter transfer function:
(6-130)
and there we are. Now, by inspection of Eq. (6-130), we get the time-domain expression for our IIR
filter as
(6-131)
The frequency magnitude response of our bilinear transform IIR design example is shown as the dark
curve in Figure 6-41(a), where, for comparison, we’ve shown the result of that impulse invariance
design example as the shaded curve. Notice how the bilinear-transform-designed filter’s magnitude
response approaches zero at the folding frequency of fs/2 = 50 Hz. This is as it should be—that’s the
whole purpose of the bilinear transform design method. Figure 6-41(b) illustrates the nonlinear phase
response of the bilinear-transform-designed IIR filter.
Figure 6-41 Comparison of the bilinear transform and impulse invariance design IIR filters: (a)
frequency magnitude responses; (b) phase of the bilinear transform IIR filter.
We might be tempted to think that not only is the bilinear transform design method easier to perform
than the impulse invariance design method, but that it gives us a much sharper roll-off for our lowpass
filter. Well, the frequency warping of the bilinear transform method does compress (sharpen) the roll-
off portion of a lowpass filter, as we saw in Figure 6-40, but an additional reason for the improved
response is the price we pay in terms of the additional complexity of the implementation of our IIR
filter. We see this by examining the implementation of our IIR filter as shown in Figure 6-42. Notice
that our new filter requires five multiplications per filter output sample where the impulse invariance
design  filter  in  Figure  6-28(a)  required  only  three  multiplications  per  filter  output  sample.  The
additional  multiplications  are,  of  course,  required  by  the  additional  feedforward  z  terms  in  the
numerator of Eq. (6-130). These added b(k) coefficient terms in the H(z) transfer function correspond
to zeros in the z-plane created by the bilinear transform that did not occur in the impulse invariance
design method.
Figure 6-42 Implementation of the bilinear transform design example filter.
Because our example prototype analog lowpass filter had a cutoff frequency that was fs/5, we don’t
see a great deal of frequency warping in the bilinear transform curve in Figure 6-41. (In fact, Kaiser
has shown that when fs is large, the impulse invariance and bilinear transform design methods result
in  essentially  identical  H(z)  transfer  functions[18].)  Had  our  cutoff  frequency  been  a  larger
percentage of fs, bilinear transform warping would have been more serious, and our resultant |Hd(fd)|
cutoff  frequency  would  have  been  below  the  desired  value.  What  the  pros  do  to  avoid  this  is  to
prewarp  the  prototype  analog  filter’s  cutoff  frequency  requirement  before  the  analog  Hc(s)  transfer
function is derived in Step 1.
In that way, they compensate for the bilinear transform’s frequency warping before it happens. We can
use Eq. (6-115) to determine the prewarped prototype analog filter lowpass cutoff frequency that we
want mapped to the desired IIR lowpass cutoff frequency. We plug the desired IIR cutoff frequency ωd
in  Eq.  (6-115)  to  calculate  the  prototype  analog  ωa  cutoff  frequency  used  to  derive  the  prototype
analog filter’s Hc(s) transfer function.
Although  we  explained  how  the  bilinear  transform  design  method  avoids  the  impulse  invariance
method’s inherent frequency response aliasing, it’s important to remember that we still have to avoid
filter  input  data  aliasing.  No  matter  what  kind  of  digital  filter  or  filter  design  method  is  used,  the
original input signal data must always be obtained using a sampling scheme that avoids the aliasing
described in Chapter 2. If the original input data contains errors due to sample rate aliasing, no filter
can remove those errors.
is  available
text  such  as
this,  more  detailed  coverage
Our  introductions  to  the  impulse  invariance  and  bilinear  transform  design  techniques  have,  by
necessity, presented only the essentials of those two design methods. Although rigorous mathematical
treatment  of  the  impulse  invariance  and  bilinear  transform  design  methods  is  inappropriate  for  an
interested
introductory
reader[20,21,25,26]. References [25] and [26],  by  the  way,  have  excellent  material  on  the  various
prototype analog filter types used as a basis for the analytical IIR filter design methods. Although our
examples  of  IIR  filter  design  using  the  impulse  invariance  and  bilinear  transform  techniques
approximated analog lowpass filters, it’s important to remember that these techniques apply equally
well to designing bandpass and highpass IIR filters. To design a highpass IIR filter, for example, we’d
merely start our design with a Laplace transfer function for the prototype analog highpass filter. Our
IIR digital filter design would then proceed to approximate that prototype highpass filter.
As we have seen, the impulse invariance and bilinear transform design techniques are both powerful
and a bit difficult to perform. The mathematics is intricate and the evaluation of the design equations
is arduous for all but the simplest filters. As such, we’ll introduce a third class of IIR filter design
methods  based  on  software  routines  that  take  advantage  of  iterative  optimization  computing
the
to
techniques. In this case, the designer defines the desired filter frequency response, and the algorithm
begins generating successive approximations until the IIR filter coefficients converge (ideally) to an
optimized design.
6.12 Optimized IIR Filter Design Method
The  final  class  of  IIR  filter  design  methods  we’ll  introduce  is  broadly  categorized  as  optimization
methods.  These  IIR  filter  design  techniques  were  developed  for  the  situation  when  the  desired  IIR
filter  frequency  response  was  not  of  the  standard  lowpass,  bandpass,  or  highpass  form.  When  the
desired response has an arbitrary shape, closed-form expressions for the filter’s z-transform do not
exist, and we have no explicit equations to work with to determine the IIR filter’s coefficients. For
this general IIR filter design problem, algorithms were developed to solve sets of linear, or nonlinear,
equations on a computer. These software routines mandate that the designer describe, in some way,
the desired IIR filter frequency response. The algorithms, then, assume a filter transfer function H(z)
as a ratio of polynomials in z and start to calculate the filter’s frequency response. Based on some
error criteria, the algorithm begins iteratively adjusting the filter’s coefficients to minimize the error
between the desired and the actual filter frequency response. The process ends when the error cannot
be  further  minimized,  or  a  predefined  number  of  iterations  has  occurred,  and  the  final  filter
coefficients  are  presented  to  the  filter  designer.  Although  these  optimization  algorithms  are  too
mathematically  complex  to  cover  in  any  detail  here,  descriptions  of  the  most  popular  optimization
schemes are readily available in the literature [20,21,29–34].
The reader may ask, “If we’re not going to cover optimization methods in any detail, why introduce
the subject here at all?” The answer is that if we spend much time designing IIR filters, we’ll end up
using  optimization  techniques  in  the  form  of  computer  software  routines  most  of  the  time.  The  vast
majority of commercially available digital signal processing software packages include one or more
IIR  filter  design  routines  that  are  based  on  optimization  methods.  When  a  computer-aided  design
technique is available, filter designers are inclined to use it to design the simpler lowpass, bandpass,
or highpass forms even though analytical techniques exist. With all due respect to Laplace, Heaviside,
and  Kaiser,  why  plow  through  all  the  z-transform  design  equations  when  the  desired  frequency
response can be applied to a software routine to yield acceptable filter coefficients in a few seconds?
As  it  turns  out,  using  commercially  available  optimized  IIR  filter  design  routines  is  very
straightforward.  Although  they  come  in  several  flavors,  most  optimization  routines  only  require  the
designer to specify a few key amplitude and frequency values, and the desired order of the IIR filter
(the  number  of  feedback  taps),  and  the  software  computes  the  final  feedforward  and  feedback
coefficients. In specifying a lowpass IIR filter, for example, a software design routine might require
us to specify the values for δp, δs, f1, and f2 shown in Figure 6-43. Some optimization design routines
require  the  user  to  specify  the  order  of  the  IIR  filter.  Those  routines  then  compute  the  filter
coefficients that best approach the required frequency response. Some software routines, on the other
hand, don’t require the user to specify the filter order. They compute the minimum order of the filter
that actually meets the desired frequency response.
Figure 6-43 Example lowpass IIR filter design parameters required for the optimized IIR filter
design method.
6.13 A Brief Comparison of IIR and FIR Filters
The  question  naturally  arises  as  to  which  filter  type,  IIR  or  FIR,  is  best  suited  for  a  given  digital
filtering application. That’s not an easy question to answer, but we can point out a few factors that
should be kept in mind. First, we can assume that the differences in the ease of design between the
two filter types are unimportant. There are usually more important performance and implementation
properties  to  consider  than  design  difficulty  when  choosing  between  an  IIR  and  an  FIR  filter.  One
design  consideration  that  may  be  significant  is  the  IIR  filter’s  ability  to  simulate  a  predefined
prototype analog filter. FIR filters do not have this design flexibility.
From a hardware standpoint, with so many fundamental differences between IIR and FIR filters, our
choice  must  be  based  on  those  filter  characteristics  that  are  most  and  least  important  to  us.  For
example, if we need a filter with exactly linear phase, then an FIR filter is the only way to go. If, on
the other hand, our design requires a filter to accept very high data rates and slight phase nonlinearity
is tolerable, we might lean toward IIR filters with their reduced number of necessary multipliers per
output sample.
One caveat, though: Just because an FIR filter has, say, three times the number of multiplies per output
sample relative to an IIR filter, that does not mean the IIR filter will execute faster on a programmable
DSP chip. Typical DSP chips have a zero-overhead looping capability whose parallelism speeds the
execution  of  multiply  and  accumulate  (MAC)  routines,  with  which  FIR  filtering  is  included.  The
code for IIR filtering has more data/coefficient pointer bookkeeping to accommodate than FIR filter
code. So, if you’re choosing between an IIR filter requiring K  multiplies  per  output  sample  and  an
FIR  filter  needing  2K  (or  3K)  multiplies  per  output  sample,  code  both  filters  and  measure  their
execution speeds.
Table  6-1  presents  a  brief  comparison  of  IIR  and  FIR  filters  based  on  several  performance  and
implementation properties.
Table 6-1 IIR and Nonrecursive FIR Filter Characteristics Comparison
References
[1] Churchill, R. V. Modern Operational Mathematics in Engineering,  McGraw-Hill,  New  York,
1944, pp. 307–334.
[2] Aseltine, J. A. Transform Method in Linear System Analysis, McGraw-Hill, New York, 1958,
pp. 287–292.
[3]  Nixon,  F.  E.  Handbook  of  Laplace  Transformation:  Tables  and  Examples,  Prentice  Hall,
Englewood Cliffs, New Jersey, 1960.
[4] Kaiser, J. F. “Digital Filters,” in System Analysis by Digital Computer, ed. by F. F. Kuo and J. F.
Kaiser, John Wiley and Sons, New York, 1966, pp. 218–277.
[5]  Kaiser,  J.  F.  “Design  Methods  for  Sampled  Data  Filters,”  Proc.  First  Annual  Allerton
Conference on Circuit and System Theory, 1963, Chapter 7, pp. 221–236.
[6] Ragazzini, J. R., and Franklin, G. F. Sampled-Data Control Systems, McGraw-Hill, New York,
1958, pp. 52–83.
[7] Milne-Thomson, L. M. The Calculus of Finite Differences, Macmillan, London, 1951, pp. 232–
251.
[8] Truxal, J. G. Automatic Feedback Control System Synthesis, McGraw-Hill, New York, 1955, p.
283.
[9]  Blackman,  R.  B.  Linear  Data-Smoothing  and  Prediction  in  Theory  and  Practice,  Addison
Wesley, Reading, Massachusetts, 1965, pp. 81–84.
[10] Oppenheim, A., Schafer, R., and Buck, J. Discrete-Time Signal Processing, 2nd ed., Prentice
Hall, Upper Saddle River, New Jersey, 1999, pp. 306–307.
[11] Gold, B., and Jordan, K. L., Jr. “A Note on Digital Filter Synthesis,” Proceedings of the IEEE,
Vol. 56, October 1968, p. 1717.
[12] Rabiner, L. R., et al. “Terminology in Digital Signal Processing,” IEEE  Trans.  on  Audio  and
Electroacoustics, Vol. AU-20, No. 5, December 1972, p. 327.
[13] Jackson, L. B. “On the Interaction of Roundoff Noise and Dynamic Range and Dynamic Range in
Digital Filters,” Bell System Technical Journal, Vol. 49, February 1970, pp. 159–184.
[14] Jackson, L. B. “Roundoff Noise Analysis for Fixed-Point Digital Filters Realized in Cascade or
Parallel Form,” IEEE Trans. Audio Electroacoustics, Vol. AU-18, June 1970, pp. 107–122.
[15] Sandberg, I. W. “A Theorem Concerning Limit Cycles in Digital Filters,” Proc. Seventh Annual
Allerton Conference on Circuit and System Theory, Monticello, Illinois, October 1969.
[16] Ebert, P. M., et al. “Overflow Oscillations in Digital Filters,” Bell System Technical Journal,
Vol. 48, November 1969, pp. 2999–3020.
[17] Oppenheim, A. V. “Realization of Digital Filters Using Block Floating Point Arithmetic,” IEEE
Trans. Audio Electroacoustics, Vol. AU-18, June 1970, pp. 130–136.
[18] Kaiser, J. F. “Some Practical Considerations in the Realization of Linear Digital Filters,” Proc.
Third Annual Allerton Conference on Circuit and System Theory, 1965, pp. 621–633.
[19] Rabiner, L. R., and Rader, C. M., eds. Digital Signal Processing, IEEE Press, New York, 1972,
p. 361.
[20]  Oppenheim,  A.  V.,  and  Schafer,  R.  W.  Discrete  Time  Signal  Processing,  Prentice  Hall,
Englewood Cliffs, New Jersey, 1989, p. 406.
[21]  Rabiner,  L.  R.,  and  Gold,  B.  Theory  and  Application  of  Digital  Signal  Processing,  Prentice
Hall, Englewood Cliffs, New Jersey, 1975, p. 216.
[22]  Grover,  D.  “Subject:  Re:  How  to  Arrange  the  (Gain,  Pole,  Zero)  of  the  Cascaded  Biquad
Filter.” Usenet group comp.dsp post, December 28, 2000.
[23] Grover, D., and Deller, J. Digital Signal Processing and the Microcontroller,  Prentice  Hall,
Upper Saddle River, New Jersey, 1998.
[24] Stearns, S. D. Digital Signal Analysis, Hayden Book Co., Rochelle Park, New Jersey, 1975, p.
114.
[25] Stanley, W. D., et al. Digital Signal Processing, Reston Publishing Co., Reston, Virginia, 1984,
p. 191.
[26] Williams, C. S. Designing Digital Filters, Prentice Hall, Englewood Cliffs, New Jersey, 1986,
pp. 166–186.
[27] Johnson, M. “Implement Stable IIR Filters Using Minimal Hardware,” EDN, April 14, 1983.
[28]  Oppenheim,  A.  V.,  Willsky,  A.  S.,  and  Young,  I.  T.  Signals  and  Systems,  Prentice  Hall,
Englewood Cliffs, New Jersey, 1983, p. 659.
[29] Deczky, A. G. “Synthesis of Digital Recursive Filters Using the Minimum P Error Criterion,”
IEEE Trans. on Audio and Electroacoustics, Vol. AU-20, No. 2, October 1972, p. 257.
[30] Steiglitz, K. “Computer-Aided Design of Recursive Digital Filters,” IEEE Trans. on Audio and
Electroacoustics, Vol. 18, No. 2, 1970, p. 123.
[31] Richards, M. A. “Application of Deczky’s Program for Recursive Filter Design to the Design of
Recursive Decimators,” IEEE  Trans.  on  Acoustics,  Speech,  and  Signal  Processing,  Vol.  ASSP-
30, October 1982, p. 811.
[32] Parks, T. W., and Burrus, C. S. Digital Filter Design, John Wiley and Sons, New York, 1987, p.
244.
[33] Rabiner, L., Graham, Y., and Helms, H. “Linear Programming Design of IIR Digital Filters with
Arbitrary Magnitude Functions,” IEEE Trans. on Acoustics, Speech, and Signal Processing., Vol.
ASSP-22, No. 2, April 1974, p. 117.
[34]  Friedlander,  B.,  and  Porat,  B.  “The  Modified  Yule-Walker  Method  of  ARMA  Spectral
Estimation,” IEEE Trans. on Aerospace Electronic Systems, Vol. AES-20, No. 2, March 1984, pp.
158–173.
Chapter 6 Problems
6.1  Review  the  z-plane  depiction  in  Figure  P6-1.  Draw  a  rough  sketch  of  the  Laplace  s-plane
showing a shaded area (on the s-plane) that corresponds to the shaded circular band in Figure P6-
1.
Figure P6-1
6.2  Write  the  H(z)  z-domain  transfer  function  equations  for  the  filters  described  by  the  following
difference equations:
(a) y(n) = x(n) − y(n−2),
(b) y(n) = x(n) + 3x(n−1) + 2x(n−2) − y(n−3),
(c) y(n) = x(n) + x(n−1) + x(n−3) + x(n−4) − y(n−2).
6.3  Knowing  the  order  of  a  digital  filter  is  important  information.  It  typically  gives  us  a  direct
indication  of  the  computational  workload  (number  of  additions  and  multiplications)  necessary  to
compute a single filter output sample. State the order of the filters in Problem 6.2.
6.4 Write the H(ω) frequency response equations, in both polar and rectangular form, for the filters in
Problem 6.2. By “polar form” we mean we want H(ω) expressed as a ratio of terms using e−jkω,
where k is an integer. By “rectangular form” we mean we want H(ω) expressed as a ratio in the
form of
where a, b, c, and d are cosine and/or sine functions whose arguments are kω.
(Note:  This  problem  is  not  “busy  work.”  The  rectangular  form  of  H(ω)  is  the  expression  you
would model using generic signal processing software to compute and plot a filter’s magnitude and
phase response in the frequency domain.)
6.5 Considering the z-domain transfer function associated with a digital filter:
(a) What does it mean if the filter has one or more poles outside the z-plane’s unit circle?
(b) What does it mean if the filter has a zero lying exactly on the z-plane’s unit circle?
6.6 In the literature of DSP, we usually see filter transfer functions expressed in terms of z where z
always  has  a  negative  exponent.  But  sometimes  we  see  transfer  functions  in  terms  of  z  having
positive exponents. For example, you might encounter an IIR filter’s transfer function expressed as
(a) What is the transfer function expression equivalent to H(z) in terms of z with z having negative-
only exponents?
(b) Is this IIR filter stable? Justify your answer.
(c) Draw the Direct Form I structure (block diagram), showing the filter’s coefficients.
(d) Draw the Direct Form II structure, showing the filter’s coefficients.
6.7 Although we didn’t need to use the z-transform  to  analyze  the  tapped-delay  line  (nonrecursive)
FIR filters in Chapter 5, we could have done so. Let’s try an FIR filter analysis example using the
z-transform. For the filter in Figure P6-7:
Figure P6-7
(a) Write the time-domain difference equation describing the filter output y(n) in terms of the x(n)
input and the h(k) coefficients.
(b) Write the z-transform of the y(n) difference equation from Part (a).
(c) Write the z-domain transfer function, H(z) = Y(z)/X(z), of the filter.
(d) What is the order of this FIR filter?
6.8 Thinking about IIR digital filters:
(a)  Is  it  true  that  to  determine  the  frequency  response  of  an  IIR  filter,  we  need  to  know  both  the
filter’s  time-domain  difference  equation  and  the  impulse  response  of  that  filter?  Explain  your
answer.
(b) If we know the H(z) z-domain transfer function equation for a digital filter, what must we do to
determine the frequency response of that filter?
6.9  Draw  the  Direct  Form  I  and  the  Direct  Form  II  block  diagrams  of  the  filter  represented  by  the
following z-domain transfer function:
6.10 Consider the two filters in Figure P6-10. (Notice the minus sign at the first adder in Figure P6-
10(b).) Determine whether each filter is an IIR or an FIR filter. Justify your answers.
Figure P6-10
6.11 The author once read a design document describing how an engineer was tasked to implement
Network A in Figure P6-11(a),  using  a  programmable  DSP  chip,  as  part  of  a  specialized  digital
filter. The engineer suggested that, due to the chip’s internal architecture, for computational speed
reasons Network B shown in Figure P6-11(b) should be used instead of Network A. He also stated
that the frequency magnitude responses of the two networks are identical. Is that last statement true?
Justify your answer.
Figure P6-11
6.12 Prove that the z-plane pole locations for the two filters in Figure P6-12 are identical.
Figure P6-12
Hint: For Filter #2, write two different equations for U(z) and set those equations equal to each
other.
6.13  The  discrete-sequence  output  of  commercial  analog-to-digital  (A/D)  converters  is  often
contaminated with a DC bias (a constant-level amplitude offset). Stated in different words, even
though the converter’s analog x(t) input signal’s average value is zero, the converter’s x(n) output
sequence  may  have  a  small  nonzero  average.  As  such,  depending  on  the  application,  A/D
converters  are  sometimes  followed  by  an  IIR  filter  shown  in  Figure P6-13  that  removes  the  DC
bias level from the filter’s x(n) input sequence. (The coefficient A is a positive value slightly less
than unity.)
(a) Derive the z-domain transfer function of the DC bias removal filter.
(b) Prove that the filter has a z-plane zero at z = 1, yielding the desired infinite attenuation at the
cyclic frequency of zero Hz.
(c) Draw the block diagram of the Direct Form II version of the DC bias removal filter.
Figure P6-13
6.14  Assume  we  have  the  software  code  to  implement  a  notch  filter  (a  filter  that  attenuates  a  very
narrow band of frequencies and passes frequencies that are above and below the notch’s ωc center
frequency),  and  the  software  documentation  states  the  filter  is  defined  by  the  following  transfer
function:
(a) If R = 0.9, draw the locations of the notch filter’s poles and zeros on the z-plane in relation to
the notch frequency ωc.
(b) Let’s say we’re processing the signal from a photodiode light sensor in our laboratory and our
signal’s time samples are arriving at a sample rate of fs = 1.8 kHz. Assume that 120 Hz flicker
noise from fluorescent lights is contaminating our photodiode output signal. What would be the
correct  value  for  ωc  to  use  in  the  notch  filter  code  to  attenuate  the  120  Hz  noise?  Show  your
work.
6.15 Show that for a 2nd-order FIR filter, whose z-domain transfer function is
H(z) = 1 + Bz−1 + z−2,
the sum of the locations of the filter’s two z-plane zeros is equal to −B.
6.16 Consider the filter in Figure P6-16.
(a) Determine the z-domain transfer function, H(z) = Y(z)/X(z), of the filter.
(b) Draw the z-plane pole/zero diagram of the filter.
(c) Using the notion of pole-zero cancellation, draw the block diagram of an exact equivalent, but
simpler, filter having fewer multipliers than shown in Figure P6-16.
Figure P6-16
6.17  Assume  we  have  a  digital  filter  (having  real-valued  coefficients)  whose  complex  frequency
response is the product of an M(ω) magnitude response and a θ(ω) phase response as
H(ω) = M(ω)ejθ(ω)
where  ω  is  a  normalized  frequency  variable  (in  the  range  of  −π  to  π,  corresponding  to  a  cyclic
frequency range of −fs/2 to fs/2 Hz) measured in radians/sample. Is it possible to have such a real-
coefficient filter whose θ(ω) phase response is of the form
where C is a nonzero constant? Explain your answer.
θ(ω) = C
6.18 Determine the H(z) transfer function of the recursive network in Figure P6-18.
Figure P6-18
6.19  The  recursive  networks  (networks  with  feedback)  that  we  discussed  in  this  chapter,  if  they’re
simple enough, can be analyzed with pencil and paper. This problem gives us practice in such an
analysis and prompts us to recall the process of converting a geometric series into a closed-form
equation.
(a) Looking at the discrete network in Figure P6-19, show that the y(n) output is equal to D/Q for
large  values  of  time  index  n when the x(n)  input  samples  have  a  constant  amplitude  of  D.  (To
keep the system stable, assume that Q is a positive number less than one and the network is “at
rest” at time n = 0. That is, w(0) = 0.)
Figure P6-19
Hint:  Write  equations  for  y(n)  when  n  =  0,  1,  2,  3,  .  .  .  etc.,  and  develop  a  general  series
expression for the y(n) output in terms of D, Q, and n. Next, use Appendix B to obtain a closed-
form (no summation sign) expression for the y(n) when n is a large number.
(b) When we arrive at a solution to a problem, it’s reassuring to verify (double-check) that solution
using a different technique. Following this advice, determine the z-domain H(z) transfer function
of  the  network  in  Figure  P6-19  and  show  that  its  zero  Hz  (DC)  gain  is  1/Q,  verifying  your
solution to Part (a) of this problem.
(c) Prove that the recursive network is stable if Q is in the range 0 < Q ≤ 1.
6.20  A  discrete  system  that  has  at  least  one  pole  on  the  z-plane’s  unit  circle  is  called  a  discrete
resonator,  such  as  the  system  in  Figure  P6-20.  Such  resonators  have  impulse  responses  that
oscillate indefinitely.
Figure P6-20
(a) Draw the z-plane pole/zero diagram of the resonator in the figure.
(b)  At  what  frequency,  measured  in  terms  of  the  x(n)  input  fs  sample  rate,  does  the  pole  of  this
system reside?
(c) Draw the time-domain impulse response of the system in Figure P6-20.
(d) Comment on how the frequency of the oscillating impulse response output samples relates to the
system’s pole location on the z-plane.
6.21  Given  the  following  six  difference  equations  for  various  digital  filters,  determine  which
equation  is  associated  with  which  |H?(f)|  filter  frequency  magnitude  response  in  Figure  P6-21.
Justify your answers.
Figure P6-21
6.22 A standard 2nd-order IIR filter (a biquad) is shown in its Direct Form I structure in Figure P6-
22.  Knowing  the  DC  gain  (the  value  H(ω)  at  ω  =  0  radians/sample)  of  a  filter  is  critical
information when we implement filtering using binary arithmetic. What is the DC gain of the filter
in terms of the filter’s coefficients?
Figure P6-22
6.23 Review the brief description of allpass filters in Appendix F.
(a) Prove that the 1st-order allpass filter, defined by the following Hap(z) transfer function, has an
|Hap(ω)| frequency magnitude response that is unity over its full operating frequency range of −π ≤
ω ≤ π radians/sample (−fs/2 ≤ f ≤ fs/2 Hz):
Variable K is a real-valued scalar constant.
Hint:  Rather  than  prove  |Hap(ω)|  =  1  for  all  ω,  prove  that  the  frequency  magnitude  response
squared, |Hap(ω)|2, is equal to unity for all ω.
(b) Draw the Direct Form I and Direct Form II block diagrams of the H(z) allpass filter.
(c) Explain why the Hap(z) allpass filter can never have a transfer function zero on its z-plane’s unit
circle.
6.24 A simple 1st-order IIR filter, whose z-domain transfer function is
has been proposed for use in synthesizing (simulating) guitar music. Is the Hg(z) filter a lowpass or
a  highpass  filter?  Justify  your  answer.  [Karjalainen,  M.,  et  al.  “Towards  High-Quality  Sound
Synthesis  of  the  Guitar  and  String  Instruments,”  International  Computer  Music  Conference,
September 10–15, 1993, Tokyo, Japan.]
6.25 There are general 2nd-order recursive networks used in practice, such as that shown in Figure
P6-25,  where  the  a(0)  coefficient  is  not  unity.  Assuming  you  need  to  analyze  such  a  network,
determine its z-domain transfer function that includes the a(0) coefficient. Show your steps.
Figure P6-25
6.26 Consider the recursive highpass filter shown in Figure P6-26(a).
(a) Derive the H(ω) frequency response equation for the filter.
(b) What is the location of the filter’s single z-plane pole?
(c) The |H(ω)| frequency magnitude response of the filter is shown in Figure P6-26(b). What are the
values of magnitudes M0 and Mπ? Show your work.
Figure P6-26
6.27 The recursive network shown in Figure P6-27 can be used to compute the N-point average of N
input  samples.  Although  this  process  works  well,  it  has  the  disadvantage  that  as  time  index  n
(where n = 1, 2, 3, 4, ...) increases, it requires the real-time computation of both the 1/n and (n−1)n
coefficients upon the arrival of each new x(n) input sample.
Figure P6-27
(a)  A  clever  DSP  engineer  always  tries  to  minimize  computations.  Show  how  to  modify  the
network’s diagram so that the real-time coefficient-computation workload is reduced.
(b)  Our  N-point  averager  network  has  a  feedback  loop,  with  possible  stability  problems.  Show
how your solution to Part (a) of this problem is a stable network as n increases starting at n = 1.
6.28 Given the z-plane pole/zero plot, associated with a 2nd-order IIR digital filter, in Figure P6-28:
(a) What is the H(z) transfer function, in terms of z−1 and z−2, of the Figure P6-28 filter having two
poles and a single zero on the z-plane? Show how you arrived at your answer.
(b) Draw the Direct Form I block diagram of the H(z) filter that implements the transfer function
arrived at in Part (a) of this problem.
(c) Draw a new block diagram of the H(z) filter that eliminates one of the multipliers in the Direct
Form I block diagram.
Figure P6-28
6.29  In  the  text’s  Section 6.5  we  learned  to  derive  a  filter  transfer  function  based  on  knowing  the
locations of the filter’s poles and zeros. We implied that the roots of polynomial P,
are equal to the roots of polynomial Q, where
P = z2 + bz + c,
Q = GP = Gz2 + Gbz + Gc,
with variable G being a real-valued constant. Prove that the roots of P are indeed equal to the roots
of Q.
6.30 Given the z-plane pole/zero plots in Figure P6-30, associated with the H(z) transfer functions of
four digital filters, draw a rough sketch of the four filters’ frequency magnitude responses over the
frequency range of −fs/2 to fs/2, where fs is the filter’s input signal sample rate.
Figure P6-30
Note: The two poles, near z = 1 in Figure P6-30(c), are lying exactly on top of two zeros.
6.31  Assume  that  you  must  implement  the  lowpass  H(z)  filter  shown  in  Figure  P6-31.  Good  DSP
engineers  always  simplify  their  digital  networks  whenever  possible.  Show  a  simplified  block
diagram  of  the  filter,  without  changing  the  filter’s  frequency  response,  that  has  a  reduced
computational workload and reduced data storage (number of delay elements).
Figure P6-31
Hint: Study the filter’s z-plane pole/zero diagram.
6.32 In Chapter 5 we had a homework problem whose solution revealed that the 3-tap FIR notch filter
in Figure P6-32(a) has complex conjugate z-plane zeros on the unit circle as shown in Figure P6-
32(b).  That  efficient  filter,  useful  for  attenuating  narrowband  noise  located  at  a  normalized
frequency of ωn (−π ≤ ωn ≤ π), has a frequency magnitude response shown in Figure P6-32(c). If
we  want  the  FIR  filter’s  stopband  notches  to  be  narrower,  we  can  implement  the  2nd-order  IIR
filter shown in Figure P6-32(d) that has conjugate z-plane poles at a radius of R just inside the unit
circle  as  shown  in  Figure P6-32(e).  The  frequency  magnitude  response  of  the  IIR  notch  filter  is
given in Figure P6-32(f).  Here’s  the  problem:  Express  the  Figure P6-32(d)  IIR  filter’s  a(1)  and
a(2) coefficients, in terms of ωn and R, that will place the z-plane poles as shown in Figure P6-
32(e). Show your work.
Figure P6-32
Hint: Recall Euler’s identity: 2cos(θ) = (ejθ + e−jθ).
6.33 Let’s exercise our IIR filter analysis skills. Suppose your colleague proposes the 2nd-order IIR
filter shown in Figure P6-33(a) to provide narrow passband filtering as shown in Figure P6-33(b).
(The |H(ω)| frequency axis uses the discrete-signal frequency variable ω (radians/sample) with ω =
π corresponding to a cyclic frequency of fs/2 Hz.)
Figure P6-33
(a) Is this 2nd-order IIR filter unconditionally stable?
(b) Over what range of negative values of the A coefficient will the filter be stable?
(c) For what negative value of A will the filter be conditionally stable (at least one pole on, and no
poles outside, the unit circle)?
(d) What is the resonant frequency (positive frequency) of the filter in terms of the fs sample rate
(in Hz) of the x(n) input?
Hint: If the z-plane’s positive-frequency pole is near the unit circle, think about how the angle of
that pole is related to the filter’s resonant frequency measured in Hz.
6.34 Think about a 4th-order (5-tap) tapped-delay line finite impulse response (FIR) filter whose z-
domain transfer function is
H(z) = b0 + b1z−1 + b2z−2+ b3z−3+ b4z−4.
Assume the filter has real-valued bk coefficients and that the filter is a linear-phase filter. If one of
the filter’s z-plane  zeros  has  a  value  of  z0  =  0.5657  +  j0.5657,  what  are  the  values  of  the  other
three z-plane zeros of this filter?
6.35  Here’s  an  interesting  problem.  As  of  this  writing,  in  an  application  note  on  their  website
(www.zilog.com), the skilled folks at Zilog Inc. describe a multistage digital bandpass filter used
to detect the pitch (frequency) of a musical tone. A two-stage Direct Form II version, where each
stage  is  a  2nd-order  IIR  filter,  of  this  detection  system  is  the  cascaded  bandpass  filter  shown  in
Figure  P6-35(a).  The  frequency  magnitude  responses  of  the  first  and  second  filters,  over  the
positive frequency range, are provided in Figure P6-35(b), and the combined (cascaded) frequency
magnitude response is provided in Figure P6-35(c).
Figure P6-35
(a) Given that the sample rate of the signal is fs = 8000 samples/second, what musical note will the
Figure P6-35(a) two-stage bandpass filter detect? That is, what musical note is closest to the fc
center frequency of the two-stage filter’s passband in Figure P6-35(c)? Explain how you arrived
at  your  answer.  For  your  convenience,  the  frequencies  of  several  musical  notes  of  an  equal-
tempered scale are provided in the following table.
Table P6-1 Musical Note Frequencies
(b) Finally, are the two 2nd-order IIR filters stable? Explain how you arrived at your answer.
6.36 Consider the Direct Form II IIR filter shown in Figure P6-36, which requires three multiplies
per  filter  output  sample.  Smart  DSP  engineers  reduce  computations  wherever  possible.  Draw  a
block diagram of a filter equivalent to that in Figure P6-36 that requires fewer than three multiplies
per filter output sample.
Figure P6-36
6.37 In high-speed, hardware-only, linear-phase filtering, the transposed structure of a tapped-delay
line FIR filter is often preferred over a traditional tapped-delay line FIR filter. That’s because the
parallel structure of transposed FIR filters reduces the time required to perform multiple addition
operations.  Draw  the  transposed  structure  of  the  traditional  FIR  filter  in  Figure  P6-37.  In  your
solution, make sure the x(n) input is on the left-hand side.
Figure P6-37
6.38 Draw the transposed structures of the networks in Figure P6-38. In your solutions, make sure the
x(n) inputs are on the left-hand side.
Figure P6-38
6.39 In the text we discussed the problem of limit cycles in IIR filters when finite-precision values
(finite  binary  word  width)  are  used  to  represent  data  values.  To  reiterate  that  concept,  the  unit-
sample impulse response of the 1st-order IIR filter in Figure P6-39(a) is shown in Figure P6-39(c).
That impulse response was computed using the very high precision of a 64-bit floating-point binary
number system within the filter. In Figure P6-39(c) we see that this stable IIR filter’s y(n) impulse
response properly decays toward zero amplitude as time advances.
Figure P6-39
In fixed-point binary filter implementations, if rounding is used to limit the binary word width (the
precision of data sample values) at the output of the filter’s adder, the ill effects of limit cycles may
occur. This rounding operation is shown in Figure P6-39(b) where the y(n) output is rounded to a
value that is a multiple of a rounding precision factor whose value is q. If rounding to the nearest q
= 0.1 value is implemented, the filter’s impulse response exhibits unwanted limit cycles as shown
in Figure P6-39(d), where the y(n) impulse response continually oscillates between ±0.1 as time
advances.
(a) Reducing the value of the rounding precision factor q is supposed to help reduce the level of the
unwanted limit cycle oscillations. Plot the unit-sample impulse response of the quantizing filter in
Figure P6-39(b) when q = 0.05.
Note: If an A(n) data value is exactly between two multiples of q, round away from zero.
(b) Comparing Figure P6-39(c), Figure P6-39(d), and your solution from the above Part (a), make
a  statement  regarding  how  the  peak-to-peak  amplitude  of  the  quantizing  filter’s  limit  cycle
behavior is related to the value of the rounding precision factor q.
6.40  Given  the  h1(k)  and  h2(k)  impulse  responses  of  the  two  filters  in  Figure  P6-40,  what  is  the
impulse response of the hCas(k) cascaded combination filter?
Figure P6-40
6.41  Here’s  a  problem  whose  solution  may,  someday,  be  useful  to  the  reader.  Many  commercial
digital  filter  design  software  packages  require  the  user  to  specify  a  desired  filter’s  maximum
passband  ripple,  in  terms  of  a  linear  peak deviation  parameter  represented  by  R,  for  a  lowpass
filter magnitude response in Figure P6-41.
Figure P6-41
(a)  Let’s  say  that  in  a  lowpass  filter  design  effort,  we  only  know  the  desired  passband  ripple
specified in terms of a peak-peak logarithmic (dB) parameter P shown in Figure P6-41. If P = 2
dB, what is R? Stated in different words, if we only have the P = 2 dB desired passband ripple
value available to us, what R value must we specify in our filter design software? Show how you
arrived at your solution.
(b) Given your solution to the above Part (a), now derive a general equation that defines the linear
R deviation parameter in terms of the logarithmic (dB) peak-peak passband ripple parameter P.
6.42  Many  digital  filters  are  implemented  as  both  cascaded  (series)  and  parallel  combinations  of
subfilters. Given the four individual Hk(ω) subfilter frequency responses in Figure P6-42, what is
the equation for the overall frequency response of this combination of subfilters in terms of H1(ω),
H2(ω), H3(ω), and H4(ω)?
Figure P6-42
6.43  Many  feedback  systems  can  be  reduced  to  the  form  of  the  generic feedback  system  shown  in
Figure P6-43(a).
Figure P6-43
(a)  Prove  that  the  z-domain  transfer  function  of  the  feedback  system  in  Figure  P6-43(a)  is  the
following expression:
Note:  The  above  H(z)  expression  is  well  known,  particularly  in  the  field  of  digital  control
systems, because it is encountered so often in practice.
(b)  If  we  replace  the  z  variable  in  H(z)  with  ejω,  we  obtain  an  H(ω)  equation,  describing  the
frequency response of the system in Figure P6-43(a), whose generic form is
(Notice  that  we  don’t  use  the  ejω  term,  for  example  H(ejω),  in  our  notation  for  a  frequency
response.  We  use  the  standard  H(ω)  notation  instead.)  With  the  above  H(ω)  equation  in  mind,
what is the expression for the H(ω) frequency response of the system shown in Figure P6-43(b)?
Hint:  Use  the  principles  of  cascaded  and  parallel  subsystems  to  obtain  a  simplified  network
structure.
6.44 In the text we discussed the analysis of digital filters comprising the parallel combination of two
subfilters.  Using  a  highpass  filter  whose  impulse  response  is  the  hHigh(k)  samples  in  Figure  P6-
44(a), we can implement a lowpass filter if we’re able to build a parallel network whose impulse
response  is  the  hPar(k)  samples  in  Figure  P6-44(b).  The  parallel  network’s  hPar(k)  samples  are
defined by
Figure P6-44
(a)  If  the  parallel  lowpass  filter  network  is  that  shown  in  Figure P6-44(c),  what  is  the  impulse
response of the h(k) subfilter?
(b)  Draw  the  parallel  lowpass  filter  network  showing  what  processing  elements  are  in  the  h(k)
subfilter block.
6.45 Assume we are given the lowpass filter shown in Figure P6-45 and, based on the IIR discussion
in the text’s Section 6.9, we must scale the filter to reduce its passband gain without changing its
frequency response shape. Draw a block diagram of the scaled filter.
Figure P6-45
6.46 You’re working on a project to upgrade an analog temperature-sensing and processing system.
Your job is to design a digital integrator, to replace an analog integrator whose Laplace s-domain
transfer function is
that will receive a new temperature sample once every 2 seconds. Because ideal integrators have a
frequency  magnitude  response  of  zero  at  high  frequencies,  your  digital  integrator  must  have  a
frequency magnitude response less than 0.01 at fs/2.
(a)  What  is  the  z-domain  transfer  function  of  a  digital  integrator  replacement  for  H(s)  designed
using the impulse invariance Method 2 design technique?
(b)  What  is  the  z-domain  transfer  function  of  a  digital  integrator  designed  using  the  bilinear
transform design technique?
(c) Verify that each of your digital integrators has a z-plane pole at the same frequency at which the
H(s) analog integrator had an s-plane pole.
(d) Which of the two digital integrators, from Part (a) and Part (b), will you submit as your final
design, and why?
6.47 Due to its simplicity, the 1st-order analog lowpass filter shown in Figure P6-47(a) is often used
to  attenuate  high-frequency  noise  in  a  vin(t)  input  signal  voltage.  This  lowpass  filter’s  s-domain
transfer function is
Figure P6-47
(a)  Determine  a  digital  filter’s  Hii(z)  z-domain  transfer  function  that  simulates  H(s),  using  the
impulse  invariance  Method  2  process.  Draw  the  digital  filter’s  Direct  Form  II  block  diagram
(structure) where the coefficients are in terms of R and C. For simplicity, assume that ts = 1.
(b)  Determine  a  digital  filter’s  Hbt(z)  z-domain  transfer  function  that  simulates  H(s),  using  the
bilinear  transform  process.  Draw  the  digital  filter’s  Direct  Form  II  block  diagram  where  the
coefficients are in terms of R and C. Again, assume that ts = 1.
(c) When properly designed, the filters’ normalized frequency magnitude responses, |Hii| and |Hbt|,
are those shown in Figure P6-47(b) (plotted on a logarithmic vertical scale). Why does the |Hbt|
response have such large attenuation at high frequencies?
6.48 A 1st-order analog highpass filter’s s-domain transfer function is
Determine a digital filter’s H(z) z-domain transfer function that simulates H(s) using the bilinear
transform process. Given that frequency ωo = 62.832 radians/second, assume that the digital filter’s
sample  rate  is  fs  =  100  Hz.  Manipulate  your  final  H(z)  expression  so  that  it  is  in  the  following
form:
where A, B, and C are constants. The above H(z) form enables convenient modeling of the digital
filter’s transfer function using commercial signal processing software.
6.49  Let’s  plow  through  the  algebra  to  design  a  2nd-order  digital  IIR  filter  that  approximates  an
analog lowpass filter. Assume the filter’s s-domain transfer function is
and the digital filter’s sample rate is 1000 samples/second. Derive, using the bilinear transform,
the  z-domain  transfer  function  equation  of  the  discrete  filter  that  simulates  the  above  H(s)
continuous lowpass filter.
6.50  Let’s  say  that  your  colleague  has  designed  a  prototype  analog  lowpass  filter  whose  cutoff
frequency  is  3.8  kHz.  (By  “cutoff  frequency”  we  mean  the  frequency  where  the  lowpass  filter’s
magnitude response is 3 dB below its average passband magnitude response.) Next, assume your
colleague  wants  you  to  use  the  bilinear  transform  method  to  design  a  digital  filter  whose
performance is equivalent to that of the analog filter when the sample rate is fs = 11 kHz.
(a) Given that the analog lowpass filter’s fa cutoff frequency is 3.8 kHz, what will be the fd cutoff
frequency of the digital lowpass filter in Hz?
(b)  Given  that  we  want  the  digital  lowpass  filter’s  cutoff  frequency  to  be  exactly  3.8  kHz,  the
prototype analog filter will have to be redesigned. What should be the fa cutoff frequency of the
new analog lowpass filter?
