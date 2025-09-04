# Digital Audio DSP (Zölzer) — Chunk 3/30

> Source: `Udo-Zolzer-digital-audio-signal-processing.9780470997857.40435.pdf` · Pages ~1-331 · Extractor: pdfminer.six
> Target ≈ 2800 words, Overlap = 200 words · Actual = 13289

> Note: DSP textbook content for SpectralCanvasPro RT-audio knowledge base

---
Preface to the First Edition
Digital audio signal processing is employed in recording and storing music and speech
signals, for sound mixing and production of digital programs, in digital transmission to
broadcast receivers as well as in consumer products like CDs, DATs and PCs. In the latter
case, the audio signal is in a digital form all the way from the microphone right up to the
loudspeakers, enabling real-time processing with fast digital signal processors.
This book provides the basis of an advanced course in Digital Audio Signal Processing
which I have been giving since 1992 at the Technical University Hamburg-Harburg.
It is directed at students studying engineering, computer science and physics and also
for professionals looking for solutions to problems in audio signal processing like in
the ﬁelds of studio engineering, consumer electronics and multimedia. The mathematical
and theoretical fundamentals of digital audio signal processing systems will be presented
and typical applications with an emphasis on realization aspects will be discussed. Prior
knowledge of systems theory, digital signal processing and multirate signal processing is
taken as a prerequisite.
The book is divided into two parts. The ﬁrst part (Chapters 1–4) presents a basis
for hardware systems used in digital audio signal processing. The second part (Chapters
5–9) discusses algorithms for processing digital audio signals. Chapter 1 describes the
course taken by an audio signal from its recording in a studio up to its reproduction
at home. Chapter 2 contains a representation of signal quantization, dither techniques
and spectral shaping of quantization errors used for reducing the nonlinear effects of
quantization. In the end, a comparison is made between the ﬁxed-point and ﬂoating-
point number representations as well as their associated effects on format conversion and
algorithms. Chapter 3 describes methods for AD/DA conversion of signals, starting with
Nyquist sampling, methods for oversampling techniques and delta-sigma modulation. The
chapter closes with a presentation of some circuit design of AD/DA converters. After an
introduction to digital signal processors and digital audio interfaces, Chapter 4 describes
simple hardware systems based on a single- and multiprocessor solutions. The algorithms
introduced in the following Chapters 5–9 are, to a great extent, implemented in real-time
on hardware platforms presented in Chapter 4. Chapter 5 describes digital audio equalizers.
Apart from the implementation aspects of recursive audio ﬁlters, nonrecursive linear phase
ﬁlters based on fast convolution and ﬁlter banks are introduced. Filter designs, parametric
ﬁlter structures and precautions for reducing quantization errors in recursive ﬁlters are dealt
with in detail. Chapter 6 deals with room simulation. Methods for simulation of artiﬁcial
room impulse response and methods for approximation of measured impulse responses
xii
Preface to the First Edition
are discussed. In Chapter 7 the dynamic range control of audio signals is described.
These methods are applied at several positions in the audio chain from the microphone
up to the loudspeakers in order to adapt to the dynamics of the recording, transmission
and listening environment. Chapter 8 contains a presentation of methods for synchronous
and asynchronous sampling rate conversion. Efﬁcient algorithms are described which are
suitable for real-time processing as well as off-line processing. Both lossless and lossy
audio coding are discussed in Chapter 9. Lossless audio coding is applied for storing of
higher word-lengths. Lossy audio coding, on the other hand, plays a signiﬁcant role in
communication systems.
I would like to thank Prof. Fliege (University of Mannheim), Prof. Kammeyer
(University of Bremen) and Prof. Heute (University of Kiel) for comments and support.
I am also grateful to my colleagues at the TUHH and especially Dr. Alfred Mertins,
Dr. Thomas Boltze, Dr. Bernd Redmer, Dr. Martin Schönle, Dr. Manfred Schusdziarra,
Dr. Tanja Karp, Georg Dickmann, Werner Eckel, Thomas Scholz, Rüdiger Wolf, Jens
Wohlers, Horst Zölzer, Bärbel Erdmann, Ursula Seifert and Dieter Gödecke. Apart from
these, I would also like to say a word of gratitude to all those students who helped me in
carrying out this work successfully.
Special thanks go to Saeed Khawaja for his help during translation and to Dr. Anthony
Macgrath for proof-reading the text. I also would like to thank Jenny Smith, Colin
McKerracher, Ian Stoneham and Christian Rauscher (Wiley).
My special thanks are directed to my wife Elke and my daughter Franziska.
Udo Zölzer
Hamburg, July 1997
Chapter 1
Introduction
It is hardly possible to make a start in the ﬁeld of digital audio signal processing without
having a ﬁrst insight into the variety of technical devices and systems of audio technology.
In this introductory chapter, the ﬁelds of application for digital audio signal processing are
presented. Starting from recording in a studio or in a concert hall, the whole chain of signal
processing is shown, up to the reproduction at home or in a car (see Fig. 1.1). The ﬁelds of
application can be divided into the following areas:
• studio technology;
• digital transmission systems;
• storage media;
• audio components for home entertainment.
The basic principles of the above-mentioned ﬁelds of application will be presented as an
overview in order to exhibit the uses of digital signal processing. Special technical devices
and systems are outside the focus of this chapter. These devices and systems are strongly
driven by the development of the computer technology with yearly changes and new
devices based on new technologies. The goal of this introduction is a trend-independent
presentation of the entire processing chain from the instrument or singer to the listener and
consumer of music. The presentation of signal processing techniques and their algorithms
will be discussed in the following chapters.
1.1 Studio Technology
While recording speech or music in a studio or in a concert hall, the analog signal from a
microphone is ﬁrst digitized, fed to a digital mixing console and then stored on a digital
storage medium. A digital sound studio is shown in Fig. 1.2. Besides the analog sources
(microphones), digital sources are fed to the digital mixing console over multichannel
MADI interfaces [AES91]. Digital storage media like digital multitrack tape machines
have been replaced by digital hard disc recording systems which are also connected via
Digital Audio Signal Processing Second Edition Udo Zölzer
© 2008 John Wiley & Sons, Ltd
2
Introduction
Concert hall
Audio studio
l
e
o
s
n
o
C
g
n
x
M
i
i
l
a
t
i
g
D
i
i
a
d
e
M
e
g
a
r
o
t
S
Digital
Transmission
Audio System
(CD,DVD,
Radio,PC)
Living room
Automobile
PC
l
a
t
i
g
D
i
Figure 1.1 Signal processing for recording, storage, transmission and reproduction.
multichannel MADI interfaces to the mixing console. The ﬁnal stereo mix is stored via a
two-channel AES/EBU interface [AES92] on a two-channel MASTER machine. External
appliances for effects or room simulators are also connected to the mixing console via
a two-channel AES/EBU interface. All systems are synchronized by a MASTER clock
reference. In digital audio technology, the sampling rates1 fS = 48 kHz for professional
studio technology, fS = 44.1 kHz for compact disc and fS = 32 kHz for broadcasting
applications are established. In addition, multiples of these sampling frequencies such as
88.2, 96, 176.4, and 192 kHz are used. The sound mixing console plays a central role
in a digital sound studio. Figure 1.3 shows the functional units. The N input signals are
processed individually. After level and panorama control, all signals are summed up to
give a stereo mix. The summation is carried out several times so that other auxiliary stereo
and/or mono signals are available for other purposes. In a sound channel (see Fig. 1.4), an
equalizer unit (EQ), a dynamic unit (DYN), a delay unit (DEL), a gain element (GAIN)
and a panorama element (PAN) are used. In addition to input and output signals in an audio
channel, inserts as well as auxiliary or direct outputs are required.
1.2 Digital Transmission Systems
In this section digital transmission will be brieﬂy explained. Besides the analog wireless
broadcasting systems based on amplitude and frequency modulation, DAB2 (Digital Audio
Broadcasting) has been introduced in several countries [Hoe01]. On the other hand, the
internet has pushed audio/video distribution, internet radio and video via cable networks.
Terrestrial Digital Broadcasting (DAB)
With the introduction of terrestrial digital broadcasting, the quality standards of a compact
disc will be achieved for mobile and stationary reception of radio signals [Ple91].
Therefore, the data rate of a two-channel AES/EBU signal from a transmitting studio
is reduced with the help of a source coder [Bra94] (see Fig. 1.5). Following the source
coder (SC), additional information (AI) like the type of program (music/speech) and
1Data rate 16 bit × 48 kHz = 768 kbit/s; data rate (AES/EBU signal) 2 × (24 + 8) bit × 48 kHz =
3.072 Mbit/s; data rate (MADI signal) 56 × (24 + 8) bit × 48 kHz = 86.016 Mbit/s.
2http://www.worlddab.org/.
1.2 Digital Transmission Systems
3
DIGITAL MULTITRACK
HARD DISC RECORDER
DIGITAL STEREO
MASTER MACHINE
MADI
AES/EBU
ROOM SIMULATOR
EFFECTS
AES/EBU
AES/EBU
DIGITAL MIXING CONSOLE
DIGITAL MIXING CONSOLE
MASTER
MASTER
SYNC
SYNC
GENERATOR
GENERATOR
MADI
MADI
AES/EBU -> MADI
CONVERSION
ANALOG-to-DIGITAL
CONVERSION
DIGITAL SIGNALS
ANALOG SIGNALS
Figure 1.2 Digital sound studio.
(cid:26)(cid:16)(cid:3)(cid:16)(cid:7)(cid:9)(cid:4)(cid:14)(cid:27)(cid:10)(cid:28)(cid:13)(cid:7)(cid:18)(cid:14)(cid:14)(cid:14)(cid:14)(cid:22)(cid:29)(cid:14)(cid:29)(cid:14)(cid:24)
(cid:22)
(cid:6)(cid:1)
(cid:22)
(cid:23)
(cid:6)(cid:1)
(cid:23)
(cid:24)
(cid:6)(cid:1)
(cid:24)
(cid:6)(cid:16)(cid:3)(cid:10)(cid:9)(cid:4)(cid:14)(cid:1)(cid:8)(cid:11)(cid:20)(cid:2)(cid:18)(cid:18)(cid:16)(cid:10)(cid:3)(cid:25)
(cid:30)(cid:31)(cid:13)(cid:9)(cid:4)(cid:16) (cid:2)(cid:8)(cid:14)!(cid:10)(cid:16)(cid:7)
(cid:26)"(cid:10)(cid:9)(cid:12)(cid:16)(cid:20)(cid:18)(cid:14)!(cid:10)(cid:16)(cid:7)
(cid:26)(cid:2)(cid:4)(cid:9)"(cid:14)!(cid:10)(cid:16)(cid:7)
(cid:6)(cid:7)(cid:2)(cid:8)(cid:2)(cid:11)(cid:14)&(cid:13)’(cid:16)(cid:4)(cid:16)(cid:9)(cid:8)(cid:16)(cid:2)(cid:18)
(cid:1)(cid:2)(cid:3)(cid:2)(cid:4)(cid:5)(cid:6)(cid:7)(cid:2)(cid:4)(cid:4)(cid:2)(cid:8)
#(cid:9)(cid:16)(cid:10)(cid:14)$(cid:11)(cid:10)(cid:7)(cid:8)(cid:11)(cid:4)
(cid:1)(cid:9)(cid:10)(cid:11)(cid:8)(cid:9)(cid:12)(cid:9)(cid:5)(cid:6)(cid:7)(cid:2)(cid:4)(cid:4)(cid:2)(cid:8)
(cid:1)(cid:9)(cid:10)(cid:14)$(cid:11)(cid:10)(cid:7)(cid:8)(cid:11)(cid:4)
(cid:6)(cid:13)(cid:12)(cid:12)(cid:2)(cid:14)(cid:15)(cid:16)(cid:10)(cid:17)(cid:18)
(cid:15)(cid:2)%(cid:7)(cid:14)$(cid:21)(cid:9)(cid:10)(cid:10)(cid:2)(cid:4)
(cid:6)(cid:13)(cid:12)(cid:12)(cid:2)(cid:14)(cid:19)(cid:2)(cid:20)(cid:21)(cid:7)(cid:18)(cid:14)
(cid:6)(cid:7)(cid:2)(cid:8)(cid:2)(cid:11)(cid:14)O(cid:13)(cid:7)(cid:28)(cid:13)(cid:7)
(cid:19)(cid:16)(cid:3)(cid:21)(cid:7)(cid:14)$(cid:21)(cid:9)(cid:10)(cid:10)(cid:2)(cid:4)(cid:14)
Figure 1.3 N-channel sound mixing console.
trafﬁc information is added. A multicarrier technique is applied for digital transmission
to stationary and mobile receivers. At the transmitter, several broadcasting programs are
combined in a multiplexer (MUX) to form a multiplex signal. The channel coding and
4
Introduction
modulation is carried out by a multi-carrier transmission technique (Coded Orthogonal
Frequency Division Multiplex, [Ala87, Kam92, Kam93, Tui93]).
(cid:26)(cid:16)(cid:3)(cid:16)(cid:7)(cid:9)(cid:4)
(cid:27)(cid:10)(cid:28)(cid:13)(cid:7)(cid:14)(cid:10)
(cid:30))
(cid:26)*(cid:24)
(cid:26)(cid:30)(cid:15)
#&(cid:27)(cid:24)
(cid:1)&(cid:24)
(cid:15)
(cid:19)
(cid:27)(cid:10)(cid:18)(cid:2)(cid:8)(cid:7)
(cid:27)(cid:10)(cid:18)(cid:2)(cid:8)(cid:7)
((cid:13)(cid:7)(cid:28)(cid:13)(cid:7)
((cid:13)(cid:7)(cid:28)(cid:13)(cid:7)
Figure 1.4 Sound channel.
,(cid:8)(cid:11)(cid:9)-(cid:20)(cid:9)(cid:18)(cid:7)(cid:14)(cid:18)(cid:7)(cid:13)-(cid:16)(cid:11)
(cid:26)(cid:16)(cid:18)(cid:7)(cid:8)(cid:16).(cid:13)(cid:7)(cid:16)(cid:11)(cid:10)
+(cid:8)(cid:9)(cid:10)(cid:18)(cid:12)(cid:16)(cid:7)(cid:7)(cid:2)(cid:8)
&(cid:30)(cid:6)1(cid:30),!
2(cid:29)34(cid:23)(cid:14)/.(cid:16)(cid:7)1(cid:18)
(cid:6)$
(cid:27)(cid:6)(1/(cid:1)(cid:30)#(cid:22)
(cid:6)(cid:7)(cid:8)(cid:2)(cid:9)(cid:12)
&(cid:27)
/!0
/((cid:26)
(cid:1)(cid:8)(cid:11)(cid:3)(cid:8)(cid:9)(cid:12)(cid:14)$(cid:11)(cid:10)(cid:7)(cid:2)(cid:10)(cid:7)(cid:18)
+(cid:8)(cid:9)%%(cid:16)(cid:20)(cid:14)(cid:27)(cid:10)%(cid:11)(cid:8)(cid:12)(cid:9)(cid:7)(cid:16)(cid:11)(cid:10)
(cid:27)(cid:10)%(cid:11)(cid:8)(cid:12)(cid:9)(cid:7)(cid:16)(cid:11)(cid:10)
Figure 1.5 DAB transmitter.
The DAB receiver (Fig. 1.6) consists of the demodulator (DMOD), the demultiplexer
(DMUX) and the source decoder (SD). The SD provides a linearly quantized PCM signal
(Pulse Code Modulation). The PCM signal is fed over a Digital-to-Analog Converter (DA
Converter) to an ampliﬁer connected to loudspeakers.
(cid:26)/((cid:26)
(cid:26)/!0
&(cid:27)
(cid:6)(cid:26)
(cid:26)&$
&/(cid:1)
Figure 1.6 DAB receiver.
For a more detailed description of the DAB transmission technique, an illustration
based on ﬁlter banks is presented (see Fig. 1.7). The audio signal at a data rate of 768 kbit/s
is decomposed into subbands with the help of an analysis ﬁlter bank (AFB). Quantization
and coding based on psychoacoustic models are carried out within each subband. The data
reduction leads to a data rate of 96–192 kbit/s. The quantized subband signals are provided
with additional information (header) and combined together in a frame. This so-called ISO-
MPEG1 frame [ISO92] is ﬁrst subjected to channel coding (CC). Time-interleaving (TIL)
follows and will be described later on. The individual transmitting programs are combined
in frequency multiplex (frequency-interleaving FIL) with a synthesis ﬁlter bank (SFB) into
1.2 Digital Transmission Systems
5
one broad-band transmitting signal. The synthesis ﬁlter bank has several complex-valued
input signals and one complex-valued output signal. The real-valued band-pass signal is
obtained by modulating with ej ωct and taking the real part. At the receiver, the complex-
valued base-band signal is obtained by demodulation followed by low-pass ﬁltering.
The complex-valued analysis ﬁlter bank provides the complex-valued band-pass signals
from which the ISO-MPEG1 frame is formed after frequency and time deinterleaving
and channel decoding. The PCM signal is combined using the synthesis ﬁlter bank after
extracting the subband signals from the frame.
ω
ω
ω
ω
− ω
+ ω
ω
ω
ω
Figure 1.7 Filter banks within DAB.
DAB Transmission Technique. The special problems of mobile communications are dealt
with using a combination of the OFDM transmission technique with DPSK modulation
and time and frequency interleaving. Possible disturbances are minimized by consecutive
channel coding. The schematic diagram in Fig. 1.8 shows the relevant subsystems.
For example, the transmission of a program P1 which is delivered as an ISO-MPEG1
stream is shown in Fig. 1.8. The channel coding doubles the data rate. The typical
characteristics of a mobile communication channel like time and frequency selectivity are
handled by using time and frequency interleaving with the help of a multicarrier technique.
6
Introduction
Figure 1.8 DAB transmission technique.
The burst disturbances of consecutive bits are reduced to single bit errors by spreading
the bits over a longer period of time. The narrow-band disturbances affect only individual
carriers by spreading the transmitter program P1 in the frequency domain, i.e. distribution
of transmitter programs of carrier frequencies at a certain displacement. The remaining
disturbances of the mobile channel are suppressed with the help of channel coding, i.e. by
adding redundancy, and decoding with a Viterbi decoder. The implementation of an OFDM
transmission is discussed in the following.
1.2 Digital Transmission Systems
7
OFDM Transmission. The OFDM transmission technique is shown in Fig. 1.9. The
technique stands out owing to its simple implementation in the digital domain. The data
sequence ct (k) which is to be transmitted, is written blockwise into a register of length 2M.
The complex numbers from d1(m) to dM (m) are formed from two consecutive bits (dibits).
Here the ﬁrst bit corresponds to the real part and the second to the imaginary part. The
signal space shows the four states for the so-called QPSK [Kam92a, Pro89]. The vector
d(m) is transformed with an inverse FFT (Fast Fourier Transform) into a vector e(m)
which describes the values of the transmitted symbol in the time domain. The transmitted
symbol xt (n) with period Tsym is formed by the transmission of the M complex numbers
ei (m) at sampling period TS. The real-valued band-pass signal is formed at high frequency
after DA conversion of the quadrature signals, modulation by ej ωct and by taking the real
part. At the receiver, the transmitted symbol becomes a complex-valued sequence xr (n)
by demodulation with e−j ωct and AD conversion of the quadrature signal. M samples of
the received sequence xr (n) are distributed over the M input values fi (m) and transformed
into the frequency domain with the help of FFT. The resulting complex numbers gi(m) are
again converted to dibits and provide the received sequence cr (k). Without the inﬂuence of
the communication channel, the transmitted sequence can be reconstructed exactly.
OFDM Transmission with a Guard Interval. In order to describe the OFDM trans-
mission with a guard interval, the schematic diagram in Fig. 1.10 is considered. The
transmission of a symbol of length M over a channel with impulse response h(n) of length
L leads to a received signal y(n) of length M + L − 1. This means that the received
symbol is longer than the transmitted signal. The exact reconstruction of the transmitted
symbol is disturbed because of the overlapping of received symbols. Reconstruction of
the transmitted symbol is possible by cyclic continuation of the transmitted symbol. Here,
the complex numbers from the vector e(m) are repeated so as to give a symbol period of
Tsym = (M + L)TS. Each of the transmitted symbols is, therefore, extended to a length
of M + L. After transmission over a channel with impulse response of length L, the
response of the channel is periodic with length M. After the initial transient state of the
channel, i.e. after the L samples of the guard interval, the following M samples are written
into a register. Since a time delay occurs between the start of the transmitted symbol
and the sampling shifted by L displacements, it is necessary to shift the sequence of
length M cyclically by L displacements. The complex values gi (m) do not correspond
to the exact transmitted values di(m) because of the transmission channel h(n). However,
there is no inﬂuence of neighboring carrier frequencies. Every received value gi(m) is
weighted with the corresponding magnitude and phase of the channel at the speciﬁc
carrier frequency. The inﬂuence of the communication channel can be eliminated by
differential coding of consecutive dibits. The decoding process can be done according to
zi (m) = gi (m)g∗
i (m − 1). The dibit corresponds to the sign of the real and imaginary parts.
The DAB transmission technique presented stands out owing to its simple implementation
with the help of FFT algorithms. The extension of the transmitted symbol by a length L
of the channel impulse response and the synchronization to collect the M samples out of
the received symbol have still to be carried out. The length of the guard interval must be
matched to the maximum echo delay of the multipath channel. Owing to differential coding
of the transmitted sequence, an equalizer at the receiver is not necessary.
8
t
Introduction
Figure 1.9 OFDM transmission.
Digital Radio Mondiale (DRM)
In the interest of national and international broadcasting stations a more widespread
program delivery across regional or worldwide regions is of speciﬁc importance. This
is accomplished by analog radio transmission in the frequency range below 30 MHz.
The limited audio quality of the amplitude modulation technique (channel bandwidth 9–
10 kHz) with an audio bandwidth of 4.5 kHz leads to a low acceptance rate for such kind
of audio broadcasting. The introduction of the digital transmission technique Digital Radio
Mondiale3 will replace the existing analog transmission systems. The digital transmission
is based on OFDM and the audio coding MPEG4-AAC in combination with SBR (Spectral
Band Replication).
3http://www.drm.org.
1.2 Digital Transmission Systems
9
Figure 1.10 OFDM transmission with a guard interval.
Internet Audio
The growth of the internet offers new distribution possibilities for information, but
especially for audio and video signals. The distribution of audio signals is mainly driven by
the MP3 format (MPEG-1 Audio Layer III [Bra94]) or in proprietary formats of different
companies. The compressed transmission is used because the data rate of home users is
10
Introduction
still low compared to lossless audio and video formats. Since the transmission is based on
ﬁle transfer of packets, the data rates strongly depend on the providing server, the actual
internet trafﬁc and the access point of the home computer. A real-time transfer of high-
quality music is still not possible. If the audio compression is high enough to achieve a just
acceptable audio quality, a real-time transfer with a streaming technology is possible, since
the ﬁle size is small and a transmission needs less time (see Fig. 1.11). For this a receiver
needs a double memory ﬁlled with incoming packets of a coded audio ﬁle and a parallel
running audio decoding. After decoding of a memory with a sufﬁciently long audio portion
the memory is transferred to the sound card of the computer. During sound playback of
the decoded audio signal further incoming packets are received and decoded. Packet loss
can lead to interrupts in the audio signal. Several techniques for error concealment and
protocols allow the transfer of coded audio.
Audio
Audio
compression
WEB Server
Internet
Intranet
WEB-
Browser
Media-Player
Figure 1.11 Audio streaming via the internet.
1.3 Storage Media
Compact Disc
The technological advances in the semiconductor industry have led to economical storage
media for digitally encoded information. Independently of developments in the computer
industry, the compact disc system was introduced by Philips and Sony in 1982. The storage
of digital audio data is carried out on an optical storage medium. The compact disc operates
at a sampling rate of fS = 44.1 kHz.4 The essential speciﬁcations are summarized in
Table 1.1.
R-DAT (Rotary-head Digital Audio on Tape)
The R-DAT system makes use of the heliscan method for two-channel recording. The
available devices enable the recording of 16-bit PCM signals with all three sampling rates
(Table 1.2) on a tape. R-DAT recorders are used in studio recording as well as in consumer
applications.
MiniDisc and MP3 Format
Advanced coding techniques are based on psychoacoustics for data reduction. A widespread
storage system is the MiniDisc by Sony. The Mini Disc system operates with the ATRAC
technique (Adaptive Transform Acoustic Coding, [Tsu92]) and has a data rate of about
43 × 490 × 30 Hz (NTSC) = 3 × 588 × 25 Hz (CCIR) = 44.1 kHz.
1.3 Storage Media
11
Table 1.1 Speciﬁcations of the CD system [Ben88].
Type of recording
Signal recognition
Storage density
Audio speciﬁcation
Number of channels
Duration
Frequency range
Dynamic range
THD
Signal format
Sampling rate
Quantization
Pre-emphasis
Error Correction
Data rate
Modulation
Channel bit rate
Redundancy
Optical
682 Mbit/in2
2
Approx. 60 min.
20–20 000 Hz
>90 dB
<0.01%
44.1 kHz
16-bit PCM (2’s complement)
None or 50/15 µs
CIRC
2.034 Mbit/s
EFM
4.3218 Mbit/s
30%
Mechanical speciﬁcation
Diameter
Thickness
Diameter of the inner hole
Program range
Reading speed
120 mm
1.2 mm
15 mm
50–116 mm
1.2–1.4 m/s
500–200 r/min.
2 · 140 kbit/s for a stereo channel. A magneto-optical storage medium is used for recording.
The MP3 format was developed simultaneously, but the availability of recording and
playback systems has taken a longer time. Simple MP3 recorders and playback systems
are now available for the consumer market.
Super Audio Compact Disc (SACD)
The SACD was speciﬁed by Philips and Sony in 1999 as a further development of the
compact disc with the objective of improved sound quality. The audio frequency range of
20 kHz is perceived as a limiting audio quality factor by some human beings, and the anti-
aliasing and reconstruction ﬁlters may lead to ringing resulting from linear phase ﬁlters.
This effect follows from short audio pulses leading to audible transients of the ﬁlters.
In order to overcome these problems the audio bandwidth is extended to 100 kHz and
the sampling frequency is increased to 2.8224 MHz (64 × 44.1 kHz). With this the ﬁlter
speciﬁcations can be met with simple ﬁrst-order ﬁlters. The quantization of the samples is
based on a 1-bit quantizer within a delta-sigma converter structure which uses noise shaping
12
Introduction
Table 1.2 Speciﬁcations of the R-DAT system [Ben88].
Type of recording
Signal recognition
Storage capacity
Audio speciﬁcation
Number of channels
Duration
Frequency range
Dynamic range
THD
Signal format
Sampling rate
Quantization
Error correction
Channel coding
Data rate
Channel bit rate
Magnetic
2 GB
2
Max. 120 min.
20–20 000 Hz
>90 dB
<0.01%
48, 44.1, 32 kHz
16-bit PCM (2’s complement)
CIRC
8/10 modulation
2.46 Mbit/s
9.4 Mbit/s
Mechanical speciﬁcation
Tapewidth of magnet
Thickness
Diameter of head drum 3 cm
Revolutions per min.
Rel. track speed
3.8 mm
13 µm
2000 r/min.
3.133 m/s
500–200 r/min.
(see Fig. 1.12). The 1-bit signal with 2.8224 MHz sampling frequency is denoted a DSD
signal (Direct Stream Digital). The DA conversion of a DSD signal into an analog signal
is accomplished with a simple analog ﬁrst-order low-pass. The storage of DSD signals is
achieved by a special compact disc (Fig. 1.13) with a CD layer in PCM format and an HD
layer (High Density) with a DVD 4.38 GByte layer. The HD layer stores a stereo signal
in 1-bit DSD format and a 6-channel 1-bit signal with a lossless compression technique
(Direct Stream Transfer DST) [Jan03]. The CD layer of the SACD can be replayed with
a conventional CD player, whereas special SACD players can replay the HD layer. An
extensive discussion of 1-bit delta-sigma techniques can be found in [Lip01a, Lip01b,
Van01, Lip02, Van04].
x(t)
Analog-
DSD
1-bi t
64.fS
1-bi t
DSD
Memory
1-bi t
64.fS
DSD-
Analog
y(t)
Figure 1.12 SACD system.
1.4 Audio Components at Home
13
Protection layer
CD Layer
Plastic
HD Layer
Plastic
Laser Scanner
Figure 1.13 Layer of the SACD.
Digital Versatile Disc – Audio (DVD-A)
To increase the storage capacity of the CD the Digital Versatile Disc (DVD) was developed.
The physical dimensions are identical to the CD. The DVD has two layers with one or
two sides, and the storage capacity per side has been increased. For a one-sided version
for audio applications the storage capacity is 4.7 GB. A comparison of speciﬁcations for
different disc media is shown in Table 1.3. Besides stereo signals with different sampling
rates and word-lengths a variety of multi-channel formats can be stored. For data reduction
a lossless compression technique, MLP (Meridian Lossless Packing), is applied. The
improved audio quality compared to the CD audio is based on the higher sampling rates
and word-lengths and the multichannel features of the DVD-A.
Table 1.3 Speciﬁcations of CD, SACD and DVD-A.
Parameter
CD
SACD
DVD-A
Coding
Sampling rate
Channels
Compression
Recording time
Frequency range
Dynamic range
Copy protection
16-bit PCM
44.1 kHz
2
No
74 min.
20–20 000 Hz
96 dB
No
1-bit DSD
2.8224 MHz
2–6
Yes (DST)
70–80 min.
20–100 000 Hz
120 dB
Yes
16-/20-/24-bit PCM
44.1/48/88.2/96/176.4/192 kHz
1–6
Yes (MLP)
62–843 min.
20–96 000 Hz
144 dB
Yes
1.4 Audio Components at Home
Domestic digital storage media are already in use, like compact discs, personal computers
and MP3 players, which have digital outputs, and can be connected to digital post-
processing systems right up to the loudspeakers. The individual tone control consists of
the following processing.
14
Equalizer
Introduction
Spectral modiﬁcation of the music signal in amplitude and phase and the automatic
correction of the frequency response from loudspeaker to listening environment are desired.
Room Simulation
The simulation of room impulse responses and the processing of music signals with special
room impulse response are used to give an impression of a room like a concert hall, a
cathedral or a jazz club.
Surround Systems
Besides the reproduction of stereo signals from a CD over two frontal loudspeakers, more
than two channels will be recorded in the prospective digital recording systems [Lin93].
This is already illustrated in the sound production for cinema movies where, besides the
stereo signal (L, R), a middle channel (M) and two additional room signals (LB, RB )
are recorded. These surround systems are also used in the prospective digital television
systems. The ambisonics technique [Ger85] is a recording technique that allows three-
dimensional recording and reproduction of sound.
Digital Ampliﬁer Concepts
The basis of a digital ampliﬁer is pulse width modulation as shown in Fig. 1.14. With the
help of a fast counter, a pulse width modulated signal is formed out of the w-bit linearly
quantized signal. Single-sided and double-sided modulated conversion are used and they
are represented by two and three states, respectively. Single-sided modulation (2 states, −1
and +1) is performed by a counter which counts upward from zero with multiples of the
sampling rate. The number range of the PCM signal from −1 to+ 1 is directly mapped
onto the counter. The duration of the pulse width is controlled by a comparator. For pulse
width modulation with three states (−1, 0, +1), the sign of the PCM signal determines the
state. The pulse width is determined by a mapping of the number range from 0 to 1 onto
a counter. For double-sided modulation, an upward/downward counter is needed which
has to be clocked at twice the rate compared with single-sided modulation. The allocation
of pulse widths is shown in Fig. 1.14. In order to reduce the clock rate for the counter,
pulse width modulation is carried out after oversampling (Oversampling) and noise shaping
(Noise Shaping) of the quantization error (see Fig. 1.15, [Gol90]). Thus the clock rate of
the counter is reduced to 180.6 MHz. The input signal is ﬁrst upsampled by a factor of
16 and then quantized to 8-bits with third-order noise shaping. The use of pulse shaping
with delta-sigma modulation is shown in Fig. 1.16 [And92]. Here a direct conversion of the
delta-sigma modulated 1-bit signal is performed. The pulse converter shapes the envelope
of the serial data bits. The low-pass ﬁlter reconstructs the analog signal. In order to reduce
nonlinear distortion, the output signal is fed back (see Fig. 1.17, [Klu92]). New methods
for the generation of pulse width modulation try to reduce the clock rates and the high
frequency components [Str99, Str01].
1.4 Audio Components at Home
15
3(cid:29)(cid:23)5
3
(cid:5)3(cid:29)5
(cid:5)(cid:22)
3(cid:29)5
3(cid:29)(cid:23)5
3
(cid:5)3(cid:29)5
(cid:5)(cid:22)
3(cid:29)5
(cid:22)(cid:29)3
(cid:5)(cid:22)(cid:29)3
+
(cid:23);
(cid:7)
(cid:8)
(cid:2)
(cid:10)
(cid:13)
(cid:11)
$
3
(cid:22)
3(cid:29)5
3
(cid:5)3(cid:29)5
(cid:5)(cid:22)
(cid:7)
(cid:16)
.
-
;
+
(cid:8)
(cid:2)
.
(cid:12)
(cid:13)
(cid:10)
(cid:14)
(cid:7)
(cid:13)
(cid:28)
(cid:10)
(cid:16)
(cid:22)(cid:29)3
(cid:5)(cid:22)(cid:29)3
+
(cid:23);(cid:5)(cid:22)
(cid:7)
(cid:8)
(cid:2)
(cid:10)
(cid:13)
(cid:11)
$
3
(cid:22)
3(cid:29)5
(cid:7)
(cid:16)
.
-
;
3
+
(cid:8)
(cid:2)
.
(cid:12)
(cid:13)
(cid:10)
(cid:14)
(cid:7)
(cid:13)
(cid:28)
(cid:10)
(cid:16)
(cid:18)(cid:16)(cid:10)(cid:3)(cid:4)(cid:2)(cid:5)(cid:18)(cid:16)-(cid:2)-(cid:14)(cid:1)6/(cid:14)7(cid:23)(cid:14)(cid:18)(cid:7)(cid:9)(cid:7)(cid:2)(cid:18)8(cid:14)9(cid:22)8(cid:5)(cid:22):
(cid:18)(cid:16)(cid:10)(cid:3)(cid:4)(cid:2)(cid:5)(cid:18)(cid:16)-(cid:2)-(cid:14)(cid:1)6/(cid:14)72(cid:14)(cid:18)(cid:7)(cid:9)(cid:7)(cid:2)(cid:18)8(cid:14)9(cid:22)838(cid:5)(cid:22):
3(cid:29)(cid:23)5
3
(cid:5)3(cid:29)5
(cid:5)(cid:22)
3(cid:29)5
3(cid:29)(cid:23)5
3
(cid:5)3(cid:29)5
(cid:5)(cid:22)
3(cid:29)5
(cid:22)(cid:29)3
(cid:5)(cid:22)(cid:29)3
+
(cid:23);
(cid:7)
(cid:8)
(cid:2)
(cid:10)
(cid:13)
(cid:11)
$
3
(cid:22)
3(cid:29)5
3
(cid:5)3(cid:29)5
(cid:5)(cid:22)
(cid:7)
(cid:16)
.
-
;
+
(cid:8)
(cid:2)
.
(cid:12)
(cid:13)
(cid:10)
(cid:14)
(cid:7)
(cid:13)
(cid:28)
(cid:10)
(cid:16)
(cid:22)(cid:29)3
(cid:5)(cid:22)(cid:29)3
+
(cid:23) ;(cid:5)(cid:22)
(cid:7)
(cid:8)
(cid:2)
(cid:10)
(cid:13)
(cid:11)
$
3
(cid:22)
3(cid:29)5
(cid:7)
(cid:16)
.
-
;
3
+
(cid:8)
(cid:2)
.
(cid:12)
(cid:13)
(cid:10)
(cid:14)
(cid:7)
(cid:13)
(cid:28)
(cid:10)
(cid:16)
-(cid:11)(cid:13).(cid:4)(cid:2)(cid:5)(cid:18)(cid:16)-(cid:2)-(cid:14)(cid:1)6/(cid:14)7(cid:23)(cid:14)(cid:18)(cid:7)(cid:9)(cid:7)(cid:2)(cid:18)8(cid:14)9(cid:22)8(cid:5)(cid:22):
-(cid:11)(cid:13).(cid:4)(cid:2)(cid:5)(cid:18)(cid:16)-(cid:2)-(cid:14)(cid:1)6/(cid:14)72(cid:14)(cid:18)(cid:7)(cid:9)(cid:7)(cid:2)(cid:18)8(cid:14)9(cid:22)838(cid:5)(cid:22):
Figure 1.14 Pulse width modulation.
(cid:22)?3(cid:29)>(cid:14)/=
’7(cid:10):
<<(cid:29)(cid:22)(cid:14)(cid:17)=
(cid:22)>-.(cid:16)(cid:7)
((cid:6)1(cid:24)(cid:6)
435(cid:29)>(cid:14)(cid:17)=
?-.(cid:16)(cid:7)
(cid:1)6/
(cid:1)6/
&(cid:12)(cid:28)(cid:4)(cid:16)%(cid:16)(cid:2)(cid:8)
&(cid:10)(cid:9)(cid:4)(cid:11)(cid:3)
(cid:15)(cid:11);(cid:5)(cid:28)(cid:9)(cid:18)(cid:18)
"7(cid:7):
Figure 1.15 Pulse width modulation with oversampling and noise shaping.
16
Introduction
(cid:22)-.(cid:16)(cid:7)(cid:14)(cid:26)&$
’7(cid:10):
<<(cid:29)(cid:22)(cid:14)(cid:17)=
(cid:22)>-.(cid:16)(cid:7)
((cid:6)1(cid:24)(cid:6)
(cid:22)(cid:22)(cid:29)2(cid:14)/=
(cid:22)-.(cid:16)(cid:7)
&(cid:10)(cid:9)(cid:4)(cid:11)(cid:3)
(cid:15)(cid:11);(cid:5)(cid:28)(cid:9)(cid:18)(cid:18)
"7(cid:7):
=@(cid:14)$(cid:4)(cid:11)(cid:20)(cid:17)
(cid:22)-.(cid:16)(cid:7)
(cid:1)(cid:13)(cid:4)(cid:18)(cid:2)
=@(cid:14)$(cid:4)(cid:11)(cid:20)(cid:17)
$(cid:11)(cid:10)A(cid:2)(cid:8)(cid:7)(cid:2)(cid:8) (cid:22)-.(cid:16)(cid:7)(cid:14)(cid:1)(cid:13)(cid:4)(cid:18)(cid:2)
&(cid:10)(cid:9)(cid:4)(cid:11)(cid:3)
(cid:15)(cid:11);(cid:5)(cid:28)(cid:9)(cid:18)(cid:18)
"7(cid:7):
=@(cid:14)$(cid:4)(cid:11)(cid:20)(cid:17)
(cid:22)-.(cid:16)(cid:7)(cid:14)(cid:26)(cid:9)(cid:7)(cid:9)
(cid:22)
(cid:22)
3
3
(cid:22)
3
3
(cid:22)-.(cid:16)(cid:7)(cid:14)(cid:1)(cid:13)(cid:4)(cid:18)(cid:2)
"7(cid:7):
Figure 1.16 Pulse shaping after delta-sigma modulation.
% (cid:6)
’7(cid:10):
=7 :
&(cid:12)(cid:28)(cid:4)(cid:16)%(cid:16)(cid:2)(cid:8)
&(cid:10)(cid:9)(cid:4)(cid:11)(cid:3)
(cid:15)(cid:11);(cid:5)(cid:28)(cid:9)(cid:18)(cid:18)
"7(cid:7):
&(cid:26)$
% (cid:6)
Figure 1.17 Delta-sigma modulated ampliﬁer with feedback.
Digital Crossover
In order to perform digital crossovers for loudspeakers, a linear phase decomposition of the
signal with a special ﬁlter bank [Zöl92] is done (Fig. 1.18). In a ﬁrst step, the input signal
is decomposed into its high-pass and low-pass components and the high-pass signal is fed
to a DAC over a delay unit. In the next step, the low-pass signal is further decomposed.
The individual band-pass signals and the low-pass signal are then fed to the respective
loudspeaker. Further developments for the control of loudspeakers can be found in [Kli94,
Kli98a, Kli98b, Mül99].
References
17
+$(cid:22)
+$(cid:23)
+$2
+$(cid:24)
’
7(cid:10) :(cid:22)
%(cid:6)(cid:22)
@(cid:6)(cid:22)
%(cid:6)(cid:22)
%(cid:6)(cid:22)
@(cid:6)(cid:23)
%(cid:6)(cid:23)
%(cid:6)(cid:23)
@(cid:6)2
%
(cid:6)2
%(cid:6)2
%(cid:6)<
% (cid:6)(cid:24)
@(cid:6)(cid:24)
%(cid:6)(cid:24)
(cid:26)(cid:30)(cid:15)(cid:22)
(cid:26)(cid:30)(cid:15)(cid:23)
(cid:26)(cid:30)(cid:15)2
(cid:26)(cid:30)(cid:15)(cid:24)
%(cid:6)(cid:23)
%(cid:6)2
" 7(cid:10)(cid:22)
:(cid:22)
" 7(cid:10) :
(cid:23)
(cid:23)
" 7(cid:10) :
2
2
=(cid:16)(cid:3)(cid:21)(cid:5)(cid:28)(cid:9)(cid:18)(cid:18)
(cid:14),(cid:9)(cid:10)-(cid:5)(cid:28)(cid:9)(cid:18)(cid:18)(cid:14)(cid:22)
,(cid:9)(cid:10)-(cid:5)(cid:28)(cid:9)(cid:18)(cid:18)(cid:14)(cid:23)
%(cid:6)(cid:24)
" 7(cid:10) :(cid:24)(cid:24)
(cid:15)(cid:11);(cid:5)(cid:28)(cid:9)(cid:18)(cid:18)
Figure 1.18 Digital crossover (FSi frequency splitting, TCi transition bandwidth control, DELi
delay).
References
[AES91] AES10-1991 (ANSI S4.43-1991): AES Recommended Practice for Digital
Audio Engineering – Serial Multichannel Audio Digital Interface (MADI).
[AES92] AES3-1992 (ANSI S4.40-1992): AES Recommended Practice for Digital
Audio Engineering – Serial Transmission Format for Two-Channel Linearly
Represented Digital Audio.
[Ala87] M. Alard, R. Lasalle: Principles of Modulation and Channel Coding for Digital
Broadcasting for Mobile Receivers, EBU Review, No. 224, pp. 168–190,
August 1987.
[And92] M. Andersen: New Principle for Digital Audio Power Ampliﬁers, Proc. 92nd
AES Convention, Preprint No. 3226, Vienna, 1992.
[Ben88] K. B. Benson: Audio Engineering Handbook, McGraw-Hill, New York, 1988.
[Bra94]
K. Brandenburg, G. Stoll: ISO/MPEG-1 Audio: A Generic Standard for Coding
of High Quality Digital Audio, J. Audio Eng. Soc., Vol. 42, No. 10, pp. 780–
792, October 1994.
[Ger85] M. A. Gerzon: Ambisonics in Multichannel Broadcasting and Video, J. Audio
Eng. Soc., Vol. 33, No. 11, pp. 859–871, November 1985.
[Gol90]
J. M. Goldberg, M. B. Sandler: New Results in PWM for Digital Power
Ampliﬁcation, Proc. 89th AES Convention, Preprint No. 2959, Los Angeles,
1990.
[Hoe01] W. Hoeg, T. Lauterbach: Digital Audio Broadcasting, John Wiley & Sons Ltd,
Chichester, 2001.
18
Introduction
[ISO92]
ISO/IEC 11172-3: Coding of Moving Pictures and Associated Audio for Digital
Storage Media at up to 1.5 Mbits/s – Audio Part, International Standard, 1992.
[Jan03]
E. Janssen, D. Reefman: Super Audio CD: An Introduction, IEEE Signal
Processing Magazine, pp. 83–90, July 2003.
[Kam92] K. D. Kammeyer, U. Tuisel, H. Schulze, H. Bochmann: Digital Multicarrier-
Transmission of Audio Signals over Mobile Radio Channels, Europ. Trans. on
Telecommun. ETT, Vol. 3, pp. 243–254, May–June 1992.
[Kam93] K. D. Kammeyer, U. Tuisel: Synchronisationsprobleme in digitalen Multi-
trägersystemen, Frequenz, Vol. 47, pp. 159–166, May 1993.
[Kli94] W. Klippel: Das nichtlineare Übertragungsverhalten elektroakustischer Wand-
ler, Habilitationsschrift, Technische Universität Dresden, 1994.
[Kli98a] W. Klippel: Direct Feedback Linearization of Nonlinear Loudspeaker Systems,
J. Audio Eng. Soc., Vol. 46, No. 6, pp. 499–507, 1998.
[Kli98b] W. Klippel: Adaptive Nonlinear Control of Loudspeaker Systems, J. Audio Eng.
Soc., Vol. 46, No. 11, pp. 939–954, 1998.
[Klu92]
J. Klugbauer-Heilmeier: A Sigma Delta Modulated Switching Power Amp,
Proc. 92nd AES Convention, Preprint No. 3227, Vienna, 1992.
[Lec92]
D. Leckschat: Verbesserung der Wiedergabequalität von Lautsprechern mit
Hilfe von Digitalﬁltern, Dissertation, RWTH Aachen, 1992.
[Lin93]
B. Link, D. Mandell: A DSP Implementation of a Pro Logic Surround Decoder,
Proc. 95th AES Convention, Preprint No. 3758, New York, 1993.
[Lip01a]
S. P. Lipshitz, J. Vanderkooy: Why 1-Bit Sigma-Delta Conversion is
Unsuitable for High-Quality Applications, Proc. 110th Convention of the
Audio Engineering Society, Preprint No. 5395, Amsterdam, 2001.
[Lip01b]
S. P. Lipshitz. J. Vanderkooy: Towards a Better Understanding of 1-Bit Sigma-
Delta Modulators – Part 2, Proc. 111th Convention of the Audio Engineering
Society, Preprint No. 5477, New York, 2001
[Lip02]
S. P. Lipshitz, J. Vanderkooy: Towards a Better Understanding of 1-Bit Sigma-
Delta Modulators – Part 3, Proc. 112th Convention of the Audio Engineering
Society, Preprint No. 5620, Munich, 2002.
[Mül99]
S. Müller: Digitale Signalverarbeitung für Lautsprecher, Dissertation, RWTH
Aachen, 1999.
[Ple91]
G. Plenge: DAB – Ein neues Hörrundfunksystem – Stand der Entwicklung und
Wege zu seiner Einführung, Rundfunktechnische Mitteilungen, Vol. 35, No. 2,
pp. 46–66, 1991.
References
[Str99]
[Str01]
[Tsu92]
[Tui93]
[Van01]
[Van04]
[Zöl92]
19
M. Streitenberger, H. Bresch, W. Mathis: A New Concept for High Performance
Class-D Audio Ampliﬁcation, Proc. AES 106th Convention, Preprint No. 4941,
Munich, 1999.
M. Streitenberger, F. Felgenhauer, H. Bresch, W. Mathis: Zero Position Coding
(ZePoC) – A Generalised Concept of Pulse-Length Modulated Signals and its
Application to Class-D Audio Power Ampliﬁers, Proc. AES 110th Convention,
Preprint No. 5365, Amsterdam, 2001.
K. Tsutsui, H. Suzuki, O. Shimoyoshi, M. Sonohara, K. Akagiri, R. Heddle:
ATRAC: Adaptive Transform Coding for MiniDisc, Proc. 91st AES Convention,
Preprint No. 3216, New York, 1991.
U. Tuisel: Multiträgerkonzepte für die digitale,
funkübertragung, Dissertation, TU Hamburg-Harburg, 1993.
terrestrische Hörrund-
J. Vanderkooy, S. P. Lipshitz: Towards a Better Understanding of 1-Bit Sigma-
Delta Modulators – Part 1, Proc. 110th Convention of the Audio Engineering
Society, Preprint No. 5398, Amsterdam, 2001.
J. Vanderkooy, S. P. Lipshitz: Towards a Better Understanding of 1-Bit Sigma-
Delta Modulators – Part 4, Proc. 116th Convention of the Audio Engineering
Society, Preprint No. 6093, Berlin, 2004.
U. Zölzer, N. Fliege: Logarithmic Spaced Analysis Filter Bank for Multiple
Loudspeaker Channels, Proc. 93rd AES Convention, Preprint No. 3453, San
Francisco, 1992.

Chapter 2
Quantization
Basic operations for AD conversion of a continuous-time signal x(t) are the sampling
and quantization of x(n) yielding the quantized sequence xQ(n) (see Fig. 2.1). Before
discussing AD/DA conversion techniques and the choice of the sampling frequency fS =
1/TS in Chapter 3 we will introduce the quantization of the samples x(n) with ﬁnite
number of bits. The digitization of a sampled signal with continuous amplitude is called
quantization. The effects of quantization starting with the classical quantization model are
discussed in Section 2.1. In Section 2.2 dither techniques are presented which, for low-level
signals, linearize the process of quantization. In Section 2.3 spectral shaping of quantization
errors is described. Section 2.4 deals with number representation for digital audio signals
and their effects on algorithms.
Sampling
fS
x(t)
Analog
Low-pass
x(n)
x (n)Q
Quantizer
Figure 2.1 AD conversion and quantization.
2.1 Signal Quantization
2.1.1 Classical Quantization Model
Quantization is described by Widrow’s quantization theorem [Wid61]. This says that a
quantizer can be modeled (see Fig. 2.2) as the addition of a uniform distributed random
signal e(n) to the original signal x(n) (see Fig. 2.2, [Wid61]). This additive model,
xQ(n) = x(n) + e(n),
is based on the difference between quantized output and input according to the error signal
e(n) = xQ(n) − x(n).
(2.2)
(2.1)
Digital Audio Signal Processing Second Edition Udo Zölzer
© 2008 John Wiley & Sons, Ltd
22
Quantization
x(n)
Q
x (n)Q
x(n)
Q
x (n)Q
x(n)
x (n)=x(n)+e(n)
Q
e(n)
_
e(n)
Figure 2.2 Quantization.
This linear model of the output xQ(n) is only then valid when the input amplitude has a
wide dynamic range and the quantization error e(n) is not correlated with the signal x(n).
Owing to the statistical independence of consecutive quantization errors the autocorrelation
of the error signal is given by rEE(m) = σ 2
· δ(m), yielding a power density spectrum
E
SEE(ej (cid:4)) = σ 2
E.
The nonlinear process of quantization is described by a nonlinear characteristic curve as
shown in Fig. 2.3a, where Q denotes the quantization step. The difference between output
and input of the quantizer provides the quantization error e(n) = xQ(n) − x(n), which is
shown in Fig. 2.3b. The uniform probability density function (PDF) of the quantization
error is given (see Fig. 2.3b) by
pE(e) = 1
Q
(cid:1)
rect
(cid:2)
.
e
Q
(2.3)
’
)
(cid:23))
)
(cid:5))
(cid:5)(cid:23))
(cid:2)
’
’
(cid:9):
.:
)
(cid:23)
)
(cid:23)
(cid:28)(cid:14)(cid:14)(cid:14)7(cid:2):
(cid:30)
(cid:22)
)
(cid:2)
)
(cid:23)
)
(cid:23)
Figure 2.3 (a) Nonlinear characteristic curve of a quantizer. (b) Quantization error e and its
probability density function (PDF) pE(e).
The mth moment of a random variable E with a PDF pE(e) is deﬁned as the expected
value of Em:
E{Em} =
(cid:3) ∞
−∞
empE(e) de.
(2.4)
2.1 Signal Quantization
23
For a uniform distributed random process, as in (2.3), the ﬁrst two moments are given by
The signal-to-noise ratio
mE = E{E} = 0 mean value
σ 2
E
= E{E2} = Q2
12
variance.
SNR = 10 log10
(cid:2)
(cid:1)
σ 2
X
σ 2
E
dB
(2.5)
(2.6)
(2.7)
is deﬁned as the ratio of signal power σ 2
X to error power σ 2
E.
For a quantizer with input range ±xmax and word-length w, the quantization step size
can be expressed as
By deﬁning a peak factor,
Q = 2xmax/2w.
PF = xmax
σX
= 2w−1Q
σX
,
the variances of the input and the quantization error can be written as
σ 2
X
σ 2
E
,
= x2
max
P 2
F
= Q2
12
= 1
12
x2
max
22w
22 = 1
3
x2
max2
−2w.
The signal-to-noise ratio is then given by
(cid:1)
SNR = 10 log10
max/P 2
x2
F
max2−2w
1
3 x2
= 6.02w − 10 log10(P 2
F /3) dB.
(cid:2)
= 10 log10
(cid:2)
(cid:1)
22w 3
P 2
F
A sinusoidal signal (PDF as in Fig. 2.4) with PF =
√
2 gives
For a signal with uniform PDF (see Fig. 2.4) and PF =
3 we can write
SNR = 6.02w + 1.76 dB.
√
SNR = 6.02w dB,
and for a Gaussian distributed signal (probability of overload <10−5 leads to PF = 4.61,
see Fig. 2.5), it follows that
SNR = 6.02w − 8.5 dB.
It is obvious that the signal-to-noise ratio depends on the PDF of the input. For digital
audio signals that exhibit nearly Gaussian distribution, the maximum signal-to-noise ratio
for given word-length w is 8.5 dB lower than the rule-of-thumb formula (2.14) for the
signal-to-noise ratio.
(2.8)
(2.9)
(2.10)
(2.11)
(2.12)
(2.13)
(2.14)
(2.15)
24
(cid:111)
)
x
(
X
p
2.5
2
1.5
1
0.5
0
(cid:86)X=xmax/(cid:151)2
-1
-0.5
0
x/xmax (cid:111)
0.5
1
(cid:111)
)
x
(
X
p
1
0.9
0.8
0.7
0.6
0.5
0.4
0.3
0.2
0.1
0
Quantization
(cid:86)X=xmax/(cid:151)3
-1
-0.5
0
x/xmax (cid:111)
0.5
1
Figure 2.4 Probability density function (sinusoidal signal and signal with uniform PDF).
(cid:111)
)
x
(
X
p
0.5
0.45
0.4
0.35
0.3
0.25
0.2
0.15
0.1
0.05
0
(cid:86)X=xmax/4.61
-1
-0.5
0
x/xmax (cid:111)
0.5
1
Figure 2.5 Probability density function (signal with Gaussian PDF).
2.1.2 Quantization Theorem
The statement of the quantization theorem for amplitude sampling (digitizing the ampli-
tude) of signals was given by Widrow [Wid61]. The analogy for digitizing the time axis is
the sampling theorem due to Shannon [Sha48]. Figure 2.6 shows the amplitude quantization
and the time quantization. First of all, the PDF of the output signal of a quantizer is
determined in terms of the PDF of the input signal. Both probability density functions
are shown in Fig. 2.7. The respective characteristic functions (Fourier transform of a PDF)
of the input and output signals form the basis for Widrow’s quantization theorem.
2.1 Signal Quantization
25
Discrete
amplitude
32767
32766
+1
x(t)
x(n)
n
o
i
t
a
z
i
t
n
a
u
q
e
d
u
t
i
l
p
m
A
1
0
-1
-32767
-32768
-1
x(8)
0
1
2
3
4
5
6
7
8
9 10 11
Time quantization
Figure 2.6 Amplitude and time quantization.
t
n
Discrete
time
x
x
+1
x(t)
x(n)
)
x
(
X
p
Q
x(8)
t
)
x
(
Q
X
p
-1
Figure 2.7 Probability density function of signal x(n) and quantized signal xQ(n).
First-order Statistics of the Quantizer Output
Quantization of a continuous-amplitude signal x with PDF pX(x) leads to a discrete-
amplitude signal y with PDF pY (y) (see Fig. 2.8). The continuous PDF of the input is
sampled by integrating over all quantization intervals (zone sampling). This leads to a
discrete PDF of the output.
In the quantization intervals, the discrete PDF of the output is determined by the
probability
W [kQ] = W
(cid:4)
− Q
2
+ kQ ≤ x <
(cid:5)
(cid:3)
+ kQ
=
Q
2
Q/2+kQ
−Q/2+kQ
pX(x) dx.
(2.16)
26
Quantization
’
0(cid:28)(cid:14)(cid:14)(cid:14)7’:
)
"
(cid:28)(cid:14)(cid:14)(cid:14)7":
*
0(cid:28)(cid:14)(cid:14)(cid:14)7’:
*(cid:28)(cid:14)(cid:14)(cid:14)7":
(cid:5)5)
(cid:5)<)
(cid:5)2)
(cid:5)(cid:23))
(cid:5))
3
)
(cid:23))
2)
<)
5)
’
"
3
(cid:22)
(cid:23)
Figure 2.8 Zone sampling of the PDF.
For the intervals k = 0, 1, 2, it follows that
(cid:3)
Q/2
pY (y) = δ(0)
−Q/2
(cid:3)
= δ(y − Q)
pX(x) dx,
Q/2+Q
−Q/2+Q
(cid:3)
Q/2+2Q
pX(x) dx,
= δ(y − 2Q)
pX(x) dx,
−Q/2+2Q
− Q
2
− Q
2
− Q
2
≤ y <
Q
2
,
+ Q ≤ y <
Q
2
+ Q,
+ 2Q ≤ y <
Q
2
+ 2Q.
The summation over all intervals gives the PDF of the output
pY (y) =
=
∞(cid:6)
k=−∞
∞(cid:6)
k=−∞
δ(y − kQ)W (kQ)
δ(y − kQ)W (y),
where
Using
(cid:3)
Q/2+kQ
−Q/2+kQ
(cid:1)
W (kQ) =
pX(x) dx,
W (y) =
(cid:3) ∞
−∞
(cid:1)
= rect
rect
(cid:2)
y
Q
(cid:2)
pX(x) dx
y − x
Q
∗ pX(y).
δQ(y) =
∞(cid:6)
k=−∞
δ(y − kQ),
the PDF of the output is given by
(cid:4)
(cid:1)
pY (y) = δQ(y)
rect
(cid:5)
(cid:5) pX(y)
.
(cid:2)
y
Q
(2.17)
(2.18)
(2.19)
(2.20)
(2.21)
(2.22)
(2.23)
2.1 Signal Quantization
27
The PDF of the output can hence be determined by convolution of a rect function [Lip92]
with the PDF of the input. This is followed by an amplitude sampling with resolution Q as
described in (2.23) (see Fig. 2.9).
(cid:22)
)
(cid:28)(cid:14)(cid:14)(cid:14)7":
0
)
(cid:23)
")
(cid:23)
*(cid:28)(cid:14)(cid:14)(cid:14)7":
Figure 2.9 Determining PDF of the output.
Using F T {f1(t) · f2(t)} = 1
2π F1(j ω) ∗ F2(j ω), the characteristic function (Fourier
transform of pY (y)) can be written, with uo = 2π/Q, as
PY (ju) = 1
2π
uo
∞(cid:6)
k=−∞
(cid:4)
sin
(cid:7)
u Q
2
u Q
2
(cid:8)
(cid:5)
· PX(ju)
(cid:5)
Q
δ(u − kuo) ∗
(cid:7)
u Q
2
u Q
2
sin
(cid:4)
(cid:8)
δ(u − kuo) ∗
· PX(ju)
∞(cid:6)
=
k=−∞
PY (ju) =
∞(cid:6)
k=−∞
PX(ju − jkuo)
(cid:9)
sin
(u − kuo) Q
2
(u − kuo) Q
2
(cid:10)
.
(2.24)
(2.25)
(2.26)
Equation (2.26) describes the sampling of the continuous PDF of the input. If the
quantization frequency uo = 2π/Q is twice the highest frequency of the characteristic
function PX(ju) then periodically recurring spectra do not overlap. Hence, a reconstruction
of the PDF of the input pX(x) from the quantized PDF of the output pY (y) is possible (see
Fig. 2.10). This is known as Widrow’s quantization theorem. Contrary to the ﬁrst sampling
theorem (Shannon’s sampling theorem, ideal amplitude sampling in the time domain)
F A(j ω) = 1
k=−∞ F (j ω − j kωo), it can be observed that there is an additional
T
(cid:11)∞
(cid:10)
multiplication of the periodically characteristic function with
If the base-band of the characteristic function (k = 0)
PY (ju) = PX(ju)
sin
(cid:8)
(cid:15)
(cid:7)
u Q
2
u Q
(cid:13)(cid:14)
2
PE (ju)
(cid:12)
(cid:9)
sin
(u−kuo) Q
2
(u−kuo) Q
2
(see (2.26)).
(2.27)
is considered, it is observed that it is a product of two characteristic functions. The
multiplication of characteristic functions leads to the convolution of PDFs from which
the addition of two statistically independent signals can be concluded. The characteristic
function of the quantization error is thus
PE(ju) =
(cid:8)
,
sin
(cid:7)
u Q
2
u Q
2
(2.28)
28
Quantization
0(cid:28)(cid:14)(cid:14)(cid:14)7’:
*(cid:28)(cid:14)(cid:14)(cid:14)7":
)
’
"
0(cid:1)(cid:14)(cid:14)(cid:14)7(cid:13):
(cid:1)(cid:14)(cid:14)(cid:14)7*
(cid:13)
:
(cid:13)
0(cid:1)(cid:14)(cid:14)(cid:14)7
:(cid:13)
(cid:18)(cid:16)(cid:10)7 )
(cid:23) :
(cid:13)
:
7
(cid:13)
)
(cid:23)
(cid:13)(cid:11)
(cid:13)(cid:11)
(cid:13)
and the PDF
(see Fig. 2.11).
(cid:28)(cid:14)(cid:14)(cid:14)7(cid:2):
(cid:30)
(cid:22)
)
)
(cid:23)
)
(cid:23)
(cid:2)
Figure 2.10 Spectral representation.
pE(e) = 1
Q
(cid:1)
rect
(cid:2)
e
Q
(2.29)
(cid:22)
(cid:1)(cid:14)(cid:14)(cid:14)7
(cid:30) (cid:13)
:
(cid:23)(cid:2)
)
(cid:23)(cid:2)
)
(cid:13)
Figure 2.11 PDF and characteristic function of quantization error.
The modeling of the quantization process as an addition of a statistically independent
noise signal to the input signal leads to a continuous PDF of the output (see Fig. 2.12,
convolution of PDFs and sampling in the interval Q gives the discrete PDF of the output).
The PDF of the discrete-valued output comprises Dirac pulses at distance Q with values
equal to the continuous PDF (see (2.23)). Only if the quantization theorem is valid can the
continuous PDF be reconstructed from the discrete PDF.
pX+E(y)= p
pX
E*
pY(y)
x(n)
Q
y(n)
Q
y
x(n)
x(n)+e(n)
e(n)
Figure 2.12 PDF of model.
In many cases, it is not necessary to reconstruct the PDF of the input. It is sufﬁcient to
calculate the moments of the input from the output. The mth moment can be expressed in
2.1 Signal Quantization
terms of the PDF or the characteristic function:
(cid:3) ∞
E{Y m} =
−∞
ympY (y) dy
(cid:16)
(cid:16)
= (−j )m d mPY (ju)
(cid:16)
(cid:16)
dum
u=0
29
(2.30)
(2.31)
.
If the quantization theorem is satisﬁed then the periodic terms in (2.26) do not overlap and
the mth derivative of PY (ju) is solely determined by the base-band1 so that, with (2.26),
E{Y m} = (−j )m d m
dum PX(ju)
sin
With (2.32), the ﬁrst two moments can be determined as
mY = E{Y } =E {X},
σ 2
Y
= E{Y 2} = E{X2}
(cid:12) (cid:13)(cid:14) (cid:15)
σ 2
X
.
+ Q2
12(cid:12)(cid:13)(cid:14)(cid:15)
σ 2
E
Second-order Statistics of Quantizer Output
(cid:8)
(cid:7)
u Q
2
u Q
2
(cid:16)
(cid:16)
(cid:16)
(cid:16)
.
u=0
(2.32)
(2.33)
(2.34)
In order to describe the properties of the output in the frequency domain, two output values
Y1 (at time n1) and Y2 (at time n2) are considered [Lip92]. For the joint density function,
(cid:4)
(cid:1)
(cid:2)
(cid:5)
pY1Y2 (y1, y2) = δQQ(y1, y2)
rect
(cid:5) pX1X2(y1, y2)
,
(2.35)
y1
Q
,
y2
Q
with
and
δQQ(y1, y2) = δQ(y1) · δQ(y2)
(cid:1)
(cid:1)
(cid:2)
(cid:2)
(cid:1)
rect
y1
Q
,
y2
Q
= rect
· rect
y1
Q
y2
Q
(cid:2)
.
(2.36)
(2.37)
For the two-dimensional Fourier transform, it follows that
PY1Y2(ju1, ju2) =
∞(cid:6)
∞(cid:6)
k=−∞
(cid:4)
sin
∗
l=−∞
(cid:7)
Q
u1
2
Q
2
∞(cid:6)
u1
∞(cid:6)
=
δ(u1 − kuo)δ(u2 − luo)
(cid:8)
(cid:8)
(cid:7)
(cid:5)
sin
·
Q
2
u2
Q
2
u2
· PX1X2(ju1, ju2)
(2.38)
k=−∞
(cid:9)
sin
·
l=−∞
(u1 − kuo) Q
2
(u1 − kuo) Q
2
PX1X2(ju1
(cid:10)
− j luo)
(cid:10)
− jkuo, ju2
(cid:9)
(u2 − luo) Q
2
(u2 − luo) Q
2
sin
·
.
(2.39)
1This is also valid owing to the weaker condition of Sripad and Snyder [Sri77] discussed in the next section.
30
Quantization
Similar to the one-dimensional quantization theorem, a two-dimensional theorem
[Wid61] can be formulated: the joint density function of the input can be reconstructed
from the joint density function of the output, if PX1X2(ju1, ju2) = 0 for u1 ≥ uo/2 and
u2 ≥ uo/2. Here again, the moments of the joint density function can be calculated as
follows:
E{Y m
1 Y n
2
} =(− j )m+n ∂ m+n
1 ∂un
∂um
2
PX1X2(ju1, ju2)
sin
(cid:8)
Q
2
(cid:7)
u1
Q
2
u1
sin
Q
2
(cid:8)
(cid:16)
(cid:16)
(cid:16)
(cid:16)
(cid:7)
u2
Q
2
u2
.
(2.40)
u1=0,u2=0
From this, the autocorrelation function with m = n2 − n1 can be written as
rYY (m) = E{Y1Y2}(m) =



E{X2} + Q2
12
E{X1X2}(m),
, m = 0,
elsewhere
(2.41)
(for m = 0 we obtain (2.34)).
2.1.3 Statistics of Quantization Error
First-order Statistics of Quantization Error
The PDF of the quantization error depends on the PDF of the input and is dealt with in
(cid:10)
the following. The quantization error e = xQ − x is restricted to the interval
. It
(cid:10)
2 , Q
depends linearly on the input (see Fig. 2.13). If the input value lies in the interval
2
then the error is e = 0 − x. For the PDF we obtain pE(e) = pX(e). If the input value lies
then the quantization error is e = Q(cid:6)Q−1x + 0.5(cid:7) − x
in the interval
(cid:10)
and is again restricted to
. The PDF of the quantization error is consequently
pE(e) = pX(e + Q) and is added to the ﬁrst term. For the sum over all intervals we can
write
(cid:10)
+ Q, Q
+ Q
(cid:9)
2
2 , Q
− Q
2 , Q
− Q
(cid:9)
2
− Q
− Q
2
(cid:9)
(cid:9)
2
pX(e − kQ), − Q
2
≤ e <
Q
2
,
elsewhere.
(2.42)


∞(cid:6)
pE(e) =

k=−∞
0,
(cid:28)(cid:14)(cid:14)(cid:14)7’:
0
(cid:5))1(cid:23)
)1(cid:23)
’
)1(cid:23)
(cid:5))1(cid:23)
(cid:2)
’
(cid:5))1(cid:23)
)1(cid:23)
Figure 2.13 Probability density function and quantization error.
Because of the restricted values of the variable of the PDF, we can write
(cid:1)
pE(e) = rect
(cid:1)
= rect
e
Q
e
Q
(cid:2) ∞(cid:6)
pX(e − kQ)
k=−∞
(cid:2)
[pX(e) ∗ δQ(e)].
(2.43)
(2.44)
2.1 Signal Quantization
31
The PDF of the quantization error is determined by the PDF of the input and can be
computed by shifting and windowing a zone. All individual zones are summed up for
calculating the PDF of the quantization error [Lip92]. A simple graphical interpretation
of this overlapping is shown in Fig. 2.14. The overlapping leads to a uniform distribution
of the quantization error if the input PDF pX(x) is spread over a sufﬁcient number of
quantization intervals.
(cid:28)(cid:14)(cid:14)(cid:14)7’:0
(cid:5))1(cid:23)
)1(cid:23)
’
(cid:30)(cid:28)(cid:14)(cid:14)(cid:14)7(cid:2):
(cid:2)
(cid:5))1(cid:23)
)1(cid:23)
(cid:2)
(cid:5))1(cid:23)
)1(cid:23)
Figure 2.14 Probability density function of the quantization error.
For the Fourier transform of the PDF from (2.44) it follows that
(cid:7)
u Q
2
u Q
2
(cid:8)
PE(ju) = 1
2π
PX(ju)
2π
Q
k=−∞
∞(cid:6)
sin
Q
(cid:4)
∗
(cid:8)
δ(u − kuo)
sin
(cid:7)
u Q
2
u Q
2
∞(cid:6)
=
=
(cid:4) ∞(cid:6)
∗
k=−∞
(cid:4)
PX(jkuo)
k=−∞
(cid:8)
PX(jkuo)δ(u − kuo)
(cid:7)
u Q
2
u Q
2
∗ δ(u − kuo)
sin
(cid:5)
(cid:5)
PE(ju) =
∞(cid:6)
k=−∞
PX(jkuo)
(cid:9)
sin
(u − kuo) Q
2
(u − kuo) Q
2
(cid:10)
.
(cid:5)
(2.45)
(2.46)
(2.47)
(2.48)
If the quantization theorem is satisﬁed, i.e. if PX(ju) = 0 for u > uo/2, then there is only
one non-zero term (k = 0 in (2.48)). The characteristic function of the quantization error is
reduced, with PX(0) = 1, to
PE(ju) =
.
(2.49)
sin
(cid:8)
(cid:7)
u Q
2
u Q
2
32
Hence, the PDF of the quantization error is
pE(e) = 1
Q
(cid:1)
rect
(cid:2)
.
e
Q
Quantization
(2.50)
Sripad and Snyder [Sri77] have modiﬁed the sufﬁcient condition of Widrow (band-limited
characteristic function of input) for a quantization error of uniform PDF by the weaker
condition
PX(j kuo) = PX
j
= 0 for all k (cid:8)= 0.
(2.51)
(cid:1)
(cid:2)
2πk
Q
The uniform distribution of the input PDF,
pX(x) = 1
Q
(cid:1)
rect
(cid:2)
,
x
Q
with characteristic function
PX(ju) =
(cid:8)
,
sin
(cid:7)
u Q
2
u Q
2
(2.52)
(2.53)
does not satisfy Widrow’s condition for a band-limited characteristic function, but instead
the weaker condition,
(cid:1)
PX
j
(cid:2)
2πk
Q
= sin(πk)
πk
= 0,
for all k (cid:8)= 0,
(2.54)
is fulﬁlled. From this follows the uniform PDF (2.49) of the quantization error. The weaker
condition from Sripad and Snyder extends the class of input signals for which a uniform
PDF of the quantization error can be assumed.
In order to show the deviation from the uniform PDF of the quantization error as a
function of the PDF of the input, (2.48) can be written as
PE (ju) = PX(0)
(cid:9)
sin
u Q
2
u Q
2
(cid:10)
+
∞(cid:6)
k=−∞,k(cid:8)=0
(cid:1)
∞(cid:6)
PX
j
(cid:1)
j
(cid:2)
PX
2πk
Q
2πk
Q
(cid:9)
sin
k=−∞,k(cid:8)=0
(cid:10)
u Q
2
u Q
2
=
(cid:10)
sin
(cid:9)
u Q
2
u Q
2
+
(cid:2)
(cid:9)
sin
(cid:10)
(u − kuo) Q
2
(u − kuo) Q
2
∗ δ(u − ku0).
(2.55)
The inverse Fourier transform yields
pE(e) = 1
Q


=

(cid:2)(cid:4)
(cid:1)
rect
e
Q
1 +
∞(cid:6)
k=−∞,k(cid:8)=0
(cid:2)
(cid:1)
PX
j
2πk
Q
(cid:4)
1 +
∞(cid:6)
k(cid:8)=0
1
Q
0,
(cid:1)
PX
j
(cid:1)
exp
j
(cid:2)
(cid:1)
exp
j
2πk
Q
2πke
Q
(cid:2)(cid:5)
(cid:2)(cid:5)
, − Q
2
2πke
Q
≤ e <
Q
2
,
(2.56)
(2.57)
elsewhere.
Equation (2.56) shows the effect of the input PDF on the deviation from a uniform PDF.
2.1 Signal Quantization
33
Second-order Statistics of Quantization Error
To describe the spectral properties of the error signal, two values E1 (at time n1) and E2
(at time n2) are considered [Lip92]. The joint PDF is given by
(cid:1)
pE1E2(e1, e2) = rect
e1
Q
,
e2
Q
(cid:2)
[pX1X2(e1, e2) ∗ δQQ(e1, e2)].
(2.58)
Here δQQ(e1, e2) = δQ(e1) · δQ(e2) and rect(e1/Q, e2/Q) = rect(e1/Q) · rect(e2/Q). For
the Fourier transform of the joint PDF, a similar procedure to that shown by (2.45)–(2.48)
leads to
PE1E2(ju1, ju2) =
∞(cid:6)
∞(cid:6)
k1=−∞
(cid:9)
sin
·
k2=−∞
(u1 − k1uo) Q
2
(u1 − k1uo) Q
2
PX1X2 (jk1uo, jk2uo)
(cid:9)
(u2 − k2uo) Q
2
(u2 − k2uo) Q
2
sin
(cid:10)
(cid:10)
.
If the quantization theorem and/or the Sripad–Snyder condition
PX1X2(jk1uo, jk2uo) = 0
for all k1, k2 (cid:8)= 0
are satisﬁed then
u1
Thus, for the joint PDF of the quantization error,
PE1E2(ju1, ju2) =
(cid:9)
sin
(cid:10)
sin
(cid:10)
.
Q
2
(cid:9)
u2
Q
2
u2
Q
2
u1
Q
2
(cid:1)
(cid:2)
pE1E2 (e1, e2) = 1
Q
rect
· 1
e1
Q
Q
= pE1 (e1) · pE2(e2).
rect
(cid:1)
(cid:2)
, − Q
2
e2
Q
≤ e1, e2 <
Q
2
Due to the statistical independence of quantization errors (2.63),
E{Em
1 En
2
} =E {Em
1
} · E{En
2
}.
For the moments of the joint PDF,
E{Em
1 En
2
} = (−j )m+n ∂ m+n
1 ∂un
∂um
2
PE1E2 (u1, u2)
(cid:16)
(cid:16)
(cid:16)
(cid:16)
.
u1=0,u2=0
From this, it follows for the autocorrelation function with m = n2 − n1 that
rEE(m) = E{E1E2} =
m = 0,
elsewhere
=
(cid:21)
E{E2},
E{E1E2},


Q2
12
0,
= Q2
12(cid:12)(cid:13)(cid:14)(cid:15)
σ 2
E
δ(m).

, m = 0,
elsewhere
(2.59)
(2.60)
(2.61)
(2.62)
(2.63)
(2.64)
(2.65)
(2.66)
(2.67)
(2.68)
34
The power density spectrum of the quantization error is then given by
SEE(ej (cid:4)) =
+∞(cid:6)
m=−∞
rEE(m) e
−j (cid:4)m = Q2
12
,
Quantization
(2.69)
which is equal to the variance σ 2
E
= Q2/12 of the quantization error (see Fig. 2.15).
r
EE
(m)
(cid:5)2
E
.(cid:6)(m)
EE
j(cid:3)
S (e )
Q2
12
-5 -4 -3 -2 -1
0
1
2
3
4
5
m
-(cid:4)
+(cid:4)
2(cid:4)
(cid:3)
Figure 2.15 Autocorrelation rEE(m) and power density spectrum SEE(ej (cid:4)) of quantization
error e(n).
Correlation of Signal and Quantization Error
To describe the correlation of the signal and the quantization error [Sri77], the second
moment of the output with (2.26) is derived as follows:
(cid:16)
(cid:16)
(cid:16)
(cid:16)
(2.70)
E{Y 2} =(− j )2 d 2PY (ju)
du2
(cid:4)
∞(cid:6)
(cid:2)
sin(πk)
πk
+ Q ˙PX
(cid:2)
(cid:1)
− 2πk
Q
sin(πk) − πk cos(πk)
π 2k2
¨PX
u=0
(cid:1)
− 2πk
Q
(cid:2)
= (−j )2
k=−∞
(cid:1)
PX
+ Q2
4
= E{X2} + Q
π
(cid:5)
− 2πk
Q
∞(cid:6)
(2 − π 2k2) sin(πk) − 2πk cos(πk)
π 3k3
(cid:1)
− 2πk
Q
(−1)k
k
+ E{E2}.
˙PX
(cid:2)
k=−∞,k(cid:8)=0
(2.71)
(2.72)
With the quantization error e(n) = y(n) − x(n),
E{Y 2} =E {X2} +2 E{X · E} +E {E2},
(2.73)
where the term E{X · E}, with (2.72), is written as
E{X · E} = Q
2π
∞(cid:6)
k=−∞,k(cid:8)=0
(−1)k
k
˙PX
(cid:1)
− 2πk
Q
(cid:2)
.
With the assumption of a Gaussian PDF of the input we obtain
pX(x) = 1√
2πσ
(cid:1)
exp
(cid:2)
−x2
2σ 2
(2.74)
(2.75)
2.1 Signal Quantization
with the characteristic function
(cid:1)
PX(ju) = exp
(cid:2)
.
−u2σ 2
2
35
(2.76)
Using (2.57), the PDF of the quantization error is then given by
pE(e) =
(cid:4)



1
Q
0,
1 + 2
∞(cid:6)
k=1
(cid:1)
cos
(cid:2)
(cid:1)
− 2π 2k2σ 2
Q2
exp
(cid:2)(cid:5)
,
2πke
Q
− Q
2
≤ e <
Q
2
,
elsewhere.
(2.77)
Figure 2.16a shows the PDF (2.77) of the quantization error for different variances of the
input.
2
1.8
1.6
1.4
1.2
1
0.8
0.6
0.4
0.2
→
)
e
(
E
p
0.2
a)
0.3
0.4
0.8
0.09
0.08
0.07
0.06
0.05
0.04
0.03
0.02
0.01
→
)
2
E
(
E
b)
Q 1
0
-0.5 -0.4 -0.3 -0.2 -0.1
0
0.1 0.2 0.3 0.4 0.5
0
0
0.1 0.2 0.3 0.4 0.5 0.6 0.7 0.8 0.9
1
e/Q →
σ →
Figure 2.16 (a) PDF of quantization error for different standard deviations of a Gaussian PDF input.
(b) Variance of quantization error for different standard deviations of a Gaussian PDF input.
For the mean value and the variance of a quantization error, it follows using (2.77) that
E{E} =0 and
E{E2} =
(cid:3) ∞
−∞
e2pE(e) de = Q2
12
(cid:4)
1 + 12
π 2
∞(cid:6)
k=1
(−1)k
k2
(cid:1)
− 2π 2k2σ 2
Q2
exp
(cid:2)(cid:5)
.
(2.78)
Figure 2.16b shows the variance of the quantization error (2.78) for different variances of
the input.
For a Gaussian PDF input as given by (2.75) and (2.76), the correlation (see (2.74))
between input and quantization error is expressed as
E{X · E} =2 σ 2
(−1)k exp
(cid:1)
− 2π 2k2σ 2
Q2
(cid:2)
.
∞(cid:6)
k=1
(2.79)
The correlation is negligible for large values of σ/Q.
36
2.2 Dither
2.2.1 Basics
Quantization
The requantization (renewed quantization of already quantized signals) to limited word-
lengths occurs repeatedly during storage, format conversion and signal processing algo-
rithms. Here, small signal levels lead to error signals which depend on the input. Owing
to quantization, nonlinear distortion occurs for low-level signals. The conditions for the
classical quantization model are not satisﬁed anymore. To reduce these effects for signals
of small amplitude, a linearization of the nonlinear characteristic curve of the quantizer is
performed. This is done by adding a random sequence d(n) to the quantized signal x(n)
(see Fig. 2.17) before the actual quantization process. The speciﬁcation of the word-length
is shown in Fig. 2.18. This random signal is called dither. The statistical independence of
the error signal from the input is not achieved, but the conditional moments of the error
signal can be affected [Lip92, Ger89, Wan92, Wan00].
-7(cid:10):
7(cid:18)(cid:14).(cid:16)(cid:7):
’7(cid:10):
7;9(cid:8)(cid:14).(cid:16)(cid:7):
)
"7(cid:10):
7;(cid:14).(cid:16)(cid:7):
Figure 2.17 Addition of a random sequence before a quantizer.
;(cid:14).(cid:16)(cid:7)
(cid:8)(cid:14).(cid:16)(cid:7)
(cid:12)
(cid:18)(cid:14)
H(cid:2)(cid:8)(cid:11)(cid:14).(cid:16)(cid:7)(cid:18)
(cid:26)(cid:16)(cid:7)(cid:21)(cid:2)(cid:8)
(cid:6)(cid:16)(cid:3)(cid:10)
Figure 2.18 Speciﬁcation of the word-length.
The sequence d(n), with amplitude range (−Q/2 ≤ d(n) ≤ Q/2), is generated with
the help of a random number generator and is added to the input. For a dither value with
Q = 2−(w−1):
dk = k2
−r Q, −2s−1 ≤ k ≤ 2s−1 − 1.
(2.80)
The index k of the random number dk characterizes the value from the set of N = 2s
possible numbers with the probability
P (dk) =
(cid:21)
2−s, −2s−1 ≤ k ≤ 2s−1 − 1,
0,
elsewhere.
(2.81)
(cid:11)
k dkP (dk), the variance σ 2
D
=
(cid:11)
k
[dk − d]2P (dk) and the
With the mean value d =
quadratic mean d 2 =
k d 2
(cid:11)
k P (dk), we can rewrite the variance as σ 2
d
For a static input amplitude V and the dither value dk the rounding operation [Lip86] is
= d 2 − d
.
2
expressed as
(cid:22)
g(V + dk) = Q
(cid:23)
+ 0.5
.
V + dk
Q
(2.82)
2.2 Dither
For the mean of the output g(V ) as a function of the input V , we can write
g(V ) =
(cid:6)
k
g(V + dk)P (dk).
The quadratic mean of the output g2(V ) for input V is given by
g2(V ) =
(cid:6)
k
g2(V + dk)P (dk).
For the variance d 2
R(V ) for input V ,
d 2
R(V ) =
(cid:6)
{g(V + dk) − g(V )}2P (dk) = g2(V ) − {g(V )}2.
k
37
(2.83)
(2.84)
(2.85)
The above equations have the input V as a parameter. Figures 2.19 and 2.20 illustrate the
mean output g(V ) and the standard deviation dR(V ) within a quantization step size, given
by (2.83), (2.84) and (2.85). The examples of rounding and truncation demonstrate the
linearization of the characteristic curve of the quantizer. The coarse step size is replaced
by a ﬁner one. The quadratic deviation from the mean output d 2
R(V ) is termed noise
modulation. For a uniform PDF dither, this noise modulation depends on the amplitude
(see Figs. 2.19 and 2.20). It is maximum in the middle of the quantization step size
and approaches zero toward the end. The linearization and the suppression of the noise
modulation can be achieved by a triangular PDF dither with bipolar characteristic [Van89]
and rounding operation (see Fig. 2.20). Triangular PDF dither is obtained by adding two
statistically independent dither signals with uniform PDF (convolution of PDFs). A dither
signal with higher-order PDF is not necessary for audio signals [Lip92, Wan00].
(cid:111)
)
V
(
R
d
d
n
a
)
V
(
¯g
1
0.9
0.8
0.7
0.6
0.5
0.4
0.3
0.2
0.1
0
Unipolar RECT Dither
¯g(V)
....................................................................................................
dR(V)
0
0.2
0.4
0.6
V/Q (cid:111)
0.8
1
(cid:111)
)
V
(
R
d
d
n
a
)
V
(
¯g
1
0.9
0.8
0.7
0.6
0.5
0.4
0.3
0.2
0.1
0
Unipolar TRI Dither
¯g(V)
....................................................................................................
dR(V)
0
0.2
0.4
0.6
V/Q (cid:111)
0.8
1
Figure 2.19 Truncation – linearizing and suppression of noise modulation (s = 4, m = 0).
The total noise power for this quantization technique consists of the dither power and
the power of the quantization error [Lip86]. The following noise powers are obtained by
integration with respect to V .
