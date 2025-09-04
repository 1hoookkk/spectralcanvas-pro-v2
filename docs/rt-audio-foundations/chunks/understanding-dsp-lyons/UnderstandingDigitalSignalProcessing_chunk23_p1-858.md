# Understanding DSP (Lyons) — Chunk 23/27

> Source: `UnderstandingDigitalSignalProcessing.pdf` · Pages ~1-858 · Extractor: pdfminer.six
> Target ≈ 2800 words, Overlap = 200 words · Actual = 4599

> Note: DSP textbook content for SpectralCanvasPro RT-audio knowledge base

---
Appendix E. Decibels (dB and dBm)
This  appendix  introduces  the  logarithmic  function  used  to  improve  the  magnitude  resolution  of
frequency-domain  plots  when  we  evaluate  signal  spectra,  digital  filter  magnitude  responses,  and
window function magnitude responses. When we use a logarithmic function to plot signal levels in the
frequency domain, the vertical axis unit of measure is decibels.
E.1 Using Logarithms to Determine Relative Signal Power
In  discussing  decibels,  it’s  interesting  to  see  how  this  unit  of  measure  evolved.  When  comparing
continuous  (analog)  signal  levels,  early  specialists  in  electronic  communications  found  it  useful  to
define  a  measure  of  the  difference  in  powers  of  two  signals.  If  that  difference  was  treated  as  the
logarithm of a ratio of powers, it could be used as a simple additive measure to determine the overall
gain  or  loss  of  cascaded  electronic  circuits.  The  positive  logarithms  associated  with  system
components having gain could be added to the negative logarithms of those components having loss
quickly to determine the overall gain or loss of the system. With this in mind, the difference between
two signal power levels (P1 and P2), measured in bels, was defined as the base 10 logarithm of the
ratio of those powers, or
(E-1)
† The dimensionless unit of measure bel was named in honor of Alexander Graham Bell.
The use of Eq. (E-1) led to another evolutionary step because the unit of bel was soon found to be
inconveniently small. For example, it was discovered that the human ear could detect audio power
level  differences  of  one-tenth  of  a  bel.  Measured  power  differences  smaller  than  one  bel  were  so
common that it led to the use of the decibel (bel/10), effectively making the unit of bel obsolete. The
decibel (dB), then, is a unit of measure of the relative power difference of two signals defined as
(E-2)
The logarithmic function 10·log10(P1/P2), plotted in Figure E-1, doesn’t seem too beneficial at first
glance, but its application turns out to be very useful. Notice the large change in the function’s value
when the power ratio (P1/P2) is small, and the gradual change when the ratio is large. The effect of
this nonlinearity is to provide greater resolution when the ratio P1/P2 is small, giving us a good way
to recognize very small differences in the power levels of signal spectra, digital filter responses, and
window function frequency responses.
Figure E-1 Logarithmic decibel function of Eq. (E-2).
Let’s demonstrate the utility of the logarithmic function’s variable resolution. First, remember that the
power  of  any  frequency-domain  sequence  representing  signal  magnitude  |X(m)|  is  proportional  to
|X(m)|  squared.  For  convenience,  the  proportionality  constant  is  assumed  to  be  one,  so  we  say  the
power of |X(m)| is
(E-3)
Although  Eq.  (E-3)  may  not  actually  represent  power  (in  watts)  in  the  classical  sense,  it’s  the
squaring operation that’s important here, because it’s analogous to the traditional magnitude squaring
operation used to determine the power of continuous signals. (Of course, if X(m) is complex, we can
calculate the power spectrum sequence using |X(m)|2 = Xreal(m)2 + Ximag(m)2.) Taking ten times the log
of Eq. (E-3) allows us to express a power spectrum sequence XdB(m) in dB as
(E-4)
Because log(x2) = log(x) + log(x) = 2log(x), we can eliminate the squaring operation in Eq. (E-4) by
doubling the factor of ten and represent the power spectrum sequence by the expression
(E-5)
Without the need for the squaring operation, Eq. (E-5)  is  a  more  convenient  way  than  Eq. (E-4)  to
calculate the XdB(m) power spectrum sequence from the X(m) sequence.
Equations (E-4)  and  (E-5),  then,  are  the  expressions  used  to  convert  a  linear  magnitude  axis  to  a
logarithmic  magnitude-squared,  or  power,  axis  measured  in  dB.  What  we  most  often  see  in  the
literature are normalized log magnitude spectral plots where each value of |X(m)|2 is divided by the
first |X(0)|2 power value (for m = 0), as
(E-6)
The  division  by  the  |X(0)|2  or  |X(0)|  value  always  forces  the  first  value  in  the  normalized  log
magnitude  sequence  XdB(m)  equal  to  0  dB.†  This  makes  it  easy  for  us  to  compare  multiple  log
magnitude  spectral  plots.  To  illustrate,  let’s  look  at  the  frequency-domain  representations  of  the
Hanning  and  triangular  window  functions.  The  magnitudes  of  those  frequency-domain  functions  are
plotted on a linear scale in Figure E-2(a) where we’ve arbitrarily assigned their peak values to be 2.
Comparing the two linear scale magnitude sequences, WHanning(m) and Wtriangular(m), we can see some
minor differences between their magnitude values. If we’re interested in the power associated with
the two window functions, we square the two magnitude functions and plot them on a linear scale as
in Figure E-2(b). The difference between the two window functions’ power sequences is impossible
to see above the frequency of, say, m = 8 in Figure E-2(b). Here’s where the dB scale helps us out. If
we  plot  the  normalized  log  magnitude  versions  of  the  two  magnitude-squared  sequences  on  a
logarithmic dB scale using Eq. (E-6), the difference between the two functions will become obvious.
† That’s because log10(|X(0)|/|X(0)|) = log10(1) = 0.
Figure E-2 Hanning (white squares) and triangular (black squares) window functions in the
frequency domain: (a) magnitude responses using a linear scale; (b) magnitude-squared responses
using a linear scale; (c) log magnitude responses using a normalized dB scale.
Normalization, in the case of the Hanning window, amounts to calculating the log magnitude sequence
normalized over |WHanning(0)| as
(E-7)
The  normalized  log  magnitude  sequences  are  plotted  in  Figure E-2(c).  We  can  now  clearly  see  the
difference  in  the  magnitude-squared  window  functions  in  Figure  E-2(c)  as  compared  to  the  linear
plots  in  Figure  E-2(b).  Notice  how  normalization  forced  the  peak  values  for  both  log  magnitude
functions in Figure E-2(c) to be zero dB. (The dots in Figure E-2 are connected by lines to emphasize
the sidelobe features of the two log magnitude sequences.)
Although  we’ve  shown  the  utility  of  dB  plots  using  window  function  frequency  responses  as
examples,  the  dB  scale  is  equally  useful  when  we’re  plotting  signal-power  spectra  or  digital  filter
frequency responses. We can further demonstrate the dB scale using a simple digital filter example.
Let’s  say  we’re  designing  an  11-tap  highpass  FIR  filter  whose  coefficients  are  shown  in  Figure  E-
3(a). If the center coefficient h(5) is –0.48, the filter’s frequency magnitude response |H–0.48(m)| can
be plotted as the white dots on the linear scale in Figure E-3(b). Should we change h(5) from –0.48 to
–0.5, the new frequency magnitude response |H–0.5(m)| would be the black dots in Figure E-3(b). It’s
difficult to see much of a difference between |H–0.48(m)| and |H–0.5(m)| on a linear scale. If we used
Eq. (E-6)  to  calculate  two  normalized  log  magnitude  sequences,  they  could  be  plotted  as  shown  in
Figure E-3(c), where the filter sidelobe effects of changing h(5) from –0.48 to –0.5 are now easy to
see.
Figure E-3 FIR filter magnitude responses: (a) FIR filter time-domain coefficients; (b) magnitude
responses using a linear scale; (c) log magnitude responses using the dB scale.
E.2 Some Useful Decibel Numbers
If the reader uses dB scales on a regular basis, there are a few constants worth committing to memory.
A power difference of 3 dB corresponds to a power factor of two; that is, if the magnitude-squared
ratio of two different frequency components is 2, then from Eq. (E-2),
(E-8)
Likewise,  if  the  magnitude-squared  ratio  of  two  different  frequency  components  is  1/2,  then  the
relative power difference is –3 dB because
(E-9)
Table E-1  lists  several  magnitude  and  power  ratios  versus  dB  values  that  are  worth  remembering.
Keep in mind that decibels indicate only relative power relationships. For example, if we’re told that
signal A is 6 dB above signal B, we know that the power of signal A is four times that of signal B,
and that the magnitude of signal A is twice the magnitude of signal B. We may not know the absolute
power of signals A and B in watts, but we do know that the power ratio is PA/PB = 4.
Table E-1 Some Useful Logarithmic Relationships
E.3 Absolute Power Using Decibels
Let’s discuss another use of decibels that the reader may encounter in the literature. It’s convenient for
practitioners  in  the  electronic  communications  field  to  measure  continuous  signal-power  levels
referenced to a specific absolute power level. In this way, they can speak of absolute power levels in
watts  while  taking  advantage  of  the  convenience  of  decibels.  The  most  common  absolute  power
reference level used is the milliwatt. For example, if P2 in Eq. (E-2) is a reference power level of
one milliwatt, then
(E-10)
The dBm unit of measure in Eq. (E-10) is read as “dB relative to a milliwatt.” Thus, if a continuous
signal is specified as having a power of 3 dBm, we know that the signal’s absolute power level is 2
times  one  milliwatt,  or  2  milliwatts.  Likewise,  a  –10  dBm  signal  has  an  absolute  power  of  0.1
milliwatts.†
† Other absolute reference power levels can be used. People involved with high-power transmitters sometimes use a single watt as their
reference power level. Their unit of power using decibels is the dBW, read as “dB relative to a watt.” In this case, for example, 3 dBW
is equal to a 2-watt power level.
The reader should take care not to inadvertently use dB and dBm interchangeably. They mean very
different things. Again, dB is a relative power level relationship, and dBm is an absolute power level
in milliwatts.

Appendix F. Digital Filter Terminology
The  first  step  in  becoming  familiar  with  digital  filters  is  to  learn  to  speak  the  language  used  in  the
filter  business.  Fortunately,  the  vocabulary  of  digital  filters  corresponds  very  well  to  the  mother
tongue  used  for  continuous  (analog)  filters—so  we  don’t  have  to  unlearn  anything  that  we  already
know. This appendix is an introduction to the terminology of digital filters.
Allpass filter—an IIR filter whose magnitude response is unity over its entire frequency range, but
whose phase response is variable. Allpass filters are typically appended in a cascade arrangement
following a standard IIR filter, H1(z), as shown in Figure F-1.
Figure F-1 Typical use of an allpass filter.
An allpass filter, Hap(z), can be designed so that its phase response compensates for, or equalizes,
the  nonlinear  phase  response  of  an  original  IIR  filter[1–3].  Thus,  the  phase  response  of  the
combined  filter,  Hcombined  (z),  is  more  linear  than  the  original  H1(z),  and  this  is  particularly
desirable in communications systems. In this context, an allpass filter is sometimes called a phase
equalizer.
Allpass  filters  have  the  property  that  the  numerator  polynomial  coefficients  in  the  filter’s  H(z)
transfer  function  are  a  reverse-order  version  of  the  denominator  polynomial  coefficients.  For
example, the following transfer function describes a 2nd-order allpass filter:
(F-1)
where  the  numerator  polynomial  coefficients  are  [B,  A,  1]  and  the  denominator  polynomial
coefficients are [1, A, B].
Attenuation—an amplitude loss, usually measured in dB, incurred by a signal after passing through a
digital filter. Filter attenuation is the ratio, at a given frequency, of the signal amplitude at the output
of the filter divided by the signal amplitude at the input of the filter, defined as
(F-2)
For a given frequency, if the output amplitude of the filter is smaller than the input amplitude, the
ratio in Eq. (F-2) is less than one, and the attenuation is a negative number.
Band reject filter—a filter that rejects (attenuates) one frequency band and passes both a lower- and
a  higher-frequency  band.  Figure  F-2(a)  depicts  the  frequency  response  of  an  ideal  band  reject
filter. This filter type is sometimes called a notch filter.
Figure F-2 Filter symbols and frequency responses: (a) band reject filter; (b) bandpass filter.
Bandpass filter—a filter, as shown in Figure F-2(b), that passes one frequency band and attenuates
frequencies above and below that band.
Bandwidth—the frequency width of the passband of a filter. For a lowpass filter, the bandwidth is
equal  to  the  cutoff  frequency.  For  a  bandpass  filter,  the  bandwidth  is  typically  defined  as  the
frequency difference between the upper and lower 3 dB points.
Bessel function—a mathematical function used to produce the most linear phase response of all IIR
filters with no consideration of the frequency magnitude response. Specifically, filter designs based
on Bessel functions have maximally constant group delay.
Butterworth  function—a  mathematical  function  used  to  produce  maximally  flat  filter  magnitude
responses with no consideration of phase linearity or group delay variations. Filter designs based
on  a  Butterworth  function  have  no  amplitude  ripple  in  either  the  passband  or  the  stopband.
Unfortunately, for a given filter order, Butterworth designs have the widest transition region of the
most popular filter design functions.
Cascaded filters—a filtering system where multiple individual filters are connected in series; that is,
the output of one filter drives the input of the following filter as illustrated in Figures F-1 and 6-
37(a).
Center frequency (f0)—the frequency lying at the midpoint of a bandpass filter. Figure F-2(b) shows
the fo center frequency of a bandpass filter.
Chebyshev  function—a  mathematical  function  used  to  produce  passband  or  stopband  ripples
constrained within fixed bounds. There are families of Chebyshev functions based on the amount of
ripple,  such  as  1  dB,  2  dB,  and  3  dB  of  ripple.  Chebyshev  filters  can  be  designed  to  have  a
frequency  response  with  ripples  in  the  passband  and  a  flat  stopband  (Chebyshev  Type  I),  or  flat
passbands and ripples in the stopband (Chebyshev Type II). Chebyshev filters cannot have ripples
in both the passband and the stopband. Digital filters based upon Chebyshev functions have steeper
transition region roll-off but more nonlinear-phase response characteristics than, say, Butterworth
filters.
CIC filter—cascaded integrator-comb filter. CIC filters are computationally efficient, linear-phase,
recursive, FIR, lowpass filters used in sample rate change applications. Those filters are discussed
in Chapter 10.
Coefficients—see filter coefficients.
Cutoff  frequency—the  highest  passband  frequency  for  lowpass  filters  (and  the  lower  passband
frequency  for  highpass  filters)  where  the  magnitude  response  is  within  the  peak-peak  passband
ripple region. Figure F-3 illustrates the fc cutoff frequency of a lowpass filter.
Figure F-3 A lowpass digital filter frequency response. The stopband relative amplitude is –20 dB.
Decibels (dB)—a unit of attenuation, or gain, used to express the relative voltage or power between
two signals. For filters, we use decibels to indicate cutoff frequencies (−3 dB) and stopband signal
levels (−20 dB) as illustrated in Figure F-3. Appendix E discusses decibels in more detail.
Decimation filter—a lowpass digital FIR filter where the output sample rate is less than the filter’s
input sample rate. As discussed in Section 10.1, to avoid aliasing problems, the output sample rate
must not violate the Nyquist criterion.
Digital filter—computational process, or algorithm, transforming a discrete sequence of numbers (the
input) into another discrete sequence of numbers (the output) having a modified frequency-domain
spectrum.  Digital  filtering  can  be  in  the  form  of  a  software  routine  operating  on  data  stored  in
computer memory or can be implemented with dedicated hardware.
Elliptic function—a mathematical function used to produce the sharpest roll-off for a given number of
filter taps. However, filters designed by using elliptic functions, also called Cauer filters, have the
poorest phase linearity of the most common IIR filter design functions. The ripples in the passband
and stopband are equal with elliptic filters.
Envelope delay—see group delay.
Filter coefficients—the  set  of  constants,  also  called  tap weights,  used  to  multiply  against  delayed
signal  sample  values  within  a  digital  filter  structure.  Digital  filter  design  is  an  exercise  in
determining the filter coefficients that will yield the desired filter frequency response. For an FIR
filter, by definition, the filter coefficients are the impulse response of the filter.
Filter order—a number describing the highest exponent in either the numerator or denominator of the
z-domain  transfer  function  of  a  digital  filter.  For  tapped-delay  line  FIR  filters,  there  is  no
denominator  in  the  transfer  function  and  the  filter  order  is  merely  the  number  of  delay  elements
used  in  the  filter  structure.  Generally,  the  larger  the  filter  order,  the  better  the  frequency-domain
performance, and the higher the computational workload, of the filter.
Finite impulse response (FIR) filter—defines a class of digital filters that have only zeros on the z-
plane. The key implications of this are: (1) FIR filter impulse responses have finite time durations,
(2) FIR filters are always stable, and (3) FIR filters can have exactly linear phase responses (so
long  as  the  filters’  impulse  response  samples  are  symmetrical,  or  antisymmetrical).  For  a  given
filter  order,  digital  FIR  filters  have  a  much  more  gradual  transition  region  roll-off  (poorer
performance)  than  digital  IIR  filters.  FIR  filters  can  be  implemented  with  both  nonrecursive
(tapped-delay line) and recursive (CIC filters, for example) structures.
Frequency magnitude response—a frequency-domain description of how a filter interacts with input
signals.  The  frequency  magnitude  response  in  Figure F-3  is  a  curve  of  filter  attenuation  (in  dB)
versus frequency. Associated with a filter’s magnitude response is a phase response.
Group  delay—the  negative  of  the  derivative  of  a  filter’s  frequency-domain  phase  response  with
respect  to  frequency,  G(ω)  =  –d(Hø(ω))/d(ω).  If  a  filter’s  complex  frequency  response  is
represented in polar form as
(F-3)
where digital frequency ω is continuous and ranges from –π to π radians/sample, corresponding to
a cyclic frequency range of –fs/2 to fs/2 Hz, then the filter’s group delay is defined as
(F-4)
Because  the  dimensions  of  Hø(ω)  are  radians,  and  the  dimensions  of  ω  are  radians/sample,  the
dimensions of group delay G(ω) are time measured in samples.
If a filter’s complex frequency response is expressed in terms of a normalized frequency variable
as
(F-5)
where frequency f is continuous and is in the range of –0.5 ≤ f ≤ 0.5, then the group delay G(f) is
defined as
(F-6)
The concept of group delay deserves additional explanation beyond a simple algebraic definition.
For an ideal lowpass filter, for example, the frequency-domain phase response will be linear and
the  group  delay  would  be  constant.  Group  delay  can  also  be  thought  of  as  the  propagation  time
delay  of  the  envelope  (the  information)  of  an  amplitude-modulated  (AM)  signal  as  it  passes
through  a  digital  filter.  (In  this  context,  group  delay  is  often  called  envelope delay.)  If  a  filter’s
passband group delay is not constant (a nonlinear-phase filter), then group delay distortion occurs
because  signals  at  different  frequencies  take  different  amounts  of  time  (a  different  number  of
sample time intervals) to pass through the filter.
Half-band  filter—a  type  of  FIR  filter  whose  transition  region  is  centered  at  one-quarter  of  the
sampling rate, or fs/4. Specifically, the end of the passband and the beginning of the stopband are
equally spaced about fs/4. Due to their frequency-domain symmetry, half-band filters are often used
in  decimation  filtering  schemes  because  half  of  their  time-domain  coefficients  are  zero.  This
reduces the number of necessary filter multiplications, as described in Section 5.7.
Highpass filter—a  filter  that  passes  high  frequencies  and  attenuates  low  frequencies,  as  shown  in
Figure F-4(a). We’ve all experienced a kind of highpass filtering in our living rooms. Notice what
happens  when  we  turn  up  the  treble  control  (or  turn  down  the  bass  control)  on  our  home  stereo
systems.  The  audio  amplifier’s  normally  flat  frequency  response  changes  to  a  kind  of  analog
highpass filter, giving us that sharp and tinny sound as the high-frequency components of the music
are being accentuated.
Figure F-4 Filter symbols and frequency responses: (a) highpass filter; (b) low pass filter.
Impulse response—a  digital  filter’s  time-domain  output  sequence  when  the  input  is  a  single  unity-
valued  sample  (impulse)  preceded  and  followed  by  zero-valued  samples.  A  digital  filter’s
frequency-domain response can be calculated by taking the discrete Fourier transform of the filter’s
time-domain impulse response[4].
Infinite impulse response (IIR) filter—a class of digital filters that may have both zeros and poles
on  the  z-plane.  As  such,  IIR  filters  are  not  guaranteed  to  be  stable  and  almost  always  have
nonlinear phase responses. For a given filter order (number of IIR feedback taps), IIR filters have a
much steeper transition region roll-off than digital FIR filters.
Linear-phase filter—a filter that exhibits a constant change in phase angle (degrees) as a function of
frequency. The resultant filter phase plot versus frequency is a straight line. As such, a linear-phase
filter’s  group  delay  is  a  constant.  To  preserve  the  integrity  of  their  information-carrying  signals,
linear phase is an important criterion for filters used in communications systems.
Lowpass  filter—a  filter  that  passes  low  frequencies  and  attenuates  high  frequencies  as  shown  in
Figure  F-4(b).  By  way  of  example,  we  experience  lowpass  filtering  when  we  turn  up  the  bass
control (or turn down the treble control) on our home stereo systems, giving us that dull, muffled
sound as the high-frequency components of the music are being attenuated.
Nonrecursive filter—a  digital  filter  implementation  where  no  filter  output  sample  is  ever  retained
for  later  use  in  computing  a  future  filter  output  sample.  Such  filters  have  no  “feedback”  signal
paths.
Notch filter—see band reject filter.
Order—see filter order.
Passband—that frequency range over which a filter passes signal energy with minimum attenuation,
usually  defined  as  the  frequency  range  where  the  magnitude  response  is  within  the  peak-peak
passband ripple region, as depicted in Figure F-3.
Passband ripple—peak-peak fluctuations, or variations, in the frequency magnitude response within
the passband of a filter as illustrated in Figure F-3.
Phase response—the difference in phase, at a particular frequency, between an input sinewave and
the  output  sinewave  at  that  frequency.  The  phase  response,  sometimes  called  phase  delay,  is
usually depicted by a curve showing the filter’s phase shift versus frequency. Section 5.8 discusses
digital filter phase response in more detail.
Phase  wrapping—an  artifact  of  arctangent  software  routines,  used  to  calculate  phase  angles,  that
causes apparent phase discontinuities. When a true phase angle is in the range of –180o to –360o,
some software routines automatically convert those angles to their equivalent positive angles in the
range of 0o to +180o. Section 5.8 illustrates an example of phase wrapping when the phase of an
FIR filter is calculated.
Quadrature filter—a dual-path digital filter operating on complex signals, as shown in Figure F-5.
One  filter  operates  on  the  in-phase  i(n)  data,  and  the  other  filter  processes  the  quadrature-phase
q(n) signal data. Quadrature filtering is normally performed on complex signals, whose spectra are
centered at zero Hz, using lowpass digital filters.
Figure F-5 Two lowpass filters used to implement quadrature filtering.
Recursive filter—a digital filter implementation where current filter output samples are retained for
later use in computing future filter output samples. Such filters have “feedback” signal paths.
Relative  attenuation—attenuation  measured  relative  to  the  largest  magnitude  value.  The  largest
signal level (minimum attenuation) is typically assigned the reference level of zero dB, as depicted
in  Figure  F-3,  making  all  other  magnitude  points  on  a  frequency-response  curve  negative  dB
values.
Ripple—refers to fluctuations (measured in dB) in the passband, or stopband, of a filter’s frequency-
response curve. Elliptic and Chebyshev-based filters have equiripple characteristics in that their
ripple is constant across their passbands. Bessel- and Butterworth-derived filters have no ripple in
their  passband  responses.  Ripples  in  the  stopband  response  are  sometimes  called  out-of-band
ripple.
Roll-off—a  term  used  to  describe  the  steepness,  or  slope,  of  the  filter  response  in  the  transition
region from the passband to the stopband. A particular digital filter may be said to have a roll-off
of 12 dB/octave, meaning that the second-octave frequency would be attenuated by 24 dB, and the
third-octave frequency would be attenuated by 36 dB, and so on.
Shape factor—a term used to indicate the steepness of a filter’s roll-off. Shape factor is normally
defined as the ratio of a filter’s passband width divided by the passband width plus the transition
region width. The smaller the shape factor value, the steeper the filter’s roll-off. For an ideal filter
with a transition region of zero width, the shape factor is unity. The term shape factor is also used
to describe analog filters.
Stopband—that band of frequencies attenuated by a digital filter. Figure F-3 shows the stopband of a
lowpass filter.
Structure—a  fancy  term  meaning  the  block  diagram,  the  signal-flow  implementation,  of  a  digital
filter.  For  example,  lowpass  moving  average  filters  may  be  built  (implemented)  with  both
nonrecursive structures and recursive structures.
Tap—a multiplication operation inside a digital filter that computes the product of a single data value
times a single filter coefficient.
Tap weights—see filter coefficients.
Tchebyschev function—see Chebyshev function.
Transfer function—a mathematical expression of the ratio of the output of a digital filter divided by
the  input  of  the  filter  as  expressed  in  a  transform  domain  (e.g.,  z-domain,  Laplace,  frequency).
Given the transfer function, we can determine the filter’s frequency magnitude and phase responses.
Transition  region—the  frequency  range  over  which  a  filter  transitions  from  the  passband  to  the
stopband. Figure F-3  illustrates  the  transition  region  of  a  lowpass  filter.  The  transition  region  is
sometimes called the transition band.
Transversal filter—in the field of digital filtering, transversal filter is another name for FIR filters
implemented with the nonrecursive structures described in Chapter 5.
Zero-phase  filter—an  off-line  (because  it  operates  on  a  block  of  filter  input  samples)  filtering
method which cancels the nonlinear phase response of an IIR filter. Section 13.12 details this non-
real-time filtering technique.
References
[1] Rabiner, L. R., and Gold, B. The Theory and Application of Digital Signal Processing, Prentice
Hall, Englewood Cliffs, New Jersey, 1975, pp. 206, 273, and 288.
[2]  Oppenheim,  A.  V.,  and  Schafer,  R.  W.  Discrete  Time  Signal  Processing,  Prentice  Hall,
Englewood Cliffs, New Jersey, 1989, pp. 236 and 441.
[3] Laakso, T. I., et al. “Splitting the Unit Delay,” IEEE Signal Processing Magazine, January 1996,
p. 46.
[4]  Pickerd,  J.  “Impulse-Response  Testing  Lets  a  Single  Test  Do  the  Work  of  Thousands,”  EDN,
April 27, 1995.
