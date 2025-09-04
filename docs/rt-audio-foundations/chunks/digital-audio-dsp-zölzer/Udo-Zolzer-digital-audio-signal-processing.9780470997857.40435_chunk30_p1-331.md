# Digital Audio DSP (Zölzer) — Chunk 30/30

> Source: `Udo-Zolzer-digital-audio-signal-processing.9780470997857.40435.pdf` · Pages ~1-331 · Extractor: pdfminer.six
> Target ≈ 2800 words, Overlap = 200 words · Actual = 5525

> Note: DSP textbook content for SpectralCanvasPro RT-audio knowledge base

---
301
a) Signal x(n)
b) X(k)=DCT[x(n)]
1
0.5
0
(cid:358)0.5
(cid:358)1
0
35
30
25
20
15
10
5
0
0
1
0.5
0
(cid:358)0.5
(cid:358)1
0
1
0.5
0
(cid:358)0.5
(cid:358)1
0
200
400
600
800
1000
n (cid:111)
c) Temporal envelope |h(n)|
200
400
600
800
1000
n (cid:111)
e) y(n)=IDCT[Y(k)]
200
400
600
800
1000
n (cid:111)
g) Difference signal x(n)(cid:358)y(n)
200
400
600
800
1000
n (cid:111)
1
0.5
0
(cid:358)0.5
(cid:358)1
0
1
0.5
0
(cid:358)0.5
(cid:358)1
0
1
0.5
0
(cid:358)0.5
(cid:358)1
0
1
0.5
0
(cid:358)0.5
(cid:358)1
0
200
400
600
800
1000
k (cid:111)
d) D(k)=X(k)(cid:13) A(k)
 = 16 dB
G
P
200
400
600
800
1000
k (cid:111)
f) D
(k)=[D(k)/D
Q
]
Q
max
(cid:152) D
max
200
400
600
800
1000
k (cid:111)
h) Y(k)=D
Q
(k) (cid:13) H(k)
200
400
600
800
1000
k (cid:111)
Figure 9.28 Temporal noise shaping: attack of castanet and spectrum.
302
Audio Coding
Table 9.2 Forward prediction in time and frequency domain.
Prediction in time domain
y(n) = x(n) + e(n) ∗ h(n)
Y (k) = X(k) + E(k) · H (k)
Prediction in frequency domain
y(n) = x(n) + e(n) · h(n)
Y (k) = X(k) + E(k) ∗ H (k)
predictor is chosen as 20 [Bos97] and the prediction along the spectral coefﬁcients X(k) is
performed by the Burg method. The prediction gain for this signal in the frequency domain
is Gp = 16 dB (see Fig. 9.28d).
Frequency-domain Prediction. A further compression of the band-pass signals is possible
by using linear prediction. A backward prediction [Var06] of the band-pass signals is
applied on the coder side (see Fig. 9.29). In using a backward prediction the predictor
coefﬁcients need not be coded and transmitted to the decoder, since the estimate of the input
sample is based on the quantized signal. The decoder derives the predictor coefﬁcients p(n)
in the same way from the quantized input. A second-order predictor is sufﬁcient, because
the bandwidth of the band-pass signals is very low [Bos97].
Q
_
p(n)
k
M-1
p(n)
k
M-1
x(n)
T
C
D
M
r
e
d
o
C
S
N
T
X(n,k)
X(n,k)
Y(n,k)
r
e
d
o
c
e
D
S
N
T
y(n)
T
C
D
M
I
1
0
0
M
2M
n
1
0
0
M
2M
n
Figure 9.29 Backward prediction of band-pass signals.
Mono/Side Coding. Coding of stereo signals with left and right signals xL(n) and xR(n)
can be achieved by coding a mono signal (M) xM (n) = (xL(n) + xR(n))/2 and a side (S,
difference) signal xS(n) = (xL(n) − xR(n))/2 (M/S coding). Since for highly correlated
left and right signals the power of the side signal is reduced, a reduction in bit rate for this
signal can be achieved. The decoder can reconstruct the left signal xL(n) = xM (n) + xS(n)
and the right signal xR(n) = xM (n) − xS(n), if no quantization and coding is applied to the
mono and side signal. This M/S coding is carried out for MPEG-2 AAC [Bra98, Bos02]
with the spectral coefﬁcients of a stereo signal (see Fig. 9.30).
9.6 MPEG-2 Advanced Audio Coding
303
X (k)L
X (k)M
X (k)M
X (k)L
0.5
_
0.5
_
X (k)R
X (k)S
X (k)S
X (k)R
M/S Coder
M/S Decoder
Figure 9.30 M/S coding in frequency domain.
Intensity Stereo Coding. For intensity stereo (IS) coding a mono signal xM (n) = xL(n) +
xR(n) and two temporal envelopes eL(n) and eR(n) of the left and right signals are coded
and transmitted. On the decoding side the left signal is reconstructed by yL(n) = xM (n) ·
eL(n) and the right signal by yR(n) = xM (n) · eR(n). This reconstruction is lossy. The IS
coding of MPEG-2 AAC [Bra98] is performed by summation of spectral coefﬁcients of
both signals and by coding of scale factors which represent the temporal envelope of both
signals (see Fig. 9.31). This type of stereo coding is only useful for higher frequency bands,
since the human perception for phase shifts is non-sensitive for frequencies above 2 kHz.
E (k)L
Env
E (k)L
X (k)L
Y (k)L
X (k)M
X (k)M
X (k)R
E (k)R
Env
E (k)R
Y (k)R
IS Coder
IS Decoder
Figure 9.31 Intensity stereo coding in frequency domain.
Quantization and Coding. During the last coding step the quantization and coding of the
spectral coefﬁcients takes place. The quantizers, which are used in the ﬁgures for prediction
along spectral coefﬁcients in frequency direction (Fig. 9.27) and prediction in the frequency
domain along band-pass signals (Fig. 9.29), are now combined into a single quantizer per
spectral coefﬁcient. This quantizer performs nonlinear quantization similar to a ﬂoating-
point quantizer of Chapter 2 such that a nearly constant signal-to-noise ratio over a wide
amplitude range is achieved. This ﬂoating-point quantization with a so-called scale factor
is applied to several frequency bands, in which several spectral coefﬁcients use a common
scale factor derived from an iteration loop (see Fig. 9.19). Finally, a Huffman coding of
the quantized spectral coefﬁcients is performed. An extensive presentation can be found in
[Bos97, Bra98, Bos02].
304
Audio Coding
9.7 MPEG-4 Audio Coding
The MPEG-4 audio coding standard consists of a family of audio and speech coding
methods for different bit rates and a variety of multimedia applications [Bos02, Her02].
Besides a higher coding efﬁciency, new functionalities such as scalability, object-oriented
representation of signals and interactive synthesis of signals at the decoder are integrated.
The MPEG-4 coding standard is based on the following speech and audio coders.
• Speech coders
– CELP: Code Excitated Linear Prediction (bit rate 4–24 kbit/s).
– HVXC: Harmonic Vector Excitation Coding (bit rate 1.4–4 kbit/s).
• Audio coders
– Parametric audio: representation of a signal as a sum of sinsoids, harmonic
components, and residual components (bit rate 4–16 kbit/s).
– Structured audio: synthetic signal generation at decoder (extension of the MIDI
standard1) (200 bit–4 kbit/s).
– Generalized audio: extension of MPEG-2 AAC with additional methods in the
time-frequency domain. The basic structure is depicted in Fig. 9.19 (bit rate
6–64 kbit/s).
Basics of speech coders can found in [Var06]. The speciﬁed audio coders allow coding with
lower bit rates (Parametric Audio and Structured Audio) and coding with higher quality at
lower bit rates compared to MPEG-2 AAC.
Compared to coding methods such as MPEG-1 and MPEG-2 introduced in previous
sections, the parametric audio coding is of special interest as an extension to the ﬁlter bank
methods [Pur99, Edl00]. A parametric audio coder is shown in Fig. 9.32. The analysis
of the audio signal leads to a decomposition into sinusoidal, harmonic and noise-like sig-
nal components and the quantization and coding of these signal components is based on
psychoacoustics [Pur02a]. According to an analysis/synthesis approach [McA86, Ser89,
Smi90, Geo92, Geo97, Rod97, Mar00a] shown in Fig. 9.33 the audio signal is represented
in a parametric form given by
x(n) =
M(cid:6)
i=1
(cid:1)
Ai(n) cos
2π
fi (n)
fA
(cid:2)
n + ϕi(n)
+ xn(n).
(9.40)
The ﬁrst term describes a sum of sinusoids with time-varying amplitudes Ai(n), fre-
quencies fi (n) and phases ϕi(n). The second term consists of a noise-like component
xn(n) with time-varying temporal envelope. This noise-like component xn(n) is derived by
subtracting the synthesized sinusoidal components from the input signal. With the help of
a further analysis step, harmonic components with a fundamental frequency and multiples
of this fundamental frequency are identiﬁed and grouped into harmonic components. The
extraction of deterministic and stochastic components from an audio signal can be found in
1http://www.midi.org/.
9.7 MPEG-4 Audio Coding
305
Psycho-
acoustic
Model
Harmonic
Components
Quant
Audio
Model-based
Signal
Decomposition
Sinusoid
Components
Quant
Mux
Bit Stream
Noise
Components
Quant
Figure 9.32 MPEG-4 parametric coder.
A , f ,
i
i
i#
x(n)
Analysis
Synthesis
x (n)
s
x (n)
n
Figure 9.33 Parameter extraction with analysis/synthesis.
[Alt99, Hai03, Kei01, Kei02, Mar00a, Mar00b, Lag02, Lev98, Lev99, Pur02b]. In addition
to the extraction of sinusoidal components, the modeling of noise-like components and
transient components is of speciﬁc importance [Lev98, Lev99]. Figure 9.34 exempliﬁes
the decomposition of an audio signal into a sum of sinusoids xs(n) and a noise-like sig-
nal xn(n). The spectrogram shown in Fig. 9.35 represents the short-time spectra of the
sinusoidal components. The extraction of the sinusoids has been achieved by a modiﬁed
FFT method [Mar00a] with an FFT length of N = 2048 and an analysis hop size of RA =
512.
The corresponding parametric MPEG-4 decoder is shown in Fig. 9.36 [Edl00, Mei02].
The synthesis of the three signal components can be achieved by inverse FFT and overlap-
add methods or can be directly performed by time-domain methods [Rod97, Mei02]. A
signiﬁcant advantage of parametric audio coding is the direct access at the decoder to the
three main signal components which allows effective post-processing for the generation of
a variety of audio effects [Zöl02]. Effects such as time and pitch scaling, virtual sources in
three-dimensional spaces and cross-synthesis of signals (karaoke) are just a few examples
of interactive sound design on the decoding side.
306
Audio Coding
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
s
(cid:111)
)
n
(
n
x
0.2
0.1
0
(cid:358)0.1
(cid:358)0.2
0
0.2
0.1
0
(cid:358)0.1
(cid:358)0.2
0
0.2
0.1
0
(cid:358)0.1
(cid:358)0.2
0
1
2
3
4
5
6
7
1
2
3
4
5
6
7
8
4
x 10
8
4
x 10
1
2
3
4
n (cid:111)
5
6
7
8
4
x 10
Figure 9.34 Original signal, sum of sinusoids and noise-like signal.
9.8 Spectral Band Replication
To further reduce the bit rate an extension of MPEG-1 Layer III with the name MP3pro
was introduced [Die02, Zie02]. The underlying method, called spectral band replication
(SBR), performs a low-pass and high-pass decomposition of the audio signal, where the
low-pass ﬁltered part is coded by a standard coding method (e.g. MPEG-1 Layer III) and
the high-pass part is represented by a spectral envelope and a difference signal [Eks02,
Zie03]. Figure 9.37 shows the functional units of an SBR coder. For the analysis of the
difference signal the high-pass part (HP Generator) is reconstructed from the low-pass part
and compared to the actual high-pass part. The difference is coded and transmitted. For
decoding (see Fig. 9.38) the decoded low-pass part of a standard decoder is used by the
HP generator to reconstruct the high-pass part. The additional coded difference signal is
added at the decoder. An equalizer provides the spectral envelope shaping for the high-pass
part. The spectral envelope of the high-pass signal can be achieved by a ﬁlter bank and
computing the RMS values of each band-pass signal [Eks02, Zie03]. The reconstruction
of the high-pass part (HP Generator) can also be achieved by a ﬁlter bank and substituting
the band-pass signals by using the low-pass parts [Schu96, Her98]. To code the difference
9.8 Spectral Band Replication
307
 = 2048, R
=512
A
FFT
Spectrogram with D-FFT, N
10000
9000
8000
7000
6000
5000
4000
3000
2000
1000
→
z
H
n
i
f
0
0
200
400
600
800
1000
t in ms →
1200
1400
1600
1800
Figure 9.35 Spectrogram of sinusoidal components.
Parameter
Decoding
Model-based
Synthesis
Dequant
Harmonic
Components
Demux
Dequant
Sinusoid
Components
Bit Stream
Audio
Dequant
Noise
Components
Figure 9.36 MPEG-4 parametric decoder.

308
Audio Coding
HP
Spectral
Envelope
HP
Generator
Difference
Part
Coding
Audio
LP
L
Audio
Coder
MUX
Bit Stream
Figure 9.37 SBR coder.
Bit Stream
Demux
Audio
Decoder
Equalizer
for Spectral
Envelope
Audio
HP
Generator
L
LP
Figure 9.38 SBR decoder.
signal of the high-pass part additive sinusoidal models can be applied such as the parametric
methods of the MPEG-4 coding approach.
Figure 9.39 shows the functional units of the SBR method in the frequency domain.
First, the short-time spectrum is used to calculate the spectral envelope (Fig. 9.39a). The
spectral envelope can be derived from an FFT, a ﬁlter bank, the cepstrum or by linear
prediction [Zöl02]. The band-limited low-pass signal can be downsampled and coded by a
standard coder which operates at a reduced sampling rate. In addition, the spectral envelope
has to be coded (Fig. 9.39b). On the decoding side the reconstruction of the upper spectrum
is achieved by frequency-shifting of the low-pass part or even speciﬁc low-pass parts and
applying the spectral envelope onto this artiﬁcial high-pass spectrum (Fig. 9.39c). An
efﬁcient implementation of a time-varying spectral envelope computation (at the coder
side) and spectral weighting of the high-pass signal (at the decoder side) with a complex-
valued QMF ﬁlter bank is described in [Eks02].
9.9 Java Applet – Psychoacoustics
The applet shown in Fig. 9.40 demonstrates psychoacoustic audio masking effects [Gui05].
It is designed for a ﬁrst insight into the perceptual experience of masking a sinusoidal signal
with band-limited noise.
You can choose between two predeﬁned audio ﬁles from our web server (audio1.wav
or audio2.wav). These are band-limited noise signals with different frequency ranges. A
sinusoidal signal is generated by the applet, and two sliders can be used to control its
frequency and magnitude values.
9.9 Java Applet – Psychoacoustics
309
B
d
/
|
)
f
(
X
|
B
d
/
|
)
f
(
X
|
B
d
/
|
)
f
(
X
|
0
-20
-40
-60
-80
-100
0
-20
-40
-60
-80
-100
0
-20
-40
-60
-80
-100
a) Short-time spectrum and spectral envelope
Low-pass
0
2
4
6
8
10
f/kHz
12
14
16
18
20
b) Short-time spectrum for audio coding and spectral envelope
Low-pass
0
2
4
6
8
10
f/kHz
12
14
16
18
20
c) Reconstruction of upper short-time spectrum with spectral envelope
Shifting of spectrum
Spectral weighting with
spectral envelope
0
2
4
6
8
10
f/kHz
12
14
16
18
20
Figure 9.39 Functional units of SBR method.
310
Audio Coding
Figure 9.40 Java applet – psychoacoustics.
9.10 Exercises
1. Psychoacoustics
1. Human hearing
(a) What is the frequency range of human sound perception?
(b) What is the frequency range of speech?
(c) In the above speciﬁed range where is the human hearing most sensitive?
(d) Explain how the absolute threshold of hearing has been obtained.
2. Masking
(a) What is frequency-domain masking?
(b) What is a critical band and why is it needed for frequency masking phenomena?
(c) Consider ai and fi to be respectively the amplitude and the frequency of a
partial at index i and V (ai) to be the corresponding volume in dB. The differ-
ence between the level of the masker and the masking threshold is −10 dB. The
masking curves toward lower and higher frequencies are described respectively
by a left slope (27 dB/Bark) and a right slope (15 dB/Bark). Explain the main
steps of frequency masking in this case and show with plots how this masking
phenomena is achieved.
(d) What are the psychoacoustic parameters used for lossy audio coding?
(e) How can we explain the temporal masking and what is its duration after stop-
ping the active masker?
References
2. Audio coding
311
1. Explain the lossless coder and decoder.
2. What is the achievable compression factor for lossless coding?
3. Explain the MPEG-1 Layer III coder and decoder.
4. Explain the MPEG-2 AAC coder and decoder.
5. What is temporal noise shaping?
6. Explain the MPEG-4 coder and decoder.
7. What is the beneﬁt of SBR?
References
[Alt99]
R. Althoff, F. Keiler, U. Zölzer: Extracting Sinusoids from Harmonic Signals,
Proc. DAFX-99 Workshop on Digital Audio Effects, pp. 97–100, Trondheim,
1999.
[Blo95]
T. Block: Untersuchung von Verfahren zur verlustlosen Datenkompression von
digitalen Audiosignalen, Studienarbeit, TU Hamburg-Harburg, 1995.
[Bos97] M. Bosi, K. Brandenburg, S. Quackenbush, L. Fielder, K. Akagiri, H. Fuchs,
M. Dietz, J. Herre, G. Davidson, Y. Oikawa: ISO/IEC MPEG-2 Advanced Audio
Coding, J. Audio Eng. Soc., Vol. 45, pp. 789–814, October 1997.
[Bos02] M. Bosi, R. E. Goldberg: Introduction to Digital Audio Coding and Standards,
Kluwer Academic, Boston, 2002.
[Bra92]
K. Brandenburg, J. Herre: Digital Audio Compression for Professional
Applications, Proc. 92nd AES Convention, Preprint No. 3330, Vienna, 1992.
[Bra94]
[Bra98]
[Cel93]
K. Brandenburg, G. Stoll: The ISO-MPEG-1 Audio: A Generic Standard for
Coding of High Quality Digital Audio, J. Audio Eng. Soc., Vol. 42, pp. 780–
792, October 1994.
K. Brandenburg: Perceptual Coding of High Quality Digital Audio, in M.
Kahrs, K. Brandenburg (Ed.), Applications of Digital Signal Processing to
Audio and Acoustics, Kluwer Academic, Boston, 1998.
C. Cellier, P. Chenes, M. Rossi: Lossless Audio Data Compression for Real-
Time Applications, Proc. 95th AES Convention, Preprint No. 3780, New York,
1993.
[Cra96]
P. Craven, M. Gerzon: Lossless Coding for Audio Discs, J. Audio Eng. Soc.,
Vol. 44, pp. 706–720, 1996.
312
[Cra97]
P. Craven, M. Law, J. Stuart: Lossless Compression Using IIR Prediction, Proc.
102nd AES Convention, Preprint No. 4415, Munich, 1997.
Audio Coding
[Die02] M. Dietz, L. Liljeryd, K. Kjörling, O. Kunz: Spectral Band Replication: A
Novel Approach in Audio Coding, Proc. 112th AES Convention, Preprint
No. 5553, Munich, 2002.
[Edl89]
[Edl00]
[Edl95]
[Eks02]
B. Edler: Codierung von Audiosignalen mit überlappender Transformation und
adaptiven Fensterfunktionen, Frequenz, Vol. 43, pp. 252–256, 1989.
B. Edler, H. Purnhagen: Parametric Audio Coding, 5th International
Conference on Signal Processing (ICSP 2000), Beijing, August 2000.
B. Edler: Äquivalenz von Transformation und Teilbandzerlegung in der
Quellencodierung, Dissertation, Universität Hannover, 1995.
P. Ekstrand: Bandwidth Extension of Audio Signals by Spectral Band
Replication, Proc. 1st IEEE Benelux Workshop on Model-Based Processing
and Coding of Audio (MPCA-2002), Leuven, Belgium, 2002.
[Ern00] M. Erne: Signal Adpative Audio Coding Using Wavelets and Rate Optimization,
Dissertation, ETH Zurich, 2000.
[Geo92]
[Geo97]
[Glu93]
E. B. George, M. J. T. Smith: Analysis-by-Synthesis/Overlap-Add Sinusoidal
Modeling Applied to the Analysis and Synthesis of Musical Tones, J. Audio
Eng. Soc., Vol. 40, pp. 497–516, June 1992.
E. B. George, M. J. T. Smith: Speech Analysis/Synthesis and Modiﬁcation
using an Analysis-by-Synthesis/Overlap-Add Sinusoidal Model, IEEE Trans.
on Speech and Audio Processing, Vol. 5, No. 5, pp. 389–406, September 1997.
R. Gluth: Beiträge zur Beschreibung und Realisierung digitaler, nichtrekursiver
Filterbänke auf der Grundlage linearer diskreter Transformationen, Disserta-
tion, Ruhr-Universität Bochum, 1993.
[Gui05] M. Guillemard, C. Ruwwe, U. Zölzer: J-DAFx – Digital Audio Effects in Java,
Proc. 8th Int. Conference on Digital Audio Effects (DAFx-05), pp. 161–166,
Madrid, 2005.
[Hai03]
S. Hainsworth, M. Macleod: On Sinusoidal Parameter Estimation, Proc.
DAFX-03 Conference on Digital Audio Effects, London, September 2003.
[Han98] M. Hans: Optimization of Digital Audio for Internet Transmission, PhD thesis,
Georgia Inst. Technol., Atlanta, 1998.
[Han01] M. Hans, R. W. Schafer: Lossless Compression of Digital Audio, IEEE Signal
Processing Magazine, Vol. 18, No. 4, pp. 21–32, July 2001.
[Hel72]
R. P. Hellman: Asymmetry in Masking between Noise and Tone, Perception and
Psychophys., Vol. 11, pp. 241–246, 1972.
References
[Her96]
[Her98]
[Her99]
313
J. Herre, J. D. Johnston: Enhancing the Performance of Perceptual Audio
Coders by Using Temporal Noise Shaping (TNS), Proc. 101st AES Convention,
Preprint No. 4384, Los Angeles, 1996.
J. Herre, D. Schultz: Extending the MPEG-4 AAC Codec by Perceptual Noise
Substitution, Proc. 104th AES Convention, Preprint No. 4720, Amsterdam,
1998.
J. Herre: Temporal Noise Shaping, Quantization and Coding Methods
in Perceptual Audio Coding: A Tutorial Introduction, Proc. AES 17th
International Conference on High Quality Audio Coding, Florence, September
1999.
[Her02]
J. Herre, B. Grill: Overview of MPEG-4 Audio and its Applications in Mobile
Communications, Proc. 112th AES Convention, Preprint No. 5553, Munich,
2002.
[Huf52] D. A. Huffman: A Method for the Construction of Minimum-Redundancy
Codes, Proc. of the IRE, Vol. 40, pp. 1098–1101, 1952.
[ISO92]
ISO/IEC 11172-3: Coding of Moving Pictures and Associated Audio for Digital
Storage Media at up to 1.5 Mbits/s – Audio Part, International Standard, 1992.
[Jay84]
N. S. Jayant, P. Noll: Digital Coding of Waveforms, Prentice Hall, Englewood
Cliffs, NJ, 1984.
[Joh88a]
J. D. Johnston: Transform Coding of Audio Signals Using Perceptual Noise
Criteria, IEEE J. Selected Areas in Communications, Vol. 6, No. 2, pp. 314–
323, February 1988.
[Joh88b]
J. D. Johnston: Estimation of Perceptual Entropy Using Noise Masking
Criteria, Proc. ICASSP-88, pp. 2524–2527, 1988.
[Kap92] R. Kapust: A Human Ear Related Objective Measurement Technique Yields
Audible Error and Error Margin, Proc. 11th Int. AES Conference – Test &
Measurement, Portland, pp. 191–202, 1992.
[Kei01]
F. Keiler, U. Zölzer: Extracting Sinusoids from Harmonic Signals, J. New
Music Research, Special Issue: Musical Applications of Digital Signal
Processing, Guest Editor: Mark Sandler, Vol. 30, No. 3, pp. 243–258,
September 2001.
[Kei02]
F. Keiler, S. Marchand: Survey on Extraction of Sinusoids in Stationary Sounds,
Proc. DAFX-02 Conference on Digital Audio Effects, pp. 51–58, Hamburg,
2002.
[Kon94] K. Konstantinides: Fast Subband Filtering in MPEG Audio Coding, IEEE
Signal Processing Letters, Vol. 1, No. 2, pp. 26–28, February 1994.
314
Audio Coding
[Lag02] M. Lagrange, S. Marchand, J.-B. Rault: Sinusoidal Parameter Extraction and
Component Selection in a Non Stationary Model, Proc. DAFX-02 Conference
on Digital Audio Effects, pp. 59–64, Hamburg, 2002.
[Lev98]
[Lev99]
S. Levine: Audio Representations for Data Compression and Compressed
Domain Processing, PhD thesis, Stanford University, 1998.
S. Levine, J. O. Smith: Improvements to the Switched Parametric & Transform
Audio Coder, Proc. 1999 IEEE Workshop on Applications of Signal Processing
to Audio and Acoustics, New Paltz, NY, October 1999.
[Lie02]
T. Liebchen: Lossless Audio Coding Using Adaptive Multichannel Prediction,
Proc. 113th AES Convention, Preprint No. 5680, Los Angeles, 2002.
[Mar00a] S. Marchand: Sound Models for Computer Music, PhD thesis, University of
Bordeaux, October 2000.
[Mar00b] S. Marchand: Compression of Sinusoidal Modeling Parameters, Proc.
DAFX-00 Conference on Digital Audio Effects, pp. 273–276, Verona,
December 2000.
[Mal92] H. S. Malvar: Signal Processing with Lapped Transforms, Artech House,
Boston, 1992.
[McA86] R. McAulay, T. Quatieri: Speech Transformations Based in a Sinusoidal
Representation, IEEE Trans. Acoustics, Speech, Signal Processing, Vol. 34,
No. 4, pp. 744–754, 1989.
[Mas85]
J. Masson, Z. Picel: Flexible Design of Computationally Efﬁcient Nearly
Perfect QMF Filter Banks, Proc. ICASSP-85, pp. 541–544, 1985.
[Mei02] N. Meine, H. Purnhagen: Fast Sinusoid Synthesis For MPEG-4 HILN
Parametric Audio Decoding, Proc. DAFX-02 Conference on Digital Audio
Effects, pp. 239–244, Hamburg, September 2002.
[Pen93] W. B. Pennebaker, J. L. Mitchell: JPEG Still Image Data Compression
Standard, Van Nostrand Reinhold, New York, 1993.
[Pri86]
[Pri87]
J. P. Princen, A. B. Bradley: Analysis/Synthesis Filter Bank Design Based on
Time Domain Aliasing Cancellation, IEEE Trans. on Acoustics, Speech, and
Signal Processing, Vol. 34, No. 5, pp. 1153–1161, October 1986.
J. P. Princen, A. W. Johnston, A. B. Bradley: Subband/Transform Coding
Using Filter Bank Designs Based on Time Domain Aliasing Cancellation, Proc.
ICASSP-87, pp. 2161–2164, 1987.
[Pur97] M. Purat, T. Liebchen, P. Noll: Lossless Transform Coding of Audio Signals,
Proc. 102nd AES Convention, Preprint No. 4414, Munich, 1997.
[Pur99]
H. Purnhagen: Advances in Parametric Audio Coding, Proc. 1999 IEEE
Workshop on Applications of Signal Processing to Audio and Acoustics, New
Paltz, NY, October 1999.
References
315
[Pur02a] H. Purnhagen, N. Meine, B. Edler: Sinusoidal Coding Using Loudness-Based
Component Selection, Proc. ICASSP-2002, May 13–17, Orlando, FL, 2002.
[Pur02b] H. Purnhagen: Parameter Estimation and Tracking for Time-Varying Sinusoids,
Proc. 1st IEEE Benelux Workshop on Model Based Processing and Coding of
Audio, Leuven, Belgium, November 2002.
[Raa02] M. Raad, A. Mertins: From Lossy to Lossless Audio Coding Using SPIHT, Proc.
DAFX-02 Conference on Digital Audio Effects, pp. 245–250, Hamburg, 2002.
[Rao90] K. R. Rao, P. Yip: Discrete Cosine Transform – Algorithms, Advantages,
Applications, Academic Press, San Diego, 1990.
[Rob94]
T. Robinson: SHORTEN: Simple Lossless and Near-Lossless Waveform Com-
pression, Technical Report CUED/F-INFENG/TR.156, Cambridge University
Engineering Department, Cambridge, December 1994.
[Rod97] X. Rodet: Musical Sound Signals Analysis/Synthesis: Sinusoidal+Residual and
Elementary Waveform Models, Proceedings of the IEEE Time-Frequency and
Time-Scale Workshop (TFTS-97), University of Warwick, Coventry, August
1997.
[Rot83]
J. H. Rothweiler: Polyphase Quadrature Filters – A New Subband Coding
Technique, Proc. ICASSP-87, pp. 1280–1283, 1983.
[Sauv90] U. Sauvagerd: Bitratenreduktion hochwertiger Musiksignale unter Verwendung
von Wellendigitalﬁltern, VDI-Verlag, Düsseldorf, 1990.
[Schr79] M. R. Schroeder, B. S. Atal, J. L. Hall: Optimizing Digital Speech Coders by
Exploiting Masking Properties of the Human Ear, J. Acoust. Soc. Am., Vol. 66,
No. 6, pp. 1647–1652, December 1979.
[Sch02]
G. D. T. Schuller, Bin Yu, Dawei Huang, B. Edler: Perceptual Audio Coding
Using Adaptive Pre- and Post-Filters and Lossless Compression, IEEE Trans.
on Speech and Audio Processing, Vol. 10, No. 6, pp. 379–390, September 2002.
[Schu96] D. Schulz: Improving Audio Codecs by Noise Substitution, J. Audio Eng. Soc.,
Vol. 44, pp. 593–598, July/August 1996.
[Ser89]
[Sin93]
X. Serra: A System for Sound Analysis/Transformation/Synthesis Based on a
Deterministic plus Stochastic Decomposition, PhD thesis, Stanford University,
1989.
D. Sinha, A. H. Tewﬁk: Low Bit Rate Transparent Audio Compression Using
Adapted Wavelets, IEEE Trans. on Signal Processing, Vol. 41, pp. 3463–3479,
1993.
[Smi90]
J. O. Smith, X. Serra: Spectral Modeling Synthesis: A Sound Analysis/Synthesis
System Based on a Deterministic plus Stochastic Decomposition, Computer
Music J., Vol. 14, No. 4, pp. 12–24, 1990.
316
Audio Coding
[Sqa88]
EBU-SQAM: Sound Quality Assessment Material, Recordings for Subjective
Tests, CompactDisc, 1988.
[Ter79]
E. Terhardt: Calculating Virtual Pitch, Hearing Res., Vol. 1, pp. 155–182, 1979.
[Thei88] G. Theile, G. Stoll, M. Link: Low Bit-Rate Coding of High-Quality Audio
Signals, EBU Review, No. 230, pp. 158–181, August 1988.
[Vai93]
[Var06]
P. P. Vaidyanathan: Multirate Systems and Filter Banks, Prentice Hall,
Englewood Cliffs, NJ, 1993.
P. Vary, R. Martin: Digital Speech Transmission. Enhancement, Coding and
Error Concealment, John Wiley & Sons, Ltd, Chichester, 2006.
[Vet95] M. Vetterli, J. Kovacevic: Wavelets and Subband Coding, Prentice Hall,
Englewood Cliffs, NJ, 1995.
[Zie02]
[Zie03]
T. Ziegler, A. Ehret, P. Ekstrand, M. Lutzky: Enhancing mp3 with SBR:
Features and Capabilities of the new mp3PRO Algorithm, Proc. 112th AES
Convention, Preprint No. 5560, Munich, 2002.
T. Ziegler, M. Dietz, K. Kjörling, A. Ehret: aacPlus-Full Bandwidth Audio
Coding for Broadcast and Mobile Applications, International Signal Processing
Conference, Dallas, 2003.
[Zöl02]
U. Zölzer (Ed.): DAFX – Digital Audio Effects, John Wiley & Sons, Ltd,
Chichester, 2002.
[Zwi82]
E. Zwicker: Psychoakustik, Springer-Verlag, Berlin, 1982.
[Zwi90]
E. Zwicker, H. Fastl: Psychoacoustics, Springer-Verlag, Berlin, 1990.
Index
AD conversion, 63
delta-sigma, 70
oversampling, 66
AD converter, 79
characteristics, 79
counter, 84
delta-sigma, 85
half-ﬂash, 83
parallel, 82
subranging, 83
successive approximation, 83
AES, 101
AES/EBU interface, 2, 101
All-pass decomposition, 130, 132, 133
Ambisonics, 14
Anti-aliasing ﬁlter, 242
Anti-imaging ﬁlter, 241
Aperture delay, 79
Aperture jitter, 80
Attack time, 231
Audio coding, 273
lossless, 273
lossy, 275
Averaging time, 231
Band-limiting, 63, 86, 251
Bandwidth, 115, 121
relative, 115, 121
Biphase code, 102
Bit allocation, 290
Boost/cut, 122, 124, 130
Center frequency, 125, 133
Coding
Intensity stereo, 303
Mono/side, 302
Coding techniques, 4, 273, 284
COFDM, 4
Comb ﬁlter, 75
recursive, 200, 216
Compact disc, 10
Compressor, 226, 231
Conversion time, 79
Critical bands, 277
DA conversion, 63
delta-sigma, 71
oversampling, 66
DA converter, 86
characteristics, 86
delta-sigma, 92
R-2R networks, 92
switched sources, 89
weighted capacitors, 90
weighted resistors, 90
DAB, 2, 5
Decimation, 75, 169, 175, 246
Deemphasis/preemphasis, 103
Deglitcher, 87
Delta modulation, 69
Delta-sigma modulation, 12, 14, 66
decimation ﬁlter, 75
ﬁrst-order, 68
higher-order, 74
multistage, 72
Digital Audio Signal Processing Second Edition Udo Zölzer
© 2008 John Wiley & Sons, Ltd
318
Index
second-order, 71
signal-to-noise ratio, 73
DFT, 158
Differential nonlinearity, 80
Digital ampliﬁer, 14
Digital crossover, 16
Digital Radio Mondiale (DRM), 8
Digital Versatile Disc – Audio
(DVD-A), 13
Direct Stream Digital, 12
Dither, 36, 45
Downsampling, 242
DSP, 98, 100, 107
DVD, 13
Dynamic range, 225
Dynamic range control, 225
Early reﬂections, 191, 195
EBU, 101
Echo density, 203
Eigenfrequency, 191, 201, 203
Entropy coding, 274
Equalizers, 115
design of nonrecursive, 167
design of recursive, 119
nonrecursive, 157
recursive, 115, 128
Expander, 226, 231
Fast convolution, 158, 161
FDDI, 105
Feedback systems, 208
FFT, 7, 280
Filter
Q-factor, 125
all-pass, 131–133, 136, 202
band-pass, 115, 121, 136
band-stop, 115, 121
bilinear transformation, 128
constant-Q, 121
decimation, 169, 177, 178
high-pass, 135, 181
interpolation, 169, 177, 178
kernel, 169, 172, 175
low-pass, 134, 181
low-pass/high-pass, 115, 119
nonrecursive, 77
octave, 115
one-third octave, 116
peak, 117, 124, 132, 135, 136,
181
shelving, 117, 121, 130, 133–136,
181
Filter bank, 5, 285
analysis, 4, 213
multi-complementary, 168
octave-band, 168
synthesis, 4, 214
Filter structures
coefﬁcient quantization, 138
limit cycles, 157
noise behavior of recursive, 143
noise shaping, 150
nonrecursive, 161, 168
parametric, 128
recursive, 138
scaling, 154
Frequency density, 202
Frequency sampling, 167
Gain error, 80
Hard disc recording, 1
Huffmann coding, 274
IDFT, 158
Image model, 192
Integral nonlinearity, 80
Internet audio, 9
Interpolation, 66, 75, 169, 174, 175,
246, 247, 254
Lagrange, 260
polynomial, 257
spline, 261
ISO-MPEG1, 4, 284
coder, 284
decoder, 286
Java applet, 59, 93, 182, 218, 238, 310
Index
Latency time, 78, 178, 275
Limit cycles, 157
Limiter, 226, 231
MADI interface, 1, 102
Masking, 279, 281, 284
index, 289
threshold, 279, 281, 282, 284, 289
global, 290
MiniDisc, 10
Mixing console, 1
Monotonicity, 80
MP3, 9, 10
MPEG-2, 291
MPEG-2 AAC, 292
MPEG-4, 8, 304
Noise
gate, 225, 226, 231
shaping, 14, 42, 47, 144, 150
Number representation, 47
ﬁxed-point, 47
ﬂoating-point, 53
format conversion, 56
Nyquist sampling, 63
OFDM transmission, 7
guard interval, 7
Offset error, 80
Oversampling, 14, 65
signal-to-noise ratio, 66
Peak factor, 23
Peak measurement, 118, 228, 229
Polyphase representation, 245
Prediction, 274
Pseudo-random sequence, 193, 213
Psychoacoustic models, 287
Pulse width modulation, 14
Q-factor, 120, 121
Quantization error
correlation with signal, 34
ﬁrst-order statistics, 30
319
noise shaping, 42
power, 65
probability density function, 22
second-order statistics, 32
Quantization model, 21
Quantization noise, 284
Quantization step, 22, 23, 65, 247, 249
Quantization theorem, 21, 24
R-DAT, 10
Ray tracing model, 192
Real-time operating system, 97
Release time, 231
Resolution, 79
Reverberation time, 191, 195, 205
frequency-dependent, 207
Room impulse response, 191
approximation, 213
measurement of, 193
Room simulation, 14, 191
Root mean square measurement, 118,
229
Sample-and-hold, 79
circuit, 63
function, 247
Sampling period, 63
Sampling rate, 2, 63
Sampling rate conversion, 241
asynchronous, 246
multistage, 252
single-stage, 250
synchronous, 244
Sampling theorem, 63
Scale factor, 303
Scaling factor, 289
Settling time, 87
Signal processor
ﬁxed-point, 98
ﬂoating-point, 100
multi-processor systems, 109
single-processor systems, 107
Signal quantization, 21
320
Index
Signal-to-mask ratio, 284, 287, 290,
Surround systems, 14
291
Signal-to-noise ratio, 23, 53, 55, 57
Sinc distortion, 65
Sound channel, 4
Sound studio, 1
SPDIF interface, 102
Spectral Band Replication SBR, 8
Spectral band replication SBR, 306
Spreading function, 281
Static curve, 226
Storage media
DVD-A, 13
MiniDisc, 10
SACD, 11
Studio technology, 1
Subsequent reverberation, 191, 200
Super Audio CD (SACD), 11
Threshold of hearing, 46, 278, 280,
289
Time constants, 230
Tonality index, 280, 282
Total harmonic distortion, 81
Transmission techniques
DRM, 8
internet audio, 9
Upsampling, 241
Weighting ﬁlter
A-, 118
CCIR-468, 118
Word-length, 23, 138
