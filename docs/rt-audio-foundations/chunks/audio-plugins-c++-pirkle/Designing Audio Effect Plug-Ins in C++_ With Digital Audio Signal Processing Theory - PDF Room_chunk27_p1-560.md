# Audio Plugins C++ (Pirkle) — Chunk 27/73

> Source: `Designing Audio Effect Plug-Ins in C++_ With Digital Audio Signal Processing Theory - PDF Room.pdf` · Pages ~1-560 · Extractor: pdfminer.six
> Target ≈ 2800 words, Overlap = 200 words · Actual = 2505

> Note: DSP textbook content for SpectralCanvasPro RT-audio knowledge base

---
1.
2.
3.
4.
5.
Audio Filter Designs: IIR Filters  175
   Start with an analog ﬁ lter with a normalized transfer function  H ( s )—“normalized” means
the cutoff frequency is set to v 5 1 rad/sec; this is the typical way analog transfer func-
tions are described and speciﬁ ed.
   Choose the cutoff frequency of the digital ﬁ lter v  d  ; get the pre-warped analog corner
frequency using  Equation 6.15 :
vd/fs
2
   Scale the normalized ﬁ lter’s cutoff frequency out to the new analog cutoff v a  by replacing
 s  with  s /v a  in the analog transfer function.
   Apply the bilinear transform by replacing s with  Equation 6.13 :
  va 5  tan c
(6.15)
d
  s 5
z 2 1
z 1 1
(6.13)
   Manipulate the transfer function  H ( z ) algebraically to get it into the familiar form so you
can identify the coefﬁ cients (format the numerator and denominator to match the transfer
functions you studied in the last chapter); this is often the most difﬁ cult part.
 NOTE: This only works for LPF designs but, fortunately, there are easy conversions. The
difference is in the step where you scale the analog ﬁ lter’s cutoff frequency just before
applying the bilinear transform. How it works is described next.
 For LPF and HPF:
(cid:129)
(cid:129)
   Specify  v  d  , the desired digital cutoff frequency.
   Calculate the pre-warped analog cutoff with  Equation 6.14 :
va 5  tan c
vd/fs
2
d
va 5  the  analog  frequency
vd 5  the  mapped  digital  frequency
fs  5  the  sample  rate
where
 For BPF and BSF:
(cid:129)
(cid:129)
   Specify  v  dL   and v  dH  , the lower and upper corner frequencies of the digital ﬁ lter.
   Calculate the two analog corner frequencies with  Equation 6.16 :
d
vaH 5 tan c
vaL 5 tan c
vdL/fs
2
vdH/fs
2
2 5 vaLvaH
v0
W 5 vaH 2 vaL
d
(6.14)
(6.16)

176  Chapter 6
 Next, scale the ﬁ lter with  Equation 6.17 :
Filter Type
LPF
HPF
BPF
BSF
s 5
s 5
Scaling Factor
s
va
va
s
s2 1 v2
0
Ws
Ws
s2 1 v2
0
s 5
s 5
(6.17)
 Example:
 Convert the basic  resistor-capacitor (RC)   analog LPF in  Figure 6.14  into a digital LPF. The
sample rate is 44.1 kHz and the desired cutoff frequency is 1 kHz.
  Step 1: Get the normalized  H ( s ):
H(  jv)  5
1
jvRC 1 1
                                  Let  s 5 jv
   Normalize  by  setting  RC 5 1
H(s) 5
1
s 1 1
  Step 2: Calculate the pre-warped analog cutoff:
fc 5 1 kHz
vd 5 2pfc 5 6283.2  rad/sec
(6.18)
(6.19)
  va 5  tana
vdT
2
b 5  tan c
(6283.2)(1/44100)
2
d 5 0.07136
(6.20)
R
VIN
VOUT
c
 Figure 6.14:    A simple analog RC low-pass ﬁ lter.

  Step 3: De-normalize the analog transfer function  H ( s ) with the appropriate factor:
Audio Filter Designs: IIR Filters  177
H(s) 5
s 5
1
s 1 1
s
va
  because  this  is  an  LPF
H'(s) 5
1
s/va 1 1
5
1
s/0.07136 1 1
5
0.07136
s 1 0.07136
  Step 4: Apply the BZT:
H(z) 5 H'(s)|s5(z21)/(z11)
5
0.07136
z 2 1
z 1 1
1 0.07136
5
5
0.07136(z 1 1)
z 2 1 1 0.07136(z 1 1)
0.07136z 1 0.07136
z 2 1 1 0.07136z 1 0.07136
5
0.07136z 1 0.07136
z 1 0.07136z 2 0.9286
 Get into standard H(z) format:
(6.21)
(6.22)
5
H(z)   5
0.07136 1 0.07136z21
1 1 0.07136 2 0.9286z21
0.0667 1 0.0667z21
1 2 0.8667z21 5
a0 1 a1z21
1 1 b1z21
  Equation 6.22  is in the format that we need with the numerator and denominator properly
formed to observe the coefﬁ cients. From the transfer function, you can see that:
(cid:129)
(cid:129)
(cid:129)
   a  0  5 0.0667
   a  1  5 0.0667
   b  1  5 –0.8667
 The difference equation is  Equation 6.23 :
  y(n) 5 0.0667x(n) 1 0.0667x(n 2 1) 1 0.8667y(n 2 1)
(6.23)

178  Chapter 6
x(n)
0.0667
Σ
–1
Z
y(n)
–1
Z
0.0667
0.8667
 Figure 6.15:    The digital equivalent of the analog RC low-pass ﬁ lter.
 The block diagram of the completed design reveals a pole-zero ﬁ lter. This makes
sense—the original analog ﬁ lter had a pole at  s  5 21 and a zero at inﬁ nity. The digital
equivalent has a pole at  z  5 0.8667 and a zero at  z  5 21 (Nyquist) both on the real axis
( Figure 6.15 ).
  6.4.1  Challenge
 The analog transfer function of a second-order LPF is  Equation 6.24 :
  H(s) 5
1
s2 1 (1/Q)s 1 1
(6.24)
 The analog LPF has the following speciﬁ cations:  Q  5 1,  f  c  5 1 kHz,  f  s  5 44.1 kHz. Apply
the bilinear transform and some algebra to ﬁ nd the coefﬁ cients. (Answer: a 0  5 0.0047,
a 1  5 0.0095, a 2  5 0.0047, b 1  5 21.8485, b 2  5 0.8673.)
    6.5    Effect of Poles or Zeros at Inﬁ nity
 In the analog transfer function ( Equation 6.19 ) of the previous example, you can see that
there is an analog pole at  s  5 21 since that would make the transfer function go to inﬁ nity,
and there is a zero at  s  5 ` because that will cause  H ( s ) to become 0.0. There is also a zero
at  s  5 2`. Interestingly, these two inﬁ nity values are in the same location because the reality
is that the s and  j v axes actually wrap around an inﬁ nitely large sphere and touch each other
at 6`. So, in this ﬁ rst-order case engineers only show the single zero at inﬁ nity and they
choose to use the one at 2` so this transfer function’s pole and zero would be plotted like
 Figure 6.16  in the  s -plane. For low-pass zeros at inﬁ nity, the bilinear transform maps the zero
at inﬁ nity to  z  5 21 (Nyquist) ( Figure 6.17 ).

Audio Filter Designs: IIR Filters  179
j ω
–1
σ
 Figure 6.16:    The pole at  s  5 21 and the zero at  s  5 6`
plotted in the  s -plane.
lm
jω
Re
–1
σ
 Figure 6.17:    The bilinear transform maps real zeros at inﬁ nity to the Nyquist
frequency in the  z -plane.
 Next consider the second-order analog low-pass ﬁ lter transfer function:
                                                       H(s) 5
which  factors  as
                                                       H(s) 5
1
s2 1 (1/Q)s 1 1
1
(s 2 P)(s 2 P*)
(6.25)
 This transfer function has a pair of conjugate poles at locations  P  and  P  *  or ( a  1  bj v) and
( a  2  bj v) as well as a pair of zeros at 6`. The bilinear transform maps the poles on the

180  Chapter 6
left side of the  s -plane to locations inside the unit circle. Once again, it maps the zeros at 6`
to  z  5 21 or the Nyquist frequency ( Figures 6.18  and  6.19 ).
 There are several methods of overcoming this problem (Orfanidis 1997 ; Massberg 2011).
The Massberg approach is discussed further in  Section 6.9 . Even with the error in the BZT,
it is still an excellent tool for converting existing designs. You will notice that in the design
methods, we start with the desired cutoff frequency (and  Q , where applicable) and calculate
the coefﬁ cients.
Im
j ω
+bj ω
Re
(cid:68)
σ
– bj ω
 Figure 6.18:    The bilinear transform maps imaginary zeros at inﬁ nity
to Nyquist in the  z -plane.
Amplitude
1. 0
Analog  Filter
0.0
1/ 4  Nyquist
fs/8
½  Nyquist
fs/4
Nyquist
fs/2
Digital  Filter
Heading to
zero at infinity
Frequency
 Figure 6.19 :    The zero at inﬁ nity causes an error in the upper part of the frequency.
The error is worst at the Nyquist frequency; the analog error is exaggerated for
the purposes of illustration.

Audio Filter Designs: IIR Filters  181
   6.6    Generic Bi-Quad Designs
 The following classical analog ﬁ lters are converted to digital and implemented as bi-quad
topologies:
(cid:129)
(cid:129)
(cid:129)
(cid:129)
(cid:129)
(cid:129)
(cid:129)
   LPF (low-pass ﬁ lter)
   HPF (high-pass ﬁ lter)
   BPF (band-pass ﬁ lter)
   BSF (band-stop ﬁ lter)
   Second-order Butterworth LPF and HPF
   Second-order Linkwitz2Riley LPF and HPF (good for crossovers)
   First- and second-order all-pass ﬁ lters (APF)
 Low-pass and high-pass ﬁ lters are characterized by their corner frequency  f  c  and (for second-order
and higher) their  Q  or resonant peaking value. A  Q  of 0.707 is the highest value  Q  can assume
before peaking occurs. It is called the Butterworth or maximally ﬂ at response. With a  Q  of 0.707
the 23 dB point of the ﬁ lter will be exactly at  f  c . For these designs, once the  Q  rises above 0.707,
it will correspond to the peak frequency and not the 23 dB frequency.  Equations 6.26  through
 6.29  relate the  Q , peak frequency, 23 dB frequency, and the peak magnitude values.
  f23dB 5 fcÇ
a
      fc 5
1
2Q2 2 1b 1 Å
f23dB
2
a
1
2Q2 2 1b
1 1
2
a
1
2Q2 2 1b
1 1
Ç
a
1
2Q2 2 1b 1 Å
Q2
"Q2 2 0.25
   Peak gain  5
 1 Q  .  0.707 only2
  PeakdB 5 20log 1 peak gain 2
(6.26)
(6.27)
(6.28)
(6.29)
 Band-pass, band-stop, graphic, and parametric EQs are speciﬁ ed by their center frequency  f  c
and  Q  or bandwidth (Equation 6.30). Depending on the ﬁ lter, the arithmetic (Equation 6.31)
or geometric (Equation 6.32) mean is used to relate fc and the band edges fU and fL.
  Q 5
fc
BW
  Arithmetic mean 5 fc 5
fU 2 fL
2
Geometric mean 5 fc 5 "fU fL
(6.30)
(6.31)
(6.32)

182  Chapter 6
x(n)
a0
(cid:153)
y(n)
z –1
z –1
a1
a2
z -1
z -1
–b 1
–b
2
 Figure 6.20:    Generic bi-quad structure.
 The block diagram is shown in  Figure 6.20 .
 The difference equation is as follows:
  y(n) 5 a0x(n) 1 a1x(n 2 1) 1 a2x(n 2 2) 2 b1y(n 2 1) 2 b2y(n 2 2)
(6.33)
  6.6.1  First-Order LPF and HPF
 Specify:
(cid:129)
    f  c , corner frequency; see Figure 6.21 for examples
 The design equations are as follows:
     LPF
uc 5 2pfc/fs
     HPF
uc 5 2pfc/fs
 g 5
a0 5
 cos uc
1 1 sin uc
1 2 g
2
1 2 g
2
a2 5 0.0
b1 5 2g
b2 5 0.0
a1 5
 g 5
a0 5
 cos uc
1 1 sin uc
1 1 g
2
1 1 g
2
b
a1 5 2a
a2 5 0.0
b1 5 2g
b2 5 0.0
(6.34)

+12.0  dB
0.0  dB
–12.0 dB
–24.0 dB
–36.0 dB
–48.0 dB
–60.0 dB
Audio Filter Designs: IIR Filters  183
fc =  1  kHz
fc = 100  Hz
10 Hz
100 Hz
1 kHz
10 kHz
 Figure 6.21 :    First-order LPF with  f  c   = 100 Hz, 250 Hz, 500 Hz, and 1 kHz.
   6.6.2  Second-Order LPF and HPF
 Specify:
(cid:129)
(cid:129)
    f  c , corner frequency
    Q , quality factor controlling resonant peaking; Figure 6.22 shows the effect of Q and
peaking
 The design equations are as follows:
         LPF
uc 5 2pfc / fs
1
Q
d 5
1 2
b 5 0.5
           HPF
uc 5 2pfc / fs
1
Q
 d 5
1 2
 b 5 0.5
 sin uc
d
2
d
2
1 1
 sin uc
g 5 1 0.5 1 b2 cos uc
0.5 1 b 2 g
2.0
a0 5
a1 5 0.5 1 b 2 g
0.5 1 b 2 g
2.0
a2 5
b1 5 22g
b2 5 2b
 sin uc
d
2
d
2
1 1
 sin uc
 g 5 1 0.5 1 b2 cos uc
0.5 1 b 1 g
a0 5
2.0
a1 5 21 0.5 1 b 1 g 2
0.5 1 b 1 g
2.0
a2 5
b1 5 22g
b2 5 2b
(6.35)

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
