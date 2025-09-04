# Audio Plugins C++ (Pirkle) — Chunk 26/73

> Source: `Designing Audio Effect Plug-Ins in C++_ With Digital Audio Signal Processing Theory - PDF Room.pdf` · Pages ~1-560 · Extractor: pdfminer.six
> Target ≈ 2800 words, Overlap = 200 words · Actual = 2565

> Note: DSP textbook content for SpectralCanvasPro RT-audio knowledge base

---
168  Chapter 6
Im
+12.0 dB
0.0  dB
–12.0  dB
Re
–24.0  dB
–36.0  dB
–48.0  dB
–60.0  dB
10Hz
100 Hz
1kHz
10 kHz
 Figure 6.6:    The asymmetrical response shape when the pole is in the ﬁ rst quadrant; notice
the difference in gain at DC versus Nyquist. This ﬁ lter has  f  s   = 44.1 kHz,
 f  c  = p/4 = 5.5125 kHz, and  Q  = 10.
Im
+12.0 dB
0.0 dB
–12.0 dB
Re
–24.0 dB
–36.0 dB
–48.0 dB
–60.0 dB
10Hz
100 Hz
1kHz
10kHz
 Figure 6.7:    The asymmetrical response shape when the pole is in the second quadrant; notice the
difference in gain at DC versus Nyquist. This ﬁ lter has  f  s  = 44.1 kHz,
 f  c  = 3p/4 =16.5375 kHz, and  Q  = 10.
low end is boosted ( Figure 6.6 ). The opposite happens ( Figure 6.7 ) when the pole moves to
the second quadrant and the high frequencies get the boost. The solution to the asymmetry
problem is a really useful ﬁ lter, especially for making ﬁ lter banks of extremely narrow band-
pass ﬁ lters.
   6.3.2  Smith-Angell Improved Resonator
 A block diagram of a Smith-Angell improved resonator is shown in  Figure 6.8 .
 The difference equation is as follows:
 y(n) 5 a0x(n) 1 a2x(n 2 2) 2 b1y(n 2 1) 2 b2y(n 2 2)
(6.7)

Audio Filter Designs: IIR Filters  169
x(n)
a0
y(n)
-1 z
-1 z
a2
z-1
-1
Z
-b1
-b2
 Figure 6.8:    Block diagram for the Smith-Angell
improved resonator.
 Specify:
(cid:129)
(cid:129)
    f  c , center frequency
    BW , 3 dB bandwidth; or  Q , quality factor
 The design equations are as follows:
   uc 5 2pfc/fs
   BW 5 fc/Q
   b2 5 ea22pBW
fs
24b2
1 1 b2
   b1 5
b
 cos(uc)
(6.8)
   a0 5 1 2 "b2
   a2 5 2a0
 This design is also gain normalized with a 0  and as before, the radius is set with b 2
ﬁ rst, then b 1  is calculated using the b 2  coefﬁ cient and the desired pole frequency. The
ﬁ lter is not truly normalized to 0.0 dB; there is actually a slight ﬂ uctuation of less
than 1 dB.
 The improved resonator is also named the Smith-Angell resonator, after its inventors. They
improved the design by dropping two zeros into the ﬁ lter, one at  z  5 1 and one at  z  5 21 in
order to pin down DC and Nyquist with a response of zero. This forces the ﬁ lter to become
somewhat more symmetric (but not entirely) and has the advantage of making the band pass
even more selective in nature ( Figure 6.9 ).

170  Chapter 6
Im
+12.0  dB
0.0  dB
–12.0  dB
Re
–24.0  dB
–36.0  dB
–48.0  dB
–60.0  dB
10Hz
100Hz
1kHz
10 kHz
 Figure 6.9:    The effect on the resonator shape with the added zeros to hold down DC and
Nyquist. This ﬁ lter has  f  s  = 44.1 kHz,  f  c  = p/4 = 5.5125 kHz, and  Q  = 20
(notice how close the poles are to the unit circle).
    6.4    Analog Filter to Digital Filter Conversion
 A more widely used approach to ﬁ lter design is to ﬁ rst start with the classic analog designs
and then convert them into digital versions. There’s good reason to do this because there
are many excellent analog designs already done for us. We just need a way to make them
work in our digital world. While analog ﬁ lter design is outside the scope of the book,
there are many similarities between the two design worlds. For example, both analog
and digital ﬁ lter design involve a transfer function that you manipulate to produce poles
and zeros. They also both use a transform to get from the time domain to the frequency
domain. A fundamental difference is that in analog, there is no Nyquist restriction and all
frequencies from 2` to 1` are included. Also, in analog, reactive components or circuits
like inductors, capacitors, and gyrators (a circuit that simulates an inductor) are used to
create the phase shift that is at the heart of the ﬁ ltering operation.  Table 6.1  summarizes the
similarities and differences.
 Table 6.1:    The differences between analog and digital ﬁ lter design technique.
Digital
Analog
(cid:129)  Uses a transfer function to relate I/O
(cid:129)  Delay elements create phase shift
(cid:129)  Uses the  z transform (sampled time to
(cid:129)  Uses a transfer function to relate I/O
(cid:129)  Reactive components create phase shift
(cid:129)  Uses the Laplace transform (continuous
frequency)
time to frequency)
(cid:129)  Poles and zeros in the z-plane
(cid:129)  Nyquist limitation
(cid:129)  Poles must be inside the unit circle for
(cid:129)  Poles and zeros in the s-plane
(cid:129)  All frequencies from 2` to 1` allowed
(cid:129)  Poles must be in the left-hand part of the
stable operation
s-plane for stable operation
½ Nyquist
lm
Nyquist
0 Hz
Re
–½ Nyquist
Audio Filter Designs: IIR Filters  171
+ oo
j ω
– oo
Nyquist
½ Nyquist
0 Hz
σ
–½  Nyquist
–Nyquist
 Figure 6.10:    Comparison of the unit circle in the  z -plane with the  s -plane.
 In  Figure 6.10  you can see the  s -plane on the right—it is also a complex plane. The
real axis is named the s axis and the imaginary axis is the  j v axis. The  j v axis is the
frequency axis and it spans 2` to 1` rad/sec. The unit circle maps to the portion on the
 j v axis between 2Nyquist and 1Nyquist. In order to transform an analog ﬁ lter design
into a digital ﬁ lter design, we need a mapping system to get the poles and zeros from
the  s -plane to become poles and zeros in the  z -plane. Once we have that, the coefﬁ cients
that produce those poles and zeros in the digital locations can be calculated. In the
analog world, poles that are close to the  j v axis will result in poles that are close to the
unit circle in the digital world. The problem is that poles and zeros can exist anywhere
along the  j v axis, even at frequencies outside the digital Nyquist zone. It is common
for analog designs to have a pole or a zero at 2` and/or 1` on either the s  and/or
 j v axes.
 In  Figure 6.11  you can see the problem: the pair of zeros close to the  j v axis at ½ Nyquist
will map to locations close to the unit circle at the ½ Nyquist angles. But what about the
pair of poles that are outside the Nyquist frequency in the  s -plane? The other way to think
about the issue is that in the analog  s -plane, the entire left-hand side, including all the inﬁ nite
frequencies, must map to the  interior  of the unit circle in the  z -plane. Also, the entire right-
hand plane must map to the  exterior  of the unit circle, as shown in  Figure 6.12 .
 So, what we need is a mathematical device to make the transformation from the  s -plane to
the  z -plane. It must map an inﬁ nitely large area into the unit circle. Fortunately, many of
these mathematical transforms already exist. The one best suited for us is called the bilinear
transform, and we can use it to transform an analog transfer function,  H ( s ) into a digital
one,  H ( z ).
172  Chapter 6
½ Nyquist
lm
Nyquist
Re
–½
Nyquist
+  oo
j ω
– oo
Nyquist
½ Nyquist
σ
–½  Nyquist
–Nyquist
 Figure 6.11:    Mapping the poles and zeros from the analog  s -plane to the digital  z -plane.
½ Nyquist
lm
Nyquist
0 Hz
Re
–½  Nyquist
lm
Nyquist
½ Nyquist
0 Hz
σ
–Nyquist
-Nyquist
+  oo
j ω
– oo
+  oo
j ω
Re
σ
 Figure 6.12:    Mapping the inﬁ nitely large left-hand plane into the ﬁ nite space inside the unit
circle and the right-hand plane into the exterior of the unit circle.
– oo
Audio Filter Designs: IIR Filters  173
 We wish to convert an analog transfer function  H ( s ) into a sampled equivalent  H ( z ).
Effectively, this means we need a way to sample the continuous  s -plane to produce the
sampled  z -plane version. In other words, we need to create a sampled analog transfer function
 H  s ( s ) where the subscript s stands for “sampled.” We seek to ﬁ nd a function  g ( z ) such that
 Equation 6.9  holds true.
s5g(z)
  H(s) h
H(z)
(6.9)
 Since the sample interval is  T , then the term  e j   v T   would correspond to one sample in time.
So, if we evaluate  H ( z ) by letting  z  5  e j   v T   then we will arrive at the sampled analog transfer
function  H  s ( s ) in  Equation 6.10 :
s5g(z)
  H(s) h
z5ejvT
H(z) h
Hs(s)
(6.10)
 To solve, we note that  s  5  j v (the analog deﬁ nition of s) and  z  5  e j   v T   to get  Equation 6.11 :
z 5 esT
                                                              ln(z) 5  ln(esT)
                                                              ln(z) 5 sT
or
sT 5  ln(z)
(6.11)
 We now have the relationship between  s  and  z , but we need to deal with taking the natural log
of  z , which doesn’t have a closed form solution. If we use the Taylor series expansion for ln(),
we get  Equation 6.12  for Re( z ) > 0:
                   sT 5 2 c
z 2 1
z 1 1
Taking  the  first  term  only
z 2 1
z 1 1
                  s 5
2
T
1
1
3
a
z 2 1
z 1 1
3
b
2
1
5
a
z 2 1
z 1 1
5
b
1
1
7
a
z 2 1
z 1 1
7
b
2 ...d
(6.12)
 This ﬁ rst term  approximation  of the general solution is the bilinear transform. The bilinear
transform we use is  Equation 6.13 :
 s5
z 2 1
z 1 1
(6.13)
 The 2/ T  term washes out mathematically, so we can neglect it. This equation does the
mapping by taking values at the inﬁ nite frequencies and mapping them to Nyquist. So, a pole
or zero at 2` or 1` is mapped to Nyquist. The other frequencies between Nyquist and 2`
or 1` Hz are squeezed into the little region right around Nyquist, just inside the unit circle,
as shown in  Figure 6.13 .

174  Chapter 6
½ Nyquist
lm
Nyquist
Re
–½ Nyquist
+ oo
jω
– oo
Nyquist
½ Nyquist
σ
–½ Nyquist
–Nyquist
 Figure 6.13:    The bilinear transform maps the area outsize the Nyquist zone on the left-hand
plane to an area just inside the circle near Nyquist.
 The bilinear transform maps analog frequencies to their corresponding digital frequencies
nonlinearly via the tan() function ( Equation 6.14 ):
where
va 5  tan c
vd/fs
2
d
va 5  the  analog  frequency
vd 5  the  mapped  digital  frequency
fs  5  the  sample  rate
(6.14)
 The tan() function is linear at low values of v but becomes more nonlinear as the frequency
increases. At low frequencies, the analog and digital frequencies map closely. At high
frequencies, the digital frequencies become warped and do not map properly to the analog
counterparts. This means that a given analog design with a cutoff frequency  f  c  might have the
wrong digital cutoff frequency after the conversion.
 The solution is to pre-warp the  analog  ﬁ lter so that its cutoff frequency is in the wrong
location in the analog domain, but will wind up in the correct location in the digital
domain. To pre-warp the analog ﬁ lter, you just use the same equation ( Equation 6.14 )
applied to the cutoff frequency of the analog ﬁ lter. When you combine all the operations
you get the bilinear  Z  transform, or BZT. The step-by-step method of conversion is as
follows (Ifeachor 1993 ):

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
