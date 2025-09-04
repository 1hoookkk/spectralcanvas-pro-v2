# Audio Plugins C++ (Pirkle) — Chunk 28/73

> Source: `Designing Audio Effect Plug-Ins in C++_ With Digital Audio Signal Processing Theory - PDF Room.pdf` · Pages ~1-560 · Extractor: pdfminer.six
> Target ≈ 2800 words, Overlap = 200 words · Actual = 2718

> Note: DSP textbook content for SpectralCanvasPro RT-audio knowledge base

---
184  Chapter 6
+30.0 dB
+15.0 dB
0.0 dB
-15.0 dB
-30.0 dB
-45.0 dB
-60.0 dB
Q = 10
Q = 0.707
10 Hz
100 Hz
1 kHz
10 kHz
 Figure 6.22:    Second-order LPF responses:  f  c  = 1 kHz,  Q  = 0.707, 2, 5, 10.
Notice that as the  Q  rises above 0.707,  f  c  becomes the peak frequency.
   6.6.3  Second-Order BPF and BSF
 Specify:
(cid:129)
(cid:129)
    f  c , corner frequency
    Q , quality factor controlling width of peak or notch 5 1/ BW; Figure 6.23 shows the BSF
version
 Note: These ﬁ lter coefﬁ cients contain the tan() function, which is undeﬁ ned at p/2 and then
ﬂ ips negative between p/2 and p. The argument is u c /2 Q , so care must be taken with these
ﬁ lters to ensure that u c /2 Q  does not fall on the range of p/2 to p. One solution is to clamp the
value of u c /2 Q  so that it never equals or exceeds p/2.
 The design equations are as follows:
                BPF
uc 5 2pfc / fs
b 5 0.5
1 2 tan 1 uc /  2Q2
1 1 tan 1 uc /  2Q2
g 5 1 0.5 1 b2 cos uc
a0 5 0.5 2 b
a1 5 0.0
a2 5 21 0.5 2 b2
b1 5 22g
b2 5 2b
                 BSF
uc 5 2pfc / fs
 b 5 0.5
1 2 tan 1 uc /  2Q2
1 1 tan 1 uc /  2Q2
 g 5 1 0.5 1 b2 cos uc
a0 5 0.5 1 b
a1 5 22g
a2 5 0.5 1 b
b1 5 22g
b2 5 2b
(6.36)
   6.6.4  Second-Order Butterworth LPF and HPF
 Specify:
(cid:129)
    f  c , corner frequency

+12.0dB
0.0 dB
-12.0dB
-24.0 dB
-36.0 dB
-48.0 dB
-60.0 dB
Audio Filter Designs: IIR Filters  185
BW =  500  Hz
BW = 10 kHz
10 Hz
100 Hz
1 kHz
10 kHz
 Figure 6.23:    Second-order BSF responses:  f  c  = 1 kHz, bandwidth = 500 Hz (narrowest),
2 kHz, 5 kHz, and 10 kHz (widest).
 Butterworth low-pass and high-pass ﬁ lters are specialized versions of the ordinary second-
order low-pass ﬁ lter. Their  Q  values are ﬁ xed at 0.707, which is the largest value it can
assume before peaking in the frequency response is observed.
 The design equations are as follows:
          LPF
 C 5
a0 5
1
 tan(pfc / fs)
1
1 1 "2C 1 C2
a1 5 2a0
a2 5 a0
b1 5 2a0(1 2 C2)
b2 5 a0(1 2 "2C 1 C2)
          HPF
 C 5  tan(pfc / fs)
a0 5
1
1 1 "2C 1 C2
a1 5 22a0
a2 5 a0
b1 5 2a0(C2 2 1)
b2 5 a0(1 2 "2C 1 C2)
(6.37)
   6.6.5  Second-Order Butterworth BPF and BSF
 Specify:
(cid:129)
(cid:129)
    f  c , corner frequency
    BW , bandwidth of peak/notch 5  f  c / Q
 Butterworth BPF and BSF are made by cascading (BPF) or paralleling (BSF) a Butterworth
LPF and Butterworth HPF.
 Note: These ﬁ lter coefﬁ cients contain the tan() function, which is undeﬁ ned at p/2 and then
ﬂ ips negative between p/2 and p. The argument is p f  c  BW / f  s , so care must be taken with these
ﬁ lters to ensure that p f  c  BW / f  s  does not fall on the range of p/2 to p. One solution is to clamp
the value of p f  c  BW / f  s  so that it never equals or exceeds p/2.

186  Chapter 6
 The design equations are as follows:
C 5
               BPF
1
 tan(pfc BW / fs)
D 5 2 cos(2pfc /  fs)
1
1 1 C
a0 5
a1 5 0.0
a2 5 2a0
b1 5 2a0(CD)
b2 5 a0(C 2 1)
             BSF
C 5  tan(pfcBW / fs)
a0 5
D 5 2 cos(2pfc /  fs)
1
1 1 C
a1 5 2a0D
a2 5 a0
b1 5 2a0D
b2 5 a0(1 2 C)
(6.38)
   6.6.6  Second-Order Linkwitz–Riley LPF and HPF
 Specify:
(cid:129)
    f  c , corner frequency (26 dB)
 Second-order Linkwitz–Riley LPFs and HPFs are designed to have an attenuation of 26 dB
at the corner frequency rather than the standard 23 dB, shown in Figure 6.24. When these
ﬁ lters are placed in parallel with the same cutoff frequency, their outputs sum exactly and
the resulting response is ﬂ at. They are often used in crossovers. We use them for the spectral
dynamics processor later on.
 The design equations are as follows:
    LPF
uc 5 pfc  /  fs
Vc5 pfc
     HPF
uc 5 pfc /  fs
Vc 5 pfc
2 1 2kVc
a0 5
k 5
Vc
 tan(uc)
d 5 k2 1 Vc
2
Vc
d
2
Vc
d
2
Vc
d
22k2 1 2Vc
2
d
a1 5 2
a2 5
b1 5
2 1 2kVc
a0 5
k 5
Vc
 tan(uc)
d 5 k2 1 Vc
k2
d
22k2
d
k2
d
22k2 1 2Vc
2
d
a2 5
a1 5
b1 5
(6.39)
b2 5
22kVc 1 k2 1 Vc
2
d
b2 5
22kVc 1 k2 1 Vc
2
d

Audio Filter Designs: IIR Filters  187
 All pass ﬁ lters have interesting designs that yield equally interesting results. Their frequency
responses are perfectly ﬂ at from DC to Nyquist. However, the phase responses are the
same as those in ﬁ rst- and second-order LPFs. You get all of the phase shift but none of the
frequency response change. These ﬁ lters can be found in crossovers and reverb algorithms.
They are also used for the phaser effect. APFs are designed with pole-zero pairs whose pole/
zero radii are reciprocals of one another. For a ﬁ rst-order APF, the pole lies somewhere on the
real axis inside the unit circle at radius  R . The zero lies outside the unit circle at radius 1/ R
( Figure 6.25 ).
 If you think about the geometric interpretation and analysis of the transfer function, as you
move around the unit circle, your vector distances from the pole and zero will always be
reciprocals, or 1/each other. The amplitude response is then ﬂ at but the phase response does
change because it is not based on the distance to the point in question but rather on the angle
of incidence of the ray line drawn from the analysis point to the pole or zero. The second-
order APF has complementary poles and zeros also at reciprocal radii (Figure 6.26).
0.0  dB
-2.0  dB
-4.0  dB
-6.0  dB
-8.0  dB
-10.0  dB
-12.0  dB
Butterworth
Linkwitz-Riley
10 Hz
100 Hz
1 kHz
10  kHz
 Figure 6.24:    A comparison between Butterworth and Linkwitz–Riley ﬁ lters
each with  f c = 1 kHz; the Linkwitz–Riley ﬁ lter is down −6 dB at  f  c .
π/4
π/2
3π/4
π
Angle
Im
R
1/R
Re
Phase
0°
–90°
–180°
 Figure 6.25:    The ﬁ rst-order APF has a ﬂ at frequency response but shifts
the phase of the signal by −90 degrees at  f  c  (p/2 in this example).

188  Chapter 6
1/R
Phase
0°
Im
R
Re
-180°
-360°
Π/4
Π/2
3Π/4
Angle
 Figure 6.26 :    The second-order APF adds another −90 degrees of phase shift at  f  c  (p/2 here).
   6.6.7  First- and Second-Order APF
 Specify:
(cid:129)
(cid:129)
    f  c , corner frequency
    Q , steepness of phase shift at  f  c  (second-order only)
 The design equations are as follows:
     First-Order APF
 tan1 pfc / fs
 tan1 pf
c / fs
2 2 1
2 1 1
a 5
    Second-Order APF
 tan1 pQ / fs
 tan1 pQ / fs
2 2 1
2 1 1
  a 5
a0 5 a
a1 5 1.0
a2 5 0.0
b1 5 a
b2 5 0.0
   b 5 2cos uc
 a0 5 2a
 a1 5 b1 1 2 a2
a2 5 1.0
 b1 5 b1 1 2 a2
 b2 5 2a
(6.40)
    6.7    Audio Speciﬁ c Filters
 The basic classical ﬁ lters provide many functions in audio and can be very musical
(e.g., resonant low-pass ﬁ lters in synthesizers) but you also need ﬁ lters that are very audio
speciﬁ c. These ﬁ lters are found all throughout plug-ins and have been used in mixing
consoles and other audio gear for decades. These designs are often not found in DSP
textbooks because of their speciﬁ c audio-only functions. These ﬁ lters include:
(cid:129)
(cid:129)
(cid:129)
   Shelving ﬁ lters
   Parametric EQ
   Graphic EQ
Π

Audio Filter Designs: IIR Filters  189
 These all require a slightly modiﬁ ed bi-quad structure. The reason is that these ﬁ lters require
mixing the original, unﬁ ltered input directly with the output in a mix ratio. The ratio is
controlled by two more coefﬁ cients, c 0  and d 0  .
  6.7.1  Modiﬁ ed Bi-Quad
 You can see that the ﬁ lter in  Figure 6.27  is a bi-quad with two extra coefﬁ cients, c 0  and d 0 ,
which make the wet-dry mix ratio needed for these ﬁ lter types.
   6.7.2  First-Order Shelving Filters
  Low shelving
 Specify:
(cid:129)
(cid:129)
    f  c , low shelf frequency
   Low-frequency  gain/attenuation in dB
   High shelving
 Specify:
(cid:129)
(cid:129)
    f  c , high shelf frequency
   High-frequency  gain/attenuation in dB
x(n)
0 a
1 a
2 a
-1
Z
-1
Z
Σ
0 d
0 c
Σ
y(n)
-1
Z
-1
Z
-b
1
-b
2
 Figure 6.27:    The modiﬁ ed bi-quad required for audio ﬁ lters.
190  Chapter 6
 The design equations are as follows:
 g 5
 b 5
   Low Shelving
uc 5 2p fc / fs
 m 5 10Gain(dB)/20
4
1 1 m
 d 5 b tan1 uc/2 2
1 2 d
1 1 d
1 2 g
2
1 2 g
2
a2 5 0.0
b1 5 2g
b2 5 0.0
c0 5 m 2 1.0
d0 5 1.0
a0 5
a1 5
 b 5
    High Shelving
uc 5 2p fc / fs
 m 5 10Gain(dB)/20
1 1 m
4
 d 5 b tan1 uc/2 2
1 2 d
1 1 d
1 1 g
2
a0 5
 g 5
a1 5 2 a
1 1 g
2
b
a2 5 0.0
b1 5 2g
b2 5 0.0
c0 5 m 2 1.0
d0 5 1.0
(6.41)
 Shelving ﬁ lters are used in many tone controls, especially when there are only two, bass
and treble, which are almost always implemented as shelf types. The ﬁ lters have a corner
frequency and gain or attenuation value.  Figure 6.28  shows a family of shelving ﬁ lter
response curves.
+12.0 dB
+8.0 dB
+4.0 dB
0.0  dB
-4.0 dB
-8.0 dB
-12.0 dB
10 Hz
100 Hz
1 kHz
10 kHz
 Figure 6.28:    Low and high shelving ﬁ lter responses. The low shelf frequency = 400 Hz,
high shelf frequency = 5 kHz, with a variety of boost/cut settings.

6.7.3  Second-Order Parametric/Peaking Filter: Non-Constant-Q

Audio Filter Designs: IIR Filters  191
 Specify:
(cid:129)
(cid:129)
(cid:129)
    f  c , center frequency
    Q  quality factor
   Gain/attenuation in dB
 Parametric EQs allow you to adjust the center frequency,  Q  and boost or cut creating any
arbitrary bumps or notches in the frequency response (Figure 6.30). The parametric EQ is a
variation on the ordinary band-pass and band-stop ﬁ lters that generates symmetrical boost/
cut curves and mixes in the dry signal to create the ﬁ nal response. A true digital parametric
EQ not only has independent controls, but each control only varies one coefﬁ cient in the
ﬁ lter. The parametric EQs in this section afford the same frequency response but adjustments
in any parameter require a recalculation of all the coefﬁ cients. These ﬁ lters are also called
peaking ﬁ lters.
 This parametric EQ is not constant-Q, which means the bandwidth varies depending on the
boost/cut value. Some analog ﬁ lters have the same issue, although there is occasional debate
over whether or not this is desirable in an EQ design.  Figure 6.29  shows this EQ with three
different boost curves with a center frequency of 1 kHz and  Q  5 1.0; therefore, the bandwidth
should also be 1000 Hz.
 Note: These ﬁ lter coefﬁ cients contain the tan() function, which is undeﬁ ned at p/2 and then
ﬂ ips negative between p/2 and p. The argument is u c /2 Q , so care must be taken with these
ﬁ lters to ensure that u c /2 Q  does not fall on the range of p/2 to p. One solution is to clamp the
value of u c /2 Q  so that it never equals or exceeds p/2.
+12.0  dB
+9.0  dB
+6.0  dB
+3.0  dB
0.0  dB
-3.0  dB
-6.0  dB
10 Hz
100 Hz
1 kHz
10 kHz
 Figure 6.29:    The non-constant-Q peaking ﬁ lter has different bandwidths for
different gain values; the bandwidth widens as the gain is reduced.
  f  c  = 1 kHz,  Q  = 0.707.
192  Chapter 6
+12.0  dB
+8.0
dB
+4.0
dB
0.0
dB
-4.0
dB
-8.0
dB
-12.0
dB
10Hz
100 Hz
1 kHz
10 kHz
 Figure 6.30 :    A set of responses for the non-constant-Q parametric/peaking
ﬁ lter with a variety of boost/cut settings.  f  c  = 1 kHz,  Q  = 2.
 The design equations are as follows:
uc 5 2pfc /  fs
 m 5 10Gain(dB)/20
4
1 1 m
 z 5
 b 5 0.5
1 2 z tan1 uc/2Q2
1 1 z tan1 uc/2Q2
 g 5 1 0.5 1 b2 cos uc
a0 5 0.5 2 b
a1 5 0.0
a2 5 2(0.5 2 b)
b1 5 22g
b2 5 2b
c0 5 m 2 1.0
d0 5 1.0
(6.42)
   6.7.4  Second-Order Parametric/Peaking Filter: Constant-Q
 Specify:
(cid:129)
(cid:129)
(cid:129)
    f  c , center frequency
    Q , quality factor
   Gain/attenuation in dB
 This design creates an almost perfect constant-Q ﬁ lter with only a small amount of error
for low-boost (or cut) values (Figure 6.31). The effect of the constant-Q design is clearly
evidenced in the frequency response plot ( Figure 6.32 ).

Audio Filter Designs: IIR Filters  193
+12.0 dB
+9.0
dB
+6.0
dB
+3.0
dB
0.0
dB
-3.0
dB
-6.0
dB
10 Hz
100 Hz
1 kHz
10 kHz
 Figure 6.31:    The constant-Q peaking ﬁ lter preserves the bandwidth over most
of the range of boost/cut values.  f  c  = 1kHz,  Q  = 0.707.
 The design equations are as follows:
 K 5  tan(p fc /  fs)
V0 5 10boost/cut(dB)/20
K 1 K2
 d0 5 1 1
  e0 5 1 1
1
Q
1
V0Q
K 1 K2
a 5 1 1
V0
Q
K 1 K2
b 5 21 K2 2 12
g 5 1 2
d 5 1 2
V0
Q
K 1 K2
1
Q
K 1 K2
h 5 1 2
1
V0Q
K 1 K2
a0 5
Boost
a
d0
b
d0
g
d0
a1 5
a2 5
b1 5
b2 5
b
d0
d
d0
c0 5 1.0
d0 5 0.0
(6.43)
Cut
a0  5
d0
e0
b
e0
d
e0
b
e0
h
e0
a1 5
a2 5
b1 5
b2 5
c0 5 1.0
d0 5 0.0
