# Digital Audio DSP (Zölzer) — Chunk 10/30

> Source: `Udo-Zolzer-digital-audio-signal-processing.9780470997857.40435.pdf` · Pages ~1-331 · Extractor: pdfminer.six
> Target ≈ 2800 words, Overlap = 200 words · Actual = 24307

> Note: DSP textbook content for SpectralCanvasPro RT-audio knowledge base

---
50
x 10(cid:358)4
Quantization
(cid:111)
)
n
(
x
(cid:111)
)
n
(
x
Q
+Q
(cid:358)Q
100
200
300
400
0
x 10(cid:358)4
500
n (cid:111)
600
700
800
900
1000
1
0
(cid:358)1
1
0
(cid:358)1
0
100
200
300
400
(cid:358)80
(cid:358)100
(cid:358)120
(cid:358)140
(cid:111)
B
d
n
i
|
)
f
/
f
(
S
S
Q
Q
X
X
|
0
0.05
0.1
0.15
0.2
600
700
800
900
1000
0.3
0.35
0.4
0.45
500
n (cid:111)
0.25
f/f
S (cid:111)
Figure 2.34 Psychoacoustic noise shaping: signal x(n), quantized signal xQ(n) and power density
spectrum of quantized signal.
,(cid:16)(cid:7)
2(cid:22)
(cid:5)(cid:23) 3
23
(cid:23)I
(cid:5)(cid:22)
(cid:14)(cid:23)
(cid:5)(cid:23)
(cid:14)(cid:23)
(cid:23)
(cid:22)
3
(cid:5)(cid:23)I
(cid:14)(cid:23)
(cid:5)23
(cid:14)(cid:23)
(cid:5)2(cid:22)
(cid:14)(cid:23)
(cid:6)(cid:16)(cid:3)(cid:10)(cid:2)-(cid:14)2'(cid:18)(cid:14)$(cid:11)(cid:12)(cid:28)(cid:4)(cid:2)(cid:12)(cid:2)(cid:10)(cid:7)
(cid:14)(cid:23) (cid:5)(cid:22)
(cid:5)(cid:23)
(cid:14)(cid:23)
(cid:5)2
(cid:14)(cid:23)
(cid:5)23
(cid:14)(cid:23)
(cid:5)2(cid:22)
(cid:14)(cid:23)
(cid:5)2(cid:23)
(cid:14)(cid:23)
!(cid:10)(cid:18)(cid:16)(cid:3)(cid:10)(cid:2)-(cid:14)2'(cid:18)(cid:14)$(cid:11)(cid:12)(cid:28)(cid:4)(cid:2)(cid:12)(cid:2)(cid:10)(cid:7)
(cid:5)(cid:23) 2(cid:22)
(cid:14)(cid:23) 23
(cid:14)(cid:23) (cid:23)I
(cid:14)(cid:23) (cid:23)
(cid:22)(cid:14)(cid:23)
(cid:14)(cid:23) 3
(cid:6)(cid:16)(cid:3)(cid:10)(cid:2)-(cid:14)(cid:27)(cid:10)(cid:7)(cid:2)(cid:3)(cid:2)(cid:8)
2(cid:22)
(cid:14)(cid:23)
(cid:14)(cid:23) 23
(cid:14)(cid:23) (cid:23)I
(cid:14)(cid:23) (cid:23)
(cid:22)(cid:14)(cid:23)
(cid:14)(cid:23) 3
!(cid:10)(cid:18)(cid:16)(cid:3)(cid:10)(cid:2)-(cid:14)(cid:27)(cid:10)(cid:7)(cid:2)(cid:3)(cid:2)(cid:8)
Figure 2.35 Fixed-point formats.
negative numbers b0 equals 1. For a 3-bit quantization (see Fig. 2.38), a quantized value can
be represented by xQ = −b0 + b1 · 0.5 + b2 · 0.25. The smallest quantization step size is
0.25. For a positive number 0.75 it follows that 0.75 = −0 + 1 · 0.5 + 1 · 0.25. The binary
coding for 0.75 is 011.

2.4 Number Representation

’
)
(cid:6)(cid:16)(cid:3)(cid:10)(cid:2)-(cid:14)2'(cid:18)(cid:14)$(cid:11)(cid:12)(cid:28)(cid:4)(cid:2)(cid:12)(cid:2)(cid:10)(cid:7)
!(cid:10)(cid:18)(cid:16)(cid:3)(cid:10)(cid:2)-(cid:14)2'(cid:18)(cid:14)$(cid:11)(cid:12)(cid:28)(cid:4)(cid:2)(cid:12)(cid:2)(cid:10)(cid:7)
’
)
(cid:5);
(cid:22)(cid:5)(cid:23)
(cid:5)7;(cid:5)(cid:22):
(cid:22)(cid:5)(cid:23)
(cid:5)(cid:22)
(cid:5)(cid:22)
(cid:22)
(cid:23)
2
<
(cid:6)(cid:16)(cid:3)(cid:10)(cid:2)-(cid:14)(cid:27)(cid:10)(cid:7)(cid:2)(cid:3)(cid:2)(cid:8)
’
)
(cid:14);(cid:5)(cid:22)
(cid:5)(cid:22)9(cid:23)
(cid:5)(cid:22)
(cid:14);(cid:5)(cid:22)
(cid:14)(cid:5)(cid:23)
(cid:22)
(cid:23)
2
<
’
’
3
;
(cid:23)(cid:14)(cid:14)(cid:14)(cid:14)(cid:5)(cid:22)
3
(cid:22)
(cid:23)
’)
!(cid:10)(cid:18)(cid:16)(cid:3)(cid:10)(cid:2)-(cid:14)(cid:27)(cid:10)(cid:7)(cid:2)(cid:3)(cid:2)(cid:8)
(cid:22)
(cid:23)
51
’
’
Figure 2.36 Number range.
Rounding
Truncation
4
3
2
1
0
-1
-2
-3
→
/
Q
Q
x
4
3
2
1
0
-1
-2
-3
→
/
Q
Q
x
-4
-4
-3
-2
-1
1
0
x/Q →
2
3
4
-4
-4
-3
-2
-1
1
0
x/Q →
2
3
4
Rounding (error e)
Truncation (error e)
1
0.8
0.6
0.4
0.2
0
-0.2
-0.4
-0.6
-0.8
→
Q
/
)
x
-
Q
x
(
1
0.8
0.6
0.4
0.2
0
-0.2
-0.4
-0.6
-0.8
→
Q
/
)
x
-
Q
x
(
-1
-4
-3
-2
-1
1
0
x/Q →
2
3
4
-1
-4
-3
-2
-1
1
0
x/Q →
2
3
4
Figure 2.37 Rounding and truncation curves.
52
Quantization
011
010
001
000
111
110
0
x
+xmax
+1
0
-1
xQ
e
101
100
-xmax
0.125
0
-0.125
-xmax
0
x
+xmax
Figure 2.38 Rounding curve and error signal for w = 3 bits.
Dynamic Range. The dynamic range of a number representation is deﬁned as the ratio of
maximum to minimum number. For ﬁxed-point representation with
xQ max = (1 − 2
xQ min = 2
−(w−1),
−(w−1)),
the dynamic range is given by
DRF = 20 log10
(cid:2)
(cid:1)
xQ max
xQ min
= 20 log10
(cid:1)
(cid:2)
1 − Q
Q
= 20 log10(2w−1 − 1) dB.
(2.132)
(2.133)
(2.134)
Multiplication and Addition of Fixed-point Numbers. For the multiplication of two
ﬁxed-point numbers in the range from −1 to +1, the result is always less than 1. For
the addition of two ﬁxed-point numbers, care must be taken for the result to remain in
the range from −1 to+ 1. An addition of 0.6 + 0.7 = 1.3 must be carried out in the
form 0.5(0.6 + 0.7) = 0.65. This multiplication with the factor 0.5 or generally 2−s is
called scaling. An integer in the range from 1 to, for instance, 8 is chosen for the scaling
coefﬁcient s.
Error Model. The quantization process for ﬁxed-point numbers can be approximated as
an the addition of an error signal e(n) to the signal x(n) (see Fig. 2.39). The error signal is
a random signal with white power density spectrum.
2.4 Number Representation
53
x(n)
Q
x (n)Q
x(n)
x (n)=x(n)+e(n)
Q
e(n)
Figure 2.39 Model of a ﬁxed-point quantizer.
Signal-to-noise Ratio. The signal-to-noise ratio for a ﬁxed-point quantizer is deﬁned by
SNR = 10 log10
(cid:1)
(cid:2)
,
σ 2
X
σ 2
E
where σ 2
X is the signal power and σ 2
E is the noise power.
2.4.2 Floating-point Number Representation
The representation of a ﬂoating-point number is given by
with
xQ = MG 2EG
0.5 ≤ MG < 1,
(2.135)
(2.136)
(2.137)
where MG denotes the normalized mantissa and EG the exponent. The normalized standard
format (IEEE) is shown in Fig. 2.40 and special cases are given in Table 2.3. The
mantissa M is implemented with a word-length of wM bits and is in ﬁxed-point number
representation. The exponent E is implemented with a word-length of wE bits and is an
integer in the range from −2wE −1 + 2 to 2wE−1 − 1. For an exponent word-length of
wE = 8 bits, its range of values lies between −126 and +127. The range of values of
the mantissa lies between 0.5 and 1. This is referred to as the normalized mantissa and is
responsible for the unique representation of a number. For a ﬁxed-point number in the range
between 0.5 and 1, it follows that the exponent of the ﬂoating-point number representation
is E = 0. To represent a ﬁxed-point number in the range between 0.25 and 0.5 in ﬂoating-
point form, the range of values of the normalized mantissa M lies between 0.5 and 1, and
for the exponent it follows that E = −1. As an example, for a ﬁxed-point number 0.75 the
ﬂoating-point number 0.75 · 20 results. The ﬁxed-point number 0.375 is not represented
as the ﬂoating-point number 0.375 · 20. With the normalized mantissa, the ﬂoating-point
number is expressed as 0.75 · 2−1. Owing to normalization, the ambiguity of ﬂoating-point
number representation is avoided. Numbers greater than 1 can be represented. For example,
1.5 becomes 0.75 · 21 in ﬂoating-point number representation.
Figure 2.41 shows the rounding and truncations curves for ﬂoating-point representation
and the absolute error e = xQ − x. The curves for ﬂoating-point quantization show
that for small amplitudes small quantization step sizes occur. In contrast to ﬁxed-point
representation, the absolute error is dependent on the input signal.
54
Quantization
.(cid:16)(cid:7)
2(cid:22)
(cid:18)
23
(cid:2)
4
(cid:23)2
(cid:2)
3
(cid:23)(cid:23)
(cid:22)(cid:29) (cid:12)
(cid:23)(cid:23)
(cid:14)3
(cid:12)
3
.(cid:16)(cid:10)(cid:9)(cid:8)"(cid:14)(cid:28)(cid:11)(cid:16)(cid:10)(cid:7)
(cid:21)(cid:16)--(cid:2)(cid:10)(cid:14).(cid:16)(cid:7)
Figure 2.40 Floating-point number representation.
Table 2.3 Special cases of ﬂoating-point number representation.
Exponent
Mantissa Value
Type
NAN
Inﬁnity
255
255
Normal
1 ≤ e ≤ 254
Zero
0
Rounding wM 3 wE 3
4
3
2
1
0
-1
-2
-3
→
Q
x
Undeﬁned
(−1)s inﬁnity
(−1)s (0.m)2e−127
(−1)s · 0
Truncation wM 3 wE 3
(cid:8)= 0
0
Any
0
→
Q
x
4
3
2
1
0
-1
-2
-3
-4
-4
-3
-2
-1
0
x →
1
2
3
4
-4
-4
-3
-2
-1
1
2
3
4
0
x →
Truncation (abs. error) wM 3 wE 3
Rounding (abs. error) wM 3 wE 3
1
0.8
0.6
0.4
0.2
0
-0.2
-0.4
-0.6
-0.8
→
x
-
Q
x
1
0.8
0.6
0.4
0.2
0
-0.2
-0.4
-0.6
-0.8
→
x
-
Q
x
-1
-4
-3
-2
-1
0
x →
1
2
3
4
-1
-4
-3
-2
-1
1
2
3
4
0
x →
Figure 2.41 Rounding and truncation curves for ﬂoating-point representation.
2.4 Number Representation
In the interval
the quantization step is given by
For the relative error
2EG ≤ x < 2EG+1,
QG = 2
−(wM −1)2EG.
er = xQ − x
x
of the ﬂoating-point representation, a constant upper limit can be stated as
|er | ≤ 2
−(wM −1).
Dynamic Range. With the maximum and minimum numbers given by
and
xQ max = (1 − 2
xQ min = 0.5 2EG min,
−(wM −1))2EG max,
EG max = 2wE −1 − 1,
EG min = − 2wE−1 + 2,
the dynamic range for ﬂoating-point representation is given by
(cid:1)
DRG = 20 log10
(cid:2)
(1 − 2−(wM −1))2EG max
0.5 2EG min
−(wM −1))2EG max−EG min+1
−(wM −1))22wE −2 dB.
= 20 log10(1 − 2
= 20 log10(1 − 2
55
(2.138)
(2.139)
(2.140)
(2.141)
(2.142)
(2.143)
(2.144)
(2.145)
(2.146)
Multiplication and Addition of Floating-point Numbers. For multiplications with
ﬂoating-point numbers, the exponents of both numbers xQ1 = M12E1 and xQ2 = M22E2
are added and the mantissas are multiplied. The resulting exponent EG = E1 + E2 is
adjusted so that MG = M1M2 lies in the interval 0.5 ≤ MG < 1. For additions the smaller
number is denormalized to get the same exponent. Then both mantissa are added and the
result is normalized.
Error Model. With the deﬁnition of the relative error er (n) = [xQ(n) − x(n)]/x(n) the
quantized signal can be written as
xQ(n) = x(n) · (1 + er (n)) = x(n) + x(n) · er (n).
(2.147)
Floating-point quantization can be modeled as an additive error signal e(n) = x(n) · er (n)
to the signal x(n) (see Fig. 2.42).
Signal-to-noise Ratio. Under the assumption that the relative error is independent of the
input x, the noise power of the ﬂoating-point quantizer can be written as
σ 2
E
= σ 2
X
· σ 2
Er
.
(2.148)
56
Quantization
x(n)
Q
xQ(n)
x(n)
x (n)= x(n) + x(n). e
Q
r(n)
e (n)
r
Figure 2.42 Model of a ﬂoating-point quantizer.
For the signal-to-noise-ratio, we can derive
SNR = 10 log10
(cid:2)
(cid:1)
σ 2
X
σ 2
E
= 10 log10
(cid:2)
(cid:1)
σ 2
X
· σ 2
Er
σ 2
X
= 10 log10
(cid:1)
(cid:2)
.
1
σ 2
Er
(2.149)
Equation (2.149) shows that the signal-to-noise ratio is independent of the level of the
input. It is only dependent on the noise power σ 2
which, in turn, is only dependent on the
Er
word-length wM of the mantissa of the ﬂoating-point representation.
2.4.3 Effects on Format Conversion and Algorithms
First, a comparison of signal-to-noise ratios is made for the ﬁxed-point and ﬂoating-
point number representation. Figure 2.43 shows the signal-to-noise ratio as a function of
input level for both number representations. The ﬁxed-point word-length is w = 16 bits.
The word-length of the mantissa in ﬂoating-point representation is also wM = 16 bits,
whereas that of the exponent is wE = 4 bits. The signal-to-noise ratio for ﬂoating-point
representation shows that it is independent of input level and varies as a sawtooth curve
in a 6 dB grid. If the input level is so low that a normalization of the mantissa due to
ﬁnite number representation is not possible, then the signal-to-noise ratio is comparable to
ﬁxed-point representation. While using the full range, both ﬁxed-point and ﬂoating-point
result in the same signal-to-noise ratio. It can be observed that the signal-to-noise ratio
for ﬁxed-point representation depends on the input level. This signal-to-noise ratio in the
digital domain is an exact image of the level-dependent signal-to-noise ratio of an analog
signal in the analog domain. A ﬂoating-point representation cannot improve this signal-to-
noise ratio. Rather, the ﬂoating-point curve is vertically shifted downwards to the value of
signal-to-noise ratio of an analog signal.
AD/DA Conversion. Before processing, storing and transmission of audio signals, the
analog audio signal is converted into a digital signal. The precision of this conversion
depends on the word-length w of the AD converter. The resulting signal-to-noise ratio
is 6w dB for uniform PDF inputs. The signal-to-noise ratio in the analog domain depends
on the level. This linear dependence of signal-to-noise ratio on level is preserved after AD
conversion with subsequent ﬁxed-point representation.
Digital Audio Formats. The basis for established digital audio transmission formats is
provided in the previous section on AD/DA conversion. The digital two-channel AES/EBU
2.4 Number Representation
57
wM 16 wE 4
Floating-point
Fixed-point
120
100
80
60
40
20
→
B
d
n
i
R
N
S
0
-120
-100
-80
-60
X in dB →
-40
-20
0
Figure 2.43 Signal-to-noise ratio for an input level.
interface [AES92] and 56-channel MADI interface [AES91] both operate with ﬁxed-point
representation with a word-length of at most 24 bits per channel.
Storage and Transmission. Besides the established storage media like compact disc and
DAT which were exclusively developed for audio application, there are storage systems like
hard discs in computers. These are based on magnetic or magneto-optic principles. The
systems operate with ﬁxed-point number representation. With regard to the transmission
of digital audio signals for band-limited transmission channels like satellite broadcasting
(Digital Satellite Radio, DSR) or terrestrial broadcasting, it is necessary to reduce bit rates.
For this, a conversion of a block of linearly coded samples is carried out in a so-called
block ﬂoating-point representation in DSR. In the context of DAB, a data reduction of
linear coded samples is carried out based on psychoacoustic criteria.
Equalizers. While implementing equalizers with recursive digital ﬁlters, the signal-to-
noise ratio depends on the choice of the recursive ﬁlter structure. By a suitable choice
of a ﬁlter structure and methods to spectrally shape the quantization errors, optimal signal-
to-noise ratios are obtained for a given word-length. The signal-to-noise ratio for ﬁxed-
point representation depends on the word-length and for ﬂoating-point representation on the
word-length of the mantissa. For ﬁlter implementations with ﬁxed-point arithmetic, boost
ﬁlters have to be implemented with a scaling within the ﬁlter algorithm. The properties of
ﬂoating-point representation take care of automatic scaling in boost ﬁlters. If an insert I/O
in ﬁxed-point representation follows a boost ﬁlter in ﬂoating-point representation then the
same scaling as in ﬁxed-point arithmetic has to be done.
Dynamic Range Control. Dynamic range control is performed by a simple multiplicative
weighting of the input signal with a control factor. The latter follows from calculating the
peak and RMS value (root mean square) of the input signal. The number representation
of the signal has no inﬂuence on the properties of the algorithm. Owing to the normalized
58
Quantization
mantissa in ﬂoating-point representation, some simpliﬁcations are produced while deter-
mining the control factor.
Mixing/Summation. While mixing signals into a stereo image, only multiplications and
additions occur. Under the assumption of incoherent signals, an overload reserve can
be estimated. This implies a reserve of 20/30 dB for 48/96 sources. For ﬁxed-point
representation the overload reserve is provided by a number of overﬂow bits in the
accumulator of a DSP (Digital Signal Processor). The properties of automatic scaling in
ﬂoating-point arithmetic provide for overload reserves. For both number representations,
the summation signal must be matched with the number representation of the output. While
dealing with AES/EBU outputs or MADI outputs, both number representations are adjusted
to ﬁxed-point format. Similarly, within heterogeneous system solutions, it is logical to
make heterogeneous use of both number representations, though corresponding number
representations have to be converted.
Since the signal-to-noise ratio in ﬁxed-point representation depends on the input level,
a conversion from ﬁxed-point to ﬂoating-point representation does not lead to a change of
signal-to-noise ratio, i.e. the conversion does not improve the signal-to-noise ratio. Further
signal processing with ﬂoating-point or ﬁxed-point arithmetic does not alter the signal-to-
noise ratio as long as the algorithms are chosen and programmed accordingly. Reconversion
from ﬂoating-point to ﬁxed-point representation again leads to a level-dependent signal-to-
noise ratio.
As a consequence, for two-channel DSP systems which operate with AES/EBU or with
analog inputs and outputs, and which are used for equalization, dynamic range control,
room simulation etc., the above-mentioned discussion holds. These conclusions are also
valid for digital mixing consoles for which digital inputs from AD converters or from
multitrack machines are represented in ﬁxed-point format (AES/EBU or MADI). The
number representation for inserts and auxiliaries is speciﬁc to a system. Digital AES/EBU
(or MADI) inputs and outputs are realized in ﬁxed-point number representation.
2.5 Java Applet – Quantization, Dither, and Noise
Shaping
This applet shown in Fig. 2.44 demonstrates audio effects resulting from quantization. It is
designed for a ﬁrst insight into the perceptual effects of quantizing an audio signal.
The following functions can be selected on the lower right of the graphical user
interface:
• Quantizer
– word-length w leads to quantization step size Q = 2w−1.
• Dither
– rect dither – uniform probability density function
– tri dither – triangular probability density function
2.6 Exercises
59
– high-pass dither – triangular probability density function and high-pass power
spectral density.
• Noise shaping
– ﬁrst-order H (z) = z−1
– second-order H (z) = −2z−1 + z−2
– psychoacoustic noise shaping.
You can choose between two predeﬁned audio ﬁles from our web server (audio1.wav or
audio2.wav) or your own local wav ﬁle to be processed [Gui05].
Figure 2.44 Java applet – quantization, dither, and noise shaping.
2.6 Exercises
1. Quantization
1. Consider a 100 Hz sine wave x(n) sampled with fS = 44.1 kHz, N = 1024 samples
and w = 3 bits (word-length). What is the number of quantization levels? What
is the quantization step Q when the signal is normalized to −1 ≤ x(n) < 1. Show
graphically how quantization is performed. What is the maximum error for this 3-bit
quantizer? Write Matlab code for quantization with rounding and truncation.
60
Quantization
2. Derive the mean value, the variance and the peak factor PF of sequence e(n), if
the signal has a uniform probability density function in the range −Q/2 < e(n) <
−Q/2. Derive the signal-to-noise ratio for this case. What will happen if we increase
our word-length by one bit?
3. As the input signal level decreases from maximum amplitude to very low amplitudes,
the error signal becomes more audible. Describe the error calculated above when w
decreases to 1 bit? Is the classical quantization model still valid? What can be done
to avoid this distortion?
4. Write Matlab code for a quantizer with w = 16 bits with rounding and truncation.
• Plot the nonlinear transfer characteristic and the error signal when the input
signal covers the range 3Q < x(n) < 3Q.
• Consider the sine wave x(n) = A sin(2π(f/fS)n), n = 0, . . . , N − 1, with
A = Q, f/fS = 64/N and N = 1024. Plot the output signal (n = 0, . . . , 99)
of a quantizer with rounding and truncation in the time domain and the
frequency domain.
• Compute for both quantization types the quantization error and the signal-to-
noise ratio.
2. Dither
1. What is dither and when do we have to use it?
2. How do we perform dither and what kinds of dither are there?
3. How do we obtain a triangular high-pass dither and why do we prefer it to other
dithers?
4. Matlab: Generate corresponding dither signals for rectangular, triangular and trian-
gular high-pass.
5. Plot the amplitude distribution and the spectrum of the output xQ(n) of a quantizer
for every dither type.
3. Noise Shaping
1. What is noise shaping and when do we do it?
2. Why is it necessary to dither during noise shaping and how do we do this?
3. Matlab: The ﬁrst noise shaper used is without dither and assumes that the transfer
function in the feedback structure can be ﬁrst-order H (z) = z−1 or second-order
H (z) = −2z−1 + z−2. Plot the output xQ(n) and the error signal e(n) and its
spectrum. Show with a plot the shape of the error signal.
4. The same noise shaper is now used with a dither signal. Is it really necessary to dither
with noise shaping? Where would you add your dither in the ﬂow graph to achieve
better results?
References
61
5. In the feedback structure we now use a psychoacoustic-based noise shaper which
uses the Wannamaker ﬁlter coefﬁcients
h3 = [1.623, −0.982, 0.109],
h5 = [2.033, −2.165, 1.959, −1.590, 0.6149],
h9 = [2.412, −3.370, 3.937, −4.174, 3.353, −2.205, 1.281, −0.569, 0.0847].
Show with a Matlab plot the shape of the error with this ﬁlter.
References
[DeK03] D. De Koning, W. Verhelst: On Psychoacoustic Noise Shaping for Audio
Requantization, Proc. ICASSP-03, Vol. 5, pp. 453–456, April 2003.
[Ger89] M. A. Gerzon, P. G. Craven: Optimal Noise Shaping and Dither of Digital
Signals, Proc. 87th AES Convention, Preprint No. 2822, New York, October
1989.
[Gui05] M. Guillemard, C. Ruwwe, U. Zölzer: J-DAFx – Digital Audio Effects in Java,
Proc. 8th Int. Conference on Digital Audio Effects (DAFx-05), pp. 161–166,
Madrid, 2005.
[Hel07]
C. R. Helmrich, M. Holters, U. Zölzer: Improved Psychoacoustic Noise
Shaping for Requantization of High-Resolution Digital Audio, AES 31st
International Conference, London, June 2007.
[ISO389]
ISO 389-7:2005: Acoustics – Reference Zero for the Calibration of Audiometric
Equipment – Part 7: Reference Threshold of Hearing Under Free-Field and
Diffuse-Field Listening Conditions, Geneva, 2005.
[Lip86]
[Lip92]
[Lip93]
S. P. Lipshitz, J. Vanderkoy: Digital Dither, Proc. 81st AES Convention,
Preprint No. 2412, Los Angeles, November 1986.
S. P. Lipshitz, R. A. Wannamaker, J. Vanderkoy: Quantization and Dither:
A Theoretical Survey, J. Audio Eng. Soc., Vol. 40, No. 5, pp. 355–375, May
1992.
S. P. Lipshitz, R. A. Wannamaker, J. Vanderkooy: Dithered Noise Shapers
and Recursive Digital Filters, Proc 94th AES Convention, Preprint No. 3515,
Berlin, March 1993.
[Sha48]
C. E. Shannon: A Mathematical Theory of Communication, Bell Systems,
Techn. J., pp. 379–423, 623–656, 1948.
[Sri77]
A. B. Sripad, D. L. Snyder: A Necessary and Sufﬁcient Condition for
Quantization Errors to be Uniform and White, IEEE Trans. ASSP, Vol. 25,
pp. 442–448, October 1977.
[Ter79]
E. Terhardt: Calculating Virtual Pitch, Hearing Res., Vol. 1, pp. 155–182, 1979.
62
[Van89]
J. Vanderkooy, S. P. Lipshitz: Digital Dither: Signal Processing with Resolution
Far Below the Least Signiﬁcant Bit, Proc. AES Int. Conf. on Audio in Digital
Times, pp. 87–96, May 1989.
Quantization
[Wan92] R. A. Wannamaker: Psychoacoustically Optimal Noise Shaping, J. Audio Eng.
Soc., Vol. 40, No. 7/8, pp. 611–620, July/August 1992.
[Wan00] R. A. Wannamaker, S. P. Lipshitz, J. Vanderkooy, J. N. Wright: A Theory of
Nonsubtractive Dither, IEEE Trans. Signal Processing, Vol. 48, No. 2, pp. 499–
516, 2000.
[Wid61] B. Widrow: Statistical Analysis of Amplitude-Quantized Sampled-Data
Systems, Trans. AIEE, Pt. II, Vol. 79, pp. 555–568, January 1961.
Chapter 3
AD/DA Conversion
The conversion of a continuous-time function x(t) (voltage, current) into a sequence of
numbers x(n) is called analog-to-digital conversion (AD conversion). The reverse process
is known as digital-to-analog conversion (DA conversion). The time-sampling of a function
x(t) is described by Shannon’s sampling theorem. This states that a continuous-time signal
with bandwidth fB can be sampled with a sampling rate fS > 2fB without changing
the information content in the signal. The original analog signal is reconstructed by
low-pass ﬁltering with bandwidth fB . Besides time-sampling, the nonlinear procedure of
digitizing the continuous-valued amplitude (quantization) of the sampled signal occurs. In
Section 3.1 basic concepts of Nyquist sampling, oversampling and delta-sigma modulation
are presented. In Sections 3.2 and 3.3 principles of AD and DA converter circuits are
discussed.
3.1 Methods
3.1.1 Nyquist Sampling
The sampling of a signal with sampling rate fS > 2fB is called Nyquist sampling. The
schematic diagram in Fig. 3.1 shows the procedure. The band-limiting of the input at fS/2
is carried out by an analog low-pass ﬁlter (Fig. 3.1a). The following sample-and-hold circuit
samples the band-limited input at a sampling rate fS. The constant amplitude of the time
function over the sampling period TS = 1/fS is converted to a number sequence x(n) by
a quantizer (Fig. 3.1b). This number sequence is fed to a digital signal processor (DSP)
which performs signal processing algorithms. The output sequence y(n) is delivered to a
DA converter which gives a staircase as its output (Fig. 3.1c). Following this, a low-pass
ﬁlter gives the analog output y(t) (Fig. 3.1d). Figure 3.2 demonstrates each step of AD/DA
conversion in the frequency domain. The individual spectra in Fig. 3.2a–d correspond to
the outputs in Fig. 3.1a–d.
After band-limiting (Fig. 3.2a) and sampling, a periodic spectrum with period fS
of the sampled signal is obtained as shown in Fig. 3.2b. Assuming that consecutive
quantization errors e(n) are statistically independent of each other, the noise power has
Digital Audio Signal Processing Second Edition Udo Zölzer
© 2008 John Wiley & Sons, Ltd
64
AD/DA Conversion
(cid:6)(cid:9)(cid:12)(cid:28)(cid:4)(cid:16)(cid:10)(cid:3)(cid:14)(cid:19)(cid:9)(cid:7)(cid:2)
(cid:6)%
’7(cid:7):
(cid:9):
&(cid:10)(cid:9)(cid:4)(cid:11)(cid:3)
(cid:15)(cid:11);(cid:5)(cid:28)(cid:9)(cid:18)(cid:18)
.:
)(cid:13)(cid:9)(cid:10)(cid:7)(cid:16) (cid:2)(cid:8)
’7(cid:10):
’7(cid:7):
(cid:6)(cid:9)(cid:12)(cid:28)(cid:4)(cid:2)1=(cid:11)(cid:4)-
(cid:7)
(cid:7)
’7(cid:10):
(cid:26)(cid:6)(cid:1)
"7(cid:10):
(cid:26)&
$(cid:11)(cid:10)A(cid:2)(cid:8)(cid:7)(cid:2)(cid:8)
’7(cid:10):
"7(cid:10):
(cid:10)
(cid:10)
+
(cid:6)
(cid:20):
+(cid:6)
&(cid:10)(cid:9)(cid:4)(cid:11)(cid:3)
(cid:15)(cid:11);(cid:5)(cid:28)(cid:9)(cid:18)(cid:18)
-:
"7(cid:7):
(cid:7)
(cid:7)
a)
b)
c)
d)
Figure 3.1 Schematic diagram of Nyquist sampling.
Input Spectrum
Analog
Low-pass
0
0
0
0
Input Spectrum
Quantization
Error
Output Spectrum
Output Spectrum
Quantization
Error
fB
fS__
2
f
fS
Image Spectrum
f
fS
fB
fS__
2
Analog
Low-pass
Image Spectrum
sinc function
fB
fS__
2
f
fS
Influence of sinc funcion
fB
fS__
2
f
fS
Figure 3.2 Nyquist sampling – interpretation in the frequency domain.
3.1 Methods
65
a spectral uniform distribution in the frequency domain 0 ≤ f ≤ fS. The output of the DA
converter still has a periodic spectrum. However, this is weighted with the sinc function
(sinc = sin(x)/x), of the sample-and-hold circuit (Fig. 3.2c). The zeros of the sinc function
are at multiples of the sampling rate fS . In order to reconstruct the output (Fig. 3.2d), the
image spectra are eliminated by an analog low-pass of sufﬁcient stop-band attenuation (see
Fig. 3.2c).
The problems of Nyquist sampling lie in the steep band-limiting ﬁlter characteristics
(anti-aliasing ﬁlter) of the analog input ﬁlter and the analog reconstruction ﬁlter (anti-
imaging ﬁlter) of similar ﬁlter characteristics and sufﬁcient stop-band attenuation. Further,
sinc distortion due to the sample-and-hold circuit needs to be compensated for.
3.1.2 Oversampling
In order to increase the resolution of the conversion process and reduce the complexity
of analog ﬁlters, oversampling techniques are employed. Owing to the spectral uniform
distribution of quantization error between 0 and fS (see Fig. 3.3a), it is possible to reduce
the power spectral density in the pass-band 0 ≤ f ≤ fB through oversampling by a factor
L, i.e. with the new sampling rate LfS (see Fig. 3.3b). For identical quantization step size
Q, the shaded areas (quantization error power σ 2
E) in Fig. 3.3a and Fig. 3.3b are equal. The
increase in the signal-to-noise ratio can also be observed in Fig. 3.3.
a) Nyquist Sampling
S (f)
EE
Q2
12fS
fB fS
fS-f1
f1
b) Oversampling
S (f)
EE
Q2
12LfS
fB fS
f1
f
LfS
c) Delta-sigma Sampling
S (f)
EE
f
LfS
LfS-f1
fB fS
f1
f
LfS
LfS-f1
Figure 3.3 Inﬂuence of oversampling and delta-sigma technique on power spectral density of
quantization error and on input sinusoid with frequency f1.
It follows that in the pass-band at a sampling rate of fS = 2fB the power spectral
density given by
SEE(f ) = Q2
12fS
(3.1)
66
leads to the noise power
AD/DA Conversion
N 2
B
= σ 2
E
= 2
(cid:3)
fB
0
SEE(f ) df = Q2
12
.
(3.2)
Owing to oversampling by a factor of L, a reduction of the power spectral density given by
SEE(f ) = Q2
12LfS
(3.3)
is obtained (see Fig. 3.3b). With fS = 2fB, the error power in the audio band is given by
N 2
B
= 2fB
Q2
12LfS
= Q2
12
1
L
.
(3.4)
The signal-to-noise ratio (with PF =
√
3) owing to oversampling can now be expressed as
SNR = 6.02 · w + 10 log10(L) dB.
(3.5)
Figure 3.4a shows a schematic diagram of anoversampling AD converter. Owing
to oversampling, the analog band-limiting low-pass ﬁlter can have a wider transition
bandwidth as shown in Fig. 3.4b. The quantization error power is distributed between 0 and
the sampling rate LfS. To reduce the sampling rate, it is necessary to limit the bandwidth
with a digital low-pass ﬁlter (see Fig. 3.4c). After this, the samplingrate is reduced by a
factor L (see Fig. 3.4d) by taking every Lth output sample of the digital low-pass ﬁlter
[Cro83, Vai93, Fli00].
Figure 3.5a shows a schematic diagram of an oversampling DA converter. The sampling
rate is ﬁrst increased by a factor of L. For this purpose, L − 1 zeros are introduced between
two consecutive input values [Cro83, Vai93, Fli00]. The following digital ﬁlter eliminates
all image spectra (Fig. 3.5b) except the base-band spectrum and spectra at multiples of
LfS (Fig. 3.5c). It interpolates L − 1 samples between two input samples. The w-bit DA
converter operates at a sampling rate LfS. Its output is fed to an analog reconstruction ﬁlter
which eliminates the image spectra at multiples of LfS.
3.1.3 Delta-sigma Modulation
Delta-sigma modulation using oversampling is a conversion strategy derived from delta
modulation. In delta modulation (Fig. 3.6a), the difference between the input x(t) and
signal x1(t) is converted into a 1-bit signal y(n) at a very high sampling rate LfS. The
sampling rate is higher than the necessary Nyquist rate fS . The quantized signal y(n) gives
the signal x1(t) via an analog integrator. The demodulator consists of an integrator and a
reconstruction low-pass ﬁlter.
The extension to delta-sigma modulation [Ino63] involves shifting the integrator from
the demodulator to the input of the modulator (see Fig. 3.6b). With this, it is possible
to combine the two integrators as a single integrator after addition (see Fig. 3.7a). The
corresponding signals are shown in Fig. 3.8.
3.1 Methods
67
(cid:6)(cid:9)(cid:12)(cid:28)(cid:4)(cid:16)(cid:10)(cid:3)(cid:14)(cid:19)(cid:9)(cid:7)(cid:2)
(cid:15) (cid:6)%
(cid:9):
’7(cid:7):
&(cid:10)(cid:9)(cid:4)(cid:11)(cid:3)
(cid:15)(cid:11);(cid:5)(cid:28)(cid:9)(cid:18)(cid:18)
;-.(cid:16)(cid:7)
&(cid:26)$
(cid:26)(cid:16)(cid:3)(cid:16)(cid:7)(cid:9)(cid:4)
(cid:15)(cid:11);(cid:5)(cid:28)(cid:9)(cid:18)(cid:18)
(cid:6)(cid:9)(cid:12)(cid:28)(cid:4)(cid:16)(cid:10)(cid:3)(cid:14)(cid:19)(cid:9)(cid:7)(cid:2)
(cid:6)%
(cid:15)
’7(cid:10):
(cid:27)(cid:10)(cid:28)(cid:13)(cid:7)(cid:14)(cid:6)(cid:28)(cid:2)(cid:20)(cid:7)(cid:8)(cid:13)(cid:12)
)(cid:13)(cid:9)(cid:10)(cid:7)(cid:16) (cid:9)(cid:7)(cid:16)(cid:11)(cid:10)
(cid:30)(cid:8)(cid:8)(cid:11)(cid:8)
&(cid:10)(cid:9)(cid:4)(cid:11)(cid:3)
(cid:15)(cid:11);(cid:5)(cid:28)(cid:9)(cid:18)(cid:18)
(cid:27)(cid:12)(cid:9)(cid:3)(cid:2)(cid:14)(cid:6)(cid:28)(cid:2)(cid:20)(cid:7)(cid:8)(cid:13)(cid:12)
3
% ,
(cid:6)%
(cid:23)
(cid:15)
(cid:6)%
(cid:6)%
(cid:23)
(cid:15)
(cid:6)%
(cid:26)(cid:16)(cid:3)(cid:16)(cid:7)(cid:9)(cid:4)
(cid:15)(cid:11);(cid:5)(cid:28)(cid:9)(cid:18)(cid:18)
3
% ,
(cid:6)%
(cid:23)
(cid:15)
(cid:6)%
(cid:6)%
(cid:23)
(cid:15)
(cid:6)%
%
%
(cid:27)(cid:10)(cid:28)(cid:13)(cid:7)(cid:14)(cid:6)(cid:28)(cid:2)(cid:20)(cid:7)(cid:8)(cid:13)(cid:12)
(cid:27)(cid:12)(cid:9)(cid:3)(cid:2)(cid:14)(cid:6)(cid:28)(cid:2)(cid:20)(cid:7)(cid:8)(cid:13)(cid:12)
3
%,
(cid:6)%
(cid:23)
%
(cid:6)%
Figure 3.4 Oversampling AD converter and sampling rate reduction.
.:
(cid:20):
-:
A time-discrete model of the delta-sigma modulator is given in Fig. 3.7b. The Z-
transform of the output signal y(n) is given by
Y (z) = H (z)
1 + H (z)
X(z) +
1
1 + H (z)
E(z) ≈ X(z) +
1
1 + H (z)
E(z).
(3.6)
For a large gain factor of the system H (z), the input signal will not be affected. In contrast,
the quantization error is shaped by the ﬁlter term 1/[(1 + H (z)].
Schematic diagrams of delta-sigma AD/DA conversion are shown in Figs 3.9 and 3.10.
For delta-sigma AD converters, a digital low-pass ﬁlter and a downsampler with factor L
are used to reduce the sampling rate LfS to fS. The 1-bit input to the digital low-pass ﬁlter
leads to a w-bit output x(n) at a sampling rate fS . The delta-sigma DA converter consists
of an upsampler with factor L, a digital low-pass ﬁlter to eliminate the mirror spectra and
a delta-sigma modulator followed by an analog reconstruction low-pass ﬁlter. In order to
illustrate noise shaping in delta-sigma modulation in detail, ﬁrst- and second-order systems
as well as multistage techniques are investigated in the following sections.
68
AD/DA Conversion
(cid:6)(cid:9)(cid:12)(cid:28)(cid:4)(cid:16)(cid:10)(cid:3)(cid:14)(cid:19)(cid:9)(cid:7)(cid:2)
(cid:6)%
(cid:6)(cid:9)(cid:12)(cid:28)(cid:4)(cid:16)(cid:10)(cid:3)(cid:14)(cid:19)(cid:9)(cid:7)(cid:2)
(cid:6)(cid:15)%
(cid:9):
’7(cid:10):
(cid:15)
(cid:26)(cid:16)(cid:3)(cid:16)(cid:7)(cid:9)(cid:4)
(cid:15)(cid:11);(cid:5)(cid:28)(cid:9)(cid:18)(cid:18)
;-.(cid:16)(cid:7)
(cid:26)&$
&(cid:10)(cid:9)(cid:4)(cid:11)(cid:3)
(cid:15)(cid:11);(cid:5)(cid:28)(cid:9)(cid:18)(cid:18)
’7(cid:7):
((cid:8)(cid:16)(cid:3)(cid:16)(cid:10)(cid:9)(cid:4)(cid:14)(cid:6)(cid:28)(cid:2)(cid:20)(cid:7)(cid:8)(cid:13)(cid:12)
(cid:27)(cid:12)(cid:9)(cid:3)(cid:2)(cid:14)(cid:6)(cid:28)(cid:2)(cid:20)(cid:7)(cid:8)(cid:13)(cid:12)(cid:14)(cid:22)
(cid:27)(cid:12)(cid:9)(cid:3)(cid:2)(cid:14)(cid:6)(cid:28)(cid:2)(cid:20)(cid:7)(cid:8)(cid:13)(cid:12)(cid:14)(cid:23)
)(cid:13)(cid:9)(cid:10)(cid:7)(cid:16) (cid:9)(cid:7)(cid:16)(cid:11)(cid:10)
(cid:30)(cid:8)(cid:8)(cid:11)(cid:8)
.:
(cid:20):
3
%,
(cid:6)%
(cid:23)
(cid:6)%
(cid:23)
(cid:6)%
(cid:27)(cid:12)(cid:9)(cid:3)(cid:2)(cid:14)(cid:6)(cid:28)(cid:2)(cid:20)(cid:7)(cid:8)(cid:13)(cid:12)(cid:14)(cid:15)
(cid:26)(cid:16)(cid:3)(cid:16)(cid:7)(cid:9)(cid:4)
(cid:15)(cid:11);(cid:5)(cid:28)(cid:9)(cid:18)(cid:18)
3
% ,
(cid:6)%
(cid:23)
&(cid:10)(cid:9)(cid:4)(cid:11)(cid:3)
(cid:15)(cid:11);(cid:5)(cid:28)(cid:9)(cid:18)(cid:18)
(cid:15)
(cid:6)%
((cid:8)(cid:16)(cid:3)(cid:16)(cid:10)(cid:9)(cid:4)(cid:14)(cid:6)(cid:28)(cid:2)(cid:20)(cid:7)(cid:8)(cid:13)(cid:12)
(cid:14)(cid:27)(cid:12)(cid:9)(cid:3)(cid:2)(cid:14)(cid:6)(cid:28)(cid:2)(cid:20)(cid:7)(cid:8)(cid:13)(cid:12)
-:
3
% ,
(cid:6)%
(cid:23)
(cid:15) (cid:6)%
(cid:6)%
(cid:23)
(cid:15)
(cid:6)%
Figure 3.5 Oversampling and DA conversion.
%
%
%
First-order Delta-sigma Modulator
A time-discrete model of a ﬁrst-order delta-sigma modulator is shown in Fig. 3.11.
The difference equation for the output y(n) is given by
y(n) = x(n − 1) + e(n) − e(n − 1).
The corresponding Z-transform leads to
Y (z) = z
−1X(z) + E(z) (1 − z
(cid:13)(cid:14)
HE(z)
(cid:12)
−1)
(cid:15)
.
(3.7)
(3.8)
The power density spectrum of the error signal e1(n) = e(n) − e(n − 1) is
(cid:2)
(cid:1)
SE1E1(ej (cid:4)) = SEE(ej (cid:4))|1 − e
−j (cid:4)|2 = SEE(ej (cid:4))4 sin2
,
(3.9)
(cid:4)
2
3.1 Methods
69
(cid:9):
’7(cid:7):
’(cid:22)7(cid:7):
(cid:15)%(cid:6)
(cid:22)
(cid:18)
"7(cid:10):
’(cid:22)7(cid:7):
(cid:22)
(cid:18)
(cid:15)(cid:1)
"7(cid:7):
/(cid:11)-(cid:13)(cid:4)(cid:9)(cid:7)(cid:11)(cid:8)
(cid:26)(cid:2)(cid:12)(cid:11)-(cid:13)(cid:4)(cid:9)(cid:7)(cid:11)(cid:8)
.:
’7(cid:7):
(cid:22)
(cid:18)
(cid:15)%(cid:6)
(cid:22)
(cid:18)
"7(cid:10):
(cid:15)(cid:1)
"7(cid:7):
Figure 3.6 Delta modulation and displacement of integrator.
(cid:22)
(cid:18)
’7(cid:7):
(cid:9):
.:
(cid:15)%(cid:6)
(cid:30)7 :
"7(cid:10):
07 :
=7 :
*7 :
Figure 3.7 Delta-sigma modulation and time-discrete model.
where SEE(ej (cid:4)) denotes the power density spectrum of the quantization error e(n). The
error power in the frequency band [−fB , fB ], with SEE(f ) = Q2/12LfS, can be written
as
N 2
B
= SEE(f )2
(cid:1)
(cid:10) Q2
12
π 2
3
(cid:3)
fB
0
2fB
LfS
4 sin2
(cid:2)
3
.
(cid:1)
π
f
LfS
(cid:2)
df
(3.10)
(3.11)
70
1
0
)
t
(
x
(cid:358)1
0
AD/DA Conversion
10
20
30
40
50
60
70
80
90
100
2
0
(cid:358)2
2
0
0
x 10(cid:358)3
10
20
30
40
50
60
70
80
90
100
10
20
30
40
50
60
70
80
90
100
)
t
(
x
d
)
t
(
t
n
i
x
)
t
(
y
s
(cid:358)2
0
1
0
(cid:358)1
0
10
20
30
40
1
0
)
n
(
y
(cid:358)1
0
10
20
30
40
50
t/T (cid:111)
50
n (cid:111)
60
70
80
90
100
60
70
80
90
100
Figure 3.8 Signals in delta-sigma modulation.
(cid:6)(cid:9)(cid:12)(cid:28)(cid:4)(cid:16)(cid:10)(cid:3)(cid:14)(cid:19)(cid:9)(cid:7)(cid:2)
(cid:15) (cid:6)%
(cid:22)(cid:14).(cid:16)(cid:7)
(cid:6)(cid:9)(cid:12)(cid:28)(cid:4)(cid:16)(cid:10)(cid:3)(cid:14)(cid:19)(cid:9)(cid:7)(cid:2)
(cid:6)%
;(cid:14).(cid:16)(cid:7)s
’7(cid:7):
&(cid:10)(cid:9)(cid:4)(cid:11)(cid:3)
(cid:15)(cid:11);(cid:5)(cid:28)(cid:9)(cid:18)(cid:18)
(cid:26)(cid:2)(cid:4)(cid:7)(cid:9)(cid:5)(cid:6)(cid:16)(cid:3)(cid:12)(cid:9)
/(cid:11)-(cid:13)(cid:4)(cid:9)(cid:7)(cid:11)(cid:8)
(cid:26)(cid:16)(cid:3)(cid:16)(cid:7)(cid:9)(cid:4)
(cid:15)(cid:11);(cid:5)(cid:28)(cid:9)(cid:18)(cid:18)
(cid:15)
’7(cid:10):
Figure 3.9 Oversampling delta-sigma AD converter.
With fS = 2fB, we get
N 2
B
= Q2
12
π 2
3
(cid:1)
(cid:2)
3
.
1
L
(3.12)
3.1 Methods
71
(cid:6)(cid:9)(cid:12)(cid:28)(cid:4)(cid:16)(cid:10)(cid:3)(cid:14)(cid:19)(cid:9)(cid:7)(cid:2)
(cid:6)%
’7(cid:10):
(cid:15)
(cid:6)(cid:9)(cid:12)(cid:28)(cid:4)(cid:16)(cid:10)(cid:3)(cid:14)(cid:19)(cid:9)(cid:7)(cid:2)
(cid:15) (cid:6)%
(cid:26)(cid:16)(cid:3)(cid:16)(cid:7)(cid:9)(cid:4)
(cid:15)(cid:11);(cid:5)(cid:28)(cid:9)(cid:18)(cid:18)
(cid:26)(cid:2)(cid:4)(cid:7)(cid:9)(cid:5)(cid:6)(cid:16)(cid:3)(cid:12)(cid:9)
/(cid:11)-(cid:13)(cid:4)(cid:9)(cid:7)(cid:11)(cid:8)
&(cid:10)(cid:9)(cid:4)(cid:11)(cid:3)
(cid:15)(cid:11);(cid:5)(cid:28)(cid:9)(cid:18)(cid:18)
’7(cid:7):
;(cid:14).(cid:16)(cid:7)s
(cid:22)(cid:14).(cid:16)(cid:7)
Figure 3.10 Oversampling delta-sigma DA converter.
)(cid:13)(cid:9)(cid:10)(cid:7)(cid:16) (cid:2)(cid:8)
(cid:30)7 :
07 :
 (cid:5)(cid:22)
*7 :
Figure 3.11 Time-discrete model of a ﬁrst-order delta-sigma modulator.
Second-order Delta-sigma Modulator
For the second-order delta-sigma modulator [Can85], shown in Fig. 3.12, the difference
equation is expressed as
y(n) = x(n − 1) + e(n) − 2e(n − 1) + e(n − 2)
(3.13)
)(cid:13)(cid:9)(cid:10)(cid:7)(cid:16) (cid:2)(cid:8)
(cid:30)7 :
07 :
 (cid:5)(cid:22)
*7 :
 (cid:5)(cid:22)
Figure 3.12 Time-discrete model of a second-order delta-sigma modulator.
and the Z-transform is given by
Y (z) = z
−1X(z) + E(z) (1 − 2z
(cid:12)
−1 + z
(cid:13)(cid:14)
HE (z)=(1−z−1)2
−2)
(cid:15)
.
(3.14)
The power density spectrum of the error signal e1(n) = e(n) − 2e(n − 1) + e(n − 2) can
be written as
SE1E1(ej (cid:4)) = SEE(ej (cid:4))|1 − e
(cid:4)
4 sin2
−j (cid:4)|4
(cid:1)
(cid:2)(cid:5)
2
(cid:4)
2
= SEE(ej (cid:4))
= SEE(ej (cid:4))4[1 − cos((cid:4))]2.
(3.15)
72
AD/DA Conversion
The error power in the frequency band [−fB , fB ] is given by
N 2
B
= SEE(f )2
(cid:1)
(cid:10) Q2
12
π 4
5
4[1 − cos((cid:4))]2 df
(cid:2)
5
(cid:3)
fB
0
2fB
LfS
,
and with fS = 2fB we obtain
N 2
B
= Q2
12
π 4
5
(cid:1)
(cid:2)
5
.
1
L
Multistage Delta-sigma Modulator
A multistage delta-sigma modulator (MASH, [Mat87]) is shown in Fig. 3.13.
07 :
*7 :
(cid:22)(cid:30)(cid:14)(cid:14)7 :
)
*(cid:14)7 :
(cid:22)
 (cid:5)(cid:22)
(cid:30)(cid:14)(cid:14)7 :
(cid:23)
)
*(cid:14)7 :
(cid:23)
(cid:22)(cid:5) (cid:5)(cid:22)
 (cid:5)(cid:22)
(cid:30)(cid:14)(cid:14)7 :
2
)
*(cid:14)7 :
2
7
(cid:22)(cid:5) (cid:5)(cid:22)
(cid:23):
 (cid:5)(cid:22)
 (cid:5)(cid:22)
(cid:22)(cid:30)(cid:14)(cid:14)7 :
 (cid:5)(cid:22)
(cid:30)(cid:14)(cid:14)7 :
(cid:23)
 (cid:5)(cid:22)
Figure 3.13 Time-discrete model of a multistage delta-sigma modulator.
The Z-transforms of the output signals yi(n), i = 1, 2, 3, are given by
−1)E1(z),
Y1(z) = X(z) + (1 − z
Y2(z) = − E1(z) + (1 − z
Y3(z) = − E2(z) + (1 − z
−1)E2(z),
−1)E3(z).
(3.16)
(3.17)
(3.18)
(3.19)
(3.20)
(3.21)
3.1 Methods
The Z-transform of the output obtained by addition and ﬁltering leads to
Y (z) = Y1(z) + (1 − z
= X(z) + (1 − z
−1)Y2(z) + (1 − z
−1)E1(z) − (1 − z
−1)2Y3(z)
−1)E1(z)
−1)2E2(z) + (1 − z
−1)3E3(z)
+ (1 − z
−1)2E2(z) − (1 − z
= X(z) + (1 − z
(cid:13)(cid:14)
HE (z)
(cid:12)
−1)3
(cid:15)
E3(z).
The error power in the frequency band [−fB, fB ],
(cid:1)
N 2
B
= Q2
12
with fS = 2fB, gives the total noise power
= Q2
12
N 2
B
(cid:2)
7
,
π 6
7
2fB
LfS
(cid:1)
(cid:2)
7
.
1
L
π 6
7
73
(3.22)
(3.23)
(3.24)
The error transfer functions in Fig. 3.14 show the noise shaping for three types of delta-
sigma modulations as discussed before. The error power is shifted toward higher frequen-
cies.
→
B
d
n
i
)
S
f
/
f
(
H
20
0
-20
-40
-60
-80
-100
-120
-140
K 1 - - -
K 2 .....
K 3 -.-.-
0
0.05
0.1
0.15
0.2
0.25
f/LfS
0.3
0.35
0.4
0.45
0.5
→
Figure 3.14 HE (z) = (1 − z
−1)K with K = 1, 2, 3.
The improvement of signal-to-noise ratio by pure oversampling and delta-sigma mod-
ulation (ﬁrst, second and third order) is shown in Fig. 3.15. For the general case of a kth-
order delta-sigma conversion with oversampling factor L one can derive the signal-to-noise
ratio as
SNR = 6.02 · w − 10 log10
+ (2k + 1)10 log10(L) dB.
(3.25)
(cid:1)
(cid:2)
π 2k
2k + 1
74
AD/DA Conversion
Here w denotes the quantizer word-length of the delta-sigma modulator. The signal quanti-
zation after digital low-pass ﬁltering and downsampling by L can be performed with (3.25)
according to the relation w = SNR/6.
140
120
100
80
60
40
20
→
B
d
n
i
R
N
S
K 3
K 2
K 1
Oversampling
0
4
4.5
5
5.5
6.5
6
L 2x
7
x →
7.5
8
8.5
9
Figure 3.15 Improvement of signal-to-noise ratio as a function of oversampling and noise shaping
(L = 2x ).
Higher-order Delta-sigma Modulator
A widening of the stop-band for the high-pass transfer function of the quantization error
is achieved with higher-order delta-sigma modulation [Cha90]. Besides the zeros at z = 1,
additional zeros are placed on the unit circle. Also, poles are integrated into the trans-
fer function. A time-discrete model of a higher-order delta-sigma modulator is shown in
Fig. 3.16.
, (cid:24)
,(cid:23)
,
(cid:22)
07 :
 (cid:5)(cid:22)
(cid:22)
 (cid:5)(cid:22)
 (cid:5)(cid:22)
(cid:22)
 (cid:5)(cid:22)
 (cid:5)(cid:22)
(cid:22)
 (cid:5)(cid:22)
&
(cid:24)
&
(cid:22)
3&
&(cid:26)$
*7 :
(cid:26)&$
Figure 3.16 Higher-order delta-sigma modulator.
3.1 Methods
75
The transfer function in Fig. 3.16 can be written as
(cid:24)
(cid:25)
2
H (z) =
A0 + A1
z−1
1−z−1
1 − B1
z−1
1−z−1
+ A2
(cid:24)
− B2
z−1
1−z−1
(cid:25)
2
z−1
1−z−1
+ · · ·
+ · · ·
= A0(z − 1)N + A1(z − 1)N−1 + · · · + AN
(z − 1)N − B1(z − 1)N−1 − · · · − BN
(cid:11)
N
i=0 Ai(z − 1)N−i
(cid:11)
=
(z − 1)N −
i=1 Bi (z − 1)N−i
N
.
(3.26)
The Z-transform of the output is given by
Y (z) = H (z)
1
1 + H (z)
= HX(z)X(z) + HE(Z)E(z).
1 + H (z)
X(z) +
E(z)
The transfer function for the input is
(cid:11)
HX(z) =
(cid:11)
(z − 1)N −
N
i=0 Ai(z − 1)N−i
(cid:11)
i=1 Bi(z − 1)N−i +
N
i=0 Ai(z − 1)N−i
N
and the transfer function for the error signal is given by
HE(z) =
(z − 1)N −
(cid:11)
(z − 1)N −
(cid:11)
N
i=1 Bi (z − 1)N−i
(cid:11)
i=1 Bi (z − 1)N−i +
N
i=0 Ai(z − 1)N−i
N
(3.27)
(3.28)
(3.29)
(3.30)
,
.
For Butterworth or Chebyshev ﬁlter designs, the frequency responses as shown in Fig. 3.17
are obtained for the error transfer functions. As a comparison, the frequency responses
of ﬁrst-, second- and third-order delta-sigma modulation are shown. The widening of the
stop-band for Butterworth and Chebyshev ﬁlters can be observed from Fig. 3.18.
Decimation Filter
Decimation ﬁlters for AD conversion and interpolation ﬁlters for DA conversion are im-
plemented with multirate systems [Fli00]. The necessary downsampler and upsampler are
simple systems. For the former, every nth sample is taken out of the input sequence. For the
latter, n − 1 zeros are inserted between two input samples. For decimation, band-limiting
is performed by H (z) followed by sampling rate reduction by a factor L. This procedure
can be implemented in stages (see Fig. 3.19). The use of easy-to-implement ﬁlter structures
at high sampling rates, like comb ﬁlters with transfer function
H1(z) = 1
L
1 − z−L
1 − z−1
(3.31)
(shown in Fig. 3.20), allows simple implementation needing only delay systems and ad-
ditions. In order to increase the stop-band attenuation, a series of comb ﬁlters is used so
76
AD/DA Conversion
→
B
d
n
i
)
S
f
/
f
(
H
20
0
-20
-40
-60
-80
-100
-120
-140
B, C
K 1 - - -
K 2 .....
K 3 -.-.-
0
0.05
0.1
0.15
0.2
0.25
f/LfS
0.3
0.35
0.4
0.45
0.5
→
Figure 3.17 Comparison of different transfer functions of error signal.
→
B
d
n
i
)
S
f
/
f
(
H
20
0
-20
-40
-60
-80
-100
-120
-140
B
C
K 1 - - -
K 2 .....
K 3 -.-.-
0
0.005 0.01 0.015 0.02 0.025 0.03 0.035 0.04 0.045
f/LfS
→
Figure 3.18 Transfer function of the error signal in stop-band.
that
is obtained.
H M
1 (z) =
(cid:5)
M
(cid:4)
1 − z−L
1 − z−1
1
L
(3.32)
Besides additions at high sampling rates, complexity can be reduced further. Owing
to sampling rate reduction by a factor of L, the numerator (1 − z−L) can be moved so
that it is placed after the downsampler (see Fig. 3.21). For a series of comb ﬁlters, the
3.1 Methods
77
’7(cid:10):
(cid:15)%(cid:6)
’7(cid:10):
(cid:15)%(cid:6)
=7 :
(cid:15)
(cid:15)%(cid:6)
"7(cid:12):
%(cid:6)
(cid:14)(cid:14)(cid:14)(cid:14)(cid:14)
=(cid:14)(cid:14)7 :
(cid:22)
(cid:22)(cid:15)
(cid:15)%(cid:6)
(cid:15) (cid:22)
%(cid:6)
=(cid:14)(cid:14)7 K:
(cid:23)
(cid:15)
%(cid:6)
(cid:22)
(cid:15)(cid:23)
"7(cid:12):
%(cid:6)
Figure 3.19 Several stages of sampling rate reduction.
 (cid:5)(cid:15)
’7(cid:10):
"7(cid:10):
 (cid:5)(cid:22)
Figure 3.20 Signal ﬂow diagram of a comb ﬁlter.
structure in Fig. 3.22 results. M simple recursive accumulators have to be performed at
the high sampling rate LfS. After this, downsampling by a factor L is carried out. The M
nonrecursive systems are calculated with the output sampling rate fS.
’7(cid:10):
(cid:15)%(cid:6)
’7(cid:10):
(cid:15)%(cid:6)
(cid:22)  (cid:5)(cid:15)
 (cid:5)(cid:22)
(cid:22)
(cid:15)%(cid:6)
(cid:15)
(cid:15)
"7(cid:12):
%(cid:6)
(cid:22)
 (cid:5)(cid:22)
(cid:22)
 (cid:5)(cid:22)
(cid:22)
(cid:15)%(cid:6)
%(cid:6)
"7(cid:12):
%(cid:6)
Figure 3.21 Comb ﬁlter for sampling rate reduction.
(cid:22)
(cid:22)
 (cid:5)(cid:22)
(cid:22)
/
(cid:22)
 (cid:5)(cid:22)
(cid:22)
’7(cid:10):
(cid:15)%(cid:6)
(cid:22)
/
(cid:15)
(cid:22)
 (cid:5)(cid:22)
(cid:22)
 (cid:5)(cid:22)
(cid:15)%(cid:6)
%(cid:6)
"7(cid:12):
%(cid:6)
Figure 3.22 Series of comb ﬁlters for sampling rate reduction.
Figure 3.23a shows the frequency responses of a series of comb ﬁlters (L = 16). Fig-
ure 3.23b shows the resulting frequency response for the quantization error of a third-order
delta-sigma modulator connected in series with a comb ﬁlter H 4
1 (z). The system delay
owing to ﬁltering and sampling rate reduction is given by
tD = N − 1
2
· 1
LfS
.
(3.33)
78
→
B
d
n
i
)
S
f
/
f
(
H
0
-20
-40
-60
-80
-100
-120
-140
a)
M 1
M 2
M 3
M 4
0 0.05 0.1 0.15 0.2 0.25 0.3 0.35 0.4 0.45 0.5
f/LfS
→
AD/DA Conversion
b)
K 3
H1(z)4 ⋅ (1-z-1)3
0 0.05 0.1 0.15 0.2 0.25 0.3 0.35 0.4 0.45 0.5
f/LfS
→
→
B
d
n
i
)
S
f
/
f
(
H
20
0
-20
-40
-60
-80
-100
-120
-140
Figure 3.23 (a) Transfer function H M
sigma modulation and in series with H 4
1 (z) =
1 (z).
(cid:26)
1
16
1−z−16
1−z−1
(cid:27)
M
with M = 1 . . .4. (b) Third-order delta-
Example: Delay time of conversion process (latency time)
1. Nyquist conversion
fS = 48 kHz
tD = 1
fS
= 20.83 µs.
2. Delta-sigma modulation with single-stage downsampling
L = 64
fS = 48 kHz
N = 4096
tD = 665 µs.
3. Delta-sigma modulation with two-stage downsampling
L = 64
fS = 48 kHz
L1 = 16
L2 = 4
N1 = 61
N2 = 255
tD1
tD2
= 9.76 µs
= 662 µs.
3.2 AD Converters
3.2 AD Converters
79
The choice of an AD converter for a certain application is inﬂuenced by a number of
factors. It mainly depends on the necessary resolution for a given conversion time. Both
of these depend upon each other and are decisively inﬂuenced by the architecture of the
AD converter. For this reason, the speciﬁcations of an AD converter are ﬁrst discussed.
This is followed by circuit principles which inﬂuence the mutual dependence of resolution
and conversion time.
3.2.1 Speciﬁcations
In the following, the most important speciﬁcations for AD conversion are presented.
Resolution. The resolution for a given word-length w of an AD converter determines the
smallest amplitude
xmin = Q = xmax 2
−(w−1),
(3.34)
which is equal to the quantization step Q.
Conversion Time. The minimum sampling period TS = 1/fS between two samples is
called the conversion time.
Sample-and-hold Circuit. Before quantization, the time-continuous function is sampled
with the help of a sample-and-hold circuit, as shown in Fig. 3.24a.
The sampling period TS is divided into the sampling time tS in which the output voltage
U2 follows the input voltage U1, and the hold time tH . During the hold time the output
voltage U2 is constant and is converted into a binary word by quantization.
(cid:9):
.:
!(cid:22)
!(cid:22)
(cid:6)1=
!(cid:23)
!(cid:23)
(cid:7)&(cid:26)
(cid:7) (cid:6)
(cid:7) =
+(cid:6)
Figure 3.24 (a) Sample-and-hold circuit. (b) Input and output with clock signal. (tS = sampling time,
tH = hold time, tAD = aperture delay.)
Aperture Delay. The time tAD elapsed between start of hold and actual hold mode (see
Fig. 3.24b) is called the aperture delay.
80
AD/DA Conversion
Aperture Jitter. The variation in aperture delay from sample to sample is called the aper-
ture jitter tADJ. The inﬂuence of aperture jitter limits the useful bandwidth of the sampled
signal. This is because at high frequency a deterioration of the signal-to-noise ratio occurs.
Assuming a Gaussian PDF aperture jitter, the signal-to-noise ratio owing to aperture jitter
as a function of frequency f can be written as
SNRJ = −20 log10(2πf tADJ) dB.
(3.35)
Offset Error and Gain Error. The offset and gain errors of an AD converter are shown in
Fig. 3.25. The offset error results in a horizontal displacement of the real curve compared
with the dashed ideal curve of an AD converter. The gain error is expressed as the deviation
from the ideal gradient of the curve.
(%%(cid:18)(cid:2)(cid:7)(cid:14)(cid:30)(cid:8)(cid:8)(cid:11)(cid:8)
#(cid:9)(cid:16)(cid:10)(cid:14)(cid:30)(cid:8)(cid:8)(cid:11)(cid:8)
(cid:2)
-
(cid:11)
$
(cid:14)
(cid:7)
(cid:13)
(cid:28)
(cid:7)
(cid:13)
(
(cid:22)(cid:22)(cid:22)
(cid:22)(cid:22)3
(cid:22)3(cid:22)
(cid:22)33
3(cid:22)(cid:22)
3(cid:22)3
33(cid:22)
333
(cid:2)
-
(cid:11)
$
(cid:14)
(cid:7)
(cid:13)
(cid:28)
(cid:7)
(cid:13)
(
(cid:22)(cid:22)(cid:22)
(cid:22)(cid:22)3
(cid:22)3(cid:22)
(cid:22)33
3(cid:22)(cid:22)
3(cid:22)3
33(cid:22)
333
(cid:27)(cid:10)(cid:28)(cid:13)(cid:7)(cid:14)L(cid:11)(cid:4)(cid:7)(cid:9)(cid:3)(cid:2)
’ (cid:12)(cid:9)’
(cid:27)(cid:10)(cid:28)(cid:13)(cid:7)(cid:14)L(cid:11)(cid:4)(cid:7)(cid:9)(cid:3)(cid:2)
’ (cid:12)(cid:9)’
Figure 3.25 Offset error and gain error.
Differential Nonlinearity. The differential nonlinearity
DNL = (cid:8)x/Q
(cid:8)xQ
− 1 LSB
(3.36)
describes the error of the step size of a certain code word in LSB units. For ideal quan-
tization, the increase (cid:8)x in the input voltage up to the next output code xQ is equal to
the quantization step Q (see Fig. 3.26). The difference of two consecutive output codes is
denoted by (cid:8)xQ. When the output code changes from 010 to 011, the step size is 1.5 LSB
and therefore the differential nonlinearity DNL = 0.5 LSB. The step size between the
codes 011 and 101 is 0 LSB and the code 200 is missing. The differential nonlinearity
is DNL = −1 LSB.
Integral Nonlinearity. The integral nonlinearity (INL) describes the error between the
quantized and the ideal continuous value. This error is given in LSB units. It arises owing
to the accumulated error of the step size. This (see Fig. 3.27) changes itself continuously
from one output code to another.
Monotonicity. The progressive increase in quantizer output code for a continuously in-
creasing input voltage and progressive decrease in quantizer output code for a continuously
decreasing input voltage is called monotonicity. An example of non-monotonic behavior is
shown in Fig. 3.28 where one output code does not occur.
3.2 AD Converters
81
(cid:2)
-
(cid:11)
$
(cid:14)
(cid:7)
(cid:13)
(cid:28)
(cid:7)
(cid:13)
(
(cid:22)(cid:22)(cid:22)
(cid:22)(cid:22)3
(cid:22)3(cid:22)
(cid:22)33
3(cid:22)(cid:22)
3(cid:22)3
33(cid:22)
333
,
(cid:6)
(cid:15)
3
,
(cid:6)
(cid:15)
(cid:14)
5
8
(cid:22)
,
(cid:6)
(cid:15)
(cid:22)
(cid:27)(cid:10)(cid:28)(cid:13)(cid:7)(cid:14)L(cid:11)(cid:4)(cid:7)(cid:9)(cid:3)(cid:2)
(cid:26)(cid:24)(cid:15)(cid:14)M(cid:14)(cid:5)(cid:22)(cid:14)(cid:15)(cid:6),
(cid:26)(cid:24)(cid:15)(cid:14)M(cid:14)3(cid:29)5(cid:14)(cid:15)(cid:6),
(cid:26)(cid:24)(cid:15)(cid:14)M(cid:14)3(cid:14)(cid:15)(cid:6),
’ (cid:12)(cid:9)’
Figure 3.26 Differential nonlinearity.
(cid:27)(cid:24)(cid:15)(cid:14)M(cid:14)(cid:23)(cid:14)(cid:15)(cid:6),
(cid:2)
-
(cid:11)
$
(cid:14)
(cid:7)
(cid:13)
(cid:28)
(cid:7)
(cid:13)
(
(cid:22)(cid:22)(cid:22)
(cid:22)(cid:22)3
(cid:22)3(cid:22)
(cid:22)33
3(cid:22)(cid:22)
3(cid:22)3
33(cid:22)
333
(cid:27)(cid:10)(cid:28)(cid:13)(cid:7)(cid:14)L(cid:11)(cid:4)(cid:7)(cid:9)(cid:3)(cid:2)
’ (cid:12)(cid:9)’
Figure 3.27 Integral nonlinearity.
(cid:2)
-
(cid:11)
$
(cid:14)
(cid:7)
(cid:13)
(cid:28)
(cid:7)
(cid:13)
(
(cid:22)(cid:22)(cid:22)
(cid:22)(cid:22)3
(cid:22)3(cid:22)
(cid:22)33
3(cid:22)(cid:22)
3(cid:22)3
33(cid:22)
333
(cid:27)(cid:10)(cid:28)(cid:13)(cid:7)(cid:14)L(cid:11)(cid:4)(cid:7)(cid:9)(cid:3)(cid:2)
’ (cid:12)(cid:9)’
Figure 3.28 Monotonicity.
Total Harmonic Distortion. The harmonic distortion is calculated for an AD converter at
full range with a sinusoid (X1 = 0 dB) of given frequency. The selective measurement of
82
AD/DA Conversion
harmonics of the second to the ninth order are used to compute
THD = 20 log
(cid:28)
(cid:29)
(cid:29)
(cid:30)
∞(cid:6)
[10(−Xn/20)]2 dB
n=2
(cid:28)
(cid:29)
(cid:29)
(cid:30)
=
∞(cid:6)
[10(−Xn/20)]2 · 100%,
n=2
(3.37)
(3.38)
where Xn are the harmonics in dB.
THD+N: Total Harmonic Distortion plus Noise. For the calculation of harmonic dis-
tortion plus noise, the test signal is suppressed by a stop-band ﬁlter. The measurement of
harmonic distortion plus noise is performed by measuring the remaining broad-band noise
signal which consists of integral and differential nonlinearity, missing codes, aperture jitter,
analog noise and quantization error.
3.2.2 Parallel Converter
Parallel Converter. A direct method for AD conversion is called parallel conversion (ﬂash
converter). In parallel converters, the output voltage of the sample-and-hold circuit is com-
pared with a reference voltage UR with the help of 2w − 1 comparators (see Fig. 3.29).
The sample-and-hold circuit is controlled with sampling rate fS so that, during the hold
time tH , a constant voltage at the output of the sample-and-hold circuit is available. The
outputs of the comparators are fed at sampling clock rate into a (2w − 1)-bit register and
converted by a coding logic to a w-bit data word. This is fed at sampling clock rate to an
output register. The sampling rates that can be achieved lie between 1 and 500 MHz for a
resolution of up to 10 bits. Owing to the large number of comparators, the technique is not
feasible for high precision.
(cid:6)(cid:9)(cid:12)(cid:28)(cid:4)(cid:16)(cid:10)(cid:3)(cid:14)(cid:19)(cid:9)(cid:7)(cid:2)
%(cid:6)
;(cid:14).(cid:16)(cid:7)s
(cid:6)1=
!(cid:22)
!(cid:19)
$(cid:11)(cid:12)(cid:28)(cid:9)(cid:8)(cid:9)(cid:7)(cid:11)(cid:8)(cid:18)
(cid:19)1(cid:23)
(cid:23)(cid:14)(cid:14)(cid:5)(cid:22);
)(cid:26)
(cid:19)
(cid:19)
(cid:19)
(cid:19)1(cid:23)
(cid:23)
(cid:22)
;
(cid:8)
(cid:2)
-
(cid:11)
$
)(cid:26)
)(cid:26)
)(cid:26)
Figure 3.29 Parallel converter.
(cid:16)
(cid:8)
(cid:2)
(cid:7)
(cid:18)
(cid:3)
(cid:2)
(cid:19)
(cid:14)
(cid:7)
(cid:13)
(cid:28)
(cid:7)
(cid:13)
(
3.2 AD Converters
83
Half-ﬂash Converter. In half-ﬂash AD converters (Fig. 3.30), two m-bit parallel converters
are used in order to convert two different ranges. The ﬁrst m-bit AD converter gives a
digital output word which is converted into an analog voltage using an m-bit DA converter.
This voltage is now subtracted from the output voltage of the sample-and-hold circuit.
The difference voltage is digitized with a second m-bit AD converter. The rough and ﬁne
quantization leads to a w-bit data word with a subsequent logic.
(cid:6)1=
!(cid:22)
(cid:12)-.(cid:16)(cid:7)
&(cid:26)$
(cid:12)-.(cid:16)(cid:7)
&(cid:26)$
(cid:12)-.(cid:16)(cid:7)
(cid:26)&$
(cid:12)
(cid:23)
(cid:6)(cid:9)(cid:12)(cid:28)(cid:4)(cid:16)(cid:10)(cid:3)(cid:14)(cid:19)(cid:9)(cid:7)(cid:2)
%(cid:6)
;
(cid:15)(cid:11)(cid:3)(cid:16)(cid:20)
;(cid:14).(cid:16)(cid:7)s
(cid:7)
(cid:13)
(cid:28)
(cid:7)
(cid:13)
(
(cid:16)
(cid:8)
(cid:2)
(cid:7)
(cid:18)
(cid:3)
(cid:2)
(cid:19)
Figure 3.30 Half-ﬂash AD converter.
Subranging Converter. A combination of direct conversion and sequential procedure is
carried out for subranging AD converters (see Fig. 3.31). In contrast to the half-ﬂash
converter, only one parallel converter is required. The switches S1 and S2 take the values
of 0 and 1. First the output voltage of a sample-and-hold circuit and then the difference
voltage ampliﬁed by a factor 2m is fed to an m-bit AD converter. The difference voltage is
formed with the help of the output voltage of an m-bit DA converter and the output voltage
of the sample-and-hold circuit. The conversion rates lie between 100 kHz and 40 MHz
where a resolution of up to 16 bits is achieved.
(cid:6)(cid:9)(cid:12)(cid:28)(cid:4)(cid:16)(cid:10)(cid:3)(cid:14)(cid:19)(cid:9)(cid:7)(cid:2)
%(cid:6)
;(cid:14).(cid:16)(cid:7)s
;
(cid:15)(cid:11)(cid:3)(cid:16)(cid:20)
(cid:7)
(cid:13)
(cid:28)
(cid:7)
(cid:13)
(
(cid:16)
(cid:8)
(cid:2)
(cid:7)
(cid:18)
(cid:3)
(cid:2)
(cid:19)
(cid:6)1=
!(cid:22)
(cid:6)(cid:22)
(cid:6)(cid:23)
(cid:12)
(cid:23)
(cid:12)-.(cid:16)(cid:7)
&(cid:26)$
(cid:12)-.(cid:16)(cid:7)
(cid:26)&$
Figure 3.31 Subranging AD converter.
3.2.3 Successive Approximation
AD converters with successive approximation consist of the functional modules shown in
Fig. 3.32. The analog voltage is converted into a w-bit word within w cycles. The converter
consists of a comparator, a w-bit DA converter and logic for controlling the successive
approximation.
84
AD/DA Conversion
(cid:6)1=
!(cid:22)
(cid:15)(cid:11)(cid:3)(cid:16)(cid:20)
(cid:6)(cid:13)(cid:20)(cid:20)(cid:2)(cid:18)(cid:18)(cid:16)A(cid:2)
&(cid:28)(cid:28)(cid:8)(cid:11)’(cid:16)(cid:12)(cid:9)(cid:7)(cid:16)(cid:11)(cid:10)
;-.(cid:16)(cid:7)
(cid:26)&$
(cid:6)(cid:9)(cid:12)(cid:28)(cid:4)(cid:16)(cid:10)(cid:3)(cid:14)(cid:19)(cid:9)(cid:7)(cid:2)
%(cid:6)
;(cid:14).(cid:16)(cid:7)s
(cid:16)
(cid:8)
(cid:2)
(cid:7)
(cid:18)
(cid:3)
(cid:2)
(cid:19)
(cid:7)
(cid:7)
(cid:13)
(cid:28)
(cid:13)
(
Figure 3.32 AD converter with successive approximation.
The conversion process is explained with the help of Fig. 3.33. First, it is checked
whether a positive or negative voltage is present at the comparator. If it is positive, the out-
put +0.5UR is fed to a DA converter to check whether the output voltage of the comparator
is greater or less than +0.5UR. Then, the output of (+0.5 ± 0.25)UR is fed to the DA
comparator. The output of the comparator is then evaluated. This procedure is performed
w times and leads to a w-bit word.
!(cid:19)
9(cid:14)3(cid:29)5(cid:14)!(cid:19)
(cid:22)
(cid:23)
2
<
5
Figure 3.33 Successive approximation.
For a resolution of 12 bits, sampling rates of up to 1 MHz can be achieved. Higher
resolutions of more than 16 bits are possible at a lower sampling rates.
3.2.4 Counter Methods
In contrast to the conversion techniques of the previous sections for high conversion rates,
the following techniques are used for sampling rates smaller than 50 kHz.
Forward-backward Counter. A technique which operates like successive approximation
is the forward-backward counter shown in Fig. 3.34. A logic controls a clocked forward-
backward counter whose output data word provides an analog output voltage via a w-bit
DA converter. The difference signal between this voltage and the output voltage of the
sample-and-hold circuit determines the direction of counting. The counter stops when the
corresponding output voltage of the DA converter is equal to the output voltage of the
sample-and-hold circuit.
Single-slope Counter. The single-slope AD converter shown in Fig. 3.35 compares the
output voltage of the sample-and-hold circuit with a voltage of a sawtooth generator. The
3.2 AD Converters
85
(cid:6)1=
!(cid:22)
!(cid:23)
(cid:15)(cid:11)(cid:3)(cid:16)(cid:20)
@1,
$(cid:11)(cid:13)(cid:10)(cid:7)(cid:2)(cid:8)
;-.(cid:16)(cid:7)
(cid:26)&$
(cid:6)(cid:9)(cid:12)(cid:28)(cid:4)(cid:16)(cid:10)(cid:3)(cid:14)(cid:19)(cid:9)(cid:7)(cid:2)
%(cid:6)
;(cid:14).(cid:16)(cid:7)s
(cid:16)
(cid:8)
(cid:2)
(cid:7)
(cid:18)
(cid:3)
(cid:2)
(cid:19)
(cid:7)
(cid:7)
(cid:13)
(cid:28)
(cid:13)
(
Figure 3.34 AD converter with forward-backward counter.
sawtooth generator is started every sampling period. As long as the input voltage is greater
than the sawtooth voltage, the clock impulses are counted. The counter value corresponds
to the digital value of the input voltage.
!(cid:22)
!(cid:19)
(cid:7)
(cid:7)
(cid:7)
(cid:6)1=
!(cid:23)
!(cid:22)
!(cid:19)
(cid:15)(cid:11)(cid:3)(cid:16)(cid:20)
$(cid:11)(cid:13)(cid:10)(cid:7)(cid:2)(cid:8)
;(cid:14).(cid:16)(cid:7)s
(cid:7)
(cid:13)
(cid:28)
(cid:7)
(cid:13)
(
(cid:16)
(cid:8)
(cid:2)
(cid:7)
(cid:18)
(cid:3)
(cid:2)
(cid:19)
(cid:6)(cid:9);(cid:7)(cid:11)(cid:11)(cid:7)(cid:21)
#(cid:2)(cid:10)(cid:2)(cid:8)(cid:9)(cid:7)(cid:11)(cid:8)
$(cid:4)(cid:11)(cid:20)(cid:17)
#(cid:2)(cid:10)(cid:2)(cid:8)(cid:9)(cid:7)(cid:11)(cid:8)
Figure 3.35 Single-slope AD converter.
(cid:6)(cid:9)(cid:12)(cid:28)(cid:4)(cid:16)(cid:10)(cid:3)(cid:14)(cid:19)(cid:9)(cid:7)(cid:2)
%(cid:6)
Dual-slope Converter. A dual-slope AD converter is shown in Fig. 3.36. In the ﬁrst phase
in which a switch S1 is closed for a counter period t1, the output voltage of the sample-
and-hold circuit is fed to an integrator of time-constant τ . During the second phase, the
switch S2 is closed and the switch S1 is opened. The reference voltage is switched to the
integrator and the time to reach a threshold is determined by counting the clock impulses by
a counter. Figure 3.36 demonstrates this for three different voltages U2. The slope during
time t1 is proportional to the output voltage U2 of the sample-and-hold circuit, whereas the
slope is constant when the reference voltage UR is connected to the integrator. The ratio
U2/UR = t2/t1 leads to the digital output word.
3.2.5 Delta-sigma AD Converter
The delta-sigma AD converter in Fig. 3.37 requires no sample-and-hold circuit owing to its
high conversion rate. The analog band-limiting low-pass ﬁlter and the digital low-pass ﬁlter
86
AD/DA Conversion
!(cid:27)
(cid:6)(cid:4)(cid:11)(cid:28)(cid:2)
(cid:23)!
(cid:11)
(cid:6)(cid:4)(cid:11)(cid:28)(cid:2)
!
(cid:19)
(cid:11)
(cid:9)
.
(cid:20)
(cid:7)(cid:22)
(cid:7)
(cid:7)(cid:23)
(cid:9)
(cid:7)(cid:23)
.
(cid:7)(cid:23)
(cid:20)
(cid:6)1=
!(cid:22)
!(cid:23)
(cid:6)(cid:22)
(cid:27)(cid:10)(cid:7)(cid:2)(cid:5)
(cid:3)(cid:8)(cid:9)(cid:7)(cid:16)(cid:11)(cid:10)
!(cid:27)
(cid:15)(cid:11)(cid:3)(cid:16)(cid:20)
$(cid:11)(cid:13)(cid:10)(cid:7)(cid:2)(cid:8)
;(cid:14).(cid:16)(cid:7)s
(cid:7)
(cid:13)
(cid:28)
(cid:7)
(cid:13)
(
(cid:16)
(cid:8)
(cid:2)
(cid:7)
(cid:18)
(cid:3)
(cid:2)
(cid:19)
!(cid:19)
(cid:6)(cid:23)
(cid:1)(cid:2)(cid:8)(cid:16)(cid:11)-
$(cid:11)(cid:13)(cid:10)(cid:7)(cid:2)(cid:8)
(cid:6)(cid:9)(cid:12)(cid:28)(cid:4)(cid:16)(cid:10)(cid:3)(cid:14)(cid:19)(cid:9)(cid:7)(cid:2)
%(cid:6)
Figure 3.36 Dual-slope AD converter.
for downsampling to a sampling rate fS are usually on the same circuit. The linear phase
nonrecursive digital low-pass ﬁlter in Fig. 3.37 has a 1-bit input signal and leads to a w-bit
output signal owing to the N ﬁlter coefﬁcients h0, h1, . . . , hN−1 which are implemented
with a word-length of w bits. The output signal of the ﬁlter results from the summation of
the ﬁlter coefﬁcients (0 or 1) of the nonrecursive low-pass ﬁlter. The downsampling by a
factor L is performed by taking every Lth sample out of the ﬁlter and writing to the output
register. In order to reduce the number of operations the ﬁltering and downsampling can be
performed only every Lth input sample.
Applications of delta-sigma AD converters are found at sampling rates of up to 100 kHz
with a resolution of up to 24 bits.
3.3 DA Converters
Circuit principles for DA converters are mainly based on direct conversion techniques of
the input code. Achievable sampling rates are accordingly high.
3.3.1 Speciﬁcations
The deﬁnitions of resolution, total harmonic distortion (THD) and total harmonic distortion
plus noise (THD+N) correspond to those for AD converters. Further speciﬁcations are
discussed in the following.
3.3 DA Converters
87
(cid:6)(cid:9)(cid:12)(cid:28)(cid:4)(cid:16)(cid:10)(cid:3)(cid:14)(cid:19)(cid:9)(cid:7)(cid:2)
(cid:6)(cid:15)%
(cid:6)(cid:9)(cid:12)(cid:28)(cid:4)(cid:16)(cid:10)(cid:3)(cid:14)(cid:19)(cid:9)(cid:7)(cid:2)
(cid:6)%
&(cid:10)(cid:9)(cid:4)(cid:11)(cid:3)
(cid:15)(cid:11);(cid:5)(cid:28)(cid:9)(cid:18)(cid:18)
(cid:26)(cid:2)(cid:4)(cid:7)(cid:9)(cid:5)(cid:6)(cid:16)(cid:3)(cid:12)(cid:9)
/(cid:11)-(cid:13)(cid:4)(cid:9)(cid:7)(cid:11)(cid:8)
(cid:22)(cid:14).(cid:16)(cid:7)
(cid:26)(cid:16)(cid:3)(cid:16)(cid:7)(cid:9)(cid:4)
(cid:15)(cid:11);(cid:5)(cid:28)(cid:9)(cid:18)(cid:18)
(cid:15)
;(cid:14).(cid:16)(cid:7)s
(cid:7)
(cid:13)
(cid:28)
(cid:7)
(cid:13)
(
(cid:16)
(cid:8)
(cid:2)
(cid:7)
(cid:18)
(cid:3)
(cid:2)
(cid:19)
!(cid:22)
(cid:22)(cid:14).(cid:16)(cid:7)
 (cid:5)(cid:22)
 (cid:5)(cid:22)
 (cid:5)(cid:22)
 (cid:5)(cid:22)
(cid:21)
3
(cid:21)
(cid:22)
(cid:21)
(cid:23)
(cid:21)
2
(cid:21)
(cid:24)(cid:5)(cid:23)
(cid:21)
(cid:24)(cid:5)(cid:22)
Figure 3.37 Delta-sigma AD converter.
;(cid:14).(cid:16)(cid:7)s
Settling Time. The time interval between transferring a binary word and achieving the
analog output value within a speciﬁc error range is called the settling time tSE. The set-
= 1/tSE. Within this time,
tling time determines the maximum conversion frequency fSmax
glitches between consecutive amplitude values can occur (see Fig. 3.38). With the help of
a sample-and-hold circuit (deglitcher), the output voltage of the DA converter is sampled
after the settling time and held.
% (cid:6)
;(cid:14).(cid:16)(cid:7)s
(cid:26)&$
(cid:6)1=
!(cid:22)
!(cid:23)
!(cid:22)
(cid:7)(cid:6)(cid:30)
+(cid:6)
+(cid:6)
!(cid:23)
(cid:7)
(cid:7)
Figure 3.38 Settling time and sample-and-hold function.
Offset and Gain Error. The offset and gain errors of a DA converter are shown in Fig. 3.39.
Differential Nonlinearity. The differential nonlinearity for DA converters describes the
step size error of a code word in LSB units. For ideal quantization, the increase (cid:8)x of
the output voltage until the next code word corresponding to the output voltage is equal to
88
AD/DA Conversion
’ (cid:12)(cid:9)’
(%%(cid:18)(cid:2)(cid:7)(cid:14)(cid:30)(cid:8)(cid:8)(cid:11)(cid:8)
’ (cid:12)(cid:9)’
#(cid:9)(cid:16)(cid:10)(cid:14)(cid:30)(cid:8)(cid:8)(cid:11)(cid:8)
(cid:2)
(cid:3)
(cid:9)
(cid:7)
(cid:4)
(cid:11)
L
(cid:14)
(cid:7)
(cid:7)
(cid:13)
(cid:28)
(cid:13)
(
(cid:2)
(cid:3)
(cid:9)
(cid:7)
(cid:4)
(cid:11)
L
(cid:14)
(cid:7)
(cid:7)
(cid:13)
(cid:28)
(cid:13)
(
3
3
3
3
3
(cid:22)
3
(cid:22)
3
3
(cid:22)
(cid:22)
(cid:22)
3
3
(cid:22)
3
(cid:22)
(cid:22)
(cid:22)
3
(cid:22)
(cid:22)
(cid:22)
3
3
3
3
3
(cid:22)
3
(cid:22)
3
3
(cid:22)
(cid:22)
(cid:22)
3
3
(cid:22)
3
(cid:22)
(cid:22)
(cid:22)
3
(cid:22)
(cid:22)
(cid:22)
(cid:27)(cid:10)(cid:28)(cid:13)(cid:7)(cid:14)$(cid:11)-(cid:2)
(cid:27)(cid:10)(cid:28)(cid:13)(cid:7)(cid:14)$(cid:11)-(cid:2)
Figure 3.39 Offset and gain error.
’ (cid:12)(cid:9)’
(cid:2)
(cid:3)
(cid:9)
(cid:7)
(cid:4)
(cid:11)
L
(cid:14)
(cid:7)
(cid:13)
(cid:28)
(cid:7)
(cid:13)
(
(cid:26)(cid:24)(cid:15)(cid:14)M(cid:14)(cid:5)(cid:22)(cid:14)(cid:15)(cid:6),
(cid:26)(cid:24)(cid:15)(cid:14)M(cid:14)(cid:5)3(cid:29)(cid:23)5(cid:14)(cid:15)(cid:6),
(cid:26)(cid:24)(cid:15)(cid:14)M(cid:14)3(cid:29)5(cid:14)(cid:15)(cid:6),
3
3
3
3
3
(cid:22)
3
(cid:22)
3
3
(cid:22)
(cid:22)
(cid:22)
3
3
(cid:22)
3
(cid:22)
(cid:22)
(cid:22)
3
(cid:22)
(cid:22)
(cid:22)
(cid:27)(cid:10)(cid:28)(cid:13)(cid:7)(cid:14)$(cid:11)-(cid:2)
Figure 3.40 Differential nonlinearity.
the quantization step size Q (see Fig. 3.40). The difference between two consecutive input
codes is termed (cid:8)xQ. Differential nonlinearity is given by
DNL = (cid:8)x/Q
(cid:8)xQ
− 1 LSB.
(3.39)
For the code steps from 001 to 010 as shown in Fig. 3.40, the step size is 1.5 LSB, and
therefore the differential nonlinearity DNL = 0.5 LSB. The step size between the codes 010
and 100 is 0.75 LSB and DNL = −0.25. The step size for the code change from 011 to 100
is 0 LSB (DNL = −1 LSB).
Integral Nonlinearity. The integral nonlinearity describes the maximum deviation of the
output voltage of a real DA converter from the ideal straight line (see Fig. 3.41).
Monotonicity. The continuous increase in the output voltage with increasing input code
and the continuous decrease in the output voltage with decreasing input code is called
monotonicity. A non-monotonic behavior is presented in Fig. 3.42.
3.3 DA Converters
89
’ (cid:12)(cid:9)’
(cid:2)
(cid:3)
(cid:9)
(cid:7)
(cid:4)
(cid:11)
L
(cid:14)
(cid:7)
(cid:7)
(cid:13)
(cid:28)
(cid:13)
(
(cid:27)(cid:24)(cid:15)(cid:14)M(cid:14)(cid:22)(cid:29)5(cid:14)(cid:15)(cid:6),
(cid:27)(cid:24)(cid:15)(cid:14)M(cid:14)(cid:22)(cid:14)(cid:15)(cid:6),
3
3
3
3
3
(cid:22)
3
(cid:22)
3
3
(cid:22)
(cid:22)
(cid:22)
3
3
(cid:22)
3
(cid:22)
(cid:22)
(cid:22)
3
(cid:22)
(cid:22)
(cid:22)
(cid:27)(cid:10)(cid:28)(cid:13)(cid:7)(cid:14)$(cid:11)-(cid:2)
Figure 3.41 Integral nonlinearity.
’ (cid:12)(cid:9)’
(cid:2)
(cid:3)
(cid:9)
(cid:7)
(cid:9)
(cid:11)
L
(cid:4)
(cid:14)
(cid:7)
(cid:13)
(cid:28)
(cid:7)
(cid:13)
(
3
3
3
3
3
(cid:22)
3
(cid:22)
3
3
(cid:22)
(cid:22)
(cid:22)
3
3
(cid:22)
3
(cid:22)
(cid:22)
(cid:22)
3
(cid:22)
(cid:22)
(cid:22)
(cid:27)(cid:10)(cid:28)(cid:13)(cid:7)(cid:14)$(cid:11)-(cid:2)
Figure 3.42 Monotonicity.
3.3.2 Switched Voltage and Current Sources
Switched Voltage Sources. The DA conversion with switched voltage sources shown in
Fig. 3.43a is carried out with a reference voltage connected to a resistor network. The
resistor network consists of 2w resistors of equal resistance and is switched in stages to a
binary-controlled decoder so that, at the output, a voltage U2 is present corresponding to
the input code. Figure 3.43b shows the decoder for a 3-bit input code 101.
Switched Current Sources. DA conversion with 2w switched current sources is shown
in Fig. 3.44. The decoder switches the corresponding number of current sources onto the
current-voltage converter. The advantage of both techniques is the monotonicity which is
guaranteed for ideal switches but also for slightly deviating resistances. The large number
of resistors in switched current sources or the large number of switched current sources
causes problems for long word-lengths. The techniques are used in combination with other
methods for DA conversion of higher signiﬁcant bits.
3.3.3 Weighted Resistors and Capacitors
A reduction in the number of identical resistors or current sources is achieved with the
following method.
90
AD/DA Conversion
!(cid:19)
(cid:19)
(cid:19)
(cid:19)
(cid:19)
;
7(cid:23)(cid:14)(cid:14)(cid:5)(cid:22):1(cid:23)(cid:14)(cid:14)(cid:14)!
;
;7(cid:23)(cid:14)(cid:14)(cid:5)(cid:23):1(cid:23)(cid:14)(cid:14)(cid:14)!;
!(cid:19)
(cid:19)
(cid:19)
(cid:8)
(cid:2)
-
(cid:11)
(cid:20)
(cid:2)
(cid:26)
!(cid:23)
3(cid:26)
(cid:26);(cid:5)(cid:22)
;(cid:22)1(cid:23)(cid:14)
(cid:19)!
3(cid:14)L
(cid:9):
(cid:19)
(cid:19)
(cid:19)
(cid:19)
(cid:19)
(cid:19)
(cid:19)
(cid:19)
(cid:22)
3
(cid:22)
3
(cid:22)
3
(cid:22)
3
(cid:22)
3
(cid:22)
3
(cid:22)
3
!(cid:23)
(cid:22)
(cid:15)(cid:6),
3
(cid:22)
/(cid:6),
.:
Figure 3.43 Switched voltage sources.
(cid:22)
(cid:23)
2
;(cid:5)(cid:22)(cid:23)
(cid:23) ;
(cid:27)
(cid:27)
(cid:27)
(cid:27)
(cid:27)
(cid:26)(cid:2)(cid:20)(cid:11)-(cid:2)(cid:8)
(cid:19)
3(cid:26)
(cid:26);(cid:5)(cid:22)
!(cid:23)
Figure 3.44 Switched current sources.
Weighted Resistors. DA conversion with w switched current sources which are weighted
according to
I1 = 2I2 = 4I3 = · · · = 2w−1Iw
(3.40)
is shown in Fig. 3.45. The output voltage is
U2 = −R · I = −R · (b1I120 + b2I221 + b3I322 + · · · + bwIw2w−1),
(3.41)
where bn takes values 0 or 1. The implementation of DA conversion with switched current
sources is carried out with weighted resistors as shown in Fig. 3.46. The output voltage is
(cid:1)
(cid:2)
U2 = R · I = R
+ b2
b1
2R
4R
−2 + b32
−1 + b22
+ b4
8R
+ · · · + bw
2wR
−w)UR.
UR
−3 + · · · + bw2
= (b12
(3.42)
(3.43)
Weighted Capacitors. DA conversion with weighted capacitors is shown in Fig. 3.47.
During the ﬁrst phase (switch position 1 in Fig. 3.47) all capacitors are discharged. During
3.3 DA Converters
91
(cid:27)
(cid:22)
(cid:27)
(cid:23)
(cid:27)
2
(cid:27)
;
(cid:19)
Figure 3.45 Weighted current sources.
/(cid:6),
(cid:15)(cid:6),
!(cid:19)
(cid:23)(cid:19)
(cid:23)
(cid:23)(cid:14)(cid:14)(cid:19)
(cid:23)(cid:14)(cid:14)(cid:19)2
;(cid:23)(cid:14)(cid:14)(cid:14)(cid:19)(cid:14)(cid:14)
(cid:19)
!(cid:23)
!(cid:23)
Figure 3.46 DA conversion with weighted resistors.
the second phase, all capacitors that belong to 1 bit are connected to a reference voltage.
Those capacitors belonging to 0 bits are connected to ground. The charge on the capacitors
Ca that are connected with the reference voltage can be set equal to the total charge on all
capacitors Cg, which leads to
UR Ca = UR
(cid:1)
b1C + b2C
2
+ b3C
22
+ · · · + bwC
2w−1
(cid:2)
= Cg U2 = 2 CU2.
(3.44)
Hence, the output voltage is
U2 = (b12
−1 + b22
−2 + b32
−3 + · · · + bw2
−w)UR.
(3.45)
!(cid:19)
(cid:22)
(cid:22)
(cid:22)
(cid:22)
$
$1(cid:23)
$1<
$
(cid:23)(cid:14)(cid:14);(cid:5)(cid:22)
(cid:22)
$
(cid:23)(cid:14)(cid:14);(cid:5)(cid:22)
(cid:22)
(cid:7)(cid:2)(cid:8)(cid:12)(cid:16)(cid:10)(cid:9)(cid:7)(cid:16)(cid:11)(cid:10)(cid:14)(cid:20)(cid:9)(cid:28)(cid:9)(cid:20)(cid:16)(cid:7)(cid:11)(cid:8)
! (cid:23)
Figure 3.47 DA conversion with weighted capacitors.
92
AD/DA Conversion
3.3.4 R-2R Resistor Networks
The DA conversion with switched current sources can also be carried out with an R-2R
resistor network as shown in Fig. 3.48. In contrast to the method with weighted resistors,
the ratio of the smallest to largest resistor is reduced to 2:1.
(cid:19)!
(cid:19)
(cid:27)(cid:22)
(cid:22)
(cid:19)
(cid:27)(cid:23)
(cid:23)
(cid:27)2
2
(cid:19)
;(cid:5)(cid:22)
(cid:27);
;
(cid:23)(cid:19)
(cid:23)(cid:19)
(cid:23)(cid:19)
(cid:23)(cid:19)
(cid:23)(cid:19)
(cid:27)(cid:22)
(cid:27)
(cid:23)
(cid:27)
2
(cid:27)
;
(cid:19)
!&
Figure 3.48 Switched current sources with R-2R resistor network.
The weighting of currents is achieved by a current division at every junction. Looking
right from every junction, a resulting resistance R + 2R (cid:11) 2R = 2R is found which is equal
to the resistance in the vertical direction downwards from the junction. For the current
from junction 1 it follows that I1 = UR/2R, and for the current from junction 2 I2 = I1/2.
Hence, a binary weighting of the w currents is given by
I1 = 2I2 = 4I3 = · · · = 2w−1Iw.
The output voltage U2 can be written as
(cid:1)
U2 = − RI = −R
= − UR(b12
b1
2R
−1 + b22
+ b2
4R
−2 + b32
+ · · · + bw
+ b3
8R
−3 + · · · + bw2
2w−1R
−w).
(cid:2)
UR
(3.46)
(3.47)
(3.48)
3.3.5 Delta-sigma DA Converter
A delta-sigma DA converter is shown in Fig. 3.49. The converter is provided with w-bit
data words by an input register with the sampling rate fS. This is followed by a sample rate
conversion up to LfS by upsampling and a digital low-pass ﬁlter. A delta-sigma modulator
converts the w-bit input signal into a 1-bit output signal. The delta-sigma modulator corre-
sponds to the model in Section 3.1.3. Subsequently, the DA conversion of the 1-bit signal
is performed followed by the reconstruction of the time-continuous signal by an analog
low-pass ﬁlter.
3.4 Java Applet – Oversampling and Quantization
The applet shown in Fig. 3.50 demonstrates the inﬂuence of oversampling on power spec-
tral density of the quantization error. For a given quantization word-length the noise level
3.4 Java Applet – Oversampling and Quantization
93
(cid:6)(cid:9)(cid:12)(cid:28)(cid:4)(cid:16)(cid:10)(cid:3)(cid:14)(cid:19)(cid:9)(cid:7)(cid:2)
(cid:6)%
(cid:6)(cid:9)(cid:12)(cid:28)(cid:4)(cid:16)(cid:10)(cid:3)(cid:14)(cid:19)(cid:9)(cid:7)(cid:2)
(cid:15) (cid:6)%
;(cid:14).(cid:16)(cid:7)s
(cid:7)
(cid:13)
(cid:28)
(cid:10)
(cid:27)
(cid:16)
(cid:8)
(cid:2)
(cid:7)
(cid:18)
(cid:3)
(cid:2)
(cid:19)
(cid:15)
(cid:26)(cid:16)(cid:3)(cid:16)(cid:7)(cid:9)(cid:4)
(cid:15)(cid:11);(cid:5)(cid:28)(cid:9)(cid:18)(cid:18)
(cid:26)(cid:2)(cid:4)(cid:7)(cid:9)(cid:5)(cid:6)(cid:16)(cid:3)(cid:12)(cid:9)
/(cid:11)-(cid:13)(cid:4)(cid:9)(cid:7)(cid:11)(cid:8)
(cid:22)-.(cid:16)(cid:7)
(cid:26)&$
&(cid:10)(cid:9)(cid:4)(cid:11)(cid:3)
(cid:15)(cid:11);(cid:5)(cid:28)(cid:9)(cid:18)(cid:18)
! (cid:23)
;(cid:14).(cid:16)ts
(cid:22)(cid:14).(cid:16)(cid:7)
=7 :
Figure 3.49 Delta-sigma DA converter.
can be reduced by changing the oversampling factor. The graphical interface of this applet
presents several quantization and oversampling values; these can be used to experiment the
noise reduction level. An additional FFT spectral representation provides a visualization of
this audio effect.
Figure 3.50 Java applet – oversampling and quantization.
The following functions can be selected on the lower right of the graphical user inter-
face:
94
• Quantizer
AD/DA Conversion
– word-length w leads to quantization step size Q = 2w−1.
• Dither
– rect dither – uniform probability density function
– tri dither – triangular probability density function
– high-pass dither – triangular probability density function and high-pass power
spectral density.
• Noise shaping
– ﬁrst-order H (z) = z−1.
• Oversampling factor
– Factors from 4 up to 64 can be tested depending on the CPU performance of
your machine.
You can choose between two predeﬁned audio ﬁles from our web server (audio1.wav or
audio2.wav) or your own local wav ﬁle to be processed [Gui05].
3.5 Exercises
1. Oversampling
1. How do we deﬁne the power spectral density SXX(ej (cid:4)) of a signal x(n)?
2. What is the relationship between signal power σ 2
X (variance) and power spectral
density SXX(ej (cid:4))?
3. Why do we need to oversample a time-domain signal?
4. Explain why an oversampled PCM A/D converter has lower quantization noise power
in the base-band than a Nyquist rate sampled PCM A/D converter.
5. How do we perform oversampling by a factor of L in the time domain?
6. Explain the frequency-domain interpretation of the oversampling operation.
7. What is the pass-band and stop-band frequency of the analog anti-aliasing ﬁlter?
8. What is the pass-band and stop-band frequency of the digital anti-aliasing ﬁlter
before downsampling?
9. How is the downsampling operation performed (time-domain and frequency-domain
explanation)?
References
2. Delta-sigma Conversion
95
1. Why can we apply noise shaping in an oversampled AD converter?
2. Show how the delta-sigma converter (DSC) has a lower quantization error power in
the base-band than an oversampled PCM A/D converter.
3. How do the power spectral density and variance change in relation to the order of the
DSC?
4. How is noise shaping achieved in an oversampled delta-sigma AD converter?
5. Show the noise shaping effect (with Matlab plots) of a delta sigma modulator and
how the improvement of the signal-to-noise for pure oversampling and delta-sigma
modulator is achieved.
6. Using the previous Matlab plots, specify which order and oversampling factor L will
be needed for a 1-bit delta-sigma converter for SNR = 100 dB.
7. What is the difference between the delta-sigma modulator in the delta-sigma AD
converter and the delta-sigma DA converter?
8. How do we achieve a w-bit signal representation at Nyquist sampling frequency from
an oversampled 1-bit signal?
9. Why do we need to oversample a w-bit signal for a delta-sigma DA converter?
References
[Can85]
J. C. Candy: A Use of Double Integration in Sigma Delta Modulation, IEEE
Trans. Commun., Vol. COM-37, pp. 249–258, March 1985.
[Can92]
J. C. Candy, G. C. Temes, Ed.: Oversampling Delta-Sigma Data Converters,
IEEE Press, Piscataway, NJ, 1992.
[Cha90] K. Chao et al: A High Order Topology for Interpolative Modulators for
Oversampling A/D Converters, IEEE Trans. Circuits and Syst., Vol. CAS-37,
pp. 309–318, March 1990.
[Cro83]
R. E. Crochiere, L. R. Rabiner: Multirate Digital Signal Processing, Prentice
Hall, Englewood Cliffs, NJ, 1983.
[Fli00]
N. Fliege: Multirate Digital Signal Processing, John Wiley & Sons, Ltd,
Chichester, 2000.
[Gui05] M. Guillemard, C. Ruwwe, U. Zölzer: J-DAFx – Digital Audio Effects in Java,
Proc. 8th Int. Conference on Digital Audio Effects (DAFx-05), pp. 161–166,
Madrid, 2005.
[Ino63]
H. Inose, Y. Yasuda: A Unity Bit Coding Method by Negative Feedback, Proc.
IEEE, Vol. 51, pp. 1524–1535, November 1963.
96
AD/DA Conversion
[Mat87] Y. Matsuya et al: A 16-bit Oversampling A-to-D Conversion Technology Using
Triple-Integration Noise Shaping, IEEE J. Solid-State Circuits, Vol. SC-22, pp.
921–929, December 1987.
[She86]
D. H. Sheingold, Ed.: Analog-Digital Conversion Handbook, 3rd edn, Prentice
Hall, Englewood Cliffs, NJ, 1986.
[Vai93]
P. P. Vaidyanathan: Multirate Systems and Filter Banks, Prentice Hall,
Englewood Cliffs, NJ, 1993.
Chapter 4
Audio Processing Systems
Digital signal processors (DSPs) are used for discrete-time signal processing. Their ar-
chitecture and instruction set is specially designed for real-time processing of signal pro-
cessing algorithms. DSPs of different manufacturers and their use in practical circuits
will be discussed. The restriction to the architecture and practical circuits will provide the
user with the criteria necessary for selecting a DSP for a particular application. From the
architectural features of different DSPs, the advantages of a certain processor with respect
to fast execution of algorithms (digital ﬁlter, adaptive ﬁlter, FFT, etc.) automatically result.
The programming methods and application programs are not dealt with here, because the
DSP user guides from different manufacturers provide adequate information in the form of
sample programs for a variety of signal processing algorithms.
After comparing DSPs with other microcomputers, the following topics will be dis-
cussed in the forthcoming sections:
• ﬁxed-point DSPs;
• ﬂoating-point DSPs;
• development tools;
• single-processor systems (peripherals, control principles);
• multi-processor systems (coupling principles, control principles).
The internal design of microcomputers is mainly based on two architectures; the von Neu-
mann architecture which uses a shared instruction/data bus; and the Harvard architecture
which has separate buses for instructions and data. Processors based on these architectures
are CISCs, RISCs and DSPs. Their characteristics are given in Table 4.1. Besides the
internal properties listed in the table, DSPs have special on-chip peripherals which are
suited to signal processing applications. The fast response to external interrupts enables
their use in real-time operating systems.
Digital Audio Signal Processing Second Edition Udo Zölzer
© 2008 John Wiley & Sons, Ltd
98
Audio Processing Systems
Table 4.1 CISC, RISC and DSP.
Type
Characteristics
CISC Complex Instruction Set Computer
• von Neumann architecture
• assembler programming
• large number of instructions
• computer families
• compilers
• application: universal microcomputers
RISC Reduced Instruction Set Computer
DSP
• von Neumann architecture/Harvard architecture
• number of instructions <50
• number of address modes <4, instruction formats <4
• hard-wired instruction (no microprogramming)
• processing most of the instructions in one cycle
• optimizing compilers for high-level programming languages
• application: workstations
Digital Signal Processor
• Harvard architecture
• several internal data buses
• assembler programming
• parallel processing of several instructions in one cycle
• optimizing compilers for high-level programming languages
• real-time operating systems
• application: real-time signal processing
4.1 Digital Signal Processors
4.1.1 Fixed-point DSPs
The discrete-time and discrete-amplitude output of an AD converter is usually represented
in 2’s complement format. The processing of these number sequences is carried out with
ﬁxed-point or ﬂoating-point arithmetic. The output of a processed signal is again in 2’s
complement format and is fed to a DA converter. The signed fractional representation (2’s
complement) is the common method for algorithms in ﬁxed-point number representation.
For address generation and modulo operations unsigned integers are used. Figure 4.1 shows
a schematic diagram of a typical ﬁxed-point DSP. The main building blocks are program
controller, arithmetic logic unit (ALU) with a multiplier-accumulator (MAC), program and
data memory and interfaces to external memory and peripherals. All blocks are connected
with each other by an internal bus system. The internal bus system has separate instruction
and data buses. The data bus itself can consist of more than one parallel bus enabling
it, for instance, to transmit both operands of a multiplication instruction to the MAC in
parallel. The internal memory consists of instruction and data RAM and additional ROM
memory. This internal memory permits fast execution of internal instructions and data
transfer. For increasing memory space, address/control and data buses are connected to
4.1 Digital Signal Processors
99
external memories like EPROM, ROM and RAM. The connection of the external bus
system to the internal bus architecture has great inﬂuence on efﬁcient execution of external
instructions as well as on processing external data. In order to connect serially operating
AD/DA converters, special serial interfaces with high transmission rates are offered by
several DSPs. Moreover, some processors support direct connection to an RS232 interface.
The control from a microprocessor can be achieved via a host interface with a word-length
of 8 bits.
/
(
(cid:19)
(cid:26)
(cid:1)
(cid:5)
1
/
&
(cid:19)
(cid:1)
(cid:5)
/
&
(cid:19)
(cid:26)
(cid:5)
(cid:26)(cid:9)(cid:7)(cid:9)(cid:14),(cid:13)(cid:18)
(cid:1)(cid:8)(cid:11)(cid:3)(cid:8)(cid:9)(cid:12)(cid:14),(cid:13)(cid:18)
&--(cid:8)(cid:2)(cid:18)(cid:18)
(cid:26)(cid:9)(cid:7)(cid:9)
$(cid:11)(cid:10)(cid:7)(cid:8)(cid:11)(cid:4)
"
(cid:8)
(cid:11)
(cid:12)
(cid:2)
/
(cid:14)
(cid:29)
(cid:7)
’
(cid:30)
=(cid:11)(cid:18)(cid:7)(cid:14)(cid:27)(cid:10)(cid:7)(cid:2)(cid:8)%(cid:9)(cid:20)(cid:2)
(cid:6)(cid:2)(cid:8)(cid:16)(cid:9)(cid:4)(cid:14)&(cid:26)1(cid:26)&(cid:14)(cid:27)(cid:10)(cid:7)(cid:2)(cid:8)%(cid:9)(cid:20)(cid:2)
(cid:6)(cid:2)(cid:8)(cid:16)(cid:9)(cid:4)(cid:14)(cid:27)(cid:10)(cid:7)(cid:2)(cid:8)%(cid:9)(cid:20)(cid:2)
(cid:4)
(cid:18)
(cid:9)
(cid:8)
(cid:2)
(cid:21)
(cid:28)
(cid:16)
(cid:8)
(cid:2)
(cid:1)
(cid:14)
(cid:29)
(cid:7)
’
(cid:30)
&(cid:15)!
/&$1&$$
(cid:1)(cid:8)(cid:11)(cid:3)(cid:8)(cid:9)(cid:12)
$(cid:11)(cid:10)(cid:7)(cid:8)(cid:11)(cid:4)(cid:4)(cid:2)(cid:8)
Figure 4.1 Schematic diagram of a ﬁxed-point DSP.
An overview of ﬁxed-point DSPs with respect to word-length and cycle time is shown
in Table 4.2. Basically, the precision of the arithmetic can be doubled if quantization affects
the stability and numeric precision of the applied algorithm. The cycle time in connection
with processing time (in processor cycles) of a combined multiplication and accumulation
command gives insight into the computing power of a particular processor type. The cycle
time directly results from the maximum clock frequency. The instruction processing time
depends mainly on the internal instruction and data structure as well as on the external
memory connections of the processor. The fast access to external instruction and data
memories is of special signiﬁcance in complex algorithms and in processing huge data
loads. Further attention has to be paid to the linking of serial data connections with AD/DA
converters and the control by a host computer over a special host interface. Complex
interface circuits could therefore be avoided. For stand-alone solutions, program loading
from a simple external EPROM can also be done.
For signal processing algorithms, the following software commands are necessary:
1. MAC (multiply and accumulate) → combined multiplication and addition command;
2. simultaneous transfer of both operands for multiplication to the MAC (parallel move);
3. bit-reversed addressing (for FFT);
4. modulo addressing (for windowing and ﬁltering).
Different signal processors have different processing times for FFT implementations.
The latest signal processors with improved architecture have shorter processing times. The
instruction cycles for the combined multiplication and accumulation command (applica-
tion: windowing, ﬁltering) are approximately equal for different processors, but processing
cycles for external operands have to be considered.
100
Audio Processing Systems
Table 4.2 Fixed-point DSPs (Analog Devices AD, Texas Instruments TI, Motorola MOT, Agere
Systems AG).
Type
Word-length
Cycle time
MHz/ns
Computation power
MMACS
ADSP-BF533
ADSP-BF561
ADSP-T201
TI-TMS320C6414
MOT-DSP56309
MOT-DSP56L307
AG-DSP16410 × 2
16
16
32
16
24
24
16
756/1.3
756/1.3
600/1.67
1000/1
100/10
160/6.3
195/5.1
1512
3024
4800
4000
100
160
780
4.1.2 Floating-point DSPs
Figure 4.2 shows the block diagram of a typical ﬂoating-point DSP. The main characteris-
tics of the different architectures are the dual-port principle (Motorola, Texas Instruments)
and the external Harvard architecture (Analog Devices, NEC). Floating-point DSPs in-
ternally have multiple bus systems in order to accelerate data transfer to the processing
unit. An on-chip DMA controller and cache memory support higher data transfer rates. An
overview of ﬂoating-point DSPs is shown in Table 4.3. Besides the standardized ﬂoating-
point representation IEEE-754, there are also manufacturer-dependent number representa-
tions.
/
(
(cid:19)
(cid:26)
(cid:1)
(cid:5)
1
/
&
(cid:19)
(cid:1)
(cid:5)
/
&
(cid:19)
(cid:26)
(cid:5)
/(cid:13)(cid:4)(cid:7)(cid:16)(cid:28)(cid:4)(cid:2)
(cid:26)(cid:9)(cid:7)(cid:9)(cid:14),(cid:13)(cid:18)
(cid:1)(cid:8)(cid:11)(cid:3)(cid:8)(cid:9)(cid:12)(cid:14),(cid:13)(cid:18)
"
(cid:8)
(cid:11)
(cid:12)
(cid:2)
/
(cid:14)
(cid:29)
(cid:7)
’
(cid:30)
&--(cid:8)(cid:2)(cid:18)(cid:18)
,
(cid:26)(cid:9)(cid:7)(cid:9)
$(cid:11)(cid:10)(cid:7)(cid:8)(cid:11)(cid:4)
&--(cid:8)(cid:2)(cid:18)(cid:18)
(cid:26)(cid:9)(cid:7)(cid:9)
$(cid:11)(cid:10)(cid:7)(cid:8)(cid:11)(cid:4)
"
(cid:8)
(cid:11)
(cid:12)
(cid:2)
/
(cid:14)
(cid:29)
(cid:7)
’
(cid:30)
&
&(cid:15)!
/&$1&$$
(cid:1)(cid:8)(cid:11)(cid:3)(cid:8)(cid:9)(cid:12)
$(cid:11)(cid:10)(cid:7)(cid:8)(cid:11)(cid:4)(cid:4)(cid:2)(cid:8)
(cid:4)
(cid:18)
(cid:9)
(cid:8)
(cid:2)
(cid:21)
(cid:28)
(cid:16)
(cid:8)
(cid:2)
(cid:1)
(cid:14)
(cid:29)
(cid:7)
’
(cid:30)
(cid:6)(cid:2)(cid:8)(cid:16)(cid:9)(cid:4)
(cid:27)(cid:10)(cid:7)(cid:2)(cid:8)%(cid:9)(cid:20)(cid:2)(cid:18)
$(cid:11)(cid:12)(cid:12)(cid:13)(cid:10)(cid:16)(cid:20)(cid:9)(cid:7)(cid:16)(cid:11)(cid:10)
(cid:1)(cid:11)(cid:8)(cid:7)(cid:18)
Figure 4.2 Block diagram of a ﬂoating-point digital signal processor.
4.2 Digital Audio Interfaces
101
Table 4.3 Floating-point DSPs.
Type
Word-length
Cycle time
MHz/ns
Computation power
MFLOPS
ADSP 21364
ADSP 21267
ADSP-21161N
TI-TMS320C6711
32
32
32
32
300/3.3
150/6.6
100/10
200/5
1800
900
600
1200
4.2 Digital Audio Interfaces
For transferring digital audio signals, two transmission standards have been established
by the AES (Audio Engineering Society) and the EBU (European Broadcasting Union), re-
spectively. These standards are for two-channel transmission [AES92] and for multichannel
transmission of up to 56 audio signals.
0
$(cid:21)(cid:9)(cid:10)(cid:10)(cid:2)(cid:4)(cid:14)(cid:22)
*
$(cid:21)(cid:9)(cid:10)(cid:10)(cid:2)(cid:4)(cid:14)(cid:23)
H
$(cid:21)(cid:9)(cid:10)(cid:10)(cid:2)(cid:4)(cid:14)(cid:22)
*
$(cid:21)(cid:9)(cid:10)(cid:10)(cid:2)(cid:4)(cid:14)(cid:23)
0
$(cid:21)(cid:9)(cid:10)(cid:10)(cid:2)(cid:4)(cid:14)(cid:22)
*
$(cid:21)(cid:9)(cid:10)(cid:10)(cid:2)(cid:4)(cid:14)(cid:23)
0
(cid:6)(cid:13).%(cid:8)(cid:9)(cid:12)(cid:2)(cid:14)(cid:22)
(cid:6)(cid:13).%(cid:8)(cid:9)(cid:12)(cid:2)(cid:14)(cid:23)
@(cid:8)(cid:9)(cid:12)(cid:2)(cid:14)(cid:22)I(cid:22)
@(cid:8)(cid:9)(cid:12)(cid:2)(cid:14)3
@(cid:8)(cid:9)(cid:12)(cid:2)(cid:14)(cid:22)
(cid:6)(cid:7)(cid:9)(cid:8)(cid:7)(cid:14)(cid:11)%(cid:14).(cid:4)(cid:11)(cid:20)(cid:17)
Figure 4.3 Two-channel format.
3
(cid:22) (cid:23) 2 <
(cid:1)(cid:8)(cid:2)(cid:9)(cid:12).(cid:4)(cid:2)
(cid:15)(cid:6),
(cid:23)<(cid:14).(cid:16)(cid:7)(cid:14)(cid:9)(cid:13)-(cid:16)(cid:11)(cid:14)(cid:18)(cid:9)(cid:12)(cid:28)(cid:4)(cid:2)(cid:14);(cid:11)(cid:8)-
/(cid:6),
L ! $ (cid:1)
(cid:23)4
(cid:23)? (cid:23)I 23 2(cid:22)
Figure 4.4 Two-channel format (subframe).
4.2.1 Two-channel AES/EBU Interface
For the two-channel AES/EBU interface, professional and consumer modes are deﬁned.
The outer frame is identical for both modes and is shown in Fig. 4.3. For a sampling period
a frame is deﬁned so that it consists of two subframes, for channel 1 with preamble X, and
for channel 2 with preamble Y. A total of 192 frames form a block, and the block start is
characterized by a special preamble Z. The bit allocation of a subframe consists of 32 bits
as in Fig. 4.4. The preamble consists of 4 bits (bit 0, . . . , 3) and the audio data of up to
24 bits (bit 4, . . . , 27). The last four bits of the subframe characterize Validity (validity
of data word or error), User Status (usable bit), Channel Status (from 192 bits/block = 24
bytes coded status information for the channel) and Parity (even parity). The transmission
102
Audio Processing Systems
of the serial data bits is carried out with a biphase code. This is done with the help of an
XOR relationship between clock (of double bit rate) and the serial data bits (Fig. 4.5).
At the receiver, clock retrieval is achieved by detecting the preamble (X = 11100010,
Y = 11100100, Z = 11101000) as it violates the coding rule (see Fig. 4.6). The meaning
of the 24 bytes for channel status information is summarized in Table 4.4. An exact bit
allocation of the ﬁrst three important bytes of this channel status information is presented
in Fig. 4.7. In the individual ﬁelds of byte 0, preemphasis and sampling rate are speciﬁed
besides professional/consumer modes and the characterization of data/audio (see Tables 4.5
and 4.6). Byte 1 determines the channel mode (Table 4.7). The consumer format (often la-
beled SPDIF = Sony/Philips Digital Interface Format) differs from the professional format
in the deﬁnition of the channel status information and the technical speciﬁcations for inputs
and outputs. The bit allocation for the ﬁrst four bits of the channel information is shown
in Fig. 4.8. For consumer applications, two-wired leads with RCA connectors are used.
The inputs and outputs are asymmetrical. Also, optical connectors exist. For professional
use, shielded two-wired leads with XLR connectors and symmetrical inputs and outputs
(professional format) are used. Table 4.8 shows the electrical speciﬁcations for professional
AES/EBU interfaces.
(cid:22)
3
(cid:22)
3
Figure 4.5 Channel coding.
(cid:22)
(cid:22)
(cid:22)
3
3
3
(cid:22)
3
$(cid:4)(cid:11)(cid:20)(cid:17)7(cid:23)’(cid:14).(cid:16)(cid:7)(cid:14)(cid:8)(cid:9)(cid:7)(cid:2):
(cid:6)(cid:11)(cid:13)(cid:8)(cid:20)(cid:2)(cid:14)(cid:20)(cid:11)-(cid:16)(cid:10)(cid:3)
$(cid:21)(cid:9)(cid:10)(cid:10)(cid:2)(cid:4)(cid:14)(cid:20)(cid:11)-(cid:16)(cid:10)(cid:3)
7.(cid:16)(cid:28)(cid:21)(cid:9)(cid:18)(cid:2)(cid:14)(cid:12)(cid:9)(cid:8)(cid:17):
$(cid:4)(cid:11)(cid:20)(cid:17)7(cid:23)’(cid:14).(cid:16)(cid:7)(cid:14)(cid:8)(cid:9)(cid:7)(cid:2):
$(cid:21)(cid:9)(cid:10)(cid:10)(cid:2)(cid:4)(cid:14)(cid:20)(cid:11)-(cid:16)(cid:10)(cid:3)
7.(cid:16)(cid:28)(cid:21)(cid:9)(cid:18)(cid:2)(cid:14)(cid:12)(cid:9)(cid:8)(cid:17):
(cid:1)(cid:9)(cid:8)(cid:16)(cid:7)"
(cid:15)(cid:6),
(cid:15)(cid:9)(cid:20)(cid:17)(cid:14)(cid:11)%(cid:14)(cid:7)(cid:8)(cid:9)(cid:10)(cid:18)(cid:16)(cid:7)(cid:16)(cid:11)(cid:10)(cid:14)(cid:9)(cid:7)(cid:14).(cid:16)(cid:7)(cid:14).(cid:11)(cid:13)(cid:10)-(cid:9)(cid:8)"
Figure 4.6 Preamble X.
4.2.2 MADI Interface
For connecting an audio processing system at different locations, a MADI interface
(Multichannel Audio Digital Interface) is used. A system link by MADI is presented in
Fig. 4.9. Analog/digital I/O systems consisting of AD/DA converters, AES/EBU interfaces
(AES) and sampling rate converters (SRC) are connected to digital distribution systems
with bi-directional MADI links. The actual audio signal processing is performed in special
DSP systems which are connected to the digital distribution systems by MADI links. The
4.2 Digital Audio Interfaces
103
Table 4.4 Channel status bytes.
Byte
Description
0
1
2
3
4
5
6–9
10–13
14–17
18–21
22
23
Emphasis, sampling rate
Channel use
Sample length
Vector for byte 1
Reference bits
Reserved
4 bytes of ASCII origin
4 bytes of ASCII destination
4 bytes of local address
Time code
Flags
CRC
,"(cid:7)(cid:2)(cid:14)3
3
(cid:22)
(cid:23)
(cid:30)(cid:12)(cid:28)(cid:21)(cid:9)(cid:18)(cid:16)(cid:18)
2
(cid:6)(cid:9)(cid:12)(cid:28)(cid:4)(cid:16)(cid:10)(cid:3)(cid:14)(cid:8)(cid:9)(cid:7)(cid:2)
<
5
>
4
(cid:1)(cid:8)(cid:11)%(cid:2)(cid:18)(cid:18)(cid:16)(cid:11)(cid:10)(cid:9)(cid:4)1
$(cid:11)(cid:10)(cid:18)(cid:13)(cid:12)(cid:2)(cid:8)
(cid:26)(cid:9)(cid:7)(cid:9)1
&(cid:13)-(cid:16)(cid:11)
!(cid:10)(cid:4)(cid:11)(cid:20)(cid:17)(cid:2)-1(cid:4)(cid:11)(cid:20)(cid:17)(cid:2)-
,"(cid:7)(cid:2)(cid:14)(cid:22)
$(cid:21)(cid:9)(cid:10)(cid:10)(cid:2)(cid:4)(cid:14)(cid:12)(cid:11)-(cid:2)
3
(cid:22)
(cid:23)
2
<
(cid:24)(cid:11)(cid:7)(cid:14)(cid:13)(cid:18)(cid:2)-
5
>
4
,"(cid:7)(cid:2)(cid:14)(cid:23)
(cid:6)(cid:9)(cid:12)(cid:28)(cid:4)(cid:2)(cid:14)(cid:4)(cid:2)(cid:10)(cid:3)(cid:7)(cid:21)
3
(cid:22)
(cid:23)
(cid:30)(cid:10)(cid:20)(cid:11)-(cid:2)-(cid:14)(cid:4)(cid:2)(cid:10)(cid:3)(cid:7)(cid:21)
2
<
5
3
>
4
Figure 4.7 Bytes 0–2 of channel status information.
Table 4.5 Emphasis ﬁeld.
0
None indicated, override enabled
4
None indicated, override disabled
50/15 µs emphasis
6
7 CCITT J.17 emphasis
Table 4.6 Sampling rate ﬁeld.
0 None indicated (48 kHz default)
1
2
3
48 kHz
44.1 kHz
32 kHz
104
Audio Processing Systems
Table 4.7 Channel mode.
Two channel
0 None indicated (2 channel default)
1
2 Monaural
3
4
7 Vector to byte 3
Primary/secondary (A = primary, B = secondary)
Stereo (A = left, B = right)
,"(cid:7)(cid:2)(cid:14)3
/(cid:11)-(cid:2)
3
(cid:22)
(cid:23)
2
<
5
>
4
$(cid:11)(cid:10)(cid:18)(cid:13)(cid:12)(cid:2)(cid:8)(cid:14)7M3:
(cid:26)(cid:9)(cid:7)(cid:9)1
&(cid:13)-(cid:16)(cid:11)
,"(cid:7)(cid:2)(cid:14)(cid:22)
(cid:20)(cid:9)(cid:7)(cid:2)(cid:3)(cid:11)(cid:8)"
(cid:22)
(cid:23)
3
3(cid:14)(cid:14)(cid:14)G(cid:2)(cid:10)(cid:2)(cid:8)(cid:9)(cid:4)(cid:14)(cid:28)(cid:13)(cid:8)(cid:28)(cid:11)(cid:18)(cid:2)
(cid:23)(cid:14)(cid:14)(cid:14)(cid:1)$/
2(cid:14)(cid:14)(cid:14)&(cid:26)$
<(cid:14)(cid:14)(cid:14)$(cid:26)
>(cid:14)(cid:14)(cid:14)(cid:26)&+
)(cid:13)(cid:9)-1(cid:6)(cid:7)(cid:2)(cid:8)(cid:2)(cid:11)
(cid:24)(cid:11)(cid:7)(cid:14)(cid:13)(cid:18)(cid:2)-
(cid:1)(cid:8)(cid:2)(cid:2)(cid:12)(cid:28)(cid:21)(cid:9)(cid:18)(cid:16)(cid:18)1(cid:24)(cid:11)(cid:10)(cid:2)
$(cid:11)(cid:28)"(cid:14)(cid:1)(cid:2)(cid:8)(cid:12)(cid:16)(cid:7)(cid:7)(cid:2)-1$(cid:11)(cid:28)"(cid:8)(cid:16)(cid:3)(cid:21)(cid:7)
3
2
<
5
>
4
#(cid:2)(cid:10)(cid:2)(cid:8)(cid:9)(cid:7)(cid:16)(cid:11)(cid:10)
,"(cid:7)(cid:2)(cid:14)(cid:23)
(cid:18)(cid:11)(cid:13)(cid:8)(cid:20)(cid:2)(cid:14)(cid:10)(cid:13)(cid:12).(cid:2)(cid:8)
(cid:20)(cid:21)(cid:9)(cid:10)(cid:10)(cid:2)(cid:4)(cid:14)(cid:10)(cid:13)(cid:12).(cid:2)(cid:8)
3
(cid:22)
(cid:23)
2
<
5
>
4
,"(cid:7)(cid:2)(cid:14)2
(cid:6)(cid:9)(cid:12)(cid:28)(cid:4)(cid:16)(cid:10)(cid:3)
(cid:8)(cid:9)(cid:7)(cid:2)
3
(cid:22)
3
&(cid:20)(cid:20)(cid:13)(cid:8)(cid:9)(cid:20)"
(cid:23)
2
<
5
(cid:19)(cid:2)(cid:18)(cid:2)(cid:8)A(cid:2)-
>
4
3(cid:14)(cid:14)(cid:14)<<(cid:29)(cid:22)(cid:14)(cid:17)=
(cid:22)(cid:14)(cid:14)(cid:14)<?(cid:14)(cid:17)=
(cid:23)(cid:14)(cid:14)(cid:14)(cid:19)(cid:2)(cid:18)(cid:2)(cid:8)A(cid:2)-
2(cid:14)(cid:14)(cid:14)2(cid:23)(cid:14)(cid:17)=
3(cid:14)(cid:14)(cid:24)(cid:11)(cid:8)(cid:12)(cid:9)(cid:4)
(cid:22)(cid:14)(cid:14)L(cid:9)(cid:8)(cid:16)(cid:9).(cid:4)(cid:2)(cid:14)(cid:18)(cid:28)(cid:2)(cid:2)-
(cid:23)(cid:14)(cid:14)=(cid:16)(cid:3)(cid:21)(cid:14)(cid:9)(cid:20)(cid:20)(cid:13)(cid:8)(cid:9)(cid:20)"
2(cid:14)(cid:14)(cid:19)(cid:2)(cid:18)(cid:2)(cid:8)A(cid:2)-
Figure 4.8 Bytes 0–3 (consumer format).
Table 4.8 Electrical speciﬁcations of professional interfaces.
Output impedance
Signal amplitude
Jitter
110 (cid:4)
2–7 V
max. 20 ns
Input impedance
Signal amplitude
Connect.
110 (cid:4)
min. 200 mV
XLR
MADI format is derived from the two-channel AES/EBU format and allows the transmis-
sion of 56 digital mono channels (see Fig. 4.10) within a sampling period. The MADI
4.2 Digital Audio Interfaces
105
frame consists of 56 AES/EBU subframes. Each channel has a preamble containing the
information shown in Fig. 4.10. The bit 0 is responsible for identifying the ﬁrst MADI
channel (MADI Channel 0). Table 4.9 shows the sampling rates and the corresponding
data transfer rates. The maximum data rate of 96.768 Mbit/s is required at sampling rate of
48 kHz + 12.5%. Data transmission is done by FDDI techniques (Fiber Distributed Digital
Interface). The transmission rate of 125 Mbit/s is implemented with special TAXI chips.
The transmission for a coaxial cable is already speciﬁed (see Table 4.10). The optical
transmission medium for audio applications is not yet deﬁned.
&(cid:26)$
(cid:26)&$
&(cid:30)(cid:6)
(cid:6)(cid:19)$
&(cid:26)$
(cid:26)&$
&(cid:30)(cid:6)
(cid:6)(cid:19)$
&(cid:10)(cid:9)(cid:4)(cid:11)(cid:3)
(cid:26)(cid:16)(cid:3)(cid:16)(cid:7)(cid:9)(cid:4)
(cid:27)1((cid:14)(cid:6)"(cid:18)(cid:7)(cid:2)(cid:12)
/&(cid:26)(cid:27)
(cid:26)(cid:16)(cid:3)(cid:16)(cid:7)(cid:9)(cid:4)
(cid:26)(cid:16)(cid:18)(cid:7)(cid:8)(cid:16).(cid:13)(cid:7)(cid:16)(cid:11)(cid:10)
(cid:6)"(cid:18)(cid:7)(cid:2)(cid:12)
/&(cid:26)(cid:27)
&(cid:13)-(cid:16)(cid:11)
(cid:6)(cid:16)(cid:3)(cid:10)(cid:9)(cid:4)
(cid:1)(cid:8)(cid:11)(cid:20)(cid:2)(cid:18)(cid:18)(cid:16)(cid:10)(cid:3)
(cid:27)
(cid:26)
&
/
&(cid:10)(cid:9)(cid:4)(cid:11)(cid:3)
(cid:26)(cid:16)(cid:3)(cid:16)(cid:7)(cid:9)(cid:4)
(cid:27)1((cid:14)(cid:6)"(cid:18)(cid:7)(cid:2)(cid:12)
/&(cid:26)(cid:27)
(cid:26)(cid:16)(cid:3)(cid:16)(cid:7)(cid:9)(cid:4)
(cid:26)(cid:16)(cid:18)(cid:7)(cid:8)(cid:16).(cid:13)(cid:7)(cid:16)(cid:11)(cid:10)
(cid:6)"(cid:18)(cid:7)(cid:2)(cid:12)
/&(cid:26)(cid:27)
&(cid:13)-(cid:16)(cid:11)
(cid:6)(cid:16)(cid:3)(cid:10)(cid:9)(cid:4)
(cid:1)(cid:8)(cid:11)(cid:20)(cid:2)(cid:18)(cid:18)(cid:16)(cid:10)(cid:3)
(cid:27)
(cid:26)
&
/
(cid:19)(cid:2)%(cid:2)(cid:8)(cid:2)(cid:10)(cid:20)(cid:2)(cid:14)$(cid:4)(cid:11)(cid:20)(cid:17)
&(cid:18)"(cid:10)(cid:20)(cid:21)(cid:8)(cid:29)
/&(cid:26)(cid:27)
/&(cid:26)(cid:27)
(cid:6)(cid:9)(cid:12)(cid:28)(cid:4)(cid:16)(cid:10)(cid:3)
@(cid:8)(cid:2)(cid:31)(cid:13)(cid:2)(cid:10)(cid:20)"
$(cid:11)(cid:10)A(cid:2)(cid:8)(cid:18)(cid:16)(cid:11)(cid:10)
Figure 4.9 A system link by MADI.
Table 4.9 MADI speciﬁcations.
Sampling rate
32 kHz–48 kHz ± 12.5%
Transmission rate
Data transfer rate
Max. data transfer rate
Min. data transfer rate
125 Mbit/s
100 Mbit/s
96.768 Mbit/s (56 channels at 48 kHz + 12.5%)
50.176 Mbit/s (56 channels at 32 kHz − 12.5%)
A unidirectional MADI link is shown in Fig. 4.11. The MADI transmitter and receiver
must be synchronized by a common master clock. The transmission between FDDI chips
is performed by a transmitter with integrated clock generation and clock retrieval at the
receiver.
106
Audio Processing Systems
&(cid:30)(cid:6)1(cid:30),!(cid:14)@(cid:11)(cid:8)(cid:12)(cid:9)(cid:7)(cid:14)(cid:6)(cid:13).%(cid:8)(cid:9)(cid:12)(cid:2)(cid:14)(cid:25)
3
(cid:22) (cid:23) 2 <
(cid:1)(cid:8)(cid:2)(cid:9)(cid:12).(cid:4)(cid:2)
&(cid:13)-(cid:16)(cid:11)(cid:14)(cid:18)(cid:9)(cid:12)(cid:28)(cid:4)(cid:2)(cid:14);(cid:11)(cid:8)-
(cid:23)4
(cid:23)? (cid:23)I 23 2(cid:22)
L ! $ (cid:1)
/&(cid:26)(cid:27)(cid:14)(cid:6)*(cid:24)$
/&(cid:26)(cid:27)(cid:14)&1,
/&(cid:26)(cid:27)(cid:14)&$+(cid:27)L(cid:30)
/&(cid:26)(cid:27)(cid:14)$=&(cid:24)(cid:24)(cid:30)(cid:15)(cid:14)3
/&(cid:26)(cid:27)(cid:14)@(cid:8)(cid:9)(cid:12)(cid:2)(cid:14)(cid:1)(cid:2)(cid:8)(cid:16)(cid:11)-(cid:14)(cid:25)
(cid:20)(cid:21)(cid:9)(cid:10)(cid:10)(cid:2)(cid:4)(cid:14)3
(cid:20)(cid:21)(cid:9)(cid:10)(cid:10)(cid:2)(cid:4)(cid:14)(cid:22)
(cid:20)(cid:21)(cid:9)(cid:10)(cid:10)(cid:2)(cid:4)(cid:14)5<
(cid:20)(cid:21)(cid:9)(cid:10)(cid:10)(cid:2)(cid:4)(cid:14)55
Figure 4.10 MADI frame format.
Table 4.10 Electrical speciﬁcations (MADI).
Output impedance
Signal ampl.
Cable length
Connect.
75 (cid:4)
0.3–0.7 V
50 m (coaxial cable)
BNC
+(cid:19)&(cid:24)(cid:6)/(cid:27)++(cid:30)(cid:19)
(cid:19)(cid:30)$(cid:30)(cid:27)L(cid:30)(cid:19)
(cid:26)&+&
2(cid:23)
%(cid:18)
+0
(cid:19)
(cid:30)
@
@
!
,
(cid:14)(cid:14)(cid:14)<,15,
(cid:30)(cid:24)$((cid:26)(cid:30)(cid:19)
2(cid:23)
(cid:26)&+&
+0
(cid:19)(cid:30)&(cid:26)
$(cid:15)($N
(cid:14)(cid:14)#(cid:30)(cid:24)
$(cid:15)($N
(cid:26)&+&
(cid:26)&+&
(cid:26)&+&
(cid:19)0
$(cid:15)($N
(cid:14)(cid:6)*(cid:24)$
(cid:14)(cid:14)(cid:14)(cid:14)5,1<,
(cid:26)(cid:30)$((cid:26)(cid:30)(cid:19)
2(cid:23)
$(cid:15)($N
%(cid:18)K
(cid:19)
(cid:30)
@
@
!
,
2(cid:23)
%(cid:18)
(cid:19)0
0+&(cid:15)
0+&(cid:15)
(cid:14)(cid:6)*(cid:24)$
(cid:19)(cid:30)#(cid:30)(cid:24)
Figure 4.11 MADI link.
(cid:14)(cid:6)*(cid:24)$
(cid:19)(cid:30)#(cid:30)(cid:24)
%(cid:18)(cid:12)
/&(cid:6)+(cid:30)(cid:19)
(cid:14)(cid:14)(cid:14)(cid:6)*(cid:24)$
4.3 Single-processor Systems
107
4.3 Single-processor Systems
4.3.1 Peripherals
A common system conﬁguration is shown in Fig. 4.12. It consists of a DSP, clock gener-
ation, instruction and data memory and a BOOT-EPROM. After RESET, the program is
loaded into the internal RAM of the signal processor. The loading is done byte by byte
so that only an EPROM with 8-bit data word-length is necessary. In terms of circuit com-
plexity the connection of AD/DA converters over serial interfaces is the simplest solution.
Most ﬁxed-point signal processors support serial connection where a lead ‘connection’
for bit clock SCLK, sampling clock/word clock WCLK, and the serial input and output
data SDRX/SDTX are used. The clock signals are obtained from a higher reference clock
CLKIN (see Fig. 4.13). For non-serially operating AD/DA converters, parallel interfaces
can also be connected to the DSP.
/
(
(cid:19)
(cid:1)
(cid:30)
(cid:1)
(cid:5)
+
(
(
,
/
(
(cid:19)
(cid:1)
(cid:30)
?
/
&
(cid:19)
(cid:1)
(cid:5)
/
&
(cid:19)
(cid:26)
(cid:5)
(cid:26)&$
(cid:26)(cid:6)(cid:1)
0
(cid:19)
(cid:26)
(cid:6)
N
(cid:15)
$
6
N
(cid:15)
$
(cid:6)
0
+
(cid:26)
(cid:6)
(cid:26)&$
(cid:6)$(cid:15)N
(cid:6)(cid:26)&+&
6$(cid:15)N
(cid:22)
(cid:23)
(cid:22)
(cid:23)
&(cid:26)$
(cid:6)$(cid:15)N
(cid:6)(cid:26)&+&
6$(cid:15)N
$(cid:15)N(cid:27)(cid:24)1(cid:23)
$(cid:15)N(cid:27)(cid:24)1(cid:22)(cid:23)?
$(cid:15)N(cid:27)(cid:24)
Figure 4.12 DSP system with two-channel AD/DA converters (C = control, A = address, D =
data, SDATA = serial data, SCLK = bit clock, WCLK = word clock, SDRX = serial input, SDTX =
serial output).
4.3.2 Control
For controlling digital signal processors and data exchange with host processors, some
DSPs provide a special host interface that can be read and written directly (see Fig. 4.14).
The data word-length depends on the processor. The host interface is included in the
external address space of the host or is connected to a local bus system, for instance a
PC bus.
A DSP as a coprocessor for special signal processing problems can be used by con-
necting it with a dual-port RAM and additional interrupt logic to a host processor. This
108
Audio Processing Systems
(cid:22)
<
?
(cid:22)(cid:23)
(cid:22)>
(cid:23)3
(cid:23)<
(cid:23)?
2(cid:23)
$(cid:15)N(cid:27)(cid:24)1(cid:23)
$(cid:15)N(cid:27)(cid:24)1(cid:22)(cid:23)?
(cid:6)(cid:26)(cid:19)0
(cid:6)(cid:26)+0
(cid:22)
(cid:27)(cid:24)
(cid:22)
(!+
(cid:23)
(cid:27)(cid:24)
(cid:23)
(!+
Figure 4.13 Serial transmission format.
/
(
(cid:19)
(cid:1)
(cid:30)
(cid:1)
(cid:5)
$(cid:6)
(cid:19)16
&
(cid:26)
(cid:27)
(cid:5)
+
(cid:6)
(
=
2
?12(cid:23)1(cid:22)>
/
&
(cid:19)
(cid:26)
(cid:5)
/
&
(cid:19)
(cid:1)
(cid:5)
(cid:26)&$
(cid:26)(cid:6)(cid:1)
Figure 4.14 Control via a host interface of the DSP (CS = chip select, R/W = read/write, A =
address, D = data).
enables data transmission between the DSP system and host processor (see Fig. 4.15). This
results in a complete separation from the host processor. The communication can either be
interrupt-controlled or carried out by polling a memory address in a dual-port RAM.
(cid:7)
(cid:8)
(cid:11)
(cid:1)
(cid:5)
(cid:4)
(cid:9)
(cid:13)
(cid:26)
/
&
(cid:19)
$
&
(cid:26)
(cid:27)(cid:24)+
/
(
(cid:19)
(cid:1)
(cid:30)
(cid:1)
(cid:5)
/
&
(cid:19)
(cid:1)
(cid:5)
/
&
(cid:19)
(cid:26)
(cid:5)
(cid:26)&$
(cid:27)(cid:24)+
(cid:26)(cid:6)(cid:1)
Figure 4.15 Control over a dual-port RAM and interrupt.
A very simple control can be done directly via an RS232-interface. This is can be car-
ried out via an additional asynchronous serial interface (Serial Communication Interface)
of the DSP (see Fig. 4.16).
4.4 Multi-processor Systems
109
/
(
(cid:19)
(cid:1)
(cid:30)
(cid:1)
(cid:5)
/
&
(cid:19)
(cid:1)
(cid:5)
/
&
(cid:19)
(cid:26)
(cid:5)
(cid:19)0(cid:26)
+0(cid:26)
(cid:6)$(cid:15)N
(cid:26)&$
(cid:26)(cid:6)(cid:1)
(cid:27)
$
(cid:6)
Figure 4.16 Control over a serial interface (RS232, RS422).
(cid:6)(cid:26)&+&
(cid:23)
(cid:6)$(cid:15)N1(cid:6)*(cid:24)$
(cid:26)(cid:6)(cid:1)
(cid:26)(cid:6)(cid:1)
(cid:26)(cid:6)(cid:1)
(cid:26)(cid:6)(cid:1)
(cid:6)(cid:26)&+&
Figure 4.17 Cascading and pipelining (SDATA = serial data, SCLK = bit clock, SYNC =
synchronization).
(cid:6)(cid:26)&+&
(cid:6)$(cid:15)N1(cid:6)*(cid:24)$
(cid:23)
(cid:6)(cid:26)&+&
(cid:26)(cid:6)(cid:1)
(cid:26)(cid:6)(cid:1)
(cid:26)(cid:6)(cid:1)
(cid:26)(cid:6)(cid:1)
Figure 4.18 Parallel conﬁguration with output time-multiplex.
(cid:26)(cid:6)(cid:1)
(cid:26)(cid:6)(cid:1)
(cid:26)(cid:6)(cid:1)
(cid:26)(cid:6)(cid:1)
(cid:6)(cid:26)&+&
(cid:6)(cid:26)&+&
(cid:23)
(cid:6)$(cid:15)N1(cid:6)*(cid:24)$
&(cid:26)(cid:19)
Figure 4.19 Time-multiplex connection (ADR = address at a particular time).
4.4 Multi-processor Systems
The design of multi-processor systems can be carried out by linking signal processors by
serial or parallel interfaces. Besides purely multi-processor DSP systems, an additional
connection to standard bus systems can be made as well.
110
Audio Processing Systems
(cid:1)(cid:26)&+&
(cid:26)(cid:6)(cid:1)
(cid:26)(cid:6)(cid:1)
(cid:26)(cid:6)(cid:1)
(cid:26)(cid:6)(cid:1)
(cid:1)(cid:26)&+&
Figure 4.20 Cascading and pipelining.
(cid:6)"(cid:18)(cid:7)(cid:2)(cid:12)(cid:14),(cid:13)(cid:18)
(cid:26)(cid:6)(cid:1)
(cid:26)(cid:6)(cid:1)
(cid:26)(cid:6)(cid:1)
(cid:26)(cid:6)(cid:1)
(cid:15)(cid:11)(cid:20)(cid:9)(cid:4)(cid:14),(cid:13)(cid:18)
Figure 4.21 Parallel conﬁguration.
(cid:6)"(cid:18)(cid:7)(cid:2)(cid:12)(cid:14),(cid:13)(cid:18)
(cid:26)(cid:6)(cid:1)
(cid:26)(cid:6)(cid:1)
<(cid:1)(cid:5)
(cid:19)&/
(cid:26)(cid:6)(cid:1)
(cid:26)(cid:6)(cid:1)
Figure 4.22 Connection over a four-port RAM.
(cid:6)"(cid:18)(cid:7)(cid:2)(cid:12)(cid:14),(cid:13)(cid:18)
(cid:26)(cid:6)(cid:1)
(cid:26)(cid:6)(cid:1)
(cid:26)(cid:6)(cid:1)
(cid:26)(cid:6)(cid:1)
+(cid:26)/(cid:14),(cid:13)(cid:18)
Figure 4.23 Signal processor systems based on standard bus system.
4.4.1 Connection via Serial Links
In connecting via serial links, signal processors are cascaded so that different program
segments are distributed over different processors (see Fig. 4.17). The serial output data is
fed into the serial input of the following signal processor. A synchronous bit clock and a
common synchronization SYNC control the serial interface. With the help of a serial time-
multiplex mode (Fig. 4.18) a parallel conﬁguration can be designed which, for instance,
feeds several parallel signal processors with serial input data. The serial outputs of signal
4.4 Multi-processor Systems
111
((cid:28)(cid:2)(cid:8)(cid:9)(cid:7)(cid:11)(cid:8)
$(cid:11)(cid:10)(cid:18)(cid:11)(cid:4)(cid:2)
=(cid:11)(cid:18)(cid:7)
$(cid:11)(cid:12)(cid:28)(cid:13)(cid:7)(cid:2)(cid:8)
(cid:26)(cid:6)(cid:1)
(cid:6)"(cid:18)(cid:7)(cid:2)(cid:12)(cid:18)
(cid:18)
(cid:13)
,
(cid:14)
(cid:11)
-
(cid:13)
&
(cid:16)
&(cid:30)(cid:6)1(cid:30),!
/&(cid:26)(cid:27)
(cid:15)&(cid:24)
&(cid:26)$ (cid:26)&$
&(cid:10)(cid:9)(cid:4)(cid:11)(cid:3)
(cid:27)(cid:10)(cid:7)(cid:2)(cid:8)%(cid:9)(cid:20)(cid:2)(cid:18)
(
1
(cid:27)
(cid:14)
(cid:4)
(cid:9)
(cid:7)
(cid:16)
(cid:3)
(cid:26)
(cid:16)
(
1
(cid:27)
(cid:14)
(cid:3)
(cid:11)
(cid:9)
(cid:10)
&
(cid:4)
(cid:26)(cid:16)(cid:3)(cid:16)(cid:7)(cid:9)(cid:4)(cid:14)
(cid:6)(cid:16)(cid:3)(cid:10)(cid:9)(cid:4)
(cid:1)(cid:8)(cid:11)(cid:20)(cid:2)(cid:18)(cid:18)(cid:16)(cid:10)(cid:3)
(cid:27)(cid:10)(cid:7)(cid:2)(cid:8)%(cid:9)(cid:20)(cid:2)(cid:18)
&(cid:10)(cid:9)(cid:4)(cid:11)(cid:3)
(cid:1)(cid:8)(cid:11)(cid:20)(cid:2)(cid:18)(cid:18)(cid:16)(cid:10)(cid:3)
Figure 4.24 Audio system.
processors provide output data in time-multiplex. A complete time-multiplex connection
via the serial interface of the signal processor is shown in Fig. 4.19. The allocation of a
signal processor at a particular time slot can either be ﬁxed or carried out by an address
control ADR.
4.4.2 Connection via Parallel Links
Connection via parallel links is possible with dual-port processors as well as with dual-port
RAMs (see Fig. 4.20). A parallel conﬁguration of signal processor systems with a local bus
is shown in Fig. 4.21. The connection to the local bus is done either over a dual-port RAM
or directly with a second signal processor port. Another possible conﬁguration is the use
of a four-port RAM as shown in Fig. 4.22. Here, one processor serves as a connector to a
112
Audio Processing Systems
(cid:6)(cid:13).(cid:18)"(cid:18)(cid:7)(cid:2)(cid:12)(cid:14)(cid:22)
(cid:15)&(cid:24)
$(cid:11)(cid:10)(cid:7)(cid:8)(cid:11)(cid:4)(cid:14)
$(cid:11)(cid:12)(cid:28)(cid:13)(cid:7)(cid:2)(cid:8)
(cid:26)(cid:6)(cid:1)
/(cid:11)-(cid:13)(cid:4)(cid:2)
(cid:22)
&(cid:13)-(cid:16)(cid:11)(cid:14),(cid:13)(cid:18)
(cid:6)"(cid:18)(cid:7)(cid:2)(cid:12)(cid:14),(cid:13)(cid:18)
(cid:26)(cid:6)(cid:1)
/(cid:11)-(cid:13)(cid:4)(cid:2)
(cid:10)
&(cid:30)(cid:6)1(cid:30),!
/&(cid:26)(cid:27)
=(cid:11)(cid:18)(cid:7)
$(cid:11)(cid:12)(cid:28)(cid:13)(cid:7)(cid:2)(cid:8)
$(cid:11)(cid:10)(cid:7)(cid:8)(cid:11)(cid:4)
$(cid:11)(cid:10)(cid:18)(cid:11)(cid:4)(cid:2)
(cid:6)(cid:13).(cid:18)"(cid:18)(cid:7)(cid:2)(cid:12)(cid:14)(cid:12)
$(cid:11)(cid:10)(cid:7)(cid:8)(cid:11)(cid:4)(cid:14)
$(cid:11)(cid:12)(cid:28)(cid:13)(cid:7)(cid:2)(cid:8)
(cid:26)(cid:6)(cid:1)
/(cid:11)-(cid:13)(cid:4)(cid:2)
(cid:22)
&(cid:13)-(cid:16)(cid:11)(cid:14),(cid:13)(cid:18)
(cid:6)"(cid:18)(cid:7)(cid:2)(cid:12)(cid:14),(cid:13)(cid:18)
(cid:26)(cid:6)(cid:1)
/(cid:11)-(cid:13)(cid:4)(cid:2)
(cid:10)
&(cid:30)(cid:6)1(cid:30),!
/&(cid:26)(cid:27)
Figure 4.25 Scalable digital audio system.
&(cid:13)-(cid:16)(cid:11)(cid:14),(cid:13)(cid:18)
(cid:26)(cid:6)(cid:1)
!(cid:10)(cid:16)(cid:7)(cid:18)
(cid:26)(cid:16)(cid:3)(cid:16)(cid:7)(cid:9)(cid:4)(cid:14)(cid:14)(cid:27)1(
&(cid:10)(cid:9)(cid:4)(cid:11)(cid:3)(cid:14)(cid:27)1(
(cid:6)(cid:9)(cid:12)(cid:28)(cid:4)(cid:16)(cid:10)(cid:3)(cid:14)(cid:19)(cid:9)(cid:7)(cid:2)
$(cid:11)(cid:10)A(cid:2)(cid:8)(cid:18)(cid:16)(cid:11)(cid:10)
(cid:6)"(cid:10)(cid:20)(cid:21)(cid:8)(cid:11)(cid:10)(cid:16) (cid:9)(cid:7)(cid:16)(cid:11)(cid:10)
&(cid:26)$
(cid:26)&$
/&(cid:26)(cid:27)
/&(cid:26)(cid:27)
&(cid:30)(cid:6)1(cid:30),!
&(cid:30)(cid:6)1(cid:30),!
&(cid:30)(cid:6)1(cid:30),!
(cid:6)"(cid:18)(cid:7)(cid:2)(cid:12)(cid:14),(cid:13)(cid:18)
Figure 4.26 Subsystem.
system bus and feeds three other processors over a four-port RAM with control and data
information.
4.4.3 Connection via Standard Bus Systems
The use of standard bus systems (VME bus, MULTIBUS, PC bus) to control multi-processor
systems is presented in Fig. 4.23. The connection of signal processors can either be carried
out directly over a control bus or with the help of a special data bus. This parallel data bus
can operate in time-multiplex. Hence, control information and data are separated. A few of
References
113
the criteria for standard bus systems are data transfer rate, interrupt request and processing,
the option of several masters, auxiliary functions (power supply, bus error, battery buffer)
and mechanical requirements.
4.4.4 Scalable Audio System
The functional segmentation of an audio system into different stages, the analog, interface,
digital and man–machine stages, is shown in Fig. 4.24. All stages are controlled by a
LAN (Local Area Network). In the analog domain, crosspoint switches and microphone
ampliﬁers are controlled. In the interface domain AD/DA converters and sampling rate
converters are used. The connection to a signal processing system is done by AES/EBU and
MADI interfaces. A host computer with a control console for the sound engineer serves as
the central control unit.
The realization of the digital domain with the help of a standard bus system is shown
in Fig. 4.25. A central mixing console controls several subsystems over a host. These sub-
systems have special control computers which control several DSP modules. The system
concept is scalable within a subsystem and by extension to several subsystems. Audio
data transfer between subsystems is performed by AES/EBU and MADI interfaces. The
segmentation within a subsystem is shown in Fig. 4.26. Here, besides DSP modules, digital
interfaces (AES/EBU, MADI, sampling rate converters, etc.) and AD/DA converters can
be integrated.
References
[AES91] AES10-1991 (ANSI S4.43-1991): AES Recommended Practice for Digital
Audio Engineering – Serial Multichannel Audio Digital Interface (MADI).
[AES92] AES3-1992 (ANSI S4.40-1992): AES Recommended Practice for Digital
Audio Engineering – Serial Transmission Format for Two-Channel Linearly
Represented Digital Audio.
