# Digital Audio DSP (Zölzer) — Chunk 16/30

> Source: `Udo-Zolzer-digital-audio-signal-processing.9780470997857.40435.pdf` · Pages ~1-331 · Extractor: pdfminer.six
> Target ≈ 2800 words, Overlap = 200 words · Actual = 1597

> Note: DSP textbook content for SpectralCanvasPro RT-audio knowledge base

---
149
With regard to the implementation of the these ﬁlters with digital signal processors, a
quantization after every multiplication is not necessary. Quantization takes place when the
accumulator has to be stored in memory. This can be seen in Figs 5.42–5.45 by introducing
quantizers where they really occur. The resulting output noise variances are also shown.
The signal-to-noise ratio is plotted versus the cutoff frequency in Figs 5.46 and 5.47. In
the case of direct-form and Gold and Rader ﬁlters, the signal-to-noise ratio increases by
3 dB whereas the output noise variance for the Kingsbury ﬁlter remains unchanged. The
Kingsbury ﬁlter and the Gold and Rader ﬁlters exhibit similar results up to a frequency of
200 kHz (see Fig. 5.46). The Zölzer ﬁlter demonstrates an improvement of 3 dB compared
with these structures. For frequencies of up to 2 kHz (see Fig. 5.47) it is seen that the
increased pole density leads to an improvement of the signal-to-noise ratio as well as a
reduced effect due to coefﬁcient quantization.
’7(cid:10):
"7(cid:10):
)
(cid:5).
(cid:22)
(cid:5).
(cid:23)
  (cid:5)(cid:22)
  (cid:5)(cid:22)
σ 2
ye
= σ 2
E
1 + b2
1 − b2
1
(1 + b2)2 − b2
1
Figure 5.42 Direct-form ﬁlter – quantization after accumulator.
’7(cid:10):
)
  (cid:5)(cid:22)
)
  (cid:5)(cid:22)
"7(cid:10):
(cid:8)(cid:14)(cid:18)(cid:16)(cid:10) (cid:15)
(cid:8)(cid:14)(cid:20)(cid:11)(cid:18) (cid:15)
(cid:8)(cid:14)(cid:20)(cid:11)(cid:18) (cid:15)
(cid:5)(cid:8)(cid:14)(cid:18)(cid:16)(cid:10)
(cid:15)
σ 2
ye
= σ 2
E
1
1 − b2
Figure 5.43 Gold and Rader ﬁlter – quantization after accumulator.
150
’7(cid:10):
)
  (cid:5)(cid:22)
(cid:17)
(cid:22)
)
(cid:17)
(cid:22)
(cid:17)
(cid:23)
Equalizers
)
  (cid:5)(cid:22)
"7(cid:10):
σ 2
ye
= σ 2
E
5 + 2b1 + 3b2
(1 − b2)(1 + b2 − b1)
Figure 5.44 Kingsbury ﬁlter – quantization after accumulator.
’7(cid:10):
)
  (cid:5)(cid:22)
(cid:22)
(cid:22)
(cid:22)
)
(cid:23)
)
  (cid:5)(cid:22)
"7(cid:10):
σ 2
ye
= σ 2
E
2(2 + b1 + b2) + (1 + b2)(1 + b1 + b2)1/3
(1 − b2)(1 + b2 − b1)
Figure 5.45 Zölzer ﬁlter – quantization after accumulator.
Zölzer
Gold and Rader
Kingsbury
Direct-form
20
40
60
80
100
120
140
160
180
200
f/Hz
SNR/dB
100
80
60
40
20
0
Figure 5.46 SNR vs. cutoff frequency – quantization after accumulator (fc < 200 Hz).
Noise Shaping in Recursive Filters
The analysis of the noise transfer function of different structures shows that for three
structures with low roundoff noise a zero at z = 1 occurs in the transfer functions G(z)

5.2 Recursive Audio Filters

151
SNR/dB
100
Zölzer
Gold and Rader
Kingsbury
90
80
70
60
50
Direct-form
2k
4k
6k
8k
10k
12k
f/Hz
Figure 5.47 SNR vs. cutoff frequency – quantization after accumulator (fc > 2 kHz).
of the error signals in addition to the complex poles. This zero near the poles reduces
the amplifying effect of the pole. If it is now possible to introduce another zero into the
noise transfer function then the effect of the poles is compensated for to a larger extent.
The procedure of feeding back the quantization error as shown in Chapter 2 produces an
additional zero in the noise transfer function [Tra77, Cha78, Abu79, Bar82, Zöl89]. The
feedback of the quantization error is ﬁrst demonstrated with the help of the direct-form
structure as shown in Fig. 5.48. This generates a zero at z = 1 in the noise transfer function
given by
G1.O(z) =
1 − z−1
1 + b1z−1 + b2z−2 .
(5.113)
(5.114)
The resulting variance σ 2 of the quantization error at the output of the ﬁlter is presented
in Fig. 5.48. In order to produce two zeros at z = 1, the quantization error is fed back
over two delays weighted 2 and −1 (see Fig. 5.48b). The noise transfer function is, hence,
given by
G2.O(z) = 1 − 2z−1 + z−2
1 + b1z−1 + b2z−2 .
The signal-to-noise ratio of the direct-form is plotted versus the cutoff frequency in
Fig. 5.49. Even a single zero signiﬁcantly improves the signal-to-noise ratio in the direct
form. The coefﬁcients b1 and b2 approach −2 and 1 respectively with the decrease of
the cutoff frequency. With this, the error is ﬁltered with a second-order high-pass. The
introduction of the additional zeros in the noise transfer function only affects the noise
signal of the ﬁlter. The input signal is only affected by the transfer function H (z). If the
feedback coefﬁcients are chosen equal to the coefﬁcients b1 and b2 in the denominator
polynomial, complex zeros are produced that are identical to the complex poles. The noise
transfer function G(z) is then reduced to unity. The choice of complex zeros directly at the
location of the complex poles corresponds to double-precision arithmetic.
In [Abu79] an improvement of noise behavior for the direct form in any desired location
of the z-plane is achieved by placing additional simple-to-implement complex zeros near
152
Equalizers
y(n)
x(n)
_
_
x(n)
z-1
Q
-b1
-b2
a)
z-1
z-1
z-1
z-1
Q
z-1
z-1
-1
2
-b1
-b2
b)
y(n)
σ 2
DF 1
= σ 2
E
σ 2
DF 2
= σ 2
E
2
(1 − b2)(1 + b2 − b1)
6 + 2b1 − 2b2
(1 − b2)(1 + b2 − b1)
Figure 5.48 Direct form with noise shaping.
Direct form
(douple zero at z=1)
Direct form
(single zero at z=1)
20
40
60
80
100
120
140
160
180
200
f/Hz
SNR/dB
100
90
80
70
60
50
Figure 5.49 SNR – Noise shaping in direct-form ﬁlter structures.
the poles. For implementing ﬁlter algorithms with digital signal processors, these kinds of
suboptimal zero are easily realized. Since the Gold and Rader, Kingsbury, and Zölzer ﬁlter
5.2 Recursive Audio Filters
153
structures already have zeros in their respective noise transfer functions, it is sufﬁcient to
use a simple feedback for the quantization error. By virtue of this extension, the block
diagrams in Figs 5.50, 5.51 and 5.52 are obtained.
  (cid:5)(cid:22)
  (cid:5)(cid:22)
(cid:8)(cid:14)(cid:18)(cid:16)(cid:10) (cid:15)
’7(cid:10):
)
  (cid:5)(cid:22)
)
  (cid:5)(cid:22)
"7(cid:10):
(cid:8)(cid:14)(cid:20)(cid:11)(cid:18) (cid:15)
(cid:8)(cid:14)(cid:20)(cid:11)(cid:18) (cid:15)
(cid:5)(cid:8)(cid:14)(cid:18)(cid:16)(cid:10)
(cid:15)
σ 2
ye
= σ 2
E
2 + b1
1 − b2
Figure 5.50 Gold and Rader ﬁlter with noise shaping.
  (cid:5)(cid:22)
)
  (cid:5)(cid:22)
  (cid:5)(cid:22)
)
  (cid:5)(cid:22)
"7(cid:10):
(cid:17)
(cid:22)
  (cid:5)(cid:22)
)
(cid:17)
(cid:23)
’7(cid:10):
(cid:17)
(cid:22)
σ 2
ye
= σ 2
E
(1 + k2
1)((1 + b2)(6 − 2b2) + 2b2
1
+ 8b1) + 2k2
(1 − b2)(1 + b2 − b1)(1 + b2 − b1)
1(1 + b1 + b2)
Figure 5.51 Kingsbury ﬁlter with noise shaping.
The effect of noise shaping on signal-to-noise ratio is shown in Figs 5.53 and 5.54. The
almost ideal noise behavior of all ﬁlter structures for 16-bit quantization and very small
cutoff frequencies can be observed. The effect of this noise shaping for increasing cutoff
frequencies is shown in Fig. 5.54. The compensating effect of the two zeros at z = 1 is
reduced.
154
’7(cid:10):
  (cid:5)(cid:22)
)
  (cid:5)(cid:22)
(cid:22)
Equalizers
  (cid:5)(cid:22)
  (cid:5)(cid:22)
)
  (cid:5)(cid:22)
"7(cid:10):
(cid:22)
(cid:22)
)
(cid:23)
σ 2
ye
= σ 2
E
SNR/dB
100
(1 + z2
1)((1 + b2)(6 − 2b2) + 2b2
1
+ 8b1) + 2z4
(1 − b2)(1 + b2 − b1)(1 + b2 − b1)
1(1 + b1 + b2)
Figure 5.52 Zölzer ﬁlter with noise shaping.
- Direct form (double zero at z=1)
- Gold and Rader
- Kingsbury
- Zölzer
90
80
70
60
50
20
40
60
80
100
120
140
160
180
200
f/Hz
Figure 5.53 SNR – noise shaping (20–200 Hz).
Scaling
In a ﬁxed-point implementation of a digital ﬁlter, a transfer function from the input of the
ﬁlter to a junction within the ﬁlter has to be determined, as well as the transfer function
from the input to the output. By scaling the input signal, it has to be guaranteed that the
signals remain within the number range at each junction and at the output.
In order to calculate scaling coefﬁcients, different criteria can be used. The Lp norm is
deﬁned as
Lp = (cid:11)H (cid:11)p =
(cid:4)
(cid:3)
π
−π
1
2π
|H (ej (cid:4))|p d(cid:4)
(cid:5)
1/p
,
and an expression for the L∞ norm follows for p = ∞:
L∞ = (cid:11)H (ej (cid:4))(cid:11)∞ = max
0≤(cid:4)≤π
|H (ej (cid:4))|.
(5.115)
(5.116)

5.2 Recursive Audio Filters
