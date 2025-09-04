# Understanding DSP (Lyons) — Chunk 27/27

> Source: `UnderstandingDigitalSignalProcessing.pdf` · Pages ~1-858 · Extractor: pdfminer.six
> Target ≈ 2800 words, Overlap = 200 words · Actual = 8873

> Note: DSP textbook content for SpectralCanvasPro RT-audio knowledge base

---
Appendix I. Computing Chebyshev Window Sequences
Because detailed methods for computing Chebyshev window functions are not readily available in the
literature of DSP, here we provide the steps for computing these useful window sequences.
Below  we  provide  methods  for  computing  two  types  of  Chebyshev  window  sequences.  The  first
window  type  yields  symmetric  window  sequences,  in  which  their  first  and  last  samples  are  equal.
That type of window is used in the Window Design Method of tapped-delay line FIR filter design.
The second Chebyshev window computation method produces nonsymmetric window sequences, in
which  their  first  and  last  samples  are  not  equal.  That  type  of  window  is  used  for  spectral  leakage
reduction  in  spectrum  analysis  applications.  (This  nonsymmetric  type  of  window  has  a  Fourier
transform that is real-only.) I thank DSP guru Prof. Fredric J. Harris, San Diego State University, for
his personal guidance enabling the creation of the following procedures.
I.1 Chebyshev Windows for FIR Filter Design
Symmetric Chebyshev window sequences, used in the Window Design Method of tapped-delay line
FIR filters, are computed as follows:
1.  Given  a  desired  Chebyshev  window  sequence  length  of  N,  where  N  is  an  odd  integer,  define
integer M = N–1.
2. Define the window’s sidelobe-level control parameter as γ. The window’s sidelobe peak levels
will be –20γ dB below the main lobe’s peak level. (For example, if we desire frequency-domain
sidelobe levels to be 40 dB below the main lobe’s peak level, then we set γ = 2.)
3. Compute parameter α as
4. Compute the M-length sequence A(m) using
(I-1)
(I-2)
where the index m is 0 ≤ m ≤ (M–1).
5. For each m, evaluate the Mth-degree Chebyshev polynomial whose argument is A(m) to generate
a  frequency-domain  sequence  W(m).  There  are  many  ways  to  evaluate  Chebyshev  polynomials.
Due to its simplicity of notation, we suggest the following:
or
(I-3)
(I-4)
depending on whether or not an individual |A(m)| value is greater than unity. In theory the resultant
W(m)  sequence  is  real-only,  but  our  software’s  computational  numerical  errors  may  produce  a
complex-valued  W(m)  with  very  small  imaginary  parts.  Those  imaginary  parts,  if  they  exist,
should be ignored. The above (−1)m factors are necessary because the frequency-domain index m
is never less than zero. Note: If your software does not accommodate complex values, then you
can avoid problems by replacing A(m) with |A(m)| in this step.
6. Compute a preliminary time-domain window sequence, w(m), using
w(m) = real part of the M-point inverse DFT of W(m).
7. Replace w(0), the first w(m) time sample, with w(0)/2.
8. Append that new w(0) sample value to the end of the M-point w(m) sequence, w(N–1) = w(0),
creating the desired N-length w(k) window sequence where the time index k is 0 ≤ k ≤ (N–1).
9. Normalize the amplitude of w(k), to obtain a unity peak amplitude, by dividing each sample of
w(k) from Step 8 by the maximum sample value in w(k).
The  above  procedure  seems  a  bit  involved  but  it’s  not  really  so  bad,  as  the  following  Chebyshev
window design example will show. Assume we need an N = 9-sample Chebyshev window function
whose  frequency-domain  sidelobes  are  60  dB  below  the  window’s  main  lobe  level.  Given  those
requirements, N = 9, M = 8, γ = 3, and from Eq. (I-1)
α = cosh[cosh–1(103)/8] = 1.4863.
After the inverse DFT operation in the above Step 6, w(m = 0)/2 = 11.91, thus we set w(k = 0) = w(k
= 8) = 11.91. The maximum value of w(k) is 229.6323, so we divide the entire w(k) sequence by that
value, yielding our final normalized 9-sample symmetric Chebyshev window sequence listed in the
rightmost column of Table I-1.
Table I-1 Nine-Point Symmetric Chebyshev Window Computations
I.2 Chebyshev Windows for Spectrum Analysis
Nonsymmetric  Chebyshev  window  sequences,  used  for  spectral  leakage  reduction  in  spectrum
analysis applications, are computed using the above steps with the following changes:
•  For  a  Q-length  nonsymmetric  Chebyshev  window  sequence,  where  Q  is  an  even  integer,  in  the
above Step 1 set M = Q.
• Skip the above Step 8, retaining the Q-length nonsymmetric w(k) sequence, where the time index k
is 0 ≤ k ≤ (Q–1). Normalize the amplitude of the w(k) sequence as described in the above Step 9.
If  Q  =  8,  for  example,  our  final  γ  =  3  normalized  8-sample  nonsymmetric  Chebyshev  window
sequence would be the samples listed in the rightmost column of Table I-2.
Table I-2 Eight-Point Nonsymmetric Chebyshev Window Computations

Index
A
Absolute value, 9. See also Magnitude.
A/D converters, quantization noise
clipping, 706
crest factor, 640
dithering, 706–709
effective bits, 641
fixed-point binary word length, effects of, 634–642
oversampling, 704–706
reducing, 704–709
SNR (signal-to-noise ratio), 637–642, 711–714
triangular dither, 708
A/D converters, testing techniques
A/D dynamic range, estimating, 714–715
histogram testing, 711
missing codes, detecting, 715–716
quantization noise, estimating with the FFT, 709–714
SFDR (spurious free dynamic range), 714–715
SINAD (signal-to-noise-and-distortion), 711–714
SNR (signal-to-noise ratio), 711–714
Adaptive filters, 184
Addition
block diagram symbol, 10
complex numbers, 850
Additive white noise (AWN), 380
AGC (automatic gain control), 783–784
Aliasing
definition, 36
frequency-domain ambiguity, 33–38
in IIR filters, 304–305
All-ones rectangular functions
DFT for, 115–118
Dirichlet kernel, 115–118, 120
Allpass filters, definition, 893
AM demodulation
filtering narrowband noise, 792–797
Hilbert transforms, 484–485
Amplitude
definition, 8
loss. See Attenuation.
Amplitude response, DFT
complex input, 73
real cosine input, 83–84
Analog, definition, 2
Analog filters
approximating, 302
vs. digital, 169
Analog signal processing, 2
Analog-to-digital (A/D) converters. See A/D converters.
Analytic signals
bandpass quadrature, 455
definition, 483
generation methods, comparing, 497–498
half-band FIR filters, 497
time-domain, generating, 495–497
Anti-aliasing filters, 42, 555–558
Anti-imaging filters, 555–558
Arctangent
approximation, 756–758
vector rotation. See Vector rotation with arctangents.
Argand, Jean Robert, 848
Argand diagrams of complex numbers, 848
Argand plane, 440–441
Attenuation
CIC filters, improving, 557–558
definition, 894
Automatic gain control (AGC), 783–784
Average, statistical measures of noise, 868–870
Average power in electrical circuits, calculating, 874–875
Averaging signals. See Signal averaging.
AWN (additive white noise), 380
B
Band reject filters, 894
Band-limited signals, 38
Bandpass design, for FIR filters, 201–203
Bandpass filters
comb filters, 400
definition, 895
from half-band FIR filters, 497
multisection complex FSFs, 398–403
Bandpass sampling
1st-order sampling, 46
definition, 43
optimum sampling frequency, 46
positioning sampled spectra, 48
real signals, 46
sampling translation, 44
SNR (signal-to-noise) ratio, 48–49
spectral inversion, 46–47
spectral replication, 44–45
Bandpass signals
in the frequency-domain, 454–455
interpolating, 728–730
Bandwidth, definition, 895
Bartlett windows. See Triangular windows.
Base 8 (octal) numbers, 624–625
Base 16 (hexadecimal) numbers, 625
Bell, Alexander Graham, 885
Bels, definition, 885
Bessel functions
definition, 895
Bessel-derived filters, ripples, 901
Bessel’s correction, 870–871
Bias
DC, sources and removal, 761
in estimates, 870–871
fixed-point binary formats, 628
in signal variance, computing, 797–799
Bilateral Laplace transforms, 258
Bilinear transform method, designing IIR filters
analytical methods, 302
definition, 257
example, 326–330
frequency warping, 319, 321–325, 328–330
mapping complex variables, 320–324
process description, 324–326
Bin centers, calculating absolute frequency, 139–140
Binary points, 629
Binary shift multiplication/division, polynomial evaluation, 773–774
Biquad filters, 299
Bit normalization, 653
Bit reversals
avoiding, 158
fast Fourier transform input/output data index, 149–151
Bits, definition, 623
Blackman windows
in FIR filter design, 195–201
spectral leakage reduction, 686
Blackman windows (exact), 686, 733
Blackman-Harris windows, 686, 733
Block averaging, SNR (signal-to-noise ratio), 770
Block convolution. See Fast convolution.
Block diagrams
filter structure, 172–174
quadrature sampling, 459–462
symbols, 10–11
uses for, 10
Block floating point, 656–657
Boxcar windows. See Rectangular windows.
Butterfly patterns in FFTs
description, 145–149
optimized, 156
radix-2 structures, 151–154
single butterfly structures, 154–158
wingless, 156
Butterworth function
definition, 895
derived filters, ripples, 901
C
Cardano, Girolamo, 439
Carrier frequency, 44
Cartesian form, quadrature signals, 442
Cascaded filters, 295–299, 895
Cascaded integrators, 563
Cascaded-comb subfilters, 412–413
Cascade/parallel filter combinations, 295–297
Cauer filters, 896
Causal systems, 258
Center frequency, definition, 895
Central Limit Theory, 723
Central-difference differentiators, 363–366
CFT (continuous Fourier transform), 59, 98–102
Chebyshev function, definition, 895
Chebyshev windows, 197–201, 927–930
Chebyshev-derived filters, ripples, 900
CIC (cascaded integrator-comb) filters
cascaded integrators, 563
comb section, 553
compensation FIR filters, 563–566
definition, 895
implementation issues, 558–563
nonrecursive, 765–768
recursive running sum filters, 551–552
structures, 553–557
substructure sharing, 765–770
transposed structures, 765–770
two’s complement overflow, 559–563
Circular buffers, IFIR filters, 388–389
Clipping A/D converter quantization noise, 706
Coefficients. See Filter coefficients.
Coherent sampling, 711
Coherent signal averaging. See Signal averaging, coherent.
Comb filters. See also Differentiators.
alternate FSF structures, 416–418
bandpass FIR filtering, 400
cascaded-comb subfilters, 412–413
with complex resonators, 392–398
frequency response, 903–904
second-order comb filters, 412–413
Comb section. CIC filters, 553
Commutative property, LTI, 18–19
Commutator model, polyphase filters, 524
Compensation FIR filters, CIC filters, 563–566
Complex conjugate, DFT symmetry, 73
Complex down-conversion
decimation, in frequency translation, 782
quadrature signals, 455, 456–462
Complex exponentials, quadrature signals, 447
Complex frequency, Laplace variable, 258
Complex frequency response, filters, 277
Complex mixing, quadrature signals, 455
Complex multipliers, down-converting quadrature signals, 458
Complex number notation, quadrature signals, 440–446
Complex numbers. See also Quadrature signals.
Argand diagrams, 848
arithmetic of, 848–858
definition, 439
as a function of time, 446–450
graphical representation of, 847–848
rectangular form, definition, 848–850
rectangular form, vs. polar, 856–857
roots of, 853–854
trigonometric form, 848–850
Complex phasors, quadrature signals, 446–450
Complex plane, quadrature signals, 440–441, 446
Complex resonators
with comb filters, 392–398
FSF (frequency sampling filters), 394–398
Complex signals. See Quadrature signals.
Conditional stability, Laplace transform, 268
Conjugation, complex numbers, 851–852
Constant-coefficient transversal FIR filters, 184
Continuous Fourier transform (CFT), 59, 98–102
Continuous lowpass filters, 41
Continuous signal processing
definition, 2
frequency in, 5–6
Continuous signals, definition, 2
Continuous systems, time representation, 5
Continuous time-domain, Laplace transform, 258–259
Converting analog to digital. See A/D converters.
Convolution. See also FIR (finite impulse response) filters, convolution.
fast, 716–722
LTI, 19
overlap-and-add, 720–722
overlap-and-save, 718–720
Cooley, J., 135
CORDIC (COordinate Rotation DIgital Computer), 756–758
Coupled quadrature oscillator, 787
Coupled-form IIR filter, 834–836
Crest factor, 640
Critical Nyquist, 37
Cutoff frequencies
definition, 896
designing FIR filters, 186
D
Data formats
base systems, 624
definition, 623
place value system, 624
Data formats, binary numbers. See also Fixed-point binary formats; Floating-point binary formats.
1.15 fixed-point, 630–632
block floating point, 656–657
converting to hexadecimal, 625
converting to octal, 624–625
definition, 623
dynamic range, 632–634
precision, 632–634
representing negative values, 625–626
Data overflow. See Overflow.
dB (decibels), definition, 886, 896
dBm (decibels), definition, 892
DC
bias, sources of, 761
block-data DC removal, 762
defined, 62
from a time-domain signal, 812–815
DC removal, real-time
using filters, 761–763
noise shaping property, 765
with quantization, 763–765
Deadband effects, 293
DEC (Digital Equipment Corp.), floating-point binary formats, 654–655
Decibels
bels, definition, 885
common constants, 889–891
dB, definition, 886, 896
dBm, definition, 892
Decimation. See also Interpolation.
combining with interpolation, 521–522
definition, 508
to implement down-conversion, 676–679
multirate filters, 521–522
sample rate converters, 521–522
drawing downsampled spectra, 515–516
frequency properties, 514–515
magnitude loss in the frequency-domain, 515
overview, 508–510
time invariance, 514
time properties, 514–515
example, 512–513
overview, 510–511
polyphase decomposition, 514
Decimation filters
choosing, 510
definition, 896
Decimation-in-frequency algorithms, FFTs
radix-2 butterfly structures, 151–154, 734–735
Decimation-in-time algorithms, FFTs
index bit reversal, 149–151
radix-2 butterfly structures, 151–154
single butterfly structures, 154–158, 735–737
Demodulation
AM, 484–485
FM, 486
quadrature signals, 453–455, 456–462
Descartes, René, 439
Detection
envelope, 784–786
peak threshold, with matched filters, 377, 379–380
quadrature signals, 453–454
signal transition, 820–821
single tone. See Single tone detection.
DFT (discrete Fourier transform). See also DTFT (discrete-time Fourier transform); SDFT (sliding
DFT).
analyzing FIR filters, 228–230
computing large DFTs from small FFTs, 826–829
definition, 60
examples, 63–73, 78–80
versus FFT, 136–137
frequency axis, 77
frequency granularity, improving. See Zero padding.
frequency spacing, 77
frequency-domain sampling, 98–102
inverse, 80–81
linearity, 75
magnitudes, 75–76
picket fence effect, 97
rectangular functions, 105–112
resolution, 77, 98–102
scalloping loss, 96–97
shifting theorem, 77–78
spectral estimation, improving. See Zero padding.
time reversal, 863–865
zero padding, 97–102
DFT leakage. See also Spectral leakage, FFTs.
cause, 82–84
definition, 81
description, 81–82
predicting, 82–84
sinc functions, 83, 89
wraparound, 86–88
DFT leakage, minimizing
Chebyshev windows, 96
Hamming windows, 89–93
Hanning windows, 89–97
Kaiser windows, 96
rectangular windows, 89–97
triangular windows, 89–93
windowing, 89–97
DFT processing gain
average output noise-power level, 103–104
inherent gain, 102–105
integration gain, 105
multiple DFTs, 105
output signal-power level, 103–104
single DFT, 102–105
SNR (signal-to-noise ratio), 103–104
DIF (decimation-in-frequency), 734–735
Difference equations
example, 5
IIR filters, 255–256
Differentiators
central-difference, 363–366
differentiating filters, 364
first-difference, 363–366
narrowband, 366–367
optimized wideband, 369–370
overview, 361–363
performance improvement, 810–812
wideband, 367–369
Digital differencer. See Differentiators.
Digital Equipment Corp. (DEC), floating-point binary formats, 654–655
Digital filters. See also specific filters.
vs. analog, 169
definition, 896
Digital signal processing, 2
Direct Form I filters, 275–278, 289
Direct Form II filters, 289–292
Direct Form implementations, IIR filters, 292–293
Dirichlet, Peter, 108
Dirichlet kernel
all-ones rectangular functions, 115–118, 120
general rectangular functions, 108–112
symmetrical rectangular functions, 113–114
Discrete convolution in FIR filters. See also FIR (finite impulse response) filters, convolution.
description, 214–215
in the time domain, 215–219
Discrete Fourier transform (DFT). See DFT (discrete Fourier transform).
Discrete Hilbert transforms. See Hilbert transforms.
Discrete linear systems, 12–16
Discrete systems
definition, 4
example, 4–5
time representation, 5
Discrete-time expression, 4
Discrete-time Fourier transform (DTFT), 101, 120–123
Discrete-time signals
example of, 2
frequency in, 5–6
sampling, frequency-domain ambiguity, 33–38
use of term, 2
Discrete-time waveforms, describing, 8
Dispersion, statistical measures of noise, 869
DIT (decimation-in-time), 735–737
Dithering
A/D converter quantization noise, 706–709
with filters, 294
triangular, 708
Dolph-Chebyshev windows in FIR filter design, 197
Down-conversion
Delay/Hilbert transform filter, 817–818, 819–820
filtering and decimation, 676–679
folded FIR filters, 818
frequency translation, without multiplication, 676–679
half-band filters, 817–818
single-decimation technique, 819–820
Down-conversion, quadrature signals
complex, 455, 456–462
complex multipliers, 458
sampling with digital mixing, 462–464
Downsampling, decimation
drawing downsampled spectra, 515–516
frequency properties, 514–515
magnitude loss in the frequency-domain, 515
overview, 508–510
time invariance, 514
time properties, 514–515
DTFT (discrete-time Fourier transform), 101, 120–123. See also DFT (discrete Fourier transform).
Dynamic range
binary numbers, 632–634
floating-point binary formats, 656–658
SFDR (spurious free dynamic range), 714–715
E
Elliptic functions, definition, 896
Elliptic-derived filters, ripples, 900
Envelope delay. See Group delay.
Envelope detection
approximate, 784–786
Hilbert transforms, 483–495
Equiripple filters, 418, 901
Estrin’s Method, polynomial evaluation, 774–775
Euler, Leonhard, 442, 444
Euler’s equation
bilinear transform design of IIR filters, 322
DFT equations, 60, 108
impulse invariance design of IIR filters, 315
quadrature signals, 442–443, 449, 453
Exact Blackman windows, 686
Exact interpolation, 778–781
Exponent, floating-point binary format, 652
Exponential averagers, 608–612
Exponential moving averages, 801–802
Exponential signal averaging. See Signal averaging, exponential.
Exponential variance computation, 801–802
F
Fast convolution, 716–722
FFT (fast Fourier transform)
averaging multiple, 139
constant-geometry algorithms, 158
convolution. See Fast convolution.
decimation-in-frequency algorithms, 151–154
decimation-in-time algorithms, 149–158
versus DFT, 136–137
exact interpolation, 778–781
fast FIR filtering, 716–722
hints for using, 137–141
history of, 135
interpolated analytic signals, computing, 781
interpolated real signals, interpolating, 779–780
interpreting results, 139–141
inverse, computing, 699–702, 831–833
in place algorithm, 157
radix-2 algorithm, 141–149
radix-2 butterfly structures, 151–158
signal averaging, 600–603
single tone detection, 737–738, 740–741
vs. single tone detection, 740–741
software programs, 141
time-domain interpolation, 778–781
Zoom FFT, 749–753
FFT (fast Fourier transform), real sequences
a 2N-point real FFT, 695–699
two N-point real FFTs, 687–694
FFT (fast Fourier transform), twiddle factors
derivation of the radix-2 FFT algorithm, 143–149
DIF (decimation-in-frequency), 734–735
DIT (decimation-in-time), 735–737
Fibonacci, 450–451
Filter coefficients
definition, 897
for FIRs. See Impulse response.
flipping, 493–494
for FSF (frequency sampling filters), 913–926
quantization, 293–295
Filter order, 897
Filter taps, estimating, 234–235, 386–387
Filters. See also FIR (finite impulse response) filters; IIR (infinite impulse response) filters; Matched
filters; specific filters.
adaptive filters, 184
allpass, 893
analog vs. digital, 169
band reject, 894
bandpass, 895
cascaded, 895
Cauer, 896
CIC, 895
DC-removal, 762–763
decimation, 896
differentiating, 364. See also Differentiators.
digital, 896
down-conversion, 676–679
equiripple, 418
highpass, 898
linear phase, 899
lowpass, 899
narrowband noise, 792–797
nonrecursive, 226–230, 290–291, 899
optimal FIR, 418
overview, 169–170
parallel, 295–297
passband, 900
process description, 169–170
prototype, 303
quadrature, 900
real-time DC removal, 762–763
recursive, 290–291, 900
recursive running sum, 551–552
Remez Exchange, 418
sharpening, 726–728
structure, diagramming, 172–174
time-domain slope detection, 820–821
transposed structure, 291–292
transversal, 173–174. See also FIR (finite impulse response) filters.
zero-phase, 725, 902
Filters, analytic signals
half-band FIR filters, 497
I-channel filters, 496
in-phase filters, 496
Q-channel filters, 496
quadrature phase filters, 496
time-domain FIR filter implementation, 489–494
Finite-word-length errors, 293–295
FIR  (finite  impulse  response)  filters.  See also FSF  (frequency  sampling  filters); IFIR  (interpolated
FIR) filters; IIR (infinite impulse response) filters.
coefficients. See Impulse response.
constant coefficients, 184
definition, 897
fast FIR filtering using the FFT, 716–722
folded structure. See Folded FIR filters.
frequency magnitude response, determining, 179
frequency-domain response, determining, 179
group delay, 211–212
half-band. See Half-band FIR filters.
vs. IIR filters, 332–333
impulse response, 177–179
narrowband lowpass. See IFIR (interpolated FIR) filters.
nonrecursive, analyzing, 226–230
phase response in, 209–214
phase unwrapping, 210
phase wrapping, 209, 900
polyphase filters, 522–527
sharpening, 726–728
signal averaging. See Signal averaging, with FIR filters.
signal averaging with, 178, 180–184
stopband attenuation, improving, 726–728
tapped delay, 181–182
transient response, 181–182
z-transform of, 288–289
FIR (finite impulse response) filters, analyzing
with DFTs, 228–230
estimating number of, 234–235
fractional delay, 233
group delay, 230–233
passband gain, 233–234
stopband attenuation, 234–235
symmetrical-coefficient FIR filters, 232–233
FIR (finite impulse response) filters, convolution
description, 175–186
discrete, description, 214–215
discrete, in the time domain, 215–219
fast convolution, 716–722
impulse response, 177–178
inputs, time order reversal, 176
signal averaging, 175–176
theorem, applying, 222–226
theorem, description, 219–222
time-domain aliasing, avoiding, 718–722
time-domain convolution vs. frequency-domain multiplication, 191–194
FIR (finite impulse response) filters, designing
bandpass method, 201–203
cutoff frequencies, 186
with forward FFT software routines, 189
Fourier series design method. See Window design method, FIR filters.
Gibbs’s phenomenon, 193
highpass method, 203–204
low-pass design, 186–201
magnitude fluctuations, 190–194
Optimal design method, 204–207
Parks-McClellan Exchange method, 204–207
passband ripples, minimizing, 190–194, 204–207. See also Windows.
Remez method, 204–207
stopband ripples, minimizing, 204–207
time-domain coefficients, determining, 186–194
time-domain convolution vs. frequency-domain multiplication, 191–194
very high performance filters, 775–778
window design method, 186–194
windows used in, 194–201
1st-order IIR filters, signal averaging, 612–614
1st-order sampling, 46
First-difference differentiators, 363–366
Fixed-point binary formats. See also Floating-point binary formats.
1.15 format, 630–632
bias, 628
binary points, 629
decimal numbers, converting to 1.5 binary, 632
fractional binary numbers, 629–632
hexadecimal (base 16) numbers, 625
integer plus fraction, 629
lsb (least significant bit), 624
msb (most significant bit), 624
octal (base 8) numbers, 624–625
offset, 627–628
overflow, 629
Q30 format, 629
radix points, 629
representing negative values, 625–626
sign extend operations, 627
sign-magnitude, 625–626
two’s complement, 626–627, 629
Fixed-point binary formats, finite word lengths
A/D converter best estimate values, 635
A/D converter quantization noise, 634–642
A/D converter vs. SNR, 640–642
convergent rounding, 651
crest factor, 640
data overflow, 642–646
data rounding, 649–652
effective bits, 641
round off noise, 636–637
round to even method, 651
round-to-nearest method, 650–651
truncation, 646–649
Floating-point binary formats. See also Fixed-point binary formats.
bit normalization, 653
common formats, 654–655
DEC (Digital Equipment Corp.), 654–655
description, 652
dynamic range, 656–658
evaluating, 652
exponent, 652
fractions, 653
gradual underflow, 656
hidden bits, 653
IBM, 654–655
IEEE Standard P754, 654–655
mantissa, 652
MIL-STD 1750A, 654–655
min/max values, determining, 656–657
unnormalized fractions, 656
word lengths, 655
FM demodulation
algorithms for, 758–761
filtering narrowband noise, 792–797
Hilbert transforms, 486
Folded FIR filters
designing Hilbert transforms, 493
down-conversion, 818
frequency translation, without multiplication, 678
half-band filters, sample rate conversion, 548
Hilbert transforms, designing, 493
multipliers, reducing, 702–704
nonrecursive, 419–420
tapped-delay line, 389
Folding frequencies, 40
Forward FFT
computing, 831–833
software routines for designing FIR filters, 189
Fourier series design FIR filters. See Window design method, FIR filters.
Fourier transform pairs, FIR filters, 178–179
Fractional binary numbers, 629–632
Fractional delay, FIR filters, 233
Frequency
continuous vs. discrete systems, 5
of discrete signals, determining. See DFT (discrete Fourier transform).
discrete-time signals, 5–6
properties, interpolation, 519
resolution, improving with FIR filters, 228–230
units of measure, 2–3
Frequency attenuation, FIR filters, 182
Frequency axis
definition, 77
DFT, 77
in Hz, 118
normalized angle variable, 118
in radians/seconds, 118–119
rectangular functions, 118–120
with zero padding, 100
Frequency domain
definition, 6
Hamming windows, 683–686
Hanning windows, 683–686
listing sequences, 7
performance. IIR filters, 282–289
quadrature signals, 451–454
spectral leak reduction, 683–686
windowing in, 683–686
windows, 683–686
Frequency magnitude response
definition, 897
determining with FIR filters, 179
Frequency response
LTI, determining, 19
for Mth-order IIR filter, 275–276
Frequency response, FIR filters
determining, 179–186
factors affecting, 174
modifying, 184–186
Frequency sampling design method vs. FSF, 393–394
Frequency sampling filters. See FSF (frequency sampling filters).
Frequency translation, bandpass sampling, 44
Frequency translation, with decimation
complex down-conversion, 782
complex signals, 781–783
real signals, 781
Frequency translation, without multiplication
by 1/2 the sampling rate, 671–673
by 1/4 the sampling rate, 674–676
down-conversion, 676–679
inverting the output spectrum, 678–679
Frequency translation to baseband, quadrature signals, 319
Frequency warping, 319, 321–325, 328–330
FSF (frequency sampling filters). See also FIR (finite impulse response) filters.
complex resonators, 394–398
designing, 423–426
frequency response, single complex FSF, 904–905
history of, 392–394
linear-phase multisection real-valued, 409–410
modeling, 413–414
multisection complex, 398–403
multisection real-valued, 406–409
vs. Parks-McClellan filters, 392
real FSF transfer function, 908–909
stability, 403–406
stopband attenuation, increasing, 414–416
stopband sidelobe level suppression, 416
transition band coefficients, 414–416
Type IV example, 419–420, 423–426
G
Gain. See also DFT processing gain.
AGC (automatic gain control), 783–784
IIR filters, scaling, 300–302
integration, signal averaging, 600–603
passband, 233–234
windows, 92
Gauss, Karl, 439, 444
Gaussian PDFs, 882–883
General numbers, 446. See also Complex numbers.
Geometric series, closed form, 107, 859–861
Gibbs’s phenomenon, 193
Goertzel algorithm, single tone detection
advantages of, 739
description, 738–740
example, 740
vs. the FFT, 740–741
stability, 838–840
Gold-Rader filter, 834–836
Gradual underflow, floating-point binary formats, 656
Gregory, James, 23
Group delay
definition, 897–898
differentiators, 365
filters, computing, 830–831
FIR filters, 211–212, 230–233
H
Half Nyquist, 37
Half-band FIR filters
analytic signals, 497
as complex bandpass filters, 497
definition, 898
description, 207–209
down-conversion, 817–818
frequency translation, 802–804
Half-band FIR filters, sample rate conversion
fundamentals, 544–546
implementation, 546–548
overview, 543
Hamming, Richard, 366
Hamming windows
in the frequency domain, 683–686
spectral peak location, 733
Hann windows. See Hanning windows.
Hanning windows
description, 89–97
DFT leakage, minimizing, 89–97
in the frequency domain, 683–686
spectral peak location, 733
Harmonic sampling. See Bandpass sampling.
Harmonics of discrete signals, determining. See DFT (discrete Fourier transform).
Harris, Fred, 791
Heaviside, Oliver, 257
Hertz, 3
Hertz, Heinrich, 3
Hexadecimal (base 16) numbers, 625
Hidden bits, floating-point binary formats, 653
Highpass filters, definition, 898
Highpass method, designing FIR filters, 203–204
Hilbert, David, 479
Hilbert transformers, designing
common mistake, 493–494
even-tap transformers, 493
frequency-domain transformers, 494–495
half-band filter coefficient modification, 804–805
half-band filter frequency translation, 802–804
odd-tap transformers, 493
time-domain FIR filter implementation, 489–494
time-domain transformers, 489–494
Hilbert transforms
AM demodulation, 484–485
definition, 480
envelope detection, 483–495
example, 481–482
FM demodulation, 486
impulse response, 487–489
one-sided spectrum, 483
signal envelope, 483–495
Hilbert transforms, analytic signals
definition, 483
generation methods, comparing, 497–498
half-band FIR filters, 497
time-domain, generating, 495–497
Histogram testing, A/D converter techniques, 711
Homogeneity property, 12
Horner, William, 773
Horner’s Rule, 772–774
Human ear, sensitivity to decibels, 886
I
IBM, floating-point binary formats, 654–655
I-channel filters, analytic signals, 496
IDFT (inverse discrete Fourier transform), 80–81
IEEE Standard P754, floating-point binary formats, 654–655
IF sampling. See Bandpass sampling.
IFIR (interpolated FIR) filters. See also FIR (finite impulse response) filters.
computational advantage, 384–385, 391
definition, 381
expansion factor M, 381, 385–386
filter taps, estimating, 386–387
image-reject subfilter, 382–384, 390
implementation issues, 388–389
interpolated, definition, 384
interpolators. See Image-reject subfilter.
lowpass design example, 389–391
optimum expansion factor, 386
performance modeling, 387–388
prototype filters, 382
shaping subfilters, 382, 385
IIR (infinite impulse response) filters. See also FIR (finite impulse response) filters; FSF (frequency
sampling filters).
allpass, 893
analytical design methods, 302
coupled-form, 834–836
definition, 899
design techniques, 257. See also specific techniques.
difference equations, 255–256
vs. FIR filters, 253, 332–333
frequency domain performance, 282–289
infinite impulse response, definition, 280
interpolated, example, 837–838
phase equalizers. See Allpass filters.
poles, 284–289
recursive filters, 290–291
scaling the gain, 300–302
SNR (signal-to-noise ratio), 302
stability, 263–270
z-domain transfer function, 282–289
zeros, 284–289
z-plane pole / zero properties, 288–289
z-transform, 270–282
IIR (infinite impulse response) filters, pitfalls in building
coefficient quantization, 293–295
deadband effects, 293
Direct Form implementations, 292–293
dither sequences, 294
finite word length errors, 293–295
limit cycles, 293
limited-precision coefficients, 293
overflow, 293–295
overflow oscillations, 293
overview, 292–293
rounding off, 293
IIR (infinite impulse response) filters, structures
biquad filters, 299
cascade filter properties, 295–297
cascaded, 295–299
cascade/parallel combinations, 295–297
changing, 291–292
Direct Form 1, 275–278, 289
Direct Form II, 289–292
optimizing partitioning, 297–299
parallel filter properties, 295–297
transposed, 291–292
transposed Direct Form II, 289–290
transposition theorem, 291–292
Imaginary numbers, 439, 446
Imaginary part, quadrature signals, 440, 454–455
Impulse invariance method, designing IIR filters
aliasing, 304–305
analytical methods, 302
definition, 257
Method 1, description, 305–307
Method 1, example, 310–313
Method 2, description, 307–310
Method 2, example, 313–319
preferred method, 317
process description, 303–310
prototype filters, 303
Impulse response
convolution in FIR filters, 177–178
definition, 898–899
FIR filters, 177–179
Hilbert transforms, 487–489
Incoherent signal averaging. See Signal averaging, incoherent.
Infinite impulse response (IIR) filters. See IIR (infinite impulse response) filters.
Integer plus fraction fixed-point binary formats, 629
Integration gain, signal averaging, 600–603
Integrators
CIC filters, 553
overview, 370
performance comparison, 373–376
rectangular rule, 371–372
Simpson’s rule, 372, 373–376
Tick’s rule, 373–376
trapezoidal rule, 372
Intermodulation distortion, 16
Interpolated analytic signals, computing, 781
Interpolated FIR (IFIR) filters. See IFIR (interpolated FIR) filters.
Interpolated real signals, interpolating, 779–780
Interpolation. See also Decimation.
accuracy, 519
bandpass signals, 728–730
combining with decimation, 521–522
definition, 384, 508
drawing upsampled spectra, 520–521
exact, 778–781
frequency properties, 519
history of, 519
linear, 815–816
multirate filters, 521–522
overview, 516–518
sample rate converters, 521–522
time properties, 519
time-domain, 778–781
unwanted spectral images, 519
upsampling, 517–518, 520–521
zero stuffing, 518
Interpolation filters, 518
Inverse DFT, 80–81
Inverse discrete Fourier transform (IDFT), 80–81
Inverse FFT, 699–702, 831–833
Inverse of complex numbers, 853
Inverse sinc filters, 563–566
I/Q demodulation, quadrature signals, 459–462
J
Jacobsen, Eric, 775
j-operator, quadrature signals, 439, 444–450
K
Kaiser, James, 270
Kaiser windows, in FIR filter design, 197–201
Kaiser-Bessel windows, in FIR filter design, 197
Kelvin, Lord, 60
Kootsookos, Peter, 603, 724
Kotelnikov, V., 42
L
Lanczos differentiators, 366–367
Laplace transfer function
conditional stability, 268
description, 262–263
determining system stability, 263–264, 268
impulse invariance design, Method 1, 305–307, 310–313
impulse invariance design, Method 2, 307–310, 313–319
in parallel filters, 295–297
second order, 265–268
Laplace transform. See also Z-transform.
bilateral transform, 258
causal systems, 258
conditional stability, 268
for continuous time-domain, 258–259
description, 257–263
development of, 257
one-sided transform, 258
one-sided/causal, 258
poles on the s-plane, 263–270
stability, 263–270
two-sided transform, 258
zeros on the s-plane, 263–270
Laplace variable, complex frequency, 258
Leakage. See DFT leakage.
Leaky integrator, 614
Least significant bit (lsb), 624
l’Hopital’s Rule, 110
Limit cycles, 293
Linear, definition, 12
Linear differential equations, solving. See Laplace transform.
Linear interpolation, 815–816
Linear phase filters, 899
Linear systems, example, 13–14
Linear time-invariant (LTI) systems. See LTI (linear time-invariant) systems.
Linearity, DFT, 75
Linear-phase filters
DC removal, 812–815
definition, 899
Logarithms
and complex numbers, 854–856
measuring signal power, 191
Lowpass design
designing FIR filters, 186–201
IFIR filters, example, 389–391
Lowpass filters, definition, 899
Lowpass signals
definition, 38
sampling, 38–42
lsb (least significant bit), 624
LTI (linear time-invariant) systems
analyzing, 19–21
commutative property, 18–19
convolution, 19
DFT (discrete Fourier transform), 19
discrete linear systems, 12–16
frequency response, determining, 19
homogeneity property, 12
intermodulation distortion, 16
internally generated sinusoids, 16
linear, definition, 12
linear system, example, 13–14
nonlinear system, example, 14–16
output sequence, determining, 19
overview, 12
proportionality characteristic, 12
rearranging sequential order, 18–19
time-invariant systems, 17–18
unit impulse response, 19–20
M
MAC (multiply and accumulate) architecture
polynomial evaluation, 773
programmable DSP chips, 333
Magnitude
approximation (vector), 679–683
of complex numbers, 848
definition, 8–9
DFT, 75–76
Magnitude and angle form of complex numbers, 848–850
Magnitude response of DFTs
aliased sinc function, 108
all-ones rectangular functions, 115–118
fluctuations. See Scalloping.
general rectangular functions, 106–112
overview, 105–106
sidelobe magnitudes, 110–111
symmetrical rectangular functions, 112–115
Magnitude response of DFTs, Dirichlet kernel
all-ones rectangular functions, 115–118, 120
general rectangular functions, 108–112
symmetrical rectangular functions, 113–114
Magnitude-angle form, quadrature signals, 442
Mantissa, floating-point binary formats, 652
Matched filters
definition, 376
example, 378–380
implementation considerations, 380
peak detection threshold, 377, 379–380
properties, 376–378
purpose, 376
SNR (signal-power-to-noise-power ratio), maximizing, 376
McClellan, James, 206. See also Parks-McClellan algorithm.
Mean (statistical measure of noise)
definition, 868–869
PDF (probability density function), 879–882
of random functions, 879–882
Mean (statistical average), of random functions, 879–882
Mehrnia, A., 386
MIL-STD 1750A, floating-point binary formats, 654–655
Missing
A/D conversion codes, checking, 715–716
sample data, recovering, 823–826. See also Interpolation.
Mixing. See Frequency translation.
Modeling FSF (frequency sampling filters), 413–414
Modulation, quadrature signals, 453–454
Modulus of complex numbers, 848
Most significant bit (msb), 624
Moving averages
CIC filters, 551–552
as digital lowpass filters, 20–21, 173, 231
sample rate conversion, CIC filters, 551–552
Moving averages, coherent signal averaging
exponential moving averages, computing, 801–802
exponential signal averaging, 801–802
moving averages, computing, 799–801
nonrecursive moving averagers, 606–608
recursive moving averagers, 606–608
time-domain averaging, 604–608
msb (most significant bit), 624
Multiplication
block diagram symbol, 10
CIC filters, simplified, 765–770
complex numbers, 850–851
Multirate filters
decimation, 521–522
interpolation, 521–522
Multirate systems, sample rate conversion
filter mathematical notation, 534–535
signal mathematical notation, 533–534
z-transform analysis, 533–535
Multirate systems, two-stage decimation, 511
N
Narrowband differentiators, 366–367
Narrowband noise filters, 792–797
Natural logarithms of complex numbers, 854
Negative frequency, quadrature signals, 450–451
Negative values in binary numbers, 625–626
Newton, Isaac, 773
Newton’s method, 372
Noble identities, polyphase filters, 536
Noise
definition, 589
measuring. See Statistical measures of noise.
random, 868
Noise shaping property, 765
Nonlinear systems, example, 14–16
Nonrecursive CIC filters
description, 765–768
prime-factor-R technique, 768–770
Nonrecursive filters. See FIR filters
Nonrecursive moving averagers, 606–608
Normal distribution of random data, generating, 722–724
Normal PDFs, 882–883
Normalized angle variable, 118–119
Notch filters. See Band reject filters.
Nyquist, H., 42
Nyquist criterion, sampling lowpass signals, 40
O
Octal (base 8) numbers, 624–625
Offset fixed-point binary formats, 627–628
1.15 fixed-point binary format, 630–632
Optimal design method, designing FIR filters, 204–207
Optimal FIR filters, 418
Optimization method, designing IIR filters
definition, 257
description, 302
iterative optimization, 330
process description, 330–332
Optimized butterflies, 156
Optimized wideband differentiators, 369–370
Optimum sampling frequency, 46
Order
of filters, 897
polyphase filters, swapping, 536–537
Orthogonality, quadrature signals, 448
Oscillation, quadrature signals, 459–462
Oscillator, quadrature
coupled, 787
overview, 786–789
Taylor series approximation, 788
Overflow
computing the magnitude of complex numbers, 815
fixed-point binary formats, 629, 642–646
two’s complement, 559–563
Overflow errors, 293–295
Overflow oscillations, 293
Oversampling A/D converter quantization noise, 704–706
P
Parallel filters, Laplace transfer function, 295–297
Parks-McClellan algorithm
designing FIR filters, 204–207
vs. FSF (frequency sampling filters), 392
optimized wideband differentiators, 369–370
Parzen windows. See Triangular windows.
Passband, definition, 900
Passband filters, definition, 900
Passband gain, FIR filters, 233–234
Passband ripples
cascaded filters, estimating, 296–297
definition, 296, 900
IFIR filters, 390
minimizing, 190–194, 204–207
PDF (probability density function)
Gaussian, 882–883
mean, calculating, 879–882
mean and variance of random functions, 879–882
normal, 882–883
variance, calculating, 879–882
Peak correlation, matched filters, 379
Peak detection threshold, matched filters, 377, 379–380
Periodic sampling
aliasing, 33–38
frequency-domain ambiguity, 33–38
Periodic sampling
1st-order sampling, 46
anti-aliasing filters, 42
bandpass, 43–49
coherent sampling, 711
definition, 43
folding frequencies, 40
Nyquist criterion, 40
optimum sampling frequency, 46
real signals, 46
sampling translation, 44
SNR (signal-to-noise) ratio, 48–49
spectral inversion, 46–47
undersampling, 40
Phase angles, signal averaging, 603–604
Phase delay. See Phase response.
Phase response
definition, 900
in FIR filters, 209–214
Phase unwrapping, FIR filters, 210
Phase wrapping, FIR filters, 209, 900
Pi, calculating, 23
Picket fence effect, 97
Pisa, Leonardo da, 450–451
Polar form
complex numbers, vs. rectangular, 856–857
quadrature signals, 442, 443–444
Poles
IIR filters, 284–289
on the s-plane, Laplace transform, 263–270
Polynomial curve fitting, 372
Polynomial evaluation
binary shift multiplication/division, 773–774
Estrin’s Method, 774–775
Horner’s Rule, 772–774
MAC (multiply and accumulate) architecture, 773
Polynomial factoring, CIC filters, 765–770
Polynomials, finding the roots of, 372
Polyphase decomposition
CIC filters, 765–770
definition, 526
diagrams, 538–539
two-stage decimation, 514
Polyphase filters
benefits of, 539
commutator model, 524
implementing, 535–540
issues with, 526
noble identities, 536
order, swapping, 536–537
overview, 522–528
polyphase decomposition, 526, 538–539
prototype FIR filters, 522
uses for, 522
Power, signal. See also Decibels.
absolute, 891–892
definition, 9
relative, 885–889
Power spectrum, 63, 140–141
Preconditioning FIR filters, 563–566
Prewarp, 329
Prime decomposition, CIC filters, 768–770
Prime factorization, CIC filters, 768–770
Probability density function (PDF). See PDF (probability density function).
Processing gain or loss. See DFT processing gain; Gain; Loss.
Prototype filters
analog, 303
FIR polyphase filters, 522
IFIR filters, 382
Q
Q30 fixed-point binary formats, 629
Q-channel filters, analytic signals, 496
Quadratic factorization formula, 266, 282
Quadrature component, 454–455
Quadrature demodulation, 455, 456–462
Quadrature filters, definition, 900
Quadrature mixing, 455
Quadrature oscillation, 459–462
Quadrature oscillator
coupled, 787
overview, 786–789
Taylor series approximation, 788
Quadrature phase, 440
Quadrature processing, 440
Quadrature sampling block diagram, 459–462
Quadrature signals. See also Complex numbers.
analytic, 455
Argand plane, 440–441
bandpass signals in the frequency-domain, 454–455
Cartesian form, 442
complex exponentials, 447
complex mixing, 455
complex number notation, 440–446
complex phasors, 446–450
complex plane, 440–441, 446
decimation, in frequency translation, 781–783
definition, 439
demodulation, 453–454
detection, 453–454
down-conversion. See Down-conversion, quadrature signals.
Euler’s identity, 442–443, 449, 453
exponential form, 442
in the frequency domain, 451–454
generating from real signals. See Hilbert transforms.
generation, 453–454
imaginary part, 440, 454–455
in-phase component, 440, 454–455
I/Q demodulation, 459–462
j-operator, 439, 444–450
magnitude-angle form, 442
mixing to baseband, 455
modulation, 453–454
negative frequency, 450–451
orthogonality, 448
polar form, 442, 443–444
positive frequency, 451
real axis, 440
real part, 440, 454–455
rectangular form, 442
representing real signals, 446–450
sampling scheme, advantages of, 459–462
simplifying mathematical analysis, 443–444
three-dimensional frequency-domain representation, 451–454
trigonometric form, 442, 444
uses for, 439–440
Quantization
coefficient/errors, 293–295
noise. See A/D converters, quantization noise.
real-time DC removal, 763–765
R
Radix points, fixed-point binary formats, 629
Radix-2 algorithm, FFT
butterfly structures, 151–154
computing large DFTs, 826–829
decimation-in-frequency algorithms, 151–154
decimation-in-time algorithms, 151–154
derivation of, 141–149
FFT (fast Fourier transform), 151–158
twiddle factors, 143–149
Raised cosine windows. See Hanning windows.
Random data
Central Limit Theory, 723
generating a normal distribution of, 722–724
Random functions, mean and variance, 879–882
Random noise, 868. See also SNR (signal-to-noise ratio).
Real numbers
definition, 440
graphical representation of, 847–848
Real sampling, 46
Real signals
bandpass sampling, 46
decimation, in frequency translation, 781
generating complex signals from. See Hilbert transforms.
representing with quadrature signals, 446–450
Rectangular form of complex numbers
definition, 848–850
vs. polar form, 856–857
Rectangular form of quadrature signals, 442
Rectangular functions
all ones, 115–118
DFT, 105–112
frequency axis, 118–120
general, 106–112
overview, 105–106
symmetrical, 112–115
time axis, 118–120
Rectangular windows, 89–97, 686
Recursive filters. See IIR filters
Recursive moving averagers, 606–608
Recursive running sum filters, 551–552
Remez Exchange, 204–207, 418
Replications, spectral. See Spectral replications.
Resolution, DFT, 77, 98–102
Ripples
in Bessel-derived filters, 901
in Butterworth-derived filters, 901
in Chebyshev-derived filters, 900
definition, 900–901
designing FIR filters, 190–194
in Elliptic-derived filters, 900
equiripple, 418, 901
out-of-band, 901
in the passband, 900
in the stopband, 901
rms value of continuous sinewaves, 874–875
Roll-off, definition, 901
Roots of
complex numbers, 853–854
polynomials, 372
Rosetta Stone, 450
Rounding fixed-point binary numbers
convergent rounding, 651
data rounding, 649–652
effective bits, 641
round off noise, 636–637
round to even method, 651
round-to-nearest method, 650–651
Roundoff errors, 293
S
Sample rate conversion. See also Polyphase filters.
decreasing. See Decimation.
definition, 507
with IFIR filters, 548–550
increasing. See Interpolation.
missing data, recovering, 823–826. See also Interpolation.
by rational factors, 540–543
Sample rate conversion, multirate systems
filter mathematical notation, 534–535
signal mathematical notation, 533–534
z-transform analysis, 533–535
Sample rate conversion, with half-band filters
folded FIR filters, 548
fundamentals, 544–546
implementation, 546–548
overview, 543
Sample rate converters, 521–522
Sampling, periodic. See Periodic sampling.
Sampling translation, 44
Sampling with digital mixing, 462–464
Scaling IIR filter gain, 300–302
Scalloping loss, 96–97
SDFT (sliding DFT)
algorithm, 742–746
overview, 741
stability, 746–747
SFDR (spurious free dynamic range), 714–715
Shannon, Claude, 42
Shape factor, 901
Sharpened FIR filters, 726–728
Shifting theorem, DFT, 77–78
Shift-invariant systems. See Time-invariant systems.
Sidelobe magnitudes, 110–111
Sidelobes
Blackman window and, 194–197
DFT leakage, 83, 89
FIR (finite impulse response) filters, 184
ripples, in low-pass FIR filters, 193–194
Sign extend operations, 627
Signal averaging. See also SNR (signal-to-noise ratio).
equation, 589
frequency-domain. See Signal averaging, incoherent.
integration gain, 600–603
mathematics, 592–594, 599
multiple FFTs, 600–603
phase angles, 603–604
postdetection. See Signal averaging, incoherent.
quantifying noise reduction, 594–597
rms. See Signal averaging, incoherent.
scalar. See Signal averaging, incoherent.
standard deviation, 590
time-domain. See Signal averaging, coherent.
time-synchronous. See Signal averaging, coherent.
variance, 589–590
video. See Signal averaging, incoherent.
Signal averaging, coherent
exponential averagers, 608–612
exponential moving averages, computing, 801–802
exponential smoothing, 608
filtering aspects, 604–608
moving averagers, 604–608
moving averages, computing, 799–801
nonrecursive moving averagers, 606–608
overview, 590–597
recursive moving averagers, 606–608
reducing measurement uncertainty, 593, 604–608
time-domain filters, 609–612
true signal level, 604–608
weighting factors, 608, 789
Signal averaging, exponential
1st-order IIR filters, 612–614
dual-mode technique, 791
example, 614
exponential smoothing, 608
frequency-domain filters, 612–614
moving average, computing, 801–802
multiplier-free technique, 790–791
overview, 608
single-multiply technique, 789–790
Signal averaging, incoherent
1st-order IIR filters, 612–614
example, 614
frequency-domain filters, 612–614
overview, 597–599
Signal averaging, with FIR filters
convolution, 175–176
example, 170–174, 183–184
as a lowpass filter, 180–182
performance improvement, 178
Signal envelope, Hilbert transforms, 483–495
Signal power. See also Decibels.
absolute, 891–892
relative, 885–889
Signal processing
analog, 2. See also Continuous signals.
definition, 2
digital, 2
operational symbols, 10–11
Signal transition detection, 820–821
Signal variance
biased and unbiased, computing, 797–799, 799–801
definition, 868–870
exponential, computing, 801–802
PDF (probability density function), 879–882
of random functions, 879–882
signal averaging, 589–590
Signal-power-to-noise-power ratio (SNR), maximizing, 376
Signal-to-noise ratio (SNR). See SNR (signal-to-noise ratio).
Sign-magnitude, fixed-point binary formats, 625–626
Simpson, Thomas, 372
SINAD (signal-to-noise-and-distortion), 711–714
Sinc filters. See CIC (cascaded integrator-comb) filters.
Sinc functions, 83, 89, 116
Single tone detection, FFT method
drawbacks, 737–738
vs. Goertzel algorithm, 740–741
Single tone detection, Goertzel algorithm
advantages of, 739
description, 738–740
example, 740
vs. the FFT, 740–741
stability, 838–840
Single tone detection, spectrum analysis, 737–741
Single-decimation down-conversion, 819–820
Single-multiply technique, exponential signal averaging, 789–790
Single-stage decimation, vs. two-stage, 514
Single-stage interpolation, vs. two-stage, 532
Sliding DFT (SDFT). See SDFT (sliding DFT).
Slope detection, 820-821
Smoothing impulsive noise, 770–772
SNDR. See SINAD (signal-to-noise-and-distortion).
SNR (signal-to-noise ratio)
vs. A/D converter, fixed-point binary finite word lengths, 640–642
A/D converters, 711–714
bandpass sampling, 48–49
block averaging, 770
corrected mean, 771
DFT processing gain, 103–104
IIR filters, 302
measuring. See Statistical measures of noise.
reducing. See Signal averaging.
smoothing impulsive noise, 770–772
SNR (signal-power-to-noise-power ratio), maximizing, 376
Software programs, fast Fourier transform, 141
Someya, I., 42
Spectral inversion
around signal center frequency, 821–823
bandpass sampling, 46–47
Spectral leakage, FFTs, 138–139, 683–686. See also DFT leakage.
Spectral leakage reduction
A/D converter testing techniques, 710–711
Blackman windows, 686
frequency domain, 683–686
Spectral peak location
estimating, algorithm for, 730–734
Hamming windows, 733
Hanning windows, 733
Spectral replications
bandpass sampling, 44–45
sampling lowpass signals, 39–40
Spectral vernier. See Zoom FFT.
Spectrum analysis. See also SDFT (sliding DFT); Zoom FFT.
center frequencies, expanding, 748–749
with SDFT (sliding DFT), 748–749
single tone detection, 737–741
weighted overlap-add, 755
windowed-presum FFT, 755
Zoom FFT, 749–753
Spectrum analyzer, 753–756
Spurious free dynamic range (SFDR), 714–715
Stability
comb filters, 403–404
conditional, 268
FSF (frequency sampling filters), 403–406
IIR filters, 263–270
Laplace transfer function, 263–264, 268
Laplace transform, 263–270
SDFT (sliding DFT), 746–747
single tone detection, 838–840
z-transform and, 272–274, 277
Stair-step effect, A/D converter quantization noise, 637
Standard deviation
of continuous sinewaves, 874–875
definition, 870
signal averaging, 590
Statistical measures of noise
average, 868–870
average power in electrical circuits, 874–875
Bessel’s correction, 870–871
biased estimates, 870–871
dispersion, 869
fluctuations around the average, 869
overview, 867–870. See also SNR (signal-to-noise ratio).
of real-valued sequences, 874
rms value of continuous sinewaves, 874–875
of short sequences, 870–871
standard deviation, definition, 870
standard deviation, of continuous sinewaves, 874–875
summed sequences, 872–874
unbiased estimates, 871
Statistical measures of noise, estimating SNR
for common devices, 876
controlling SNR test signals, 879
in the frequency domain, 877–879
overview, 875–876
in the time domain, 876–877
Statistical measures of noise, mean
definition, 868–869
PDF (probability density function), 879–882
of random functions, 879–882
Statistical measures of noise, variance. See also Signal variance.
definition, 868–870
PDF (probability density function), 879–882
of random functions, 879–882
Steinmetz, Charles P., 446
Stockham, Thomas, 716
Stopband, definition, 901
Stopband ripples
definition, 901
minimizing, 204–207
Stopband sidelobe level suppression, 416
Structure, definition, 901
Structures, IIR filters
biquad filters, 299
cascade filter properties, 295–297
cascaded, 295–299
cascade/parallel combinations, 295–297
changing, 291–292
Direct Form 1, 275–278, 289
Direct Form II, 289–292
optimizing partitioning, 297–299
parallel filter properties, 295–297
transposed, 291–292
transposed Direct Form II, 289–290
transposition theorem, 291–292
Sub-Nyquist sampling. See Bandpass sampling.
Substructure sharing, 765–770
Subtraction
block diagram symbol, 10
complex numbers, 850
Summation
block diagram symbol, 10
description, 11
equation, 10
notation, 11
Symbols
block diagram, 10–11
signal processing, 10–11
Symmetrical rectangular functions, 112–115
Symmetrical-coefficient FIR filters, 232–233
Symmetry, DFT, 73–75
T
Tacoma Narrows Bridge collapse, 263
Tap, definition, 901
Tap weights. See Filter coefficients.
Tapped delay, FIR filters, 174, 181–182
Taylor series approximation, 788
Tchebyschev function, definition, 902
Tchebyschev windows, in FIR filter design, 197
Time data, manipulating in FFTs, 138–139
Time invariance, decimation, 514
Time properties
decimation, 514–515
interpolation, 519
Time representation, continuous vs. discrete systems, 5
Time reversal, 863–865
Time sequences, notation syntax, 7
Time-domain
aliasing, avoiding, 718–722
analytic signals, generating, 495–497
coefficients, determining, 186–194
convolution, matched filters, 380
convolution vs. frequency-domain multiplication, 191–194
equations, example, 7
FIR filter implementation, 489–494
Hilbert transforms, designing, 489–494
interpolation, 778–781
slope filters, 820–821
Time-domain data, converting
from frequency-domain data. See IDFT (inverse discrete Fourier transform).
to frequency-domain data. See DFT (discrete Fourier transform).
Time-domain filters
coherent signal averaging, 609–612
exponential signal averaging, 609–612
Time-domain signals
amplitude, determining, 140
continuous, Laplace transform for, 258
DC removal, 812–815
definition, 4
vs. frequency-domain, 120–123
Time-invariant systems. See also LTI (linear time-invariant) systems.
analyzing, 19–21
commutative property, 18–19
definition, 17–18
example of, 17–18
Tone detection. See Single tone detection.
Transfer functions. See also Laplace transfer function.
definition, 902
real FSF, 908–909
z-domain, 282–289
Transient response, FIR filters, 181–182
Transition region, definition, 902
Translation, sampling, 44
Transposed Direct Form II filters, 289–290
Transposed Direct Form II structure, 289–290
Transposed filters, 291–292
Transposed structures, 765–770
Transposition theorem, 291–292
Transversal filters, 173–174. See also FIR (finite impulse response) filters.
Triangular dither, 708
Triangular windows, 89–93
Trigonometric form, quadrature signals, 442, 444
Trigonometric form of complex numbers, 848–850
Truncation, fixed-point binary numbers, 646–649
Tukey, J., 135
Two’s complement
fixed-point binary formats, 626–627, 629
overflow, 559–563
Two-sided Laplace transform, 258
Type-IV FSF
examples, 419–420, 423–426
frequency response, 910–912
optimum transition coefficients, 913–926
U
Unbiased estimates, 871
Unbiased signal variance, computing, 797–799, 799–801
Undersampling lowpass signals, 40. See also Bandpass sampling.
Uniform windows. See Rectangular windows.
Unit circles
definition, 271
z-transform, 271
Unit circles, FSF
forcing poles and zeros inside, 405
pole / zero cancellation, 395–398
Unit delay
block diagram symbol, 10
description, 11
Unit impulse response, LTI, 19–20
Unnormalized fractions, floating-point binary formats, 656
Unwrapping, phase, 210
Upsampling, interpolation, 517–518, 520–521
V
Variance. See Signal variance.
Vector, definition, 848
Vector rotation with arctangents
to the 1st octant, 805–808
division by zero, avoiding, 808
jump address index bits, 807
overview, 805
by ±π/8, 809–810
rotational symmetries, 807
Vector-magnitude approximation, 679–683
von Hann windows. See Hanning windows.
W
Warping, frequency, 319, 321–325, 328–330
Weighted overlap-add spectrum analysis, 755
Weighting factors, coherent signal averaging, 608, 789
Wideband compensation, 564
Wideband differentiators, 367–370
Willson, A., 386
Window design method, FIR filters, 186–194
Windowed-presum FFT spectrum analysis, 755
Windows
Blackman, 195–201, 686, 733
Blackman-Harris, 686, 733
exact Blackman, 686
FFTs, 139
in the frequency domain, 683–686
magnitude response, 92–93
mathematical expressions of, 91
minimizing DFT leakage, 89–97
processing gain or loss, 92
purpose of, 96
rectangular, 89–97, 686
selecting, 96
triangular, 89–93
Windows, Hamming
description, 89–93
DFT leakage reduction, 89–93
in the frequency domain, 683–686
spectral peak location, 733
Windows, Hanning
description, 89–97
DFT leakage, minimizing, 89–97
in the frequency domain, 683–686
spectral peak location, 733
Windows used in FIR filter design
Bessel functions, 198–199
Blackman, 195–201
Chebyshev, 197–201, 927–930
choosing, 199–201
Dolph-Chebyshev, 197
Kaiser, 197–201
Kaiser-Bessel, 197
Tchebyschev, 197
Wingless butterflies, 156
Wraparound leakage, 86–88
Wrapping, phase, 209, 900
Z
z-domain expression for Mth-order IIR filter, 275–276
z-domain transfer function, IIR filters, 282–289
Zero padding
alleviating scalloping loss, 97–102
FFTs, 138–139
FIR filters, 228–230
improving DFT frequency granularity, 97–102
spectral peak location, 731
Zero stuffing
interpolation, 518
narrowband lowpass filters, 834–836
Zero-overhead looping
DSP chips, 333
FSF (frequency sampling filters), 422–423
IFIR filters, 389
Zero-phase filters
definition, 902
techniques, 725
Zeros
IIR filters, 284–289
on the s-plane, Laplace transform, 263–270
Zoom FFT, 749–753
Zoom FFT, 749–753
z-plane pole / zero properties, IIR filters, 288–289
z-transform. See also Laplace transform.
definition, 270
description of, 270–272
FIR filters, 288–289
IIR filters, 270–282
infinite impulse response, definition, 280
polar form, 271
poles, 272–274
unit circles, 271
zeros, 272–274
z-transform, analyzing IIR filters
digital filter stability, 272–274, 277
Direct Form 1 structure, 275–278
example, 278–282
frequency response, 277–278
overview, 274–275
time delay, 274–278
z-domain transfer function, 275–278, 279–280
